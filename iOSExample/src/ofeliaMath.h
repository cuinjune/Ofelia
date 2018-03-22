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

#ifndef ofeliaMath_h
#define ofeliaMath_h

#pragma once

#include "ofeliaBase.h"

extern "C" {
    
    /* ofAngleDifferenceDegrees object class */
    static t_class *ofeliaAngleDifferenceDegrees_class;
    
    typedef struct _ofeliaAngleDifferenceDegrees
    {
        t_object x_obj;
        static const char *objName;
        float currentAngle;
        
    } t_ofeliaAngleDifferenceDegrees;
    
    /* ofAngleDifferenceRadians object class */
    static t_class *ofeliaAngleDifferenceRadians_class;
    
    typedef struct _ofeliaAngleDifferenceRadians
    {
        t_object x_obj;
        static const char *objName;
        float currentAngle;
        
    } t_ofeliaAngleDifferenceRadians;
    
    /* ofDegToRad object class */
    static t_class *ofeliaDegToRad_class;
    
    typedef struct _ofeliaDegToRad
    {
        t_object x_obj;
        static const char *objName;

    } t_ofeliaDegToRad;
    
    /* ofRadToDeg object class */
    static t_class *ofeliaRadToDeg_class;
    
    typedef struct _ofeliaRagToDeg
    {
        t_object x_obj;
        static const char *objName;
        
    } t_ofeliaRadToDeg;
    
    /* ofDist2d object class */
    static t_class *ofeliaDist2d_class;
    
    typedef struct _ofeliaDist2d
    {
        t_object x_obj;
        static const char *objName;
        ofVec2f p1;
        
    } t_ofeliaDist2d;
    
    /* ofDist3d object class */
    static t_class *ofeliaDist3d_class;
    
    typedef struct _ofeliaDist3d
    {
        t_object x_obj;
        static const char *objName;
        ofVec3f p1;
        
    } t_ofeliaDist3d;
    
    /* ofDistSquared2d object class */
    static t_class *ofeliaDistSquared2d_class;
    
    typedef struct _ofeliaDistSquared2d
    {
        t_object x_obj;
        static const char *objName;
        ofVec2f p1;
        
    } t_ofeliaDistSquared2d;
    
    /* ofDistSquared3d object class */
    static t_class *ofeliaDistSquared3d_class;
    
    typedef struct _ofeliaDistSquared3d
    {
        t_object x_obj;
        static const char *objName;
        ofVec3f p1;
        
    } t_ofeliaDistSquared3d;
    
    /* common structs */
    typedef struct _ofeliaRangeElem
    {
        float min;
        float max;
        
    } t_ofeliaRangeElem;
    
    /* ofInRange object class */
    static t_class *ofeliaInRange_class;
    
    typedef struct _ofeliaInRange
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaRangeElem elem;
        
    } t_ofeliaInRange;
    
    /* ofClamp object class */
    static t_class *ofeliaClamp_class;
    
    typedef struct _ofeliaClamp
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaRangeElem elem;
        
    } t_ofeliaClamp;
    
    /* ofNormalize object class */
    static t_class *ofeliaNormalize_class;
    
    typedef struct _ofeliaNormalize
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaRangeElem elem;
        
    } t_ofeliaNormalize;
    
    /* ofLerp object class */
    static t_class *ofeliaLerp_class;
    
    typedef struct _ofeliaLerp
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaRangeElem elem;
        
    } t_ofeliaLerp;
    
    /* ofLerpDegrees object class */
    static t_class *ofeliaLerpDegrees_class;
    
    typedef struct _ofeliaLerpDegrees
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaRangeElem elem;
        
    } t_ofeliaLerpDegrees;
    
    /* ofLerpRadians object class */
    static t_class *ofeliaLerpRadians_class;
    
    typedef struct _ofeliaLerpRadians
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaRangeElem elem;
        
    } t_ofeliaLerpRadians;
    
    /* ofRandom object class */
    static t_class *ofeliaRandom_class;
    
    typedef struct _ofeliaRandom
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaRangeElem elem;
        
    } t_ofeliaRandom;
    
    /* ofRandomf object class */
    static t_class *ofeliaRandomf_class;
    
    typedef struct _ofeliaRandomf
    {
        t_object x_obj;
        static const char *objName;
        
    } t_ofeliaRandomf;
    
    /* ofRandomuf object class */
    static t_class *ofeliaRandomuf_class;
    
    typedef struct _ofeliaRandomuf
    {
        t_object x_obj;
        static const char *objName;
        
    } t_ofeliaRandomuf;
    
    /* ofSeedRandom object class */
    static t_class *ofeliaSeedRandom_class;
    
    typedef struct _ofeliaSeedRandom
    {
        t_object x_obj;
        static const char *objName;
        
    } t_ofeliaSeedRandom;
    
    /* ofWrap object class */
    static t_class *ofeliaWrap_class;
    
    typedef struct _ofeliaWrap
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaRangeElem elem;
        
    } t_ofeliaWrap;
    
    /* ofWrapDegrees object class */
    static t_class *ofeliaWrapDegrees_class;
    
    typedef struct _ofeliaWrapDegrees
    {
        t_object x_obj;
        static const char *objName;
        
    } t_ofeliaWrapDegrees;
    
    /* ofWrapRadians object class */
    static t_class *ofeliaWrapRadians_class;
    
    typedef struct _ofeliaWrapRadians
    {
        t_object x_obj;
        static const char *objName;
        
    } t_ofeliaWrapRadians;
    
    /* ofMap object class */
    static t_class *ofeliaMap_class;
    
    typedef struct _ofeliaMapElem
    {
        float inputMin;
        float inputMax;
        float outputMin;
        float outputMax;
        bool clamp;
        
    } t_ofeliaMapElem;
    
    typedef struct _ofeliaMap
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaMapElem elem;
        
    } t_ofeliaMap;
    
    /* ofNextPow2 object class */
    static t_class *ofeliaNextPow2_class;
    
    typedef struct _ofeliaNextPow2
    {
        t_object x_obj;
        static const char *objName;
        
    } t_ofeliaNextPow2;
    
    /* ofNoise object class */
    static t_class *ofeliaNoise_class;
    
    typedef struct _ofeliaNoise
    {
        t_object x_obj;
        static const char *objName;
        
    } t_ofeliaNoise;
    
    /* ofSignedNoise object class */
    static t_class *ofeliaSignedNoise_class;
    
    typedef struct _ofeliaSignedNoise
    {
        t_object x_obj;
        static const char *objName;
        
    } t_ofeliaSignedNoise;
    
    /* ofSign object class */
    static t_class *ofeliaSign_class;
    
    typedef struct _ofeliaSign
    {
        t_object x_obj;
        static const char *objName;
        
    } t_ofeliaSign;
    
    /* setup methods */
    void ofeliaMath_setup();
}

#endif /* ofeliaMath_h */
