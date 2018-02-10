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

#include "ofeliaExpr.h"

/* ________________________________________________________________________________
 * common methods
 */
int findVarPosFromName(const vector<t_ofeliaVarData> &var, const t_string &name) {
    
    auto it = std::find_if(begin(var), end(var),
                           [&name](const t_ofeliaVarData& data){return data.name == name;});
    if (it == end(var))
        return -1;
    return static_cast<int>(it - begin(var));
}

int findVarPosFromNum(const vector<t_ofeliaVarData> &var, const int num) {
    
    auto it = std::find_if(begin(var), end(var),
                           [&num](const t_ofeliaVarData& data){return data.num == num;});
    if (it == end(var))
        return -1;
    return static_cast<int>(it - begin(var));
}

/* ________________________________________________________________________________
 * ofExpr object methods
 */
bool ofeliaExpr_init(t_ofeliaExpr *x, const int argc, const t_atom *argv, const char *objName)
{
    vector<t_string> eqStr;
    stringstream ss;
    const char space = ' ';
    x->numInlets = 1; /* default number of inlets */
    x->numOutlets = 0;
    x->inletVars.push_back({"$i1", 0.0f, 1}); /* add default inlet variable */
    
    if (argc) {
        
        /* get string equations from arguments */
        for (int i=0; i<argc; ++i) {
            
            if (argv[i].a_type == A_SYMBOL) {
                
                if (argv[i].a_w.w_symbol->s_name[0] == ';') {
                    
                    if (!ss.str().empty()) {
                        
                        t_string str = ss.str().c_str();
                        str.pop_back();
                        eqStr.push_back(str.c_str());
                        ss.str("");
                    }
                }
                else {
                    
                    ss << argv[i].a_w.w_symbol->s_name;
                    ss << space;
                }
            }
            else if (argv[i].a_type == A_FLOAT) {
                
                ss << argv[i].a_w.w_float;
                ss << space;
            }
            else {
                
                error("%s: wrong argument type", objName);
                return 0;
            }
        }
        if (!ss.str().empty()) {
            
            t_string str = ss.str().c_str();
            str.pop_back();
            eqStr.push_back(str.c_str());
        }
    }
    else {
        
        eqStr.push_back("$i1 = 0"); /* default equation if there's no arguments */
    }
    /* process all string equations */
    for (size_t i=0; i<eqStr.size(); ++i) {
        
        t_ofeliaEq eq;
        const int firstSpace = eqStr[i].find(space);
        
        /*  detect if it is an equation format (result = expression) */
        if (firstSpace != t_string::npos &&
            eqStr[i].length() > firstSpace + 2 &&
            eqStr[i].substr(firstSpace, 3) == " = ") {
            
            /* split the equation into result and expression */
            eq.resultStr = eqStr[i].substr(0, firstSpace);
            eq.exprStr = eqStr[i].substr(firstSpace + 3);
        }
        else {
            
            /* if it is not an equation, make it one */
            if (!x->isDefine) { /* if it is ofExpr object, default is expression */
                
                eq.resultStr = t_string("$o") += ofToString(x->numOutlets+1).c_str();
                eq.exprStr = eqStr[i];
            }
            else { /* if it is ofDefine object, default is result */
            
                /* check if the result variable name contains any space */
                if (firstSpace != t_string::npos) {
                    
                    error("%s: variable name cannot contain any space", objName);
                    return 0;
                }
                eq.resultStr = eqStr[i];
                eq.exprStr = "0";
            }
        }
        /* check if the result variable name is valid */
        t_string &resultVar = eq.resultStr;
        const int resultVarLen = resultVar.length();
        
        if ((resultVar[0] >= '"' && resultVar[0] <= '$') || resultVar[0] == ':' ||
            (resultVar[0] >= '?' && resultVar[0] <= 'Z') ||
            (resultVar[0] >= '_' && resultVar[0] <= 'z') || resultVar[0] == '~') {
            
            for (int j=1; j<resultVarLen; ++j) {
                
                if (!(resultVar[j] >= '"' && resultVar[j] <= '$') &&
                    !(resultVar[j] >= '0' && resultVar[j] <= ':') &&
                    !(resultVar[j] >= '?' && resultVar[j] <= 'Z') &&
                    !(resultVar[j] >= '_' && resultVar[j] <= 'z') &&
                    !(resultVar[j] == '~')) {
                    
                    error("%s: invalid variable name '%s'", objName, resultVar.c_str());
                    return 0;
                }
            }
        }
        else {
            
            error("%s: invalid variable name '%s'", objName, resultVar.c_str());
            return 0;
        }
        /* iterate through the expression by characters to get variable names */
        t_string &exprStr = eq.exprStr;
        const int exprStrLen = exprStr.length();
        
        for (int j=0; j<exprStrLen; ++j) {
            
            /* detect beginning character of the variable name (can't start with numbers or operators) */
            if ((exprStr[j] >= '"' && exprStr[j] <= '$') || exprStr[j] == ':' ||
                (exprStr[j] >= '?' && exprStr[j] <= 'Z') ||
                (exprStr[j] >= '_' && exprStr[j] <= 'z') || exprStr[j] == '~') {
                
                t_string exprVar; /* expression variable name buffer */
                
                while ((exprStr[j] >= '"' && exprStr[j] <= '$') ||
                       (exprStr[j] >= '0' && exprStr[j] <= ':') ||
                       (exprStr[j] >= '?' && exprStr[j] <= 'Z') ||
                       (exprStr[j] >= '_' && exprStr[j] <= 'z') ||
                        exprStr[j] == '~') {
                    
                    /* append characters to the variable name buffer */
                    exprVar += exprStr[j];
                    
                    /* exit the while loop if it reaches the end of the expression */
                    if (++j == exprStrLen) {
                        
                        --j; /* make it fit within the length */
                        break;
                    }
                }
                /* make sure it's not a function */
                if (exprStr[j] != '(' && !(exprStr[j] == ' ' && exprStr[j+1] == '(')) {
                    
                    switch (exprVar[0]) { /* detect expression variable type */
                            
                        case '$': /* inlet and outlet variables */
                        {
                            if (exprVar.length() < 3) {
                                
                                error("%s: wrong use of '$' variable", objName);
                                return 0;
                            }
                            const t_string &str = exprVar.substr(2);
                            
                            if (!isNumeric(str.c_str())) {
                                
                                error("%s: wrong use of '$' variable", objName);
                                return 0;
                            }
                            switch (exprVar[1]) {
                                    
                                case 'i':
                                {
                                    const int inletNum = ofToInt(str.c_str());
                                    
                                    if (inletNum < 1 || inletNum > 64) {
                                        
                                        error("%s: invalid number of inlets", objName);
                                        return 0;
                                    }
                                    if (inletNum > x->numInlets)
                                        x->numInlets = inletNum;
                                    
                                    /* add the inlet variable to vector if it doesn't exist yet */
                                    const int pos = findVarPosFromName(x->inletVars, exprVar);
                                    
                                    if (pos == -1) {
                                        
                                        eq.exprVarInfos.push_back({EQ_VAR_INLET, x->inletVars.size()});
                                        x->inletVars.push_back({exprVar, 0.0f, inletNum});
                                    }
                                    else {
                                        
                                        eq.exprVarInfos.push_back({EQ_VAR_INLET, static_cast<size_t>(pos)});
                                    }
                                    break;
                                }
                                case 'o':
                                {
                                    /* outlet variable should already exist in the vector */
                                    const int pos = findVarPosFromName(x->outletVars, exprVar);
                                    
                                    if (pos == -1) {
                                        
                                        error("%s: outlet '%s' is not predefined", objName, exprVar.c_str());
                                        return 0;
                                    }
                                    else {
                                        
                                        eq.exprVarInfos.push_back({EQ_VAR_OUTLET, static_cast<size_t>(pos)});
                                    }
                                    break;
                                }
                                default:
                                {
                                    error("%s: wrong use of '$' variable", objName);
                                    return 0;
                                }
                            }
                            break;
                        }
                        case '_': /* private variables */
                        {
                            if (exprVar.length() < 2) {
                                
                                error("%s: private variable name is missing after '_'", objName);
                                return 0;
                            }
                            /* private variable should already exist in the vector if not it's error */
                            const int pos = findVarPosFromName(x->privateVars, exprVar);
                            
                            if (pos == -1) {
                                
                                error("%s: private variable '%s' is not predefined", objName, exprVar.c_str());
                                return 0;
                            }
                            else {
                                
                                eq.exprVarInfos.push_back({EQ_VAR_PRIVATE, static_cast<size_t>(pos)});
                            }
                            break;
                        }
                        default: /* public variables */
                        {
                            /* add the public variable to vector if it doesn't exist yet */
                            const int pos = findVarPosFromName(x->publicVars, exprVar);
                            
                            if (pos == -1) {
                                
                                eq.exprVarInfos.push_back({EQ_VAR_PUBLIC, x->publicVars.size()});
                                x->publicVars.push_back({exprVar, 0.0f, 0});
                            }
                            else {
                                
                                eq.exprVarInfos.push_back({EQ_VAR_PUBLIC, static_cast<size_t>(pos)});
                            }
                            break;
                        }
                    }
                }
            }
        }
        switch (resultVar[0]) { /* detect result variable type */
                
            case '$': /* inlet and outlet variables */
            {
                if (resultVar.length() < 3) {
                    
                    error("%s: wrong use of '$' variable", objName);
                    return 0;
                }
                const t_string &str = resultVar.substr(2);
                
                if (!isNumeric(str.c_str())) {
                    
                    error("%s: wrong use of '$' variable", objName);
                    return 0;
                }
                switch (resultVar[1]) {
                        
                    case 'i':
                    {
                        const int inletNum = ofToInt(str.c_str());
                        
                        if (inletNum < 1 || inletNum > 64) {
                            
                            error("%s: invalid number of inlets", objName);
                            return 0;
                        }
                        if (inletNum > x->numInlets)
                            x->numInlets = inletNum;
                        
                        /* add the inlet variable to vector if it doesn't exist yet */
                        const int pos = findVarPosFromName(x->inletVars, resultVar);
                        
                        if (pos == -1) {
                            
                            eq.resultVarInfo = {EQ_VAR_INLET, x->inletVars.size()};
                            x->inletVars.push_back({resultVar, 0.0f, inletNum});
                        }
                        else {
                            
                            eq.resultVarInfo = {EQ_VAR_INLET, static_cast<size_t>(pos)};
                        }
                        break;
                    }
                    case 'o':
                    {
                        const int outletNum = ofToInt(str.c_str());
                        
                        if (outletNum < 1 || outletNum > 64) {
                            
                            error("%s: invalid number of outlets", objName);
                            return 0;
                        }
                        if (outletNum > x->numOutlets)
                            x->numOutlets = outletNum;
                        
                        /* add the outlet variable to vector if it doesn't exist yet */
                        const int pos = findVarPosFromName(x->outletVars, resultVar);
                        
                        if (pos == -1) {
                            
                            eq.resultVarInfo = {EQ_VAR_OUTLET, x->outletVars.size()};
                            x->outletVars.push_back({resultVar, 0.0f, outletNum});
                        }
                        else {
                            
                            eq.resultVarInfo = {EQ_VAR_OUTLET, static_cast<size_t>(pos)};
                        }
                        break;
                    }
                    default:
                    {
                        error("%s: wrong use of '$' variable", objName);
                        return 0;
                    }
                }
                break;
            }
            case '_': /* private variables */
            {
                if (resultVar.length() < 2) {
                    
                    error("%s: private variable name is missing after '_'", objName);
                    return 0;
                }
                /* add the private variable to vector if it doesn't exist yet */
                const int pos = findVarPosFromName(x->privateVars, resultVar);
                
                if (pos == -1) {
                    
                    eq.resultVarInfo = {EQ_VAR_PRIVATE, x->privateVars.size()};
                    x->privateVars.push_back({resultVar, 0.0f, 0});
                }
                else {
                    
                    eq.resultVarInfo = {EQ_VAR_PRIVATE, static_cast<size_t>(pos)};
                }
                break;
            }
            default: /* public variables */
            {
                /* add the public variable to vector if it doesn't exist yet */
                const int pos = findVarPosFromName(x->publicVars, resultVar);
                
                if (pos == -1) {
                    
                    eq.resultVarInfo = {EQ_VAR_PUBLIC, x->publicVars.size()};
                    x->publicVars.push_back({resultVar, 0.0f, 0});
                }
                else {
                    
                    eq.resultVarInfo = {EQ_VAR_PUBLIC, static_cast<size_t>(pos)};
                }
                break;
            }
        }
        x->eqs.push_back(eq);
    }
    /* bind all variables used in expressions and compile */
    for (size_t i=0; i<x->eqs.size(); ++i) {
        
        for (size_t j=0; j<x->eqs[i].exprVarInfos.size(); ++j) {
            
            const size_t index = x->eqs[i].exprVarInfos[j].index;
            
            switch (x->eqs[i].exprVarInfos[j].type) {
                    
                case EQ_VAR_INLET:
                    x->eqs[i].exprVars.push_back({x->inletVars[index].name.c_str(), &x->inletVars[index].value});
                    break;
                case EQ_VAR_OUTLET:
                    x->eqs[i].exprVars.push_back({x->outletVars[index].name.c_str(), &x->outletVars[index].value});
                    break;
                case EQ_VAR_PRIVATE:
                    x->eqs[i].exprVars.push_back({x->privateVars[index].name.c_str(), &x->privateVars[index].value});
                    break;
                case EQ_VAR_PUBLIC:
                    x->eqs[i].exprVars.push_back({x->publicVars[index].name.c_str(), &x->publicVars[index].value});
                    break;
                default:
                    break;
            }
        }
        int err;
        x->eqs[i].expr = te_compile(x->eqs[i].exprStr.c_str(), x->eqs[i].exprVars.data(),
                                    static_cast<int>(x->eqs[i].exprVars.size()), &err);
        
        if (!x->eqs[i].expr) {
            
            /* show the user where the error is found */
            error("%s: syntax error in '%s' at %d", objName, x->eqs[i].exprStr.c_str(), err-1);
            te_free(x->eqs[i].expr);
            return 0;
        }
    }
    /* detect local variables and replace symbol */
    if (!x->publicVars.empty()) {
        
        t_ofeliaVarName varName;
        getVarNameLocalPrefixes(varName);
        
        for (size_t i=0; i<x->publicVars.size(); ++i) {
            
            if (x->publicVars[i].name[0] == '@') /* patch local variable */
                x->publicVars[i].name.replace(0, 1, varName.patchPrefix);
            else if (x->publicVars[i].name[0] == '#') /* canvas local variable */
                x->publicVars[i].name.replace(0, 1, varName.canvasPrefix);
        }
        if (x->isDefine) {
            
            for (size_t i=0; i<x->publicVars.size(); ++i)
                value_get(gensym(x->publicVars[i].name.c_str()));
        }
    }
    /* add inlets */
    for (int i=1; i<=x->numInlets; ++i) {
        
        const int pos = findVarPosFromNum(x->inletVars, i);
        
        if (i != 1) /* skip the first one */ {
            
            if (pos == -1)
                floatinlet_new(&x->x_obj, &x->missingInletValue);
            else
                floatinlet_new(&x->x_obj, &x->inletVars[pos].value);
        }
        else {
            
            x->firstInletIndex = pos;
        }
    }
    /* add outlets */
    for (int i=1; i<=x->numOutlets; ++i) {
        
        const int pos = findVarPosFromNum(x->outletVars, i);
        
        if (pos == -1) {
            
            error("%s: cannot find the outlet '$o%d'", objName, i);
            return 0;
        }
        x->outletIndices.push_back(static_cast<size_t>(pos));
        x->result_outs.push_back(outlet_new(&x->x_obj, &s_float));
    }
    return 1;
}

