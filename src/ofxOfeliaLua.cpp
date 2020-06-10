#include "ofxOfeliaLua.h"
#include "ofxOfeliaGL.h"
#include "ofxOfeliaData.h"
#include <algorithm>
#include <cctype>
#include <deque>
#include <cstring>
#include <sstream>

/*
 calling ofxOfeliaLua::doString(const char *s) function
 is like running the following script in Lua:
 
 * [ofelia define] object:
 
 local name = "foo" -- module name set as ofelia object argument
 package.preload[name] = nil
 package.loaded[name] = nil
 package.preload[name] = function(this)
 local ofelia = {}
 local M = ofelia
 s -- a written script in pd
 return M
 end
 
 * [ofelia function] object:
 
 local name = "foo" -- module name set as ofelia object argument
 package.preload[name] = nil
 package.loaded[name] = nil
 package.preload[name] = function(this)
 local ofelia = {}
 local M = ofelia
 function M.bang()
 return M.anything(nil)
 end
 function M.float(f)
 return M.anything(f)
 end
 function M.symbol(s)
 return M.anything(s)
 end
 function M.pointer(p)
 return M.anything(p)
 end
 function M.list(l)
 return M.anything(l)
 end
 function M.anything(a)
 s -- a written script in pd
 end
 return M
 end
 
 sending 'bang' message to [ofelia define] or [ofelia function] object
 calls ofxOfeliaLua::doFunction(t_symbol *s) function and it is like running
 the following script in Lua:
 
 local m = require("foo") -- module name set as ofelia object argument
 if type(m.bang) == "function" then
 return m:bang() -- ofelia object will output the returned value through its outlet
 end
 */

lua_State *ofxOfeliaLua::L;

extern "C"
{
    int luaopen_of(lua_State *L);
    int luaopen_pd(lua_State *L);
    int luaopen_print(lua_State *L);
}

/* redefined print function for the pd window */
int l_my_print(lua_State *L)
{
    int argc = lua_gettop(L);
    if (argc) startpost(luaL_tolstring(L, 1, nullptr));
    for (int i = 2; i <= argc; ++i)
    {
        poststring(luaL_tolstring(L, i, nullptr));
        lua_pop(L, 1); /* remove the string */
    }
    endpost();
    return 0;
}

const struct luaL_Reg printlib[] =
{
    {"print", l_my_print},
    {NULL, NULL} /* end of array */
};

int luaopen_print(lua_State *L)
{
    lua_getglobal(L, "_G");
#if LUA_VERSION_NUM < 502
    luaL_register(L, NULL, printlib);
#else
    luaL_setfuncs(L, printlib, 0);
#endif
    lua_pop(L, 1);
    return 1;
}

void ofxOfeliaLua::unpackModule(lua_State *L, const std::string &moduleName, const std::string &prefix)
{
    std::string upperPrefix = prefix;
    std::transform(upperPrefix.begin(), upperPrefix.end(),upperPrefix.begin(), ::toupper);
    upperPrefix += "_"; /* prefix for constants and enums */
    lua_getglobal(L, moduleName.c_str());
    lua_pushnil(L);
    while (lua_next(L, -2) != 0)
    {
        const std::string &type = luaL_typename(L, -1);
        const std::string &str = lua_tostring(L, -2);
        std::string renamedStr;
        lua_getfield(L, -3, str.c_str());
        if ((type == "table" || type == "userdata") && (::isupper(str[0]) || ::isdigit(str[0]))) /* classes and structs */
            renamedStr = prefix + str;
        else if (type == "function") /* global functions */
        {
            renamedStr = str;
            renamedStr[0] = static_cast<char>(::toupper(str[0]));
            renamedStr = prefix + renamedStr;
        }
        else if (type == "number" || type == "string" || type == "boolean")
        {
            if (std::any_of(str.begin(), str.end(), ::islower)) /* static member variables */
                renamedStr = prefix + str;
            else /* constants or enums */
                renamedStr = upperPrefix + str;
        }
        lua_setglobal(L, renamedStr.c_str());
        lua_pop(L, 1);
        lua_pushnil(L); /* assign nil to original element */
        lua_setfield(L, -3, str.c_str());
    }
    lua_pop(L, 1);
    lua_pushnil(L); /* assign nil to module */
    lua_setglobal(L, moduleName.c_str());
}

