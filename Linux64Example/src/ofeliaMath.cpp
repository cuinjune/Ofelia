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

#include "ofeliaMath.h"

/* ________________________________________________________________________________
 * static variables
 */
const char *t_ofeliaAngleDifferenceDegrees::objName = "ofAngleDifferenceDegrees";
const char *t_ofeliaAngleDifferenceRadians::objName = "ofAngleDifferenceRadians";
const char *t_ofeliaDegToRad::objName = "ofDegToRad";
const char *t_ofeliaRadToDeg::objName = "ofRadToDeg";
const char *t_ofeliaDist2d::objName = "ofDist2d";
const char *t_ofeliaDist3d::objName = "ofDist3d";
const char *t_ofeliaDistSquared2d::objName = "ofDistSquared2d";
const char *t_ofeliaDistSquared3d::objName = "ofDistSquared3d";
const char *t_ofeliaInRange::objName = "ofInRange";
const char *t_ofeliaClamp::objName = "ofClamp";
const char *t_ofeliaNormalize::objName = "ofNormalize";
const char *t_ofeliaLerp::objName = "ofLerp";
const char *t_ofeliaLerpDegrees::objName = "ofLerpDegrees";
const char *t_ofeliaLerpRadians::objName = "ofLerpRadians";
const char *t_ofeliaRandom::objName = "ofRandom";
const char *t_ofeliaRandomf::objName = "ofRandomf";
const char *t_ofeliaRandomuf::objName = "ofRandomuf";
const char *t_ofeliaSeedRandom::objName = "ofSeedRandom";
const char *t_ofeliaWrap::objName = "ofWrap";
const char *t_ofeliaWrapDegrees::objName = "ofWrapDegrees";
const char *t_ofeliaWrapRadians::objName = "ofWrapRadians";
const char *t_ofeliaMap::objName = "ofMap";
const char *t_ofeliaNextPow2::objName = "ofNextPow2";
const char *t_ofeliaNoise::objName = "ofNoise";
const char *t_ofeliaSignedNoise::objName = "ofSignedNoise";
const char *t_ofeliaSign::objName = "ofSign";

/* ________________________________________________________________________________
 * ofAngleDifferenceDegrees object methods
 */
void *ofeliaAngleDifferenceDegrees_new(t_floatarg f)
{
    t_ofeliaAngleDifferenceDegrees *x = reinterpret_cast<t_ofeliaAngleDifferenceDegrees*>(pd_new(ofeliaAngleDifferenceDegrees_class));
    x->currentAngle = f;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("currentAngle"));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaAngleDifferenceDegrees_float(t_ofeliaAngleDifferenceDegrees *x, t_floatarg f)
{
    outlet_float(x->x_obj.ob_outlet, ofAngleDifferenceDegrees(x->currentAngle, f));
}

void ofeliaAngleDifferenceDegrees_set(t_ofeliaAngleDifferenceDegrees *x, t_floatarg f)
{
    x->currentAngle = f;
}

void ofeliaAngleDifferenceDegrees_print(t_ofeliaAngleDifferenceDegrees *x)
{
    post("\n[%s]", t_ofeliaAngleDifferenceDegrees::objName);
    post("currentAngle : %g", x->currentAngle);
}

