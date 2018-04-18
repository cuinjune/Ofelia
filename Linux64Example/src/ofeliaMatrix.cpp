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

#include "ofeliaMatrix.h"
#include "ofeliaOrien.h"
#include "ofeliaWindow.h"

/* ________________________________________________________________________________
 * static variables
 */
const char *t_ofeliaPushMatrix::objName = "ofPushMatrix";
const char *t_ofeliaPopMatrix::objName = "ofPopMatrix";
const char *t_ofeliaTranslate::objName = "ofTranslate";
const char *t_ofeliaRotateX::objName = "ofRotateX";
const char *t_ofeliaRotateY::objName = "ofRotateY";
const char *t_ofeliaRotateZ::objName = "ofRotateZ";
const char *t_ofeliaRotateXYZ::objName = "ofRotateXYZ";
const char *t_ofeliaRotate::objName = "ofRotate";
const char *t_ofeliaScale::objName = "ofScale";
const char *t_ofeliaGetTranslate::objName = "ofGetTranslate";
const char *t_ofeliaGetRotate::objName = "ofGetRotate";
const char *t_ofeliaGetScale::objName = "ofGetScale";

/* ________________________________________________________________________________
 * ofPushMatrix object methods
 */
void *ofeliaPushMatrix_new()
{
    t_ofeliaPushMatrix *x = reinterpret_cast<t_ofeliaPushMatrix*>(pd_new(ofeliaPushMatrix_class));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaPushMatrix_bang(t_ofeliaPushMatrix *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofGetCurrentRenderer()->pushMatrix();
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaPushMatrix::objName);
    }
}

void ofeliaPushMatrix_setup()
{
    ofeliaPushMatrix_class = class_new(gensym("ofPushMatrix"),
                                       reinterpret_cast<t_newmethod>(ofeliaPushMatrix_new),
                                       0, sizeof(t_ofeliaPushMatrix),
                                       CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaPushMatrix_class, reinterpret_cast<t_method>(ofeliaPushMatrix_bang));
}

/* ________________________________________________________________________________
 * ofPopMatrix object methods
 */
void *ofeliaPopMatrix_new()
{
    t_ofeliaPopMatrix *x = reinterpret_cast<t_ofeliaPopMatrix*>(pd_new(ofeliaPopMatrix_class));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaPopMatrix_bang(t_ofeliaPopMatrix *x)
{
    if (t_ofeliaWindow::bRenderGate) {
    
        ofGetCurrentRenderer()->popMatrix();
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaPopMatrix::objName);
    }
}

void ofeliaPopMatrix_setup()
{
    ofeliaPopMatrix_class = class_new(gensym("ofPopMatrix"),
                                      reinterpret_cast<t_newmethod>(ofeliaPopMatrix_new),
                                      0, sizeof(t_ofeliaPopMatrix),
                                      CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaPopMatrix_class, reinterpret_cast<t_method>(ofeliaPopMatrix_bang));
}

/* ________________________________________________________________________________
 * ofTranslate object methods
 */