bool ofxOfeliaLua::addGlobals(lua_State *L)
{
    lua_settop(L, 0);
    const char *s =
    "ofRequire = require\n"
    "function ofTable(...)\n"
    "  return {...}\n"
    "end\n";
    const int ret = luaL_dostring(L, s);
    if (ret != LUA_OK)
    {
        error("ofelia: %s", lua_tostring(L, -1));
        lua_pop(L, 1);
        return false;
    }
    return true;
}

bool ofxOfeliaLua::init()
{
    /* initialize lua */
    L = luaL_newstate();
    if (L == nullptr)
    {
        error("ofelia: failed initializing lua");
        return false;
    }
    /* open libs */
    luaL_openlibs(L);
    luaopen_of(L);
    luaopen_pd(L);
    luaopen_print(L);
    
    /* clear stack since opening libs leaves tables on the stack */
    lua_settop(L, 0);
    
    /* unpack module elements into global namespace with the new prefix */
    unpackModule(L, "of", "of");
    unpackModule(L, "pd", "of");
    
    /* add GL preprocessor defines */
    ofxOfeliaGL::addDefines(L);
    
    /* add global variables needed */
    if (!addGlobals(L)) return false;
    
    /* make garbage collector run more frequently (default: 200) */
    lua_gc(L, LUA_GCSETPAUSE, 100);
    return true;
}

bool ofxOfeliaLua::require()
{
    if (!isChunkRun) return false;
    lua_getglobal(L, "require");
    lua_pushstring(L, dataPtr->sym->s_name);
    if (lua_pcall(L, 1, LUA_MULTRET, 0))
    {
        error("ofelia: %s", lua_tostring(L, -1));
        lua_pop(L, 1);
        isChunkRun = false;
        return false;
    }
    return true;
}

int ofxOfeliaLua::getType(t_symbol *s)
{
    lua_getfield(L, -1, s->s_name);
    return lua_type(L, -1);
}

int ofxOfeliaLua::getType(t_symbol *s, int &top)
{
    top = lua_gettop(L);
    lua_getfield(L, -1, s->s_name);
    return lua_type(L, -1);
}

bool ofxOfeliaLua::isNil(t_symbol *s)
{
    if (!require())
        return false;
    if (getType(s) != LUA_TNIL)
    {
        lua_pop(L, 2);
        return false;
    }
    return true;
}

bool ofxOfeliaLua::isBoolean(t_symbol *s)
{
    if (!require())
        return false;
    if (getType(s) != LUA_TBOOLEAN)
    {
        lua_pop(L, 2);
        return false;
    }
    return true;
}

bool ofxOfeliaLua::isNumber(t_symbol *s)
{
    if (!require())
        return false;
    if (getType(s) != LUA_TNUMBER)
    {
        lua_pop(L, 2);
        return false;
    }
    return true;
}

bool ofxOfeliaLua::isString(t_symbol *s)
{
    if (!require())
        return false;
    if (getType(s) != LUA_TSTRING)
    {
        lua_pop(L, 2);
        return false;
    }
    return true;
}

bool ofxOfeliaLua::isUserData(t_symbol *s)
{
    if (!require())
        return false;
    if (getType(s) != LUA_TUSERDATA)
    {
        lua_pop(L, 2);
        return false;
    }
    return true;
}

bool ofxOfeliaLua::isTable(t_symbol *s)
{
    if (!require())
        return false;
    if (getType(s) != LUA_TTABLE)
    {
        lua_pop(L, 2);
        return false;
    }
    return true;
}

bool ofxOfeliaLua::isFunction(t_symbol *s, int &top)
{
    if (!require())
        return false;
    if (getType(s, top) != LUA_TFUNCTION)
    {
        lua_pop(L, 2);
        return false;
    }
    return true;
}

void ofxOfeliaLua::pushUserData(t_gpointer *p)
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, static_cast<lua_Integer>(*reinterpret_cast<int *>(p)));
}

void ofxOfeliaLua::setVariable(t_symbol *s)
{
    lua_pushnil(L);
    lua_setfield(L, -3, s->s_name);
    lua_pop(L, 2);
}

void ofxOfeliaLua::setVariable(t_symbol *s, bool b)
{
    lua_pushboolean(L, static_cast<int>(b));
    lua_setfield(L, -3, s->s_name);
    lua_pop(L, 2);
}

