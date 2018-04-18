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

#ifndef ofeliaImage_h
#define ofeliaImage_h

#pragma once

#include "ofeliaBase.h"
#include "ofeliaImageLoader.h"
#include "ofeliaImageSaver.h"

extern "C" {
    
    /* common structs */
    typedef struct _ofeliaCommonImageData {
        
        unsigned int objID;
        t_ofeliaVarName varName;
        vector<t_string> paths;
        t_ofeliaImageData data;
        
    } t_ofeliaCommonImageData;
    
    /* methods */
    int getPositionByImageName(const t_string &name, bool &isLoadedImage);
    
    /* ofCreateImage object class */
    static t_class *ofeliaCreateImage_class;
    
    typedef struct _ofeliaCreateImage
    {
        t_object x_obj;
        static const char *objName;
        static unsigned int counter;
        static vector<t_ofeliaCommonImageData> imageData;
        static vector<shared_ptr<ofImage>> images;
        static bool bInited;
        bool bInitGate;
        unsigned int objID;
        bool shouldAlloc;
        bool shouldColor;
        t_outlet *jobDone_out;
        
    } t_ofeliaCreateImage;
    
    /* ofLoadImage object class */
    static t_class *ofeliaLoadImage_class;
    
    enum LoadImageCmdState {
        
        IMAGE_LOAD_CMD_INSERT,
        IMAGE_LOAD_CMD_ERASE,
        IMAGE_LOAD_CMD_FILL
    };
    typedef struct _ofeliaLoadImageCmdData {
        
        LoadImageCmdState state;
        t_string path;
        t_float fromIndex, toIndex;
       
    } t_ofeliaLoadImageCmdData;
    
    typedef struct _ofeliaLoadImage
    {
        t_object x_obj;
        static const char *objName;
        static unsigned int counter;
        static vector<t_ofeliaCommonImageData> imageData;
        static vector<vector<shared_ptr<ofImage>>> images;
        static vector<vector<shared_ptr<ofImage>>> tempImages;
        static vector<unique_ptr<ofeliaImageLoader>> loaders;
        vector<t_ofeliaLoadImageCmdData> cmdVec;
        unique_ptr<vector<char>> imageLoaded;
        unique_ptr<vector<unsigned int>> imageIDs;
        unique_ptr<vector<unsigned int>> tempImageIDs;
        vector<t_string> failPaths;
        bool failPathsShouldClear;
        vector<int> vecSizes;
        bool vecSizesShouldClear;
        static bool bInited;
        bool bInitGate;
        unsigned int objID;
        unsigned int imgID;
        bool shouldOutlet;
        t_canvas *canvas;
        t_outlet *jobDone_out, *vecSize_out;

    } t_ofeliaLoadImage;
    
    /* ofEditImage object class */
    static t_class *ofeliaEditImage_class;
    
    enum EditImageCmdState {
        
        IMAGE_EDIT_CMD_COLORAT,
        IMAGE_EDIT_CMD_COLOR,
        IMAGE_EDIT_CMD_GRAYSCALE,
        IMAGE_EDIT_CMD_INVERT,
        IMAGE_EDIT_CMD_BRIGHTNESS,
        IMAGE_EDIT_CMD_CONTRAST,
        IMAGE_EDIT_CMD_GAMMA,
        IMAGE_EDIT_CMD_TYPE,
        IMAGE_EDIT_CMD_RESIZE,
        IMAGE_EDIT_CMD_CROP,
        IMAGE_EDIT_CMD_MIRROR,
        IMAGE_EDIT_CMD_GENERATEMIPMAP,
        IMAGE_EDIT_CMD_MINMAGFILTER,
        IMAGE_EDIT_CMD_TEXWRAP,
        IMAGE_EDIT_CMD_CLONE,
        IMAGE_EDIT_CMD_ALPHAMASK,
        IMAGE_EDIT_CMD_GRABSCREEN
    };
    enum EditImageErrorType {
        
        IMAGE_EDIT_ERROR_NOERROR,
        IMAGE_EDIT_ERROR_NOTASSIGNED,
        IMAGE_EDIT_ERROR_FAILTOFIND,
        IMAGE_EDIT_ERROR_EMPTY,
        IMAGE_EDIT_ERROR_NOTLOADED,
        IMAGE_EDIT_ERROR_NOTALLOCATED
    };
    enum MinMagFilterType {
        
        IMAGE_MINMAG_NEAREST,
        IMAGE_MINMAG_LINEAR,
        IMAGE_MINMAG_NEAREST_MIPMAP_NEAREST,
        IMAGE_MINMAG_LINEAR_MIPMAP_NEAREST,
        IMAGE_MINMAG_NEAREST_MIPMAP_LINEAR,
        IMAGE_MINMAG_LINEAR_MIPMAP_LINEAR
    };
    enum TexWrapType {
        
        IMAGE_TEXWRAP_REPEAT,
        IMAGE_TEXWRAP_MIRRORED_REPEAT,
        IMAGE_TEXWRAP_CLAMP_TO_EDGE,
        IMAGE_TEXWRAP_CLAMP_TO_BORDER
    };
    typedef struct _ofeliaEditImageCmdData {
        
        EditImageCmdState state;
        ofColor color;
        int args[4];
        t_ofeliaVarName varName;
        
    } t_ofeliaEditImageCmdData;
    
    typedef struct _ofeliaEditImage
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        vector<t_ofeliaEditImageCmdData> cmdVec;
        EditImageErrorType errorType;
        t_string errorName;
        
    } t_ofeliaEditImage;
    
    /* ofSaveImage object class */
    static t_class *ofeliaSaveImage_class;
    
    typedef struct _ofeliaSaveImage
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        unique_ptr<ofeliaImageSaver> saver;
        t_clock *jobDoneOutClock;
        
    } t_ofeliaSaveImage;
    
    /* ofBindImageTex object class */
    static t_class *ofeliaBindImageTex_class;
    
    typedef struct _ofeliaBindImageTex
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaBindImageTex;
    
    /* ofDrawImage object class */
    static t_class *ofeliaDrawImage_class;
    
    typedef struct _ofeliaDrawImage
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaNameDimen2dElem elem;
        
    } t_ofeliaDrawImage;
    
    /* ofDrawSubImage object class */
    static t_class *ofeliaDrawSubImage_class;
    
    typedef struct _ofeliaDrawSubImageElem
    {
        t_ofeliaVarName varName;
        float width;
        float height;
        float subX;
        float subY;
        float subWidth;
        float subHeight;
        
    } t_ofeliaDrawSubImageElem;
    
    typedef struct _ofeliaDrawSubImage
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaDrawSubImageElem elem;

    } t_ofeliaDrawSubImage;
    
    /* ofDoesImageNameExist object class */
    static t_class *ofeliaDoesImageNameExist_class;
    
    typedef struct _ofeliaDoesImageNameExist
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaDoesImageNameExist;
    
    /* ofGetImagePath object class */
    static t_class *ofeliaGetImagePath_class;
    
    typedef struct _ofeliaGetImagePath
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetImagePath;
    
    /* ofIsImageAllocated object class */
    static t_class *ofeliaIsImageAllocated_class;
    
    typedef struct _ofeliaIsImageAllocated
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaIsImageAllocated;
    
    /* ofGetImageDimen object class */
    static t_class *ofeliaGetImageDimen_class;
    
    typedef struct _ofeliaGetImageDimen
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetImageDimen;
    
    /* ofGetImageType object class */
    static t_class *ofeliaGetImageType_class;
    
    typedef struct _ofeliaGetImageType
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetImageType;
    
    /* ofGetImageColorAt object class */
    static t_class *ofeliaGetImageColorAt_class;
    
    typedef struct _ofeliaGetImageColorAt
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetImageColorAt;
    
    /* ofGetImageTexCoord object class */
    static t_class *ofeliaGetImageTexCoord_class;
    
    typedef struct _ofeliaGetImageTexCoord
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetImageTexCoord;
    
    /* ofGetImageTexCoords object class */
    static t_class *ofeliaGetImageTexCoords_class;
    
    typedef struct _ofeliaGetImageTexCoords
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetImageTexCoords;
    
    /* ofGetImageTexID object class */
    static t_class *ofeliaGetImageTexID_class;
    
    typedef struct _ofeliaGetImageTexID
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetImageTexID;
    
    /* setup method */
    void ofeliaImage_setup();
}

#endif /* ofeliaImage_h */
