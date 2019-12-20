#pragma once

#include "m_pd.h"
#include "g_canvas.h"
#include "lua.hpp"
#include <string>

class ofxOfeliaData;
class ofxOfeliaLua
{
public:
    ofxOfeliaLua(ofxOfeliaData *dataPtr)
    :isChunkRun(false)
    ,dataPtr(dataPtr){};
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
    void outletRet(int nret);
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
    static void unpackModule(lua_State *L, const std::string &moduleName,  const std::string &prefix);
    static bool addGlobals(lua_State *L);
    ofxOfeliaData *dataPtr; /* pointer to data */
};
