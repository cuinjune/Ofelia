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

#ifndef ofeliaVec2f_h
#define ofeliaVec2f_h

#include "ofeliaBase.h"

extern "C" {
    
    /* ofLoadVec2f object class */
    static t_class *ofeliaLoadVec2f_class;
    
    typedef struct _ofeliaLoadVec2fData {
        
        unsigned int objID;
        t_ofeliaVarName varName;
        vector<ofVec2f> vecFloats;
        
    } t_ofeliaLoadVec2fData;
    
    typedef struct _ofeliaLoadVec2fCmdData {
        
        ofVec2f vecFloat;
        t_float fromIndex, toIndex;
        
    } t_ofeliaLoadVec2fCmdData;
    
    typedef struct _ofeliaLoadVec2f
    {
        t_object x_obj;
        static const char *objName;
        static unsigned int counter;
        static vector<t_ofeliaLoadVec2fData> vecData;
        unsigned int objID;
        int vecSize;
        t_clock *vecSizeOutClock;
        
    } t_ofeliaLoadVec2f;
    
    /* ofEditVec2f object class */
    static t_class *ofeliaEditVec2f_class;
    
    typedef struct _ofeliaEditVec2f
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaEditVec2f;
    
    /* ofDoesVec2fNameExist object class */
    static t_class *ofeliaDoesVec2fNameExist_class;
    
    typedef struct _ofeliaDoesVec2fNameExist
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaDoesVec2fNameExist;
    
    /* ofGetVec2f object class */
    static t_class *ofeliaGetVec2f_class;
    
    typedef struct _ofeliaGetVec2f
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetVec2f;
    
    /* ofGetVec2fs object class */
    static t_class *ofeliaGetVec2fs_class;
    
    typedef struct _ofeliaGetVec2fs
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetVec2fs;
    
    /* ofGetVec2fAverage object class */
    static t_class *ofeliaGetVec2fAverage_class;
    
    typedef struct _ofeliaGetVec2fAverage
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetVec2fAverage;
    
    /* ofGetVec2fAngle object class */
    static t_class *ofeliaGetVec2fAngle_class;
    
    typedef struct _ofeliaGetVec2fAngle
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetVec2fAngle;
    
    /* ofGetVec2fAngleRad object class */
    static t_class *ofeliaGetVec2fAngleRad_class;
    
    typedef struct _ofeliaGetVec2fAngleRad
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetVec2fAngleRad;
    
    /* ofGetVec2fDist object class */
    static t_class *ofeliaGetVec2fDist_class;
    
    typedef struct _ofeliaGetVec2fDist
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetVec2fDist;
    
    /* ofGetVec2fDistSquared object class */
    static t_class *ofeliaGetVec2fDistSquared_class;
    
    typedef struct _ofeliaGetVec2fDistSquared
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetVec2fDistSquared;
    
    /* ofGetVec2fDot object class */
    static t_class *ofeliaGetVec2fDot_class;
    
    typedef struct _ofeliaGetVec2fDot
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetVec2fDot;
    
    /* ofGetVec2fLength object class */
    static t_class *ofeliaGetVec2fLength_class;
    
    typedef struct _ofeliaGetVec2fLength
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetVec2fLength;
    
    /* ofGetVec2fLengthSquared object class */
    static t_class *ofeliaGetVec2fLengthSquared_class;
    
    typedef struct _ofeliaGetVec2fLengthSquared
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetVec2fLengthSquared;
    
    /* setup method */
    void ofeliaVec2f_setup();
}

#endif /* ofeliaVec2f_h */
