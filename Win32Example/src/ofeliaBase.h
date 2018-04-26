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

#ifndef ofeliaBase_h
#define ofeliaBase_h

#pragma once

#include "m_pd.h"
#include "ofMain.h"
#include "ofeliaString.h"

/* ofelia version info */
#define OFELIA_MAJOR_VERSION 1
#define OFELIA_MINOR_VERSION 0
#define OFELIA_BUGFIX_VERSION 8

/* default window property values */
#define OFELIA_DEFAULT_WINDOWWIDTH 320
#define OFELIA_DEFAULT_WINDOWHEIGHT 240
#define OFELIA_DEFAULT_BGCOLOR 200
#define OFELIA_MAX_SCREENSIZE 8192
#define OFELIA_MAX_WINDOWSCALE 10.0f

/* maximum number of simulated multitouches */
#define OFELIA_MAX_TOUCHES 5

/* default size of graphical objects */
#define OFELIA_DEFAULT_SIZE 100.0f
#define OFELIA_DEFAULT_SIZE_HALF 50.0f
#define OFELIA_DEFAULT_SIZE_DOUBLE 200.0f

/* default circle & curve resolution */
#define OFELIA_DEFAULT_RESOLUTION 50

/* default rendering order */
#define OFELIA_DEFAULT_RENDER_ORDER 50.0f

/* path separator character */
#if defined(TARGET_WIN32)
#define OFELIA_PATH_SEPARATOR '\\'
#else
#define OFELIA_PATH_SEPARATOR '/'
#endif

/* custom string class is used since std::string caused a crash on Android */
typedef ofeliaString t_string;

extern "C" {
    
    /* constants */
    namespace ofeliaConstants {
        
        const float heightFactor = sqrtf(3.0f) * 0.5f; // height = side * factor (equilateral triangle)
    }
    
    /* enumerations */
    enum TextAlignHorMode {
        
        TEXT_ALIGN_LEFT,
        TEXT_ALIGN_CENTER,
        TEXT_ALIGN_RIGHT
    };
    enum TextAlignVerMode {
        
        TEXT_ALIGN_TOP,
        TEXT_ALIGN_MIDDLE,
        TEXT_ALIGN_BOTTOM
    };
    enum AtomElemType {
        
        ATOM_ELEM_TYPE_FLOAT,
        ATOM_ELEM_TYPE_SYMBOL
    };
    
    /* structs */
    typedef struct _ofeliaVarName
    {
        t_string name;
        t_string patchPrefix;
        t_string canvasPrefix;
        int index;
        
    } t_ofeliaVarName;
    
    typedef struct _ofeliaTextModeElem
    {
        TextAlignHorMode horMode;
        TextAlignVerMode verMode;
        
    } t_ofeliaTextModeElem;
    
    typedef struct _ofeliaAtomElem
    {
        AtomElemType type;
        float value;
        t_string name;
        
    } t_ofeliaAtomElem;
    
    typedef struct _ofeliaRotateElem
    {
        float angle;
        ofVec3f axis;
        
    } t_ofeliaRotateElem;
    
    typedef struct _ofeliaImageData {
        
        int width;
        int height;
        ofImageType type;
        ofColor color;
        
    } t_ofeliaImageData;
    
    typedef struct _ofeliaNameDimen2dElem
    {
        t_ofeliaVarName varName;
        float width;
        float height;
        
    } t_ofeliaNameDimen2dElem;
    
    typedef struct _ofeliaNameDimen3dElem
    {
        t_ofeliaVarName varName;
        float width;
        float height;
        float depth;
        
    } t_ofeliaNameDimen3dElem;
    
    /* pd related */
    struct _glist
    {
        t_object gl_obj;
        t_gobj *gl_list;
        struct _gstub *gl_stub;
        int gl_valid;
        struct _glist *gl_owner;
    };
    t_symbol *canvas_realizedollar(t_canvas *x, t_symbol *s);
    void glist_delete(t_glist *x, t_gobj *y);
    
    /* methods */
    void removeSemicolonFromArgs(int &argc, t_atom *argv);
    void getVarNameLocalPrefixes(t_ofeliaVarName &varName);
    void getVarNameLocalized(t_ofeliaVarName &varName);
    void getVarNameIndexed(t_ofeliaVarName &varName);
    void getCoordinatesFromRadAngle(float &posX, float &posY, const float radius, const float angle);
    bool doesFilenameHaveExtension(t_string &path);
    bool makePathAbsolute(t_string &path, t_canvas *canvas);
    int getClampIntValue(const int value, const int min, const int max);
    bool isNumeric(const char *str);
    bool isFloat(const char *str);
    bool endsWith(const char *str, const char *suffix);
    bool getColorFromArgs(const int argc, const t_atom *argv, ofColor &color, const char *objName);
    bool getColorAlphaFromArgs(const int argc, const t_atom *argv, ofColor &color, const char *objName);
    bool getFloatColorFromArgs(const int argc, const t_atom *argv, ofFloatColor &color, const char *objName);
    bool getFloatColorAlphaFromArgs(const int argc, const t_atom *argv,
                                    ofFloatColor &color, const char *objName);
    bool getRectModeFromArgs(const int argc, const t_atom *argv,
                             ofRectMode &rectMode, const char *objName);
    bool getTextModeFromArgs(const int argc, const t_atom *argv,
                             t_ofeliaTextModeElem &textMode, const char *objName);
    bool getPolyModeFromArgs(const int argc, const t_atom *argv,
                             ofPolyWindingMode &polyMode, const char *objName);
    void getBoundingBoxDimen2d(ofVec2f &boxDimen, const vector<ofVec3f> &vertices);
    void getBoundingBoxDimen3d(ofVec3f &boxDimen, const vector<ofVec3f> &vertices);
    bool getRotateElemFromArgs(const int argc, const t_atom *argv,
                               t_ofeliaRotateElem &elem, const char *objName);
    bool getImageDataFromArgs(const int argc, const t_atom *argv,
                              t_ofeliaImageData &data, const char *objName);
    bool getNameDimen2dElemFromArgs(const int argc, const t_atom *argv,
                                    t_ofeliaNameDimen2dElem &elem, const char *objName);
    bool getNameDimen3dElemFromArgs(const int argc, const t_atom *argv,
                                    t_ofeliaNameDimen3dElem &elem, const char *objName);
    bool getVec2fFromArgs(const int argc, const t_atom *argv, ofVec2f &pos, const char *objName);
    bool getVec3fFromArgs(const int argc, const t_atom *argv, ofVec3f &pos, const char *objName);
    bool getToggleFromSym(const t_symbol *s, bool &toggle, const char *objName);
    bool getToggleFromArgs(const int argc, const t_atom *argv, bool &toggle, const char *objName);
    void printToggle(const char *name, const bool state);
}

#endif /* ofeliaBase_h */
