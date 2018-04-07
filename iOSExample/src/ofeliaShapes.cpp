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

#include "ofeliaShapes.h"
#include "ofeliaWindow.h"

/* ________________________________________________________________________________
 * static variables
 */
const float t_ofeliaShapesDefault::curveControlY = OFELIA_DEFAULT_SIZE * 10.0f;
const float t_ofeliaShapesDefault::cornerRadius = OFELIA_DEFAULT_SIZE * 0.15f;
const float t_ofeliaShapesDefault::crossThickness = OFELIA_DEFAULT_SIZE * 0.25f;
const float t_ofeliaShapesDefault::moonPhaseRadius = OFELIA_DEFAULT_SIZE * 0.3f;
const float t_ofeliaShapesDefault::arrowHeadSize = OFELIA_DEFAULT_SIZE * 0.05f;
const char *t_ofeliaBeginShape::objName = "ofBeginShape";
const char *t_ofeliaEndShape::objName = "ofEndShape";
const char *t_ofeliaNextContour::objName = "ofNextContour";
const char *t_ofeliaVertex2d::objName = "ofVertex2d";
const char *t_ofeliaVertex3d::objName = "ofVertex3d";
const char *t_ofeliaCurveVertex2d::objName = "ofCurveVertex2d";
const char *t_ofeliaCurveVertex3d::objName = "ofCurveVertex3d";
const char *t_ofeliaBezierVertex2d::objName = "ofBezierVertex2d";
const char *t_ofeliaBezierVertex3d::objName = "ofBezierVertex3d";
const char *t_ofeliaCircle::objName = "ofCircle";
const char *t_ofeliaEllipse::objName = "ofEllipse";
const char *t_ofeliaArc::objName = "ofArc";
const char *t_ofeliaSector::objName = "ofSector";
const char *t_ofeliaLine2d::objName = "ofLine2d";
const char *t_ofeliaLine3d::objName = "ofLine3d";
const char *t_ofeliaCurve2d::objName = "ofCurve2d";
const char *t_ofeliaCurve3d::objName = "ofCurve3d";
const char *t_ofeliaBezier2d::objName = "ofBezier2d";
const char *t_ofeliaBezier3d::objName = "ofBezier3d";
const char *t_ofeliaQuadBezier2d::objName = "ofQuadBezier2d";
const char *t_ofeliaQuadBezier3d::objName = "ofQuadBezier3d";
const char *t_ofeliaTriangle2d::objName = "ofTriangle2d";
const char *t_ofeliaTriangle3d::objName = "ofTriangle3d";
const char *t_ofeliaEqTriangle::objName = "ofEqTriangle";
const char *t_ofeliaIsoTriangle::objName = "ofIsoTriangle";
const char *t_ofeliaQuad2d::objName = "ofQuad2d";
const char *t_ofeliaQuad3d::objName = "ofQuad3d";
const char *t_ofeliaSquare::objName = "ofSquare";
const char *t_ofeliaRectangle::objName = "ofRectangle";
const char *t_ofeliaRectRounded::objName = "ofRectRounded";
const char *t_ofeliaRectRounded4::objName = "ofRectRounded4";
const char *t_ofeliaCross::objName = "ofCross";
const char *t_ofeliaHeart::objName = "ofHeart";
const char *t_ofeliaMoon::objName = "ofMoon";
const char *t_ofeliaRegPolygon::objName = "ofRegPolygon";
const char *t_ofeliaStar::objName = "ofStar";
const char *t_ofeliaAxis::objName = "ofAxis";
const char *t_ofeliaBox::objName = "ofBox";
const char *t_ofeliaCone::objName = "ofCone";
const char *t_ofeliaCylinder::objName = "ofCylinder";
const char *t_ofeliaIcosphere::objName = "ofIcosphere";
const char *t_ofeliaPlane::objName = "ofPlane";
const char *t_ofeliaSphere::objName = "ofSphere";
const char *t_ofeliaArrow::objName = "ofArrow";
const char *t_ofeliaGrid::objName = "ofGrid";
const char *t_ofeliaGridPlane::objName = "ofGridPlane";
const char *t_ofeliaRotationAxes::objName = "ofRotationAxes";

/* ________________________________________________________________________________
 * common methods
 */
