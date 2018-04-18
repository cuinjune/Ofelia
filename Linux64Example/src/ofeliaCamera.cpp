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

#include "ofeliaCamera.h"
#include "ofeliaWindow.h"

/* ________________________________________________________________________________
 * static variables
 */
const char *t_ofeliaCamera::objName = "ofCamera";

/* ________________________________________________________________________________
 * ofCamera object methods
 */
void *ofeliaCamera_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaCamera *x = reinterpret_cast<t_ofeliaCamera*>(pd_new(ofeliaCamera_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getVec3fFromArgs(argc, argv, x->pos, t_ofeliaCamera::objName))
        return 0;
    x->eulerAngles.set(0.0f, 0.0f, 0.0f);
    x->quaternion.set(0.0f, 0.0f, 0.0f, 1.0f);
    x->rotate.angle = 0.0f;
    x->rotate.axis.set(0.0f, 0.0f, 0.0f);
    x->lookAt.set(0.0f, 0.0f, 0.0f);
    x->orienType = CAMERA_ORIEN_TYPE_ROTATE;
    x->fov = 60.0f;
    x->nearClip = 0.0f;
    x->farClip = 0.0f;
    x->aspectRatio = 0.0f;
    x->lensOffset.set(0.0f, 0.0f);
    x->VFlip = true;
    x->debug = false;
    x->shouldUpdateCamera = true;
    x->bInitGate = false;
    x->camera = make_unique<ofCamera>();
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("x"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("y"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("z"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaCamera_init(t_ofeliaCamera *x)
{
    if (!x->bInitGate)
        return;
    x->shouldUpdateCamera = true;
}

void ofeliaCamera_exit(t_ofeliaCamera *x)
{
    x->bInitGate = true;
}

void ofeliaCamera_bang(t_ofeliaCamera *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (x->shouldUpdateCamera) {
            
            x->camera->setFov(x->fov);
            x->camera->setNearClip(x->nearClip);
            x->camera->setFarClip(x->farClip);
            
            if (x->aspectRatio) {
                
                x->camera->setAspectRatio(x->aspectRatio);
                x->camera->setForceAspectRatio(true);
            }
            else {
                
                x->camera->setForceAspectRatio(false);
            }
            x->camera->setLensOffset(x->lensOffset);
            x->camera->setVFlip(x->VFlip);
            x->shouldUpdateCamera = false;
        }
        x->camera->setPosition(x->pos);
        
        switch (x->orienType) {
                
            case CAMERA_ORIEN_TYPE_ROTATE:
                x->camera->setOrientation(ofQuaternion(x->rotate.angle, x->rotate.axis));
                break;
            case CAMERA_ORIEN_TYPE_EULERANGLES:
                x->camera->setOrientation(x->eulerAngles);
                break;
            case CAMERA_ORIEN_TYPE_QUATERNION:
                x->camera->setOrientation(ofQuaternion(x->quaternion));
                break;
            case CAMERA_ORIEN_TYPE_LOOKAT:
                x->camera->lookAt(x->lookAt);
                break;
            default:
                break;
        }
        if (x->debug) {
            
            x->camera->draw();
            outlet_bang(x->x_obj.ob_outlet);
        }
        else {
            
            x->camera->begin();
            outlet_bang(x->x_obj.ob_outlet);
            x->camera->end();
        }
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaCamera::objName);
    }
}

void ofeliaCamera_x(t_ofeliaCamera *x, t_floatarg f)
{
    x->pos.x = f;
}

void ofeliaCamera_y(t_ofeliaCamera *x, t_floatarg f)
{
    x->pos.y = f;
}

void ofeliaCamera_z(t_ofeliaCamera *x, t_floatarg f)
{
    x->pos.z = f;
}

void ofeliaCamera_rotate(t_ofeliaCamera *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc == 4) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT &&
            argv[3].a_type == A_FLOAT) {
            
            x->rotate.angle = argv[0].a_w.w_float;
            x->rotate.axis.set(argv[1].a_w.w_float, argv[2].a_w.w_float, argv[3].a_w.w_float);
            x->orienType = CAMERA_ORIEN_TYPE_ROTATE;
        }
        else
            error("%s: wrong argument type", t_ofeliaCamera::objName);
    }
    else
        error("%s: invalid number of arguments", t_ofeliaCamera::objName);
}

