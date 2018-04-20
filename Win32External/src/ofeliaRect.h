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

#ifndef ofeliaRect_h
#define ofeliaRect_h

#include "ofeliaBase.h"

extern "C" {
    
    /* ofLoadRect object class */
    static t_class *ofeliaLoadRect_class;
    
    typedef struct _ofeliaLoadRectData {
        
        unsigned int objID;
        t_ofeliaVarName varName;
        vector<ofRectangle> rects;
        
    } t_ofeliaLoadRectData;
    
    typedef struct _ofeliaLoadRectCmdData {
        
        ofRectangle rect;
        t_float fromIndex, toIndex;
        
    } t_ofeliaLoadRectCmdData;
    
    typedef struct _ofeliaLoadRect
    {
        t_object x_obj;
        static const char *objName;
        static unsigned int counter;
        static vector<t_ofeliaLoadRectData> rectData;
        unsigned int objID;
        int vecSize;
        t_clock *vecSizeOutClock;
        
    } t_ofeliaLoadRect;
    
    /* ofEditRect object class */
    static t_class *ofeliaEditRect_class;
    
    typedef struct _ofeliaEditRect
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaEditRect;
    
    /* ofDoesRectNameExist object class */
    static t_class *ofeliaDoesRectNameExist_class;
    
    typedef struct _ofeliaDoesRectNameExist
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaDoesRectNameExist;
    
    /* ofGetRect object class */
    static t_class *ofeliaGetRect_class;
    
    typedef struct _ofeliaGetRect
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetRect;
    
    /* ofGetRects object class */
    static t_class *ofeliaGetRects_class;
    
    typedef struct _ofeliaGetRects
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetRects;
    
    /* ofIsPointInsideRect object class */
    static t_class *ofeliaIsPointInsideRect_class;
    
    typedef struct _ofeliaIsPointInsideRect
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaIsPointInsideRect;
    
    /* ofIsLineInsideRect object class */
    static t_class *ofeliaIsLineInsideRect_class;
    
    typedef struct _ofeliaIsLineInsideRect
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaIsLineInsideRect;
    
    /* ofIsRectInsideRect object class */
    static t_class *ofeliaIsRectInsideRect_class;
    
    typedef struct _ofeliaIsRectInsideRect
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaIsRectInsideRect;
    
    /* ofDoesLineIntersectRect object class */
    static t_class *ofeliaDoesLineIntersectRect_class;
    
    typedef struct _ofeliaDoesLineIntersectRect
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaDoesLineIntersectRect;
    
    /* ofDoesRectIntersectRect object class */
    static t_class *ofeliaDoesRectIntersectRect_class;
    
    typedef struct _ofeliaDoesRectIntersectRect
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaDoesRectIntersectRect;
    
    /* ofGetRectCenter object class */
    static t_class *ofeliaGetRectCenter_class;
    
    typedef struct _ofeliaGetRectCenter
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetRectCenter;
    
    /* ofGetRectArea object class */
    static t_class *ofeliaGetRectArea_class;
    
    typedef struct _ofeliaGetRectArea
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetRectArea;
    
    /* ofGetRectPerimeter object class */
    static t_class *ofeliaGetRectPerimeter_class;
    
    typedef struct _ofeliaGetRectPerimeter
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetRectPerimeter;
    
    /* setup method */
    void ofeliaRect_setup();
}

#endif /* ofeliaRect_h */