void *ofeliaExpr_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaExpr *x = reinterpret_cast<t_ofeliaExpr*>(pd_new(ofeliaExpr_class));
    x->isDefine = false;
    
    if (!ofeliaExpr_init(x, argc, argv, "ofExpr"))
        return 0;
    return (x);
}

void *ofeliaDefine_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaExpr *x = reinterpret_cast<t_ofeliaExpr*>(pd_new(ofeliaExpr_class));
    x->isDefine = true;
    
    if (!ofeliaExpr_init(x, argc, argv, "ofDefine"))
        return 0;
    return (x);
}

void ofeliaExpr_bang(t_ofeliaExpr *x)
{
    /* check public variables' existance and get current values */
    for (size_t i=0; i<x->publicVars.size(); ++i) {
        
        t_float value;
        
        if (value_getfloat(gensym(x->publicVars[i].name.c_str()), &value)) {
            
            if (!x->isDefine)
                error("ofExpr: variable '%s' is not defined", x->publicVars[i].name.c_str());
            else
                error("ofDefine: variable '%s' is not defined", x->publicVars[i].name.c_str());
            value = 0.0f;
        }
        x->publicVars[i].value = value;
    }
    /* compile expressions and update result variables */
    for (size_t i=0; i<x->eqs.size(); ++i) {
    
        const size_t index = x->eqs[i].resultVarInfo.index;
        const float result = te_eval(x->eqs[i].expr);
        
        /* update result variables */
        switch (x->eqs[i].resultVarInfo.type) {
                
            case EQ_VAR_INLET:
                x->inletVars[index].value = result;
                break;
            case EQ_VAR_OUTLET:
                x->outletVars[index].value = result;
                break;
            case EQ_VAR_PRIVATE:
                x->privateVars[index].value = result;
                break;
            case EQ_VAR_PUBLIC:
                x->publicVars[index].value = result;
                value_setfloat(gensym(x->publicVars[index].name.c_str()), result);
                break;
            default:
                break;
        }
    }
    /* output result varibles from right to left order */
    for (size_t i=x->numOutlets; i-- > 0;) {
        
        const size_t index = x->outletIndices[i];
        outlet_float(x->result_outs[i], x->outletVars[index].value);
    }
}

