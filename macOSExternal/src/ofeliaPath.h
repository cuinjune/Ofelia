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

#ifndef ofeliaPath_h
#define ofeliaPath_h

#include "ofeliaBase.h"

extern "C" {
    
    /* enumerations */
    enum LoadPathCmdType {
        
        PATH_LOAD_CMDTYPE_UNKNOWN,
        PATH_LOAD_CMDTYPE_NEWSUBPATH,
        PATH_LOAD_CMDTYPE_VERTEX,
        PATH_LOAD_CMDTYPE_MOVETO,
        PATH_LOAD_CMDTYPE_LINETO,
        PATH_LOAD_CMDTYPE_ARC,
        PATH_LOAD_CMDTYPE_CURVETO,
        PATH_LOAD_CMDTYPE_BEZIERTO,
        PATH_LOAD_CMDTYPE_QUADBEZIERTO,
        PATH_LOAD_CMDTYPE_CLOSE,
        PATH_LOAD_CMDTYPE_CIRCLE,
        PATH_LOAD_CMDTYPE_ELLIPSE,
        PATH_LOAD_CMDTYPE_SECTOR,
        PATH_LOAD_CMDTYPE_TRIANGLE,
        PATH_LOAD_CMDTYPE_EQTRIANGLE,
        PATH_LOAD_CMDTYPE_ISOTRIANGLE,
        PATH_LOAD_CMDTYPE_QUAD,
        PATH_LOAD_CMDTYPE_SQUARE,
        PATH_LOAD_CMDTYPE_RECTANGLE,
        PATH_LOAD_CMDTYPE_RECTROUNDED,
        PATH_LOAD_CMDTYPE_RECTROUNDED4,
        PATH_LOAD_CMDTYPE_CROSS,
        PATH_LOAD_CMDTYPE_HEART,
        PATH_LOAD_CMDTYPE_MOON,
        PATH_LOAD_CMDTYPE_REGPOLYGON,
        PATH_LOAD_CMDTYPE_STAR
    };
    enum LoadPathCmdState {
        
        PATH_LOAD_CMD_INSERT,
        PATH_LOAD_CMD_ERASE,
        PATH_LOAD_CMD_FILL
    };
    /* common structs */
    typedef struct _ofeliaLoadPathSet {
        
        unique_ptr<ofPath> path;
        unique_ptr<ofVboMesh> mesh;
        unique_ptr<vector<ofPolyline>> polylines;
 
    } t_ofeliaLoadPathSet;
    
    /* ofLoadPath2d object class */
    static t_class *ofeliaLoadPath2d_class;
    
    typedef struct _ofeliaLoadPath2dElem {
        
        LoadPathCmdType type;
        int numArgs;
        float args[8];
        
    } t_ofeliaLoadPath2dElem;
    
    typedef struct _ofeliaLoadPath2dData {
        
        unsigned int objID;
        t_ofeliaVarName varName;
        vector<t_ofeliaLoadPath2dElem> elems;
        vector<pair<size_t, size_t>> pointIndices;
        ofVec2f boxDimen, centroid;
        float area, perimeter;
        bool shouldUpdateProperties;
        
    } t_ofeliaLoadPath2dData;
    
    typedef struct _ofeliaLoadPath2dCmdData {
        
        LoadPathCmdState state;
        t_ofeliaLoadPath2dElem elem;
        t_float fromIndex, toIndex;
        
    } t_ofeliaLoadPath2dCmdData;
    
    typedef struct _ofeliaLoadPath2d
    {
        t_object x_obj;
        static const char *objName;
        static unsigned int counter;
        static vector<t_ofeliaLoadPath2dData> pathData;
        static vector<t_ofeliaLoadPathSet> pathSets;
        vector<t_ofeliaLoadPath2dCmdData> cmdVec;
        ofRectMode rectMode;
        ofPolyWindingMode polyMode;
        int circleRes;
        int curveRes;
        float simplify;
        static bool bInited;
        bool bInitGate;
        unsigned int objID;
        bool shouldOutlet;
        int vecSize, numPoints;
        t_outlet *vecSize_out, *numPoints_out;
        
    } t_ofeliaLoadPath2d;
    
    /* ofLoadPath3d object class */
    static t_class *ofeliaLoadPath3d_class;
    
    typedef struct _ofeliaLoadPath3dElem {
        
        LoadPathCmdType type;
        int numArgs;
        float args[12];
        
    } t_ofeliaLoadPath3dElem;
    
    typedef struct _ofeliaLoadPath3dData {
        
        unsigned int objID;
        t_ofeliaVarName varName;
        vector<t_ofeliaLoadPath3dElem> elems;
        vector<pair<size_t, size_t>> pointIndices;
        ofVec3f boxDimen, centroid;
        float area, perimeter;
        bool shouldUpdateProperties;
        
    } t_ofeliaLoadPath3dData;
    
    typedef struct _ofeliaLoadPath3dCmdData {
        
        LoadPathCmdState state;
        t_ofeliaLoadPath3dElem elem;
        t_float fromIndex, toIndex;
        
    } t_ofeliaLoadPath3dCmdData;
    
    typedef struct _ofeliaLoadPath3d
    {
        t_object x_obj;
        static const char *objName;
        static unsigned int counter;
        static vector<t_ofeliaLoadPath3dData> pathData;
        static vector<t_ofeliaLoadPathSet> pathSets;
        vector<t_ofeliaLoadPath3dCmdData> cmdVec;
        ofRectMode rectMode;
        ofPolyWindingMode polyMode;
        int circleRes;
        int curveRes;
        float simplify;
        static bool bInited;
        bool bInitGate;
        unsigned int objID;
        bool shouldOutlet;
        int vecSize, numPoints;
        t_outlet *vecSize_out, *numPoints_out;
        
    } t_ofeliaLoadPath3d;
    
    /* ofDrawPath2d object class */
    static t_class *ofeliaDrawPath2d_class;
    
    typedef struct _ofeliaDrawPath2d
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaNameDimen2dElem elem;
        
    } t_ofeliaDrawPath2d;
    
    /* ofDrawPath3d object class */
    static t_class *ofeliaDrawPath3d_class;
    
    typedef struct _ofeliaDrawPath3d
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaNameDimen3dElem elem;
        
    } t_ofeliaDrawPath3d;
    
    /* ofDoesPath2dNameExist object class */
    static t_class *ofeliaDoesPath2dNameExist_class;
    
    typedef struct _ofeliaDoesPath2dNameExist
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaDoesPath2dNameExist;
    
    /* ofDoesPath3dNameExist object class */
    static t_class *ofeliaDoesPath3dNameExist_class;
    
    typedef struct _ofeliaDoesPath3dNameExist
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaDoesPath3dNameExist;
    
    /* ofGetPath2dPoint object class */
    static t_class *ofeliaGetPath2dPoint_class;
    
    typedef struct _ofeliaGetPath2dPoint
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPath2dPoint;
    
    /* ofGetPath3dPoint object class */
    static t_class *ofeliaGetPath3dPoint_class;
    
    typedef struct _ofeliaGetPath3dPoint
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPath3dPoint;
    
    /* ofGetPath2dPoints object class */
    static t_class *ofeliaGetPath2dPoints_class;
    
    typedef struct _ofeliaGetPath2dPoints
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPath2dPoints;
    
    /* ofGetPath3dPoints object class */
    static t_class *ofeliaGetPath3dPoints_class;
    
    typedef struct _ofeliaGetPath3dPoints
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPath3dPoints;
    
    /* ofIsPointInsidePath2d object class */
    static t_class *ofeliaIsPointInsidePath2d_class;
    
    typedef struct _ofeliaIsPointInsidePath2d
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaIsPointInsidePath2d;
    
    /* ofIsPointInsidePath3d object class */
    static t_class *ofeliaIsPointInsidePath3d_class;
    
    typedef struct _ofeliaIsPointInsidePath3d
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaIsPointInsidePath3d;
    
    /* ofGetPath2dCommand object class */
    static t_class *ofeliaGetPath2dCommand_class;
    
    typedef struct _ofeliaGetPath2dCommand
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPath2dCommand;
    
    /* ofGetPath3dCommand object class */
    static t_class *ofeliaGetPath3dCommand_class;
    
    typedef struct _ofeliaGetPath3dCommand
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPath3dCommand;
    
    /* ofGetPath2dCommands object class */
    static t_class *ofeliaGetPath2dCommands_class;
    
    typedef struct _ofeliaGetPath2dCommands
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPath2dCommands;
    
    /* ofGetPath3dCommands object class */
    static t_class *ofeliaGetPath3dCommands_class;
    
    typedef struct _ofeliaGetPath3dCommands
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPath3dCommands;
    
    /* ofGetPath2dTessellation object class */
    static t_class *ofeliaGetPath2dTessellation_class;
    
    typedef struct _ofeliaGetPath2dTessellation
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPath2dTessellation;
    
    /* ofGetPath3dTessellation object class */
    static t_class *ofeliaGetPath3dTessellation_class;
    
    typedef struct _ofeliaGetPath3dTessellation
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPath3dTessellation;
    
    /* ofGetPath2dBoundingBox object class */
    static t_class *ofeliaGetPath2dBoundingBox_class;
    
    typedef struct _ofeliaGetPath2dBoundingBox
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPath2dBoundingBox;
    
    /* ofGetPath3dBoundingBox object class */
    static t_class *ofeliaGetPath3dBoundingBox_class;
    
    typedef struct _ofeliaGetPath3dBoundingBox
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPath3dBoundingBox;
    
    /* ofGetPath2dCentroid object class */
    static t_class *ofeliaGetPath2dCentroid_class;
    
    typedef struct _ofeliaGetPath2dCentroid
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPath2dCentroid;
    
    /* ofGetPath3dCentroid object class */
    static t_class *ofeliaGetPath3dCentroid_class;
    
    typedef struct _ofeliaGetPath3dCentroid
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPath3dCentroid;
    
    /* ofGetPath2dArea object class */
    static t_class *ofeliaGetPath2dArea_class;
    
    typedef struct _ofeliaGetPath2dArea
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPath2dArea;
    
    /* ofGetPath3dArea object class */
    static t_class *ofeliaGetPath3dArea_class;
    
    typedef struct _ofeliaGetPath3dArea
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPath3dArea;
    
    /* ofGetPath2dPerimeter object class */
    static t_class *ofeliaGetPath2dPerimeter_class;
    
    typedef struct _ofeliaGetPath2dPerimeter
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPath2dPerimeter;
    
    /* ofGetPath3dPerimeter object class */
    static t_class *ofeliaGetPath3dPerimeter_class;
    
    typedef struct _ofeliaGetPath3dPerimeter
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetPath3dPerimeter;
    
    /* setup method */
    void ofeliaPath_setup();
}

#endif /* ofeliaPath_h */
