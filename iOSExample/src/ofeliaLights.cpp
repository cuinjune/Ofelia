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

#include "ofeliaLights.h"
#include "ofeliaWindow.h"

/* ________________________________________________________________________________
 * static variables
 */
const char *t_ofeliaPointLight::objName = "ofPointLight";
const char *t_ofeliaSpotlight::objName = "ofSpotlight";
const char *t_ofeliaDirectionalLight::objName = "ofDirectionalLight";

/* ________________________________________________________________________________
 * ofPointLight object methods
 */
void *ofeliaPointLight_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaPointLight *x = reinterpret_cast<t_ofeliaPointLight*>(pd_new(ofeliaPointLight_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getVec3fFromArgs(argc, argv, x->pos, t_ofeliaPointLight::objName))
        return 0;
    x->color.specular.set(ofColor(100));
    x->color.diffuse.set(ofColor(200));
    x->color.ambient.set(ofColor(0));
    x->attenuation.set(1.0f, 0.0f, 0.0f);
    x->debug = false;
    x->shouldUpdateLight = true;
    x->bInitGate = false;
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("x"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("y"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("z"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaPointLight_init(t_ofeliaPointLight *x)
{
    if (!x->bInitGate)
        return;
    x->shouldUpdateLight = true;
}

void ofeliaPointLight_exit(t_ofeliaPointLight *x)
{
    x->light.reset();
    x->bInitGate = true;
}

void ofeliaPointLight_bang(t_ofeliaPointLight *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (x->shouldUpdateLight) {
            
            x->light = make_unique<ofLight>();
            x->light->setPointLight();
            x->light->setSpecularColor(x->color.specular);
            x->light->setDiffuseColor(x->color.diffuse);
            x->light->setAmbientColor(x->color.ambient);
            x->light->setAttenuation(x->attenuation.x, x->attenuation.y, x->attenuation.z);
            x->shouldUpdateLight = false;
        }
        x->light->setPosition(x->pos * ofeliaWindow::scaleFactor);

        if (x->debug) {
            
            ofGetCurrentRenderer()->pushMatrix();
            const float scaleAmt = 1.0f / ofeliaWindow::scaleFactor;
            ofGetCurrentRenderer()->scale(scaleAmt, scaleAmt, scaleAmt);
            x->light->setScale(ofVec3f(ofeliaWindow::scaleFactor));
            x->light->draw();
            ofGetCurrentRenderer()->popMatrix();
        }
        ofGetGLRenderer()->enableLighting();
        x->light->enable();
        outlet_bang(x->x_obj.ob_outlet);
        x->light->disable();
        ofGetGLRenderer()->disableLighting();
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaPointLight::objName);
    }
}

void ofeliaPointLight_x(t_ofeliaPointLight *x, t_floatarg f)
{
    x->pos.x = f;
}

void ofeliaPointLight_y(t_ofeliaPointLight *x, t_floatarg f)
{
    x->pos.y = f;
}

void ofeliaPointLight_z(t_ofeliaPointLight *x, t_floatarg f)
{
    x->pos.z = f;
}

void ofeliaPointLight_specular(t_ofeliaPointLight *x, t_symbol *s, int argc, t_atom *argv)
{
    getColorFromArgs(argc, argv, x->color.specular, t_ofeliaPointLight::objName);
    x->shouldUpdateLight = true;
}

void ofeliaPointLight_diffuse(t_ofeliaPointLight *x, t_symbol *s, int argc, t_atom *argv)
{
    getColorFromArgs(argc, argv, x->color.diffuse, t_ofeliaPointLight::objName);
    x->shouldUpdateLight = true;
}

void ofeliaPointLight_ambient(t_ofeliaPointLight *x, t_symbol *s, int argc, t_atom *argv)
{
    getColorFromArgs(argc, argv, x->color.ambient, t_ofeliaPointLight::objName);
    x->shouldUpdateLight = true;
}

void ofeliaPointLight_attenuation(t_ofeliaPointLight *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc == 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            x->attenuation.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
            x->shouldUpdateLight = true;
        }
        else
            error("%s: wrong argument type", t_ofeliaPointLight::objName);
    }
    else
        error("%s: invalid number of arguments", t_ofeliaPointLight::objName);
}


void ofeliaPointLight_debug(t_ofeliaPointLight *x, t_symbol *s, int argc, t_atom *argv)
{
    getToggleFromArgs(argc, argv, x->debug, t_ofeliaPointLight::objName);
}

