#include "ofxOfeliaSignal.h"
#include "ofxOfeliaData.h"

void ofxOfeliaSignal::addDsp(t_signal **sp)
{
    dataPtr->lua.doFunction(gensym("dsp"));
    const ofxOfeliaIO &io = dataPtr->io;
    int sum = io.numInlets + io.numOutlets;
    t_int **w = this->w;
    w[0] = reinterpret_cast<t_int *>(dataPtr);
    w[1] = reinterpret_cast<t_int *>(sp[0]->s_n);
    for (int i = 0; i < sum; i++)
        w[i + 2] = reinterpret_cast<t_int *>(sp[i]->s_vec);
    dsp_addv(perform, sum + 2, reinterpret_cast<t_int *>(w));
}

t_int *ofxOfeliaSignal::perform(t_int *w)
{
    ofxOfeliaData *x = reinterpret_cast<ofxOfeliaData *>(w[1]);
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
        for (int i = numOutlets - 1; i >= 0; --i)
        {
            t_float *out = reinterpret_cast<t_float *>(w[i + 3 + numInlets]);
            for (int j = 0; j < nsamples; ++j)
            {
                lua_pushinteger(L, static_cast<lua_Integer>(j + 1));
                lua_gettable(L, -2);
                if (lua_isnumber(L, -1))
                    out[j] = static_cast<t_float>(lua_tonumber(L, -1));
                else if (lua_isboolean(L, -1))
                    out[j] = static_cast<t_float>(lua_toboolean(L, -1));
                else
                    out[j] = 0.0f;
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
