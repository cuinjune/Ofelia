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

#ifndef ofeliaHead_h
#define ofeliaHead_h

#pragma once

#include "ofeliaBase.h"

extern "C" {
    
    /* ofHead object class */
    static t_class *ofeliaHead_class;
    
    typedef struct _ofeliaHeadElem {
        
        t_float order;
        bool rendering;
        
    } t_ofeliaHeadElem;
    
    typedef struct _ofeliaHead
    {
        t_object x_obj;
        static const char *objName;
        static unsigned int counter;
        t_ofeliaHeadElem elem;
        static vector<pair<t_float, unsigned int>> vec; /* order, id */
        unsigned int objID;
        
    } t_ofeliaHead;
    
    /* setup method */
    void ofeliaHead_setup();
}

#endif /* ofeliaHead_h */