void ofeliaCamera_eulerAngles(t_ofeliaCamera *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc == 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            x->eulerAngles.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
            x->orienType = CAMERA_ORIEN_TYPE_EULERANGLES;
        }
        else
            error("%s: wrong argument type", t_ofeliaCamera::objName);
    }
    else
        error("%s: invalid number of arguments", t_ofeliaCamera::objName);
}

void ofeliaCamera_quaternion(t_ofeliaCamera *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc == 4) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT &&
            argv[3].a_type == A_FLOAT) {
            
            x->quaternion.set(argv[0].a_w.w_float,
                              argv[1].a_w.w_float,
                              argv[2].a_w.w_float,
                              argv[3].a_w.w_float);
            x->orienType = CAMERA_ORIEN_TYPE_QUATERNION;
        }
        else
            error("%s: wrong argument type", t_ofeliaCamera::objName);
    }
    else
        error("%s: invalid number of arguments", t_ofeliaCamera::objName);
}

void ofeliaCamera_lookAt(t_ofeliaCamera *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc == 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            x->lookAt.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
            x->orienType = CAMERA_ORIEN_TYPE_LOOKAT;
        }
        else
            error("%s: wrong argument type", t_ofeliaCamera::objName);
    }
    else
        error("%s: invalid number of arguments", t_ofeliaCamera::objName);
}

void ofeliaCamera_fov(t_ofeliaCamera *x, t_floatarg f)
{
    x->fov = f;
    x->shouldUpdateCamera = true;
}

void ofeliaCamera_nearClip(t_ofeliaCamera *x, t_floatarg f)
{
    x->nearClip = f;
    x->shouldUpdateCamera = true;
}

void ofeliaCamera_farClip(t_ofeliaCamera *x, t_floatarg f)
{
    x->farClip = f;
    x->shouldUpdateCamera = true;
}

void ofeliaCamera_aspectRatio(t_ofeliaCamera *x, t_floatarg f)
{
    x->aspectRatio = f;
    x->shouldUpdateCamera = true;
}

void ofeliaCamera_lensOffset(t_ofeliaCamera *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc == 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            x->lensOffset.set(argv[0].a_w.w_float, argv[1].a_w.w_float);
            x->shouldUpdateCamera = true;
        }
        else
            error("%s: wrong argument type", t_ofeliaCamera::objName);
    }
    else
        error("%s: invalid number of arguments", t_ofeliaCamera::objName);
}

void ofeliaCamera_VFlip(t_ofeliaCamera *x, t_symbol *s, int argc, t_atom *argv)
{
    getToggleFromArgs(argc, argv, x->VFlip, t_ofeliaCamera::objName);
    x->shouldUpdateCamera = true;
}

void ofeliaCamera_debug(t_ofeliaCamera *x, t_symbol *s, int argc, t_atom *argv)
{
    getToggleFromArgs(argc, argv, x->debug, t_ofeliaCamera::objName);
}

void ofeliaCamera_set(t_ofeliaCamera *x, t_symbol *s, int argc, t_atom *argv)
{
    getVec3fFromArgs(argc, argv, x->pos, t_ofeliaCamera::objName);
}

void ofeliaCamera_print(t_ofeliaCamera *x)
{
    post("\n[%s]", t_ofeliaCamera::objName);
    post("x : %g", x->pos.x);
    post("y : %g", x->pos.y);
    post("z : %g", x->pos.z);
    post("rotate : %g %g %g %g", x->rotate.angle,
         x->rotate.axis.x, x->rotate.axis.y, x->rotate.axis.z);
    post("eulerAngles : %g %g %g", x->eulerAngles.x, x->eulerAngles.y, x->eulerAngles.z);
    post("quaternion : %g %g %g %g", x->quaternion.x, x->quaternion.y, x->quaternion.z, x->quaternion.w);
    post("lookAt : %g %g %g", x->lookAt.x, x->lookAt.y, x->lookAt.z);
    post("fov : %g", x->fov);
    post("nearClip : %g", x->nearClip);
    post("farClip : %g", x->farClip);
    post("aspectRatio : %g", x->aspectRatio);
    post("lensOffset : %g %g", x->lensOffset.x, x->lensOffset.y);
    printToggle("VFlip", x->VFlip);
    printToggle("debug", x->debug);
}