void ofeliaPointLight_set(t_ofeliaPointLight *x, t_symbol *s, int argc, t_atom *argv)
{
    getVec3fFromArgs(argc, argv, x->pos, t_ofeliaPointLight::objName);
}

void ofeliaPointLight_print(t_ofeliaPointLight *x)
{
    post("\n[%s]", t_ofeliaPointLight::objName);
    post("x : %g", x->pos.x);
    post("y : %g", x->pos.y);
    post("z : %g", x->pos.z);
    post("specular : %d %d %d", x->color.specular.r, x->color.specular.g, x->color.specular.b);
    post("diffuse : %d %d %d", x->color.diffuse.r, x->color.diffuse.g, x->color.diffuse.b);
    post("ambient : %d %d %d", x->color.ambient.r, x->color.ambient.g, x->color.ambient.b);
    post("attenuation : %g %g %g", x->attenuation.x, x->attenuation.y, x->attenuation.z);
    printToggle("debug", x->debug);
}

void ofeliaPointLight_free(t_ofeliaPointLight *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    x->light.reset();
}

void ofeliaPointLight_setup()
{
    ofeliaPointLight_class = class_new(gensym("ofPointLight"),
                                       reinterpret_cast<t_newmethod>(ofeliaPointLight_new),
                                       reinterpret_cast<t_method>(ofeliaPointLight_free),
                                       sizeof(t_ofeliaPointLight),
                                       CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaPointLight_class, reinterpret_cast<t_method>(ofeliaPointLight_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaPointLight_class, reinterpret_cast<t_method>(ofeliaPointLight_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addbang(ofeliaPointLight_class, reinterpret_cast<t_method>(ofeliaPointLight_bang));
    class_addmethod(ofeliaPointLight_class, reinterpret_cast<t_method>(ofeliaPointLight_x),
                    gensym("x"), A_FLOAT, 0);
    class_addmethod(ofeliaPointLight_class, reinterpret_cast<t_method>(ofeliaPointLight_y),
                    gensym("y"), A_FLOAT, 0);
    class_addmethod(ofeliaPointLight_class, reinterpret_cast<t_method>(ofeliaPointLight_z),
                    gensym("z"), A_FLOAT, 0);
    class_addmethod(ofeliaPointLight_class, reinterpret_cast<t_method>(ofeliaPointLight_specular),
                    gensym("specular"), A_GIMME, 0);
    class_addmethod(ofeliaPointLight_class, reinterpret_cast<t_method>(ofeliaPointLight_diffuse),
                    gensym("diffuse"), A_GIMME, 0);
    class_addmethod(ofeliaPointLight_class, reinterpret_cast<t_method>(ofeliaPointLight_ambient),
                    gensym("ambient"), A_GIMME, 0);
    class_addmethod(ofeliaPointLight_class, reinterpret_cast<t_method>(ofeliaPointLight_attenuation),
                    gensym("attenuation"), A_GIMME, 0);
    class_addmethod(ofeliaPointLight_class, reinterpret_cast<t_method>(ofeliaPointLight_debug),
                    gensym("debug"), A_GIMME, 0);
    class_addmethod(ofeliaPointLight_class, reinterpret_cast<t_method>(ofeliaPointLight_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaPointLight_class, reinterpret_cast<t_method>(ofeliaPointLight_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSpotlight object methods
 */
void *ofeliaSpotlight_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSpotlight *x = reinterpret_cast<t_ofeliaSpotlight*>(pd_new(ofeliaSpotlight_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getVec3fFromArgs(argc, argv, x->pos, t_ofeliaSpotlight::objName))
        return 0;
    x->eulerAngles.set(0.0f, 0.0f, 0.0f);
    x->quaternion.set(0.0f, 0.0f, 0.0f, 1.0f);
    x->rotate.angle = 0.0f;
    x->rotate.axis.set(0.0f, 0.0f, 0.0f);
    x->lookAt.set(0.0f, 0.0f, 0.0f);
    x->orienType = LIGHT_ORIEN_TYPE_ROTATE;
    x->color.specular.set(ofColor(100));
    x->color.diffuse.set(ofColor(200));
    x->color.ambient.set(ofColor(0));
    x->cutoff = 45.0f;
    x->concentration = 64.0f;
    x->attenuation.set(1.0f, 0.0f, 0.0f);
    x->debug = false;
    x->shouldUpdateLight = true;
    x->bInitGate = false;
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("x"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("y"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("z"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSpotlight_init(t_ofeliaSpotlight *x)
{
    if (!x->bInitGate)
        return;
    x->shouldUpdateLight = true;
}

void ofeliaSpotlight_exit(t_ofeliaSpotlight *x)
{
    x->light.reset();
    x->bInitGate = true;
}

void ofeliaSpotlight_bang(t_ofeliaSpotlight *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (x->shouldUpdateLight) {
            
            x->light = make_unique<ofLight>();
            x->light->setSpotlight();
            x->light->setSpecularColor(x->color.specular);
            x->light->setDiffuseColor(x->color.diffuse);
            x->light->setAmbientColor(x->color.ambient);
            x->light->setSpotlightCutOff(x->cutoff);
            x->light->setSpotConcentration(x->concentration);
            x->light->setAttenuation(x->attenuation.x, x->attenuation.y, x->attenuation.z);
            x->shouldUpdateLight = false;
        }
        x->light->setPosition(x->pos * ofeliaWindow::scaleFactor);
        
        switch (x->orienType) {
                
            case LIGHT_ORIEN_TYPE_ROTATE:
                x->light->setOrientation(ofQuaternion(x->rotate.angle, x->rotate.axis));
                break;
            case LIGHT_ORIEN_TYPE_EULERANGLES:
                x->light->setOrientation(x->eulerAngles);
                break;
            case LIGHT_ORIEN_TYPE_QUATERNION:
                x->light->setOrientation(ofQuaternion(x->quaternion));
                break;
            case LIGHT_ORIEN_TYPE_LOOKAT:
                x->light->lookAt(x->lookAt * ofeliaWindow::scaleFactor);
                break;
            default:
                break;
        }
        if (x->debug) {
            
            ofGetCurrentRenderer()->pushMatrix();
            const float scaleAmt = 1.0f / ofeliaWindow::scaleFactor;
            ofGetCurrentRenderer()->scale(scaleAmt, scaleAmt, scaleAmt);
            x->light->setScale(ofVec3f(ofeliaWindow::scaleFactor));
            x->light->draw();
            ofGetCurrentRenderer()->popMatrix();
        }
        ofGetGLRenderer()->enableLighting();
        x->light->enable();
        outlet_bang(x->x_obj.ob_outlet);
        x->light->disable();
        ofGetGLRenderer()->disableLighting();
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSpotlight::objName);
    }
}

void ofeliaSpotlight_x(t_ofeliaSpotlight *x, t_floatarg f)
{
    x->pos.x = f;
}

void ofeliaSpotlight_y(t_ofeliaSpotlight *x, t_floatarg f)
{
    x->pos.y = f;
}

void ofeliaSpotlight_z(t_ofeliaSpotlight *x, t_floatarg f)
{
    x->pos.z = f;
}

void ofeliaSpotlight_rotate(t_ofeliaSpotlight *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc == 4) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT &&
            argv[3].a_type == A_FLOAT) {
            
            x->rotate.angle = argv[0].a_w.w_float;
            x->rotate.axis.set(argv[1].a_w.w_float, argv[2].a_w.w_float, argv[3].a_w.w_float);
            x->orienType = LIGHT_ORIEN_TYPE_ROTATE;
        }
        else
            error("%s: wrong argument type", t_ofeliaSpotlight::objName);
    }
    else
        error("%s: invalid number of arguments", t_ofeliaSpotlight::objName);
}

void ofeliaSpotlight_eulerAngles(t_ofeliaSpotlight *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc == 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            x->eulerAngles.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
            x->orienType = LIGHT_ORIEN_TYPE_EULERANGLES;
        }
        else
            error("%s: wrong argument type", t_ofeliaSpotlight::objName);
    }
    else
        error("%s: invalid number of arguments", t_ofeliaSpotlight::objName);
}

void ofeliaSpotlight_quaternion(t_ofeliaSpotlight *x, t_symbol *s, int argc, t_atom *argv)
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
            x->orienType = LIGHT_ORIEN_TYPE_QUATERNION;
        }
        else
            error("%s: wrong argument type", t_ofeliaSpotlight::objName);
    }
    else
        error("%s: invalid number of arguments", t_ofeliaSpotlight::objName);
}

void ofeliaSpotlight_lookAt(t_ofeliaSpotlight *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc == 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            x->lookAt.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
            x->orienType = LIGHT_ORIEN_TYPE_LOOKAT;
        }
        else
            error("%s: wrong argument type", t_ofeliaSpotlight::objName);
    }
    else
        error("%s: invalid number of arguments", t_ofeliaSpotlight::objName);
}

