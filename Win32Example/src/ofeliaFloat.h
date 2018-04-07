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

#ifndef ofeliaFloat_h
#define ofeliaFloat_h

#include "ofeliaBase.h"

extern "C" {
    
    /* ofLoadFloat object class */
    static t_class *ofeliaLoadFloat_class;
    
    typedef struct _ofeliaLoadFloatData {
        
        unsigned int objID;
        t_ofeliaVarName varName;
        vector<float> vecFloats;
        
    } t_ofeliaLoadFloatData;
    
    typedef struct _ofeliaLoadFloatCmdData {

        float vecFloat;
        t_float fromIndex, toIndex;
        
    } t_ofeliaLoadFloatCmdData;
    
    typedef struct _ofeliaLoadFloat
    {
        t_object x_obj;
        static const char *objName;
        static unsigned int counter;
        static vector<t_ofeliaLoadFloatData> vecData;
        unsigned int objID;
        int vecSize;
        t_clock *vecSizeOutClock;
        
    } t_ofeliaLoadFloat;
    
    /* ofEditFloat object class */
    static t_class *ofeliaEditFloat_class;
    
    typedef struct _ofeliaEditFloat
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaEditFloat;
    
    /* ofDoesFloatNameExist object class */
    static t_class *ofeliaDoesFloatNameExist_class;
    
    typedef struct _ofeliaDoesFloatNameExist
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaDoesFloatNameExist;

    /* ofGetFloat object class */
    static t_class *ofeliaGetFloat_class;
    
    typedef struct _ofeliaGetFloat
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetFloat;
    
    /* ofGetFloats object class */
    static t_class *ofeliaGetFloats_class;
    
    typedef struct _ofeliaGetFloats
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetFloats;
    
    /* ofGetFloatAverage object class */
    static t_class *ofeliaGetFloatAverage_class;
    
    typedef struct _ofeliaGetFloatAverage
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetFloatAverage;
    
    /* setup method */
    void ofeliaFloat_setup();
}

#endif /* ofeliaFloat_h */
