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

#include "ofeliaSetters.h"
#include "ofeliaWindow.h"
#include "ofeliaStyle.h"

/* ________________________________________________________________________________
 * static variables
 */
const char *t_ofeliaSetDepthTest::objName = "ofSetDepthTest";
const char *t_ofeliaSetArbTex::objName = "ofSetArbTex";
const char *t_ofeliaSetAntiAliasing::objName = "ofSetAntiAliasing";
const char *t_ofeliaSetBgAuto::objName = "ofSetBgAuto";
const char *t_ofeliaClear::objName = "ofClear";
const char *t_ofeliaClearColor::objName = "ofClearColor";
const char *t_ofeliaClearDepth::objName = "ofClearDepth";
const char *t_ofeliaClearAlpha::objName = "ofClearAlpha";

/* ________________________________________________________________________________
 * ofSetDepthTest object methods
 */
void *ofeliaSetDepthTest_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetDepthTest *x = reinterpret_cast<t_ofeliaSetDepthTest*>(pd_new(ofeliaSetDepthTest_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (argc) {
        
        if (!getToggleFromArgs(argc, argv, x->depthTest, t_ofeliaSetDepthTest::objName))
            return 0;
    }
    else {
        
        x->depthTest = false;
    }
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("depthTest"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetDepthTest_bang(t_ofeliaSetDepthTest *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofGetCurrentRenderer()->setDepthTest(x->depthTest);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetDepthTest::objName);
    }
}

void ofeliaSetDepthTest_set(t_ofeliaSetDepthTest *x, t_symbol *s, int argc, t_atom *argv)
{
    getToggleFromArgs(argc, argv, x->depthTest, t_ofeliaSetDepthTest::objName);
}

void ofeliaSetDepthTest_print(t_ofeliaSetDepthTest *x)
{
    post("\n[%s]", t_ofeliaSetDepthTest::objName);
    printToggle("depthTest", x->depthTest);
}

void ofeliaSetDepthTest_setup()
{
    ofeliaSetDepthTest_class = class_new(gensym("ofSetDepthTest"),
                                         reinterpret_cast<t_newmethod>(ofeliaSetDepthTest_new),
                                         0, sizeof(t_ofeliaSetDepthTest),
                                         CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetDepthTest_class, reinterpret_cast<t_method>(ofeliaSetDepthTest_bang));
    class_addmethod(ofeliaSetDepthTest_class, reinterpret_cast<t_method>(ofeliaSetDepthTest_set),
                    gensym("depthTest"), A_GIMME, 0);
    class_addmethod(ofeliaSetDepthTest_class, reinterpret_cast<t_method>(ofeliaSetDepthTest_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetDepthTest_class, reinterpret_cast<t_method>(ofeliaSetDepthTest_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetArbTex object methods
 */
void *ofeliaSetArbTex_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetArbTex *x = reinterpret_cast<t_ofeliaSetArbTex*>(pd_new(ofeliaSetArbTex_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (argc) {
        
        if (!getToggleFromArgs(argc, argv, x->arbTex, t_ofeliaSetArbTex::objName))
            return 0;
    }
    else {
        
        x->arbTex = false;
    }
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("arbTex"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetArbTex_bang(t_ofeliaSetArbTex *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (x->arbTex)
            ofEnableArbTex();
        else
            ofDisableArbTex();
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetArbTex::objName);
    }
}

void ofeliaSetArbTex_set(t_ofeliaSetArbTex *x, t_symbol *s, int argc, t_atom *argv)
{
    getToggleFromArgs(argc, argv, x->arbTex, t_ofeliaSetArbTex::objName);
}

void ofeliaSetArbTex_print(t_ofeliaSetArbTex *x)
{
    post("\n[%s]", t_ofeliaSetArbTex::objName);
    printToggle("arbTex", x->arbTex);
}

void ofeliaSetArbTex_setup()
{
    ofeliaSetArbTex_class = class_new(gensym("ofSetArbTex"),
                                      reinterpret_cast<t_newmethod>(ofeliaSetArbTex_new),
                                      0, sizeof(t_ofeliaSetArbTex),
                                      CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetArbTex_class, reinterpret_cast<t_method>(ofeliaSetArbTex_bang));
    class_addmethod(ofeliaSetArbTex_class, reinterpret_cast<t_method>(ofeliaSetArbTex_set),
                    gensym("arbTex"), A_GIMME, 0);
    class_addmethod(ofeliaSetArbTex_class, reinterpret_cast<t_method>(ofeliaSetArbTex_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetArbTex_class, reinterpret_cast<t_method>(ofeliaSetArbTex_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetAntiAliasing object methods
 */
void *ofeliaSetAntiAliasing_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetAntiAliasing *x = reinterpret_cast<t_ofeliaSetAntiAliasing*>(pd_new(ofeliaSetAntiAliasing_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (argc) {
        
        if (!getToggleFromArgs(argc, argv, x->antiAliasing, t_ofeliaSetAntiAliasing::objName))
            return 0;
    }
    else {
        
        x->antiAliasing = false;
    }
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("antiAliasing"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetAntiAliasing_bang(t_ofeliaSetAntiAliasing *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (x->antiAliasing)
            ofGetCurrentRenderer()->enableAntiAliasing();
        else
            ofGetCurrentRenderer()->disableAntiAliasing();
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetAntiAliasing::objName);
    }
}

void ofeliaSetAntiAliasing_set(t_ofeliaSetAntiAliasing *x, t_symbol *s, int argc, t_atom *argv)
{
    getToggleFromArgs(argc, argv, x->antiAliasing, t_ofeliaSetAntiAliasing::objName);
}

void ofeliaSetAntiAliasing_print(t_ofeliaSetAntiAliasing *x)
{
    post("\n[%s]", t_ofeliaSetAntiAliasing::objName);
    printToggle("antiAliasing", x->antiAliasing);
}

void ofeliaSetAntiAliasing_setup()
{
    ofeliaSetAntiAliasing_class = class_new(gensym("ofSetAntiAliasing"),
                                            reinterpret_cast<t_newmethod>(ofeliaSetAntiAliasing_new),
                                            0, sizeof(t_ofeliaSetAntiAliasing),
                                            CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetAntiAliasing_class, reinterpret_cast<t_method>(ofeliaSetAntiAliasing_bang));
    class_addmethod(ofeliaSetAntiAliasing_class, reinterpret_cast<t_method>(ofeliaSetAntiAliasing_set),
                    gensym("antiAliasing"), A_GIMME, 0);
    class_addmethod(ofeliaSetAntiAliasing_class, reinterpret_cast<t_method>(ofeliaSetAntiAliasing_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetAntiAliasing_class, reinterpret_cast<t_method>(ofeliaSetAntiAliasing_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetBgAuto object methods
 */
void *ofeliaSetBgAuto_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetBgAuto *x = reinterpret_cast<t_ofeliaSetBgAuto*>(pd_new(ofeliaSetBgAuto_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (argc) {
        
        if (!getToggleFromArgs(argc, argv, x->bgAuto, t_ofeliaSetBgAuto::objName))
            return 0;
    }
    else {
        
        x->bgAuto = true;
    }
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("bgAuto"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetBgAuto_bang(t_ofeliaSetBgAuto *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofSetBackgroundAuto(x->bgAuto);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetBgAuto::objName);
    }
}

void ofeliaSetBgAuto_set(t_ofeliaSetBgAuto *x, t_symbol *s, int argc, t_atom *argv)
{
    getToggleFromArgs(argc, argv, x->bgAuto, t_ofeliaSetBgAuto::objName);
}

void ofeliaSetBgAuto_print(t_ofeliaSetBgAuto *x)
{
    post("\n[%s]", t_ofeliaSetBgAuto::objName);
    printToggle("bgAuto", x->bgAuto);
}

void ofeliaSetBgAuto_setup()
{
    ofeliaSetBgAuto_class = class_new(gensym("ofSetBgAuto"),
                                           reinterpret_cast<t_newmethod>(ofeliaSetBgAuto_new),
                                           0, sizeof(t_ofeliaSetBgAuto),
                                           CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetBgAuto_class, reinterpret_cast<t_method>(ofeliaSetBgAuto_bang));
    class_addmethod(ofeliaSetBgAuto_class, reinterpret_cast<t_method>(ofeliaSetBgAuto_set),
                    gensym("bgAuto"), A_GIMME, 0);
    class_addmethod(ofeliaSetBgAuto_class, reinterpret_cast<t_method>(ofeliaSetBgAuto_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetBgAuto_class, reinterpret_cast<t_method>(ofeliaSetBgAuto_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofClear object methods
 */
void *ofeliaClear_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaClear *x = reinterpret_cast<t_ofeliaClear*>(pd_new(ofeliaClear_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getColorAlphaFromArgs(argc, argv, x->color, t_ofeliaClear::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("r"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("g"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("b"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("a"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaClear_bang(t_ofeliaClear *x)
{
    if (t_ofeliaWindow::bRenderGate) {

        ofGetCurrentRenderer()->clear(x->color.r, x->color.g, x->color.b, x->color.a);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaClear::objName);
    }
}

void ofeliaClear_r(t_ofeliaClear *x, t_floatarg f)
{
    x->color.r = static_cast<unsigned char>(f);
}

void ofeliaClear_g(t_ofeliaClear *x, t_floatarg f)
{
    x->color.g = static_cast<unsigned char>(f);
}

void ofeliaClear_b(t_ofeliaClear *x, t_floatarg f)
{
    x->color.b = static_cast<unsigned char>(f);
}

void ofeliaClear_a(t_ofeliaClear *x, t_floatarg f)
{
    x->color.a = static_cast<unsigned char>(f);
}

void ofeliaClear_set(t_ofeliaClear *x, t_symbol *s, int argc, t_atom *argv)
{
    getColorAlphaFromArgs(argc, argv, x->color, t_ofeliaClear::objName);
}

void ofeliaClear_print(t_ofeliaClear *x)
{
    post("\n[%s]", t_ofeliaClear::objName);
    post("r : %d", x->color.r);
    post("g : %d", x->color.g);
    post("b : %d", x->color.b);
    post("a : %d", x->color.a);
}

void ofeliaClear_setup()
{
    ofeliaClear_class = class_new(gensym("ofClear"),
                                  reinterpret_cast<t_newmethod>(ofeliaClear_new),
                                  0, sizeof(t_ofeliaClear),
                                  CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaClear_class, reinterpret_cast<t_method>(ofeliaClear_bang));
    class_addmethod(ofeliaClear_class, reinterpret_cast<t_method>(ofeliaClear_r),
                    gensym("r"), A_FLOAT, 0);
    class_addmethod(ofeliaClear_class, reinterpret_cast<t_method>(ofeliaClear_g),
                    gensym("g"), A_FLOAT, 0);
    class_addmethod(ofeliaClear_class, reinterpret_cast<t_method>(ofeliaClear_b),
                    gensym("b"), A_FLOAT, 0);
    class_addmethod(ofeliaClear_class, reinterpret_cast<t_method>(ofeliaClear_a),
                    gensym("a"), A_FLOAT, 0);
    class_addmethod(ofeliaClear_class, reinterpret_cast<t_method>(ofeliaClear_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaClear_class, reinterpret_cast<t_method>(ofeliaClear_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofClearColor object methods
 */
void *ofeliaClearColor_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaClearColor *x = reinterpret_cast<t_ofeliaClearColor*>(pd_new(ofeliaClearColor_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getColorAlphaFromArgs(argc, argv, x->color, t_ofeliaClearColor::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("r"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("g"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("b"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("a"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaClearColor_bang(t_ofeliaClearColor *x)
{
    if (t_ofeliaWindow::bRenderGate) {

        glClearColor(x->color.r / 255., x->color.g / 255., x->color.b / 255., x->color.a / 255.);
        glClear(GL_COLOR_BUFFER_BIT);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaClearColor::objName);
    }
}

void ofeliaClearColor_r(t_ofeliaClearColor *x, t_floatarg f)
{
    x->color.r = static_cast<unsigned char>(f);
}

void ofeliaClearColor_g(t_ofeliaClearColor *x, t_floatarg f)
{
    x->color.g = static_cast<unsigned char>(f);
}

void ofeliaClearColor_b(t_ofeliaClearColor *x, t_floatarg f)
{
    x->color.b = static_cast<unsigned char>(f);
}

void ofeliaClearColor_a(t_ofeliaClearColor *x, t_floatarg f)
{
    x->color.a = static_cast<unsigned char>(f);
}

void ofeliaClearColor_set(t_ofeliaClearColor *x, t_symbol *s, int argc, t_atom *argv)
{
    getColorAlphaFromArgs(argc, argv, x->color, t_ofeliaClearColor::objName);
}

void ofeliaClearColor_print(t_ofeliaClearColor *x)
{
    post("\n[%s]", t_ofeliaClearColor::objName);
    post("r : %d", x->color.r);
    post("g : %d", x->color.g);
    post("b : %d", x->color.b);
    post("a : %d", x->color.a);
}

void ofeliaClearColor_setup()
{
    ofeliaClearColor_class = class_new(gensym("ofClearColor"),
                                       reinterpret_cast<t_newmethod>(ofeliaClearColor_new),
                                       0, sizeof(t_ofeliaClearColor),
                                       CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaClearColor_class, reinterpret_cast<t_method>(ofeliaClearColor_bang));
    class_addmethod(ofeliaClearColor_class, reinterpret_cast<t_method>(ofeliaClearColor_r),
                    gensym("r"), A_FLOAT, 0);
    class_addmethod(ofeliaClearColor_class, reinterpret_cast<t_method>(ofeliaClearColor_g),
                    gensym("g"), A_FLOAT, 0);
    class_addmethod(ofeliaClearColor_class, reinterpret_cast<t_method>(ofeliaClearColor_b),
                    gensym("b"), A_FLOAT, 0);
    class_addmethod(ofeliaClearColor_class, reinterpret_cast<t_method>(ofeliaClearColor_a),
                    gensym("a"), A_FLOAT, 0);
    class_addmethod(ofeliaClearColor_class, reinterpret_cast<t_method>(ofeliaClearColor_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaClearColor_class, reinterpret_cast<t_method>(ofeliaClearColor_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofClearDepth object methods
 */
void *ofeliaClearDepth_new()
{
    t_ofeliaClearDepth *x = reinterpret_cast<t_ofeliaClearDepth*>(pd_new(ofeliaClearDepth_class));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaClearDepth_bang(t_ofeliaClearDepth *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        glClear(GL_DEPTH_BUFFER_BIT);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaClearDepth::objName);
    }
}

void ofeliaClearDepth_setup()
{
    ofeliaClearDepth_class = class_new(gensym("ofClearDepth"),
                                       reinterpret_cast<t_newmethod>(ofeliaClearDepth_new),
                                       0, sizeof(t_ofeliaClearDepth),
                                       CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaClearDepth_class, reinterpret_cast<t_method>(ofeliaClearDepth_bang));
}

/* ________________________________________________________________________________
 * ofClearAlpha object methods
 */
void *ofeliaClearAlpha_new()
{
    t_ofeliaClearAlpha *x = reinterpret_cast<t_ofeliaClearAlpha*>(pd_new(ofeliaClearAlpha_class));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaClearAlpha_bang(t_ofeliaClearAlpha *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofGetCurrentRenderer()->clearAlpha();
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaClearAlpha::objName);
    }
}

void ofeliaClearAlpha_setup()
{
    ofeliaClearAlpha_class = class_new(gensym("ofClearAlpha"),
                                       reinterpret_cast<t_newmethod>(ofeliaClearAlpha_new),
                                       0, sizeof(t_ofeliaClearAlpha),
                                       CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaClearAlpha_class, reinterpret_cast<t_method>(ofeliaClearAlpha_bang));
}

/* ________________________________________________________________________________
 * setup methods
 */
void ofeliaSetters_setup()
{
    ofeliaSetDepthTest_setup();
    ofeliaSetArbTex_setup();
    ofeliaSetAntiAliasing_setup();
    ofeliaSetBgAuto_setup();
    ofeliaClear_setup();
    ofeliaClearColor_setup();
    ofeliaClearDepth_setup();
    ofeliaClearAlpha_setup();
}

