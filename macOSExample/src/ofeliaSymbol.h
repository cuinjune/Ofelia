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

#ifndef ofeliaSymbol_h
#define ofeliaSymbol_h

#include "ofeliaBase.h"

extern "C" {
    
    /* ofLoadSymbol object class */
    static t_class *ofeliaLoadSymbol_class;
    
    typedef struct _ofeliaLoadVecSymData {
        
        unsigned int objID;
        t_ofeliaVarName varName;
        vector<t_string> vecSymbols;
        
    } t_ofeliaLoadSymbolData;
    
    typedef struct _ofeliaLoadSymbolCmdData {
        
        t_string vecSymbol;
        t_float fromIndex, toIndex;
        
    } t_ofeliaLoadSymbolCmdData;
    
    typedef struct _ofeliaLoadSymbol
    {
        t_object x_obj;
        static const char *objName;
        static unsigned int counter;
        static vector<t_ofeliaLoadSymbolData> vecData;
        unsigned int objID;
        int vecSize;
        t_clock *vecSizeOutClock;
        
    } t_ofeliaLoadSymbol;
    
    /* ofEditSymbol object class */
    static t_class *ofeliaEditSymbol_class;
    
    typedef struct _ofeliaEditSymbol
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaEditSymbol;
    
    /* ofDoesSymbolNameExist object class */
    static t_class *ofeliaDoesSymbolNameExist_class;
    
    typedef struct _ofeliaDoesSymbolNameExist
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaDoesSymbolNameExist;
    
    /* ofGetSymbol object class */
    static t_class *ofeliaGetSymbol_class;
    
    typedef struct _ofeliaGetSymbol
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetSymbol;
    
    /* ofGetSymbols object class */
    static t_class *ofeliaGetSymbols_class;
    
    typedef struct _ofeliaGetSymbols
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetSymbols;
    
    /* setup method */
    void ofeliaSymbol_setup();
}

#endif /* ofeliaSymbol_h */
