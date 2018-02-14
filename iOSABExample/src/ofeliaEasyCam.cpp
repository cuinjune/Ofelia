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

#include "ofeliaEasyCam.h"
#include "ofeliaWindow.h"

/* ________________________________________________________________________________
 * static variables
 */
const char *t_ofeliaEasyCam::objName = "ofEasyCam";

/* ________________________________________________________________________________
 * ofEasyCam object methods
 */
void *ofeliaEasyCam_new()
{
    t_ofeliaEasyCam *x = reinterpret_cast<t_ofeliaEasyCam*>(pd_new(ofeliaEasyCam_class));
    x->VFlip = true;
    x->debug = false;
    x->shouldUpdateCamera = true;
    x->bInitGate = false;
    x->easyCam = make_unique<ofEasyCam>();
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaEasyCam_init(t_ofeliaEasyCam *x)
{
    if (!x->bInitGate)
        return;
    x->shouldUpdateCamera = true;
}

void ofeliaEasyCam_exit(t_ofeliaEasyCam *x)
{
    x->bInitGate = true;
}

void ofeliaEasyCam_bang(t_ofeliaEasyCam *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (x->shouldUpdateCamera) {
            
            x->easyCam->setVFlip(x->VFlip);
            x->shouldUpdateCamera = false;
        }
        if (x->debug) {
            
            x->easyCam->draw();
            outlet_bang(x->x_obj.ob_outlet);
        }
        else {
            
            x->easyCam->begin();
            outlet_bang(x->x_obj.ob_outlet);
            x->easyCam->end();
        }
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaEasyCam::objName);
    }
}

void ofeliaEasyCam_VFlip(t_ofeliaEasyCam *x, t_symbol *s, int argc, t_atom *argv)
{
    getToggleFromArgs(argc, argv, x->VFlip, t_ofeliaEasyCam::objName);
    x->shouldUpdateCamera = true;
}

void ofeliaEasyCam_debug(t_ofeliaEasyCam *x, t_symbol *s, int argc, t_atom *argv)
{
    getToggleFromArgs(argc, argv, x->debug, t_ofeliaEasyCam::objName);
}

void ofeliaEasyCam_print(t_ofeliaEasyCam *x)
{
    post("\n[%s]", t_ofeliaEasyCam::objName);
    printToggle("VFlip", x->VFlip);
    printToggle("debug", x->debug);
}

void ofeliaEasyCam_free(t_ofeliaEasyCam *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    x->easyCam.reset();
}

void ofeliaEasyCam_setup()
{
    ofeliaEasyCam_class = class_new(gensym("ofEasyCam"),
                                    reinterpret_cast<t_newmethod>(ofeliaEasyCam_new),
                                    reinterpret_cast<t_method>(ofeliaEasyCam_free),
                                    sizeof(t_ofeliaEasyCam),
                                    CLASS_DEFAULT, A_NULL, 0);
    class_addmethod(ofeliaEasyCam_class, reinterpret_cast<t_method>(ofeliaEasyCam_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaEasyCam_class, reinterpret_cast<t_method>(ofeliaEasyCam_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addbang(ofeliaEasyCam_class, reinterpret_cast<t_method>(ofeliaEasyCam_bang));
    class_addmethod(ofeliaEasyCam_class, reinterpret_cast<t_method>(ofeliaEasyCam_VFlip),
                    gensym("VFlip"), A_GIMME, 0);
    class_addmethod(ofeliaEasyCam_class, reinterpret_cast<t_method>(ofeliaEasyCam_debug),
                    gensym("debug"), A_GIMME, 0);
    class_addmethod(ofeliaEasyCam_class, reinterpret_cast<t_method>(ofeliaEasyCam_print),
                    gensym("print"), A_NULL, 0);
}

