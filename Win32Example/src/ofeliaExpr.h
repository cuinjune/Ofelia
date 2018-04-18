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

/*==============================================================================
	[ofExpr] object makes use of Lewis Van Winkle's tinyexpr
	which is free open source software, licensed under the zlib License.
 
	Copyright (C) 2015, 2016 Lewis Van Winkle
 
	This software is provided 'as-is', without any express or implied
	warranty. In no event will the authors be held liable for any damages
	arising from the use of this software.
 
	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:
 
	1. The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software
	in a product, an acknowledgement in the product documentation would be
	appreciated but is not required.
	2. Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.
	3. This notice may not be removed or altered from any source distribution.
 
	See https://github.com/codeplea/tinyexpr for documentation
 ==============================================================================*/

#ifndef ofExpr_h
#define ofExpr_h

#pragma once

#include "ofeliaBase.h"
#include "tinyexpr.h"

extern "C" {
    
    /* ofExpr object class */
    static t_class *ofeliaExpr_class;
    
    enum EqVarType {
        
        EQ_VAR_INLET,
        EQ_VAR_OUTLET,
        EQ_VAR_PRIVATE,
        EQ_VAR_PUBLIC /* this includes global and local variables */
    };
    typedef struct _ofeliaVarData {
        
        t_string name;
        float value;
        int num; /* used for inlet or outlet number */
        
    } t_ofeliaVarData;
    
    typedef struct _ofeliaVarInfo { /* used to access variable data from vectors */
        
        EqVarType type;
        size_t index;
        
    } t_ofeliaVarInfo;
    
    typedef struct _ofeliaEq { /* equation */
   
        t_string resultStr, exprStr;
        t_ofeliaVarInfo resultVarInfo;
        vector<t_ofeliaVarInfo> exprVarInfos;
        vector<te_variable> exprVars;
        te_expr *expr;
        
    } t_ofeliaEq;
    
    typedef struct _ofeliaExpr
    {
        t_object x_obj;
        bool isDefine;
        vector<t_ofeliaEq> eqs;
        vector<t_ofeliaVarData> inletVars, outletVars, privateVars, publicVars;
        int numInlets, numOutlets;
        size_t firstInletIndex;
        vector<size_t> outletIndices;
        vector<t_outlet*> result_outs;
        t_float missingInletValue;
        
    } t_ofeliaExpr;
    
    /* setup methods */
    void ofeliaExpr_setup();
}



#endif /* ofeliaExpr_h */