void ofeliaSpotlight_specular(t_ofeliaSpotlight *x, t_symbol *s, int argc, t_atom *argv)
{
    getColorFromArgs(argc, argv, x->color.specular, t_ofeliaSpotlight::objName);
    x->shouldUpdateLight = true;
}

void ofeliaSpotlight_diffuse(t_ofeliaSpotlight *x, t_symbol *s, int argc, t_atom *argv)
{
    getColorFromArgs(argc, argv, x->color.diffuse, t_ofeliaSpotlight::objName);
    x->shouldUpdateLight = true;
}

void ofeliaSpotlight_ambient(t_ofeliaSpotlight *x, t_symbol *s, int argc, t_atom *argv)
{
    getColorFromArgs(argc, argv, x->color.ambient, t_ofeliaSpotlight::objName);
    x->shouldUpdateLight = true;
}

void ofeliaSpotlight_cutoff(t_ofeliaSpotlight *x, t_floatarg f)
{
    x->cutoff = f;
    x->shouldUpdateLight = true;
}

void ofeliaSpotlight_concentration(t_ofeliaSpotlight *x, t_floatarg f)
{
    x->concentration = f;
    x->shouldUpdateLight = true;
}

void ofeliaSpotlight_attenuation(t_ofeliaSpotlight *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc == 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            x->attenuation.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
            x->shouldUpdateLight = true;
        }
        else
            error("%s: wrong argument type", t_ofeliaSpotlight::objName);
    }
    else
        error("%s: invalid number of arguments", t_ofeliaSpotlight::objName);
}

