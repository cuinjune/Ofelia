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

#ifndef ofeliaVec3f_h
#define ofeliaVec3f_h

#include "ofeliaBase.h"

extern "C" {
    
    /* ofLoadVec3f object class */
    static t_class *ofeliaLoadVec3f_class;
    
    typedef struct _ofeliaLoadVec3fData {
        
        unsigned int objID;
        t_ofeliaVarName varName;
        vector<ofVec3f> vecFloats;
        
    } t_ofeliaLoadVec3fData;
    
    typedef struct _ofeliaLoadVec3fCmdData {
        
        ofVec3f vecFloat;
        t_float fromIndex, toIndex;
        
    } t_ofeliaLoadVec3fCmdData;
    
    typedef struct _ofeliaLoadVec3f
    {
        t_object x_obj;
        static const char *objName;
        static unsigned int counter;
        static vector<t_ofeliaLoadVec3fData> vecData;
        unsigned int objID;
        int vecSize;
        t_clock *vecSizeOutClock;
        
    } t_ofeliaLoadVec3f;
    
    /* ofEditVec3f object class */
    static t_class *ofeliaEditVec3f_class;
    
    typedef struct _ofeliaEditVec3f
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaEditVec3f;
    
    /* ofDoesVec3fNameExist object class */
    static t_class *ofeliaDoesVec3fNameExist_class;
    
    typedef struct _ofeliaDoesVec3fNameExist
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaDoesVec3fNameExist;
    
    /* ofGetVec3f object class */
    static t_class *ofeliaGetVec3f_class;
    
    typedef struct _ofeliaGetVec3f
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetVec3f;
    
    /* ofGetVec3fs object class */
    static t_class *ofeliaGetVec3fs_class;
    
    typedef struct _ofeliaGetVec3fs
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetVec3fs;
    
    /* ofGetVec3fAverage object class */
    static t_class *ofeliaGetVec3fAverage_class;
    
    typedef struct _ofeliaGetVec3fAverage
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetVec3fAverage;
    
    /* ofGetVec3fAngle object class */
    static t_class *ofeliaGetVec3fAngle_class;
    
    typedef struct _ofeliaGetVec3fAngle
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetVec3fAngle;
    
    /* ofGetVec3fAngleRad object class */
    static t_class *ofeliaGetVec3fAngleRad_class;
    
    typedef struct _ofeliaGetVec3fAngleRad
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetVec3fAngleRad;
    
    /* ofGetVec3fDist object class */
    static t_class *ofeliaGetVec3fDist_class;
    
    typedef struct _ofeliaGetVec3fDist
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetVec3fDist;
    
    /* ofGetVec3fDistSquared object class */
    static t_class *ofeliaGetVec3fDistSquared_class;
    
    typedef struct _ofeliaGetVec3fDistSquared
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetVec3fDistSquared;
    
    /* ofGetVec3fDot object class */
    static t_class *ofeliaGetVec3fDot_class;
    
    typedef struct _ofeliaGetVec3fDot
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetVec3fDot;
    
    /* ofGetVec3fLength object class */
    static t_class *ofeliaGetVec3fLength_class;
    
    typedef struct _ofeliaGetVec3fLength
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetVec3fLength;
    
    /* ofGetVec3fLengthSquared object class */
    static t_class *ofeliaGetVec3fLengthSquared_class;
    
    typedef struct _ofeliaGetVec3fLengthSquared
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetVec3fLengthSquared;
    
    /* setup method */
    void ofeliaVec3f_setup();
}

#endif /* ofeliaVec3f_h */
