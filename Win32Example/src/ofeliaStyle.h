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

#ifndef ofeliaStyle_h
#define ofeliaStyle_h

#pragma once

#include "ofeliaBase.h"

extern "C" {
    
    /* ofPushStyle object class */
    static t_class *ofeliaPushStyle_class;
    
    typedef struct _ofeliaPushStyle
    {
        t_object x_obj;
        static const char *objName;
        
    } t_ofeliaPushStyle;
    
    /* ofPopStyle object class */
    static t_class *ofeliaPopStyle_class;
    
    typedef struct _ofeliaPopStyle
    {
        t_object x_obj;
        static const char *objName;
        
    } t_ofeliaPopStyle;
    
    /* ofSetColor object class */
    static t_class *ofeliaSetColor_class;
    
    typedef struct _ofeliaSetColor
    {
        t_object x_obj;
        static const char *objName;
        static bool getColorByName(const char *colorName, ofColor &color);
        ofColor color;
        
    } t_ofeliaSetColor;
    
    /* ofSetBgColor object class */
    static t_class *ofeliaSetBgColor_class;
    
    typedef struct _ofeliaSetBgColor
    {
        t_object x_obj;
        static const char *objName;
        ofColor bgColor;
        
    } t_ofeliaSetBgColor;
    
    /* ofSetRectMode object class */
    static t_class *ofeliaSetRectMode_class;
    
    typedef struct _ofeliaSetRectMode
    {
        t_object x_obj;
        static const char *objName;
        ofRectMode rectMode;
        
    } t_ofeliaSetRectMode;
    
    /* ofSetTextMode object class */
    static t_class *ofeliaSetTextMode_class;

    typedef struct _ofeliaSetTextMode
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaTextModeElem textMode;
        static vector<t_ofeliaTextModeElem> textModeVec;
        
    } t_ofeliaSetTextMode;
    
    /* ofSetFillMode object class */
    static t_class *ofeliaSetFillMode_class;
    
    typedef struct _ofeliaSetFillMode
    {
        t_object x_obj;
        static const char *objName;
        ofFillFlag fillMode;
        
    } t_ofeliaSetFillMode;

    /* ofSetPolyMode object class */
    static t_class *ofeliaSetPolyMode_class;
    
    typedef struct _ofeliaSetPolyMode
    {
        t_object x_obj;
        static const char *objName;
        ofPolyWindingMode polyMode;
        
    } t_ofeliaSetPolyMode;
    
    /* ofSetBlendMode object class */
    static t_class *ofeliaSetBlendMode_class;
    
    typedef struct _ofeliaSetBlendMode
    {
        t_object x_obj;
        static const char *objName;
        ofBlendMode blendMode;
        
    } t_ofeliaSetBlendMode;
    
    /* ofSetLineWidth object class */
    static t_class *ofeliaSetLineWidth_class;
    
    typedef struct _ofeliaSetLineWidth
    {
        t_object x_obj;
        static const char *objName;
        float lineWidth;
        
    } t_ofeliaSetLineWidth;
    
    /* ofSetLineSmoothing object class */
    static t_class *ofeliaSetLineSmoothing_class;
    
    typedef struct _ofeliaSetLineSmoothing
    {
        t_object x_obj;
        static const char *objName;
        bool lineSmoothing;
        
    } t_ofeliaSetLineSmoothing;
    
    /* ofSetCircleRes object class */
    static t_class *ofeliaSetCircleRes_class;
    
    typedef struct _ofeliaSetCircleRes
    {
        t_object x_obj;
        static const char *objName;
        int circleRes;
        
    } t_ofeliaSetCircleRes;
    
    /* ofSetCurveRes object class */
    static t_class *ofeliaSetCurveRes_class;
    
    typedef struct _ofeliaSetCurveRes
    {
        t_object x_obj;
        static const char *objName;
        int curveRes;
        
    } t_ofeliaSetCurveRes;
    
    void ofeliaStyle_setup();
}

#endif /* ofeliaStyle_h */
