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

#ifndef ofeliaVec4f_h
#define ofeliaVec4f_h

#include "ofeliaBase.h"

extern "C" {
    
    /* ofLoadVec4f object class */
    static t_class *ofeliaLoadVec4f_class;
    
    typedef struct _ofeliaLoadVec4fData {
        
        unsigned int objID;
        t_ofeliaVarName varName;
        vector<ofVec4f> vecFloats;
        
    } t_ofeliaLoadVec4fData;
    
    typedef struct _ofeliaLoadVec4fCmdData {
        
        ofVec4f vecFloat;
        t_float fromIndex, toIndex;
        
    } t_ofeliaLoadVec4fCmdData;
    
    typedef struct _ofeliaLoadVec4f
    {
        t_object x_obj;
        static const char *objName;
        static unsigned int counter;
        static vector<t_ofeliaLoadVec4fData> vecData;
        unsigned int objID;
        int vecSize;
        t_clock *vecSizeOutClock;
        
    } t_ofeliaLoadVec4f;
    
    /* ofEditVec4f object class */
    static t_class *ofeliaEditVec4f_class;
    
    typedef struct _ofeliaEditVec4f
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaEditVec4f;
    
    /* ofDoesVec4fNameExist object class */
    static t_class *ofeliaDoesVec4fNameExist_class;
    
    typedef struct _ofeliaDoesVec4fNameExist
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaDoesVec4fNameExist;
    
    /* ofGetVec4f object class */
    static t_class *ofeliaGetVec4f_class;
    
    typedef struct _ofeliaGetVec4f
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetVec4f;
    
    /* ofGetVec4fs object class */
    static t_class *ofeliaGetVec4fs_class;
    
    typedef struct _ofeliaGetVec4fs
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetVec4fs;
    
    /* ofGetVec4fAverage object class */
    static t_class *ofeliaGetVec4fAverage_class;
    
    typedef struct _ofeliaGetVec4fAverage
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetVec4fAverage;
    
    /* ofGetVec4fAngle object class */
    static t_class *ofeliaGetVec4fAngle_class;
    
    typedef struct _ofeliaGetVec4fAngle
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetVec4fAngle;
    
    /* ofGetVec4fAngleRad object class */
    static t_class *ofeliaGetVec4fAngleRad_class;
    
    typedef struct _ofeliaGetVec4fAngleRad
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetVec4fAngleRad;
    
    /* ofGetVec4fDist object class */
    static t_class *ofeliaGetVec4fDist_class;
    
    typedef struct _ofeliaGetVec4fDist
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetVec4fDist;
    
    /* ofGetVec4fDistSquared object class */
    static t_class *ofeliaGetVec4fDistSquared_class;
    
    typedef struct _ofeliaGetVec4fDistSquared
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetVec4fDistSquared;
    
    /* ofGetVec4fDot object class */
    static t_class *ofeliaGetVec4fDot_class;
    
    typedef struct _ofeliaGetVec4fDot
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetVec4fDot;
    
    /* ofGetVec4fLength object class */
    static t_class *ofeliaGetVec4fLength_class;
    
    typedef struct _ofeliaGetVec4fLength
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetVec4fLength;
    
    /* ofGetVec4fLengthSquared object class */
    static t_class *ofeliaGetVec4fLengthSquared_class;
    
    typedef struct _ofeliaGetVec4fLengthSquared
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetVec4fLengthSquared;
    
    /* setup method */
    void ofeliaVec4f_setup();
}

#endif /* ofeliaVec4f_h */