void ofeliaExpr_float(t_ofeliaExpr *x, t_floatarg f)
{
    if (!x->inletVars.empty())
        x->inletVars[x->firstInletIndex].value = f;
    ofeliaExpr_bang(x);
}

void ofeliaExpr_free(t_ofeliaExpr *x)
{
    if (x->isDefine) {
        
        for (size_t i=0; i<x->publicVars.size(); ++i)
            value_release(gensym(x->publicVars[i].name.c_str()));
    }
    for (size_t i=0; i<x->eqs.size(); ++i)
        te_free(x->eqs[i].expr);
    for (size_t i=0; i<x->result_outs.size(); ++i)
        outlet_free(x->result_outs[i]);
}

void ofeliaExpr_setup()
{
    ofeliaExpr_class = class_new(gensym("ofExpr"),
                                 reinterpret_cast<t_newmethod>(ofeliaExpr_new),
                                 reinterpret_cast<t_method>(ofeliaExpr_free),
                                 sizeof(t_ofeliaExpr),
                                 CLASS_DEFAULT, A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaDefine_new),
                     gensym("ofDefine"), A_GIMME, 0);
    class_addbang(ofeliaExpr_class, reinterpret_cast<t_method>(ofeliaExpr_bang));
    class_addfloat(ofeliaExpr_class, reinterpret_cast<t_method>(ofeliaExpr_float));
}

