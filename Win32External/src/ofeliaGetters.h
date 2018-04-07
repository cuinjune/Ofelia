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

#ifndef ofeliaGetters_h
#define ofeliaGetters_h

#pragma once

#include "ofeliaBase.h"

extern "C" {
    
    /* ofGetWidth object class */
    static t_class *ofeliaGetWidth_class;
    
    typedef struct _ofeliaGetWidth
    {
        t_object x_obj;
        static t_symbol *getWidthSym;
        
    } t_ofeliaGetWidth;
    
    /* ofGetHeight object class */
    static t_class *ofeliaGetHeight_class;
    
    typedef struct _ofeliaGetHeight
    {
        t_object x_obj;
        static t_symbol *getHeightSym;
        
    } t_ofeliaGetHeight;
    
    /* ofGetDimen object class */
    static t_class *ofeliaGetDimen_class;
    
    typedef struct _ofeliaGetDimen
    {
        t_object x_obj;
        static t_symbol *getDimenSym;
        
    } t_ofeliaGetDimen;
    
    /* ofGetWindowScale object class */
    static t_class *ofeliaGetWindowScale_class;
    
    typedef struct _ofeliaGetWinowScale
    {
        t_object x_obj;
        static t_symbol *getWindowScaleSym;
        
    } t_ofeliaGetWindowScale;
    
    /* ofGetFrameNum object class */
    static t_class *ofeliaGetFrameNum_class;
    
    typedef struct _ofeliaGetFrameNum
    {
        t_object x_obj;
        static t_symbol *getFrameNumSym;
        
    } t_ofeliaGetFrameNum;

    /* ofGetFrameRate object class */
    static t_class *ofeliaGetFrameRate_class;
    
    typedef struct _ofeliaGetFrameRate
    {
        t_object x_obj;
        static t_symbol *getFrameRateSym;
        
    } t_ofeliaGetFrameRate;
    
    /* ofGetTargetFrameRate object class */
    static t_class *ofeliaGetTargetFrameRate_class;
    
    typedef struct _ofeliaGetTargetFrameRate
    {
        t_object x_obj;
        static t_symbol *getTargetFrameRateSym;
        
    } t_ofeliaGetTargetFrameRate;
    
    /* ofGetElapsedTime object class */
    static t_class *ofeliaGetElapsedTime_class;
    
    typedef struct _ofeliaGetElapsedTime
    {
        t_object x_obj;
        static t_symbol *getElapsedTimeSym;
        
    } t_ofeliaGetElapsedTime;
    
    /* ofGetElapsedTimeMillis object class */
    static t_class *ofeliaGetElapsedTimeMillis_class;
    
    typedef struct _ofeliaGetElapsedTimeMillis
    {
        t_object x_obj;
        static t_symbol *getElapsedTimeMillisSym;
        
    } t_ofeliaGetElapsedTimeMillis;
    
    /* ofGetLastFrameTime object class */
    static t_class *ofeliaGetLastFrameTime_class;
    
    typedef struct _ofeliaGetLastFrameTime
    {
        t_object x_obj;
        static t_symbol *getLastFrameTimeSym;
        
    } t_ofeliaGetLastFrameTime;
    
    /* ofGetLastFrameTimeMillis object class */
    static t_class *ofeliaGetLastFrameTimeMillis_class;
    
    typedef struct _ofeliaGetLastFrameTimeMillis
    {
        t_object x_obj;
        static t_symbol *getLastFrameTimeMillisSym;
        
    } t_ofeliaGetLastFrameTimeMillis;
    
    /* ofGetOrienLock object class */
    static t_class *ofeliaGetOrienLock_class;
    
    typedef struct _ofeliaGetOrienLock
    {
        t_object x_obj;
        static t_symbol *getOrienLockSym;
        
    } t_ofeliaGetOrienLock;
    
    /* ofGetOrien object class */
    static t_class *ofeliaGetOrien_class;
    
    typedef struct _ofeliaGetOrien
    {
        t_object x_obj;
        static t_symbol *getOrienSym;
        
    } t_ofeliaGetOrien;
    
    /* ofGetFullscreen object class */
    static t_class *ofeliaGetFullscreen_class;
    
    typedef struct _ofeliaGetFullscreen
    {
        t_object x_obj;
        static t_symbol *getFullscreenSym;
        
    } t_ofeliaGetFullscreen;
    
    /* ofGetFocus object class */
    static t_class *ofeliaGetFocus_class;
    
    typedef struct _ofeliaGetFocus
    {
        t_object x_obj;
        static t_symbol *getFocusSym;
        
    } t_ofeliaGetFocus;
    
    /* ofGetWindowPos_x object class */
    static t_class *ofeliaGetWindowPosX_class;
    
    typedef struct _ofeliaGetWindowPosX
    {
        t_object x_obj;
        static t_symbol *getWindowPosXSym;
        
    } t_ofeliaGetWindowPosX;
    
    /* ofGetWindowPosY object class */
    static t_class *ofeliaGetWindowPosY_class;
    
    typedef struct _ofeliaGetWindowPosY
    {
        t_object x_obj;
        static t_symbol *getWindowPosYSym;
        
    } t_ofeliaGetWindowPosY;
    
    /* ofGetWindowPos object class */
    static t_class *ofeliaGetWindowPos_class;
    
    typedef struct _ofeliaGetWindowPos
    {
        t_object x_obj;
        static t_symbol *getWindowPosSym;
        
    } t_ofeliaGetWindowPos;
    
    /* ofGetScreenWidth object class */
    static t_class *ofeliaGetScreenWidth_class;
    
    typedef struct _ofeliaGetScreenWidth
    {
        t_object x_obj;
        static t_symbol *getScreenWidthSym;
        
    } t_ofeliaGetScreenWidth;
    
    /* ofGetScreenHeight object class */
    static t_class *ofeliaGetScreenHeight_class;
    
    typedef struct _ofeliaGetScreenHeight
    {
        t_object x_obj;
        static t_symbol *getScreenHeightSym;
        
    } t_ofeliaGetScreenHeight;
    
    /* ofGetScreenDimen object class */
    static t_class *ofeliaGetScreenDimen_class;
    
    typedef struct _ofeliaGetScreenDimen
    {
        t_object x_obj;
        static t_symbol *getScreenDimenSym;
        
    } t_ofeliaGetScreenDimen;
    
    /* ofGetRetina object class */
    static t_class *ofeliaGetRetina_class;
    
    typedef struct _ofeliaGetRetina
    {
        t_object x_obj;
        static t_symbol *getRetinaSym;
        
    } t_ofeliaGetRetina;
    
    /* ofGetBgColorR object class */
    static t_class *ofeliaGetBgColorR_class;
    
    typedef struct _ofeliaGetBgColorR
    {
        t_object x_obj;
        static t_symbol *getBgColorRSym;
        
    } t_ofeliaGetBgColorR;
    
    /* ofGetBgColorG object class */
    static t_class *ofeliaGetBgColorG_class;
    
    typedef struct _ofeliaGetBgColorG
    {
        t_object x_obj;
        static t_symbol *getBgColorGSym;
        
    } t_ofeliaGetBgColorG;
    
    /* ofGetBgColorB object class */
    static t_class *ofeliaGetBgColorB_class;
    
    typedef struct _ofeliaGetBgColorB
    {
        t_object x_obj;
        static t_symbol *getBgColorBSym;
        
    } t_ofeliaGetBgColorB;
    
    /* ofGetBgColor object class */
    static t_class *ofeliaGetBgColor_class;
    
    typedef struct _ofeliaGetBgColor
    {
        t_object x_obj;
        static t_symbol *getBgColorSym;
        
    } t_ofeliaGetBgColor;
    
    /* ofGetWindow object class */
    static t_class *ofeliaGetWindow_class;
    
    typedef struct _ofeliaGetWindow
    {
        t_object x_obj;
        static t_symbol *getWindowSym;
        
    } t_ofeliaGetWindow;
    
    /* ofGetFirstRenderOrder object class */
    static t_class *ofeliaGetFirstRenderOrder_class;
    
    typedef struct _ofeliaGetFirstRenderOrder
    {
        t_object x_obj;
        
    } t_ofeliaGetFirstRenderOrder;
    
    /* ofGetLastRenderOrder object class */
    static t_class *ofeliaGetLastRenderOrder_class;
    
    typedef struct _ofeliaGetLastRenderOrder
    {
        t_object x_obj;
        
    } t_ofeliaGetLastRenderOrder;
    
    /* setup method */
    void ofeliaGetters_setup();
}

#endif /* ofeliaGetters_h */