void ofeliaCamera_free(t_ofeliaCamera *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    x->camera.reset();
}

void ofeliaCamera_setup()
{
    ofeliaCamera_class = class_new(gensym("ofCamera"),
                                   reinterpret_cast<t_newmethod>(ofeliaCamera_new),
                                   reinterpret_cast<t_method>(ofeliaCamera_free),
                                   sizeof(t_ofeliaCamera),
                                   CLASS_DEFAULT, A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaCamera_new),
                     gensym("ofCam"), A_GIMME, 0);
    class_addmethod(ofeliaCamera_class, reinterpret_cast<t_method>(ofeliaCamera_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaCamera_class, reinterpret_cast<t_method>(ofeliaCamera_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addbang(ofeliaCamera_class, reinterpret_cast<t_method>(ofeliaCamera_bang));
    class_addmethod(ofeliaCamera_class, reinterpret_cast<t_method>(ofeliaCamera_x),
                    gensym("x"), A_FLOAT, 0);
    class_addmethod(ofeliaCamera_class, reinterpret_cast<t_method>(ofeliaCamera_y),
                    gensym("y"), A_FLOAT, 0);
    class_addmethod(ofeliaCamera_class, reinterpret_cast<t_method>(ofeliaCamera_z),
                    gensym("z"), A_FLOAT, 0);
    class_addmethod(ofeliaCamera_class, reinterpret_cast<t_method>(ofeliaCamera_rotate),
                    gensym("rotate"), A_GIMME, 0);
    class_addmethod(ofeliaCamera_class, reinterpret_cast<t_method>(ofeliaCamera_eulerAngles),
                    gensym("eulerAngles"), A_GIMME, 0);
    class_addmethod(ofeliaCamera_class, reinterpret_cast<t_method>(ofeliaCamera_quaternion),
                    gensym("quaternion"), A_GIMME, 0);
    class_addmethod(ofeliaCamera_class, reinterpret_cast<t_method>(ofeliaCamera_lookAt),
                    gensym("lookAt"), A_GIMME, 0);
    class_addmethod(ofeliaCamera_class, reinterpret_cast<t_method>(ofeliaCamera_fov),
                    gensym("fov"), A_FLOAT, 0);
    class_addmethod(ofeliaCamera_class, reinterpret_cast<t_method>(ofeliaCamera_nearClip),
                    gensym("nearClip"), A_FLOAT, 0);
    class_addmethod(ofeliaCamera_class, reinterpret_cast<t_method>(ofeliaCamera_farClip),
                    gensym("farClip"), A_FLOAT, 0);
    class_addmethod(ofeliaCamera_class, reinterpret_cast<t_method>(ofeliaCamera_aspectRatio),
                    gensym("aspectRatio"), A_FLOAT, 0);
    class_addmethod(ofeliaCamera_class, reinterpret_cast<t_method>(ofeliaCamera_lensOffset),
                    gensym("lensOffset"), A_GIMME, 0);
    class_addmethod(ofeliaCamera_class, reinterpret_cast<t_method>(ofeliaCamera_VFlip),
                    gensym("VFlip"), A_GIMME, 0);
    class_addmethod(ofeliaCamera_class, reinterpret_cast<t_method>(ofeliaCamera_debug),
                    gensym("debug"), A_GIMME, 0);
    class_addmethod(ofeliaCamera_class, reinterpret_cast<t_method>(ofeliaCamera_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaCamera_class, reinterpret_cast<t_method>(ofeliaCamera_print),
                    gensym("print"), A_NULL, 0);
}