void ofeliaSpotlight_debug(t_ofeliaSpotlight *x, t_symbol *s, int argc, t_atom *argv)
{
    getToggleFromArgs(argc, argv, x->debug, t_ofeliaSpotlight::objName);
}

void ofeliaSpotlight_set(t_ofeliaSpotlight *x, t_symbol *s, int argc, t_atom *argv)
{
    getVec3fFromArgs(argc, argv, x->pos, t_ofeliaSpotlight::objName);
}

void ofeliaSpotlight_print(t_ofeliaSpotlight *x)
{
    post("\n[%s]", t_ofeliaSpotlight::objName);
    post("x : %g", x->pos.x);
    post("y : %g", x->pos.y);
    post("z : %g", x->pos.z);
    post("rotate : %g %g %g %g", x->rotate.angle,
                                     x->rotate.axis.x, x->rotate.axis.y, x->rotate.axis.z);
    post("eulerAngles : %g %g %g", x->eulerAngles.x, x->eulerAngles.y, x->eulerAngles.z);
    post("quaternion : %g %g %g %g", x->quaternion.x, x->quaternion.y, x->quaternion.z, x->quaternion.w);
    post("lookAt : %g %g %g", x->lookAt.x, x->lookAt.y, x->lookAt.z);
    post("specular : %d %d %d", x->color.specular.r, x->color.specular.g, x->color.specular.b);
    post("diffuse : %d %d %d", x->color.diffuse.r, x->color.diffuse.g, x->color.diffuse.b);
    post("ambient : %d %d %d", x->color.ambient.r, x->color.ambient.g, x->color.ambient.b);
    post("cutoff : %g", x->cutoff);
    post("concentration : %g", x->concentration);
    post("attenuation : %g %g %g", x->attenuation.x, x->attenuation.y, x->attenuation.z);
    printToggle("debug", x->debug);
}

void ofeliaSpotlight_free(t_ofeliaSpotlight *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    x->light.reset();
}

