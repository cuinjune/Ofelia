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

#ifndef ofeliaColor_h
#define ofeliaColor_h

#include "ofeliaBase.h"

extern "C" {
    
    /* ofLoadColor object class */
    static t_class *ofeliaLoadColor_class;
    
    typedef struct _ofeliaLoadColorData {
        
        unsigned int objID;
        t_ofeliaVarName varName;
        vector<ofColor> colors;
        
    } t_ofeliaLoadColorData;
    
    typedef struct _ofeliaLoadColorCmdData {
        
        ofColor color;
        t_float fromIndex, toIndex;
        
    } t_ofeliaLoadColorCmdData;
    
    typedef struct _ofeliaLoadColor
    {
        t_object x_obj;
        static const char *objName;
        static unsigned int counter;
        static vector<t_ofeliaLoadColorData> colorData;
        unsigned int objID;
        int vecSize;
        t_clock *vecSizeOutClock;
        
    } t_ofeliaLoadColor;
    
    /* ofEditColor object class */
    static t_class *ofeliaEditColor_class;
    
    typedef struct _ofeliaEditColor
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaEditColor;
    
    /* ofDoesColorNameExist object class */
    static t_class *ofeliaDoesColorNameExist_class;
    
    typedef struct _ofeliaDoesColorNameExist
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaDoesColorNameExist;
    
    /* ofGetColor object class */
    static t_class *ofeliaGetColor_class;
    
    typedef struct _ofeliaGetColor
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetColor;
    
    /* ofGetColors object class */
    static t_class *ofeliaGetColors_class;
    
    typedef struct _ofeliaGetColors
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetColors;
    
    /* setup method */
    void ofeliaColor_setup();
}

#endif /* ofeliaColor_h */