bool getDimenFromArgs(const int argc, const t_atom *argv,
                      t_ofeliaShapesDimen &dimen, const char *objName)
{
    switch (argc) {
            
        case 0:
        {
            dimen.width = OFELIA_DEFAULT_SIZE;
            dimen.height = OFELIA_DEFAULT_SIZE;
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT) {
                
                dimen.width = argv[0].a_w.w_float;
                dimen.height = argv[1].a_w.w_float;
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

bool getArcElemFromArgs(const int argc, const t_atom *argv,
                        t_ofeliaArcElem &elem, const char *objName)
{
    switch (argc) {
            
        case 0:
        {
            elem.radiusX = OFELIA_DEFAULT_SIZE_HALF;
            elem.radiusY = OFELIA_DEFAULT_SIZE_HALF;
            elem.angleBegin = -180.0f;
            elem.angleEnd = 0.0f;
            break;
        }
        case 4:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT) {
                
                elem.radiusX = argv[0].a_w.w_float;
                elem.radiusY = argv[1].a_w.w_float;
                elem.angleBegin = argv[2].a_w.w_float;
                elem.angleEnd = argv[3].a_w.w_float;
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

bool getBezier2dPointsFromArgs(const int argc, const t_atom *argv,
                               t_ofeliaBezier2dPoints &points, const char *objName)
{
    switch (argc) {
            
        case 0:
        {
            points.p0.set(-OFELIA_DEFAULT_SIZE_HALF, OFELIA_DEFAULT_SIZE_HALF);
            points.p1.set(-OFELIA_DEFAULT_SIZE_HALF, -OFELIA_DEFAULT_SIZE_HALF);
            points.p2.set(OFELIA_DEFAULT_SIZE_HALF, -OFELIA_DEFAULT_SIZE_HALF);
            points.p3.set(OFELIA_DEFAULT_SIZE_HALF, OFELIA_DEFAULT_SIZE_HALF);
            break;
        }
        case 8:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT &&
                argv[4].a_type == A_FLOAT &&
                argv[5].a_type == A_FLOAT &&
                argv[6].a_type == A_FLOAT &&
                argv[7].a_type == A_FLOAT) {
                
                points.p0.set(argv[0].a_w.w_float, argv[1].a_w.w_float);
                points.p1.set(argv[2].a_w.w_float, argv[3].a_w.w_float);
                points.p2.set(argv[4].a_w.w_float, argv[5].a_w.w_float);
                points.p3.set(argv[6].a_w.w_float, argv[7].a_w.w_float);
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

bool getBezier3dPointsFromArgs(const int argc, const t_atom *argv,
                               t_ofeliaBezier3dPoints &points, const char *objName)
{
    switch (argc) {
            
        case 0:
        {
            points.p0.set(-OFELIA_DEFAULT_SIZE_HALF, OFELIA_DEFAULT_SIZE_HALF);
            points.p1.set(-OFELIA_DEFAULT_SIZE_HALF, -OFELIA_DEFAULT_SIZE_HALF);
            points.p2.set(OFELIA_DEFAULT_SIZE_HALF, -OFELIA_DEFAULT_SIZE_HALF);
            points.p3.set(OFELIA_DEFAULT_SIZE_HALF, OFELIA_DEFAULT_SIZE_HALF);
            break;
        }
        case 12:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT &&
                argv[4].a_type == A_FLOAT &&
                argv[5].a_type == A_FLOAT &&
                argv[6].a_type == A_FLOAT &&
                argv[7].a_type == A_FLOAT &&
                argv[8].a_type == A_FLOAT &&
                argv[9].a_type == A_FLOAT &&
                argv[10].a_type == A_FLOAT &&
                argv[11].a_type == A_FLOAT) {
                
                points.p0.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                points.p1.set(argv[3].a_w.w_float, argv[4].a_w.w_float, argv[5].a_w.w_float);
                points.p2.set(argv[6].a_w.w_float, argv[7].a_w.w_float, argv[8].a_w.w_float);
                points.p3.set(argv[9].a_w.w_float, argv[10].a_w.w_float, argv[11].a_w.w_float);
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
 * ofBeginShape object methods
 */
void *ofeliaBeginShape_new()
{
    t_ofeliaBeginShape *x = reinterpret_cast<t_ofeliaBeginShape*>(pd_new(ofeliaBeginShape_class));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaBeginShape_bang(t_ofeliaBeginShape *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofBeginShape();
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaBeginShape::objName);
    }
}

void ofeliaBeginShape_setup()
{
    ofeliaBeginShape_class = class_new(gensym("ofBeginShape"),
                                       reinterpret_cast<t_newmethod>(ofeliaBeginShape_new),
                                       0, sizeof(t_ofeliaBeginShape),
                                       CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaBeginShape_class, reinterpret_cast<t_method>(ofeliaBeginShape_bang));
}

/* ________________________________________________________________________________
 * ofEndShape object methods
 */
void *ofeliaEndShape_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaEndShape *x = reinterpret_cast<t_ofeliaEndShape*>(pd_new(ofeliaEndShape_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (argc) {
        
        if (!getToggleFromArgs(argc, argv, x->autoClose, t_ofeliaEndShape::objName))
            return 0;
    }
    else {
        
        x->autoClose = false;
    }
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("autoClose"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaEndShape_bang(t_ofeliaEndShape *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofEndShape(x->autoClose);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaEndShape::objName);
    }
}

void ofeliaEndShape_set(t_ofeliaEndShape *x, t_symbol *s, int argc, t_atom *argv)
{
    getToggleFromArgs(argc, argv, x->autoClose, t_ofeliaEndShape::objName);
}

void ofeliaEndShape_print(t_ofeliaEndShape *x)
{
    post("\n[%s]", t_ofeliaEndShape::objName);
    printToggle("autoClose", x->autoClose);
}

void ofeliaEndShape_setup()
{
    ofeliaEndShape_class = class_new(gensym("ofEndShape"),
                                     reinterpret_cast<t_newmethod>(ofeliaEndShape_new),
                                     0, sizeof(t_ofeliaEndShape),
                                     CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaEndShape_class, reinterpret_cast<t_method>(ofeliaEndShape_bang));
    class_addmethod(ofeliaEndShape_class, reinterpret_cast<t_method>(ofeliaEndShape_set),
                    gensym("autoClose"), A_GIMME, 0);
    class_addmethod(ofeliaEndShape_class, reinterpret_cast<t_method>(ofeliaEndShape_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaEndShape_class, reinterpret_cast<t_method>(ofeliaEndShape_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofNextContour object methods
 */
void *ofeliaNextContour_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaNextContour *x = reinterpret_cast<t_ofeliaNextContour*>(pd_new(ofeliaNextContour_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (argc) {
        
        if (!getToggleFromArgs(argc, argv, x->autoClose, t_ofeliaNextContour::objName))
            return 0;
    }
    else {
        
        x->autoClose = false;
    }
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("autoClose"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaNextContour_bang(t_ofeliaNextContour *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofNextContour(x->autoClose);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaNextContour::objName);
    }
}

void ofeliaNextContour_set(t_ofeliaNextContour *x, t_symbol *s, int argc, t_atom *argv)
{
    getToggleFromArgs(argc, argv, x->autoClose, t_ofeliaNextContour::objName);
}

void ofeliaNextContour_print(t_ofeliaNextContour *x)
{
    post("\n[%s]", t_ofeliaNextContour::objName);
    printToggle("autoClose", x->autoClose);
}

void ofeliaNextContour_setup()
{
    ofeliaNextContour_class = class_new(gensym("ofNextContour"),
                                        reinterpret_cast<t_newmethod>(ofeliaNextContour_new),
                                        0, sizeof(t_ofeliaNextContour),
                                        CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaNextContour_class, reinterpret_cast<t_method>(ofeliaNextContour_bang));
    class_addmethod(ofeliaNextContour_class, reinterpret_cast<t_method>(ofeliaNextContour_set),
                    gensym("autoClose"), A_GIMME, 0);
    class_addmethod(ofeliaNextContour_class, reinterpret_cast<t_method>(ofeliaNextContour_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaNextContour_class, reinterpret_cast<t_method>(ofeliaNextContour_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofVertex2d object methods
 */
void *ofeliaVertex2d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaVertex2d *x = reinterpret_cast<t_ofeliaVertex2d*>(pd_new(ofeliaVertex2d_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getVec2fFromArgs(argc, argv, x->vertex, t_ofeliaVertex2d::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("vertex"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaVertex2d_bang(t_ofeliaVertex2d *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofGetCurrentRenderer()->getPath().lineTo(x->vertex);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaVertex2d::objName);
    }
}

void ofeliaVertex2d_set(t_ofeliaVertex2d *x, t_symbol *s, int argc, t_atom *argv)
{
    getVec2fFromArgs(argc, argv, x->vertex, t_ofeliaVertex2d::objName);
}

void ofeliaVertex2d_print(t_ofeliaVertex2d *x)
{
    post("\n[%s]", t_ofeliaVertex2d::objName);
    post("vertex : %g %g", x->vertex.x, x->vertex.y);
}

void ofeliaVertex2d_setup()
{
    ofeliaVertex2d_class = class_new(gensym("ofVertex2d"),
                                     reinterpret_cast<t_newmethod>(ofeliaVertex2d_new),
                                     0, sizeof(t_ofeliaVertex2d),
                                     CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaVertex2d_class, reinterpret_cast<t_method>(ofeliaVertex2d_bang));
    class_addmethod(ofeliaVertex2d_class, reinterpret_cast<t_method>(ofeliaVertex2d_set),
                    gensym("vertex"), A_GIMME, 0);
    class_addmethod(ofeliaVertex2d_class, reinterpret_cast<t_method>(ofeliaVertex2d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaVertex2d_class, reinterpret_cast<t_method>(ofeliaVertex2d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofVertex3d object methods
 */
void *ofeliaVertex3d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaVertex3d *x = reinterpret_cast<t_ofeliaVertex3d*>(pd_new(ofeliaVertex3d_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getVec3fFromArgs(argc, argv, x->vertex, t_ofeliaVertex3d::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("vertex"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaVertex3d_bang(t_ofeliaVertex3d *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofGetCurrentRenderer()->getPath().lineTo(x->vertex);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaVertex3d::objName);
    }
}

void ofeliaVertex3d_set(t_ofeliaVertex3d *x, t_symbol *s, int argc, t_atom *argv)
{
    getVec3fFromArgs(argc, argv, x->vertex, t_ofeliaVertex3d::objName);
}

void ofeliaVertex3d_print(t_ofeliaVertex3d *x)
{
    post("\n[%s]", t_ofeliaVertex3d::objName);
    post("vertex : %g %g %g", x->vertex.x, x->vertex.y, x->vertex.z);
}

void ofeliaVertex3d_setup()
{
    ofeliaVertex3d_class = class_new(gensym("ofVertex3d"),
                                     reinterpret_cast<t_newmethod>(ofeliaVertex3d_new),
                                     0, sizeof(t_ofeliaVertex3d),
                                     CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaVertex3d_class, reinterpret_cast<t_method>(ofeliaVertex3d_bang));
    class_addmethod(ofeliaVertex3d_class, reinterpret_cast<t_method>(ofeliaVertex3d_set),
                    gensym("vertex"), A_GIMME, 0);
    class_addmethod(ofeliaVertex3d_class, reinterpret_cast<t_method>(ofeliaVertex3d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaVertex3d_class, reinterpret_cast<t_method>(ofeliaVertex3d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofCurveVertex2d object methods
 */
void *ofeliaCurveVertex2d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaCurveVertex2d *x = reinterpret_cast<t_ofeliaCurveVertex2d*>(pd_new(ofeliaCurveVertex2d_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getVec2fFromArgs(argc, argv, x->vertex, t_ofeliaCurveVertex2d::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("vertex"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaCurveVertex2d_bang(t_ofeliaCurveVertex2d *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofGetCurrentRenderer()->getPath().curveTo(x->vertex);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaCurveVertex2d::objName);
    }
}

void ofeliaCurveVertex2d_set(t_ofeliaCurveVertex2d *x, t_symbol *s, int argc, t_atom *argv)
{
    getVec2fFromArgs(argc, argv, x->vertex, t_ofeliaCurveVertex2d::objName);
}

void ofeliaCurveVertex2d_print(t_ofeliaCurveVertex2d *x)
{
    post("\n[%s]", t_ofeliaCurveVertex2d::objName);
    post("vertex : %g %g", x->vertex.x, x->vertex.y);
}

void ofeliaCurveVertex2d_setup()
{
    ofeliaCurveVertex2d_class = class_new(gensym("ofCurveVertex2d"),
                                          reinterpret_cast<t_newmethod>(ofeliaCurveVertex2d_new),
                                          0, sizeof(t_ofeliaCurveVertex2d),
                                          CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaCurveVertex2d_class, reinterpret_cast<t_method>(ofeliaCurveVertex2d_bang));
    class_addmethod(ofeliaCurveVertex2d_class, reinterpret_cast<t_method>(ofeliaCurveVertex2d_set),
                    gensym("vertex"), A_GIMME, 0);
    class_addmethod(ofeliaCurveVertex2d_class, reinterpret_cast<t_method>(ofeliaCurveVertex2d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaCurveVertex2d_class, reinterpret_cast<t_method>(ofeliaCurveVertex2d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofCurveVertex3d object methods
 */
void *ofeliaCurveVertex3d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaCurveVertex3d *x = reinterpret_cast<t_ofeliaCurveVertex3d*>(pd_new(ofeliaCurveVertex3d_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getVec3fFromArgs(argc, argv, x->vertex, t_ofeliaCurveVertex3d::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("vertex"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaCurveVertex3d_bang(t_ofeliaCurveVertex3d *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofGetCurrentRenderer()->getPath().curveTo(x->vertex);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaCurveVertex3d::objName);
    }
}

void ofeliaCurveVertex3d_set(t_ofeliaCurveVertex3d *x, t_symbol *s, int argc, t_atom *argv)
{
    getVec3fFromArgs(argc, argv, x->vertex, t_ofeliaCurveVertex3d::objName);
}

void ofeliaCurveVertex3d_print(t_ofeliaCurveVertex3d *x)
{
    post("\n[%s]", t_ofeliaCurveVertex3d::objName);
    post("vertex : %g %g %g", x->vertex.x, x->vertex.y, x->vertex.z);
}

void ofeliaCurveVertex3d_setup()
{
    ofeliaCurveVertex3d_class = class_new(gensym("ofCurveVertex3d"),
                                          reinterpret_cast<t_newmethod>(ofeliaCurveVertex3d_new),
                                          0, sizeof(t_ofeliaCurveVertex3d),
                                          CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaCurveVertex3d_class, reinterpret_cast<t_method>(ofeliaCurveVertex3d_bang));
    class_addmethod(ofeliaCurveVertex3d_class, reinterpret_cast<t_method>(ofeliaCurveVertex3d_set),
                    gensym("vertex"), A_GIMME, 0);
    class_addmethod(ofeliaCurveVertex3d_class, reinterpret_cast<t_method>(ofeliaCurveVertex3d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaCurveVertex3d_class, reinterpret_cast<t_method>(ofeliaCurveVertex3d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofBezierVertex2d object methods
 */
bool getBezierVertex2dPointsFromArgs(const int argc, const t_atom *argv, t_ofeliaBezierVertex2dPoints &points)
{
    switch (argc) {
            
        case 0:
        {
            points.p1.set(0.0f, 0.0f);
            points.p2.set(0.0f, 0.0f);
            points.p3.set(0.0f, 0.0f);
            break;
        }
        case 6:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT &&
                argv[4].a_type == A_FLOAT &&
                argv[5].a_type == A_FLOAT) {
                
                points.p1.set(argv[0].a_w.w_float, argv[1].a_w.w_float);
                points.p2.set(argv[2].a_w.w_float, argv[3].a_w.w_float);
                points.p3.set(argv[4].a_w.w_float, argv[5].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaBezierVertex2d::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaBezierVertex2d::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaBezierVertex2d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaBezierVertex2d *x = reinterpret_cast<t_ofeliaBezierVertex2d*>(pd_new(ofeliaBezierVertex2d_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getBezierVertex2dPointsFromArgs(argc, argv, x->points))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p1"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p2"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p3"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaBezierVertex2d_bang(t_ofeliaBezierVertex2d *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofGetCurrentRenderer()->getPath().bezierTo(x->points.p1, x->points.p2, x->points.p3);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaBezierVertex2d::objName);
    }
}

void ofeliaBezierVertex2d_p1(t_ofeliaBezierVertex2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            x->points.p1.set(argv[0].a_w.w_float, argv[1].a_w.w_float);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaBezierVertex2d::objName);
    }
}

void ofeliaBezierVertex2d_p2(t_ofeliaBezierVertex2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            x->points.p2.set(argv[0].a_w.w_float, argv[1].a_w.w_float);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaBezierVertex2d::objName);
    }
}

void ofeliaBezierVertex2d_p3(t_ofeliaBezierVertex2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            x->points.p3.set(argv[0].a_w.w_float, argv[1].a_w.w_float);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaBezierVertex2d::objName);
    }
}

void ofeliaBezierVertex2d_set(t_ofeliaBezierVertex2d *x, t_symbol *s, int argc, t_atom *argv)
{
    getBezierVertex2dPointsFromArgs(argc, argv, x->points);
}

void ofeliaBezierVertex2d_print(t_ofeliaBezierVertex2d *x)
{
    post("\n[%s]", t_ofeliaBezierVertex2d::objName);
    post("p1 : %g %g", x->points.p1.x, x->points.p1.y);
    post("p2 : %g %g", x->points.p2.x, x->points.p2.y);
    post("p3 : %g %g", x->points.p3.x, x->points.p3.y);
}

void ofeliaBezierVertex2d_setup()
{
    ofeliaBezierVertex2d_class = class_new(gensym("ofBezierVertex2d"),
                                           reinterpret_cast<t_newmethod>(ofeliaBezierVertex2d_new),
                                           0, sizeof(t_ofeliaBezierVertex2d),
                                           CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaBezierVertex2d_class, reinterpret_cast<t_method>(ofeliaBezierVertex2d_bang));
    class_addmethod(ofeliaBezierVertex2d_class, reinterpret_cast<t_method>(ofeliaBezierVertex2d_p1),
                    gensym("p1"), A_GIMME, 0);
    class_addmethod(ofeliaBezierVertex2d_class, reinterpret_cast<t_method>(ofeliaBezierVertex2d_p2),
                    gensym("p2"), A_GIMME, 0);
    class_addmethod(ofeliaBezierVertex2d_class, reinterpret_cast<t_method>(ofeliaBezierVertex2d_p3),
                    gensym("p3"), A_GIMME, 0);
    class_addmethod(ofeliaBezierVertex2d_class, reinterpret_cast<t_method>(ofeliaBezierVertex2d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaBezierVertex2d_class, reinterpret_cast<t_method>(ofeliaBezierVertex2d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofBezierVertex3d object methods
 */
bool getBezierVertex3dPointsFromArgs(const int argc, const t_atom *argv, t_ofeliaBezierVertex3dPoints &points)
{
    switch (argc) {
            
        case 0:
        {
            points.p1.set(0.0f, 0.0f, 0.0f);
            points.p2.set(0.0f, 0.0f, 0.0f);
            points.p3.set(0.0f, 0.0f, 0.0f);
            break;
        }
        case 9:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT &&
                argv[4].a_type == A_FLOAT &&
                argv[5].a_type == A_FLOAT &&
                argv[6].a_type == A_FLOAT &&
                argv[7].a_type == A_FLOAT &&
                argv[8].a_type == A_FLOAT) {
                
                points.p1.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                points.p2.set(argv[3].a_w.w_float, argv[4].a_w.w_float, argv[5].a_w.w_float);
                points.p3.set(argv[6].a_w.w_float, argv[7].a_w.w_float, argv[8].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaBezierVertex3d::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaBezierVertex3d::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaBezierVertex3d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaBezierVertex3d *x = reinterpret_cast<t_ofeliaBezierVertex3d*>(pd_new(ofeliaBezierVertex3d_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getBezierVertex3dPointsFromArgs(argc, argv, x->points))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p1"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p2"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p3"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaBezierVertex3d_bang(t_ofeliaBezierVertex3d *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofGetCurrentRenderer()->getPath().bezierTo(x->points.p1, x->points.p2, x->points.p3);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaBezierVertex3d::objName);
    }
}

void ofeliaBezierVertex3d_p1(t_ofeliaBezierVertex3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            x->points.p1.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaBezierVertex3d::objName);
    }
}

void ofeliaBezierVertex3d_p2(t_ofeliaBezierVertex3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            x->points.p2.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaBezierVertex3d::objName);
    }
}

void ofeliaBezierVertex3d_p3(t_ofeliaBezierVertex3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            x->points.p3.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaBezierVertex3d::objName);
    }
}

void ofeliaBezierVertex3d_set(t_ofeliaBezierVertex3d *x, t_symbol *s, int argc, t_atom *argv)
{
    getBezierVertex3dPointsFromArgs(argc, argv, x->points);
}

void ofeliaBezierVertex3d_print(t_ofeliaBezierVertex3d *x)
{
    post("\n[%s]", t_ofeliaBezierVertex3d::objName);
    post("p1 : %g %g %g", x->points.p1.x, x->points.p1.y, x->points.p1.z);
    post("p2 : %g %g %g", x->points.p2.x, x->points.p2.y, x->points.p2.z);
    post("p3 : %g %g %g", x->points.p3.x, x->points.p3.y, x->points.p3.z);
}

void ofeliaBezierVertex3d_setup()
{
    ofeliaBezierVertex3d_class = class_new(gensym("ofBezierVertex3d"),
                                           reinterpret_cast<t_newmethod>(ofeliaBezierVertex3d_new),
                                           0, sizeof(t_ofeliaBezierVertex3d),
                                           CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaBezierVertex3d_class, reinterpret_cast<t_method>(ofeliaBezierVertex3d_bang));
    class_addmethod(ofeliaBezierVertex3d_class, reinterpret_cast<t_method>(ofeliaBezierVertex3d_p1),
                    gensym("p1"), A_GIMME, 0);
    class_addmethod(ofeliaBezierVertex3d_class, reinterpret_cast<t_method>(ofeliaBezierVertex3d_p2),
                    gensym("p2"), A_GIMME, 0);
    class_addmethod(ofeliaBezierVertex3d_class, reinterpret_cast<t_method>(ofeliaBezierVertex3d_p3),
                    gensym("p3"), A_GIMME, 0);
    class_addmethod(ofeliaBezierVertex3d_class, reinterpret_cast<t_method>(ofeliaBezierVertex3d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaBezierVertex3d_class, reinterpret_cast<t_method>(ofeliaBezierVertex3d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofCircle object methods
 */
void *ofeliaCircle_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaCircle *x = reinterpret_cast<t_ofeliaCircle*>(pd_new(ofeliaCircle_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    switch (argc) {
            
        case 0:
        {
            x->radius = OFELIA_DEFAULT_SIZE_HALF;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                x->radius = argv[0].a_w.w_float;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaCircle::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaCircle::objName);
            return 0;
        }
    }
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("radius"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaCircle_bang(t_ofeliaCircle *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofGetCurrentRenderer()->drawCircle(0.0f, 0.0f, 0.0f, x->radius);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaCircle::objName);
    }
}

void ofeliaCircle_set(t_ofeliaCircle *x, t_floatarg f)
{
    x->radius = f;
}

void ofeliaCircle_print(t_ofeliaCircle *x)
{
    post("\n[%s]", t_ofeliaCircle::objName);
    post("radius : %g", x->radius);
}

void ofeliaCircle_setup()
{
    ofeliaCircle_class = class_new(gensym("ofCircle"),
                                   reinterpret_cast<t_newmethod>(ofeliaCircle_new),
                                   0, sizeof(t_ofeliaCircle),
                                   CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaCircle_class, reinterpret_cast<t_method>(ofeliaCircle_bang));
    class_addmethod(ofeliaCircle_class, reinterpret_cast<t_method>(ofeliaCircle_set),
                    gensym("radius"), A_FLOAT, 0);
    class_addmethod(ofeliaCircle_class, reinterpret_cast<t_method>(ofeliaCircle_set),
                    gensym("set"), A_FLOAT, 0);
    class_addmethod(ofeliaCircle_class, reinterpret_cast<t_method>(ofeliaCircle_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofEllipse object methods
 */
void *ofeliaEllipse_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaEllipse *x = reinterpret_cast<t_ofeliaEllipse*>(pd_new(ofeliaEllipse_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getDimenFromArgs(argc, argv, x->dimen, t_ofeliaEllipse::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("width"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("height"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaEllipse_bang(t_ofeliaEllipse *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofGetCurrentRenderer()->drawEllipse(0.0f, 0.0f, 0.0f, x->dimen.width, x->dimen.height);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaEllipse::objName);
    }
}

void ofeliaEllipse_width(t_ofeliaEllipse *x, t_floatarg f)
{
    x->dimen.width = f;
}

void ofeliaEllipse_height(t_ofeliaEllipse *x, t_floatarg f)
{
    x->dimen.height = f;
}

void ofeliaEllipse_set(t_ofeliaEllipse *x, t_symbol *s, int argc, t_atom *argv)
{
    getDimenFromArgs(argc, argv, x->dimen, t_ofeliaEllipse::objName);
}

void ofeliaEllipse_print(t_ofeliaEllipse *x)
{
    post("\n[%s]", t_ofeliaEllipse::objName);
    post("width : %g", x->dimen.width);
    post("height : %g", x->dimen.height);
}

void ofeliaEllipse_setup()
{
    ofeliaEllipse_class = class_new(gensym("ofEllipse"),
                                    reinterpret_cast<t_newmethod>(ofeliaEllipse_new),
                                    0, sizeof(t_ofeliaEllipse),
                                    CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaEllipse_class, reinterpret_cast<t_method>(ofeliaEllipse_bang));
    class_addmethod(ofeliaEllipse_class, reinterpret_cast<t_method>(ofeliaEllipse_width),
                    gensym("width"), A_FLOAT, 0);
    class_addmethod(ofeliaEllipse_class, reinterpret_cast<t_method>(ofeliaEllipse_height),
                    gensym("height"), A_FLOAT, 0);
    class_addmethod(ofeliaEllipse_class, reinterpret_cast<t_method>(ofeliaEllipse_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaEllipse_class, reinterpret_cast<t_method>(ofeliaEllipse_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofArc object methods
 */
void *ofeliaArc_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaArc *x = reinterpret_cast<t_ofeliaArc*>(pd_new(ofeliaArc_class));
    x->path = make_unique<ofPath>();
    x->mesh = make_unique<ofVboMesh>();
    x->polyline = make_unique<ofPolyline>();
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getArcElemFromArgs(argc, argv, x->elem, t_ofeliaArc::objName))
        return 0;
    x->shouldLoad = true;
    x->bInitGate = false;
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("radiusX"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("radiusY"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("angleBegin"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("angleEnd"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaArc_init(t_ofeliaArc *x)
{
    if (!x->bInitGate)
        return;
    x->path = make_unique<ofPath>();
    x->mesh = make_unique<ofVboMesh>();
    x->polyline = make_unique<ofPolyline>();
    x->shouldLoad = true;
}

void ofeliaArc_exit(t_ofeliaArc *x)
{
    x->bInitGate = true;
}

void ofeliaArc_bang(t_ofeliaArc *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const ofStyle &style = ofGetStyle();
        
        if (x->circleResolution != style.circleResolution ||
            x->bFill != style.bFill) {
            
            x->circleResolution = style.circleResolution;
            x->bFill = style.bFill;
            x->shouldLoad = true;
        }
        if (x->shouldLoad) {
            
            x->polyline->clear();
            x->polyline->arc(ofVec2f(0.0f, 0.0f),
                             x->elem.radiusX, x->elem.radiusY,
                             x->elem.angleBegin, x->elem.angleEnd,
                             x->elem.angleBegin < x->elem.angleEnd, x->circleResolution);
            if (x->bFill) {
                
                x->path->clear();
                x->path->moveTo(x->polyline->getVertices()[0]);
                
                for (size_t i=1; i<x->polyline->getVertices().size(); ++i)
                    x->path->lineTo(x->polyline->getVertices()[i]);
                x->mesh->operator=(x->path->getTessellation());
            }
            x->shouldLoad = false;
        }
        if (!x->bFill)
            x->polyline->draw();
        else
            x->mesh->draw();
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaArc::objName);
    }
}

void ofeliaArc_radiusX(t_ofeliaArc *x, t_floatarg f)
{
    x->elem.radiusX = f;
    x->shouldLoad = true;
}

void ofeliaArc_radiusY(t_ofeliaArc *x, t_floatarg f)
{
    x->elem.radiusY = f;
    x->shouldLoad = true;
}

void ofeliaArc_angleBegin(t_ofeliaArc *x, t_floatarg f)
{
    x->elem.angleBegin = f;
    x->shouldLoad = true;
}

void ofeliaArc_angleEnd(t_ofeliaArc *x, t_floatarg f)
{
    x->elem.angleEnd = f;
    x->shouldLoad = true;
}

void ofeliaArc_set(t_ofeliaArc *x, t_symbol *s, int argc, t_atom *argv)
{
    if (getArcElemFromArgs(argc, argv, x->elem, t_ofeliaArc::objName))
        x->shouldLoad = true;
}

void ofeliaArc_print(t_ofeliaArc *x)
{
    post("\n[%s]", t_ofeliaArc::objName);
    post("radiusX : %g", x->elem.radiusX);
    post("radiusY : %g", x->elem.radiusY);
    post("angleBegin : %g", x->elem.angleBegin);
    post("angleEnd : %g", x->elem.angleEnd);
}

void ofeliaArc_free(t_ofeliaArc *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
}

void ofeliaArc_setup()
{
    ofeliaArc_class = class_new(gensym("ofArc"),
                                reinterpret_cast<t_newmethod>(ofeliaArc_new),
                                reinterpret_cast<t_method>(ofeliaArc_free),
                                sizeof(t_ofeliaArc),
                                CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaArc_class, reinterpret_cast<t_method>(ofeliaArc_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaArc_class, reinterpret_cast<t_method>(ofeliaArc_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addbang(ofeliaArc_class, reinterpret_cast<t_method>(ofeliaArc_bang));
    class_addmethod(ofeliaArc_class, reinterpret_cast<t_method>(ofeliaArc_radiusX),
                    gensym("radiusX"), A_FLOAT, 0);
    class_addmethod(ofeliaArc_class, reinterpret_cast<t_method>(ofeliaArc_radiusY),
                    gensym("radiusY"), A_FLOAT, 0);
    class_addmethod(ofeliaArc_class, reinterpret_cast<t_method>(ofeliaArc_angleBegin),
                    gensym("angleBegin"), A_FLOAT, 0);
    class_addmethod(ofeliaArc_class, reinterpret_cast<t_method>(ofeliaArc_angleEnd),
                    gensym("angleEnd"), A_FLOAT, 0);
    class_addmethod(ofeliaArc_class, reinterpret_cast<t_method>(ofeliaArc_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaArc_class, reinterpret_cast<t_method>(ofeliaArc_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSector object methods
 */
void *ofeliaSector_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSector *x = reinterpret_cast<t_ofeliaSector*>(pd_new(ofeliaSector_class));
    x->path = make_unique<ofPath>();
    x->mesh = make_unique<ofVboMesh>();
    x->polyline = make_unique<ofPolyline>();
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getArcElemFromArgs(argc, argv, x->elem, t_ofeliaSector::objName))
        return 0;
    x->shouldLoad = true;
    x->bInitGate = false;
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("radiusX"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("radiusY"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("angleBegin"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("angleEnd"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSector_init(t_ofeliaSector *x)
{
    if (!x->bInitGate)
        return;
    x->path = make_unique<ofPath>();
    x->mesh = make_unique<ofVboMesh>();
    x->polyline = make_unique<ofPolyline>();
    x->shouldLoad = true;
}

void ofeliaSector_exit(t_ofeliaSector *x)
{
    x->bInitGate = true;
}

void ofeliaSector_bang(t_ofeliaSector *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const ofStyle &style = ofGetStyle();
        
        if (x->circleResolution != style.circleResolution ||
            x->bFill != style.bFill) {
            
            x->circleResolution = style.circleResolution;
            x->bFill = style.bFill;
            x->shouldLoad = true;
        }
        if (x->shouldLoad) {
            
            x->polyline->clear();
            x->polyline->addVertex(ofVec2f(0.0f, 0.0f));
            x->polyline->arc(ofVec2f(0.0f, 0.0f),
                             x->elem.radiusX, x->elem.radiusY,
                             x->elem.angleBegin, x->elem.angleEnd,
                             x->elem.angleBegin < x->elem.angleEnd, x->circleResolution);
            x->polyline->close();
            
            if (x->bFill) {
                
                x->path->clear();
                x->path->moveTo(x->polyline->getVertices()[0]);
                
                for (size_t i=1; i<x->polyline->getVertices().size(); ++i)
                    x->path->lineTo(x->polyline->getVertices()[i]);
                x->mesh->operator=(x->path->getTessellation());
            }
            x->shouldLoad = false;
        }
        if (!x->bFill)
            x->polyline->draw();
        else
            x->mesh->draw();
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSector::objName);
    }
}

void ofeliaSector_radiusX(t_ofeliaSector *x, t_floatarg f)
{
    x->elem.radiusX = f;
    x->shouldLoad = true;
}

void ofeliaSector_radiusY(t_ofeliaSector *x, t_floatarg f)
{
    x->elem.radiusY = f;
    x->shouldLoad = true;
}

void ofeliaSector_angleBegin(t_ofeliaSector *x, t_floatarg f)
{
    x->elem.angleBegin = f;
    x->shouldLoad = true;
}

void ofeliaSector_angleEnd(t_ofeliaSector *x, t_floatarg f)
{
    x->elem.angleEnd = f;
    x->shouldLoad = true;
}

void ofeliaSector_set(t_ofeliaSector *x, t_symbol *s, int argc, t_atom *argv)
{
    if (getArcElemFromArgs(argc, argv, x->elem, t_ofeliaSector::objName))
        x->shouldLoad = true;
}

void ofeliaSector_print(t_ofeliaSector *x)
{
    post("\n[%s]", t_ofeliaSector::objName);
    post("radiusX : %g", x->elem.radiusX);
    post("radiusY : %g", x->elem.radiusY);
    post("angleBegin : %g", x->elem.angleBegin);
    post("angleEnd : %g", x->elem.angleEnd);
}

void ofeliaSector_free(t_ofeliaSector *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
}

void ofeliaSector_setup()
{
    ofeliaSector_class = class_new(gensym("ofSector"),
                                   reinterpret_cast<t_newmethod>(ofeliaSector_new),
                                   reinterpret_cast<t_method>(ofeliaSector_free),
                                   sizeof(t_ofeliaSector),
                                   CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaSector_class, reinterpret_cast<t_method>(ofeliaSector_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaSector_class, reinterpret_cast<t_method>(ofeliaSector_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addbang(ofeliaSector_class, reinterpret_cast<t_method>(ofeliaSector_bang));
    class_addmethod(ofeliaSector_class, reinterpret_cast<t_method>(ofeliaSector_radiusX),
                    gensym("radiusX"), A_FLOAT, 0);
    class_addmethod(ofeliaSector_class, reinterpret_cast<t_method>(ofeliaSector_radiusY),
                    gensym("radiusY"), A_FLOAT, 0);
    class_addmethod(ofeliaSector_class, reinterpret_cast<t_method>(ofeliaSector_angleBegin),
                    gensym("angleBegin"), A_FLOAT, 0);
    class_addmethod(ofeliaSector_class, reinterpret_cast<t_method>(ofeliaSector_angleEnd),
                    gensym("angleEnd"), A_FLOAT, 0);
    class_addmethod(ofeliaSector_class, reinterpret_cast<t_method>(ofeliaSector_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSector_class, reinterpret_cast<t_method>(ofeliaSector_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofLine2d object methods
 */
bool getLine2dPointsFromArgs(const int argc, const t_atom *argv, t_ofeliaLine2dPoints &points)
{
    switch (argc) {
            
        case 0:
        {
            points.p1.set(-OFELIA_DEFAULT_SIZE_HALF, 0.0f);
            points.p2.set(OFELIA_DEFAULT_SIZE_HALF, 0.0f);
            break;
        }
        case 4:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT) {
                
                points.p1.set(argv[0].a_w.w_float, argv[1].a_w.w_float);
                points.p2.set(argv[2].a_w.w_float, argv[3].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaLine2d::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaLine2d::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaLine2d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLine2d *x = reinterpret_cast<t_ofeliaLine2d*>(pd_new(ofeliaLine2d_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getLine2dPointsFromArgs(argc, argv, x->points))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p1"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p2"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaLine2d_bang(t_ofeliaLine2d *x)
{
    if (t_ofeliaWindow::bRenderGate) {

        ofGetCurrentRenderer()->drawLine(x->points.p1.x, x->points.p1.y, 0.0f,
                                         x->points.p2.x, x->points.p2.y, 0.0f);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaLine2d::objName);
    }
}

void ofeliaLine2d_p1(t_ofeliaLine2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            x->points.p1.set(argv[0].a_w.w_float, argv[1].a_w.w_float);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLine2d::objName);
    }
}

void ofeliaLine2d_p2(t_ofeliaLine2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            x->points.p2.set(argv[0].a_w.w_float, argv[1].a_w.w_float);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLine2d::objName);
    }
}

void ofeliaLine2d_set(t_ofeliaLine2d *x, t_symbol *s, int argc, t_atom *argv)
{
    getLine2dPointsFromArgs(argc, argv, x->points);
}

void ofeliaLine2d_print(t_ofeliaLine2d *x)
{
    post("\n[%s]", t_ofeliaLine2d::objName);
    post("p1 : %g %g", x->points.p1.x, x->points.p1.y);
    post("p2 : %g %g", x->points.p2.x, x->points.p2.y);
}

void ofeliaLine2d_setup()
{
    ofeliaLine2d_class = class_new(gensym("ofLine2d"),
                                   reinterpret_cast<t_newmethod>(ofeliaLine2d_new),
                                   0, sizeof(t_ofeliaLine2d),
                                   CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaLine2d_class, reinterpret_cast<t_method>(ofeliaLine2d_bang));
    class_addmethod(ofeliaLine2d_class, reinterpret_cast<t_method>(ofeliaLine2d_p1),
                    gensym("p1"), A_GIMME, 0);
    class_addmethod(ofeliaLine2d_class, reinterpret_cast<t_method>(ofeliaLine2d_p2),
                    gensym("p2"), A_GIMME, 0);
    class_addmethod(ofeliaLine2d_class, reinterpret_cast<t_method>(ofeliaLine2d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaLine2d_class, reinterpret_cast<t_method>(ofeliaLine2d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofLine3d object methods
 */
bool getLine3dPointsFromArgs(const int argc, const t_atom *argv, t_ofeliaLine3dPoints &points)
{
    switch (argc) {
            
        case 0:
        {
            points.p1.set(-OFELIA_DEFAULT_SIZE_HALF, 0.0f, 0.0f);
            points.p2.set(OFELIA_DEFAULT_SIZE_HALF, 0.0f, 0.0f);
            break;
        }
        case 6:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT &&
                argv[4].a_type == A_FLOAT &&
                argv[5].a_type == A_FLOAT) {
                
                points.p1.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                points.p2.set(argv[3].a_w.w_float, argv[4].a_w.w_float, argv[5].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaLine3d::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaLine3d::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaLine3d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLine3d *x = reinterpret_cast<t_ofeliaLine3d*>(pd_new(ofeliaLine3d_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getLine3dPointsFromArgs(argc, argv, x->points))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p1"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p2"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaLine3d_bang(t_ofeliaLine3d *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofGetCurrentRenderer()->drawLine(x->points.p1.x, x->points.p1.y, x->points.p1.z,
                                         x->points.p2.x, x->points.p2.y, x->points.p2.z);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaLine3d::objName);
    }
}

void ofeliaLine3d_p1(t_ofeliaLine3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            x->points.p1.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLine3d::objName);
    }
}

void ofeliaLine3d_p2(t_ofeliaLine3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            x->points.p2.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLine3d::objName);
    }
}

void ofeliaLine3d_set(t_ofeliaLine3d *x, t_symbol *s, int argc, t_atom *argv)
{
    getLine3dPointsFromArgs(argc, argv, x->points);
}

void ofeliaLine3d_print(t_ofeliaLine3d *x)
{
    post("\n[%s]", t_ofeliaLine3d::objName);
    post("p1 : %g %g %g", x->points.p1.x, x->points.p1.y, x->points.p1.z);
    post("p2 : %g %g %g", x->points.p2.x, x->points.p2.y, x->points.p2.z);
}

void ofeliaLine3d_setup()
{
    ofeliaLine3d_class = class_new(gensym("ofLine3d"),
                                   reinterpret_cast<t_newmethod>(ofeliaLine3d_new),
                                   0, sizeof(t_ofeliaLine3d),
                                   CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaLine3d_class, reinterpret_cast<t_method>(ofeliaLine3d_bang));
    class_addmethod(ofeliaLine3d_class, reinterpret_cast<t_method>(ofeliaLine3d_p1),
                    gensym("p1"), A_GIMME, 0);
    class_addmethod(ofeliaLine3d_class, reinterpret_cast<t_method>(ofeliaLine3d_p2),
                    gensym("p2"), A_GIMME, 0);
    class_addmethod(ofeliaLine3d_class, reinterpret_cast<t_method>(ofeliaLine3d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaLine3d_class, reinterpret_cast<t_method>(ofeliaLine3d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofCurve2d object methods
 */
bool getCurve2dPointsFromArgs(const int argc, const t_atom *argv, t_ofeliaCurve2dPoints &points)
{
    switch (argc) {
            
        case 0:
        {
            points.p0.set(-OFELIA_DEFAULT_SIZE_DOUBLE, t_ofeliaShapesDefault::curveControlY);
            points.p1.set(-OFELIA_DEFAULT_SIZE_HALF, 0.0f);
            points.p2.set(OFELIA_DEFAULT_SIZE_HALF, 0.0f);
            points.p3.set(OFELIA_DEFAULT_SIZE_DOUBLE, -t_ofeliaShapesDefault::curveControlY);
            break;
        }
        case 8:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT &&
                argv[4].a_type == A_FLOAT &&
                argv[5].a_type == A_FLOAT &&
                argv[6].a_type == A_FLOAT &&
                argv[7].a_type == A_FLOAT) {
                
                points.p0.set(argv[0].a_w.w_float, argv[1].a_w.w_float);
                points.p1.set(argv[2].a_w.w_float, argv[3].a_w.w_float);
                points.p2.set(argv[4].a_w.w_float, argv[5].a_w.w_float);
                points.p3.set(argv[6].a_w.w_float, argv[7].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaCurve2d::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaCurve2d::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaCurve2d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaCurve2d *x = reinterpret_cast<t_ofeliaCurve2d*>(pd_new(ofeliaCurve2d_class));
    x->path = make_unique<ofPath>();
    x->mesh = make_unique<ofVboMesh>();
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getCurve2dPointsFromArgs(argc, argv, x->points))
        return 0;
    x->shouldLoad = true;
    x->bInitGate = false;
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p0"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p1"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p2"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p3"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaCurve2d_init(t_ofeliaCurve2d *x)
{
    if (!x->bInitGate)
        return;
    x->path = make_unique<ofPath>();
    x->mesh = make_unique<ofVboMesh>();
    x->shouldLoad = true;
}

void ofeliaCurve2d_exit(t_ofeliaCurve2d *x)
{
    x->bInitGate = true;
}

void ofeliaCurve2d_bang(t_ofeliaCurve2d *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const ofStyle &style = ofGetStyle();
        
        if (x->curveResolution != style.curveResolution ||
            x->bFill != style.bFill) {
            
            x->curveResolution = style.curveResolution;
            x->bFill = style.bFill;
            x->shouldLoad = true;
        }
        if (x->shouldLoad && x->bFill) {
            
            x->path->clear();
            x->path->setCurveResolution(x->curveResolution);
            x->path->moveTo(x->points.p1);
            x->path->curveTo(x->points.p0);
            x->path->curveTo(x->points.p1);
            x->path->curveTo(x->points.p2);
            x->path->curveTo(x->points.p3);
            x->path->close();
            x->mesh->operator=(x->path->getTessellation());
            x->shouldLoad = false;
        }
        if (!x->bFill)
            ofDrawCurve(x->points.p0.x, x->points.p0.y,
                        x->points.p1.x, x->points.p1.y,
                        x->points.p2.x, x->points.p2.y,
                        x->points.p3.x, x->points.p3.y);
        else
            x->mesh->draw();
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaCurve2d::objName);
    }
}

void ofeliaCurve2d_p0(t_ofeliaCurve2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            x->points.p0.set(argv[0].a_w.w_float, argv[1].a_w.w_float);
            x->shouldLoad = true;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaCurve2d::objName);
    }
}

void ofeliaCurve2d_p1(t_ofeliaCurve2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            x->points.p1.set(argv[0].a_w.w_float, argv[1].a_w.w_float);
            x->shouldLoad = true;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaCurve2d::objName);
    }
}

void ofeliaCurve2d_p2(t_ofeliaCurve2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            x->points.p2.set(argv[0].a_w.w_float, argv[1].a_w.w_float);
            x->shouldLoad = true;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaCurve2d::objName);
    }
}

void ofeliaCurve2d_p3(t_ofeliaCurve2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            x->points.p3.set(argv[0].a_w.w_float, argv[1].a_w.w_float);
            x->shouldLoad = true;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaCurve2d::objName);
    }
}

void ofeliaCurve2d_set(t_ofeliaCurve2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (getCurve2dPointsFromArgs(argc, argv, x->points))
        x->shouldLoad = true;
}

void ofeliaCurve2d_print(t_ofeliaCurve2d *x)
{
    post("\n[%s]", t_ofeliaCurve2d::objName);
    post("p0 : %g %g", x->points.p0.x, x->points.p0.y);
    post("p1 : %g %g", x->points.p1.x, x->points.p1.y);
    post("p2 : %g %g", x->points.p2.x, x->points.p2.y);
    post("p3 : %g %g", x->points.p3.x, x->points.p3.y);
}

void ofeliaCurve2d_free(t_ofeliaCurve2d *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
}

void ofeliaCurve2d_setup()
{
    ofeliaCurve2d_class = class_new(gensym("ofCurve2d"),
                                    reinterpret_cast<t_newmethod>(ofeliaCurve2d_new),
                                    reinterpret_cast<t_method>(ofeliaCurve2d_free),
                                    sizeof(t_ofeliaCurve2d),
                                    CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaCurve2d_class, reinterpret_cast<t_method>(ofeliaCurve2d_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaCurve2d_class, reinterpret_cast<t_method>(ofeliaCurve2d_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addbang(ofeliaCurve2d_class, reinterpret_cast<t_method>(ofeliaCurve2d_bang));
    class_addmethod(ofeliaCurve2d_class, reinterpret_cast<t_method>(ofeliaCurve2d_p0),
                    gensym("p0"), A_GIMME, 0);
    class_addmethod(ofeliaCurve2d_class, reinterpret_cast<t_method>(ofeliaCurve2d_p1),
                    gensym("p1"), A_GIMME, 0);
    class_addmethod(ofeliaCurve2d_class, reinterpret_cast<t_method>(ofeliaCurve2d_p2),
                    gensym("p2"), A_GIMME, 0);
    class_addmethod(ofeliaCurve2d_class, reinterpret_cast<t_method>(ofeliaCurve2d_p3),
                    gensym("p3"), A_GIMME, 0);
    class_addmethod(ofeliaCurve2d_class, reinterpret_cast<t_method>(ofeliaCurve2d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaCurve2d_class, reinterpret_cast<t_method>(ofeliaCurve2d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofCurve3d object methods
 */
bool getCurve3dPointsFromArgs(const int argc, const t_atom *argv, t_ofeliaCurve3dPoints &points)
{
    switch (argc) {
            
        case 0:
        {
            points.p0.set(-OFELIA_DEFAULT_SIZE_DOUBLE, t_ofeliaShapesDefault::curveControlY, 0.0f);
            points.p1.set(-OFELIA_DEFAULT_SIZE_HALF, 0.0f, 0.0f);
            points.p2.set(OFELIA_DEFAULT_SIZE_HALF, 0.0f, 0.0f);
            points.p3.set(OFELIA_DEFAULT_SIZE_DOUBLE, -t_ofeliaShapesDefault::curveControlY, 0.0f);
            break;
        }
        case 12:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT &&
                argv[4].a_type == A_FLOAT &&
                argv[5].a_type == A_FLOAT &&
                argv[6].a_type == A_FLOAT &&
                argv[7].a_type == A_FLOAT &&
                argv[8].a_type == A_FLOAT &&
                argv[9].a_type == A_FLOAT &&
                argv[10].a_type == A_FLOAT &&
                argv[11].a_type == A_FLOAT) {
                
                points.p0.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                points.p1.set(argv[3].a_w.w_float, argv[4].a_w.w_float, argv[5].a_w.w_float);
                points.p2.set(argv[6].a_w.w_float, argv[7].a_w.w_float, argv[8].a_w.w_float);
                points.p3.set(argv[9].a_w.w_float, argv[10].a_w.w_float, argv[11].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaCurve3d::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaCurve3d::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaCurve3d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaCurve3d *x = reinterpret_cast<t_ofeliaCurve3d*>(pd_new(ofeliaCurve3d_class));
    x->path = make_unique<ofPath>();
    x->mesh = make_unique<ofVboMesh>();
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getCurve3dPointsFromArgs(argc, argv, x->points))
        return 0;
    x->shouldLoad = true;
    x->bInitGate = false;
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p0"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p1"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p2"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p3"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaCurve3d_init(t_ofeliaCurve3d *x)
{
    if (!x->bInitGate)
        return;
    x->path = make_unique<ofPath>();
    x->mesh = make_unique<ofVboMesh>();
    x->shouldLoad = true;
}

void ofeliaCurve3d_exit(t_ofeliaCurve3d *x)
{
    x->bInitGate = true;
}

void ofeliaCurve3d_bang(t_ofeliaCurve3d *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const ofStyle &style = ofGetStyle();
        
        if (x->curveResolution != style.curveResolution ||
            x->bFill != style.bFill) {
            
            x->curveResolution = style.curveResolution;
            x->bFill = style.bFill;
            x->shouldLoad = true;
        }
        if (x->shouldLoad && x->bFill) {
            
            x->path->clear();
            x->path->setCurveResolution(x->curveResolution);
            x->path->moveTo(x->points.p1);
            x->path->curveTo(x->points.p0);
            x->path->curveTo(x->points.p1);
            x->path->curveTo(x->points.p2);
            x->path->curveTo(x->points.p3);
            x->path->close();
            x->mesh->operator=(x->path->getTessellation());
            x->shouldLoad = false;
        }
        if (!x->bFill)
            ofDrawCurve(x->points.p0.x, x->points.p0.y, x->points.p0.z,
                        x->points.p1.x, x->points.p1.y, x->points.p1.z,
                        x->points.p2.x, x->points.p2.y, x->points.p2.z,
                        x->points.p3.x, x->points.p3.y, x->points.p3.z);
        else
            x->mesh->draw();
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaCurve3d::objName);
    }
}

void ofeliaCurve3d_p0(t_ofeliaCurve3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            x->points.p0.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
            x->shouldLoad = true;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaCurve3d::objName);
    }
}

void ofeliaCurve3d_p1(t_ofeliaCurve3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            x->points.p1.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
            x->shouldLoad = true;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaCurve3d::objName);
    }
}

void ofeliaCurve3d_p2(t_ofeliaCurve3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            x->points.p2.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
            x->shouldLoad = true;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaCurve3d::objName);
    }
}

void ofeliaCurve3d_p3(t_ofeliaCurve3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            x->points.p3.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
            x->shouldLoad = true;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaCurve3d::objName);
    }
}

void ofeliaCurve3d_set(t_ofeliaCurve3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (getCurve3dPointsFromArgs(argc, argv, x->points))
        x->shouldLoad = true;
}

void ofeliaCurve3d_print(t_ofeliaCurve3d *x)
{
    post("\n[%s]", t_ofeliaCurve3d::objName);
    post("p0 : %g %g %g", x->points.p0.x, x->points.p0.y, x->points.p0.z);
    post("p1 : %g %g %g", x->points.p1.x, x->points.p1.y, x->points.p1.z);
    post("p2 : %g %g %g", x->points.p2.x, x->points.p2.y, x->points.p2.z);
    post("p3 : %g %g %g", x->points.p3.x, x->points.p3.y, x->points.p3.z);
}

void ofeliaCurve3d_free(t_ofeliaCurve3d *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
}

void ofeliaCurve3d_setup()
{
    ofeliaCurve3d_class = class_new(gensym("ofCurve3d"),
                                    reinterpret_cast<t_newmethod>(ofeliaCurve3d_new),
                                    reinterpret_cast<t_method>(ofeliaCurve3d_free),
                                    sizeof(t_ofeliaCurve3d),
                                    CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaCurve3d_class, reinterpret_cast<t_method>(ofeliaCurve3d_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaCurve3d_class, reinterpret_cast<t_method>(ofeliaCurve3d_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addbang(ofeliaCurve3d_class, reinterpret_cast<t_method>(ofeliaCurve3d_bang));
    class_addmethod(ofeliaCurve3d_class, reinterpret_cast<t_method>(ofeliaCurve3d_p0),
                    gensym("p0"), A_GIMME, 0);
    class_addmethod(ofeliaCurve3d_class, reinterpret_cast<t_method>(ofeliaCurve3d_p1),
                    gensym("p1"), A_GIMME, 0);
    class_addmethod(ofeliaCurve3d_class, reinterpret_cast<t_method>(ofeliaCurve3d_p2),
                    gensym("p2"), A_GIMME, 0);
    class_addmethod(ofeliaCurve3d_class, reinterpret_cast<t_method>(ofeliaCurve3d_p3),
                    gensym("p3"), A_GIMME, 0);
    class_addmethod(ofeliaCurve3d_class, reinterpret_cast<t_method>(ofeliaCurve3d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaCurve3d_class, reinterpret_cast<t_method>(ofeliaCurve3d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofBezier2d object methods
 */
void *ofeliaBezier2d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaBezier2d *x = reinterpret_cast<t_ofeliaBezier2d*>(pd_new(ofeliaBezier2d_class));
    x->path = make_unique<ofPath>();
    x->mesh = make_unique<ofVboMesh>();
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getBezier2dPointsFromArgs(argc, argv, x->points, t_ofeliaBezier2d::objName))
        return 0;
    x->shouldLoad = true;
    x->bInitGate = false;
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p0"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p1"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p2"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p3"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaBezier2d_init(t_ofeliaBezier2d *x)
{
    if (!x->bInitGate)
        return;
    x->path = make_unique<ofPath>();
    x->mesh = make_unique<ofVboMesh>();
    x->shouldLoad = true;
}

void ofeliaBezier2d_exit(t_ofeliaBezier2d *x)
{
    x->bInitGate = true;
}

void ofeliaBezier2d_bang(t_ofeliaBezier2d *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const ofStyle &style = ofGetStyle();
        
        if (x->curveResolution != style.curveResolution ||
            x->bFill != style.bFill) {
            
            x->curveResolution = style.curveResolution;
            x->bFill = style.bFill;
            x->shouldLoad = true;
        }
        if (x->shouldLoad && x->bFill) {
            
            x->path->clear();
            x->path->setCurveResolution(x->curveResolution);
            x->path->moveTo(x->points.p0);
            x->path->bezierTo(x->points.p1, x->points.p2, x->points.p3);
            x->path->close();
            x->mesh->operator=(x->path->getTessellation());
            x->shouldLoad = false;
        }
        if (!x->bFill)
            ofDrawBezier(x->points.p0.x, x->points.p0.y,
                         x->points.p1.x, x->points.p1.y,
                         x->points.p2.x, x->points.p2.y,
                         x->points.p3.x, x->points.p3.y);
        else
            x->mesh->draw();
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaBezier2d::objName);
    }
}

void ofeliaBezier2d_p0(t_ofeliaBezier2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            x->points.p0.set(argv[0].a_w.w_float, argv[1].a_w.w_float);
            x->shouldLoad = true;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaBezier2d::objName);
    }
}

void ofeliaBezier2d_p1(t_ofeliaBezier2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            x->points.p1.set(argv[0].a_w.w_float, argv[1].a_w.w_float);
            x->shouldLoad = true;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaBezier2d::objName);
    }
}

void ofeliaBezier2d_p2(t_ofeliaBezier2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            x->points.p2.set(argv[0].a_w.w_float, argv[1].a_w.w_float);
            x->shouldLoad = true;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaBezier2d::objName);
    }
}

void ofeliaBezier2d_p3(t_ofeliaBezier2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            x->points.p3.set(argv[0].a_w.w_float, argv[1].a_w.w_float);
            x->shouldLoad = true;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaBezier2d::objName);
    }
}

void ofeliaBezier2d_set(t_ofeliaBezier2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (getBezier2dPointsFromArgs(argc, argv, x->points, t_ofeliaBezier2d::objName))
        x->shouldLoad = true;
}

void ofeliaBezier2d_print(t_ofeliaBezier2d *x)
{
    post("\n[%s]", t_ofeliaBezier2d::objName);
    post("p0 : %g %g", x->points.p0.x, x->points.p0.y);
    post("p1 : %g %g", x->points.p1.x, x->points.p1.y);
    post("p2 : %g %g", x->points.p2.x, x->points.p2.y);
    post("p3 : %g %g", x->points.p3.x, x->points.p3.y);
}

void ofeliaBezier2d_free(t_ofeliaBezier2d *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
}

void ofeliaBezier2d_setup()
{
    ofeliaBezier2d_class = class_new(gensym("ofBezier2d"),
                                     reinterpret_cast<t_newmethod>(ofeliaBezier2d_new),
                                     reinterpret_cast<t_method>(ofeliaBezier2d_free),
                                     sizeof(t_ofeliaBezier2d),
                                     CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaBezier2d_class, reinterpret_cast<t_method>(ofeliaBezier2d_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaBezier2d_class, reinterpret_cast<t_method>(ofeliaBezier2d_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addbang(ofeliaBezier2d_class, reinterpret_cast<t_method>(ofeliaBezier2d_bang));
    class_addmethod(ofeliaBezier2d_class, reinterpret_cast<t_method>(ofeliaBezier2d_p0),
                    gensym("p0"), A_GIMME, 0);
    class_addmethod(ofeliaBezier2d_class, reinterpret_cast<t_method>(ofeliaBezier2d_p1),
                    gensym("p1"), A_GIMME, 0);
    class_addmethod(ofeliaBezier2d_class, reinterpret_cast<t_method>(ofeliaBezier2d_p2),
                    gensym("p2"), A_GIMME, 0);
    class_addmethod(ofeliaBezier2d_class, reinterpret_cast<t_method>(ofeliaBezier2d_p3),
                    gensym("p3"), A_GIMME, 0);
    class_addmethod(ofeliaBezier2d_class, reinterpret_cast<t_method>(ofeliaBezier2d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaBezier2d_class, reinterpret_cast<t_method>(ofeliaBezier2d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofBezier3d object methods
 */
void *ofeliaBezier3d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaBezier3d *x = reinterpret_cast<t_ofeliaBezier3d*>(pd_new(ofeliaBezier3d_class));
    x->path = make_unique<ofPath>();
    x->mesh = make_unique<ofVboMesh>();
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getBezier3dPointsFromArgs(argc, argv, x->points, t_ofeliaBezier3d::objName))
        return 0;
    x->shouldLoad = true;
    x->bInitGate = false;
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p0"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p1"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p2"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p3"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaBezier3d_init(t_ofeliaBezier3d *x)
{
    if (!x->bInitGate)
        return;
    x->path = make_unique<ofPath>();
    x->mesh = make_unique<ofVboMesh>();
    x->shouldLoad = true;
}

void ofeliaBezier3d_exit(t_ofeliaBezier3d *x)
{
    x->bInitGate = true;
}

void ofeliaBezier3d_bang(t_ofeliaBezier3d *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const ofStyle &style = ofGetStyle();
        
        if (x->curveResolution != style.curveResolution ||
            x->bFill != style.bFill) {
            
            x->curveResolution = style.curveResolution;
            x->bFill = style.bFill;
            x->shouldLoad = true;
        }
        if (x->shouldLoad && x->bFill) {
            
            x->path->clear();
            x->path->setCurveResolution(x->curveResolution);
            x->path->moveTo(x->points.p0);
            x->path->bezierTo(x->points.p1, x->points.p2, x->points.p3);
            x->path->close();
            x->mesh->operator=(x->path->getTessellation());
            x->shouldLoad = false;
        }
        if (!x->bFill)
            ofDrawBezier(x->points.p0.x, x->points.p0.y, x->points.p0.z,
                         x->points.p1.x, x->points.p1.y, x->points.p1.z,
                         x->points.p2.x, x->points.p2.y, x->points.p2.z,
                         x->points.p3.x, x->points.p3.y, x->points.p3.z);
        else
            x->mesh->draw();
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaBezier3d::objName);
    }
}

void ofeliaBezier3d_p0(t_ofeliaBezier3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            x->points.p0.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
            x->shouldLoad = true;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaBezier3d::objName);
    }
}

void ofeliaBezier3d_p1(t_ofeliaBezier3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            x->points.p1.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
            x->shouldLoad = true;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaBezier3d::objName);
    }
}

void ofeliaBezier3d_p2(t_ofeliaBezier3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            x->points.p2.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
            x->shouldLoad = true;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaBezier3d::objName);
    }
}

void ofeliaBezier3d_p3(t_ofeliaBezier3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            x->points.p3.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
            x->shouldLoad = true;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaBezier3d::objName);
    }
}

void ofeliaBezier3d_set(t_ofeliaBezier3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (getBezier3dPointsFromArgs(argc, argv, x->points, t_ofeliaBezier3d::objName))
        x->shouldLoad = true;
}

void ofeliaBezier3d_print(t_ofeliaBezier3d *x)
{
    post("\n[%s]", t_ofeliaBezier3d::objName);
    post("p0 : %g %g %g", x->points.p0.x, x->points.p0.y, x->points.p0.z);
    post("p1 : %g %g %g", x->points.p1.x, x->points.p1.y, x->points.p1.z);
    post("p2 : %g %g %g", x->points.p2.x, x->points.p2.y, x->points.p2.z);
    post("p3 : %g %g %g", x->points.p3.x, x->points.p3.y, x->points.p3.z);
}

void ofeliaBezier3d_free(t_ofeliaBezier3d *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
}

void ofeliaBezier3d_setup()
{
    ofeliaBezier3d_class = class_new(gensym("ofBezier3d"),
                                     reinterpret_cast<t_newmethod>(ofeliaBezier3d_new),
                                     reinterpret_cast<t_method>(ofeliaBezier3d_free),
                                     sizeof(t_ofeliaBezier3d),
                                     CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaBezier3d_class, reinterpret_cast<t_method>(ofeliaBezier3d_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaBezier3d_class, reinterpret_cast<t_method>(ofeliaBezier3d_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addbang(ofeliaBezier3d_class, reinterpret_cast<t_method>(ofeliaBezier3d_bang));
    class_addmethod(ofeliaBezier3d_class, reinterpret_cast<t_method>(ofeliaBezier3d_p0),
                    gensym("p0"), A_GIMME, 0);
    class_addmethod(ofeliaBezier3d_class, reinterpret_cast<t_method>(ofeliaBezier3d_p1),
                    gensym("p1"), A_GIMME, 0);
    class_addmethod(ofeliaBezier3d_class, reinterpret_cast<t_method>(ofeliaBezier3d_p2),
                    gensym("p2"), A_GIMME, 0);
    class_addmethod(ofeliaBezier3d_class, reinterpret_cast<t_method>(ofeliaBezier3d_p3),
                    gensym("p3"), A_GIMME, 0);
    class_addmethod(ofeliaBezier3d_class, reinterpret_cast<t_method>(ofeliaBezier3d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaBezier3d_class, reinterpret_cast<t_method>(ofeliaBezier3d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofQuadBezier2d object methods
 */
void *ofeliaQuadBezier2d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaQuadBezier2d *x = reinterpret_cast<t_ofeliaQuadBezier2d*>(pd_new(ofeliaQuadBezier2d_class));
    x->path = make_unique<ofPath>();
    x->mesh = make_unique<ofVboMesh>();
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getBezier2dPointsFromArgs(argc, argv, x->points, t_ofeliaQuadBezier2d::objName))
        return 0;
    x->shouldLoad = true;
    x->bInitGate = false;
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p0"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p1"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p2"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p3"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaQuadBezier2d_init(t_ofeliaQuadBezier2d *x)
{
    if (!x->bInitGate)
        return;
    x->path = make_unique<ofPath>();
    x->mesh = make_unique<ofVboMesh>();
    x->shouldLoad = true;
}

void ofeliaQuadBezier2d_exit(t_ofeliaQuadBezier2d *x)
{
    x->bInitGate = true;
}

void ofeliaQuadBezier2d_bang(t_ofeliaQuadBezier2d *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const ofStyle &style = ofGetStyle();
        
        if (x->curveResolution != style.curveResolution ||
            x->bFill != style.bFill) {
            
            x->curveResolution = style.curveResolution;
            x->bFill = style.bFill;
            x->shouldLoad = true;
        }
        if (x->shouldLoad && x->bFill) {
            
            x->path->clear();
            x->path->setCurveResolution(x->curveResolution);
            x->path->moveTo(x->points.p0);
            x->path->quadBezierTo(x->points.p1, x->points.p2, x->points.p3);
            x->path->close();
            x->mesh->operator=(x->path->getTessellation());
            x->shouldLoad = false;
        }
        if (!x->bFill) {
            
            ofGetCurrentRenderer()->getPath().clear();
            ofGetCurrentRenderer()->getPath().moveTo(x->points.p0);
            ofGetCurrentRenderer()->getPath().quadBezierTo(x->points.p1, x->points.p2, x->points.p3);
            ofGetCurrentRenderer()->draw(ofGetCurrentRenderer()->getPath());
        }
        else
            x->mesh->draw();
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaQuadBezier2d::objName);
    }
}

void ofeliaQuadBezier2d_p0(t_ofeliaQuadBezier2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            x->points.p0.set(argv[0].a_w.w_float, argv[1].a_w.w_float);
            x->shouldLoad = true;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaQuadBezier2d::objName);
    }
}

void ofeliaQuadBezier2d_p1(t_ofeliaQuadBezier2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            x->points.p1.set(argv[0].a_w.w_float, argv[1].a_w.w_float);
            x->shouldLoad = true;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaQuadBezier2d::objName);
    }
}

void ofeliaQuadBezier2d_p2(t_ofeliaQuadBezier2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            x->points.p2.set(argv[0].a_w.w_float, argv[1].a_w.w_float);
            x->shouldLoad = true;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaQuadBezier2d::objName);
    }
}

void ofeliaQuadBezier2d_p3(t_ofeliaQuadBezier2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            x->points.p3.set(argv[0].a_w.w_float, argv[1].a_w.w_float);
            x->shouldLoad = true;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaQuadBezier2d::objName);
    }
}

void ofeliaQuadBezier2d_set(t_ofeliaQuadBezier2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (getBezier2dPointsFromArgs(argc, argv, x->points, t_ofeliaQuadBezier2d::objName))
        x->shouldLoad = true;
}

void ofeliaQuadBezier2d_print(t_ofeliaQuadBezier2d *x)
{
    post("\n[%s]", t_ofeliaQuadBezier2d::objName);
    post("p0 : %g %g", x->points.p0.x, x->points.p0.y);
    post("p1 : %g %g", x->points.p1.x, x->points.p1.y);
    post("p2 : %g %g", x->points.p2.x, x->points.p2.y);
    post("p3 : %g %g", x->points.p3.x, x->points.p3.y);
}

void ofeliaQuadBezier2d_free(t_ofeliaQuadBezier2d *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
}

void ofeliaQuadBezier2d_setup()
{
    ofeliaQuadBezier2d_class = class_new(gensym("ofQuadBezier2d"),
                                         reinterpret_cast<t_newmethod>(ofeliaQuadBezier2d_new),
                                         reinterpret_cast<t_method>(ofeliaQuadBezier2d_free),
                                         sizeof(t_ofeliaQuadBezier2d),
                                         CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaQuadBezier2d_class, reinterpret_cast<t_method>(ofeliaQuadBezier2d_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaQuadBezier2d_class, reinterpret_cast<t_method>(ofeliaQuadBezier2d_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addbang(ofeliaQuadBezier2d_class, reinterpret_cast<t_method>(ofeliaQuadBezier2d_bang));
    class_addmethod(ofeliaQuadBezier2d_class, reinterpret_cast<t_method>(ofeliaQuadBezier2d_p0),
                    gensym("p0"), A_GIMME, 0);
    class_addmethod(ofeliaQuadBezier2d_class, reinterpret_cast<t_method>(ofeliaQuadBezier2d_p1),
                    gensym("p1"), A_GIMME, 0);
    class_addmethod(ofeliaQuadBezier2d_class, reinterpret_cast<t_method>(ofeliaQuadBezier2d_p2),
                    gensym("p2"), A_GIMME, 0);
    class_addmethod(ofeliaQuadBezier2d_class, reinterpret_cast<t_method>(ofeliaQuadBezier2d_p3),
                    gensym("p3"), A_GIMME, 0);
    class_addmethod(ofeliaQuadBezier2d_class, reinterpret_cast<t_method>(ofeliaQuadBezier2d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaQuadBezier2d_class, reinterpret_cast<t_method>(ofeliaQuadBezier2d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofQuadBezier3d object methods
 */
void *ofeliaQuadBezier3d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaQuadBezier3d *x = reinterpret_cast<t_ofeliaQuadBezier3d*>(pd_new(ofeliaQuadBezier3d_class));
    x->path = make_unique<ofPath>();
    x->mesh = make_unique<ofVboMesh>();
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getBezier3dPointsFromArgs(argc, argv, x->points, t_ofeliaQuadBezier3d::objName))
        return 0;
    x->shouldLoad = true;
    x->bInitGate = false;
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p0"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p1"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p2"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p3"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaQuadBezier3d_init(t_ofeliaQuadBezier3d *x)
{
    if (!x->bInitGate)
        return;
    x->path = make_unique<ofPath>();
    x->mesh = make_unique<ofVboMesh>();
    x->shouldLoad = true;
}

void ofeliaQuadBezier3d_exit(t_ofeliaQuadBezier3d *x)
{
    x->bInitGate = true;
}

void ofeliaQuadBezier3d_bang(t_ofeliaQuadBezier3d *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const ofStyle &style = ofGetStyle();
        
        if (x->curveResolution != style.curveResolution ||
            x->bFill != style.bFill) {
            
            x->curveResolution = style.curveResolution;
            x->bFill = style.bFill;
            x->shouldLoad = true;
        }
        if (x->shouldLoad && x->bFill) {
            
            x->path->clear();
            x->path->setCurveResolution(x->curveResolution);
            x->path->moveTo(x->points.p0);
            x->path->quadBezierTo(x->points.p1, x->points.p2, x->points.p3);
            x->path->close();
            x->mesh->operator=(x->path->getTessellation());
            x->shouldLoad = false;
        }
        if (!x->bFill) {
            
            ofGetCurrentRenderer()->getPath().clear();
            ofGetCurrentRenderer()->getPath().moveTo(x->points.p0);
            ofGetCurrentRenderer()->getPath().quadBezierTo(x->points.p1, x->points.p2, x->points.p3);
            ofGetCurrentRenderer()->draw(ofGetCurrentRenderer()->getPath());
        }
        else
            x->mesh->draw();
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaQuadBezier3d::objName);
    }
}

void ofeliaQuadBezier3d_p0(t_ofeliaQuadBezier3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            x->points.p0.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
            x->shouldLoad = true;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaQuadBezier3d::objName);
    }
}

void ofeliaQuadBezier3d_p1(t_ofeliaQuadBezier3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            x->points.p1.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
            x->shouldLoad = true;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaQuadBezier3d::objName);
    }
}

void ofeliaQuadBezier3d_p2(t_ofeliaQuadBezier3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            x->points.p2.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
            x->shouldLoad = true;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaQuadBezier3d::objName);
    }
}

void ofeliaQuadBezier3d_p3(t_ofeliaQuadBezier3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            x->points.p3.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
            x->shouldLoad = true;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaQuadBezier3d::objName);
    }
}

void ofeliaQuadBezier3d_set(t_ofeliaQuadBezier3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (getBezier3dPointsFromArgs(argc, argv, x->points, t_ofeliaQuadBezier3d::objName))
        x->shouldLoad = true;
}

void ofeliaQuadBezier3d_print(t_ofeliaQuadBezier3d *x)
{
    post("\n[%s]", t_ofeliaQuadBezier3d::objName);
    post("p0 : %g %g %g", x->points.p0.x, x->points.p0.y, x->points.p0.z);
    post("p1 : %g %g %g", x->points.p1.x, x->points.p1.y, x->points.p1.z);
    post("p2 : %g %g %g", x->points.p2.x, x->points.p2.y, x->points.p2.z);
    post("p3 : %g %g %g", x->points.p3.x, x->points.p3.y, x->points.p3.z);
}

void ofeliaQuadBezier3d_free(t_ofeliaQuadBezier3d *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
}

void ofeliaQuadBezier3d_setup()
{
    ofeliaQuadBezier3d_class = class_new(gensym("ofQuadBezier3d"),
                                         reinterpret_cast<t_newmethod>(ofeliaQuadBezier3d_new),
                                         reinterpret_cast<t_method>(ofeliaQuadBezier3d_free),
                                         sizeof(t_ofeliaQuadBezier3d),
                                         CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaQuadBezier3d_class, reinterpret_cast<t_method>(ofeliaQuadBezier3d_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaQuadBezier3d_class, reinterpret_cast<t_method>(ofeliaQuadBezier3d_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addbang(ofeliaQuadBezier3d_class, reinterpret_cast<t_method>(ofeliaQuadBezier3d_bang));
    class_addmethod(ofeliaQuadBezier3d_class, reinterpret_cast<t_method>(ofeliaQuadBezier3d_p0),
                    gensym("p0"), A_GIMME, 0);
    class_addmethod(ofeliaQuadBezier3d_class, reinterpret_cast<t_method>(ofeliaQuadBezier3d_p1),
                    gensym("p1"), A_GIMME, 0);
    class_addmethod(ofeliaQuadBezier3d_class, reinterpret_cast<t_method>(ofeliaQuadBezier3d_p2),
                    gensym("p2"), A_GIMME, 0);
    class_addmethod(ofeliaQuadBezier3d_class, reinterpret_cast<t_method>(ofeliaQuadBezier3d_p3),
                    gensym("p3"), A_GIMME, 0);
    class_addmethod(ofeliaQuadBezier3d_class, reinterpret_cast<t_method>(ofeliaQuadBezier3d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaQuadBezier3d_class, reinterpret_cast<t_method>(ofeliaQuadBezier3d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofTriangle2d object methods
 */
bool getTri2dPointsFromArgs(const int argc, const t_atom *argv, t_ofeliaTri2dPoints &points)
{
    switch (argc) {
            
        case 0:
        {
            points.p1.set(-OFELIA_DEFAULT_SIZE_HALF, OFELIA_DEFAULT_SIZE_HALF);
            points.p2.set(OFELIA_DEFAULT_SIZE_HALF, OFELIA_DEFAULT_SIZE_HALF);
            points.p3.set(0.0f, -OFELIA_DEFAULT_SIZE_HALF);
            break;
        }
        case 6:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT &&
                argv[4].a_type == A_FLOAT &&
                argv[5].a_type == A_FLOAT) {
                
                points.p1.set(argv[0].a_w.w_float, argv[1].a_w.w_float);
                points.p2.set(argv[2].a_w.w_float, argv[3].a_w.w_float);
                points.p3.set(argv[4].a_w.w_float, argv[5].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaTriangle2d::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaTriangle2d::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaTriangle2d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaTriangle2d *x = reinterpret_cast<t_ofeliaTriangle2d*>(pd_new(ofeliaTriangle2d_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getTri2dPointsFromArgs(argc, argv, x->points))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p1"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p2"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p3"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaTriangle2d_bang(t_ofeliaTriangle2d *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofGetCurrentRenderer()->drawTriangle(x->points.p1.x, x->points.p1.y, 0.0f,
                                             x->points.p2.x, x->points.p2.y, 0.0f,
                                             x->points.p3.x, x->points.p3.y, 0.0f);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaTriangle2d::objName);
    }
}

void ofeliaTriangle2d_p1(t_ofeliaTriangle2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            x->points.p1.set(argv[0].a_w.w_float, argv[1].a_w.w_float);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaTriangle2d::objName);
    }
}

void ofeliaTriangle2d_p2(t_ofeliaTriangle2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            x->points.p2.set(argv[0].a_w.w_float, argv[1].a_w.w_float);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaTriangle2d::objName);
    }
}

void ofeliaTriangle2d_p3(t_ofeliaTriangle2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            x->points.p3.set(argv[0].a_w.w_float, argv[1].a_w.w_float);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaTriangle2d::objName);
    }
}

void ofeliaTriangle2d_set(t_ofeliaTriangle2d *x, t_symbol *s, int argc, t_atom *argv)
{
    getTri2dPointsFromArgs(argc, argv, x->points);
}

void ofeliaTriangle2d_print(t_ofeliaTriangle2d *x)
{
    post("\n[%s]", t_ofeliaTriangle2d::objName);
    post("p1 : %g %g", x->points.p1.x, x->points.p1.y);
    post("p2 : %g %g", x->points.p2.x, x->points.p2.y);
    post("p3 : %g %g", x->points.p3.x, x->points.p3.y);
}

void ofeliaTriangle2d_setup()
{
    ofeliaTriangle2d_class = class_new(gensym("ofTriangle2d"),
                                       reinterpret_cast<t_newmethod>(ofeliaTriangle2d_new),
                                       0, sizeof(t_ofeliaTriangle2d),
                                       CLASS_DEFAULT, A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaTriangle2d_new),
                     gensym("ofTri2d"), A_GIMME, 0);
    class_addbang(ofeliaTriangle2d_class, reinterpret_cast<t_method>(ofeliaTriangle2d_bang));
    class_addmethod(ofeliaTriangle2d_class, reinterpret_cast<t_method>(ofeliaTriangle2d_p1),
                    gensym("p1"), A_GIMME, 0);
    class_addmethod(ofeliaTriangle2d_class, reinterpret_cast<t_method>(ofeliaTriangle2d_p2),
                    gensym("p2"), A_GIMME, 0);
    class_addmethod(ofeliaTriangle2d_class, reinterpret_cast<t_method>(ofeliaTriangle2d_p3),
                    gensym("p3"), A_GIMME, 0);
    class_addmethod(ofeliaTriangle2d_class, reinterpret_cast<t_method>(ofeliaTriangle2d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaTriangle2d_class, reinterpret_cast<t_method>(ofeliaTriangle2d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofTriangle3d object methods
 */
bool getTri3dPointsFromArgs(const int argc, const t_atom *argv, t_ofeliaTri3dPoints &points)
{
    switch (argc) {
            
        case 0:
        {
            points.p1.set(-OFELIA_DEFAULT_SIZE_HALF, OFELIA_DEFAULT_SIZE_HALF);
            points.p2.set(OFELIA_DEFAULT_SIZE_HALF, OFELIA_DEFAULT_SIZE_HALF);
            points.p3.set(0.0f, -OFELIA_DEFAULT_SIZE_HALF);
            break;
        }
        case 9:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT &&
                argv[4].a_type == A_FLOAT &&
                argv[5].a_type == A_FLOAT &&
                argv[6].a_type == A_FLOAT &&
                argv[7].a_type == A_FLOAT &&
                argv[8].a_type == A_FLOAT) {
                
                points.p1.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                points.p2.set(argv[3].a_w.w_float, argv[4].a_w.w_float, argv[5].a_w.w_float);
                points.p3.set(argv[6].a_w.w_float, argv[7].a_w.w_float, argv[8].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaTriangle3d::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaTriangle3d::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaTriangle3d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaTriangle3d *x = reinterpret_cast<t_ofeliaTriangle3d*>(pd_new(ofeliaTriangle3d_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getTri3dPointsFromArgs(argc, argv, x->points))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p1"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p2"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p3"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaTriangle3d_bang(t_ofeliaTriangle3d *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofGetCurrentRenderer()->drawTriangle(x->points.p1.x, x->points.p1.y, x->points.p1.z,
                                             x->points.p2.x, x->points.p2.y, x->points.p2.z,
                                             x->points.p3.x, x->points.p3.y, x->points.p3.z);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaTriangle3d::objName);
    }
}

void ofeliaTriangle3d_p1(t_ofeliaTriangle3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            x->points.p1.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaTriangle3d::objName);
    }
}

void ofeliaTriangle3d_p2(t_ofeliaTriangle3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            x->points.p2.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaTriangle3d::objName);
    }
}

void ofeliaTriangle3d_p3(t_ofeliaTriangle3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            x->points.p3.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaTriangle3d::objName);
    }
}

void ofeliaTriangle3d_set(t_ofeliaTriangle3d *x, t_symbol *s, int argc, t_atom *argv)
{
    getTri3dPointsFromArgs(argc, argv, x->points);
}

void ofeliaTriangle3d_print(t_ofeliaTriangle3d *x)
{
    post("\n[%s]", t_ofeliaTriangle3d::objName);
    post("p1 : %g %g %g", x->points.p1.x, x->points.p1.y, x->points.p1.z);
    post("p2 : %g %g %g", x->points.p2.x, x->points.p2.y, x->points.p2.z);
    post("p3 : %g %g %g", x->points.p3.x, x->points.p3.y, x->points.p3.z);
}

void ofeliaTriangle3d_setup()
{
    ofeliaTriangle3d_class = class_new(gensym("ofTriangle3d"),
                                       reinterpret_cast<t_newmethod>(ofeliaTriangle3d_new),
                                       0, sizeof(t_ofeliaTriangle3d),
                                       CLASS_DEFAULT, A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaTriangle3d_new),
                     gensym("ofTri3d"), A_GIMME, 0);
    class_addbang(ofeliaTriangle3d_class, reinterpret_cast<t_method>(ofeliaTriangle3d_bang));
    class_addmethod(ofeliaTriangle3d_class, reinterpret_cast<t_method>(ofeliaTriangle3d_p1),
                    gensym("p1"), A_GIMME, 0);
    class_addmethod(ofeliaTriangle3d_class, reinterpret_cast<t_method>(ofeliaTriangle3d_p2),
                    gensym("p2"), A_GIMME, 0);
    class_addmethod(ofeliaTriangle3d_class, reinterpret_cast<t_method>(ofeliaTriangle3d_p3),
                    gensym("p3"), A_GIMME, 0);
    class_addmethod(ofeliaTriangle3d_class, reinterpret_cast<t_method>(ofeliaTriangle3d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaTriangle3d_class, reinterpret_cast<t_method>(ofeliaTriangle3d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofEqTriangle object methods
 */
void *ofeliaEqTriangle_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaEqTriangle *x = reinterpret_cast<t_ofeliaEqTriangle*>(pd_new(ofeliaEqTriangle_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    switch (argc) {
            
        case 0:
        {
            x->size = OFELIA_DEFAULT_SIZE;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                x->size = argv[0].a_w.w_float;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaEqTriangle::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaEqTriangle::objName);
            return 0;
        }
    }
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("size"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaEqTriangle_bang(t_ofeliaEqTriangle *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const float halfSize = x->size * 0.5f;
        const float inRadius = x->size * ofeliaConstants::heightFactor / 3; // inradius = height / 3
        ofGetCurrentRenderer()->drawTriangle(0.0f, -inRadius*2.0f, 0.0f,
                                             halfSize, inRadius, 0.0f,
                                             -halfSize, inRadius, 0.0f);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaEqTriangle::objName);
    }
}

void ofeliaEqTriangle_set(t_ofeliaEqTriangle *x, t_floatarg f)
{
    x->size = f;
}

void ofeliaEqTriangle_print(t_ofeliaEqTriangle *x)
{
    post("\n[%s]", t_ofeliaEqTriangle::objName);
    post("size : %g", x->size);
}

void ofeliaEqTriangle_setup()
{
    ofeliaEqTriangle_class = class_new(gensym("ofEqTriangle"),
                                       reinterpret_cast<t_newmethod>(ofeliaEqTriangle_new),
                                       0, sizeof(t_ofeliaEqTriangle),
                                       CLASS_DEFAULT, A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaEqTriangle_new),
                     gensym("ofEqTri"), A_GIMME, 0);
    class_addbang(ofeliaEqTriangle_class, reinterpret_cast<t_method>(ofeliaEqTriangle_bang));
    class_addmethod(ofeliaEqTriangle_class, reinterpret_cast<t_method>(ofeliaEqTriangle_set),
                    gensym("size"), A_FLOAT, 0);
    class_addmethod(ofeliaEqTriangle_class, reinterpret_cast<t_method>(ofeliaEqTriangle_set),
                    gensym("set"), A_FLOAT, 0);
    class_addmethod(ofeliaEqTriangle_class, reinterpret_cast<t_method>(ofeliaEqTriangle_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofIsoTriangle object methods
 */
void *ofeliaIsoTriangle_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaIsoTriangle *x = reinterpret_cast<t_ofeliaIsoTriangle*>(pd_new(ofeliaIsoTriangle_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getDimenFromArgs(argc, argv, x->dimen, t_ofeliaIsoTriangle::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("width"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("height"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaIsoTriangle_bang(t_ofeliaIsoTriangle *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const float halfWidth = x->dimen.width * 0.5f;
        const float halfHeight = x->dimen.height * 0.5f;
        
        ofGetCurrentRenderer()->drawTriangle(0.0f, -halfHeight, 0.0f,
                                             halfWidth, halfHeight, 0.0f,
                                             -halfWidth, halfHeight, 0.0f);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaIsoTriangle::objName);
    }
}

void ofeliaIsoTriangle_width(t_ofeliaIsoTriangle *x, t_floatarg f)
{
    x->dimen.width = f;
}

void ofeliaIsoTriangle_height(t_ofeliaIsoTriangle *x, t_floatarg f)
{
    x->dimen.height = f;
}

void ofeliaIsoTriangle_set(t_ofeliaIsoTriangle *x, t_symbol *s, int argc, t_atom *argv)
{
    getDimenFromArgs(argc, argv, x->dimen, t_ofeliaIsoTriangle::objName);
}

void ofeliaIsoTriangle_print(t_ofeliaIsoTriangle *x)
{
    post("\n[%s]", t_ofeliaIsoTriangle::objName);
    post("width : %g", x->dimen.width);
    post("height : %g", x->dimen.height);
}

void ofeliaIsoTriangle_setup()
{
    ofeliaIsoTriangle_class = class_new(gensym("ofIsoTriangle"),
                                        reinterpret_cast<t_newmethod>(ofeliaIsoTriangle_new),
                                        0, sizeof(t_ofeliaIsoTriangle),
                                        CLASS_DEFAULT, A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaIsoTriangle_new),
                     gensym("ofIsoTri"), A_GIMME, 0);
    class_addbang(ofeliaIsoTriangle_class, reinterpret_cast<t_method>(ofeliaIsoTriangle_bang));
    class_addmethod(ofeliaIsoTriangle_class, reinterpret_cast<t_method>(ofeliaIsoTriangle_width),
                    gensym("width"), A_FLOAT, 0);
    class_addmethod(ofeliaIsoTriangle_class, reinterpret_cast<t_method>(ofeliaIsoTriangle_height),
                    gensym("height"), A_FLOAT, 0);
    class_addmethod(ofeliaIsoTriangle_class, reinterpret_cast<t_method>(ofeliaIsoTriangle_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaIsoTriangle_class, reinterpret_cast<t_method>(ofeliaIsoTriangle_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofQuad2d object methods
 */
bool getQuad2dPointsFromArgs(const int argc, const t_atom *argv, ofVec2f (&points)[4])
{
    switch (argc) {
            
        case 0:
        {
            points[0].set(0.0f, 0.0f);
            points[1].set(OFELIA_DEFAULT_SIZE, 0.0f);
            points[2].set(OFELIA_DEFAULT_SIZE, OFELIA_DEFAULT_SIZE);
            points[3].set(0.0f, OFELIA_DEFAULT_SIZE);
            break;
        }
        case 8:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT &&
                argv[4].a_type == A_FLOAT &&
                argv[5].a_type == A_FLOAT &&
                argv[6].a_type == A_FLOAT &&
                argv[7].a_type == A_FLOAT) {
                
                points[0].set(argv[0].a_w.w_float, argv[1].a_w.w_float);
                points[1].set(argv[2].a_w.w_float, argv[3].a_w.w_float);
                points[2].set(argv[4].a_w.w_float, argv[5].a_w.w_float);
                points[3].set(argv[6].a_w.w_float, argv[7].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaQuad2d::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaQuad2d::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaQuad2d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaQuad2d *x = reinterpret_cast<t_ofeliaQuad2d*>(pd_new(ofeliaQuad2d_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getQuad2dPointsFromArgs(argc, argv, x->points))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p1"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p2"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p3"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p4"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaQuad2d_bang(t_ofeliaQuad2d *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const ofStyle &style = ofGetStyle();
        
        // startSmoothing
        if (style.smoothing && !style.bFill) {
            
#ifndef TARGET_OPENGLES
            glPushAttrib(GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT);
#endif
            
            glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
            glEnable(GL_LINE_SMOOTH);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, sizeof(ofVec2f), &x->points[0].x);
        glDrawArrays(style.bFill ? GL_TRIANGLE_FAN : GL_LINE_LOOP, 0, 4);
        
        // endSmoothing
        if (style.smoothing && !style.bFill)
#ifndef TARGET_OPENGLES
            glPopAttrib();
#endif
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaQuad2d::objName);
    }
}

void ofeliaQuad2d_p1(t_ofeliaQuad2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            x->points[0].set(argv[0].a_w.w_float, argv[1].a_w.w_float);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaQuad2d::objName);
    }
}

void ofeliaQuad2d_p2(t_ofeliaQuad2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            x->points[1].set(argv[0].a_w.w_float, argv[1].a_w.w_float);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaQuad2d::objName);
    }
}

void ofeliaQuad2d_p3(t_ofeliaQuad2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            x->points[2].set(argv[0].a_w.w_float, argv[1].a_w.w_float);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaQuad2d::objName);
    }
}

void ofeliaQuad2d_p4(t_ofeliaQuad2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            x->points[3].set(argv[0].a_w.w_float, argv[1].a_w.w_float);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaQuad2d::objName);
    }
}

void ofeliaQuad2d_set(t_ofeliaQuad2d *x, t_symbol *s, int argc, t_atom *argv)
{
    getQuad2dPointsFromArgs(argc, argv, x->points);
}

void ofeliaQuad2d_print(t_ofeliaQuad2d *x)
{
    post("\n[%s]", t_ofeliaQuad2d::objName);
    post("p1 : %g %g", x->points[0].x, x->points[0].y);
    post("p2 : %g %g", x->points[1].x, x->points[1].y);
    post("p3 : %g %g", x->points[2].x, x->points[2].y);
    post("p4 : %g %g", x->points[3].x, x->points[3].y);
}

void ofeliaQuad2d_setup()
{
    ofeliaQuad2d_class = class_new(gensym("ofQuad2d"),
                                   reinterpret_cast<t_newmethod>(ofeliaQuad2d_new),
                                   0, sizeof(t_ofeliaQuad2d),
                                   CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaQuad2d_class, reinterpret_cast<t_method>(ofeliaQuad2d_bang));
    class_addmethod(ofeliaQuad2d_class, reinterpret_cast<t_method>(ofeliaQuad2d_p1),
                    gensym("p1"), A_GIMME, 0);
    class_addmethod(ofeliaQuad2d_class, reinterpret_cast<t_method>(ofeliaQuad2d_p2),
                    gensym("p2"), A_GIMME, 0);
    class_addmethod(ofeliaQuad2d_class, reinterpret_cast<t_method>(ofeliaQuad2d_p3),
                    gensym("p3"), A_GIMME, 0);
    class_addmethod(ofeliaQuad2d_class, reinterpret_cast<t_method>(ofeliaQuad2d_p4),
                    gensym("p4"), A_GIMME, 0);
    class_addmethod(ofeliaQuad2d_class, reinterpret_cast<t_method>(ofeliaQuad2d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaQuad2d_class, reinterpret_cast<t_method>(ofeliaQuad2d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofQuad3d object methods
 */
bool getQuad3dPointsFromArgs(const int argc, const t_atom *argv, ofVec3f (&points)[4])
{
    switch (argc) {
            
        case 0:
        {
            points[0].set(0.0f, 0.0f);
            points[1].set(OFELIA_DEFAULT_SIZE, 0.0f);
            points[2].set(OFELIA_DEFAULT_SIZE, OFELIA_DEFAULT_SIZE);
            points[3].set(0.0f, OFELIA_DEFAULT_SIZE);
            break;
        }
        case 12:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT &&
                argv[4].a_type == A_FLOAT &&
                argv[5].a_type == A_FLOAT &&
                argv[6].a_type == A_FLOAT &&
                argv[7].a_type == A_FLOAT &&
                argv[8].a_type == A_FLOAT &&
                argv[9].a_type == A_FLOAT &&
                argv[10].a_type == A_FLOAT &&
                argv[11].a_type == A_FLOAT) {
                
                points[0].set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                points[1].set(argv[3].a_w.w_float, argv[4].a_w.w_float, argv[5].a_w.w_float);
                points[2].set(argv[6].a_w.w_float, argv[7].a_w.w_float, argv[8].a_w.w_float);
                points[3].set(argv[9].a_w.w_float, argv[10].a_w.w_float, argv[11].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaQuad3d::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaQuad3d::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaQuad3d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaQuad3d *x = reinterpret_cast<t_ofeliaQuad3d*>(pd_new(ofeliaQuad3d_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getQuad3dPointsFromArgs(argc, argv, x->points))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p1"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p2"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p3"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("p4"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaQuad3d_bang(t_ofeliaQuad3d *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const ofStyle &style = ofGetStyle();
        
        // startSmoothing
        if (style.smoothing && !style.bFill) {
            
#ifndef TARGET_OPENGLES
            glPushAttrib(GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT);
#endif
            
            glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
            glEnable(GL_LINE_SMOOTH);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, sizeof(ofVec3f), &x->points[0].x);
        glDrawArrays(style.bFill ? GL_TRIANGLE_FAN : GL_LINE_LOOP, 0, 4);
        
        // endSmoothing
        if (style.smoothing && !style.bFill)
#ifndef TARGET_OPENGLES
            glPopAttrib();
#endif
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaQuad3d::objName);
    }
}

void ofeliaQuad3d_p1(t_ofeliaQuad3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            x->points[0].set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaQuad3d::objName);
    }
}

void ofeliaQuad3d_p2(t_ofeliaQuad3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            x->points[1].set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaQuad3d::objName);
    }
}

void ofeliaQuad3d_p3(t_ofeliaQuad3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            x->points[2].set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaQuad3d::objName);
    }
}

void ofeliaQuad3d_p4(t_ofeliaQuad3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            x->points[3].set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaQuad3d::objName);
    }
}

void ofeliaQuad3d_set(t_ofeliaQuad3d *x, t_symbol *s, int argc, t_atom *argv)
{
    getQuad3dPointsFromArgs(argc, argv, x->points);
}

void ofeliaQuad3d_print(t_ofeliaQuad3d *x)
{
    post("\n[%s]", t_ofeliaQuad3d::objName);
    post("p1 : %g %g %g", x->points[0].x, x->points[0].y, x->points[0].z);
    post("p2 : %g %g %g", x->points[1].x, x->points[1].y, x->points[1].z);
    post("p3 : %g %g %g", x->points[2].x, x->points[2].y, x->points[2].z);
    post("p4 : %g %g %g", x->points[3].x, x->points[3].y, x->points[3].z);
}

void ofeliaQuad3d_setup()
{
    ofeliaQuad3d_class = class_new(gensym("ofQuad3d"),
                                   reinterpret_cast<t_newmethod>(ofeliaQuad3d_new),
                                   0, sizeof(t_ofeliaQuad3d),
                                   CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaQuad3d_class, reinterpret_cast<t_method>(ofeliaQuad3d_bang));
    class_addmethod(ofeliaQuad3d_class, reinterpret_cast<t_method>(ofeliaQuad3d_p1),
                    gensym("p1"), A_GIMME, 0);
    class_addmethod(ofeliaQuad3d_class, reinterpret_cast<t_method>(ofeliaQuad3d_p2),
                    gensym("p2"), A_GIMME, 0);
    class_addmethod(ofeliaQuad3d_class, reinterpret_cast<t_method>(ofeliaQuad3d_p3),
                    gensym("p3"), A_GIMME, 0);
    class_addmethod(ofeliaQuad3d_class, reinterpret_cast<t_method>(ofeliaQuad3d_p4),
                    gensym("p4"), A_GIMME, 0);
    class_addmethod(ofeliaQuad3d_class, reinterpret_cast<t_method>(ofeliaQuad3d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaQuad3d_class, reinterpret_cast<t_method>(ofeliaQuad3d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSquare object methods
 */
void *ofeliaSquare_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSquare *x = reinterpret_cast<t_ofeliaSquare*>(pd_new(ofeliaSquare_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    switch (argc) {
            
        case 0:
        {
            x->size = OFELIA_DEFAULT_SIZE;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                x->size = argv[0].a_w.w_float;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSquare::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaSquare::objName);
            return 0;
        }
    }
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("size"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSquare_bang(t_ofeliaSquare *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofGetCurrentRenderer()->drawRectangle(0.0f, 0.0f, 0.0f, x->size, x->size);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSquare::objName);
    }
}

void ofeliaSquare_set(t_ofeliaSquare *x, t_floatarg f)
{
    x->size = f;
}

void ofeliaSquare_print(t_ofeliaSquare *x)
{
    post("\n[%s]", t_ofeliaSquare::objName);
    post("size : %g", x->size);
}

void ofeliaSquare_setup()
{
    ofeliaSquare_class = class_new(gensym("ofSquare"),
                                   reinterpret_cast<t_newmethod>(ofeliaSquare_new),
                                   0, sizeof(t_ofeliaSquare),
                                   CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSquare_class, reinterpret_cast<t_method>(ofeliaSquare_bang));
    class_addmethod(ofeliaSquare_class, reinterpret_cast<t_method>(ofeliaSquare_set),
                    gensym("size"), A_FLOAT, 0);
    class_addmethod(ofeliaSquare_class, reinterpret_cast<t_method>(ofeliaSquare_set),
                    gensym("set"), A_FLOAT, 0);
    class_addmethod(ofeliaSquare_class, reinterpret_cast<t_method>(ofeliaSquare_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofRectangle object methods
 */
void *ofeliaRectangle_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaRectangle *x = reinterpret_cast<t_ofeliaRectangle*>(pd_new(ofeliaRectangle_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getDimenFromArgs(argc, argv, x->dimen, t_ofeliaRectangle::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("width"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("height"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaRectangle_bang(t_ofeliaRectangle *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofGetCurrentRenderer()->drawRectangle(0.0f, 0.0f, 0.0f, x->dimen.width, x->dimen.height);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaRectangle::objName);
    }
}

void ofeliaRectangle_width(t_ofeliaRectangle *x, t_floatarg f)
{
    x->dimen.width = f;
}

void ofeliaRectangle_height(t_ofeliaRectangle *x, t_floatarg f)
{
    x->dimen.height = f;
}

void ofeliaRectangle_set(t_ofeliaRectangle *x, t_symbol *s, int argc, t_atom *argv)
{
    getDimenFromArgs(argc, argv, x->dimen, t_ofeliaRectangle::objName);
}

void ofeliaRectangle_print(t_ofeliaRectangle *x)
{
    post("\n[%s]", t_ofeliaRectangle::objName);
    post("width : %g", x->dimen.width);
    post("height : %g", x->dimen.height);
}

void ofeliaRectangle_setup()
{
    ofeliaRectangle_class = class_new(gensym("ofRectangle"),
                                      reinterpret_cast<t_newmethod>(ofeliaRectangle_new),
                                      0, sizeof(t_ofeliaRectangle),
                                      CLASS_DEFAULT, A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaRectangle_new),
                     gensym("ofRect"), A_GIMME, 0);
    class_addbang(ofeliaRectangle_class, reinterpret_cast<t_method>(ofeliaRectangle_bang));
    class_addmethod(ofeliaRectangle_class, reinterpret_cast<t_method>(ofeliaRectangle_width),
                    gensym("width"), A_FLOAT, 0);
    class_addmethod(ofeliaRectangle_class, reinterpret_cast<t_method>(ofeliaRectangle_height),
                    gensym("height"), A_FLOAT, 0);
    class_addmethod(ofeliaRectangle_class, reinterpret_cast<t_method>(ofeliaRectangle_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaRectangle_class, reinterpret_cast<t_method>(ofeliaRectangle_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofRectRounded object methods
 */
bool getDimenRadFromArgs(const int argc, const t_atom *argv, t_ofeliaShapesDimenRad &dimenRad)
{
    switch (argc) {
            
        case 0:
        {
            dimenRad.width = OFELIA_DEFAULT_SIZE;
            dimenRad.height = OFELIA_DEFAULT_SIZE;
            dimenRad.radius = t_ofeliaShapesDefault::cornerRadius;
            break;
        }
        case 3:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT) {
                
                dimenRad.width = argv[0].a_w.w_float;
                dimenRad.height = argv[1].a_w.w_float;
                dimenRad.radius = argv[2].a_w.w_float;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaRectRounded::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaRectRounded::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaRectRounded_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaRectRounded *x = reinterpret_cast<t_ofeliaRectRounded*>(pd_new(ofeliaRectRounded_class));
    x->path = make_unique<ofPath>();
    x->mesh = make_unique<ofVboMesh>();
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getDimenRadFromArgs(argc, argv, x->dimenRad))
        return 0;
    x->shouldLoad = true;
    x->bInitGate = false;
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("width"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("height"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("radius"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaRectRounded_init(t_ofeliaRectRounded *x)
{
    if (!x->bInitGate)
        return;
    x->path = make_unique<ofPath>();
    x->mesh = make_unique<ofVboMesh>();
    x->shouldLoad = true;
}

void ofeliaRectRounded_exit(t_ofeliaRectRounded *x)
{
    x->bInitGate = true;
}

void ofeliaRectRounded_bang(t_ofeliaRectRounded *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const ofStyle &style = ofGetStyle();
        
        if (x->rectMode != style.rectMode ||
            x->circleResolution != style.circleResolution ||
            x->bFill != style.bFill) {
            
            x->rectMode = style.rectMode;
            x->circleResolution = style.circleResolution;
            x->bFill = style.bFill;
            x->shouldLoad = true;
        }
        if (x->shouldLoad && x->bFill) {
            
            float posX = 0.0f, posY = 0.0f;
            
            if (x->rectMode == OF_RECTMODE_CENTER) {
                
                posX = x->dimenRad.width * -0.5f;
                posY = x->dimenRad.height * -0.5f;
            }
            x->path->clear();
            x->path->setCircleResolution(x->circleResolution);
            x->path->rectRounded(posX, posY, 0.0f, x->dimenRad.width, x->dimenRad.height,
                                 x->dimenRad.radius, x->dimenRad.radius, x->dimenRad.radius, x->dimenRad.radius);
            x->mesh->operator=(x->path->getTessellation());
            x->shouldLoad = false;
        }
        if (!x->bFill)
            ofDrawRectRounded(0.0f, 0.0f, 0.0f, x->dimenRad.width, x->dimenRad.height,
                              x->dimenRad.radius, x->dimenRad.radius, x->dimenRad.radius, x->dimenRad.radius);
        else
            x->mesh->draw();
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaRectRounded::objName);
    }
}

void ofeliaRectRounded_width(t_ofeliaRectRounded *x, t_floatarg f)
{
    x->dimenRad.width = f;
    x->shouldLoad = true;
}

void ofeliaRectRounded_height(t_ofeliaRectRounded *x, t_floatarg f)
{
    x->dimenRad.height = f;
    x->shouldLoad = true;
}

void ofeliaRectRounded_radius(t_ofeliaRectRounded *x, t_floatarg f)
{
    x->dimenRad.radius = f;
    x->shouldLoad = true;
}

void ofeliaRectRounded_set(t_ofeliaRectRounded *x, t_symbol *s, int argc, t_atom *argv)
{
    if (getDimenRadFromArgs(argc, argv, x->dimenRad))
        x->shouldLoad = true;
}

void ofeliaRectRounded_print(t_ofeliaRectRounded *x)
{
    post("\n[%s]", t_ofeliaRectRounded::objName);
    post("width : %g", x->dimenRad.width);
    post("height : %g", x->dimenRad.height);
    post("radius : %g", x->dimenRad.radius);
}

void ofeliaRectRounded_free(t_ofeliaRectRounded *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
}

void ofeliaRectRounded_setup()
{
    ofeliaRectRounded_class = class_new(gensym("ofRectRounded"),
                                        reinterpret_cast<t_newmethod>(ofeliaRectRounded_new),
                                        reinterpret_cast<t_method>(ofeliaRectRounded_free),
                                        sizeof(t_ofeliaRectRounded),
                                        CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaRectRounded_class, reinterpret_cast<t_method>(ofeliaRectRounded_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaRectRounded_class, reinterpret_cast<t_method>(ofeliaRectRounded_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addbang(ofeliaRectRounded_class, reinterpret_cast<t_method>(ofeliaRectRounded_bang));
    class_addmethod(ofeliaRectRounded_class, reinterpret_cast<t_method>(ofeliaRectRounded_width),
                    gensym("width"), A_FLOAT, 0);
    class_addmethod(ofeliaRectRounded_class, reinterpret_cast<t_method>(ofeliaRectRounded_height),
                    gensym("height"), A_FLOAT, 0);
    class_addmethod(ofeliaRectRounded_class, reinterpret_cast<t_method>(ofeliaRectRounded_radius),
                    gensym("radius"), A_FLOAT, 0);
    class_addmethod(ofeliaRectRounded_class, reinterpret_cast<t_method>(ofeliaRectRounded_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaRectRounded_class, reinterpret_cast<t_method>(ofeliaRectRounded_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofRectRounded4 object methods
 */
bool getDimenRad4FromArgs(const int argc, const t_atom *argv, t_ofeliaShapesDimenRad4 &dimenRad)
{
    switch (argc) {
            
        case 0:
        {
            dimenRad.width = OFELIA_DEFAULT_SIZE;
            dimenRad.height = OFELIA_DEFAULT_SIZE;
            fill(dimenRad.radius, dimenRad.radius+4, t_ofeliaShapesDefault::cornerRadius);
            break;
        }
        case 6:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT &&
                argv[4].a_type == A_FLOAT &&
                argv[5].a_type == A_FLOAT) {
                
                dimenRad.width = argv[0].a_w.w_float;
                dimenRad.height = argv[1].a_w.w_float;
                dimenRad.radius[0] = argv[2].a_w.w_float;
                dimenRad.radius[1] = argv[3].a_w.w_float;
                dimenRad.radius[2] = argv[4].a_w.w_float;
                dimenRad.radius[3] = argv[5].a_w.w_float;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaRectRounded4::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaRectRounded4::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaRectRounded4_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaRectRounded4 *x = reinterpret_cast<t_ofeliaRectRounded4*>(pd_new(ofeliaRectRounded4_class));
    x->path = make_unique<ofPath>();
    x->mesh = make_unique<ofVboMesh>();
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getDimenRad4FromArgs(argc, argv, x->dimenRad))
        return 0;
    x->shouldLoad = true;
    x->bInitGate = false;
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("width"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("height"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("radius1"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("radius2"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("radius3"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("radius4"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaRectRounded4_init(t_ofeliaRectRounded4 *x)
{
    if (!x->bInitGate)
        return;
    x->path = make_unique<ofPath>();
    x->mesh = make_unique<ofVboMesh>();
    x->shouldLoad = true;
}

void ofeliaRectRounded4_exit(t_ofeliaRectRounded4 *x)
{
    x->bInitGate = true;
}

void ofeliaRectRounded4_bang(t_ofeliaRectRounded4 *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const ofStyle &style = ofGetStyle();
        
        if (x->rectMode != style.rectMode ||
            x->circleResolution != style.circleResolution ||
            x->bFill != style.bFill) {
            
            x->rectMode = style.rectMode;
            x->circleResolution = style.circleResolution;
            x->bFill = style.bFill;
            x->shouldLoad = true;
        }
        if (x->shouldLoad && x->bFill) {
            
            float posX = 0.0f, posY = 0.0f;
            
            if (x->rectMode == OF_RECTMODE_CENTER) {
                
                posX = x->dimenRad.width * -0.5f;
                posY = x->dimenRad.height * -0.5f;
            }
            x->path->clear();
            x->path->setCircleResolution(x->circleResolution);
            x->path->rectRounded(posX, posY, 0.0f, x->dimenRad.width, x->dimenRad.height,
                                 x->dimenRad.radius[0], x->dimenRad.radius[1],
                                 x->dimenRad.radius[2], x->dimenRad.radius[3]);
            x->mesh->operator=(x->path->getTessellation());
            x->shouldLoad = false;
        }
        if (!x->bFill)
            ofDrawRectRounded(0.0f, 0.0f, 0.0f, x->dimenRad.width, x->dimenRad.height,
                              x->dimenRad.radius[0], x->dimenRad.radius[1],
                              x->dimenRad.radius[2], x->dimenRad.radius[3]);
        else
            x->mesh->draw();
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaRectRounded4::objName);
    }
}

void ofeliaRectRounded4_width(t_ofeliaRectRounded4 *x, t_floatarg f)
{
    x->dimenRad.width = f;
    x->shouldLoad = true;
}

void ofeliaRectRounded4_height(t_ofeliaRectRounded4 *x, t_floatarg f)
{
    x->dimenRad.height = f;
    x->shouldLoad = true;
}

void ofeliaRectRounded4_radius1(t_ofeliaRectRounded4 *x, t_floatarg f)
{
    x->dimenRad.radius[0] = f;
    x->shouldLoad = true;
}

void ofeliaRectRounded4_radius2(t_ofeliaRectRounded4 *x, t_floatarg f)
{
    x->dimenRad.radius[1] = f;
    x->shouldLoad = true;
}

void ofeliaRectRounded4_radius3(t_ofeliaRectRounded4 *x, t_floatarg f)
{
    x->dimenRad.radius[2] = f;
    x->shouldLoad = true;
}

void ofeliaRectRounded4_radius4(t_ofeliaRectRounded4 *x, t_floatarg f)
{
    x->dimenRad.radius[3] = f;
    x->shouldLoad = true;
}

void ofeliaRectRounded4_set(t_ofeliaRectRounded4 *x, t_symbol *s, int argc, t_atom *argv)
{
    if (getDimenRad4FromArgs(argc, argv, x->dimenRad))
        x->shouldLoad = true;
}

void ofeliaRectRounded4_print(t_ofeliaRectRounded4 *x)
{
    post("\n[%s]", t_ofeliaRectRounded4::objName);
    post("width : %g", x->dimenRad.width);
    post("height : %g", x->dimenRad.height);
    post("radius1 : %g", x->dimenRad.radius[0]);
    post("radius2 : %g", x->dimenRad.radius[1]);
    post("radius3 : %g", x->dimenRad.radius[2]);
    post("radius4 : %g", x->dimenRad.radius[3]);
}

void ofeliaRectRounded4_free(t_ofeliaRectRounded4 *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
}

void ofeliaRectRounded4_setup()
{
    ofeliaRectRounded4_class = class_new(gensym("ofRectRounded4"),
                                         reinterpret_cast<t_newmethod>(ofeliaRectRounded4_new),
                                         reinterpret_cast<t_method>(ofeliaRectRounded4_free),
                                         sizeof(t_ofeliaRectRounded4),
                                         CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaRectRounded4_class, reinterpret_cast<t_method>(ofeliaRectRounded4_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaRectRounded4_class, reinterpret_cast<t_method>(ofeliaRectRounded4_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addbang(ofeliaRectRounded4_class, reinterpret_cast<t_method>(ofeliaRectRounded4_bang));
    class_addmethod(ofeliaRectRounded4_class, reinterpret_cast<t_method>(ofeliaRectRounded4_width),
                    gensym("width"), A_FLOAT, 0);
    class_addmethod(ofeliaRectRounded4_class, reinterpret_cast<t_method>(ofeliaRectRounded4_height),
                    gensym("height"), A_FLOAT, 0);
    class_addmethod(ofeliaRectRounded4_class, reinterpret_cast<t_method>(ofeliaRectRounded4_radius1),
                    gensym("radius1"), A_FLOAT, 0);
    class_addmethod(ofeliaRectRounded4_class, reinterpret_cast<t_method>(ofeliaRectRounded4_radius2),
                    gensym("radius2"), A_FLOAT, 0);
    class_addmethod(ofeliaRectRounded4_class, reinterpret_cast<t_method>(ofeliaRectRounded4_radius3),
                    gensym("radius3"), A_FLOAT, 0);
    class_addmethod(ofeliaRectRounded4_class, reinterpret_cast<t_method>(ofeliaRectRounded4_radius4),
                    gensym("radius4"), A_FLOAT, 0);
    class_addmethod(ofeliaRectRounded4_class, reinterpret_cast<t_method>(ofeliaRectRounded4_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaRectRounded4_class, reinterpret_cast<t_method>(ofeliaRectRounded4_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofCross object methods
 */
bool getCrossElemFromArgs(const int argc, const t_atom *argv, t_ofeliaCrossElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.size = OFELIA_DEFAULT_SIZE;
            elem.thickness = t_ofeliaShapesDefault::crossThickness;
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT) {
                
                elem.size = argv[0].a_w.w_float;
                elem.thickness = argv[1].a_w.w_float;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaCross::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaCross::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaCross_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaCross *x = reinterpret_cast<t_ofeliaCross*>(pd_new(ofeliaCross_class));
    x->path = make_unique<ofPath>();
    x->mesh = make_unique<ofVboMesh>();
    x->polylines = make_unique<vector<ofPolyline>>();
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getCrossElemFromArgs(argc, argv, x->elem))
        return 0;
    x->shouldLoad = true;
    x->bInitGate = false;
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("size"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("thickness"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaCross_init(t_ofeliaCross *x)
{
    if (!x->bInitGate)
        return;
    x->path = make_unique<ofPath>();
    x->mesh = make_unique<ofVboMesh>();
    x->polylines = make_unique<vector<ofPolyline>>();
    x->shouldLoad = true;
}

void ofeliaCross_exit(t_ofeliaCross *x)
{
    x->bInitGate = true;
}

void ofeliaCross_bang(t_ofeliaCross *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const ofStyle &style = ofGetStyle();
        
        if (x->bFill != style.bFill) {
            
            x->bFill = style.bFill;
            x->shouldLoad = true;
        }
        if (x->shouldLoad) {
            
            const float sHalf = x->elem.size * 0.5f;
            const float tHalf = x->elem.thickness * 0.5f;
            
            x->path->clear();
            x->path->moveTo(ofVec2f(-tHalf, -sHalf));
            x->path->lineTo(ofVec2f(tHalf, -sHalf));
            x->path->lineTo(ofVec2f(tHalf, -tHalf));
            x->path->lineTo(ofVec2f(sHalf, -tHalf));
            x->path->lineTo(ofVec2f(sHalf, tHalf));
            x->path->lineTo(ofVec2f(tHalf, tHalf));
            x->path->lineTo(ofVec2f(tHalf, sHalf));
            x->path->lineTo(ofVec2f(-tHalf, sHalf));
            x->path->lineTo(ofVec2f(-tHalf, tHalf));
            x->path->lineTo(ofVec2f(-sHalf, tHalf));
            x->path->lineTo(ofVec2f(-sHalf, -tHalf));
            x->path->lineTo(ofVec2f(-tHalf, -tHalf));
            x->path->close();
            
            if (!x->bFill)
                x->polylines->operator=(x->path->getOutline());
            else
                x->mesh->operator=(x->path->getTessellation());
            x->shouldLoad = false;
        }
        if (!x->bFill)
            for (size_t i=0; i<x->polylines->size(); ++i)
                x->polylines->at(i).draw();
        else
            x->mesh->draw();
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaCross::objName);
    }
}

void ofeliaCross_size(t_ofeliaCross *x, t_floatarg f)
{
    x->elem.size = f;
    x->shouldLoad = true;
}

void ofeliaCross_thickness(t_ofeliaCross *x, t_floatarg f)
{
    x->elem.thickness = f;
    x->shouldLoad = true;
}

void ofeliaCross_set(t_ofeliaCross *x, t_symbol *s, int argc, t_atom *argv)
{
    if (getCrossElemFromArgs(argc, argv, x->elem))
        x->shouldLoad = true;
}

void ofeliaCross_print(t_ofeliaCross *x)
{
    post("\n[%s]", t_ofeliaCross::objName);
    post("size : %g", x->elem.size);
    post("thickness : %g", x->elem.thickness);
}

void ofeliaCross_free(t_ofeliaCross *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
}

void ofeliaCross_setup()
{
    ofeliaCross_class = class_new(gensym("ofCross"),
                                  reinterpret_cast<t_newmethod>(ofeliaCross_new),
                                  reinterpret_cast<t_method>(ofeliaCross_free),
                                  sizeof(t_ofeliaCross),
                                  CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaCross_class, reinterpret_cast<t_method>(ofeliaCross_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaCross_class, reinterpret_cast<t_method>(ofeliaCross_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addbang(ofeliaCross_class, reinterpret_cast<t_method>(ofeliaCross_bang));
    class_addmethod(ofeliaCross_class, reinterpret_cast<t_method>(ofeliaCross_size),
                    gensym("size"), A_FLOAT, 0);
    class_addmethod(ofeliaCross_class, reinterpret_cast<t_method>(ofeliaCross_thickness),
                    gensym("thickness"), A_FLOAT, 0);
    class_addmethod(ofeliaCross_class, reinterpret_cast<t_method>(ofeliaCross_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaCross_class, reinterpret_cast<t_method>(ofeliaCross_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofHeart object methods
 */
void *ofeliaHeart_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaHeart *x = reinterpret_cast<t_ofeliaHeart*>(pd_new(ofeliaHeart_class));
    x->path = make_unique<ofPath>();
    x->mesh = make_unique<ofVboMesh>();
    x->polylines = make_unique<vector<ofPolyline>>();
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getDimenFromArgs(argc, argv, x->dimen, t_ofeliaHeart::objName))
        return 0;
    x->shouldLoad = true;
    x->bInitGate = false;
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("width"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("height"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaHeart_init(t_ofeliaHeart *x)
{
    if (!x->bInitGate)
        return;
    x->path = make_unique<ofPath>();
    x->mesh = make_unique<ofVboMesh>();
    x->polylines = make_unique<vector<ofPolyline>>();
    x->shouldLoad = true;
}

void ofeliaHeart_exit(t_ofeliaHeart *x)
{
    x->bInitGate = true;
}

void ofeliaHeart_bang(t_ofeliaHeart *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const ofStyle &style = ofGetStyle();
        
        if (x->curveResolution != style.curveResolution ||
            x->bFill != style.bFill) {
            
            x->curveResolution = style.curveResolution;
            x->bFill = style.bFill;
            x->shouldLoad = true;
        }
        if (x->shouldLoad) {
            
            const float width = x->dimen.width;
            const float height = x->dimen.height;
            const float widthStretched = width * 1.125f;
            const float heightHalf = height * 0.5f;
            const ofVec2f beginEnd(0.0f, height * -0.25f);
            const ofVec2f bezierP1(0.0f, height * -0.75f);
            const ofVec2f bezierP3(0.0f, heightHalf);
            
            x->path->clear();
            x->path->setCurveResolution(x->curveResolution);
            x->path->moveTo(beginEnd);
            x->path->bezierTo(bezierP1,
                              ofVec2f(widthStretched, 5.0f-heightHalf),
                              bezierP3);
            x->path->moveTo(beginEnd);
            x->path->bezierTo(bezierP1,
                              ofVec2f(-widthStretched, 5.0f-heightHalf),
                              bezierP3);
            if (!x->bFill)
                x->polylines->operator=(x->path->getOutline());
            else
                x->mesh->operator=(x->path->getTessellation());
            x->shouldLoad = false;
        }
        if (!x->bFill)
            for (size_t i=0; i<x->polylines->size(); ++i)
                x->polylines->at(i).draw();
        else
            x->mesh->draw();
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaHeart::objName);
    }
}

void ofeliaHeart_width(t_ofeliaHeart *x, t_floatarg f)
{
    x->dimen.width = f;
    x->shouldLoad = true;
}

void ofeliaHeart_height(t_ofeliaHeart *x, t_floatarg f)
{
    x->dimen.height = f;
    x->shouldLoad = true;
}

void ofeliaHeart_set(t_ofeliaHeart *x, t_symbol *s, int argc, t_atom *argv)
{
    if (getDimenFromArgs(argc, argv, x->dimen, t_ofeliaHeart::objName))
        x->shouldLoad = true;
}

void ofeliaHeart_print(t_ofeliaHeart *x)
{
    post("\n[%s]", t_ofeliaHeart::objName);
    post("width : %g", x->dimen.width);
    post("height : %g", x->dimen.height);
}

void ofeliaHeart_free(t_ofeliaHeart *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
}

void ofeliaHeart_setup()
{
    ofeliaHeart_class = class_new(gensym("ofHeart"),
                                  reinterpret_cast<t_newmethod>(ofeliaHeart_new),
                                  reinterpret_cast<t_method>(ofeliaHeart_free),
                                  sizeof(t_ofeliaHeart),
                                  CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaHeart_class, reinterpret_cast<t_method>(ofeliaHeart_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaHeart_class, reinterpret_cast<t_method>(ofeliaHeart_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addbang(ofeliaHeart_class, reinterpret_cast<t_method>(ofeliaHeart_bang));
    class_addmethod(ofeliaHeart_class, reinterpret_cast<t_method>(ofeliaHeart_width),
                    gensym("width"), A_FLOAT, 0);
    class_addmethod(ofeliaHeart_class, reinterpret_cast<t_method>(ofeliaHeart_height),
                    gensym("height"), A_FLOAT, 0);
    class_addmethod(ofeliaHeart_class, reinterpret_cast<t_method>(ofeliaHeart_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaHeart_class, reinterpret_cast<t_method>(ofeliaHeart_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofMoon object methods
 */
bool getMoonElemFromArgs(const int argc, const t_atom *argv, t_ofeliaMoonElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.width = OFELIA_DEFAULT_SIZE;
            elem.height = OFELIA_DEFAULT_SIZE;
            elem.phase = t_ofeliaShapesDefault::moonPhaseRadius;
            break;
        }
        case 3:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT) {
                
                elem.width = argv[0].a_w.w_float;
                elem.height = argv[1].a_w.w_float;
                elem.phase = argv[2].a_w.w_float;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaMoon::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaMoon::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaMoon_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaMoon *x = reinterpret_cast<t_ofeliaMoon*>(pd_new(ofeliaMoon_class));
    x->path = make_unique<ofPath>();
    x->mesh = make_unique<ofVboMesh>();
    x->polyline = make_unique<ofPolyline>();
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getMoonElemFromArgs(argc, argv, x->elem))
        return 0;
    x->shouldLoad = true;
    x->bInitGate = false;
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("width"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("height"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("phase"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaMoon_init(t_ofeliaMoon *x)
{
    if (!x->bInitGate)
        return;
    x->path = make_unique<ofPath>();
    x->mesh = make_unique<ofVboMesh>();
    x->polyline = make_unique<ofPolyline>();
    x->shouldLoad = true;
}

void ofeliaMoon_exit(t_ofeliaMoon *x)
{
    x->bInitGate = true;
}

void ofeliaMoon_bang(t_ofeliaMoon *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const ofStyle &style = ofGetStyle();
        
        if (x->circleResolution != style.circleResolution ||
            x->bFill != style.bFill) {
            
            x->circleResolution = style.circleResolution;
            x->bFill = style.bFill;
            x->shouldLoad = true;
        }
        if (x->shouldLoad) {
            
            const float radiusX = x->elem.width * 0.5f;
            const float radiusY = x->elem.height * 0.5f;
            
            x->polyline->clear();
            x->polyline->arc(ofVec2f(0.0f, 0.0f),
                             x->elem.phase, radiusY,
                             -90.0f, 90.0f, true, x->circleResolution);
            x->polyline->arc(ofVec2f(0.0f, 0.0f),
                             radiusX, radiusY,
                             90.0f, -90.0f, false, x->circleResolution);
            if (x->bFill) {
                
                x->path->clear();
                x->path->moveTo(x->polyline->getVertices()[0]);
                
                for (size_t i=1; i<x->polyline->getVertices().size(); ++i)
                    x->path->lineTo(x->polyline->getVertices()[i]);
                
                x->path->close();
                x->mesh->operator=(x->path->getTessellation());
            }
            x->shouldLoad = false;
        }
        if (!x->bFill)
            x->polyline->draw();
        else
            x->mesh->draw();
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaMoon::objName);
    }
}

void ofeliaMoon_width(t_ofeliaMoon *x, t_floatarg f)
{
    x->elem.width = f;
    x->shouldLoad = true;
}

void ofeliaMoon_height(t_ofeliaMoon *x, t_floatarg f)
{
    x->elem.height = f;
    x->shouldLoad = true;
}

void ofeliaMoon_phase(t_ofeliaMoon *x, t_floatarg f)
{
    x->elem.phase = f;
    x->shouldLoad = true;
}

void ofeliaMoon_set(t_ofeliaMoon *x, t_symbol *s, int argc, t_atom *argv)
{
    if (getMoonElemFromArgs(argc, argv, x->elem))
        x->shouldLoad = true;
}

void ofeliaMoon_print(t_ofeliaMoon *x)
{
    post("\n[%s]", t_ofeliaMoon::objName);
    post("width : %g", x->elem.width);
    post("height : %g", x->elem.height);
    post("phase : %g", x->elem.phase);
}

void ofeliaMoon_free(t_ofeliaMoon *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
}

void ofeliaMoon_setup()
{
    ofeliaMoon_class = class_new(gensym("ofMoon"),
                                 reinterpret_cast<t_newmethod>(ofeliaMoon_new),
                                 reinterpret_cast<t_method>(ofeliaMoon_free),
                                 sizeof(t_ofeliaMoon),
                                 CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaMoon_class, reinterpret_cast<t_method>(ofeliaMoon_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaMoon_class, reinterpret_cast<t_method>(ofeliaMoon_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addbang(ofeliaMoon_class, reinterpret_cast<t_method>(ofeliaMoon_bang));
    class_addmethod(ofeliaMoon_class, reinterpret_cast<t_method>(ofeliaMoon_width),
                    gensym("width"), A_FLOAT, 0);
    class_addmethod(ofeliaMoon_class, reinterpret_cast<t_method>(ofeliaMoon_height),
                    gensym("height"), A_FLOAT, 0);
    class_addmethod(ofeliaMoon_class, reinterpret_cast<t_method>(ofeliaMoon_phase),
                    gensym("phase"), A_FLOAT, 0);
    class_addmethod(ofeliaMoon_class, reinterpret_cast<t_method>(ofeliaMoon_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaMoon_class, reinterpret_cast<t_method>(ofeliaMoon_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofRegPolygon object methods
 */
bool getSideRadFromArgs(const int argc, const t_atom *argv, t_ofeliaRegPolygonElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.radius = OFELIA_DEFAULT_SIZE_HALF;
            elem.numSides = 5;
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT) {
                
                elem.radius = argv[0].a_w.w_float;
                elem.numSides = static_cast<int>(argv[1].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaRegPolygon::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaRegPolygon::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaRegPolygon_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaRegPolygon *x = reinterpret_cast<t_ofeliaRegPolygon*>(pd_new(ofeliaRegPolygon_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getSideRadFromArgs(argc, argv, x->elem))
        return 0;
    x->shouldLoad = true;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("radius"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("numSides"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaRegPolygon_bang(t_ofeliaRegPolygon *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (x->shouldLoad) {
            
            const int numSides = x->elem.numSides;
            const float divAngle = 360.0f / numSides;
            const float radius = x->elem.radius;
            const float beginAngle = 90.0f - divAngle*0.5;
            float posX, posY;
            
            x->points.clear();
            getCoordinatesFromRadAngle(posX, posY, radius, beginAngle);
            x->points.push_back(ofVec2f(posX, posY));
            
            for (int i=1; i<numSides; ++i) {
                
                getCoordinatesFromRadAngle(posX, posY, radius, beginAngle + divAngle*i);
                x->points.push_back(ofVec2f(posX, posY));
            }
        }
        const ofStyle &style = ofGetStyle();
        
        // startSmoothing
        if (style.smoothing && !style.bFill) {
            
#ifndef TARGET_OPENGLES
            glPushAttrib(GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT);
#endif
            
            glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
            glEnable(GL_LINE_SMOOTH);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(2, GL_FLOAT, sizeof(ofVec2f), &x->points[0].x);
        glDrawArrays(style.bFill ? GL_TRIANGLE_FAN : GL_LINE_LOOP, 0, x->points.size());
        
        // endSmoothing
        if (style.smoothing && !style.bFill)
#ifndef TARGET_OPENGLES
            glPopAttrib();
#endif
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaRegPolygon::objName);
    }
}

void ofeliaRegPolygon_radius(t_ofeliaRegPolygon *x, t_floatarg f)
{
    x->elem.radius = f;
    x->shouldLoad = true;
}

void ofeliaRegPolygon_numSides(t_ofeliaRegPolygon *x, t_floatarg f)
{
    const int numSides = static_cast<int>(f);
    
    if (numSides > 1) {
        
        x->elem.numSides = numSides;
        x->shouldLoad = true;
    }
    else {
        
        error("%s: numSides out of range", t_ofeliaRegPolygon::objName);
    }
}

void ofeliaRegPolygon_set(t_ofeliaRegPolygon *x, t_symbol *s, int argc, t_atom *argv)
{
    if (getSideRadFromArgs(argc, argv, x->elem))
        x->shouldLoad = true;
}

void ofeliaRegPolygon_print(t_ofeliaRegPolygon *x)
{
    post("\n[%s]", t_ofeliaRegPolygon::objName);
    post("radius : %g", x->elem.radius);
    post("numSides : %d", x->elem.numSides);
}

void ofeliaRegPolygon_setup()
{
    ofeliaRegPolygon_class = class_new(gensym("ofRegPolygon"),
                                       reinterpret_cast<t_newmethod>(ofeliaRegPolygon_new),
                                       0, sizeof(t_ofeliaRegPolygon),
                                       CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaRegPolygon_class, reinterpret_cast<t_method>(ofeliaRegPolygon_bang));
    class_addmethod(ofeliaRegPolygon_class, reinterpret_cast<t_method>(ofeliaRegPolygon_radius),
                    gensym("radius"), A_FLOAT, 0);
    class_addmethod(ofeliaRegPolygon_class, reinterpret_cast<t_method>(ofeliaRegPolygon_numSides),
                    gensym("numSides"), A_FLOAT, 0);
    class_addmethod(ofeliaRegPolygon_class, reinterpret_cast<t_method>(ofeliaRegPolygon_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaRegPolygon_class, reinterpret_cast<t_method>(ofeliaRegPolygon_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofStar object methods
 */
bool getStarElemFromArgs(const int argc, const t_atom *argv, t_ofeliaStarElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.radius[0] = OFELIA_DEFAULT_SIZE_HALF;
            elem.radius[1] = OFELIA_DEFAULT_SIZE_HALF * 0.5f;
            elem.numPoints = 5;
            break;
        }
        case 3:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT) {
                
                elem.radius[0] = argv[0].a_w.w_float;
                elem.radius[1] = argv[1].a_w.w_float;
                elem.numPoints = static_cast<int>(argv[2].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaStar::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaStar::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaStar_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaStar *x = reinterpret_cast<t_ofeliaStar*>(pd_new(ofeliaStar_class));
    x->path = make_unique<ofPath>();
    x->mesh = make_unique<ofVboMesh>();
    x->polylines = make_unique<vector<ofPolyline>>();
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getStarElemFromArgs(argc, argv, x->elem))
        return 0;
    x->shouldLoad = true;
    x->bInitGate = false;
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("radius1"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("radius2"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("numPoints"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaStar_init(t_ofeliaStar *x)
{
    if (!x->bInitGate)
        return;
    x->path = make_unique<ofPath>();
    x->mesh = make_unique<ofVboMesh>();
    x->polylines = make_unique<vector<ofPolyline>>();
    x->shouldLoad = true;
}

void ofeliaStar_exit(t_ofeliaStar *x)
{
    x->bInitGate = true;
}

void ofeliaStar_bang(t_ofeliaStar *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const ofStyle &style = ofGetStyle();
        
        if (x->bFill != style.bFill) {
            
            x->bFill = style.bFill;
            x->shouldLoad = true;
        }
        if (x->shouldLoad) {
            
            const int sideNum = x->elem.numPoints * 2;
            const float divAngle = 360.0f / sideNum;
            const float beginAngle = 90.0f - divAngle;
            float posX, posY;
            
            x->path->clear();
            getCoordinatesFromRadAngle(posX, posY, x->elem.radius[0], beginAngle);
            x->path->moveTo(ofVec2f(posX, posY));
            
            for (int i=1; i<sideNum; ++i) {
                
                getCoordinatesFromRadAngle(posX, posY, x->elem.radius[i%2], beginAngle + divAngle*i);
                x->path->lineTo(ofVec2f(posX, posY));
            }
            x->path->close();
            
            if (!x->bFill)
                x->polylines->operator=(x->path->getOutline());
            else
                x->mesh->operator=(x->path->getTessellation());
            x->shouldLoad = false;
        }
        if (!x->bFill)
            for (size_t i=0; i<x->polylines->size(); ++i)
                x->polylines->at(i).draw();
        else
            x->mesh->draw();
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaStar::objName);
    }
}

void ofeliaStar_radius1(t_ofeliaStar *x, t_floatarg f)
{
    x->elem.radius[0] = f;
    x->shouldLoad = true;
}

void ofeliaStar_radius2(t_ofeliaStar *x, t_floatarg f)
{
    x->elem.radius[1] = f;
    x->shouldLoad = true;
}

void ofeliaStar_numPoints(t_ofeliaStar *x, t_floatarg f)
{
    const int numPoints = static_cast<int>(f);
    
    if (numPoints > 1) {
        
        x->elem.numPoints = numPoints;
        x->shouldLoad = true;
    }
    else {
        
        error("%s: numPoints out of range", t_ofeliaStar::objName);
    }
}

void ofeliaStar_set(t_ofeliaStar *x, t_symbol *s, int argc, t_atom *argv)
{
    if (getStarElemFromArgs(argc, argv, x->elem))
        x->shouldLoad = true;
}

void ofeliaStar_print(t_ofeliaStar *x)
{
    post("\n[%s]", t_ofeliaStar::objName);
    post("radius1 : %g", x->elem.radius[0]);
    post("radius2 : %g", x->elem.radius[1]);
    post("numPoints : %d", x->elem.numPoints);
}

void ofeliaStar_free(t_ofeliaStar *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
}

void ofeliaStar_setup()
{
    ofeliaStar_class = class_new(gensym("ofStar"),
                                 reinterpret_cast<t_newmethod>(ofeliaStar_new),
                                 reinterpret_cast<t_method>(ofeliaStar_free),
                                 sizeof(t_ofeliaStar),
                                 CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaStar_class, reinterpret_cast<t_method>(ofeliaStar_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaStar_class, reinterpret_cast<t_method>(ofeliaStar_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addbang(ofeliaStar_class, reinterpret_cast<t_method>(ofeliaStar_bang));
    class_addmethod(ofeliaStar_class, reinterpret_cast<t_method>(ofeliaStar_numPoints),
                    gensym("numPoints"), A_FLOAT, 0);
    class_addmethod(ofeliaStar_class, reinterpret_cast<t_method>(ofeliaStar_radius1),
                    gensym("radius1"), A_FLOAT, 0);
    class_addmethod(ofeliaStar_class, reinterpret_cast<t_method>(ofeliaStar_radius2),
                    gensym("radius2"), A_FLOAT, 0);
    class_addmethod(ofeliaStar_class, reinterpret_cast<t_method>(ofeliaStar_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaStar_class, reinterpret_cast<t_method>(ofeliaStar_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofAxis object methods
 */
bool getAxisElemFromArgs(const int argc, const t_atom *argv, t_ofeliaAxisElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.size = OFELIA_DEFAULT_SIZE;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                elem.size = argv[0].a_w.w_float;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaAxis::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaAxis::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaAxis_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaAxis *x = reinterpret_cast<t_ofeliaAxis*>(pd_new(ofeliaAxis_class));
    x->mesh = make_unique<ofVboMesh>();
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getAxisElemFromArgs(argc, argv, x->elem))
        return 0;
    x->shouldLoad = true;
    x->bInitGate = false;
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("size"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaAxis_init(t_ofeliaAxis *x)
{
    if (!x->bInitGate)
        return;
    x->mesh = make_unique<ofVboMesh>();
    x->shouldLoad = true;
}

void ofeliaAxis_exit(t_ofeliaAxis *x)
{
    x->bInitGate = true;
}

void ofeliaAxis_bang(t_ofeliaAxis *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (x->shouldLoad) {
            
            x->mesh->operator=(ofMesh::axis(x->elem.size));
            x->shouldLoad = false;
        }
        x->mesh->drawWireframe();
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaAxis::objName);
    }
}

void ofeliaAxis_size(t_ofeliaAxis *x, t_floatarg f)
{
    x->elem.size = f;
    x->shouldLoad = true;
}

void ofeliaAxis_set(t_ofeliaAxis *x, t_symbol *s, int argc, t_atom *argv)
{
    if (getAxisElemFromArgs(argc, argv, x->elem))
        x->shouldLoad = true;
}

void ofeliaAxis_print(t_ofeliaAxis *x)
{
    post("\n[%s]", t_ofeliaAxis::objName);
    post("size : %g", x->elem.size);
}

void ofeliaAxis_free(t_ofeliaAxis *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
}

void ofeliaAxis_setup()
{
    ofeliaAxis_class = class_new(gensym("ofAxis"),
                                 reinterpret_cast<t_newmethod>(ofeliaAxis_new),
                                 reinterpret_cast<t_method>(ofeliaAxis_free),
                                 sizeof(t_ofeliaAxis),
                                 CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaAxis_class, reinterpret_cast<t_method>(ofeliaAxis_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaAxis_class, reinterpret_cast<t_method>(ofeliaAxis_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addbang(ofeliaAxis_class, reinterpret_cast<t_method>(ofeliaAxis_bang));
    class_addmethod(ofeliaAxis_class, reinterpret_cast<t_method>(ofeliaAxis_size),
                    gensym("size"), A_FLOAT, 0);
    class_addmethod(ofeliaAxis_class, reinterpret_cast<t_method>(ofeliaAxis_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaAxis_class, reinterpret_cast<t_method>(ofeliaAxis_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofBox object methods
 */
bool getBoxElemFromArgs(const int argc, const t_atom *argv, t_ofeliaBoxElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.width = OFELIA_DEFAULT_SIZE;
            elem.height = OFELIA_DEFAULT_SIZE;
            elem.depth = OFELIA_DEFAULT_SIZE;
            elem.resX = 2;
            elem.resY = 2;
            elem.resZ = 2;
            break;
        }
        case 3:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT) {
                
                elem.width = argv[0].a_w.w_float;
                elem.height = argv[1].a_w.w_float;
                elem.depth = argv[2].a_w.w_float;
                elem.resX = 2;
                elem.resY = 2;
                elem.resZ = 2;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaBox::objName);
                return 0;
            }
            break;
        }
        case 6:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT &&
                argv[4].a_type == A_FLOAT &&
                argv[5].a_type == A_FLOAT) {
                
                elem.width = argv[0].a_w.w_float;
                elem.height = argv[1].a_w.w_float;
                elem.depth = argv[2].a_w.w_float;
                const int resX = static_cast<int>(argv[3].a_w.w_float);
                const int resY = static_cast<int>(argv[4].a_w.w_float);
                const int resZ = static_cast<int>(argv[5].a_w.w_float);
                
                if (resX > 0) {
                    
                    elem.resX = resX;
                }
                else {
                    
                    error("%s: resX out of range", t_ofeliaBox::objName);
                    return 0;
                }
                if (resY > 0) {
                    
                    elem.resY = resY;
                }
                else {
                    
                    error("%s: resY out of range", t_ofeliaBox::objName);
                    return 0;
                }
                if (resZ > 0) {
                    
                    elem.resZ = resZ;
                }
                else {
                    
                    error("%s: resZ out of range", t_ofeliaBox::objName);
                    return 0;
                }
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaBox::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaBox::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaBox_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaBox *x = reinterpret_cast<t_ofeliaBox*>(pd_new(ofeliaBox_class));
    x->mesh = make_unique<ofVboMesh>();
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getBoxElemFromArgs(argc, argv, x->elem))
        return 0;
    x->shouldLoad = true;
    x->bInitGate = false;
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("width"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("height"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("depth"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaBox_init(t_ofeliaBox *x)
{
    if (!x->bInitGate)
        return;
    x->mesh = make_unique<ofVboMesh>();
    x->shouldLoad = true;
}

void ofeliaBox_exit(t_ofeliaBox *x)
{
    x->bInitGate = true;
}

void ofeliaBox_bang(t_ofeliaBox *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (x->shouldLoad) {
            
            x->mesh->operator=(ofMesh::box(x->elem.width, x->elem.height, x->elem.depth,
                                           x->elem.resX, x->elem.resY, x->elem.resZ));
            x->shouldLoad = false;
        }
        if (!ofGetStyle().bFill)
            x->mesh->drawWireframe();
        else
            x->mesh->drawFaces();
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaBox::objName);
    }
}

void ofeliaBox_width(t_ofeliaBox *x, t_floatarg f)
{
    x->elem.width = f;
    x->shouldLoad = true;
}

void ofeliaBox_height(t_ofeliaBox *x, t_floatarg f)
{
    x->elem.height = f;
    x->shouldLoad = true;
}

void ofeliaBox_depth(t_ofeliaBox *x, t_floatarg f)
{
    x->elem.depth = f;
    x->shouldLoad = true;
}

void ofeliaBox_resX(t_ofeliaBox *x, t_floatarg f)
{
    const int resX = static_cast<int>(f);
    
    if (resX > 0) {
        
        x->elem.resX = resX;
        x->shouldLoad = true;
    }
    else {
        
        error("%s: resX out of range", t_ofeliaBox::objName);
    }
}

void ofeliaBox_resY(t_ofeliaBox *x, t_floatarg f)
{
    const int resY = static_cast<int>(f);
    
    if (resY > 0) {
        
        x->elem.resY = resY;
        x->shouldLoad = true;
    }
    else {
        
        error("%s: resY out of range", t_ofeliaBox::objName);
    }
}

void ofeliaBox_resZ(t_ofeliaBox *x, t_floatarg f)
{
    const int resZ = static_cast<int>(f);
    
    if (resZ > 0) {
        
        x->elem.resZ = resZ;
        x->shouldLoad = true;
    }
    else {
        
        error("%s: resZ out of range", t_ofeliaBox::objName);
    }
}

void ofeliaBox_set(t_ofeliaBox *x, t_symbol *s, int argc, t_atom *argv)
{
    if (getBoxElemFromArgs(argc, argv, x->elem))
        x->shouldLoad = true;
}

void ofeliaBox_print(t_ofeliaBox *x)
{
    post("\n[%s]", t_ofeliaBox::objName);
    post("width : %g", x->elem.width);
    post("height : %g", x->elem.height);
    post("depth : %g", x->elem.depth);
    post("resX : %d", x->elem.resX);
    post("resY : %d", x->elem.resY);
    post("resZ : %d", x->elem.resZ);
}

void ofeliaBox_free(t_ofeliaBox *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
}

void ofeliaBox_setup()
{
    ofeliaBox_class = class_new(gensym("ofBox"),
                                reinterpret_cast<t_newmethod>(ofeliaBox_new),
                                reinterpret_cast<t_method>(ofeliaBox_free),
                                sizeof(t_ofeliaBox),
                                CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaBox_class, reinterpret_cast<t_method>(ofeliaBox_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaBox_class, reinterpret_cast<t_method>(ofeliaBox_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addbang(ofeliaBox_class, reinterpret_cast<t_method>(ofeliaBox_bang));
    class_addmethod(ofeliaBox_class, reinterpret_cast<t_method>(ofeliaBox_width),
                    gensym("width"), A_FLOAT, 0);
    class_addmethod(ofeliaBox_class, reinterpret_cast<t_method>(ofeliaBox_height),
                    gensym("height"), A_FLOAT, 0);
    class_addmethod(ofeliaBox_class, reinterpret_cast<t_method>(ofeliaBox_depth),
                    gensym("depth"), A_FLOAT, 0);
    class_addmethod(ofeliaBox_class, reinterpret_cast<t_method>(ofeliaBox_resX),
                    gensym("resX"), A_FLOAT, 0);
    class_addmethod(ofeliaBox_class, reinterpret_cast<t_method>(ofeliaBox_resY),
                    gensym("resY"), A_FLOAT, 0);
    class_addmethod(ofeliaBox_class, reinterpret_cast<t_method>(ofeliaBox_resZ),
                    gensym("resZ"), A_FLOAT, 0);
    class_addmethod(ofeliaBox_class, reinterpret_cast<t_method>(ofeliaBox_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaBox_class, reinterpret_cast<t_method>(ofeliaBox_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofCone object methods
 */
bool getConeElemFromArgs(const int argc, const t_atom *argv, t_ofeliaConeElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.radius = OFELIA_DEFAULT_SIZE_HALF;
            elem.height = OFELIA_DEFAULT_SIZE;
            elem.radiusSegments = OFELIA_DEFAULT_RESOLUTION;
            elem.heightSegments = 6;
            elem.capSegments = 2;
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT) {
                
                elem.radius = argv[0].a_w.w_float;
                elem.height = argv[1].a_w.w_float;
                elem.radiusSegments = OFELIA_DEFAULT_RESOLUTION;
                elem.heightSegments = 6;
                elem.capSegments = 2;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaCone::objName);
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
                
                elem.radius = argv[0].a_w.w_float;
                elem.height = argv[1].a_w.w_float;
                const int radiusSegments = static_cast<int>(argv[2].a_w.w_float);
                const int heightSegments = static_cast<int>(argv[3].a_w.w_float);
                const int capSegments = static_cast<int>(argv[4].a_w.w_float);
                
                if (radiusSegments > 0) {
                    
                    elem.radiusSegments = radiusSegments;
                }
                else {
                    
                    error("%s: radiusSegments out of range", t_ofeliaCone::objName);
                    return 0;
                }
                if (heightSegments > 0) {
                    
                    elem.heightSegments = heightSegments;
                }
                else {
                    
                    error("%s: heightSegments out of range", t_ofeliaCone::objName);
                    return 0;
                }
                if (capSegments > 0) {
                    
                    elem.capSegments = capSegments;
                }
                else {
                    
                    error("%s: capSegments out of range", t_ofeliaCone::objName);
                    return 0;
                }
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaCone::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaCone::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaCone_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaCone *x = reinterpret_cast<t_ofeliaCone*>(pd_new(ofeliaCone_class));
    x->mesh = make_unique<ofVboMesh>();
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getConeElemFromArgs(argc, argv, x->elem))
        return 0;
    x->shouldLoad = true;
    x->bInitGate = false;
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("radius"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("height"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaCone_init(t_ofeliaCone *x)
{
    if (!x->bInitGate)
        return;
    x->mesh = make_unique<ofVboMesh>();
    x->shouldLoad = true;
}

void ofeliaCone_exit(t_ofeliaCone *x)
{
    x->bInitGate = true;
}

void ofeliaCone_bang(t_ofeliaCone *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (x->shouldLoad) {
            
            x->mesh->operator=(ofMesh::cone(x->elem.radius, x->elem.height,
                                            x->elem.radiusSegments,
                                            x->elem.heightSegments,
                                            x->elem.capSegments));
            x->shouldLoad = false;
        }
        if (!ofGetStyle().bFill)
            x->mesh->drawWireframe();
        else
            x->mesh->drawFaces();
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaCone::objName);
    }
}

void ofeliaCone_radius(t_ofeliaCone *x, t_floatarg f)
{
    x->elem.radius = f;
    x->shouldLoad = true;
}

void ofeliaCone_height(t_ofeliaCone *x, t_floatarg f)
{
    x->elem.height = f;
    x->shouldLoad = true;
}

void ofeliaCone_radiusSegments(t_ofeliaCone *x, t_floatarg f)
{
    const int radiusSegments = static_cast<int>(f);
    
    if (radiusSegments > 0) {
        
        x->elem.radiusSegments = radiusSegments;
        x->shouldLoad = true;
    }
    else {
        
        error("%s: radiusSegments out of range", t_ofeliaCone::objName);
    }
}

void ofeliaCone_heightSegments(t_ofeliaCone *x, t_floatarg f)
{
    const int heightSegments = static_cast<int>(f);
    
    if (heightSegments > 0) {
        
        x->elem.heightSegments = heightSegments;
        x->shouldLoad = true;
    }
    else {
        
        error("%s: heightSegments out of range", t_ofeliaCone::objName);
    }
}

void ofeliaCone_capSegments(t_ofeliaCone *x, t_floatarg f)
{
    const int capSegments = static_cast<int>(f);
    
    if (capSegments > 0) {
        
        x->elem.capSegments = capSegments;
        x->shouldLoad = true;
    }
    else {
        
        error("%s: capSegments out of range", t_ofeliaCone::objName);
    }
}

void ofeliaCone_set(t_ofeliaCone *x, t_symbol *s, int argc, t_atom *argv)
{
    if (getConeElemFromArgs(argc, argv, x->elem))
        x->shouldLoad = true;
}

void ofeliaCone_print(t_ofeliaCone *x)
{
    post("\n[%s]", t_ofeliaCone::objName);
    post("radius : %g", x->elem.radius);
    post("height : %g", x->elem.height);
    post("radiusSegments : %d", x->elem.radiusSegments);
    post("heightSegments : %d", x->elem.heightSegments);
    post("capSegments : %d", x->elem.capSegments);
}

void ofeliaCone_free(t_ofeliaCone *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
}

void ofeliaCone_setup()
{
    ofeliaCone_class = class_new(gensym("ofCone"),
                                 reinterpret_cast<t_newmethod>(ofeliaCone_new),
                                 reinterpret_cast<t_method>(ofeliaCone_free),
                                 sizeof(t_ofeliaCone),
                                 CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaCone_class, reinterpret_cast<t_method>(ofeliaCone_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaCone_class, reinterpret_cast<t_method>(ofeliaCone_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addbang(ofeliaCone_class, reinterpret_cast<t_method>(ofeliaCone_bang));
    class_addmethod(ofeliaCone_class, reinterpret_cast<t_method>(ofeliaCone_radius),
                    gensym("radius"), A_FLOAT, 0);
    class_addmethod(ofeliaCone_class, reinterpret_cast<t_method>(ofeliaCone_height),
                    gensym("height"), A_FLOAT, 0);
    class_addmethod(ofeliaCone_class, reinterpret_cast<t_method>(ofeliaCone_radiusSegments),
                    gensym("radiusSegments"), A_FLOAT, 0);
    class_addmethod(ofeliaCone_class, reinterpret_cast<t_method>(ofeliaCone_heightSegments),
                    gensym("heightSegments"), A_FLOAT, 0);
    class_addmethod(ofeliaCone_class, reinterpret_cast<t_method>(ofeliaCone_capSegments),
                    gensym("capSegments"), A_FLOAT, 0);
    class_addmethod(ofeliaCone_class, reinterpret_cast<t_method>(ofeliaCone_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaCone_class, reinterpret_cast<t_method>(ofeliaCone_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofCylinder object methods
 */
bool getCylinderElemFromArgs(const int argc, const t_atom *argv, t_ofeliaCylinderElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.radius = OFELIA_DEFAULT_SIZE_HALF;
            elem.height = OFELIA_DEFAULT_SIZE;
            elem.radiusSegments = OFELIA_DEFAULT_RESOLUTION;
            elem.heightSegments = 6;
            elem.capSegments = 2;
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT) {
                
                elem.radius = argv[0].a_w.w_float;
                elem.height = argv[1].a_w.w_float;
                elem.radiusSegments = OFELIA_DEFAULT_RESOLUTION;
                elem.heightSegments = 6;
                elem.capSegments = 2;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaCylinder::objName);
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
                
                elem.radius = argv[0].a_w.w_float;
                elem.height = argv[1].a_w.w_float;
                const int radiusSegments = static_cast<int>(argv[2].a_w.w_float);
                const int heightSegments = static_cast<int>(argv[3].a_w.w_float);
                const int capSegments = static_cast<int>(argv[4].a_w.w_float);
                
                if (radiusSegments > 0) {
                    
                    elem.radiusSegments = radiusSegments;
                }
                else {
                    
                    error("%s: radiusSegments out of range", t_ofeliaCylinder::objName);
                    return 0;
                }
                if (heightSegments > 0) {
                    
                    elem.heightSegments = heightSegments;
                }
                else {
                    
                    error("%s: heightSegments out of range", t_ofeliaCylinder::objName);
                    return 0;
                }
                if (capSegments > 0) {
                    
                    elem.capSegments = capSegments;
                }
                else {
                    
                    error("%s: capSegments out of range", t_ofeliaCylinder::objName);
                    return 0;
                }
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaCylinder::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaCylinder::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaCylinder_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaCylinder *x = reinterpret_cast<t_ofeliaCylinder*>(pd_new(ofeliaCylinder_class));
    x->mesh = make_unique<ofVboMesh>();
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getCylinderElemFromArgs(argc, argv, x->elem))
        return 0;
    x->shouldLoad = true;
    x->bInitGate = false;
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("radius"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("height"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaCylinder_init(t_ofeliaCylinder *x)
{
    if (!x->bInitGate)
        return;
    x->mesh = make_unique<ofVboMesh>();
    x->shouldLoad = true;
}

void ofeliaCylinder_exit(t_ofeliaCylinder *x)
{
    x->bInitGate = true;
}

void ofeliaCylinder_bang(t_ofeliaCylinder *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (x->shouldLoad) {
            
            x->mesh->operator=(ofMesh::cylinder(x->elem.radius, x->elem.height,
                                                x->elem.radiusSegments,
                                                x->elem.heightSegments,
                                                x->elem.capSegments));
            x->shouldLoad = false;
        }
        if (!ofGetStyle().bFill)
            x->mesh->drawWireframe();
        else
            x->mesh->drawFaces();
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaCylinder::objName);
    }
}

void ofeliaCylinder_radius(t_ofeliaCylinder *x, t_floatarg f)
{
    x->elem.radius = f;
    x->shouldLoad = true;
}

void ofeliaCylinder_height(t_ofeliaCylinder *x, t_floatarg f)
{
    x->elem.height = f;
    x->shouldLoad = true;
}

void ofeliaCylinder_radiusSegments(t_ofeliaCylinder *x, t_floatarg f)
{
    const int radiusSegments = static_cast<int>(f);
    
    if (radiusSegments > 0) {
        
        x->elem.radiusSegments = radiusSegments;
        x->shouldLoad = true;
    }
    else {
        
        error("%s: radiusSegments out of range", t_ofeliaCylinder::objName);
    }
}

void ofeliaCylinder_heightSegments(t_ofeliaCylinder *x, t_floatarg f)
{
    const int heightSegments = static_cast<int>(f);
    
    if (heightSegments > 0) {
        
        x->elem.heightSegments = heightSegments;
        x->shouldLoad = true;
    }
    else {
        
        error("%s: heightSegments out of range", t_ofeliaCylinder::objName);
    }
}

void ofeliaCylinder_capSegments(t_ofeliaCylinder *x, t_floatarg f)
{
    const int capSegments = static_cast<int>(f);
    
    if (capSegments > 0) {
        
        x->elem.capSegments = capSegments;
        x->shouldLoad = true;
    }
    else {
        
        error("%s: capSegments out of range", t_ofeliaCylinder::objName);
    }
}

void ofeliaCylinder_set(t_ofeliaCylinder *x, t_symbol *s, int argc, t_atom *argv)
{
    if (getCylinderElemFromArgs(argc, argv, x->elem))
        x->shouldLoad = true;
}

void ofeliaCylinder_print(t_ofeliaCylinder *x)
{
    post("\n[%s]", t_ofeliaCylinder::objName);
    post("radius : %g", x->elem.radius);
    post("height : %g", x->elem.height);
    post("radiusSegments : %d", x->elem.radiusSegments);
    post("heightSegments : %d", x->elem.heightSegments);
    post("capSegments : %d", x->elem.capSegments);
}

void ofeliaCylinder_free(t_ofeliaCylinder *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
}

void ofeliaCylinder_setup()
{
    ofeliaCylinder_class = class_new(gensym("ofCylinder"),
                                     reinterpret_cast<t_newmethod>(ofeliaCylinder_new),
                                     reinterpret_cast<t_method>(ofeliaCylinder_free),
                                     sizeof(t_ofeliaCylinder),
                                     CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaCylinder_class, reinterpret_cast<t_method>(ofeliaCylinder_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaCylinder_class, reinterpret_cast<t_method>(ofeliaCylinder_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addbang(ofeliaCylinder_class, reinterpret_cast<t_method>(ofeliaCylinder_bang));
    class_addmethod(ofeliaCylinder_class, reinterpret_cast<t_method>(ofeliaCylinder_radius),
                    gensym("radius"), A_FLOAT, 0);
    class_addmethod(ofeliaCylinder_class, reinterpret_cast<t_method>(ofeliaCylinder_height),
                    gensym("height"), A_FLOAT, 0);
    class_addmethod(ofeliaCylinder_class, reinterpret_cast<t_method>(ofeliaCylinder_radiusSegments),
                    gensym("radiusSegments"), A_FLOAT, 0);
    class_addmethod(ofeliaCylinder_class, reinterpret_cast<t_method>(ofeliaCylinder_heightSegments),
                    gensym("heightSegments"), A_FLOAT, 0);
    class_addmethod(ofeliaCylinder_class, reinterpret_cast<t_method>(ofeliaCylinder_capSegments),
                    gensym("capSegments"), A_FLOAT, 0);
    class_addmethod(ofeliaCylinder_class, reinterpret_cast<t_method>(ofeliaCylinder_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaCylinder_class, reinterpret_cast<t_method>(ofeliaCylinder_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofIcosphere object methods
 */
bool getIcosphereElemFromArgs(const int argc, const t_atom *argv, t_ofeliaIcosphereElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.radius = OFELIA_DEFAULT_SIZE_HALF;
            elem.iterations = 2;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                elem.radius = argv[0].a_w.w_float;
                elem.iterations = 2;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaIcosphere::objName);
                return 0;
            }
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT) {
                
                elem.radius = argv[0].a_w.w_float;
                const int iterations = static_cast<int>(argv[1].a_w.w_float);
                
                if (iterations < 1 || iterations > 5) {
                    
                    error("%s: iterations out of range", t_ofeliaIcosphere::objName);
                    return 0;
                }
                elem.iterations = static_cast<size_t>(iterations);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaIcosphere::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaIcosphere::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaIcosphere_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaIcosphere *x = reinterpret_cast<t_ofeliaIcosphere*>(pd_new(ofeliaIcosphere_class));
    x->mesh = make_unique<ofVboMesh>();
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getIcosphereElemFromArgs(argc, argv, x->elem))
        return 0;
    x->shouldLoad = true;
    x->bInitGate = false;
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("radius"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaIcosphere_init(t_ofeliaIcosphere *x)
{
    if (!x->bInitGate)
        return;
    x->mesh = make_unique<ofVboMesh>();
    x->shouldLoad = true;
}

void ofeliaIcosphere_exit(t_ofeliaIcosphere *x)
{
    x->bInitGate = true;
}

void ofeliaIcosphere_bang(t_ofeliaIcosphere *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (x->shouldLoad) {
            
            x->mesh->operator=(ofMesh::icosphere(x->elem.radius, x->elem.iterations));
            x->shouldLoad = false;
        }
        if (!ofGetStyle().bFill)
            x->mesh->drawWireframe();
        else
            x->mesh->drawFaces();
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaIcosphere::objName);
    }
}

void ofeliaIcosphere_radius(t_ofeliaIcosphere *x, t_floatarg f)
{
    x->elem.radius = f;
    x->shouldLoad = true;
}

void ofeliaIcosphere_iterations(t_ofeliaIcosphere *x, t_floatarg f)
{
    const int iterations = static_cast<int>(f);
    
    if (iterations < 1 || iterations > 5) {
        
        error("%s: iterations out of range", t_ofeliaIcosphere::objName);
        return;
    }
    x->elem.iterations = static_cast<size_t>(iterations);
    x->shouldLoad = true;
}

void ofeliaIcosphere_set(t_ofeliaIcosphere *x, t_symbol *s, int argc, t_atom *argv)
{
    if (getIcosphereElemFromArgs(argc, argv, x->elem))
        x->shouldLoad = true;
}

void ofeliaIcosphere_print(t_ofeliaIcosphere *x)
{
    post("\n[%s]", t_ofeliaIcosphere::objName);
    post("radius : %g", x->elem.radius);
    post("iterations : %d", x->elem.iterations);
}

void ofeliaIcosphere_free(t_ofeliaIcosphere *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
}

void ofeliaIcosphere_setup()
{
    ofeliaIcosphere_class = class_new(gensym("ofIcosphere"),
                                      reinterpret_cast<t_newmethod>(ofeliaIcosphere_new),
                                      reinterpret_cast<t_method>(ofeliaIcosphere_free),
                                      sizeof(t_ofeliaIcosphere),
                                      CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaIcosphere_class, reinterpret_cast<t_method>(ofeliaIcosphere_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaIcosphere_class, reinterpret_cast<t_method>(ofeliaIcosphere_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addbang(ofeliaIcosphere_class, reinterpret_cast<t_method>(ofeliaIcosphere_bang));
    class_addmethod(ofeliaIcosphere_class, reinterpret_cast<t_method>(ofeliaIcosphere_radius),
                    gensym("radius"), A_FLOAT, 0);
    class_addmethod(ofeliaIcosphere_class, reinterpret_cast<t_method>(ofeliaIcosphere_iterations),
                    gensym("iterations"), A_FLOAT, 0);
    class_addmethod(ofeliaIcosphere_class, reinterpret_cast<t_method>(ofeliaIcosphere_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaIcosphere_class, reinterpret_cast<t_method>(ofeliaIcosphere_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofPlane object methods
 */
bool getPlaneElemFromArgs(const int argc, const t_atom *argv, t_ofeliaPlaneElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.width = OFELIA_DEFAULT_SIZE;
            elem.height = OFELIA_DEFAULT_SIZE;
            elem.columns = 2;
            elem.rows = 2;
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT) {
                
                elem.width = argv[0].a_w.w_float;
                elem.height = argv[1].a_w.w_float;
                elem.columns = 2;
                elem.rows = 2;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaPlane::objName);
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
                
                elem.width = argv[0].a_w.w_float;
                elem.height = argv[1].a_w.w_float;
                const int columns = static_cast<int>(argv[2].a_w.w_float);
                const int rows = static_cast<int>(argv[3].a_w.w_float);
                
                if (columns > 0) {
                    
                    elem.columns = columns;
                }
                else {
                    
                    error("%s: columns out of range", t_ofeliaPlane::objName);
                    return 0;
                }
                if (rows > 0) {
                    
                    elem.rows = rows;
                }
                else {
                    
                    error("%s: rows out of range", t_ofeliaPlane::objName);
                    return 0;
                }
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaPlane::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaPlane::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaPlane_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaPlane *x = reinterpret_cast<t_ofeliaPlane*>(pd_new(ofeliaPlane_class));
    x->mesh = make_unique<ofVboMesh>();
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getPlaneElemFromArgs(argc, argv, x->elem))
        return 0;
    x->shouldLoad = true;
    x->bInitGate = false;
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("width"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("height"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaPlane_init(t_ofeliaPlane *x)
{
    if (!x->bInitGate)
        return;
    x->mesh = make_unique<ofVboMesh>();
    x->shouldLoad = true;
}

void ofeliaPlane_exit(t_ofeliaPlane *x)
{
    x->bInitGate = true;
}

void ofeliaPlane_bang(t_ofeliaPlane *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (x->shouldLoad) {
            
            x->mesh->operator=(ofMesh::plane(x->elem.width, x->elem.height,
                                             x->elem.columns, x->elem.rows));
            x->shouldLoad = false;
        }
        if (!ofGetStyle().bFill)
            x->mesh->drawWireframe();
        else
            x->mesh->drawFaces();
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaPlane::objName);
    }
}

void ofeliaPlane_width(t_ofeliaPlane *x, t_floatarg f)
{
    x->elem.width = f;
    x->shouldLoad = true;
}

void ofeliaPlane_height(t_ofeliaPlane *x, t_floatarg f)
{
    x->elem.height = f;
    x->shouldLoad = true;
}

void ofeliaPlane_columns(t_ofeliaPlane *x, t_floatarg f)
{
    const int columns = static_cast<int>(f);
    
    if (columns > 0) {
        
        x->elem.columns = columns;
        x->shouldLoad = true;
    }
    else {
        
        error("%s: columns out of range", t_ofeliaPlane::objName);
    }
}

void ofeliaPlane_rows(t_ofeliaPlane *x, t_floatarg f)
{
    const int rows = static_cast<int>(f);
    
    if (rows > 0) {
        
        x->elem.rows = rows;
        x->shouldLoad = true;
    }
    else {
        
        error("%s: rows out of range", t_ofeliaPlane::objName);
    }
}

void ofeliaPlane_set(t_ofeliaPlane *x, t_symbol *s, int argc, t_atom *argv)
{
    if (getPlaneElemFromArgs(argc, argv, x->elem))
        x->shouldLoad = true;
}

void ofeliaPlane_print(t_ofeliaPlane *x)
{
    post("\n[%s]", t_ofeliaPlane::objName);
    post("width : %g", x->elem.width);
    post("height : %g", x->elem.height);
    post("columns : %d", x->elem.columns);
    post("rows : %d", x->elem.rows);
}

void ofeliaPlane_free(t_ofeliaPlane *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
}

void ofeliaPlane_setup()
{
    ofeliaPlane_class = class_new(gensym("ofPlane"),
                                  reinterpret_cast<t_newmethod>(ofeliaPlane_new),
                                  reinterpret_cast<t_method>(ofeliaPlane_free),
                                  sizeof(t_ofeliaPlane),
                                  CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaPlane_class, reinterpret_cast<t_method>(ofeliaPlane_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaPlane_class, reinterpret_cast<t_method>(ofeliaPlane_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addbang(ofeliaPlane_class, reinterpret_cast<t_method>(ofeliaPlane_bang));
    class_addmethod(ofeliaPlane_class, reinterpret_cast<t_method>(ofeliaPlane_width),
                    gensym("width"), A_FLOAT, 0);
    class_addmethod(ofeliaPlane_class, reinterpret_cast<t_method>(ofeliaPlane_height),
                    gensym("height"), A_FLOAT, 0);
    class_addmethod(ofeliaPlane_class, reinterpret_cast<t_method>(ofeliaPlane_columns),
                    gensym("columns"), A_FLOAT, 0);
    class_addmethod(ofeliaPlane_class, reinterpret_cast<t_method>(ofeliaPlane_rows),
                    gensym("rows"), A_FLOAT, 0);
    class_addmethod(ofeliaPlane_class, reinterpret_cast<t_method>(ofeliaPlane_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaPlane_class, reinterpret_cast<t_method>(ofeliaPlane_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSphere object methods
 */
bool getSphereElemFromArgs(const int argc, const t_atom *argv, t_ofeliaSphereElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.radius = OFELIA_DEFAULT_SIZE_HALF;
            elem.sphereRes = OFELIA_DEFAULT_RESOLUTION;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                elem.radius = argv[0].a_w.w_float;
                elem.sphereRes = OFELIA_DEFAULT_RESOLUTION;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSphere::objName);
                return 0;
            }
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT) {
                
                elem.radius = argv[0].a_w.w_float;
                const int res = static_cast<int>(argv[1].a_w.w_float);
                
                if (res > 0) {
                    
                    elem.sphereRes = res;
                }
                else {
                    
                    error("%s: sphereRes out of range", t_ofeliaSphere::objName);
                    return 0;
                }
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSphere::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaSphere::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaSphere_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSphere *x = reinterpret_cast<t_ofeliaSphere*>(pd_new(ofeliaSphere_class));
    x->mesh = make_unique<ofVboMesh>();
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getSphereElemFromArgs(argc, argv, x->elem))
        return 0;
    x->shouldLoad = true;
    x->bInitGate = false;
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("radius"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSphere_init(t_ofeliaSphere *x)
{
    if (!x->bInitGate)
        return;
    x->mesh = make_unique<ofVboMesh>();
    x->shouldLoad = true;
}

void ofeliaSphere_exit(t_ofeliaSphere *x)
{
    x->bInitGate = true;
}

void ofeliaSphere_bang(t_ofeliaSphere *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (x->shouldLoad) {
            
            x->mesh->operator=(ofMesh::sphere(x->elem.radius, x->elem.sphereRes));
            x->shouldLoad = false;
        }
        if (!ofGetStyle().bFill)
            x->mesh->drawWireframe();
        else
            x->mesh->drawFaces();
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSphere::objName);
    }
}

void ofeliaSphere_radius(t_ofeliaSphere *x, t_floatarg f)
{
    x->elem.radius = f;
    x->shouldLoad = true;
}

void ofeliaSphere_res(t_ofeliaSphere *x, t_floatarg f)
{
    const int res = static_cast<int>(f);
    
    if (res > 0) {
        
        x->elem.sphereRes = res;
        x->shouldLoad = true;
    }
    else {
        
        error("%s: sphereRes out of range", t_ofeliaSphere::objName);
    }
}

void ofeliaSphere_set(t_ofeliaSphere *x, t_symbol *s, int argc, t_atom *argv)
{
    if (getSphereElemFromArgs(argc, argv, x->elem))
        x->shouldLoad = true;
}

void ofeliaSphere_print(t_ofeliaSphere *x)
{
    post("\n[%s]", t_ofeliaSphere::objName);
    post("radius : %g", x->elem.radius);
    post("sphereRes : %d", x->elem.sphereRes);
}

void ofeliaSphere_free(t_ofeliaSphere *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
}

void ofeliaSphere_setup()
{
    ofeliaSphere_class = class_new(gensym("ofSphere"),
                                   reinterpret_cast<t_newmethod>(ofeliaSphere_new),
                                   reinterpret_cast<t_method>(ofeliaSphere_free),
                                   sizeof(t_ofeliaSphere),
                                   CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaSphere_class, reinterpret_cast<t_method>(ofeliaSphere_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaSphere_class, reinterpret_cast<t_method>(ofeliaSphere_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addbang(ofeliaSphere_class, reinterpret_cast<t_method>(ofeliaSphere_bang));
    class_addmethod(ofeliaSphere_class, reinterpret_cast<t_method>(ofeliaSphere_radius),
                    gensym("radius"), A_FLOAT, 0);
    class_addmethod(ofeliaSphere_class, reinterpret_cast<t_method>(ofeliaSphere_res),
                    gensym("sphereRes"), A_FLOAT, 0);
    class_addmethod(ofeliaSphere_class, reinterpret_cast<t_method>(ofeliaSphere_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSphere_class, reinterpret_cast<t_method>(ofeliaSphere_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofArrow object methods
 */
bool getArrowElemFromArgs(const int argc, const t_atom *argv, t_ofeliaArrowElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.start.set(-OFELIA_DEFAULT_SIZE_HALF, 0.0f, 0.0f);
            elem.end.set(OFELIA_DEFAULT_SIZE_HALF, 0.0f, 0.0f);
            elem.headSize = t_ofeliaShapesDefault::arrowHeadSize;
            break;
        }
        case 6:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT &&
                argv[4].a_type == A_FLOAT &&
                argv[5].a_type == A_FLOAT) {
                
                elem.start.set(argv[0].a_w.w_float,
                               argv[1].a_w.w_float,
                               argv[2].a_w.w_float);
                elem.end.set(argv[3].a_w.w_float,
                             argv[4].a_w.w_float,
                             argv[5].a_w.w_float);
                elem.headSize = t_ofeliaShapesDefault::arrowHeadSize;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaArrow::objName);
                return 0;
            }
            break;
        }
        case 7:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT &&
                argv[4].a_type == A_FLOAT &&
                argv[5].a_type == A_FLOAT &&
                argv[6].a_type == A_FLOAT) {
                
                elem.start.set(argv[0].a_w.w_float,
                               argv[1].a_w.w_float,
                               argv[2].a_w.w_float);
                elem.end.set(argv[3].a_w.w_float,
                             argv[4].a_w.w_float,
                             argv[5].a_w.w_float);
                elem.headSize = argv[6].a_w.w_float;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaArrow::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaArrow::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaArrow_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaArrow *x = reinterpret_cast<t_ofeliaArrow*>(pd_new(ofeliaArrow_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getArrowElemFromArgs(argc, argv, x->elem))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("start"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("end"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaArrow_bang(t_ofeliaArrow *x)
{
    if (t_ofeliaWindow::bRenderGate) {
      
        ofGetCurrentRenderer()->drawArrow(x->elem.start, x->elem.end, x->elem.headSize);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaArrow::objName);
    }
}

void ofeliaArrow_start(t_ofeliaArrow *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc == 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            x->elem.start.set(argv[0].a_w.w_float,
                              argv[1].a_w.w_float,
                              argv[2].a_w.w_float);
        }
        else
            error("%s: wrong argument type", t_ofeliaArrow::objName);
    }
    else
        error("%s: invalid number of arguments", t_ofeliaArrow::objName);
}

void ofeliaArrow_end(t_ofeliaArrow *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc == 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            x->elem.end.set(argv[0].a_w.w_float,
                            argv[1].a_w.w_float,
                            argv[2].a_w.w_float);
        }
        else
            error("%s: wrong argument type", t_ofeliaArrow::objName);
    }
    else
        error("%s: invalid number of arguments", t_ofeliaArrow::objName);
}

void ofeliaArrow_headSize(t_ofeliaArrow *x, t_floatarg f)
{
    x->elem.headSize = f;
}

void ofeliaArrow_set(t_ofeliaArrow *x, t_symbol *s, int argc, t_atom *argv)
{
    getArrowElemFromArgs(argc, argv, x->elem);
}

void ofeliaArrow_print(t_ofeliaArrow *x)
{
    post("\n[%s]", t_ofeliaArrow::objName);
    post("start : %g %g %g", x->elem.start.x, x->elem.start.y, x->elem.start.z);
    post("end : %g %g %g", x->elem.end.x, x->elem.end.y, x->elem.end.z);
    post("headSize : %g", x->elem.headSize);
}

void ofeliaArrow_setup()
{
    ofeliaArrow_class = class_new(gensym("ofArrow"),
                                  reinterpret_cast<t_newmethod>(ofeliaArrow_new),
                                  0, sizeof(t_ofeliaArrow),
                                  CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaArrow_class, reinterpret_cast<t_method>(ofeliaArrow_bang));
    class_addmethod(ofeliaArrow_class, reinterpret_cast<t_method>(ofeliaArrow_start),
                    gensym("start"), A_GIMME, 0);
    class_addmethod(ofeliaArrow_class, reinterpret_cast<t_method>(ofeliaArrow_end),
                    gensym("end"), A_GIMME, 0);
    class_addmethod(ofeliaArrow_class, reinterpret_cast<t_method>(ofeliaArrow_headSize),
                    gensym("headSize"), A_FLOAT, 0);
    class_addmethod(ofeliaArrow_class, reinterpret_cast<t_method>(ofeliaArrow_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaArrow_class, reinterpret_cast<t_method>(ofeliaArrow_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGrid object methods
 */
bool getGridElemFromArgs(const int argc, const t_atom *argv, t_ofeliaGridElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.stepSize = 50.0f;
            elem.numberOfSteps = 16;
            elem.visX = true;
            elem.visY = true;
            elem.visZ = true;
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT) {
                
                elem.stepSize = argv[0].a_w.w_float;
                elem.numberOfSteps = static_cast<int>(argv[1].a_w.w_float);
                elem.visX = true;
                elem.visY = true;
                elem.visZ = true;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaGrid::objName);
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
                
                elem.stepSize = argv[0].a_w.w_float;
                elem.numberOfSteps = static_cast<int>(argv[1].a_w.w_float);
                elem.visX = argv[2].a_w.w_float != 0.0f;
                elem.visY = argv[3].a_w.w_float != 0.0f;
                elem.visZ = argv[4].a_w.w_float != 0.0f;
            }
            else if (argv[0].a_type == A_FLOAT &&
                     argv[1].a_type == A_FLOAT &&
                     argv[2].a_type == A_SYMBOL &&
                     argv[3].a_type == A_SYMBOL &&
                     argv[4].a_type == A_SYMBOL) {
                
                elem.stepSize = argv[0].a_w.w_float;
                elem.numberOfSteps = static_cast<int>(argv[1].a_w.w_float);
                
                if (!getToggleFromSym(argv[2].a_w.w_symbol, elem.visX, t_ofeliaGrid::objName) ||
                    !getToggleFromSym(argv[3].a_w.w_symbol, elem.visY, t_ofeliaGrid::objName) ||
                    !getToggleFromSym(argv[4].a_w.w_symbol, elem.visZ, t_ofeliaGrid::objName))
                    return 0;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaGrid::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaGrid::objName);
            return 0;
        }
    }
    if (elem.numberOfSteps < 0) {
        
        error("%s: numberOfSteps out of range", t_ofeliaGrid::objName);
        return 0;
    }
    return 1;
}

void *ofeliaGrid_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaGrid *x = reinterpret_cast<t_ofeliaGrid*>(pd_new(ofeliaGrid_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getGridElemFromArgs(argc, argv, x->elem))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("stepSize"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("numberOfSteps"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaGrid_bang(t_ofeliaGrid *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofGetCurrentRenderer()->drawGrid(x->elem.stepSize,
                                         static_cast<size_t>(x->elem.numberOfSteps),
                                         false, x->elem.visX, x->elem.visY, x->elem.visZ);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaGrid::objName);
    }
}

void ofeliaGrid_stepSize(t_ofeliaGrid *x, t_floatarg f)
{
    x->elem.stepSize = f;
}

void ofeliaGrid_numberOfSteps(t_ofeliaGrid *x, t_floatarg f)
{
    const int numberOfSteps = static_cast<int>(f);
    
    if (numberOfSteps >= 0)
        x->elem.numberOfSteps = numberOfSteps;
    else
        error("%s: numberOfSteps out of range", t_ofeliaGrid::objName);
}

void ofeliaGrid_visX(t_ofeliaGrid *x, t_symbol *s, int argc, t_atom *argv)
{
    getToggleFromArgs(argc, argv, x->elem.visX, t_ofeliaGrid::objName);
}

void ofeliaGrid_visY(t_ofeliaGrid *x, t_symbol *s, int argc, t_atom *argv)
{
    getToggleFromArgs(argc, argv, x->elem.visY, t_ofeliaGrid::objName);
}

void ofeliaGrid_visZ(t_ofeliaGrid *x, t_symbol *s, int argc, t_atom *argv)
{
    getToggleFromArgs(argc, argv, x->elem.visZ, t_ofeliaGrid::objName);
}

void ofeliaGrid_set(t_ofeliaGrid *x, t_symbol *s, int argc, t_atom *argv)
{
    getGridElemFromArgs(argc, argv, x->elem);
}

void ofeliaGrid_print(t_ofeliaGrid *x)
{
    post("\n[%s]", t_ofeliaGrid::objName);
    post("stepSize : %g", x->elem.stepSize);
    post("numberOfSteps : %d", x->elem.numberOfSteps);
    printToggle("visX", x->elem.visX);
    printToggle("visY", x->elem.visY);
    printToggle("visZ", x->elem.visZ);
}

void ofeliaGrid_setup()
{
    ofeliaGrid_class = class_new(gensym("ofGrid"),
                                 reinterpret_cast<t_newmethod>(ofeliaGrid_new),
                                 0, sizeof(t_ofeliaGrid),
                                 CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaGrid_class, reinterpret_cast<t_method>(ofeliaGrid_bang));
    class_addmethod(ofeliaGrid_class, reinterpret_cast<t_method>(ofeliaGrid_stepSize),
                    gensym("stepSize"), A_FLOAT, 0);
    class_addmethod(ofeliaGrid_class, reinterpret_cast<t_method>(ofeliaGrid_numberOfSteps),
                    gensym("numberOfSteps"), A_FLOAT, 0);
    class_addmethod(ofeliaGrid_class, reinterpret_cast<t_method>(ofeliaGrid_visX),
                    gensym("visX"), A_GIMME, 0);
    class_addmethod(ofeliaGrid_class, reinterpret_cast<t_method>(ofeliaGrid_visY),
                    gensym("visY"), A_GIMME, 0);
    class_addmethod(ofeliaGrid_class, reinterpret_cast<t_method>(ofeliaGrid_visZ),
                    gensym("visZ"), A_GIMME, 0);
    class_addmethod(ofeliaGrid_class, reinterpret_cast<t_method>(ofeliaGrid_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaGrid_class, reinterpret_cast<t_method>(ofeliaGrid_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGridPlane object methods
 */
bool getGridPlaneElemFromArgs(const int argc, const t_atom *argv, t_ofeliaGridPlaneElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.stepSize = 50.0f;
            elem.numberOfSteps = 16;
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT) {
                
                elem.stepSize = argv[0].a_w.w_float;
                elem.numberOfSteps = static_cast<int>(argv[1].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaGridPlane::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaGridPlane::objName);
            return 0;
        }
    }
    if (elem.numberOfSteps < 0) {
        
        error("%s: numberOfSteps out of range", t_ofeliaGridPlane::objName);
        return 0;
    }
    return 1;
}

void *ofeliaGridPlane_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaGridPlane *x = reinterpret_cast<t_ofeliaGridPlane*>(pd_new(ofeliaGridPlane_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getGridPlaneElemFromArgs(argc, argv, x->elem))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("stepSize"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("numberOfSteps"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaGridPlane_bang(t_ofeliaGridPlane *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        ofGetCurrentRenderer()->drawGridPlane(x->elem.stepSize,
                                              static_cast<size_t>(x->elem.numberOfSteps), false);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaGridPlane::objName);
    }
}

void ofeliaGridPlane_stepSize(t_ofeliaGridPlane *x, t_floatarg f)
{
    x->elem.stepSize = f;
}

void ofeliaGridPlane_numberOfSteps(t_ofeliaGridPlane *x, t_floatarg f)
{
    const int numberOfSteps = static_cast<int>(f);
    
    if (numberOfSteps >= 0)
        x->elem.numberOfSteps = numberOfSteps;
    else
        error("%s: numberOfSteps out of range", t_ofeliaGridPlane::objName);
}

void ofeliaGridPlane_set(t_ofeliaGridPlane *x, t_symbol *s, int argc, t_atom *argv)
{
    getGridPlaneElemFromArgs(argc, argv, x->elem);
}

void ofeliaGridPlane_print(t_ofeliaGridPlane *x)
{
    post("\n[%s]", t_ofeliaGridPlane::objName);
    post("stepSize : %g", x->elem.stepSize);
    post("numberOfSteps : %d", x->elem.numberOfSteps);
}

void ofeliaGridPlane_setup()
{
    ofeliaGridPlane_class = class_new(gensym("ofGridPlane"),
                                      reinterpret_cast<t_newmethod>(ofeliaGridPlane_new),
                                      0, sizeof(t_ofeliaGridPlane),
                                      CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaGridPlane_class, reinterpret_cast<t_method>(ofeliaGridPlane_bang));
    class_addmethod(ofeliaGridPlane_class, reinterpret_cast<t_method>(ofeliaGridPlane_stepSize),
                    gensym("stepSize"), A_FLOAT, 0);
    class_addmethod(ofeliaGridPlane_class, reinterpret_cast<t_method>(ofeliaGridPlane_numberOfSteps),
                    gensym("numberOfSteps"), A_FLOAT, 0);
    class_addmethod(ofeliaGridPlane_class, reinterpret_cast<t_method>(ofeliaGridPlane_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaGridPlane_class, reinterpret_cast<t_method>(ofeliaGridPlane_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofRotationAxes object methods
 */
bool getRotationAxesElemFromArgs(const int argc, const t_atom *argv, t_ofeliaRotationAxesElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.radius = OFELIA_DEFAULT_SIZE_HALF;
            elem.stripWidth = 2.0f;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                elem.radius = argv[0].a_w.w_float;
                elem.stripWidth = 2.0f;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaRotationAxes::objName);
                return 0;
            }
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT) {
                
                elem.radius = argv[0].a_w.w_float;
                elem.stripWidth = argv[1].a_w.w_float;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaRotationAxes::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaRotationAxes::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaRotationAxes_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaRotationAxes *x = reinterpret_cast<t_ofeliaRotationAxes*>(pd_new(ofeliaRotationAxes_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getRotationAxesElemFromArgs(argc, argv, x->elem))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("radius"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("stripWidth"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaRotationAxes_bang(t_ofeliaRotationAxes *x)
{
    if (t_ofeliaWindow::bRenderGate) {
    
        ofGetCurrentRenderer()->drawRotationAxes(x->elem.radius,
                                                 x->elem.stripWidth,
                                                 ofGetStyle().circleResolution);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaRotationAxes::objName);
    }
}

void ofeliaRotationAxes_radius(t_ofeliaRotationAxes *x, t_floatarg f)
{
    x->elem.radius = f;
}

void ofeliaRotationAxes_stripWidth(t_ofeliaRotationAxes *x, t_floatarg f)
{
    x->elem.stripWidth = f;
}

void ofeliaRotationAxes_set(t_ofeliaRotationAxes *x, t_symbol *s, int argc, t_atom *argv)
{
    getRotationAxesElemFromArgs(argc, argv, x->elem);
}

void ofeliaRotationAxes_print(t_ofeliaRotationAxes *x)
{
    post("\n[%s]", t_ofeliaRotationAxes::objName);
    post("radius : %g", x->elem.radius);
    post("stripWidth : %g", x->elem.stripWidth);
}

void ofeliaRotationAxes_setup()
{
    ofeliaRotationAxes_class = class_new(gensym("ofRotationAxes"),
                                         reinterpret_cast<t_newmethod>(ofeliaRotationAxes_new),
                                         0, sizeof(t_ofeliaRotationAxes),
                                         CLASS_DEFAULT, A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaRotationAxes_new),
                     gensym("ofRotAxes"), A_GIMME, 0);
    class_addbang(ofeliaRotationAxes_class, reinterpret_cast<t_method>(ofeliaRotationAxes_bang));
    class_addmethod(ofeliaRotationAxes_class, reinterpret_cast<t_method>(ofeliaRotationAxes_radius),
                    gensym("radius"), A_FLOAT, 0);
    class_addmethod(ofeliaRotationAxes_class, reinterpret_cast<t_method>(ofeliaRotationAxes_stripWidth),
                    gensym("stripWidth"), A_FLOAT, 0);
    class_addmethod(ofeliaRotationAxes_class, reinterpret_cast<t_method>(ofeliaRotationAxes_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaRotationAxes_class, reinterpret_cast<t_method>(ofeliaRotationAxes_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * setup methods
 */
void ofeliaShapes_setup()
{
    ofeliaBeginShape_setup();
    ofeliaEndShape_setup();
    ofeliaNextContour_setup();
    ofeliaVertex2d_setup();
    ofeliaVertex3d_setup();
    ofeliaCurveVertex2d_setup();
    ofeliaCurveVertex3d_setup();
    ofeliaBezierVertex2d_setup();
    ofeliaBezierVertex3d_setup();
    ofeliaCircle_setup();
    ofeliaEllipse_setup();
    ofeliaArc_setup();
    ofeliaSector_setup();
    ofeliaLine2d_setup();
    ofeliaLine3d_setup();
    ofeliaCurve2d_setup();
    ofeliaCurve3d_setup();
    ofeliaBezier2d_setup();
    ofeliaBezier3d_setup();
    ofeliaQuadBezier2d_setup();
    ofeliaQuadBezier3d_setup();
    ofeliaTriangle2d_setup();
    ofeliaTriangle3d_setup();
    ofeliaEqTriangle_setup();
    ofeliaIsoTriangle_setup();
    ofeliaQuad2d_setup();
    ofeliaQuad3d_setup();
    ofeliaSquare_setup();
    ofeliaRectangle_setup();
    ofeliaRectRounded_setup();
    ofeliaRectRounded4_setup();
    ofeliaCross_setup();
    ofeliaHeart_setup();
    ofeliaMoon_setup();
    ofeliaRegPolygon_setup();
    ofeliaStar_setup();
    ofeliaAxis_setup();
    ofeliaBox_setup();
    ofeliaCone_setup();
    ofeliaCylinder_setup();
    ofeliaIcosphere_setup();
    ofeliaPlane_setup();
    ofeliaSphere_setup();
    ofeliaArrow_setup();
    ofeliaGrid_setup();
    ofeliaGridPlane_setup();
    ofeliaRotationAxes_setup();
}