void ofeliaSpotlight_setup()
{
    ofeliaSpotlight_class = class_new(gensym("ofSpotlight"),
                                      reinterpret_cast<t_newmethod>(ofeliaSpotlight_new),
                                      reinterpret_cast<t_method>(ofeliaSpotlight_free),
                                      sizeof(t_ofeliaSpotlight),
                                      CLASS_DEFAULT, A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaSpotlight_new),
                     gensym("ofSpotLight"), A_GIMME, 0);
    class_addmethod(ofeliaSpotlight_class, reinterpret_cast<t_method>(ofeliaSpotlight_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaSpotlight_class, reinterpret_cast<t_method>(ofeliaSpotlight_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addbang(ofeliaSpotlight_class, reinterpret_cast<t_method>(ofeliaSpotlight_bang));
    class_addmethod(ofeliaSpotlight_class, reinterpret_cast<t_method>(ofeliaSpotlight_x),
                    gensym("x"), A_FLOAT, 0);
    class_addmethod(ofeliaSpotlight_class, reinterpret_cast<t_method>(ofeliaSpotlight_y),
                    gensym("y"), A_FLOAT, 0);
    class_addmethod(ofeliaSpotlight_class, reinterpret_cast<t_method>(ofeliaSpotlight_z),
                    gensym("z"), A_FLOAT, 0);
    class_addmethod(ofeliaSpotlight_class, reinterpret_cast<t_method>(ofeliaSpotlight_rotate),
                    gensym("rotate"), A_GIMME, 0);
    class_addmethod(ofeliaSpotlight_class, reinterpret_cast<t_method>(ofeliaSpotlight_eulerAngles),
                    gensym("eulerAngles"), A_GIMME, 0);
    class_addmethod(ofeliaSpotlight_class, reinterpret_cast<t_method>(ofeliaSpotlight_quaternion),
                    gensym("quaternion"), A_GIMME, 0);
    class_addmethod(ofeliaSpotlight_class, reinterpret_cast<t_method>(ofeliaSpotlight_lookAt),
                    gensym("lookAt"), A_GIMME, 0);
    class_addmethod(ofeliaSpotlight_class, reinterpret_cast<t_method>(ofeliaSpotlight_specular),
                    gensym("specular"), A_GIMME, 0);
    class_addmethod(ofeliaSpotlight_class, reinterpret_cast<t_method>(ofeliaSpotlight_diffuse),
                    gensym("diffuse"), A_GIMME, 0);
    class_addmethod(ofeliaSpotlight_class, reinterpret_cast<t_method>(ofeliaSpotlight_ambient),
                    gensym("ambient"), A_GIMME, 0);
    class_addmethod(ofeliaSpotlight_class, reinterpret_cast<t_method>(ofeliaSpotlight_cutoff),
                    gensym("cutoff"), A_FLOAT, 0);
    class_addmethod(ofeliaSpotlight_class, reinterpret_cast<t_method>(ofeliaSpotlight_concentration),
                    gensym("concentration"), A_FLOAT, 0);
    class_addmethod(ofeliaSpotlight_class, reinterpret_cast<t_method>(ofeliaSpotlight_attenuation),
                    gensym("attenuation"), A_GIMME, 0);
    class_addmethod(ofeliaSpotlight_class, reinterpret_cast<t_method>(ofeliaSpotlight_debug),
                    gensym("debug"), A_GIMME, 0);
    class_addmethod(ofeliaSpotlight_class, reinterpret_cast<t_method>(ofeliaSpotlight_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSpotlight_class, reinterpret_cast<t_method>(ofeliaSpotlight_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofDirectionalLight object methods
 */
void *ofeliaDirectionalLight_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaDirectionalLight *x = reinterpret_cast<t_ofeliaDirectionalLight*>(pd_new(ofeliaDirectionalLight_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getRotateElemFromArgs(argc, argv, x->rotate, t_ofeliaDirectionalLight::objName))
        return 0;
    x->eulerAngles.set(0.0f, 0.0f, 0.0f);
    x->quaternion.set(0.0f, 0.0f, 0.0f, 1.0f);
    x->orienType = LIGHT_ORIEN_TYPE_ROTATE;
    x->color.specular.set(ofColor(100));
    x->color.diffuse.set(ofColor(200));
    x->color.ambient.set(ofColor(0));
    x->shouldUpdateLight = true;
    x->bInitGate = false;
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("angle"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("x"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("y"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("z"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaDirectionalLight_init(t_ofeliaDirectionalLight *x)
{
    if (!x->bInitGate)
        return;
    x->shouldUpdateLight = true;
}

void ofeliaDirectionalLight_exit(t_ofeliaDirectionalLight *x)
{
    x->light.reset();
    x->bInitGate = true;
}

void ofeliaDirectionalLight_bang(t_ofeliaDirectionalLight *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (x->shouldUpdateLight) {
            
            x->light = make_unique<ofLight>();
            x->light->setDirectional();
            x->light->setSpecularColor(x->color.specular);
            x->light->setDiffuseColor(x->color.diffuse);
            x->light->setAmbientColor(x->color.ambient);
            x->shouldUpdateLight = false;
        }
        switch (x->orienType) {
                
            case LIGHT_ORIEN_TYPE_ROTATE:
                x->light->setOrientation(ofQuaternion(x->rotate.angle, x->rotate.axis));
                break;
            case LIGHT_ORIEN_TYPE_EULERANGLES:
                x->light->setOrientation(x->eulerAngles);
                break;
            case LIGHT_ORIEN_TYPE_QUATERNION:
                x->light->setOrientation(ofQuaternion(x->quaternion));
                break;
            default:
                break;
        }
        ofGetGLRenderer()->enableLighting();
        x->light->enable();
        outlet_bang(x->x_obj.ob_outlet);
        x->light->disable();
        ofGetGLRenderer()->disableLighting();
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaDirectionalLight::objName);
    }
}

void ofeliaDirectionalLight_angle(t_ofeliaDirectionalLight *x, t_floatarg f)
{
    x->rotate.angle = f;
    x->orienType = LIGHT_ORIEN_TYPE_ROTATE;
}

void ofeliaDirectionalLight_x(t_ofeliaDirectionalLight *x, t_floatarg f)
{
    x->rotate.axis.x = f;
    x->orienType = LIGHT_ORIEN_TYPE_ROTATE;
}

void ofeliaDirectionalLight_y(t_ofeliaDirectionalLight *x, t_floatarg f)
{
    x->rotate.axis.y = f;
    x->orienType = LIGHT_ORIEN_TYPE_ROTATE;
}

void ofeliaDirectionalLight_z(t_ofeliaDirectionalLight *x, t_floatarg f)
{
    x->rotate.axis.z = f;
    x->orienType = LIGHT_ORIEN_TYPE_ROTATE;
}

void ofeliaDirectionalLight_rotate(t_ofeliaDirectionalLight *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc == 4) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT &&
            argv[3].a_type == A_FLOAT) {
            
            x->rotate.angle = argv[0].a_w.w_float;
            x->rotate.axis.set(argv[1].a_w.w_float, argv[2].a_w.w_float, argv[3].a_w.w_float);
            x->orienType = LIGHT_ORIEN_TYPE_ROTATE;
        }
        else
            error("%s: wrong argument type", t_ofeliaDirectionalLight::objName);
    }
    else
        error("%s: invalid number of arguments", t_ofeliaDirectionalLight::objName);
}

void ofeliaDirectionalLight_eulerAngles(t_ofeliaDirectionalLight *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc == 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            x->eulerAngles.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
            x->orienType = LIGHT_ORIEN_TYPE_EULERANGLES;
        }
        else
            error("%s: wrong argument type", t_ofeliaDirectionalLight::objName);
    }
    else
        error("%s: invalid number of arguments", t_ofeliaDirectionalLight::objName);
}

void ofeliaDirectionalLight_quaternion(t_ofeliaDirectionalLight *x, t_symbol *s, int argc, t_atom *argv)
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
            x->orienType = LIGHT_ORIEN_TYPE_QUATERNION;
        }
        else
            error("%s: wrong argument type", t_ofeliaDirectionalLight::objName);
    }
    else
        error("%s: invalid number of arguments", t_ofeliaDirectionalLight::objName);
}

void ofeliaDirectionalLight_specular(t_ofeliaDirectionalLight *x, t_symbol *s, int argc, t_atom *argv)
{
    getColorFromArgs(argc, argv, x->color.specular, t_ofeliaDirectionalLight::objName);
    x->shouldUpdateLight = true;
}

void ofeliaDirectionalLight_diffuse(t_ofeliaDirectionalLight *x, t_symbol *s, int argc, t_atom *argv)
{
    getColorFromArgs(argc, argv, x->color.diffuse, t_ofeliaDirectionalLight::objName);
    x->shouldUpdateLight = true;
}

void ofeliaDirectionalLight_ambient(t_ofeliaDirectionalLight *x, t_symbol *s, int argc, t_atom *argv)
{
    getColorFromArgs(argc, argv, x->color.ambient, t_ofeliaDirectionalLight::objName);
    x->shouldUpdateLight = true;
}

void ofeliaDirectionalLight_set(t_ofeliaDirectionalLight *x, t_symbol *s, int argc, t_atom *argv)
{
    getRotateElemFromArgs(argc, argv, x->rotate, t_ofeliaDirectionalLight::objName);
}

void ofeliaDirectionalLight_print(t_ofeliaDirectionalLight *x)
{
    post("\n[%s]", t_ofeliaDirectionalLight::objName);
    post("rotate : %g %g %g %g", x->rotate.angle,
                                     x->rotate.axis.x, x->rotate.axis.y, x->rotate.axis.z);
    post("eulerAngles : %g %g %g", x->eulerAngles.x, x->eulerAngles.y, x->eulerAngles.z);
    post("quaternion : %g %g %g %g", x->quaternion.x, x->quaternion.y, x->quaternion.z, x->quaternion.w);
    post("specular : %d %d %d", x->color.specular.r, x->color.specular.g, x->color.specular.b);
    post("diffuse : %d %d %d", x->color.diffuse.r, x->color.diffuse.g, x->color.diffuse.b);
    post("ambient : %d %d %d", x->color.ambient.r, x->color.ambient.g, x->color.ambient.b);
}

void ofeliaDirectionalLight_free(t_ofeliaDirectionalLight *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    x->light.reset();
}

void ofeliaDirectionalLight_setup()
{
    ofeliaDirectionalLight_class = class_new(gensym("ofDirectionalLight"),
                                             reinterpret_cast<t_newmethod>(ofeliaDirectionalLight_new),
                                             reinterpret_cast<t_method>(ofeliaDirectionalLight_free),
                                             sizeof(t_ofeliaDirectionalLight),
                                             CLASS_DEFAULT, A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaDirectionalLight_new),
                     gensym("ofDirLight"), A_GIMME, 0);
    class_addmethod(ofeliaDirectionalLight_class, reinterpret_cast<t_method>(ofeliaDirectionalLight_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaDirectionalLight_class, reinterpret_cast<t_method>(ofeliaDirectionalLight_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addbang(ofeliaDirectionalLight_class, reinterpret_cast<t_method>(ofeliaDirectionalLight_bang));
    class_addmethod(ofeliaDirectionalLight_class, reinterpret_cast<t_method>(ofeliaDirectionalLight_angle),
                    gensym("angle"), A_FLOAT, 0);
    class_addmethod(ofeliaDirectionalLight_class, reinterpret_cast<t_method>(ofeliaDirectionalLight_x),
                    gensym("x"), A_FLOAT, 0);
    class_addmethod(ofeliaDirectionalLight_class, reinterpret_cast<t_method>(ofeliaDirectionalLight_y),
                    gensym("y"), A_FLOAT, 0);
    class_addmethod(ofeliaDirectionalLight_class, reinterpret_cast<t_method>(ofeliaDirectionalLight_z),
                    gensym("z"), A_FLOAT, 0);
    class_addmethod(ofeliaDirectionalLight_class, reinterpret_cast<t_method>(ofeliaDirectionalLight_rotate),
                    gensym("rotate"), A_GIMME, 0);
    class_addmethod(ofeliaDirectionalLight_class, reinterpret_cast<t_method>(ofeliaDirectionalLight_eulerAngles),
                    gensym("eulerAngles"), A_GIMME, 0);
    class_addmethod(ofeliaDirectionalLight_class, reinterpret_cast<t_method>(ofeliaDirectionalLight_quaternion),
                    gensym("quaternion"), A_GIMME, 0);
    class_addmethod(ofeliaDirectionalLight_class, reinterpret_cast<t_method>(ofeliaDirectionalLight_specular),
                    gensym("specular"), A_GIMME, 0);
    class_addmethod(ofeliaDirectionalLight_class, reinterpret_cast<t_method>(ofeliaDirectionalLight_diffuse),
                    gensym("diffuse"), A_GIMME, 0);
    class_addmethod(ofeliaDirectionalLight_class, reinterpret_cast<t_method>(ofeliaDirectionalLight_ambient),
                    gensym("ambient"), A_GIMME, 0);
    class_addmethod(ofeliaDirectionalLight_class, reinterpret_cast<t_method>(ofeliaDirectionalLight_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaDirectionalLight_class, reinterpret_cast<t_method>(ofeliaDirectionalLight_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * setup methods
 */
void ofeliaLights_setup()
{
    ofeliaPointLight_setup();
    ofeliaSpotlight_setup();
    ofeliaDirectionalLight_setup();
}

