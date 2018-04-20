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

#ifndef ofeliaFont_h
#define ofeliaFont_h

#pragma once

#include "ofeliaBase.h"
#include "ofeliaStyle.h"

extern "C" {
    
    /* ofLoadFont object class */
    static t_class *ofeliaLoadFont_class;
    
    enum LoadFontCmdState {
        
        FONT_LOAD_CMD_INSERT,
        FONT_LOAD_CMD_ERASE,
        FONT_LOAD_CMD_FILL
    };
    typedef struct _ofeliaLoadFontElem {
        
        t_string path;
        int fontSize;
        unsigned int fontEditID;
        
    } t_ofeliaLoadFontElem;
    
    typedef struct _ofeliaLoadFontData {
        
        unsigned int objID;
        t_ofeliaVarName varName;
        vector<t_ofeliaLoadFontElem> elems;
        
    } t_ofeliaLoadFontData;
    
    typedef struct _ofeliaLoadFontCmdData {
        
        LoadFontCmdState state;
        t_ofeliaLoadFontElem elem;
        t_float fromIndex, toIndex;
        
    } t_ofeliaLoadFontCmdData;
    
    typedef struct _ofeliaLoadFont
    {
        t_object x_obj;
        static const char *objName;
        static unsigned int counter;
        static vector<t_ofeliaLoadFontData> fontData;
        static vector<vector<shared_ptr<ofTrueTypeFont>>> fonts;
        vector<t_ofeliaLoadFontCmdData> cmdVec;
        unique_ptr<vector<char>> fontLoaded;
        unique_ptr<vector<unsigned int>> fontIDs;
        unique_ptr<vector<unsigned int>> tempFontIDs;
        vector<t_string> failPaths;
        bool failPathsShouldClear;
        vector<int> vecSizes;
        bool vecSizesShouldClear;
        static bool bInited;
        bool bInitGate;
        unsigned int objID;
        unsigned int ftID;
        bool shouldOutlet;
        t_canvas *canvas;
        t_outlet *jobDone_out, *vecSize_out;
        
    } t_ofeliaLoadFont;
    
    /* ofEditFont object class */
    static t_class *ofeliaEditFont_class;
    
    enum EditFontCmdState {
        
        FONT_EDIT_CMD_LETTERSPACING,
        FONT_EDIT_CMD_LINEHEIGHT,
        FONT_EDIT_CMD_SPACESIZE,
        FONT_EDIT_CMD_CLONE,
    };
    enum EditFontErrorType {
        
        FONT_EDIT_ERROR_NOERROR,
        FONT_EDIT_ERROR_NOTASSIGNED,
        FONT_EDIT_ERROR_FAILTOFIND,
        FONT_EDIT_ERROR_EMPTY
    };
    typedef struct _ofeliaEditFontCmdData {
        
        EditFontCmdState state;
        float arg;
        t_ofeliaVarName varName;
        
    } t_ofeliaEditFontCmdData;
    
    typedef struct _ofeliaEditFont
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        vector<t_ofeliaEditFontCmdData> cmdVec;
        EditFontErrorType errorType;
        t_string errorName;
        bool bInitGate;
        
    } t_ofeliaEditFont;
    
    /* ofBindFontTex object class */
    static t_class *ofeliaBindFontTex_class;
    
    typedef struct _ofeliaBindFontTex
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaBindFontTex;
    
    /* common structs */
    struct TextLineElem {
        
        t_string text;
        float posX;
        float posY;
    };
    
    /* ofDrawText object class */
    static t_class *ofeliaDrawText_class;
    
    typedef struct _ofeliaDrawTextElem
    {
        t_ofeliaVarName varName;
        vector<TextLineElem> textLines;
        
    } t_ofeliaDrawTextElem;
    
    typedef struct _ofeliaDrawText
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaDrawTextElem elem;
        t_ofeliaTextModeElem textMode;
        t_ofeliaLoadFontElem fontElem;
        bool shouldUpdateTextLines;
        
    } t_ofeliaDrawText;
    
    /* ofDrawTextAsShapes object class */
    static t_class *ofeliaDrawTextAsShapes_class;
    
    typedef struct _ofeliaDrawTextAsShapesElem
    {
        t_ofeliaVarName varName;
        float size;
        vector<TextLineElem> textLines;
        
    } t_ofeliaDrawTextAsShapesElem;
    
    typedef struct _ofeliaDrawTextAsShapes
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaDrawTextAsShapesElem elem;
        t_ofeliaTextModeElem textMode;
        t_ofeliaLoadFontElem fontElem;
        bool shouldUpdateTextLines;
        
    } t_ofeliaDrawTextAsShapes;
    
    /* ofDoesFontNameExist object class */
    static t_class *ofeliaDoesFontNameExist_class;
    
    typedef struct _ofeliaDoesFontNameExist
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaDoesFontNameExist;
    
    /* ofGetFontPath object class */
    static t_class *ofeliaGetFontPath_class;
    
    typedef struct _ofeliaGetFontPath
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetFontPath;
    
    /* ofGetFontSize object class */
    static t_class *ofeliaGetFontSize_class;
    
    typedef struct _ofeliaGetFontSize
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetFontSize;
    
    /* ofIsFontLoaded object class */
    static t_class *ofeliaIsFontLoaded_class;
    
    typedef struct _ofeliaIsFontLoaded
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaIsFontLoaded;
    
    /* ofGetFontLetterSpacing object class */
    static t_class *ofeliaGetFontLetterSpacing_class;
    
    typedef struct _ofeliaGetFontLetterSpacing
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetFontLetterSpacing;
    
    /* ofGetFontLineHeight object class */
    static t_class *ofeliaGetFontLineHeight_class;
    
    typedef struct _ofeliaGetFontLineHeight
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetFontLineHeight;
    
    /* ofGetFontSpaceSize object class */
    static t_class *ofeliaGetFontSpaceSize_class;
    
    typedef struct _ofeliaGetFontSpaceSize
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetFontSpaceSize;
    
    /* ofGetTextBoundingBox object class */
    static t_class *ofeliaGetTextBoundingBox_class;
    
    typedef struct _ofeliaGetTextBoundingBox
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaDrawTextElem elem;
        
    } t_ofeliaGetTextBoundingBox;
    
    /* ofGetTextMesh2dCommands object class */
    static t_class *ofeliaGetTextMesh2dCommands_class;
    
    typedef struct _ofeliaGetTextMesh2dCommands
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaDrawTextElem elem;
        t_ofeliaTextModeElem textMode;
        
    } t_ofeliaGetTextMesh2dCommands;
    
    /* ofGetTextMesh3dCommands object class */
    static t_class *ofeliaGetTextMesh3dCommands_class;
    
    typedef struct _ofeliaGetTextMesh3dCommands
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaDrawTextElem elem;
        t_ofeliaTextModeElem textMode;
        
    } t_ofeliaGetTextMesh3dCommands;
    
    /* setup method */
    void ofeliaFont_setup();
}

#endif /* ofeliaFont_h */
