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

#ifndef ofeliaLua_h
#define ofeliaLua_h

#pragma once

#include "m_pd.h"
#include "g_canvas.h"
#include "lua.hpp"

#define OFELIA_USERDATAREF_SIZE 1000 /* maximum table size that can handle userdata */

extern "C"
{
    class ofeliaData;
    class ofeliaLua
    {
    public:
        ofeliaLua(ofeliaData *dataPtr)
        :dataPtr(dataPtr)
        ,isChunkRun(false){};
        ~ofeliaLua(){};
        static bool init();
        bool require();
        int  getType(t_symbol *s);
        int  getType(t_symbol *s, int &top);
        bool isNil(t_symbol *s);
        bool isBoolean(t_symbol *s);
        bool isNumber(t_symbol *s);
        bool isString(t_symbol *s);
        bool isUserData(t_symbol *s);
        bool isTable(t_symbol *s);
        bool isFunction(t_symbol *s, int &top);
        void pushUserData(t_gpointer *p);
        void setVariable(t_symbol *s);
        void setVariable(t_symbol *s, bool b);
        void setVariable(t_symbol *s, t_floatarg f);
        void setVariable(t_symbol *s, t_symbol *s2);
        void setVariable(t_symbol *s, t_gpointer *p);
        void setVariable(t_symbol *s, int argc, t_atom *argv);
        void doVariable(t_symbol *s);
        void doVariable(t_symbol *s, bool b);
        void doVariable(t_symbol *s, t_floatarg f);
        void doVariable(t_symbol *s, t_symbol *s2);
        void doVariable(t_symbol *s, t_gpointer *p);
        void doVariable(t_symbol *s, int argc, t_atom *argv);
        void outletUserData();
        void outletTable();
        void outletRet();
        void callFunction(int top);
        void setFunction(int top);
        void setFunction(t_floatarg f, int top);
        void setFunction(t_symbol *s, int top);
        void setFunction(t_gpointer *p, int top);
        void setFunction(int argc, t_atom *argv,int top);
        void doFunction(t_symbol *s);
        void doFunction(t_symbol *s, t_floatarg f);
        void doFunction(t_symbol *s, t_symbol *s2);
        void doFunction(t_symbol *s, t_gpointer *p);
        void doFunction(t_symbol *s, int argc, t_atom *argv);
        void doNewFunction();
        void doFreeFunction();
        void realizeDollar(char **bufp, int *lengthp);
        void doString(const char *s);
        void doArgs(int argc, t_atom *argv);
        void doText();
        void getVariableByArgs(t_symbol *s, int argc, t_atom *argv);
        void setVariableByArgs(t_symbol *s, int argc, t_atom *argv);
        static lua_State *L;
        bool isChunkRun; /* whether the chunk is run or not */
    private:
        ofeliaData *dataPtr; /* pointer to data */
    };
}

#endif /* ofeliaLua_h */