void ofxOfeliaLua::setVariable(t_symbol *s, t_floatarg f)
{
    lua_pushnumber(L, static_cast<lua_Number>(f));
    lua_setfield(L, -3, s->s_name);
    lua_pop(L, 2);
}

void ofxOfeliaLua::setVariable(t_symbol *s, t_symbol *s2)
{
    lua_pushstring(L, s2->s_name);
    lua_setfield(L, -3, s->s_name);
    lua_pop(L, 2);
}

void ofxOfeliaLua::setVariable(t_symbol *s, t_gpointer *p)
{
    pushUserData(p);
    lua_setfield(L, -3, s->s_name);
    lua_pop(L, 2);
}

void ofxOfeliaLua::setVariable(t_symbol *s, int argc, t_atom *argv)
{
    lua_newtable(L);
    for (int i = 0; i < argc; ++i)
    {
        lua_pushinteger(L, static_cast<lua_Integer>(i + 1));
        if (argv[i].a_type == A_FLOAT)
            lua_pushnumber(L, static_cast<lua_Number>(argv[i].a_w.w_float));
        else if (argv[i].a_type == A_SYMBOL)
            lua_pushstring(L, argv[i].a_w.w_symbol->s_name);
        else if (argv[i].a_type == A_POINTER)
            pushUserData(argv[i].a_w.w_gpointer);
        lua_settable(L, -3);
    }
    lua_setfield(L, -3, s->s_name);
    lua_pop(L, 2);
}

void ofxOfeliaLua::doVariable(t_symbol *s)
{
    if (!isNil(s)) return;
    setVariable(s);
}

void ofxOfeliaLua::doVariable(t_symbol *s, bool b)
{
    if (!isBoolean(s)) return;
    setVariable(s, b);
}

void ofxOfeliaLua::doVariable(t_symbol *s, t_floatarg f)
{
    if (!isNumber(s)) return;
    setVariable(s, f);
}

void ofxOfeliaLua::doVariable(t_symbol *s, t_symbol *s2)
{
    if (!isString(s)) return;
    setVariable(s, s2);
}

void ofxOfeliaLua::doVariable(t_symbol *s, t_gpointer *p)
{
    if (!isString(s)) return;
    setVariable(s, p);
}

void ofxOfeliaLua::doVariable(t_symbol *s, int argc, t_atom *argv)
{
    if (!isTable(s)) return;
    setVariable(s, argc, argv);
}

void ofxOfeliaLua::outletUserData()
{
    int userDataRef = luaL_ref(L, LUA_REGISTRYINDEX);
    outlet_pointer(dataPtr->io.outlets[0], reinterpret_cast<t_gpointer *>(&userDataRef));
    luaL_unref(L, LUA_REGISTRYINDEX, userDataRef);
}

void ofxOfeliaLua::outletTable()
{
    lua_pushvalue(L, -1);
    lua_pushnil(L);
    int ac = 0;
    t_atom *av = static_cast<t_atom *>(getbytes(sizeof(t_atom) * ac));
    std::deque<int> userDataRef;
    while (lua_next(L, -2))
    {
        if (lua_type(L, -2) == LUA_TSTRING) /* if the table has a string key */
        {
            lua_pop(L, 2);
            outletUserData(); /* treat the table as a userdata */
            if (ac) freebytes(av, sizeof(t_atom) * ac);
            return;
        }
        av = static_cast<t_atom *>(resizebytes(av, sizeof(t_atom) * ac,
                                               sizeof(t_atom) * (ac + 1)));
        if (lua_isboolean(L, -1))
        {
            av[ac].a_type = A_FLOAT;
            av[ac].a_w.w_float = static_cast<t_float>(lua_toboolean(L, -1));
            lua_pop(L, 1);
        }
        else if (lua_isnumber(L, -1))
        {
            av[ac].a_type = A_FLOAT;
            av[ac].a_w.w_float = static_cast<t_float>(lua_tonumber(L, -1));
            lua_pop(L, 1);
        }
        else if (lua_isstring(L, -1))
        {
            av[ac].a_type = A_SYMBOL;
            av[ac].a_w.w_symbol = gensym(lua_tostring(L, -1));
            lua_pop(L, 1);
        }
        else if (lua_isuserdata(L, -1) || lua_istable(L, -1))
        {
            av[ac].a_type = A_POINTER;
            userDataRef.push_back(luaL_ref(L, LUA_REGISTRYINDEX));
            av[ac].a_w.w_gpointer = reinterpret_cast<t_gpointer *>(&userDataRef.back());
        }
        ac++;
    }
    lua_pop(L, 1);
    const ofxOfeliaIO &io = dataPtr->io;
    if (io.hasMultiControlOutlets)
    {
        int last = (io.numOutlets >= ac ? ac : io.numOutlets) - 1;
        for (int i = last; i >= 0; --i)
        {
            if (av[i].a_type == A_FLOAT)
                outlet_float(io.outlets[i], av[i].a_w.w_float);
            else if (av[i].a_type == A_SYMBOL)
                outlet_symbol(io.outlets[i], av[i].a_w.w_symbol);
            else if (av[i].a_type == A_POINTER)
                outlet_pointer(io.outlets[i], av[i].a_w.w_gpointer);
        }
    }
    else
        outlet_list(dataPtr->ob.ob_outlet, &s_list, ac, av);
    for (const int i : userDataRef)
        luaL_unref(L, LUA_REGISTRYINDEX, i);
    freebytes(av, sizeof(t_atom) * ac);
}

