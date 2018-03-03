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

#ifndef ofeliaPolyline_h
#define ofeliaPolyline_h

#include "ofeliaBase.h"

extern "C" {
    
    /* enumerations */
    enum LoadPolylineCmdType {
        
        POLYLINE_LOAD_CMDTYPE_UNKNOWN,
        POLYLINE_LOAD_CMDTYPE_NEWSUBPATH,
        POLYLINE_LOAD_CMDTYPE_VERTEX,
        POLYLINE_LOAD_CMDTYPE_MOVETO,
        POLYLINE_LOAD_CMDTYPE_LINETO,
        POLYLINE_LOAD_CMDTYPE_ARC,
        POLYLINE_LOAD_CMDTYPE_CURVETO,
        POLYLINE_LOAD_CMDTYPE_BEZIERTO,
        POLYLINE_LOAD_CMDTYPE_QUADBEZIERTO,
        POLYLINE_LOAD_CMDTYPE_CLOSE,
        POLYLINE_LOAD_CMDTYPE_CIRCLE,
        POLYLINE_LOAD_CMDTYPE_ELLIPSE,
        POLYLINE_LOAD_CMDTYPE_SECTOR,
        POLYLINE_LOAD_CMDTYPE_TRIANGLE,
        POLYLINE_LOAD_CMDTYPE_EQTRIANGLE,
        POLYLINE_LOAD_CMDTYPE_ISOTRIANGLE,
        POLYLINE_LOAD_CMDTYPE_QUAD,
        POLYLINE_LOAD_CMDTYPE_SQUARE,
        POLYLINE_LOAD_CMDTYPE_RECTANGLE,
        POLYLINE_LOAD_CMDTYPE_RECTROUNDED,
        POLYLINE_LOAD_CMDTYPE_RECTROUNDED4,
        POLYLINE_LOAD_CMDTYPE_CROSS,
        POLYLINE_LOAD_CMDTYPE_HEART,
        POLYLINE_LOAD_CMDTYPE_MOON,
        POLYLINE_LOAD_CMDTYPE_REGPOLYGON,
        POLYLINE_LOAD_CMDTYPE_STAR
    };
    enum LoadPolylineCmdState {
        
        POLYLINE_LOAD_CMD_INSERT,
        POLYLINE_LOAD_CMD_ERASE,
        POLYLINE_LOAD_CMD_FILL
    };
    /* common structs */
    typedef struct _ofeliaLoadPolylineSet {
        
        unique_ptr<ofPath> path;
        unique_ptr<vector<ofPolyline>> polylines;
        
    } t_ofeliaLoadPolylineSet;
    
    /* ofLoadPolyline2d object class */
    static t_class *ofeliaLoadPolyline2d_class;
    
    typedef struct _ofeliaLoadPolyline2dElem {
        
        LoadPolylineCmdType type;
        int numArgs;
        float args[8];
        
    } t_ofeliaLoadPolyline2dElem;
    
    typedef struct _ofeliaLoadPolyline2dData {
        
        unsigned int objID;
        t_ofeliaVarName varName;
        vector<t_ofeliaLoadPolyline2dElem> elems;
        vector<pair<size_t, size_t>> pointIndices;
        ofVec2f boxDimen, centroid;
        float area, perimeter;
        bool shouldUpdateProperties;
        
    } t_ofeliaLoadPolyline2dData;
    
    typedef struct _ofeliaLoadPolyline2dCmdData {
        
        LoadPolylineCmdState state;
        t_ofeliaLoadPolyline2dElem elem;
        t_float fromIndex, toIndex;
        
    } t_ofeliaLoadPolyline2dCmdData;
    
    typedef struct _ofeliaLoadPolyline2d
    {
        t_object x_obj;
        static const char *objName;
        static unsigned int counter;
        static vector<t_ofeliaLoadPolyline2dData> polylineData;
        static vector<t_ofeliaLoadPolylineSet> polylineSets;
        vector<t_ofeliaLoadPolyline2dCmdData> cmdVec;
        ofRectMode rectMode;
        int circleRes;
        int curveRes;
        float simplify;
        static bool bInited;
        bool bInitGate;
        unsigned int objID;
        bool shouldOutlet;
        int vecSize, numPoints;
        t_outlet *vecSize_out, *numPoints_out;
        
    } t_ofeliaLoadPolyline2d;
    
    /* ofLoadPolyline3d object class */
    static t_class *ofeliaLoadPolyline3d_class;
    
    typedef struct _ofeliaLoadPolyline3dElem {
        
        LoadPolylineCmdType type;
        int numArgs;
        float args[12];
        
    } t_ofeliaLoadPolyline3dElem;
    
    typedef struct _ofeliaLoadPolyline3dData {
        
        unsigned int objID;
        t_ofeliaVarName varName;
        vector<t_ofeliaLoadPolyline3dElem> elems;
        vector<pair<size_t, size_t>> pointIndices;
        ofVec3f boxDimen, centroid;
        float area, perimeter;
        bool shouldUpdateProperties;
        
    } t_ofeliaLoadPolyline3dData;
    
    typedef struct _ofeliaLoadPolyline3dCmdData {
        
        LoadPolylineCmdState state;
        t_ofeliaLoadPolyline3dElem elem;
        t_float fromIndex, toIndex;
        
    } t_ofeliaLoadPolyline3dCmdData;
    
    typedef struct _ofeliaLoadPolyline3d
    {
        t_object x_obj;
        static const char *objName;
        static unsigned int counter;
        static vector<t_ofeliaLoadPolyline3dData> polylineData;
        static vector<t_ofeliaLoadPolylineSet> polylineSets;
        vector<t_ofeliaLoadPolyline3dCmdData> cmdVec;
        ofRectMode rectMode;
        int circleRes;
        int curveRes;
        float simplify;
        static bool bInited;
        bool bInitGate;
        unsigned int objID;
        bool shouldOutlet;
        int vecSize, numPoints;
        t_outlet *vecSize_out, *numPoints_out;
        
    } t_ofeliaLoadPolyline3d;
    
    /* drawPolyline2d object class */
    static t_class *ofeliaDrawPolyline2d_class;
    
    typedef struct _ofeliaDrawPolyline2d
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaNameDimen2dElem elem;
        
    } t_ofeliaDrawPolyline2d;
    
    /* ofDrawPolyline3d object class */
    static t_class *ofeliaDrawPolyline3d_class;
    
    typedef struct _ofeliaDrawPolyline3d
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaNameDimen3dElem elem;
        
    } t_ofeliaDrawPolyline3d;
    
    /* ofDoesPolyline2dNameExist object class */
    static t_class *ofeliaDoesPolyline2dNameExist_class;
    
    typedef struct _ofeliaDoesPolyline2dNameExist
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaDoesPolyline2dNameExist;
    
    /* ofDoesPolyline3dNameExist object class */
    static t_class *ofeliaDoesPolyline3dNameExist_class;
    
    typedef struct _ofeliaDoesPolyline3dNameExist
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaDoesPolyline3dNameExist;
    
    /* ofEditPolyline2dPoint object class */
    static t_class *ofeliaEditPolyline2dPoint_class;
    
    typedef struct _ofeliaEditPolyline2dPoint
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaEditPolyline2dPoint;
    
    /* ofEditPolyline3dPoint object class */
    static t_class *ofeliaEditPolyline3dPoint_class;
    
    typedef struct _ofeliaEditPolyline3dPoint
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaEditPolyline3dPoint;
    
    /* ofGetPolyline2dPoint object class */
    static t_class *ofeliaGetPolyline2dPoint_class;
    
    typedef struct _ofeliaGetPolyline2dPoint
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPolyline2dPoint;
    
    /* ofGetPolyline3dPoint object class */
    static t_class *ofeliaGetPolyline3dPoint_class;
    
    typedef struct _ofeliaGetPolyline3dPoint
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPolyline3dPoint;
    
    /* ofGetPolyline2dPoints object class */
    static t_class *ofeliaGetPolyline2dPoints_class;
    
    typedef struct _ofeliaGetPolyline2dPoints
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPolyline2dPoints;
    
    /* ofGetPolyline3dPoints object class */
    static t_class *ofeliaGetPolyline3dPoints_class;
    
    typedef struct _ofeliaGetPolyline3dPoints
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPolyline3dPoints;
    
    /* ofIsPointInsidePolyline2d object class */
    static t_class *ofeliaIsPointInsidePolyline2d_class;
    
    typedef struct _ofeliaIsPointInsidePolyline2d
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaIsPointInsidePolyline2d;
    
    /* ofIsPointInsidePolyline3d object class */
    static t_class *ofeliaIsPointInsidePolyline3d_class;
    
    typedef struct _ofeliaIsPointInsidePolyline3d
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaIsPointInsidePolyline3d;
    
    /* ofGetPolyline2dCommand object class */
    static t_class *ofeliaGetPolyline2dCommand_class;
    
    typedef struct _ofeliaGetPolyline2dCommand
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPolyline2dCommand;
    
    /* ofGetPolyline3dCommand object class */
    static t_class *ofeliaGetPolyline3dCommand_class;
    
    typedef struct _ofeliaGetPolyline3dCommand
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPolyline3dCommand;
    
    /* ofGetPolyline2dCommands object class */
    static t_class *ofeliaGetPolyline2dCommands_class;
    
    typedef struct _ofeliaGetPolyline2dCommands
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPolyline2dCommands;
    
    /* ofGetPolyline3dCommands object class */
    static t_class *ofeliaGetPolyline3dCommands_class;
    
    typedef struct _ofeliaGetPolyline3dCommands
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPolyline3dCommands;
    
    /* ofGetPolyline2dBoundingBox object class */
    static t_class *ofeliaGetPolyline2dBoundingBox_class;
    
    typedef struct _ofeliaGetPolyline2dBoundingBox
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPolyline2dBoundingBox;
    
    /* ofGetPolyline3dBoundingBox object class */
    static t_class *ofeliaGetPolyline3dBoundingBox_class;
    
    typedef struct _ofeliaGetPolyline3dBoundingBox
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPolyline3dBoundingBox;
    
    /* ofGetPolyline2dCentroid object class */
    static t_class *ofeliaGetPolyline2dCentroid_class;
    
    typedef struct _ofeliaGetPolyline2dCentroid
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPolyline2dCentroid;
    
    /* ofGetPolyline3dCentroid object class */
    static t_class *ofeliaGetPolyline3dCentroid_class;
    
    typedef struct _ofeliaGetPolyline3dCentroid
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPolyline3dCentroid;
    
    /* ofGetPolyline2dArea object class */
    static t_class *ofeliaGetPolyline2dArea_class;
    
    typedef struct _ofeliaGetPolyline2dArea
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPolyline2dArea;
    
    /* ofGetPolyline3dArea object class */
    static t_class *ofeliaGetPolyline3dArea_class;
    
    typedef struct _ofeliaGetPolyline3dArea
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPolyline3dArea;
    
    /* ofGetPolyline2dPerimeter object class */
    static t_class *ofeliaGetPolyline2dPerimeter_class;
    
    typedef struct _ofeliaGetPolyline2dPerimeter
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPolyline2dPerimeter;
    
    /* ofGetPolyline3dPerimeter object class */
    static t_class *ofeliaGetPolyline3dPerimeter_class;
    
    typedef struct _ofeliaGetPolyline3dPerimeter
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPolyline3dPerimeter;
    
    /* setup method */
    void ofeliaPolyline_setup();
}

#endif /* ofeliaPolyline_h */
