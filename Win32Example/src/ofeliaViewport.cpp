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

#include "ofeliaViewport.h"
#include "ofeliaWindow.h"

/* ________________________________________________________________________________
 * static variables
 */
const char *t_ofeliaViewport::objName = "ofViewport";

/* ________________________________________________________________________________
 * ofViewport object methods
 */
bool getViewportElemFromArgs(const int argc, const t_atom *argv, t_ofeliaViewportElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.x = 0.0f;
            elem.y = 0.0f;
            elem.width = OFELIA_DEFAULT_SIZE;
            elem.height = OFELIA_DEFAULT_SIZE;
            elem.VFlip = true;
            break;
        }
        case 4:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT) {
                
                elem.x = argv[0].a_w.w_float;
                elem.y = argv[1].a_w.w_float;
                elem.width = argv[2].a_w.w_float;
                elem.height = argv[3].a_w.w_float;
                elem.VFlip = true;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaViewport::objName);
                return 0;
            }
            break;
        }
        case 5:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT &&
                argv[4].a_type == A_FLOAT) {
                
                elem.x = argv[0].a_w.w_float;
                elem.y = argv[1].a_w.w_float;
                elem.width = argv[2].a_w.w_float;
                elem.height = argv[3].a_w.w_float;
                elem.VFlip = argv[4].a_w.w_float != 0.0f;
            }
            else if (argv[0].a_type == A_FLOAT &&
                     argv[1].a_type == A_FLOAT &&
                     argv[2].a_type == A_FLOAT &&
                     argv[3].a_type == A_FLOAT &&
                     argv[4].a_type == A_SYMBOL) {
                
                elem.x = argv[0].a_w.w_float;
                elem.y = argv[1].a_w.w_float;
                elem.width = argv[2].a_w.w_float;
                elem.height = argv[3].a_w.w_float;
                getToggleFromSym(argv[4].a_w.w_symbol, elem.VFlip, t_ofeliaViewport::objName);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaViewport::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaViewport::objName);
            return 0;
        }
    }
    if (elem.width < 0.0f) {
        
        error("%s: width out of range", t_ofeliaViewport::objName);
        return 0;
    }
    if (elem.height < 0.0f) {
        
        error("%s: height out of range", t_ofeliaViewport::objName);
        return 0;
    }
    return 1;
}


void *ofeliaViewport_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaViewport *x = reinterpret_cast<t_ofeliaViewport*>(pd_new(ofeliaViewport_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getViewportElemFromArgs(argc, argv, x->elem))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("x"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("y"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("width"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("height"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaViewport_bang(t_ofeliaViewport *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const float scaleAmt = ofeliaWindow::scaleFactor;
        ofVec2f fsOffset;
        
        if (ofeliaWindow::bFullscreenMode)
            fsOffset = ofeliaWindow::fullscreenOffset;
        ofGetCurrentRenderer()->pushView();
        ofGetCurrentRenderer()->viewport(x->elem.x * scaleAmt + fsOffset.x,
                                         x->elem.y * scaleAmt + fsOffset.y,
                                         x->elem.width * scaleAmt,
                                         x->elem.height * scaleAmt,
                                         x->elem.VFlip);
        ofGetCurrentRenderer()->setupScreen();
        ofGetCurrentRenderer()->pushMatrix();
        ofGetCurrentRenderer()->scale(scaleAmt, scaleAmt, scaleAmt);
        outlet_bang(x->x_obj.ob_outlet);
        ofGetCurrentRenderer()->popMatrix();
        ofGetCurrentRenderer()->popView();
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaViewport::objName);
    }
}

void ofeliaViewport_x(t_ofeliaViewport *x, t_floatarg f)
{
    x->elem.x = f;
}

void ofeliaViewport_y(t_ofeliaViewport *x, t_floatarg f)
{
    x->elem.y = f;
}

void ofeliaViewport_width(t_ofeliaViewport *x, t_floatarg f)
{
    if (f >= 0.0f)
        x->elem.width = f;
    else
        error("%s: width out of range", t_ofeliaViewport::objName);
}

void ofeliaViewport_height(t_ofeliaViewport *x, t_floatarg f)
{
    if (f >= 0.0f)
        x->elem.height = f;
    else
        error("%s: height out of range", t_ofeliaViewport::objName);
}

void ofeliaViewport_VFlip(t_ofeliaViewport *x, t_symbol *s, int argc, t_atom *argv)
{
    getToggleFromArgs(argc, argv, x->elem.VFlip, t_ofeliaViewport::objName);
}

void ofeliaViewport_set(t_ofeliaViewport *x, t_symbol *s, int argc, t_atom *argv)
{
    getViewportElemFromArgs(argc, argv, x->elem);
}

void ofeliaViewport_print(t_ofeliaViewport *x)
{
    post("\n[%s]", t_ofeliaViewport::objName);
    post("x : %g", x->elem.x);
    post("y : %g", x->elem.y);
    post("width : %g", x->elem.width);
    post("height : %g", x->elem.height);
    printToggle("VFlip", x->elem.VFlip);
}

void ofeliaViewport_setup()
{
    ofeliaViewport_class = class_new(gensym("ofViewport"),
                                     reinterpret_cast<t_newmethod>(ofeliaViewport_new),
                                     0, sizeof(t_ofeliaViewport),
                                     CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaViewport_class, reinterpret_cast<t_method>(ofeliaViewport_bang));
    class_addmethod(ofeliaViewport_class, reinterpret_cast<t_method>(ofeliaViewport_x),
                    gensym("x"), A_FLOAT, 0);
    class_addmethod(ofeliaViewport_class, reinterpret_cast<t_method>(ofeliaViewport_y),
                    gensym("y"), A_FLOAT, 0);
    class_addmethod(ofeliaViewport_class, reinterpret_cast<t_method>(ofeliaViewport_width),
                    gensym("width"), A_FLOAT, 0);
    class_addmethod(ofeliaViewport_class, reinterpret_cast<t_method>(ofeliaViewport_height),
                    gensym("height"), A_FLOAT, 0);
    class_addmethod(ofeliaViewport_class, reinterpret_cast<t_method>(ofeliaViewport_VFlip),
                    gensym("VFlip"), A_GIMME, 0);
    class_addmethod(ofeliaViewport_class, reinterpret_cast<t_method>(ofeliaViewport_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaViewport_class, reinterpret_cast<t_method>(ofeliaViewport_print),
                    gensym("print"), A_NULL, 0);
}