void ofxOfeliaLua::outletRet(int nret)
{
    if (!nret) return;
    const ofxOfeliaIO &io = dataPtr->io;
    if (!io.hasControlOutlet) return;
    if (lua_isnil(L, -1))
        outlet_bang(io.outlets[0]);
    else if (lua_isboolean(L, -1))
        outlet_float(io.outlets[0], static_cast<t_float>(lua_toboolean(L, -1)));
    else if (lua_isnumber(L, -1))
        outlet_float(io.outlets[0], static_cast<t_float>(lua_tonumber(L, -1)));
    else if (lua_isstring(L, -1))
        outlet_symbol(io.outlets[0], gensym(lua_tostring(L, -1)));
    else if (lua_isuserdata(L, -1))
    {
        outletUserData();
        return;
    }
    else if (lua_istable(L, -1))
        outletTable();
    lua_pop(L, nret);
}

void ofxOfeliaLua::callFunction(int top)
{
    /* note: it currently passes only one argument */
    if (lua_pcall(L, 1, LUA_MULTRET, 0))
    {
        error("ofelia: %s", lua_tostring(L, -1));
        lua_pop(L, 1);
        return;
    }
    /* outlet return value if any exists */
    outletRet(lua_gettop(L) - top);
}

void ofxOfeliaLua::setFunction(int top)
{
    lua_pushnil(L);
    callFunction(top);
    lua_pop(L, 1);
}

void ofxOfeliaLua::setFunction(t_floatarg f, int top)
{
    lua_pushnumber(L, static_cast<lua_Number>(f));
    callFunction(top);
    lua_pop(L, 1);
}

void ofxOfeliaLua::setFunction(t_symbol *s, int top)
{
    lua_pushstring(L, s->s_name);
    callFunction(top);
    lua_pop(L, 1);
}

void ofxOfeliaLua::setFunction(t_gpointer *p, int top)
{
    pushUserData(p);
    callFunction(top);
    lua_pop(L, 1);
}

void ofxOfeliaLua::setFunction(int argc, t_atom *argv, int top)
{
    lua_newtable(L);
    for (int i = 0; i < argc; ++i)
    {
        lua_pushinteger(L, static_cast<lua_Integer>(i + 1));
        if (argv[i].a_type == A_FLOAT)
            lua_pushnumber(L, static_cast<lua_Number>(argv[i].a_w.w_float));
        else if (argv[i].a_type == A_SYMBOL)
            lua_pushstring(L, argv[i].a_w.w_symbol->s_name);
        else if (argv[i].a_type == A_POINTER)
            pushUserData(argv[i].a_w.w_gpointer);
        lua_settable(L, -3);
    }
    callFunction(top);
    lua_pop(L, 1);
}

void ofxOfeliaLua::doFunction(t_symbol *s)
{
    int top; if (!isFunction(s, top)) return;
    setFunction(top);
}

void ofxOfeliaLua::doFunction(t_symbol *s, t_floatarg f)
{
    int top; if (!isFunction(s, top)) return;
    setFunction(static_cast<lua_Number>(f), top);
}

void ofxOfeliaLua::doFunction(t_symbol *s, t_symbol *s2)
{
    int top; if (!isFunction(s, top)) return;
    setFunction(s2, top);
}

