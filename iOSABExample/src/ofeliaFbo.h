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

#ifndef ofeliaFbo_h
#define ofeliaFbo_h

#pragma once

#include "ofeliaBase.h"

extern "C" {
    
    /* common structs */
    typedef struct _ofeliaCreateFboData {
        
        unsigned int objID;
        t_ofeliaVarName varName;
        t_ofeliaImageData data;
        
    } t_ofeliaCreateFboData;
    
    /* methods */
    int getPositionByFboName(const t_string &name);
    
    /* ofCreateFbo object class */
    static t_class *ofeliaCreateFbo_class;
    
    typedef struct _ofeliaCreateFbo
    {
        t_object x_obj;
        static const char *objName;
        static unsigned int counter;
        static vector<t_ofeliaCreateFboData> fboData;
        static vector<shared_ptr<ofFbo>> fbos;
        static bool bInited;
        bool bInitGate;
        unsigned int objID;
        bool shouldAlloc;
        bool shouldColor;
        
    } t_ofeliaCreateFbo;
    
    /* ofBindFboTex object class */
    static t_class *ofeliaBindFboTex_class;
    
    typedef struct _ofeliaBindFboTex
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaBindFboTex;
    
    /* ofDrawFbo object class */
    static t_class *ofeliaDrawFbo_class;
    
    typedef struct _ofeliaDrawFbo
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaNameDimen2dElem elem;
        
    } t_ofeliaDrawFbo;
    
    /* ofDoesFboNameExist object class */
    static t_class *ofeliaDoesFboNameExist_class;
    
    typedef struct _ofeliaDoesFboNameExist
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaDoesFboNameExist;
    
    /* ofIsFboAllocated object class */
    static t_class *ofeliaIsFboAllocated_class;
    
    typedef struct _ofeliaIsFboAllocated
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaIsFboAllocated;
    
    /* ofGetFboDimen object class */
    static t_class *ofeliaGetFboDimen_class;
    
    typedef struct _ofeliaGetFboDimen
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetFboDimen;
    
    /* ofGetFboType object class */
    static t_class *ofeliaGetFboType_class;
    
    typedef struct _ofeliaGetFboType
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetFboType;
    
    /* ofGetFboMaxSamples object class */
    static t_class *ofeliaGetFboMaxSamples_class;
    
    typedef struct _ofeliaGetFboMaxSamples
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetFboMaxSamples;
    
    /* ofGetFboTexID object class */
    static t_class *ofeliaGetFboTexID_class;
    
    typedef struct _ofeliaGetFboTexID
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetFboTexID;
    
    /* setup methods */
    void ofeliaFbo_setup();
}

#endif /* ofeliaFbo_h */