void ofeliaAngleDifferenceDegrees_setup()
{
    ofeliaAngleDifferenceDegrees_class = class_new(gensym("ofAngleDifferenceDegrees"),
                                                   reinterpret_cast<t_newmethod>(ofeliaAngleDifferenceDegrees_new),
                                                   0, sizeof(t_ofeliaAngleDifferenceDegrees),
                                                   CLASS_DEFAULT, A_DEFFLOAT, 0);
    class_addfloat(ofeliaAngleDifferenceDegrees_class, reinterpret_cast<t_method>(ofeliaAngleDifferenceDegrees_float));
    class_addmethod(ofeliaAngleDifferenceDegrees_class, reinterpret_cast<t_method>(ofeliaAngleDifferenceDegrees_set),
                    gensym("currentAngle"), A_FLOAT, 0);
    class_addmethod(ofeliaAngleDifferenceDegrees_class, reinterpret_cast<t_method>(ofeliaAngleDifferenceDegrees_set),
                    gensym("set"), A_FLOAT, 0);
    class_addmethod(ofeliaAngleDifferenceDegrees_class, reinterpret_cast<t_method>(ofeliaAngleDifferenceDegrees_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofAngleDifferenceRadians object methods
 */
void *ofeliaAngleDifferenceRadians_new(t_floatarg f)
{
    t_ofeliaAngleDifferenceRadians *x = reinterpret_cast<t_ofeliaAngleDifferenceRadians*>(pd_new(ofeliaAngleDifferenceRadians_class));
    x->currentAngle = f;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("currentAngle"));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaAngleDifferenceRadians_float(t_ofeliaAngleDifferenceRadians *x, t_floatarg f)
{
    outlet_float(x->x_obj.ob_outlet, ofAngleDifferenceRadians(x->currentAngle, f));
}

void ofeliaAngleDifferenceRadians_set(t_ofeliaAngleDifferenceRadians *x, t_floatarg f)
{
    x->currentAngle = f;
}

void ofeliaAngleDifferenceRadians_print(t_ofeliaAngleDifferenceRadians *x)
{
    post("\n[%s]", t_ofeliaAngleDifferenceRadians::objName);
    post("currentAngle : %g", x->currentAngle);
}

void ofeliaAngleDifferenceRadians_setup()
{
    ofeliaAngleDifferenceRadians_class = class_new(gensym("ofAngleDifferenceRadians"),
                                                   reinterpret_cast<t_newmethod>(ofeliaAngleDifferenceRadians_new),
                                                   0, sizeof(t_ofeliaAngleDifferenceRadians),
                                                   CLASS_DEFAULT, A_DEFFLOAT, 0);
    class_addfloat(ofeliaAngleDifferenceRadians_class, reinterpret_cast<t_method>(ofeliaAngleDifferenceRadians_float));
    class_addmethod(ofeliaAngleDifferenceRadians_class, reinterpret_cast<t_method>(ofeliaAngleDifferenceRadians_set),
                    gensym("currentAngle"), A_FLOAT, 0);
    class_addmethod(ofeliaAngleDifferenceRadians_class, reinterpret_cast<t_method>(ofeliaAngleDifferenceRadians_set),
                    gensym("set"), A_FLOAT, 0);
    class_addmethod(ofeliaAngleDifferenceRadians_class, reinterpret_cast<t_method>(ofeliaAngleDifferenceRadians_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofDegToRad object methods
 */
void *ofeliaDegToRad_new()
{
    t_ofeliaDegToRad *x = reinterpret_cast<t_ofeliaDegToRad*>(pd_new(ofeliaDegToRad_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaDegToRad_float(t_ofeliaDegToRad *x, t_floatarg f)
{
    outlet_float(x->x_obj.ob_outlet, ofDegToRad(f));
}

void ofeliaDegToRad_setup()
{
    ofeliaDegToRad_class = class_new(gensym("ofDegToRad"),
                                     reinterpret_cast<t_newmethod>(ofeliaDegToRad_new),
                                     0, sizeof(t_ofeliaDegToRad),
                                     CLASS_DEFAULT, A_NULL, 0);
    class_addfloat(ofeliaDegToRad_class, reinterpret_cast<t_method>(ofeliaDegToRad_float));
}

/* ________________________________________________________________________________
 * ofRadToDeg object methods
 */
void *ofeliaRadToDeg_new()
{
    t_ofeliaRadToDeg *x = reinterpret_cast<t_ofeliaRadToDeg*>(pd_new(ofeliaRadToDeg_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaRadToDeg_float(t_ofeliaRadToDeg *x, t_floatarg f)
{
    outlet_float(x->x_obj.ob_outlet, ofRadToDeg(f));
}

void ofeliaRadToDeg_setup()
{
    ofeliaRadToDeg_class = class_new(gensym("ofRadToDeg"),
                                     reinterpret_cast<t_newmethod>(ofeliaRadToDeg_new),
                                     0, sizeof(t_ofeliaRadToDeg),
                                     CLASS_DEFAULT, A_NULL, 0);
    class_addfloat(ofeliaRadToDeg_class, reinterpret_cast<t_method>(ofeliaRadToDeg_float));
}

/* ________________________________________________________________________________
 * ofDist2d object methods
 */
bool getDist2dPointsFromArgs(const int argc, const t_atom *argv, ofVec2f &p1)
{
    switch (argc) {
            
        case 0:
        {
            p1.set(0.0f, 0.0f);
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT) {
                
                p1.set(argv[0].a_w.w_float, argv[1].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaDist2d::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaDist2d::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaDist2d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaDist2d *x = reinterpret_cast<t_ofeliaDist2d*>(pd_new(ofeliaDist2d_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getDist2dPointsFromArgs(argc, argv, x->p1))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p1"));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaDist2d_list(t_ofeliaDist2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            outlet_float(x->x_obj.ob_outlet, ofDist(x->p1.x, x->p1.y,
                                                    argv[0].a_w.w_float, argv[1].a_w.w_float));
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaDist2d::objName);
    }
}

void ofeliaDist2d_set(t_ofeliaDist2d *x, t_symbol *s, int argc, t_atom *argv)
{
    getDist2dPointsFromArgs(argc, argv, x->p1);
}

void ofeliaDist2d_print(t_ofeliaDist2d *x)
{
    post("\n[%s]", t_ofeliaDist2d::objName);
    post("p1 : %g %g", x->p1.x, x->p1.y);
}

void ofeliaDist2d_setup()
{
    ofeliaDist2d_class = class_new(gensym("ofDist2d"),
                                   reinterpret_cast<t_newmethod>(ofeliaDist2d_new),
                                   0, sizeof(t_ofeliaDist2d),
                                   CLASS_DEFAULT, A_GIMME, 0);
    class_addlist(ofeliaDist2d_class, reinterpret_cast<t_method>(ofeliaDist2d_list));
    class_addmethod(ofeliaDist2d_class, reinterpret_cast<t_method>(ofeliaDist2d_set),
                    gensym("p1"), A_GIMME, 0);
    class_addmethod(ofeliaDist2d_class, reinterpret_cast<t_method>(ofeliaDist2d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaDist2d_class, reinterpret_cast<t_method>(ofeliaDist2d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofDist3d object methods
 */
bool getDist3dPointsFromArgs(const int argc, const t_atom *argv, ofVec3f &p1)
{
    switch (argc) {
            
        case 0:
        {
            p1.set(0.0f, 0.0f, 0.0f);
            break;
        }
        case 3:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT) {
                
                p1.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaDist3d::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaDist3d::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaDist3d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaDist3d *x = reinterpret_cast<t_ofeliaDist3d*>(pd_new(ofeliaDist3d_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getDist3dPointsFromArgs(argc, argv, x->p1))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p1"));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaDist3d_list(t_ofeliaDist3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            outlet_float(x->x_obj.ob_outlet, ofDist(x->p1.x, x->p1.y, x->p1.z,
                                                    argv[0].a_w.w_float,
                                                    argv[1].a_w.w_float,
                                                    argv[2].a_w.w_float));
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaDist3d::objName);
    }
}

void ofeliaDist3d_set(t_ofeliaDist3d *x, t_symbol *s, int argc, t_atom *argv)
{
    getDist3dPointsFromArgs(argc, argv, x->p1);
}

void ofeliaDist3d_print(t_ofeliaDist3d *x)
{
    post("\n[%s]", t_ofeliaDist3d::objName);
    post("p1 : %g %g %g", x->p1.x, x->p1.y, x->p1.z);
}

void ofeliaDist3d_setup()
{
    ofeliaDist3d_class = class_new(gensym("ofDist3d"),
                                   reinterpret_cast<t_newmethod>(ofeliaDist3d_new),
                                   0, sizeof(t_ofeliaDist3d),
                                   CLASS_DEFAULT, A_GIMME, 0);
    class_addlist(ofeliaDist3d_class, reinterpret_cast<t_method>(ofeliaDist3d_list));
    class_addmethod(ofeliaDist3d_class, reinterpret_cast<t_method>(ofeliaDist3d_set),
                    gensym("p1"), A_GIMME, 0);
    class_addmethod(ofeliaDist3d_class, reinterpret_cast<t_method>(ofeliaDist3d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaDist3d_class, reinterpret_cast<t_method>(ofeliaDist3d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofDistSquared2d object methods
 */
bool getDistSquared2dPointsFromArgs(const int argc, const t_atom *argv, ofVec2f &p1)
{
    switch (argc) {
            
        case 0:
        {
            p1.set(0.0f, 0.0f);
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT) {
                
                p1.set(argv[0].a_w.w_float, argv[1].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaDistSquared2d::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaDistSquared2d::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaDistSquared2d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaDistSquared2d *x = reinterpret_cast<t_ofeliaDistSquared2d*>(pd_new(ofeliaDistSquared2d_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getDistSquared2dPointsFromArgs(argc, argv, x->p1))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p1"));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaDistSquared2d_list(t_ofeliaDistSquared2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            outlet_float(x->x_obj.ob_outlet, ofDistSquared(x->p1.x, x->p1.y,
                                                           argv[0].a_w.w_float, argv[1].a_w.w_float));
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaDistSquared2d::objName);
    }
}

void ofeliaDistSquared2d_set(t_ofeliaDistSquared2d *x, t_symbol *s, int argc, t_atom *argv)
{
    getDistSquared2dPointsFromArgs(argc, argv, x->p1);
}

void ofeliaDistSquared2d_print(t_ofeliaDistSquared2d *x)
{
    post("\n[%s]", t_ofeliaDistSquared2d::objName);
    post("p1 : %g %g", x->p1.x, x->p1.y);
}

void ofeliaDistSquared2d_setup()
{
    ofeliaDistSquared2d_class = class_new(gensym("ofDistSquared2d"),
                                          reinterpret_cast<t_newmethod>(ofeliaDistSquared2d_new),
                                          0, sizeof(t_ofeliaDistSquared2d),
                                          CLASS_DEFAULT, A_GIMME, 0);
    class_addlist(ofeliaDistSquared2d_class, reinterpret_cast<t_method>(ofeliaDistSquared2d_list));
    class_addmethod(ofeliaDistSquared2d_class, reinterpret_cast<t_method>(ofeliaDistSquared2d_set),
                    gensym("p1"), A_GIMME, 0);
    class_addmethod(ofeliaDistSquared2d_class, reinterpret_cast<t_method>(ofeliaDistSquared2d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaDistSquared2d_class, reinterpret_cast<t_method>(ofeliaDistSquared2d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofDistSquared3d object methods
 */
bool getDistSquared3dPointsFromArgs(const int argc, const t_atom *argv, ofVec3f &p1)
{
    switch (argc) {
            
        case 0:
        {
            p1.set(0.0f, 0.0f, 0.0f);
            break;
        }
        case 3:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT) {
                
                p1.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaDistSquared3d::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaDistSquared3d::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaDistSquared3d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaDistSquared3d *x = reinterpret_cast<t_ofeliaDistSquared3d*>(pd_new(ofeliaDistSquared3d_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getDistSquared3dPointsFromArgs(argc, argv, x->p1))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p1"));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaDistSquared3d_list(t_ofeliaDistSquared3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            outlet_float(x->x_obj.ob_outlet, ofDistSquared(x->p1.x, x->p1.y, x->p1.z,
                                                           argv[0].a_w.w_float,
                                                           argv[1].a_w.w_float,
                                                           argv[2].a_w.w_float));
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaDistSquared3d::objName);
    }
}

void ofeliaDistSquared3d_set(t_ofeliaDistSquared3d *x, t_symbol *s, int argc, t_atom *argv)
{
    getDistSquared3dPointsFromArgs(argc, argv, x->p1);
}

void ofeliaDistSquared3d_print(t_ofeliaDistSquared3d *x)
{
    post("\n[%s]", t_ofeliaDistSquared3d::objName);
    post("p1 : %g %g %g", x->p1.x, x->p1.y, x->p1.z);
}

void ofeliaDistSquared3d_setup()
{
    ofeliaDistSquared3d_class = class_new(gensym("ofDistSquared3d"),
                                          reinterpret_cast<t_newmethod>(ofeliaDistSquared3d_new),
                                          0, sizeof(t_ofeliaDistSquared3d),
                                          CLASS_DEFAULT, A_GIMME, 0);
    class_addlist(ofeliaDistSquared3d_class, reinterpret_cast<t_method>(ofeliaDistSquared3d_list));
    class_addmethod(ofeliaDistSquared3d_class, reinterpret_cast<t_method>(ofeliaDistSquared3d_set),
                    gensym("p1"), A_GIMME, 0);
    class_addmethod(ofeliaDistSquared3d_class, reinterpret_cast<t_method>(ofeliaDistSquared3d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaDistSquared3d_class, reinterpret_cast<t_method>(ofeliaDistSquared3d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * common methods
 */
bool getRangeElemFromArgs(const int argc, const t_atom *argv, t_ofeliaRangeElem &elem, const char *objName)
{
    switch (argc) {
            
        case 0:
        {
            elem.min = 0.0f;
            elem.max = 1.0f;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                elem.min = 0.0f;
                elem.max = argv[0].a_w.w_float;
            }
            else {
                
                error("%s: wrong argument type", objName);
                return 0;
            }
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT) {
                
                elem.min = argv[0].a_w.w_float;
                elem.max = argv[1].a_w.w_float;
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
 * ofInRange object methods
 */
void *ofeliaInRange_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaInRange *x = reinterpret_cast<t_ofeliaInRange*>(pd_new(ofeliaInRange_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getRangeElemFromArgs(argc, argv, x->elem, t_ofeliaInRange::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("min"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("max"));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaInRange_float(t_ofeliaInRange *x, t_floatarg f)
{
    outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(ofInRange(f, x->elem.min, x->elem.max)));
}

void ofeliaInRange_min(t_ofeliaInRange *x, t_floatarg f)
{
    x->elem.min = f;
}

void ofeliaInRange_max(t_ofeliaInRange *x, t_floatarg f)
{
    x->elem.max = f;
}

void ofeliaInRange_set(t_ofeliaInRange *x, t_symbol *s, int argc, t_atom *argv)
{
    getRangeElemFromArgs(argc, argv, x->elem, t_ofeliaInRange::objName);
}

void ofeliaInRange_print(t_ofeliaInRange *x)
{
    post("\n[%s]", t_ofeliaInRange::objName);
    post("min : %g", x->elem.min);
    post("max : %g", x->elem.max);
}

void ofeliaInRange_setup()
{
    ofeliaInRange_class = class_new(gensym("ofInRange"),
                                    reinterpret_cast<t_newmethod>(ofeliaInRange_new),
                                    0, sizeof(t_ofeliaInRange),
                                    CLASS_DEFAULT, A_GIMME, 0);
    class_addfloat(ofeliaInRange_class, reinterpret_cast<t_method>(ofeliaInRange_float));
    class_addmethod(ofeliaInRange_class, reinterpret_cast<t_method>(ofeliaInRange_min),
                    gensym("min"), A_FLOAT, 0);
    class_addmethod(ofeliaInRange_class, reinterpret_cast<t_method>(ofeliaInRange_max),
                    gensym("max"), A_FLOAT, 0);
    class_addmethod(ofeliaInRange_class, reinterpret_cast<t_method>(ofeliaInRange_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaInRange_class, reinterpret_cast<t_method>(ofeliaInRange_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofClamp object methods
 */
void *ofeliaClamp_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaClamp *x = reinterpret_cast<t_ofeliaClamp*>(pd_new(ofeliaClamp_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getRangeElemFromArgs(argc, argv, x->elem, t_ofeliaClamp::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("min"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("max"));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaClamp_float(t_ofeliaClamp *x, t_floatarg f)
{
    outlet_float(x->x_obj.ob_outlet, ofClamp(f, x->elem.min, x->elem.max));
}

void ofeliaClamp_min(t_ofeliaClamp *x, t_floatarg f)
{
    x->elem.min = f;
}

void ofeliaClamp_max(t_ofeliaClamp *x, t_floatarg f)
{
    x->elem.max = f;
}

void ofeliaClamp_set(t_ofeliaClamp *x, t_symbol *s, int argc, t_atom *argv)
{
    getRangeElemFromArgs(argc, argv, x->elem, t_ofeliaClamp::objName);
}

void ofeliaClamp_print(t_ofeliaClamp *x)
{
    post("\n[%s]", t_ofeliaClamp::objName);
    post("min : %g", x->elem.min);
    post("max : %g", x->elem.max);
}

void ofeliaClamp_setup()
{
    ofeliaClamp_class = class_new(gensym("ofClamp"),
                                  reinterpret_cast<t_newmethod>(ofeliaClamp_new),
                                  0, sizeof(t_ofeliaClamp),
                                  CLASS_DEFAULT, A_GIMME, 0);
    class_addfloat(ofeliaClamp_class, reinterpret_cast<t_method>(ofeliaClamp_float));
    class_addmethod(ofeliaClamp_class, reinterpret_cast<t_method>(ofeliaClamp_min),
                    gensym("min"), A_FLOAT, 0);
    class_addmethod(ofeliaClamp_class, reinterpret_cast<t_method>(ofeliaClamp_max),
                    gensym("max"), A_FLOAT, 0);
    class_addmethod(ofeliaClamp_class, reinterpret_cast<t_method>(ofeliaClamp_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaClamp_class, reinterpret_cast<t_method>(ofeliaClamp_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofNormalize object methods
 */
void *ofeliaNormalize_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaNormalize *x = reinterpret_cast<t_ofeliaNormalize*>(pd_new(ofeliaNormalize_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getRangeElemFromArgs(argc, argv, x->elem, t_ofeliaNormalize::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("min"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("max"));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaNormalize_float(t_ofeliaNormalize *x, t_floatarg f)
{
    outlet_float(x->x_obj.ob_outlet, ofNormalize(f, x->elem.min, x->elem.max));
}

void ofeliaNormalize_min(t_ofeliaNormalize *x, t_floatarg f)
{
    x->elem.min = f;
}

void ofeliaNormalize_max(t_ofeliaNormalize *x, t_floatarg f)
{
    x->elem.max = f;
}

void ofeliaNormalize_set(t_ofeliaNormalize *x, t_symbol *s, int argc, t_atom *argv)
{
    getRangeElemFromArgs(argc, argv, x->elem, t_ofeliaNormalize::objName);
}

void ofeliaNormalize_print(t_ofeliaNormalize *x)
{
    post("\n[%s]", t_ofeliaNormalize::objName);
    post("min : %g", x->elem.min);
    post("max : %g", x->elem.max);
}

void ofeliaNormalize_setup()
{
    ofeliaNormalize_class = class_new(gensym("ofNormalize"),
                                      reinterpret_cast<t_newmethod>(ofeliaNormalize_new),
                                      0, sizeof(t_ofeliaNormalize),
                                      CLASS_DEFAULT, A_GIMME, 0);
    class_addfloat(ofeliaNormalize_class, reinterpret_cast<t_method>(ofeliaNormalize_float));
    class_addmethod(ofeliaNormalize_class, reinterpret_cast<t_method>(ofeliaNormalize_min),
                    gensym("min"), A_FLOAT, 0);
    class_addmethod(ofeliaNormalize_class, reinterpret_cast<t_method>(ofeliaNormalize_max),
                    gensym("max"), A_FLOAT, 0);
    class_addmethod(ofeliaNormalize_class, reinterpret_cast<t_method>(ofeliaNormalize_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaNormalize_class, reinterpret_cast<t_method>(ofeliaNormalize_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofLerp object methods
 */
void *ofeliaLerp_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLerp *x = reinterpret_cast<t_ofeliaLerp*>(pd_new(ofeliaLerp_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getRangeElemFromArgs(argc, argv, x->elem, t_ofeliaLerp::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("min"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("max"));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaLerp_float(t_ofeliaLerp *x, t_floatarg f)
{
    outlet_float(x->x_obj.ob_outlet, ofLerp(x->elem.min, x->elem.max, f));
}

void ofeliaLerp_min(t_ofeliaLerp *x, t_floatarg f)
{
    x->elem.min = f;
}

void ofeliaLerp_max(t_ofeliaLerp *x, t_floatarg f)
{
    x->elem.max = f;
}

void ofeliaLerp_set(t_ofeliaLerp *x, t_symbol *s, int argc, t_atom *argv)
{
    getRangeElemFromArgs(argc, argv, x->elem, t_ofeliaLerp::objName);
}

void ofeliaLerp_print(t_ofeliaLerp *x)
{
    post("\n[%s]", t_ofeliaLerp::objName);
    post("min : %g", x->elem.min);
    post("max : %g", x->elem.max);
}

void ofeliaLerp_setup()
{
    ofeliaLerp_class = class_new(gensym("ofLerp"),
                                 reinterpret_cast<t_newmethod>(ofeliaLerp_new),
                                 0, sizeof(t_ofeliaLerp),
                                 CLASS_DEFAULT, A_GIMME, 0);
    class_addfloat(ofeliaLerp_class, reinterpret_cast<t_method>(ofeliaLerp_float));
    class_addmethod(ofeliaLerp_class, reinterpret_cast<t_method>(ofeliaLerp_min),
                    gensym("min"), A_FLOAT, 0);
    class_addmethod(ofeliaLerp_class, reinterpret_cast<t_method>(ofeliaLerp_max),
                    gensym("max"), A_FLOAT, 0);
    class_addmethod(ofeliaLerp_class, reinterpret_cast<t_method>(ofeliaLerp_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaLerp_class, reinterpret_cast<t_method>(ofeliaLerp_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofLerpDegrees object methods
 */
void *ofeliaLerpDegrees_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLerpDegrees *x = reinterpret_cast<t_ofeliaLerpDegrees*>(pd_new(ofeliaLerpDegrees_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getRangeElemFromArgs(argc, argv, x->elem, t_ofeliaLerpDegrees::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("min"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("max"));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaLerpDegrees_float(t_ofeliaLerpDegrees *x, t_floatarg f)
{
    outlet_float(x->x_obj.ob_outlet, ofLerpDegrees(x->elem.min, x->elem.max, f));
}

void ofeliaLerpDegrees_min(t_ofeliaLerpDegrees *x, t_floatarg f)
{
    x->elem.min = f;
}

void ofeliaLerpDegrees_max(t_ofeliaLerpDegrees *x, t_floatarg f)
{
    x->elem.max = f;
}

void ofeliaLerpDegrees_set(t_ofeliaLerpDegrees *x, t_symbol *s, int argc, t_atom *argv)
{
    getRangeElemFromArgs(argc, argv, x->elem, t_ofeliaLerpDegrees::objName);
}

void ofeliaLerpDegrees_print(t_ofeliaLerpDegrees *x)
{
    post("\n[%s]", t_ofeliaLerpDegrees::objName);
    post("min : %g", x->elem.min);
    post("max : %g", x->elem.max);
}

void ofeliaLerpDegrees_setup()
{
    ofeliaLerpDegrees_class = class_new(gensym("ofLerpDegrees"),
                                        reinterpret_cast<t_newmethod>(ofeliaLerpDegrees_new),
                                        0, sizeof(t_ofeliaLerpDegrees),
                                        CLASS_DEFAULT, A_GIMME, 0);
    class_addfloat(ofeliaLerpDegrees_class, reinterpret_cast<t_method>(ofeliaLerpDegrees_float));
    class_addmethod(ofeliaLerpDegrees_class, reinterpret_cast<t_method>(ofeliaLerpDegrees_min),
                    gensym("min"), A_FLOAT, 0);
    class_addmethod(ofeliaLerpDegrees_class, reinterpret_cast<t_method>(ofeliaLerpDegrees_max),
                    gensym("max"), A_FLOAT, 0);
    class_addmethod(ofeliaLerpDegrees_class, reinterpret_cast<t_method>(ofeliaLerpDegrees_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaLerpDegrees_class, reinterpret_cast<t_method>(ofeliaLerpDegrees_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofLerpRadians object methods
 */
void *ofeliaLerpRadians_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLerpRadians *x = reinterpret_cast<t_ofeliaLerpRadians*>(pd_new(ofeliaLerpRadians_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getRangeElemFromArgs(argc, argv, x->elem, t_ofeliaLerpRadians::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("min"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("max"));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaLerpRadians_float(t_ofeliaLerpRadians *x, t_floatarg f)
{
    outlet_float(x->x_obj.ob_outlet, ofLerpRadians(x->elem.min, x->elem.max, f));
}

void ofeliaLerpRadians_min(t_ofeliaLerpRadians *x, t_floatarg f)
{
    x->elem.min = f;
}

void ofeliaLerpRadians_max(t_ofeliaLerpRadians *x, t_floatarg f)
{
    x->elem.max = f;
}

void ofeliaLerpRadians_set(t_ofeliaLerpRadians *x, t_symbol *s, int argc, t_atom *argv)
{
    getRangeElemFromArgs(argc, argv, x->elem, t_ofeliaLerpRadians::objName);
}

void ofeliaLerpRadians_print(t_ofeliaLerpRadians *x)
{
    post("\n[%s]", t_ofeliaLerpRadians::objName);
    post("min : %g", x->elem.min);
    post("max : %g", x->elem.max);
}

void ofeliaLerpRadians_setup()
{
    ofeliaLerpRadians_class = class_new(gensym("ofLerpRadians"),
                                        reinterpret_cast<t_newmethod>(ofeliaLerpRadians_new),
                                        0, sizeof(t_ofeliaLerpRadians),
                                        CLASS_DEFAULT, A_GIMME, 0);
    class_addfloat(ofeliaLerpRadians_class, reinterpret_cast<t_method>(ofeliaLerpRadians_float));
    class_addmethod(ofeliaLerpRadians_class, reinterpret_cast<t_method>(ofeliaLerpRadians_min),
                    gensym("min"), A_FLOAT, 0);
    class_addmethod(ofeliaLerpRadians_class, reinterpret_cast<t_method>(ofeliaLerpRadians_max),
                    gensym("max"), A_FLOAT, 0);
    class_addmethod(ofeliaLerpRadians_class, reinterpret_cast<t_method>(ofeliaLerpRadians_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaLerpRadians_class, reinterpret_cast<t_method>(ofeliaLerpRadians_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofRandom object methods
 */
void *ofeliaRandom_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaRandom *x = reinterpret_cast<t_ofeliaRandom*>(pd_new(ofeliaRandom_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getRangeElemFromArgs(argc, argv, x->elem, t_ofeliaRandom::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("min"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("max"));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaRandom_bang(t_ofeliaRandom *x)
{
    outlet_float(x->x_obj.ob_outlet, ofRandom(x->elem.min, x->elem.max));
}

void ofeliaRandom_min(t_ofeliaRandom *x, t_floatarg f)
{
    x->elem.min = f;
}

void ofeliaRandom_max(t_ofeliaRandom *x, t_floatarg f)
{
    x->elem.max = f;
}

void ofeliaRandom_set(t_ofeliaRandom *x, t_symbol *s, int argc, t_atom *argv)
{
    getRangeElemFromArgs(argc, argv, x->elem, t_ofeliaRandom::objName);
}

void ofeliaRandom_print(t_ofeliaRandom *x)
{
    post("\n[%s]", t_ofeliaRandom::objName);
    post("min : %g", x->elem.min);
    post("max : %g", x->elem.max);
}

void ofeliaRandom_setup()
{
    ofeliaRandom_class = class_new(gensym("ofRandom"),
                                   reinterpret_cast<t_newmethod>(ofeliaRandom_new),
                                   0, sizeof(t_ofeliaRandom),
                                   CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaRandom_class, reinterpret_cast<t_method>(ofeliaRandom_bang));
    class_addmethod(ofeliaRandom_class, reinterpret_cast<t_method>(ofeliaRandom_min),
                    gensym("min"), A_FLOAT, 0);
    class_addmethod(ofeliaRandom_class, reinterpret_cast<t_method>(ofeliaRandom_max),
                    gensym("max"), A_FLOAT, 0);
    class_addmethod(ofeliaRandom_class, reinterpret_cast<t_method>(ofeliaRandom_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaRandom_class, reinterpret_cast<t_method>(ofeliaRandom_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofRandomf object methods
 */
void *ofeliaRandomf_new()
{
    t_ofeliaRandomf *x = reinterpret_cast<t_ofeliaRandomf*>(pd_new(ofeliaRandomf_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaRandomf_bang(t_ofeliaRandomf *x)
{
    outlet_float(x->x_obj.ob_outlet, ofRandomf());
}

void ofeliaRandomf_setup()
{
    ofeliaRandomf_class = class_new(gensym("ofRandomf"),
                                    reinterpret_cast<t_newmethod>(ofeliaRandomf_new),
                                    0, sizeof(t_ofeliaRandomf),
                                    CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaRandomf_class, reinterpret_cast<t_method>(ofeliaRandomf_bang));
}

/* ________________________________________________________________________________
 * ofRandomuf object methods
 */
void *ofeliaRandomuf_new()
{
    t_ofeliaRandomuf *x = reinterpret_cast<t_ofeliaRandomuf*>(pd_new(ofeliaRandomuf_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaRandomuf_bang(t_ofeliaRandomuf *x)
{
    outlet_float(x->x_obj.ob_outlet, ofRandomuf());
}

void ofeliaRandomuf_setup()
{
    ofeliaRandomuf_class = class_new(gensym("ofRandomuf"),
                                     reinterpret_cast<t_newmethod>(ofeliaRandomuf_new),
                                     0, sizeof(t_ofeliaRandomuf),
                                     CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaRandomuf_class, reinterpret_cast<t_method>(ofeliaRandomuf_bang));
}

/* ________________________________________________________________________________
 * ofSeedRandom object methods
 */
void *ofeliaSeedRandom_new()
{
    t_ofeliaSeedRandom *x = reinterpret_cast<t_ofeliaSeedRandom*>(pd_new(ofeliaSeedRandom_class));
    return (x);
}

void ofeliaSeedRandom_bang(t_ofeliaSeedRandom *x)
{
    ofSeedRandom();
    ofRandomuf();
}

void ofeliaSeedRandom_float(t_ofeliaSeedRandom *x, t_floatarg f)
{
    ofSeedRandom(static_cast<int>(f));
    ofRandomuf();
}

void ofeliaSeedRandom_setup()
{
    ofeliaSeedRandom_class = class_new(gensym("ofSeedRandom"),
                                       reinterpret_cast<t_newmethod>(ofeliaSeedRandom_new),
                                       0, sizeof(t_ofeliaSeedRandom),
                                       CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaSeedRandom_class, reinterpret_cast<t_method>(ofeliaSeedRandom_bang));
    class_addfloat(ofeliaSeedRandom_class, reinterpret_cast<t_method>(ofeliaSeedRandom_float));
}

/* ________________________________________________________________________________
 * ofWrap object methods
 */
void *ofeliaWrap_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaWrap *x = reinterpret_cast<t_ofeliaWrap*>(pd_new(ofeliaWrap_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getRangeElemFromArgs(argc, argv, x->elem, t_ofeliaWrap::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("min"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("max"));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaWrap_float(t_ofeliaWrap *x, t_floatarg f)
{
    outlet_float(x->x_obj.ob_outlet, ofWrap(f, x->elem.min, x->elem.max));
}

void ofeliaWrap_min(t_ofeliaWrap *x, t_floatarg f)
{
    x->elem.min = f;
}

void ofeliaWrap_max(t_ofeliaWrap *x, t_floatarg f)
{
    x->elem.max = f;
}

void ofeliaWrap_set(t_ofeliaWrap *x, t_symbol *s, int argc, t_atom *argv)
{
    getRangeElemFromArgs(argc, argv, x->elem, t_ofeliaWrap::objName);
}

void ofeliaWrap_print(t_ofeliaWrap *x)
{
    post("\n[%s]", t_ofeliaWrap::objName);
    post("min : %g", x->elem.min);
    post("max : %g", x->elem.max);
}

void ofeliaWrap_setup()
{
    ofeliaWrap_class = class_new(gensym("ofWrap"),
                                 reinterpret_cast<t_newmethod>(ofeliaWrap_new),
                                 0, sizeof(t_ofeliaWrap),
                                 CLASS_DEFAULT, A_GIMME, 0);
    class_addfloat(ofeliaWrap_class, reinterpret_cast<t_method>(ofeliaWrap_float));
    class_addmethod(ofeliaWrap_class, reinterpret_cast<t_method>(ofeliaWrap_min),
                    gensym("min"), A_FLOAT, 0);
    class_addmethod(ofeliaWrap_class, reinterpret_cast<t_method>(ofeliaWrap_max),
                    gensym("max"), A_FLOAT, 0);
    class_addmethod(ofeliaWrap_class, reinterpret_cast<t_method>(ofeliaWrap_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaWrap_class, reinterpret_cast<t_method>(ofeliaWrap_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofWrapDegrees object methods
 */
void *ofeliaWrapDegrees_new()
{
    t_ofeliaWrapDegrees *x = reinterpret_cast<t_ofeliaWrapDegrees*>(pd_new(ofeliaWrapDegrees_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaWrapDegrees_float(t_ofeliaWrapDegrees *x, t_floatarg f)
{
    outlet_float(x->x_obj.ob_outlet, ofWrapDegrees(f));
}

void ofeliaWrapDegrees_setup()
{
    ofeliaWrapDegrees_class = class_new(gensym("ofWrapDegrees"),
                                        reinterpret_cast<t_newmethod>(ofeliaWrapDegrees_new),
                                        0, sizeof(t_ofeliaWrapDegrees),
                                        CLASS_DEFAULT, A_NULL, 0);
    class_addfloat(ofeliaWrapDegrees_class, reinterpret_cast<t_method>(ofeliaWrapDegrees_float));
}

/* ________________________________________________________________________________
 * ofWrapRadians object methods
 */
void *ofeliaWrapRadians_new()
{
    t_ofeliaWrapRadians *x = reinterpret_cast<t_ofeliaWrapRadians*>(pd_new(ofeliaWrapRadians_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaWrapRadians_float(t_ofeliaWrapRadians *x, t_floatarg f)
{
    outlet_float(x->x_obj.ob_outlet, ofWrapRadians(f));
}

void ofeliaWrapRadians_setup()
{
    ofeliaWrapRadians_class = class_new(gensym("ofWrapRadians"),
                                        reinterpret_cast<t_newmethod>(ofeliaWrapRadians_new),
                                        0, sizeof(t_ofeliaWrapRadians),
                                        CLASS_DEFAULT, A_NULL, 0);
    class_addfloat(ofeliaWrapRadians_class, reinterpret_cast<t_method>(ofeliaWrapRadians_float));
}

/* ________________________________________________________________________________
 * ofMap object methods
 */
bool getMapElemFromArgs(const int argc, const t_atom *argv, t_ofeliaMapElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.inputMin = 0.0f;
            elem.inputMax = 1.0f;
            elem.outputMin = 0.0f;
            elem.outputMax = 1.0f;
            elem.clamp = false;
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT) {
                
                elem.inputMin = argv[0].a_w.w_float;
                elem.inputMax = argv[1].a_w.w_float;
                elem.outputMin = 0.0f;
                elem.outputMax = 1.0f;
                elem.clamp = false;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaMap::objName);
                return 0;
            }
            break;
        }
        case 4:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT) {
                
                elem.inputMin = argv[0].a_w.w_float;
                elem.inputMax = argv[1].a_w.w_float;
                elem.outputMin = argv[2].a_w.w_float;
                elem.outputMax = argv[3].a_w.w_float;
                elem.clamp = false;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaMap::objName);
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
                
                elem.inputMin = argv[0].a_w.w_float;
                elem.inputMax = argv[1].a_w.w_float;
                elem.outputMin = argv[2].a_w.w_float;
                elem.outputMax = argv[3].a_w.w_float;
                elem.clamp = argv[4].a_w.w_float != 0.0f;
            }
            else if (argv[0].a_type == A_FLOAT &&
                     argv[1].a_type == A_FLOAT &&
                     argv[2].a_type == A_FLOAT &&
                     argv[3].a_type == A_FLOAT &&
                     argv[4].a_type == A_SYMBOL) {
                
                elem.inputMin = argv[0].a_w.w_float;
                elem.inputMax = argv[1].a_w.w_float;
                elem.outputMin = argv[2].a_w.w_float;
                elem.outputMax = argv[3].a_w.w_float;
                getToggleFromSym(argv[4].a_w.w_symbol, elem.clamp, t_ofeliaMap::objName);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaMap::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaMap::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaMap_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaMap *x = reinterpret_cast<t_ofeliaMap*>(pd_new(ofeliaMap_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getMapElemFromArgs(argc, argv, x->elem))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("inputMin"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("inputMax"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("outputMin"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("outputMax"));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaMap_float(t_ofeliaMap *x, t_floatarg f)
{
    outlet_float(x->x_obj.ob_outlet, ofMap(f, x->elem.inputMin, x->elem.inputMax,
                                           x->elem.outputMin, x->elem.outputMax, x->elem.clamp));
}

void ofeliaMap_inputMin(t_ofeliaMap *x, t_floatarg f)
{
    x->elem.inputMin = f;
}

void ofeliaMap_inputMax(t_ofeliaMap *x, t_floatarg f)
{
    x->elem.inputMax = f;
}

void ofeliaMap_outputMin(t_ofeliaMap *x, t_floatarg f)
{
    x->elem.outputMin = f;
}

void ofeliaMap_outputMax(t_ofeliaMap *x, t_floatarg f)
{
    x->elem.outputMax = f;
}

void ofeliaMap_clamp(t_ofeliaMap *x, t_symbol *s, int argc, t_atom *argv)
{
    getToggleFromArgs(argc, argv, x->elem.clamp, t_ofeliaMap::objName);
}

void ofeliaMap_set(t_ofeliaMap *x, t_symbol *s, int argc, t_atom *argv)
{
    getMapElemFromArgs(argc, argv, x->elem);
}

void ofeliaMap_print(t_ofeliaMap *x)
{
    post("\n[%s]", t_ofeliaMap::objName);
    post("inputMin : %g", x->elem.inputMin);
    post("inputMax : %g", x->elem.inputMax);
    post("outputMin : %g", x->elem.outputMin);
    post("outputMax : %g", x->elem.outputMax);
    printToggle("clamp", x->elem.clamp);
}

void ofeliaMap_setup()
{
    ofeliaMap_class = class_new(gensym("ofMap"),
                                reinterpret_cast<t_newmethod>(ofeliaMap_new),
                                0, sizeof(t_ofeliaMap),
                                CLASS_DEFAULT, A_GIMME, 0);
    class_addfloat(ofeliaMap_class, reinterpret_cast<t_method>(ofeliaMap_float));
    class_addmethod(ofeliaMap_class, reinterpret_cast<t_method>(ofeliaMap_inputMin),
                    gensym("inputMin"), A_FLOAT, 0);
    class_addmethod(ofeliaMap_class, reinterpret_cast<t_method>(ofeliaMap_inputMax),
                    gensym("inputMax"), A_FLOAT, 0);
    class_addmethod(ofeliaMap_class, reinterpret_cast<t_method>(ofeliaMap_outputMin),
                    gensym("outputMin"), A_FLOAT, 0);
    class_addmethod(ofeliaMap_class, reinterpret_cast<t_method>(ofeliaMap_outputMax),
                    gensym("outputMax"), A_FLOAT, 0);
    class_addmethod(ofeliaMap_class, reinterpret_cast<t_method>(ofeliaMap_clamp),
                    gensym("clamp"), A_GIMME, 0);
    class_addmethod(ofeliaMap_class, reinterpret_cast<t_method>(ofeliaMap_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaMap_class, reinterpret_cast<t_method>(ofeliaMap_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofNextPow2 object methods
 */
void *ofeliaNextPow2_new()
{
    t_ofeliaNextPow2 *x = reinterpret_cast<t_ofeliaNextPow2*>(pd_new(ofeliaNextPow2_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaNextPow2_float(t_ofeliaNextPow2 *x, t_floatarg f)
{
    outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(ofNextPow2(static_cast<int>(f))));
}

void ofeliaNextPow2_setup()
{
    ofeliaNextPow2_class = class_new(gensym("ofNextPow2"),
                                     reinterpret_cast<t_newmethod>(ofeliaNextPow2_new),
                                     0, sizeof(t_ofeliaNextPow2),
                                     CLASS_DEFAULT, A_NULL, 0);
    class_addfloat(ofeliaNextPow2_class, reinterpret_cast<t_method>(ofeliaNextPow2_float));
}

/* ________________________________________________________________________________
 * ofNoise object methods
 */
void *ofeliaNoise_new()
{
    t_ofeliaNoise *x = reinterpret_cast<t_ofeliaNoise*>(pd_new(ofeliaNoise_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaNoise_float(t_ofeliaNoise *x, t_floatarg f)
{
    outlet_float(x->x_obj.ob_outlet, ofNoise(f));
}

void ofeliaNoise_list(t_ofeliaNoise *x, t_symbol *s, int argc, t_atom *argv)
{
    switch (argc) {
            
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT)
                outlet_float(x->x_obj.ob_outlet, ofNoise(argv[0].a_w.w_float,
                                                         argv[1].a_w.w_float));
            else
                error("%s: wrong argument type", t_ofeliaNoise::objName);
            break;
        }
        case 3:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT)
                outlet_float(x->x_obj.ob_outlet, ofNoise(argv[0].a_w.w_float,
                                                         argv[1].a_w.w_float,
                                                         argv[2].a_w.w_float));
            else
                error("%s: wrong argument type", t_ofeliaNoise::objName);
            break;
        }
        case 4:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT)
                outlet_float(x->x_obj.ob_outlet, ofNoise(argv[0].a_w.w_float,
                                                         argv[1].a_w.w_float,
                                                         argv[2].a_w.w_float,
                                                         argv[3].a_w.w_float));
            else
                error("%s: wrong argument type", t_ofeliaNoise::objName);
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaNoise::objName);
            break;
        }
    }
}

void ofeliaNoise_setup()
{
    ofeliaNoise_class = class_new(gensym("ofNoise"),
                                  reinterpret_cast<t_newmethod>(ofeliaNoise_new),
                                  0, sizeof(t_ofeliaNoise),
                                  CLASS_DEFAULT, A_NULL, 0);
    class_addfloat(ofeliaNoise_class, reinterpret_cast<t_method>(ofeliaNoise_float));
    class_addlist(ofeliaNoise_class, reinterpret_cast<t_method>(ofeliaNoise_list));
}

/* ________________________________________________________________________________
 * ofSignedNoise object methods
 */
void *ofeliaSignedNoise_new()
{
    t_ofeliaSignedNoise *x = reinterpret_cast<t_ofeliaSignedNoise*>(pd_new(ofeliaSignedNoise_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaSignedNoise_float(t_ofeliaSignedNoise *x, t_floatarg f)
{
    outlet_float(x->x_obj.ob_outlet, ofSignedNoise(f));
}

void ofeliaSignedNoise_list(t_ofeliaSignedNoise *x, t_symbol *s, int argc, t_atom *argv)
{
    switch (argc) {
            
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT)
                outlet_float(x->x_obj.ob_outlet, ofSignedNoise(argv[0].a_w.w_float,
                                                               argv[1].a_w.w_float));
            else
                error("%s: wrong argument type", t_ofeliaSignedNoise::objName);
            break;
        }
        case 3:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT)
                outlet_float(x->x_obj.ob_outlet, ofSignedNoise(argv[0].a_w.w_float,
                                                               argv[1].a_w.w_float,
                                                               argv[2].a_w.w_float));
            else
                error("%s: wrong argument type", t_ofeliaSignedNoise::objName);
            break;
        }
        case 4:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT)
                outlet_float(x->x_obj.ob_outlet, ofSignedNoise(argv[0].a_w.w_float,
                                                               argv[1].a_w.w_float,
                                                               argv[2].a_w.w_float,
                                                               argv[3].a_w.w_float));
            else
                error("%s: wrong argument type", t_ofeliaSignedNoise::objName);
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaSignedNoise::objName);
            break;
        }
    }
}

void ofeliaSignedNoise_setup()
{
    ofeliaSignedNoise_class = class_new(gensym("ofSignedNoise"),
                                        reinterpret_cast<t_newmethod>(ofeliaSignedNoise_new),
                                        0, sizeof(t_ofeliaSignedNoise),
                                        CLASS_DEFAULT, A_NULL, 0);
    class_addfloat(ofeliaSignedNoise_class, reinterpret_cast<t_method>(ofeliaSignedNoise_float));
    class_addlist(ofeliaSignedNoise_class, reinterpret_cast<t_method>(ofeliaSignedNoise_list));
}

/* ________________________________________________________________________________
 * ofSign object methods
 */
void *ofeliaSign_new()
{
    t_ofeliaSign *x = reinterpret_cast<t_ofeliaSign*>(pd_new(ofeliaSign_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaSign_float(t_ofeliaSign *x, t_floatarg f)
{
    outlet_float(x->x_obj.ob_outlet, ofSign(f));
}

void ofeliaSign_setup()
{
    ofeliaSign_class = class_new(gensym("ofSign"),
                                 reinterpret_cast<t_newmethod>(ofeliaSign_new),
                                 0, sizeof(t_ofeliaSign),
                                 CLASS_DEFAULT, A_NULL, 0);
    class_addfloat(ofeliaSign_class, reinterpret_cast<t_method>(ofeliaSign_float));
}

/* ________________________________________________________________________________
 * setup methods
 */
void ofeliaMath_setup()
{
    ofeliaAngleDifferenceDegrees_setup();
    ofeliaAngleDifferenceRadians_setup();
    ofeliaDegToRad_setup();
    ofeliaRadToDeg_setup();
    ofeliaDist2d_setup();
    ofeliaDist3d_setup();
    ofeliaDistSquared2d_setup();
    ofeliaDistSquared3d_setup();
    ofeliaInRange_setup();
    ofeliaClamp_setup();
    ofeliaNormalize_setup();
    ofeliaLerp_setup();
    ofeliaLerpDegrees_setup();
    ofeliaLerpRadians_setup();
    ofeliaRandom_setup();
    ofeliaRandomf_setup();
    ofeliaRandomuf_setup();
    ofeliaSeedRandom_setup();
    ofeliaWrap_setup();
    ofeliaWrapDegrees_setup();
    ofeliaWrapRadians_setup();
    ofeliaMap_setup();
    ofeliaNextPow2_setup();
    ofeliaNoise_setup();
    ofeliaSignedNoise_setup();
    ofeliaSign_setup();
}