void ofxOfeliaLua::doFunction(t_symbol *s, t_gpointer *p)
{
    int top; if (!isFunction(s, top)) return;
    setFunction(p, top);
}

void ofxOfeliaLua::doFunction(t_symbol *s, int argc, t_atom *argv)
{
    int top; if (!isFunction(s, top)) return;
    setFunction(argc, argv, top);
}

void ofxOfeliaLua::doNewFunction()
{
    isChunkRun = true;
    doFunction(gensym("new")); /* isChunkRun will set to false if fail */
}

void ofxOfeliaLua::doFreeFunction()
{
    doFunction(gensym("free"));
    isChunkRun = false;
}

void ofxOfeliaLua::realizeDollar(char **bufp, int *lengthp)
{
    char *buf = static_cast<char *>(getbytes(0)); /* output buffer */
    int length = 0; /* output length */
    for (int i = 0; i < *lengthp; ++i)
    {
        if ((*bufp)[i] == '$')
        {
            int first = i + 1; /* index to first digit */
            char dbuf[MAXPDSTRING] = "$"; /* buffer to store dollarsym */
            int dlen = 1; /* length of dollarsym */
            while (first < *lengthp && std::isdigit((*bufp)[first]))
                dbuf[dlen++] = (*bufp)[first++];
            int tlen = dlen - 1; /* number of trailing digits */
            if (tlen)
            {
                dbuf[dlen] = '\0';
                t_symbol *s = canvas_realizedollar(dataPtr->canvas, gensym(dbuf));
                std::strcpy(dbuf, s->s_name);
                dlen = std::strlen(s->s_name);
                int newlength = length + dlen;
                buf = static_cast<char *>(resizebytes(buf, length, newlength));
                std::strcpy(buf + length, dbuf);
                length = newlength;
                i += tlen;
            }
            else goto append;
        }
        else
        {
        append: /* append a character */
            buf = static_cast<char *>(resizebytes(buf, length, length + 1));
            buf[length++] = (*bufp)[i];
        }
    }
    freebytes(*bufp, *lengthp); /* free the previously allocated memory */
    buf = static_cast<char *>(resizebytes(buf, length, length + 1));
    buf[length++] = '\0';
    *bufp = buf;
    *lengthp = length;
}

void ofxOfeliaLua::doString(const char *s)
{
    lua_settop(L, 0); /* empty the stack */
    std::ostringstream ss;
    const char *name = dataPtr->sym->s_name;
    ss << "package.preload['" << name << "'] = nil package.loaded['" << name << "'] = nil\n"
    << "package.preload['" << name << "'] = function(this) local ofelia = {} local M = ofelia\n";
    if (!dataPtr->isFunctionMode)
        ss << s;
    else if (!dataPtr->isSignalObject)
    {
        ss << "function M.bang() return M.anything(nil) end function M.float(f) return M.anything(f) end function M.symbol(s) return M.anything(s) end function M.pointer(p) return M.anything(p) end function M.list(l) return M.anything(l) end function M.anything(a)\n" << s << "\nend";
    }
    else
    {
        ss << "function M.perform(";
        const int numInlets = dataPtr->io.numInlets;
        for (int i = 0; i < numInlets; ++i)
        {
            if (i) ss << ',';
            ss << 'a' << i + 1;
        }
        ss << ")\n" << s << "\nend";
    }
    ss << "\nreturn M end";
    /* run the lua chunk */
    const int ret = luaL_dostring(L, ss.str().c_str());
    if (ret != LUA_OK)
    {
        error("ofelia: %s", lua_tostring(L, -1));
        lua_pop(L, 1);
        return;
    }
    /* call the new function */
    doNewFunction();
}

void ofxOfeliaLua::doArgs(int argc, t_atom *argv)
{
    std::ostringstream ss;
    for (int i = 0; i < argc; ++i)
    {
        if (argv[i].a_type == A_SYMBOL)
            ss << argv[i].a_w.w_symbol->s_name;
        else if (argv[i].a_type == A_FLOAT)
            ss << argv[i].a_w.w_float;
        ss << ' ';
    }
    doString(ss.str().c_str());
}

void ofxOfeliaLua::doText()
{
    /* get the string from text editor gui */
    int ntxt;
    char *txt;
    binbuf_gettext(dataPtr->binbuf, &txt, &ntxt);
    realizeDollar(&txt, &ntxt);
    doString(txt);
    freebytes(txt, ntxt);
}

