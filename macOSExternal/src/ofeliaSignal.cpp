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

#include "ofeliaSignal.h"
#include "ofeliaData.h"

void ofeliaSignal::addDsp(t_signal **sp)
{
    dataPtr->lua.doFunction(gensym("dsp"));
    const ofeliaIO &io = dataPtr->io;
    int sum = io.numInlets + io.numOutlets;
    t_int **w = this->w;
    w[0] = reinterpret_cast<t_int *>(dataPtr);
    w[1] = reinterpret_cast<t_int *>(sp[0]->s_n);
    for (int i = 0; i < sum; i++)
        w[i + 2] = reinterpret_cast<t_int *>(sp[i]->s_vec);
    dsp_addv(perform, sum + 2, reinterpret_cast<t_int *>(w));
}

t_int *ofeliaSignal::perform(t_int *w)
{
    ofeliaData *x = reinterpret_cast<ofeliaData *>(w[1]);
    const int nsamples = static_cast<int>(w[2]);
    lua_State *L = x->lua.L;
    const int numInlets = x->io.numInlets;
    const int numOutlets = x->io.numOutlets;
    int top; if (x->lua.isFunction(gensym("perform"), top))
    {
        for (int i = 0; i < numInlets; ++i)
        {
            lua_newtable(L);
            t_float *in = reinterpret_cast<t_float *>(w[i + 3]);
            for (int j = 0; j < nsamples; ++j)
            {
                lua_pushinteger(L, static_cast<lua_Integer>(j + 1));
                lua_pushnumber(L, static_cast<lua_Number>(in[j]));
                lua_settable(L, -3);
            }
        }
        if (lua_pcall(L, numInlets, numOutlets, 0))
        {
            error("ofelia: %s", lua_tostring(L, -1));
            lua_pop(L, 2);
            goto error;
        }
        if (!lua_istable(L, -1))
        {
            const char *s = "ofelia: 'perform' function should return";
            if (numOutlets == 1)
                error("%s %s", s, "a table");
            else if (numOutlets > 1)
                error("%s %d %s", s, numOutlets, "tables");
            lua_pop(L, 1 + numOutlets);
            goto error;
        }
        for (int i = 0; i < numOutlets; ++i)
        {
            t_float *out = reinterpret_cast<t_float *>(w[i + 3 + numInlets]);
            const int index = -numOutlets - 1 + i;
            for (int j = 0; j < nsamples; ++j)
            {
                lua_pushinteger(L, static_cast<lua_Integer>(j + 1));
                lua_gettable(L, index);
                out[j] = static_cast<t_float>(lua_tonumber(L, -1));
                lua_pop(L, 1);
            }
            lua_pop(L, 1);
        }
        lua_pop(L, 1);
    }
    else
    {
    error: /* silence the audio if something is wrong */
        for (int i = 0; i < numOutlets; ++i)
        {
            t_float *out = reinterpret_cast<t_float *>(w[i + 3 + numInlets]);
            for (int j = 0; j < nsamples; ++j) out[j] = 0;
        }
    }
    return w + numInlets + numOutlets + 3;
}

