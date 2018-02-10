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

#ifndef ofeliaSeparators_h
#define ofeliaSeparators_h

#pragma once

#include "ofeliaBase.h"

extern "C" {
    
    /* ofSepStyle object class */
    static t_class *ofeliaSepStyle_class;
    
    typedef struct _ofeliaSepStyle
    {
        t_object x_obj;
        static const char *objName;
        vector<t_outlet*> sep_outs;
        
    } t_ofeliaSepStyle;
    
    /* ofSepMatrix object class */
    static t_class *ofeliaSepMatrix_class;
    
    typedef struct _ofeliaSepMatrix
    {
        t_object x_obj;
        static const char *objName;
        vector<t_outlet*> sep_outs;
        
    } t_ofeliaSepMatrix;
    
    /* ofSeparator object class */
    static t_class *ofeliaSeparator_class;
    
    typedef struct _ofeliaSeparator
    {
        t_object x_obj;
        static const char *objName;
        vector<t_outlet*> sep_outs;
        
    } t_ofeliaSeparator;
    
    /* setup methods */
    void ofeliaSeparators_setup();
}

#endif /* ofeliaSeparators_h */
