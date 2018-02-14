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

#ifndef ofeliaSetters_h
#define ofeliaSetters_h

#pragma once

#include "ofeliaBase.h"

extern "C" {
    
    /* ofSetDepthTest object class */
    static t_class *ofeliaSetDepthTest_class;

    typedef struct _ofeliaSetDepthTest
    {
        t_object x_obj;
        static const char *objName;
        bool depthTest;
        
    } t_ofeliaSetDepthTest;
    
    /* ofSetArbTex object class */
    static t_class *ofeliaSetArbTex_class;
    
    typedef struct _ofeliaSetArbTex
    {
        t_object x_obj;
        static const char *objName;
        bool arbTex;
        
    } t_ofeliaSetArbTex;
    
    /* ofSetAntiAliasing object class */
    static t_class *ofeliaSetAntiAliasing_class;
    
    typedef struct _ofeliaSetAntiAliasing
    {
        t_object x_obj;
        static const char *objName;
        bool antiAliasing;
        
    } t_ofeliaSetAntiAliasing;
    
    /* ofSetBgAuto object class */
    static t_class *ofeliaSetBgAuto_class;
    
    typedef struct _ofeliaSetBgAuto
    {
        t_object x_obj;
        static const char *objName;
        bool bgAuto;
        
    } t_ofeliaSetBgAuto;
    
    /* ofClear object class */
    static t_class *ofeliaClear_class;
    
    typedef struct _ofeliaClear
    {
        t_object x_obj;
        static const char *objName;
        ofColor color;
        
    } t_ofeliaClear;
    
    /* ofClearColor object class */
    static t_class *ofeliaClearColor_class;
    
    typedef struct _ofeliaClearColor
    {
        t_object x_obj;
        static const char *objName;
        ofColor color;
        
    } t_ofeliaClearColor;
    
    /* ofClearDepth object class */
    static t_class *ofeliaClearDepth_class;
    
    typedef struct _ofeliaClearDepth
    {
        t_object x_obj;
        static const char *objName;
        
    } t_ofeliaClearDepth;
    
    /* ofClearAlpha object class */
    static t_class *ofeliaClearAlpha_class;
    
    typedef struct _ofeliaClearAlpha
    {
        t_object x_obj;
        static const char *objName;
   
    } t_ofeliaClearAlpha;
    
    /* setup method */
    void ofeliaSetters_setup();
}

#endif /* ofeliaSetters_h */
