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

#ifndef ofeliaShapes_h
#define ofeliaShapes_h

#pragma once

#include "ofeliaBase.h"

extern "C" {
    
    /* common structs */
    typedef struct _ofeliaShapesDefault {
        
        static const float curveControlY;
        static const float cornerRadius;
        static const float crossThickness;
        static const float moonPhaseRadius;
        static const float arrowHeadSize;
        
    } t_ofeliaShapesDefault;
    
    typedef struct _ofeliaShapesDimen {
        
        float width;
        float height;
        
    } t_ofeliaShapesDimen;
    
    typedef struct _ofeliaArcElem {
        
        float radiusX;
        float radiusY;
        float angleBegin;
        float angleEnd;
        
    } t_ofeliaArcElem;
    
    /* ofBeginShape object class */
    static t_class *ofeliaBeginShape_class;
    
    typedef struct _ofeliaBeginShape
    {
        t_object x_obj;
        static const char *objName;
        
    } t_ofeliaBeginShape;
    
    /* ofEndShape object class */
    static t_class *ofeliaEndShape_class;
    
    typedef struct _ofeliaEndShape
    {
        t_object x_obj;
        static const char *objName;
        bool autoClose;
        
    } t_ofeliaEndShape;
    
    /* ofNextContour object class */
    static t_class *ofeliaNextContour_class;
    
    typedef struct _ofeliaNextContour
    {
        t_object x_obj;
        static const char *objName;
        bool autoClose;
        
    } t_ofeliaNextContour;
    
    /* ofVertex2d object class */
    static t_class *ofeliaVertex2d_class;
    
    typedef struct _ofeliaVertex2d
    {
        t_object x_obj;
        static const char *objName;
        ofVec2f vertex;
        
    } t_ofeliaVertex2d;
    
    /* ofVertex3d object class */
    static t_class *ofeliaVertex3d_class;
    
    typedef struct _ofeliaVertex3d
    {
        t_object x_obj;
        static const char *objName;
        ofVec3f vertex;
        
    } t_ofeliaVertex3d;
    
    /* ofCurveVertex2d object class */
    static t_class *ofeliaCurveVertex2d_class;
    
    typedef struct _ofeliaCurveVertex2d
    {
        t_object x_obj;
        static const char *objName;
        ofVec2f vertex;
        
    } t_ofeliaCurveVertex2d;
    
    /* ofCurveVertex3d object class */
    static t_class *ofeliaCurveVertex3d_class;
    
    typedef struct _ofeliaCurveVertex3d
    {
        t_object x_obj;
        static const char *objName;
        ofVec3f vertex;
        
    } t_ofeliaCurveVertex3d;
    
    /* ofBezierVertex2d object class */
    static t_class *ofeliaBezierVertex2d_class;
    
    typedef struct _ofeliaBezierVertex2dPoints {
        
        ofVec2f p1, p2, p3;
        
    } t_ofeliaBezierVertex2dPoints;
    
    typedef struct _ofeliaBezierVertex2d
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaBezierVertex2dPoints points;
        
    } t_ofeliaBezierVertex2d;
    
    /* ofBezierVertex3d object class */
    static t_class *ofeliaBezierVertex3d_class;
    
    typedef struct _ofeliaBezierVertex3dPoints {
        
        ofVec3f p1, p2, p3;
        
    } t_ofeliaBezierVertex3dPoints;
    
    typedef struct _ofeliaBezierVertex3d
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaBezierVertex3dPoints points;
        
    } t_ofeliaBezierVertex3d;
    
    /* ofCircle object class */
    static t_class *ofeliaCircle_class;
    
    typedef struct _ofeliaCircle
    {
        t_object x_obj;
        static const char *objName;
        float radius;
        
    } t_ofeliaCircle;
    
    /* ofEllipse object class */
    static t_class *ofeliaEllipse_class;
    
    typedef struct _ofeliaEllipse
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaShapesDimen dimen;
        
    } t_ofeliaEllipse;
    
    /* ofArc object class */
    static t_class *ofeliaArc_class;
    
    typedef struct _ofeliaArc
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaArcElem elem;
        unique_ptr<ofPath> path;
        unique_ptr<ofVboMesh> mesh;
        unique_ptr<ofPolyline> polyline;
        int circleResolution;
        bool bFill;
        bool shouldLoad;
        bool bInitGate;
        
    } t_ofeliaArc;
    
    /* ofSector object class */
    static t_class *ofeliaSector_class;
    
    typedef struct _ofeliaSector
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaArcElem elem;
        unique_ptr<ofPath> path;
        unique_ptr<ofVboMesh> mesh;
        unique_ptr<ofPolyline> polyline;
        int circleResolution;
        bool bFill;
        bool shouldLoad;
        bool bInitGate;
        
    } t_ofeliaSector;
    
    /* ofLine2d object class */
    static t_class *ofeliaLine2d_class;
    
    typedef struct _ofeliaLine2dPoints {
        
        ofVec2f p1, p2;
        
    } t_ofeliaLine2dPoints;
    
    typedef struct _ofeliaLine2d
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaLine2dPoints points;
        
    } t_ofeliaLine2d;
    
    /* ofLine3d object class */
    static t_class *ofeliaLine3d_class;
    
    typedef struct _ofeliaLine3dPoints {
        
        ofVec3f p1, p2;
        
    } t_ofeliaLine3dPoints;
    
    typedef struct _ofeliaLine3d
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaLine3dPoints points;
        
    } t_ofeliaLine3d;
    
    /* ofCurve2d object class */
    static t_class *ofeliaCurve2d_class;
    
    typedef struct _ofeliaCurve2dPoints {
        
        ofVec2f p0, p1, p2, p3;
        
    } t_ofeliaCurve2dPoints;
    
    typedef struct _ofeliaCurve2d
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaCurve2dPoints points;
        unique_ptr<ofPath> path;
        unique_ptr<ofVboMesh> mesh;
        int curveResolution;
        bool bFill;
        bool shouldLoad;
        bool bInitGate;
        
    } t_ofeliaCurve2d;
    
    /* ofCurve3d object class */
    static t_class *ofeliaCurve3d_class;
    
    typedef struct _ofeliaCurve3dPoints {
        
        ofVec3f p0, p1, p2, p3;
        
    } t_ofeliaCurve3dPoints;
    
    typedef struct _ofeliaCurve3d
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaCurve3dPoints points;
        unique_ptr<ofPath> path;
        unique_ptr<ofVboMesh> mesh;
        int curveResolution;
        bool bFill;
        bool shouldLoad;
        bool bInitGate;
        
    } t_ofeliaCurve3d;
    
    /* ofBezier2d object class */
    static t_class *ofeliaBezier2d_class;
    
    typedef struct _ofeliaBezier2dPoints {
        
        ofVec2f p0, p1, p2, p3;
        
    } t_ofeliaBezier2dPoints;
    
    typedef struct _ofeliaBezier2d
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaBezier2dPoints points;
        unique_ptr<ofPath> path;
        unique_ptr<ofVboMesh> mesh;
        int curveResolution;
        bool bFill;
        bool shouldLoad;
        bool bInitGate;
        
    } t_ofeliaBezier2d;
    
    /* ofBezier3d object class */
    static t_class *ofeliaBezier3d_class;
    
    typedef struct _ofeliaBezier3dPoints {
        
        ofVec3f p0, p1, p2, p3;
        
    } t_ofeliaBezier3dPoints;
    
    typedef struct _ofeliaBezier3d
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaBezier3dPoints points;
        unique_ptr<ofPath> path;
        unique_ptr<ofVboMesh> mesh;
        int curveResolution;
        bool bFill;
        bool shouldLoad;
        bool bInitGate;
        
    } t_ofeliaBezier3d;
    
    /* ofQuadBezier2d object class */
    static t_class *ofeliaQuadBezier2d_class;
    
    typedef struct _ofeliaQuadBezier2d
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaBezier2dPoints points;
        unique_ptr<ofPath> path;
        unique_ptr<ofVboMesh> mesh;
        int curveResolution;
        bool bFill;
        bool shouldLoad;
        bool bInitGate;
        
    } t_ofeliaQuadBezier2d;
    
    /* ofQuadBezier3d object class */
    static t_class *ofeliaQuadBezier3d_class;
    
    typedef struct _ofeliaQuadBezier3d
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaBezier3dPoints points;
        unique_ptr<ofPath> path;
        unique_ptr<ofVboMesh> mesh;
        int curveResolution;
        bool bFill;
        bool shouldLoad;
        bool bInitGate;
        
    } t_ofeliaQuadBezier3d;
    
    /* ofTriangle2d object class */
    static t_class *ofeliaTriangle2d_class;
    
    typedef struct _ofeliaTri2dPoints {
        
        ofVec2f p1, p2, p3;
        
    } t_ofeliaTri2dPoints;
    
    typedef struct _ofeliaTriangle2d
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaTri2dPoints points;
        
    } t_ofeliaTriangle2d;
    
    /* ofTriangle3d object class */
    static t_class *ofeliaTriangle3d_class;
    
    typedef struct _ofeliaTri3dPoints {
        
        ofVec3f p1, p2, p3;
        
    } t_ofeliaTri3dPoints;
    
    typedef struct _ofeliaTriangle3d
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaTri3dPoints points;
        
    } t_ofeliaTriangle3d;
    
    /* ofEqTriangle object class */
    static t_class *ofeliaEqTriangle_class;
    
    typedef struct _ofeliaEqTriangle
    {
        t_object x_obj;
        static const char *objName;
        float size;
        
    } t_ofeliaEqTriangle;
    
    /* ofIsoTriangle object class */
    static t_class *ofeliaIsoTriangle_class;
    
    typedef struct _ofeliaIsoTriangle
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaShapesDimen dimen;
        
    } t_ofeliaIsoTriangle;
    
    /* ofQuad2d object class */
    static t_class *ofeliaQuad2d_class;
    
    typedef struct _ofeliaQuad2d
    {
        t_object x_obj;
        static const char *objName;
        ofVec2f points[4];
        
    } t_ofeliaQuad2d;
    
    /* ofQuad3d object class */
    static t_class *ofeliaQuad3d_class;
    
    typedef struct _ofeliaQuad3d
    {
        t_object x_obj;
        static const char *objName;
        ofVec3f points[4];
        
    } t_ofeliaQuad3d;

    /* ofSquare object class */
    static t_class *ofeliaSquare_class;
    
    typedef struct _ofeliaSquare
    {
        t_object x_obj;
        static const char *objName;
        float size;
        
    } t_ofeliaSquare;
    
    /* ofRectangle object class */
    static t_class *ofeliaRectangle_class;
    
    typedef struct _ofeliaRectangle
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaShapesDimen dimen;
        
    } t_ofeliaRectangle;
    
    /* ofRectRounded object class */
    static t_class *ofeliaRectRounded_class;
    
    typedef struct _ofeliaShapesDimenRad {
        
        float width;
        float height;
        float radius;
        
    } t_ofeliaShapesDimenRad;
    
    typedef struct _ofeliaRectRounded
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaShapesDimenRad dimenRad;
        unique_ptr<ofPath> path;
        unique_ptr<ofVboMesh> mesh;
        ofRectMode rectMode;
        int circleResolution;
        bool bFill;
        bool shouldLoad;
        bool bInitGate;
        
    } t_ofeliaRectRounded;
    
    /* ofRectRounded4 object class */
    static t_class *ofeliaRectRounded4_class;
    
    typedef struct _ofeliaShapesDimenRad4 {
        
        float width;
        float height;
        float radius[4];
        
    } t_ofeliaShapesDimenRad4;
    
    typedef struct _ofeliaRectRounded4
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaShapesDimenRad4 dimenRad;
        unique_ptr<ofPath> path;
        unique_ptr<ofVboMesh> mesh;
        ofRectMode rectMode;
        int circleResolution;
        bool bFill;
        bool shouldLoad;
        bool bInitGate;
        
    } t_ofeliaRectRounded4;
    
    /* ofCross object class */
    static t_class *ofeliaCross_class;
    
    typedef struct _ofeliaCrossElem {
        
        float size;
        float thickness;
        
    } t_ofeliaCrossElem;
    
    typedef struct _ofeliaCross
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaCrossElem elem;
        unique_ptr<ofPath> path;
        unique_ptr<ofVboMesh> mesh;
        unique_ptr<vector<ofPolyline>> polylines;
        bool bFill;
        bool shouldLoad;
        bool bInitGate;
        
    } t_ofeliaCross;
    
    /* ofHeart object class */
    static t_class *ofeliaHeart_class;
    
    typedef struct _ofeliaHeart
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaShapesDimen dimen;
        unique_ptr<ofPath> path;
        unique_ptr<ofVboMesh> mesh;
        unique_ptr<vector<ofPolyline>> polylines;
        int curveResolution;
        bool bFill;
        bool shouldLoad;
        bool bInitGate;
        
    } t_ofeliaHeart;
    
    /* ofMoon object class */
    static t_class *ofeliaMoon_class;
    
    typedef struct _ofeliaMoonElem {
        
        float width;
        float height;
        float phase;
        
    } t_ofeliaMoonElem;
    
    typedef struct _ofeliaMoon
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaMoonElem elem;
        unique_ptr<ofPath> path;
        unique_ptr<ofVboMesh> mesh;
        unique_ptr<ofPolyline> polyline;
        int circleResolution;
        bool bFill;
        bool shouldLoad;
        bool bInitGate;
        
    } t_ofeliaMoon;
    
    /* ofRegPolygon object class */
    static t_class *ofeliaRegPolygon_class;
    
    typedef struct _ofeliaRegPolygonElem {
        
        float radius;
        int numSides;
        
    } t_ofeliaRegPolygonElem;
    
    typedef struct _ofeliaRegPolygon
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaRegPolygonElem elem;
        vector<ofVec2f> points;
        bool shouldLoad;
        
    } t_ofeliaRegPolygon;
    
    /* ofStar object class */
    static t_class *ofeliaStar_class;
    
    typedef struct _ofeliaStarElem {
        
        float radius[2];
        int numPoints;
        
    } t_ofeliaStarElem;
    
    typedef struct _ofeliaStar
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaStarElem elem;
        unique_ptr<ofPath> path;
        unique_ptr<ofVboMesh> mesh;
        unique_ptr<vector<ofPolyline>> polylines;
        bool bFill;
        bool shouldLoad;
        bool bInitGate;
        
    } t_ofeliaStar;
    
    /* ofAxis object class */
    static t_class *ofeliaAxis_class;
    
    typedef struct _ofeliaAxisElem {
        
        float size;
        
    } t_ofeliaAxisElem;
    
    typedef struct _ofeliaAxis
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaAxisElem elem;
        unique_ptr<ofVboMesh> mesh;
        bool shouldLoad;
        bool bInitGate;
        
    } t_ofeliaAxis;
    
    /* ofBox object class */
    static t_class *ofeliaBox_class;
    
    typedef struct _ofeliaBoxElem {
        
        float width;
        float height;
        float depth;
        int resX, resY, resZ;
        
    } t_ofeliaBoxElem;
    
    typedef struct _ofeliaBox
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaBoxElem elem;
        unique_ptr<ofVboMesh> mesh;
        bool shouldLoad;
        bool bInitGate;
        
    } t_ofeliaBox;
    
    /* ofCone object class */
    static t_class *ofeliaCone_class;
    
    typedef struct _ofeliaConeElem {
        
        float radius;
        float height;
        int radiusSegments;
        int heightSegments;
        int capSegments;
        
    } t_ofeliaConeElem;
    
    typedef struct _ofeliaCone
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaConeElem elem;
        unique_ptr<ofVboMesh> mesh;
        bool shouldLoad;
        bool bInitGate;
        
    } t_ofeliaCone;
    
    /* ofCylinder object class */
    static t_class *ofeliaCylinder_class;
    
    typedef struct _ofeliaCylinderElem {
        
        float radius;
        float height;
        int radiusSegments;
        int heightSegments;
        int capSegments;
        
    } t_ofeliaCylinderElem;
    
    typedef struct _ofeliaCylinder
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaCylinderElem elem;
        unique_ptr<ofVboMesh> mesh;
        bool shouldLoad;
        bool bInitGate;
        
    } t_ofeliaCylinder;
    
    /* ofIcosphere object class */
    static t_class *ofeliaIcosphere_class;
    
    typedef struct _ofeliaIcosphereElem {
        
        float radius;
        size_t iterations;
        
    } t_ofeliaIcosphereElem;
    
    typedef struct _ofeliaIcosphere
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaIcosphereElem elem;
        unique_ptr<ofVboMesh> mesh;
        bool shouldLoad;
        bool bInitGate;
        
    } t_ofeliaIcosphere;
    
    /* ofPlane object class */
    static t_class *ofeliaPlane_class;
    
    typedef struct _ofeliaPlaneElem {
        
        float width;
        float height;
        int columns;
        int rows;
        
    } t_ofeliaPlaneElem;
    
    typedef struct _ofeliaPlane
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaPlaneElem elem;
        unique_ptr<ofVboMesh> mesh;
        bool shouldLoad;
        bool bInitGate;
        
    } t_ofeliaPlane;
    
    /* ofSphere object class */
    static t_class *ofeliaSphere_class;
    
    typedef struct _ofeliaSphereElem {
        
        float radius;
        int sphereRes;
        
    } t_ofeliaSphereElem;
    
    typedef struct _ofeliaSphere
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaSphereElem elem;
        unique_ptr<ofVboMesh> mesh;
        bool shouldLoad;
        bool bInitGate;
        
    } t_ofeliaSphere;
    
    /* ofArrow object class */
    static t_class *ofeliaArrow_class;
    
    typedef struct _ofeliaArrowElem {
        
        ofVec3f start;
        ofVec3f end;
        float headSize;
        
    } t_ofeliaArrowElem;
    
    typedef struct _ofeliaArrow
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaArrowElem elem;
        
    } t_ofeliaArrow;
    
    /* ofGrid object class */
    static t_class *ofeliaGrid_class;
    
    typedef struct _ofeliaGridElem {
        
        float stepSize;
        int numberOfSteps;
        bool visX, visY, visZ;
        
    } t_ofeliaGridElem;
    
    typedef struct _ofeliaGrid
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaGridElem elem;
        
    } t_ofeliaGrid;
    
    /* ofGridPlane object class */
    static t_class *ofeliaGridPlane_class;
    
    typedef struct _ofeliaGridPlaneElem {
        
        float stepSize;
        int numberOfSteps;
        
    } t_ofeliaGridPlaneElem;
    
    typedef struct _ofeliaGridPlane
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaGridPlaneElem elem;
        
    } t_ofeliaGridPlane;
    
    /* ofRotationAxes object class */
    static t_class *ofeliaRotationAxes_class;
    
    typedef struct _ofeliaRotationAxesElem {
        
        float radius;
        float stripWidth;
        
    } t_ofeliaRotationAxesElem;
    
    typedef struct _ofeliaRotationAxes
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaRotationAxesElem elem;
        int circleResolution;
        
    } t_ofeliaRotationAxes;
    
    /* setup methods */
    void ofeliaShapes_setup();
}

#endif /* ofeliaShapes_h */
