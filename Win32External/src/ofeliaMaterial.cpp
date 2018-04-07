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

#include "ofeliaMaterial.h"
#include "ofeliaWindow.h"

/* ________________________________________________________________________________
 * static variables
 */
const char *t_ofeliaMaterial::objName = "ofMaterial";

/* ________________________________________________________________________________
 * ofMaterial object methods
 */
void *ofeliaMaterial_new()
{
    t_ofeliaMaterial *x = reinterpret_cast<t_ofeliaMaterial*>(pd_new(ofeliaMaterial_class));
    x->elem.specular.set(ofColor(100));
    x->elem.diffuse.set(ofColor(200));
    x->elem.ambient.set(ofColor(0));
    x->elem.emissive.set(ofColor(50));
    x->elem.shininess = 120.0f;
    x->shouldUpdateMaterial = true;
    x->bInitGate = false;
    x->material = make_unique<ofMaterial>();
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaMaterial_init(t_ofeliaMaterial *x)
{
    if (!x->bInitGate)
        return;
    x->shouldUpdateMaterial = true;
}

void ofeliaMaterial_exit(t_ofeliaMaterial *x)
{
    x->bInitGate = true;
}

void ofeliaMaterial_bang(t_ofeliaMaterial *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (x->shouldUpdateMaterial) {
            
            x->material->setSpecularColor(x->elem.specular);
            x->material->setDiffuseColor(x->elem.diffuse);
            x->material->setAmbientColor(x->elem.ambient);
            x->material->setEmissiveColor(x->elem.emissive);
            x->material->setShininess(x->elem.shininess);
            x->shouldUpdateMaterial = false;
        }
        x->material->begin();
        outlet_bang(x->x_obj.ob_outlet);
        x->material->end();
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaMaterial::objName);
    }
}

void ofeliaMaterial_specular(t_ofeliaMaterial *x, t_symbol *s, int argc, t_atom *argv)
{
    getColorFromArgs(argc, argv, x->elem.specular, t_ofeliaMaterial::objName);
    x->shouldUpdateMaterial = true;
}

void ofeliaMaterial_diffuse(t_ofeliaMaterial *x, t_symbol *s, int argc, t_atom *argv)
{
    getColorFromArgs(argc, argv, x->elem.diffuse, t_ofeliaMaterial::objName);
    x->shouldUpdateMaterial = true;
}

void ofeliaMaterial_ambient(t_ofeliaMaterial *x, t_symbol *s, int argc, t_atom *argv)
{
    getColorFromArgs(argc, argv, x->elem.ambient, t_ofeliaMaterial::objName);
    x->shouldUpdateMaterial = true;
}

void ofeliaMaterial_emissive(t_ofeliaMaterial *x, t_symbol *s, int argc, t_atom *argv)
{
    getColorFromArgs(argc, argv, x->elem.emissive, t_ofeliaMaterial::objName);
    x->shouldUpdateMaterial = true;
}

void ofeliaMaterial_shininess(t_ofeliaMaterial *x, t_floatarg f)
{
    x->elem.shininess = f;
    x->shouldUpdateMaterial = true;
}

void ofeliaMaterial_print(t_ofeliaMaterial *x)
{
    post("\n[%s]", t_ofeliaMaterial::objName);
    post("specular : %d %d %d", x->elem.specular.r, x->elem.specular.g, x->elem.specular.b);
    post("diffuse : %d %d %d", x->elem.diffuse.r, x->elem.diffuse.g, x->elem.diffuse.b);
    post("ambient : %d %d %d", x->elem.ambient.r, x->elem.ambient.g, x->elem.ambient.b);
    post("emissive : %d %d %d", x->elem.emissive.r, x->elem.emissive.g, x->elem.emissive.b);
    post("shininess : %g", x->elem.shininess);
}

void ofeliaMaterial_free(t_ofeliaMaterial *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
}

void ofeliaMaterial_setup()
{
    ofeliaMaterial_class = class_new(gensym("ofMaterial"),
                                     reinterpret_cast<t_newmethod>(ofeliaMaterial_new),
                                     reinterpret_cast<t_method>(ofeliaMaterial_free),
                                     sizeof(t_ofeliaMaterial),
                                     CLASS_DEFAULT, A_NULL, 0);
    class_addmethod(ofeliaMaterial_class, reinterpret_cast<t_method>(ofeliaMaterial_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaMaterial_class, reinterpret_cast<t_method>(ofeliaMaterial_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addbang(ofeliaMaterial_class, reinterpret_cast<t_method>(ofeliaMaterial_bang));
    class_addmethod(ofeliaMaterial_class, reinterpret_cast<t_method>(ofeliaMaterial_specular),
                    gensym("specular"), A_GIMME, 0);
    class_addmethod(ofeliaMaterial_class, reinterpret_cast<t_method>(ofeliaMaterial_diffuse),
                    gensym("diffuse"), A_GIMME, 0);
    class_addmethod(ofeliaMaterial_class, reinterpret_cast<t_method>(ofeliaMaterial_ambient),
                    gensym("ambient"), A_GIMME, 0);
    class_addmethod(ofeliaMaterial_class, reinterpret_cast<t_method>(ofeliaMaterial_emissive),
                    gensym("emissive"), A_GIMME, 0);
    class_addmethod(ofeliaMaterial_class, reinterpret_cast<t_method>(ofeliaMaterial_shininess),
                    gensym("shininess"), A_FLOAT, 0);
    class_addmethod(ofeliaMaterial_class, reinterpret_cast<t_method>(ofeliaMaterial_print),
                    gensym("print"), A_NULL, 0);
}