bool getPositionFromArgs(const int argc, const t_atom *argv, ofVec3f &pos)
{
    switch (argc) {
            
        case 0:
        {
            pos.set(0.0f, 0.0f);
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT) {
                
                pos.set(argv[0].a_w.w_float, argv[1].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaTranslate::objName);
                return 0;
            }
            break;
        }
        case 3:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT) {
                
                pos.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaTranslate::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaTranslate::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaTranslate_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaTranslate *x = reinterpret_cast<t_ofeliaTranslate*>(pd_new(ofeliaTranslate_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getPositionFromArgs(argc, argv, x->pos))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("x"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("y"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("z"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaTranslate_bang(t_ofeliaTranslate *x)
{
    if (t_ofeliaWindow::bRenderGate) {

        ofGetCurrentRenderer()->translate(x->pos.x, x->pos.y, x->pos.z);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaTranslate::objName);
    }
}

void ofeliaTranslate_x(t_ofeliaTranslate *x, t_floatarg f)
{
    x->pos.x = f;
}

void ofeliaTranslate_y(t_ofeliaTranslate *x, t_floatarg f)
{
    x->pos.y = f;
}

void ofeliaTranslate_z(t_ofeliaTranslate *x, t_floatarg f)
{
    x->pos.z = f;
}

void ofeliaTranslate_set(t_ofeliaTranslate *x, t_symbol *s, int argc, t_atom *argv)
{
    getPositionFromArgs(argc, argv, x->pos);
}

void ofeliaTranslate_print(t_ofeliaTranslate *x)
{
    post("\n[%s]", t_ofeliaTranslate::objName);
    post("x : %g", x->pos.x);
    post("y : %g", x->pos.y);
    post("z : %g", x->pos.z);
}

void ofeliaTranslate_setup()
{
    ofeliaTranslate_class = class_new(gensym("ofTranslate"),
                                      reinterpret_cast<t_newmethod>(ofeliaTranslate_new),
                                      0, sizeof(t_ofeliaTranslate),
                                      CLASS_DEFAULT, A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaTranslate_new),
                     gensym("ofTrans"), A_GIMME, 0);
    class_addbang(ofeliaTranslate_class, reinterpret_cast<t_method>(ofeliaTranslate_bang));
    class_addmethod(ofeliaTranslate_class, reinterpret_cast<t_method>(ofeliaTranslate_x),
                    gensym("x"), A_FLOAT, 0);
    class_addmethod(ofeliaTranslate_class, reinterpret_cast<t_method>(ofeliaTranslate_y),
                    gensym("y"), A_FLOAT, 0);
    class_addmethod(ofeliaTranslate_class, reinterpret_cast<t_method>(ofeliaTranslate_z),
                    gensym("z"), A_FLOAT, 0);
    class_addmethod(ofeliaTranslate_class, reinterpret_cast<t_method>(ofeliaTranslate_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaTranslate_class, reinterpret_cast<t_method>(ofeliaTranslate_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * common methods
 */
bool getDegreeFromArgs(const int argc, const t_atom *argv, float &degree, const char *objName)
{
    switch (argc) {
            
        case 0:
        {
            degree = 0.0f;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                degree = argv[0].a_w.w_float;
            }
            else {
                
                error("%s: wrong argument type", objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", objName);
            return 0;
        }
    }
    return 1;
}

/* ________________________________________________________________________________
 * ofRotateX object methods
 */
void *ofeliaRotateX_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaRotateX *x = reinterpret_cast<t_ofeliaRotateX*>(pd_new(ofeliaRotateX_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getDegreeFromArgs(argc, argv, x->degree, t_ofeliaRotateX::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("x"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaRotateX_bang(t_ofeliaRotateX *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofGetCurrentRenderer()->rotateX(x->degree);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaRotateX::objName);
    }
}

void ofeliaRotateX_x(t_ofeliaRotateX *x, t_floatarg f)
{
    x->degree = f;
}

void ofeliaRotateX_set(t_ofeliaRotateX *x, t_symbol *s, int argc, t_atom *argv)
{
    getDegreeFromArgs(argc, argv, x->degree, t_ofeliaRotateX::objName);
}

void ofeliaRotateX_print(t_ofeliaRotateX *x)
{
    post("\n[%s]", t_ofeliaRotateX::objName);
    post("x : %g", x->degree);
}

void ofeliaRotateX_setup()
{
    ofeliaRotateX_class = class_new(gensym("ofRotateX"),
                                    reinterpret_cast<t_newmethod>(ofeliaRotateX_new),
                                    0, sizeof(t_ofeliaRotateX),
                                    CLASS_DEFAULT, A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaRotateX_new),
                     gensym("ofRotX"), A_GIMME, 0);
    class_addbang(ofeliaRotateX_class, reinterpret_cast<t_method>(ofeliaRotateX_bang));
    class_addmethod(ofeliaRotateX_class, reinterpret_cast<t_method>(ofeliaRotateX_x),
                    gensym("x"), A_FLOAT, 0);
    class_addmethod(ofeliaRotateX_class, reinterpret_cast<t_method>(ofeliaRotateX_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaRotateX_class, reinterpret_cast<t_method>(ofeliaRotateX_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofRotateY object methods
 */
void *ofeliaRotateY_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaRotateY *x = reinterpret_cast<t_ofeliaRotateY*>(pd_new(ofeliaRotateY_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getDegreeFromArgs(argc, argv, x->degree, t_ofeliaRotateY::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("y"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaRotateY_bang(t_ofeliaRotateY *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofGetCurrentRenderer()->rotateY(x->degree);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaRotateY::objName);
    }
}

void ofeliaRotateY_y(t_ofeliaRotateY *x, t_floatarg f)
{
    x->degree = f;
}

void ofeliaRotateY_set(t_ofeliaRotateY *x, t_symbol *s, int argc, t_atom *argv)
{
    getDegreeFromArgs(argc, argv, x->degree, t_ofeliaRotateY::objName);
}

void ofeliaRotateY_print(t_ofeliaRotateY *x)
{
    post("\n[%s]", t_ofeliaRotateY::objName);
    post("y : %g", x->degree);
}

void ofeliaRotateY_setup()
{
    ofeliaRotateY_class = class_new(gensym("ofRotateY"),
                                    reinterpret_cast<t_newmethod>(ofeliaRotateY_new),
                                    0, sizeof(t_ofeliaRotateY),
                                    CLASS_DEFAULT, A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaRotateY_new),
                     gensym("ofRotY"), A_GIMME, 0);
    class_addbang(ofeliaRotateY_class, reinterpret_cast<t_method>(ofeliaRotateY_bang));
    class_addmethod(ofeliaRotateY_class, reinterpret_cast<t_method>(ofeliaRotateY_y),
                    gensym("y"), A_FLOAT, 0);
    class_addmethod(ofeliaRotateY_class, reinterpret_cast<t_method>(ofeliaRotateY_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaRotateY_class, reinterpret_cast<t_method>(ofeliaRotateY_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofRotateZ object methods
 */
void *ofeliaRotateZ_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaRotateZ *x = reinterpret_cast<t_ofeliaRotateZ*>(pd_new(ofeliaRotateZ_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getDegreeFromArgs(argc, argv, x->degree, t_ofeliaRotateZ::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("z"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaRotateZ_bang(t_ofeliaRotateZ *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofGetCurrentRenderer()->rotateZ(x->degree);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaRotateZ::objName);
    }
}

void ofeliaRotateZ_z(t_ofeliaRotateZ *x, t_floatarg f)
{
    x->degree = f;
}

void ofeliaRotateZ_set(t_ofeliaRotateZ *x, t_symbol *s, int argc, t_atom *argv)
{
    getDegreeFromArgs(argc, argv, x->degree, t_ofeliaRotateZ::objName);
}

void ofeliaRotateZ_print(t_ofeliaRotateZ *x)
{
    post("\n[%s]", t_ofeliaRotateZ::objName);
    post("z : %g", x->degree);
}

void ofeliaRotateZ_setup()
{
    ofeliaRotateZ_class = class_new(gensym("ofRotateZ"),
                                    reinterpret_cast<t_newmethod>(ofeliaRotateZ_new),
                                    0, sizeof(t_ofeliaRotateZ),
                                    CLASS_DEFAULT, A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaRotateZ_new),
                     gensym("ofRotZ"), A_GIMME, 0);
    class_addbang(ofeliaRotateZ_class, reinterpret_cast<t_method>(ofeliaRotateZ_bang));
    class_addmethod(ofeliaRotateZ_class, reinterpret_cast<t_method>(ofeliaRotateZ_z),
                    gensym("z"), A_FLOAT, 0);
    class_addmethod(ofeliaRotateZ_class, reinterpret_cast<t_method>(ofeliaRotateZ_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaRotateZ_class, reinterpret_cast<t_method>(ofeliaRotateZ_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofRotateXYZ object methods
 */
bool getDegreesFromArgs(const int argc, const t_atom *argv, ofVec3f &degrees)
{
    switch (argc) {
            
        case 0:
        {
            degrees.set(0.0f, 0.0f);
            break;
        }
        case 3:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT) {
                
                degrees.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaRotateXYZ::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaRotateXYZ::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaRotateXYZ_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaRotateXYZ *x = reinterpret_cast<t_ofeliaRotateXYZ*>(pd_new(ofeliaRotateXYZ_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getDegreesFromArgs(argc, argv, x->degrees))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("x"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("y"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("z"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaRotateXYZ_bang(t_ofeliaRotateXYZ *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofGetCurrentRenderer()->rotateX(x->degrees.x);
        ofGetCurrentRenderer()->rotateY(x->degrees.y);
        ofGetCurrentRenderer()->rotateZ(x->degrees.z);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaRotateXYZ::objName);
    }
}

void ofeliaRotateXYZ_x(t_ofeliaRotateXYZ *x, t_floatarg f)
{
    x->degrees.x = f;
}

void ofeliaRotateXYZ_y(t_ofeliaRotateXYZ *x, t_floatarg f)
{
    x->degrees.y = f;
}

void ofeliaRotateXYZ_z(t_ofeliaRotateXYZ *x, t_floatarg f)
{
    x->degrees.z = f;
}

void ofeliaRotateXYZ_set(t_ofeliaRotateXYZ *x, t_symbol *s, int argc, t_atom *argv)
{
    getDegreesFromArgs(argc, argv, x->degrees);
}

void ofeliaRotateXYZ_print(t_ofeliaRotateXYZ *x)
{
    post("\n[%s]", t_ofeliaRotateXYZ::objName);
    post("x : %g", x->degrees.x);
    post("y : %g", x->degrees.y);
    post("z : %g", x->degrees.z);
}

void ofeliaRotateXYZ_setup()
{
    ofeliaRotateXYZ_class = class_new(gensym("ofRotateXYZ"),
                                      reinterpret_cast<t_newmethod>(ofeliaRotateXYZ_new),
                                      0, sizeof(t_ofeliaRotateXYZ),
                                      CLASS_DEFAULT, A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaRotateXYZ_new),
                     gensym("ofRotXYZ"), A_GIMME, 0);
    class_addbang(ofeliaRotateXYZ_class, reinterpret_cast<t_method>(ofeliaRotateXYZ_bang));
    class_addmethod(ofeliaRotateXYZ_class, reinterpret_cast<t_method>(ofeliaRotateXYZ_x),
                    gensym("x"), A_FLOAT, 0);
    class_addmethod(ofeliaRotateXYZ_class, reinterpret_cast<t_method>(ofeliaRotateXYZ_y),
                    gensym("y"), A_FLOAT, 0);
    class_addmethod(ofeliaRotateXYZ_class, reinterpret_cast<t_method>(ofeliaRotateXYZ_z),
                    gensym("z"), A_FLOAT, 0);
    class_addmethod(ofeliaRotateXYZ_class, reinterpret_cast<t_method>(ofeliaRotateXYZ_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaRotateXYZ_class, reinterpret_cast<t_method>(ofeliaRotateXYZ_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofRotate object methods
 */
void *ofeliaRotate_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaRotate *x = reinterpret_cast<t_ofeliaRotate*>(pd_new(ofeliaRotate_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getRotateElemFromArgs(argc, argv, x->elem, t_ofeliaRotate::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("angle"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("x"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("y"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("z"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaRotate_bang(t_ofeliaRotate *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofGetCurrentRenderer()->rotate(x->elem.angle, x->elem.axis.x, x->elem.axis.y, x->elem.axis.z);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaRotate::objName);
    }
}

void ofeliaRotate_angle(t_ofeliaRotate *x, t_floatarg f)
{
    x->elem.angle = f;
}

void ofeliaRotate_x(t_ofeliaRotate *x, t_floatarg f)
{
    x->elem.axis.x = f;
}

void ofeliaRotate_y(t_ofeliaRotate *x, t_floatarg f)
{
    x->elem.axis.y = f;
}

void ofeliaRotate_z(t_ofeliaRotate *x, t_floatarg f)
{
    x->elem.axis.z = f;
}

void ofeliaRotate_set(t_ofeliaRotate *x, t_symbol *s, int argc, t_atom *argv)
{
    getRotateElemFromArgs(argc, argv, x->elem, t_ofeliaRotate::objName);
}

void ofeliaRotate_print(t_ofeliaRotate *x)
{
    post("\n[%s]", t_ofeliaRotate::objName);
    post("angle : %g", x->elem.angle);
    post("x : %g", x->elem.axis.x);
    post("y : %g", x->elem.axis.y);
    post("z : %g", x->elem.axis.z);
}

void ofeliaRotate_setup()
{
    ofeliaRotate_class = class_new(gensym("ofRotate"),
                                       reinterpret_cast<t_newmethod>(ofeliaRotate_new),
                                       0, sizeof(t_ofeliaRotate),
                                       CLASS_DEFAULT, A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaRotate_new),
                     gensym("ofRot"), A_GIMME, 0);
    class_addbang(ofeliaRotate_class, reinterpret_cast<t_method>(ofeliaRotate_bang));
    class_addmethod(ofeliaRotate_class, reinterpret_cast<t_method>(ofeliaRotate_angle),
                    gensym("angle"), A_FLOAT, 0);
    class_addmethod(ofeliaRotate_class, reinterpret_cast<t_method>(ofeliaRotate_x),
                    gensym("x"), A_FLOAT, 0);
    class_addmethod(ofeliaRotate_class, reinterpret_cast<t_method>(ofeliaRotate_y),
                    gensym("y"), A_FLOAT, 0);
    class_addmethod(ofeliaRotate_class, reinterpret_cast<t_method>(ofeliaRotate_z),
                    gensym("z"), A_FLOAT, 0);
    class_addmethod(ofeliaRotate_class, reinterpret_cast<t_method>(ofeliaRotate_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaRotate_class, reinterpret_cast<t_method>(ofeliaRotate_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofScale object methods
 */
bool getScalesFromArgs(const int argc, const t_atom *argv, ofVec3f &scales)
{
    switch (argc) {
            
        case 0:
        {
            scales.set(1.0f, 1.0f, 1.0f);
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT) {
                
                scales.set(argv[0].a_w.w_float, argv[1].a_w.w_float, 1.0f);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaScale::objName);
                return 0;
            }
            break;
        }
        case 3:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT) {
                
                scales.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaScale::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaScale::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaScale_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaScale *x = reinterpret_cast<t_ofeliaScale*>(pd_new(ofeliaScale_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getScalesFromArgs(argc, argv, x->scales))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("x"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("y"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("z"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaScale_bang(t_ofeliaScale *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofGetCurrentRenderer()->scale(x->scales.x, x->scales.y, x->scales.z);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaScale::objName);
    }
}

void ofeliaScale_x(t_ofeliaScale *x, t_floatarg f)
{
    x->scales.x = f;
}

void ofeliaScale_y(t_ofeliaScale *x, t_floatarg f)
{
    x->scales.y = f;
}

void ofeliaScale_z(t_ofeliaScale *x, t_floatarg f)
{
    x->scales.z = f;
}

void ofeliaScale_set(t_ofeliaScale *x, t_symbol *s, int argc, t_atom *argv)
{
    getScalesFromArgs(argc, argv, x->scales);
}

void ofeliaScale_print(t_ofeliaScale *x)
{
    post("\n[%s]", t_ofeliaScale::objName);
    post("x : %g", x->scales.x);
    post("y : %g", x->scales.y);
    post("z : %g", x->scales.z);
}

void ofeliaScale_setup()
{
    ofeliaScale_class = class_new(gensym("ofScale"),
                                  reinterpret_cast<t_newmethod>(ofeliaScale_new),
                                  0, sizeof(t_ofeliaScale),
                                  CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaScale_class, reinterpret_cast<t_method>(ofeliaScale_bang));
    class_addmethod(ofeliaScale_class, reinterpret_cast<t_method>(ofeliaScale_x),
                    gensym("x"), A_FLOAT, 0);
    class_addmethod(ofeliaScale_class, reinterpret_cast<t_method>(ofeliaScale_y),
                    gensym("y"), A_FLOAT, 0);
    class_addmethod(ofeliaScale_class, reinterpret_cast<t_method>(ofeliaScale_z),
                    gensym("z"), A_FLOAT, 0);
    class_addmethod(ofeliaScale_class, reinterpret_cast<t_method>(ofeliaScale_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaScale_class, reinterpret_cast<t_method>(ofeliaScale_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetTranslate object methods
 */
void *ofeliaGetTranslate_new()
{
    t_ofeliaGetTranslate *x = reinterpret_cast<t_ofeliaGetTranslate*>(pd_new(ofeliaGetTranslate_class));
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetTranslate_bang(t_ofeliaGetTranslate *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const ofMatrix4x4 &modelMatrix = ofGetCurrentMatrix(OF_MATRIX_MODELVIEW) * ofGetCurrentViewMatrix().getInverse();
        const ofVec3f &translate = modelMatrix.getTranslation() / ofeliaWindow::scaleFactor;
        t_atom av[3];
        av[0].a_type = A_FLOAT;
        av[0].a_w.w_float = translate.x;
        av[1].a_type = A_FLOAT;
        av[1].a_w.w_float = translate.y;
        av[2].a_type = A_FLOAT;
        av[2].a_w.w_float = translate.z;
        outlet_list(x->x_obj.ob_outlet, &s_list, 3, av);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaGetTranslate::objName);
    }
}

void ofeliaGetTranslate_setup()
{
    ofeliaGetTranslate_class = class_new(gensym("ofGetTranslate"),
                                         reinterpret_cast<t_newmethod>(ofeliaGetTranslate_new),
                                         0, sizeof(t_ofeliaGetTranslate),
                                         CLASS_DEFAULT, A_NULL, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaGetTranslate_new),
                     gensym("ofGetTrans"), A_GIMME, 0);
    class_addbang(ofeliaGetTranslate_class, reinterpret_cast<t_method>(ofeliaGetTranslate_bang));
}

/* ________________________________________________________________________________
 * ofGetRotate object methods
 */
void *ofeliaGetRotate_new()
{
    t_ofeliaGetRotate *x = reinterpret_cast<t_ofeliaGetRotate*>(pd_new(ofeliaGetRotate_class));
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetRotate_bang(t_ofeliaGetRotate *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const ofMatrix4x4 &modelMatrix = ofGetCurrentMatrix(OF_MATRIX_MODELVIEW) * ofGetCurrentViewMatrix().getInverse();
        const ofQuaternion &rotate = modelMatrix.getRotate();
        float angle;
        ofVec3f axis;
        rotate.getRotate(angle, axis);
        t_atom av[4];
        av[0].a_type = A_FLOAT;
        av[0].a_w.w_float = angle;
        av[1].a_type = A_FLOAT;
        av[1].a_w.w_float = axis.x;
        av[2].a_type = A_FLOAT;
        av[2].a_w.w_float = axis.y;
        av[3].a_type = A_FLOAT;
        av[3].a_w.w_float = axis.z;
        outlet_list(x->x_obj.ob_outlet, &s_list, 4, av);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaGetRotate::objName);
    }
}

void ofeliaGetRotate_setup()
{
    ofeliaGetRotate_class = class_new(gensym("ofGetRotate"),
                                      reinterpret_cast<t_newmethod>(ofeliaGetRotate_new),
                                      0, sizeof(t_ofeliaGetRotate),
                                      CLASS_DEFAULT, A_NULL, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaGetRotate_new),
                     gensym("ofGetRot"), A_GIMME, 0);
    class_addbang(ofeliaGetRotate_class, reinterpret_cast<t_method>(ofeliaGetRotate_bang));
}

/* ________________________________________________________________________________
 * ofGetScale object methods
 */
void *ofeliaGetScale_new()
{
    t_ofeliaGetScale *x = reinterpret_cast<t_ofeliaGetScale*>(pd_new(ofeliaGetScale_class));
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetScale_bang(t_ofeliaGetScale *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const ofMatrix4x4 &modelMatrix = ofGetCurrentMatrix(OF_MATRIX_MODELVIEW) * ofGetCurrentViewMatrix().getInverse();
        ofVec3f scale = modelMatrix.getScale() / ofeliaWindow::scaleFactor;;
        t_atom av[3];
        av[0].a_type = A_FLOAT;
        av[0].a_w.w_float = scale.x;
        av[1].a_type = A_FLOAT;
        av[1].a_w.w_float = scale.y;
        av[2].a_type = A_FLOAT;
        av[2].a_w.w_float = scale.z;
        outlet_list(x->x_obj.ob_outlet, &s_list, 3, av);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaGetScale::objName);
    }
}

void ofeliaGetScale_setup()
{
    ofeliaGetScale_class = class_new(gensym("ofGetScale"),
                                     reinterpret_cast<t_newmethod>(ofeliaGetScale_new),
                                     0, sizeof(t_ofeliaGetScale),
                                     CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetScale_class, reinterpret_cast<t_method>(ofeliaGetScale_bang));
}

/* ________________________________________________________________________________
 * setup methods
 */
void ofeliaMatrix_setup()
{
    ofeliaPushMatrix_setup();
    ofeliaPopMatrix_setup();
    ofeliaTranslate_setup();
    ofeliaRotateX_setup();
    ofeliaRotateY_setup();
    ofeliaRotateZ_setup();
    ofeliaRotateXYZ_setup();
    ofeliaRotate_setup();
    ofeliaScale_setup();
    ofeliaGetTranslate_setup();
    ofeliaGetRotate_setup();
    ofeliaGetScale_setup();
}