void ofxOfeliaLua::getVariableByArgs(t_symbol *s, int argc, t_atom *argv)
{
    if (!require()) return;
    const ofxOfeliaIO &io = dataPtr->io;
    int top; switch (getType(s, top))
    {
        case LUA_TNIL:
            if (io.hasControlOutlet)
                outlet_bang(io.outlets[0]);
            break;
        case LUA_TBOOLEAN:
            if (io.hasControlOutlet)
                outlet_float(io.outlets[0], static_cast<t_float>(lua_toboolean(L, -1)));
            break;
        case LUA_TNUMBER:
            if (io.hasControlOutlet)
                outlet_float(io.outlets[0], static_cast<t_float>(lua_tonumber(L, -1)));
            break;
        case LUA_TSTRING:
            if (io.hasControlOutlet)
                outlet_symbol(io.outlets[0], gensym(lua_tostring(L, -1)));
            break;
        case LUA_TUSERDATA:
            if (io.hasControlOutlet)
                outletUserData();
            break;
        case LUA_TTABLE:
            if (io.hasControlOutlet)
                outletTable();
            break;
        case LUA_TFUNCTION:
            if (!argc)
                setFunction(top);
            else if (argc == 1)
            {
                if (argv[0].a_type == A_FLOAT)
                    setFunction(argv[0].a_w.w_float, top);
                else if (argv[0].a_type == A_SYMBOL)
                    setFunction(argv[0].a_w.w_symbol, top);
                else if (argv[0].a_type == A_POINTER)
                    setFunction(argv[0].a_w.w_gpointer, top);
            }
            else
                setFunction(argc, argv, top);
            break;
        default:
            break;
    }
}

void ofxOfeliaLua::setVariableByArgs(t_symbol *s, int argc, t_atom *argv)
{
    if (!require()) return;
    int top; switch (getType(s, top))
    {
        case LUA_TNIL:
            if (!argc)
                setVariable(s);
            else if (argc == 1)
            {
                if (argv[0].a_type == A_FLOAT)
                    setVariable(s, argv[0].a_w.w_float);
                else if (argv[0].a_type == A_SYMBOL)
                    setVariable(s, argv[0].a_w.w_symbol);
                else if (argv[0].a_type == A_POINTER)
                    setVariable(s, argv[0].a_w.w_gpointer);
            }
            else
                setVariable(s, argc, argv);
            break;
        case LUA_TBOOLEAN:
            if (!argc)
                setVariable(s);
            else if (argv[0].a_type == A_FLOAT)
                setVariable(s, argv[0].a_w.w_float != 0);
            else
                error("ofelia: failed to set the boolean variable '%s'", s->s_name);
            break;
        case LUA_TNUMBER:
            if (!argc)
                setVariable(s);
            else if (argv[0].a_type == A_FLOAT)
                setVariable(s, argv[0].a_w.w_float);
            else
                error("ofelia: failed to set the number variable '%s'", s->s_name);
            break;
        case LUA_TSTRING:
            if (!argc)
                setVariable(s);
            else if (argv[0].a_type == A_SYMBOL)
                setVariable(s, argv[0].a_w.w_symbol);
            else
                error("ofelia: failed to set the string variable '%s'", s->s_name);
            break;
        case LUA_TUSERDATA:
            if (!argc)
                setVariable(s);
            else if (argv[0].a_type == A_POINTER)
                setVariable(s, argv[0].a_w.w_gpointer);
            else
                error("ofelia: failed to set the pointer variable '%s'", s->s_name);
            break;
        case LUA_TTABLE:
            if (!argc)
                setVariable(s);
            else
                setVariable(s, argc, argv);
            break;
        case LUA_TFUNCTION:
            if (!argc)
                setFunction(top);
            else if (argc == 1)
            {
                if (argv[0].a_type == A_FLOAT)
                    setFunction(argv[0].a_w.w_float, top);
                else if (argv[0].a_type == A_SYMBOL)
                    setFunction(argv[0].a_w.w_symbol, top);
                else if (argv[0].a_type == A_POINTER)
                    setFunction(argv[0].a_w.w_gpointer, top);
            }
            else
                setFunction(argc, argv, top);
            break;
        default:
            break;
    }
}
