/*==============================================================================
	ofelia: OpenFrameworks as an External Library for Interactive Applications
 
	Copyright (c) 2018 Zack Lee <cuinjune@gmail.com>
 
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.
 
	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.
	
	See https://github.com/cuinjune/ofxOfelia for documentation
 ==============================================================================*/

#include "ofeliaHead.h"
#include "ofeliaWindow.h"
#include "ofeliaStyle.h"

/* ________________________________________________________________________________
 * static variables
 */
const char *t_ofeliaHead::objName = "ofHead";
unsigned int t_ofeliaHead::counter;
vector<pair<t_float, unsigned int>> t_ofeliaHead::vec; /* order, id */

/* ________________________________________________________________________________
 * ofHead object methods
 */
size_t getPositionByHeadObjID(vector<pair<t_float, unsigned int>> &vec, unsigned int objID)
{
    for (size_t i=0; i<vec.size(); ++i) {
        
        if (vec[i].second == objID)
            return i;
    }
    return 0;
}

bool getHeadElemFromArgs(int argc, t_atom *argv, t_ofeliaHeadElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.order = OFELIA_DEFAULT_RENDER_ORDER;
            elem.rendering = true;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                elem.order = argv[0].a_w.w_float;
                elem.rendering = true;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaHead::objName);
                return 0;
            }
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT) {
                
                elem.order = argv[0].a_w.w_float;
                elem.rendering = argv[1].a_w.w_float != 0.0f;
            }
            else if (argv[0].a_type == A_FLOAT &&
                     argv[1].a_type == A_SYMBOL) {
                
                elem.order = argv[0].a_w.w_float;
                const char *mode = argv[1].a_w.w_symbol->s_name;
                
                if (!strcmp(mode, "OFF"))
                    elem.rendering = false;
                else if (!strcmp(mode, "ON"))
                    elem.rendering = true;
                else {
                    
                    error("%s: no method for '%s'", t_ofeliaHead::objName, mode);
                    return 0;
                }
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaHead::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaHead::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaHead_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaHead *x = reinterpret_cast<t_ofeliaHead*>(pd_new(ofeliaHead_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getHeadElemFromArgs(argc, argv, x->elem))
        return 0;
    x->objID = t_ofeliaHead::counter++;
    t_ofeliaHead::vec.push_back(make_pair(x->elem.order, x->objID));
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::drawSym);
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("order"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaHead_list(t_ofeliaHead *x, t_symbol *s, int argc, t_atom *argv)
{
    getToggleFromArgs(argc, argv, x->elem.rendering, t_ofeliaHead::objName);
}

void ofeliaHead_draw(t_ofeliaHead *x, t_floatarg f)
{
    if (x->objID == static_cast<unsigned int>(f) && x->elem.rendering && t_ofeliaWindow::bRenderGate) {
        
        t_ofeliaSetTextMode::textModeVec.push_back(t_ofeliaSetTextMode::textModeVec.back());
        ofGetCurrentRenderer()->pushStyle();
        ofGetCurrentRenderer()->pushMatrix();
        outlet_bang(x->x_obj.ob_outlet);
        ofGetCurrentRenderer()->popMatrix();
        ofGetCurrentRenderer()->popStyle();
        t_ofeliaSetTextMode::textModeVec.pop_back();
    }
}

void ofeliaHead_order(t_ofeliaHead *x, t_floatarg f)
{
    const int pos = getPositionByHeadObjID(t_ofeliaHead::vec, x->objID);
    t_ofeliaHead::vec[pos].first = x->elem.order = f;
}

void ofeliaHead_rendering(t_ofeliaHead *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc == 1) {
        
        if (argv[0].a_type == A_FLOAT) {
            
            x->elem.rendering = argv[0].a_w.w_float != 0.0f;
        }
        else if (argv[0].a_type == A_SYMBOL) {
            
            const char *mode = argv[0].a_w.w_symbol->s_name;
            
            if (!strcmp(mode, "OFF"))
                x->elem.rendering = false;
            else if (!strcmp(mode, "ON"))
                x->elem.rendering = true;
            else
                error("%s: no method for '%s'", t_ofeliaHead::objName, mode);
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaHead::objName);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaHead::objName);
    }
}

void ofeliaHead_set(t_ofeliaHead *x, t_symbol *s, int argc, t_atom *argv)
{
    if (getHeadElemFromArgs(argc, argv, x->elem)) {
        
        const int pos = getPositionByHeadObjID(t_ofeliaHead::vec, x->objID);
        t_ofeliaHead::vec[pos].first = x->elem.order;
    }
}

void ofeliaHead_print(t_ofeliaHead *x)
{
    post("\n[%s]", t_ofeliaHead::objName);
    post("order : %g", x->elem.order);
    printToggle("rendering", x->elem.rendering);
}

void ofeliaHead_free(t_ofeliaHead *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::drawSym);
    const int pos = getPositionByHeadObjID(t_ofeliaHead::vec, x->objID);
    t_ofeliaHead::vec.erase(t_ofeliaHead::vec.begin() + pos);
}

void ofeliaHead_setup()
{
    ofeliaHead_class = class_new(gensym("ofHead"),
                                 reinterpret_cast<t_newmethod>(ofeliaHead_new),
                                 reinterpret_cast<t_method>(ofeliaHead_free),
                                 sizeof(t_ofeliaHead),
                                 CLASS_DEFAULT, A_GIMME, 0);
    class_addlist(ofeliaHead_class, reinterpret_cast<t_method>(ofeliaHead_list));
    class_addmethod(ofeliaHead_class, reinterpret_cast<t_method>(ofeliaHead_draw),
                    t_ofeliaWindow::drawMess, A_FLOAT, 0);
    class_addmethod(ofeliaHead_class, reinterpret_cast<t_method>(ofeliaHead_order),
                    gensym("order"), A_FLOAT, 0);
    class_addmethod(ofeliaHead_class, reinterpret_cast<t_method>(ofeliaHead_rendering),
                    gensym("rendering"), A_GIMME, 0);
    class_addmethod(ofeliaHead_class, reinterpret_cast<t_method>(ofeliaHead_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaHead_class, reinterpret_cast<t_method>(ofeliaHead_print),
                    gensym("print"), A_NULL, 0);
}

