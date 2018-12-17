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

#include "ofeliaLua.h"
#include "ofeliaData.h"
#include <deque>
#include <cstring>
#include <sstream>

lua_State *ofeliaLua::L;

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

bool ofeliaLua::init()
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
    
    /* make garbage collector run more frequently (default: 200) */
    lua_gc(L, LUA_GCSETPAUSE, 100);
    return true;
}

bool ofeliaLua::require()
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

int ofeliaLua::getType(t_symbol *s)
{
    lua_getfield(L, -1, s->s_name);
    return lua_type(L, -1);
}

int ofeliaLua::getType(t_symbol *s, int &top)
{
    top = lua_gettop(L);
    lua_getfield(L, -1, s->s_name);
    return lua_type(L, -1);
}

bool ofeliaLua::isNil(t_symbol *s)
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

bool ofeliaLua::isBoolean(t_symbol *s)
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

bool ofeliaLua::isNumber(t_symbol *s)
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

bool ofeliaLua::isString(t_symbol *s)
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

bool ofeliaLua::isUserData(t_symbol *s)
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

bool ofeliaLua::isTable(t_symbol *s)
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

bool ofeliaLua::isFunction(t_symbol *s, int &top)
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

void ofeliaLua::pushUserData(t_gpointer *p)
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, static_cast<lua_Integer>(*reinterpret_cast<int *>(p)));
}

void ofeliaLua::setVariable(t_symbol *s)
{
    lua_pushnil(L);
    lua_setfield(L, -3, s->s_name);
    lua_pop(L, 2);
}

void ofeliaLua::setVariable(t_symbol *s, bool b)
{
    lua_pushboolean(L, static_cast<int>(b));
    lua_setfield(L, -3, s->s_name);
    lua_pop(L, 2);
}

void ofeliaLua::setVariable(t_symbol *s, t_floatarg f)
{
    lua_pushnumber(L, static_cast<lua_Number>(f));
    lua_setfield(L, -3, s->s_name);
    lua_pop(L, 2);
}

void ofeliaLua::setVariable(t_symbol *s, t_symbol *s2)
{
    lua_pushstring(L, s2->s_name);
    lua_setfield(L, -3, s->s_name);
    lua_pop(L, 2);
}

void ofeliaLua::setVariable(t_symbol *s, t_gpointer *p)
{
    pushUserData(p);
    lua_setfield(L, -3, s->s_name);
    lua_pop(L, 2);
}

void ofeliaLua::setVariable(t_symbol *s, int argc, t_atom *argv)
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

void ofeliaLua::doVariable(t_symbol *s)
{
    if (!isNil(s)) return;
    setVariable(s);
}

void ofeliaLua::doVariable(t_symbol *s, bool b)
{
    if (!isBoolean(s)) return;
    setVariable(s, b);
}

void ofeliaLua::doVariable(t_symbol *s, t_floatarg f)
{
    if (!isNumber(s)) return;
    setVariable(s, f);
}

void ofeliaLua::doVariable(t_symbol *s, t_symbol *s2)
{
    if (!isString(s)) return;
    setVariable(s, s2);
}

void ofeliaLua::doVariable(t_symbol *s, t_gpointer *p)
{
    if (!isString(s)) return;
    setVariable(s, p);
}

void ofeliaLua::doVariable(t_symbol *s, int argc, t_atom *argv)
{
    if (!isTable(s)) return;
    setVariable(s, argc, argv);
}

void ofeliaLua::outletUserData()
{
    int userDataRef = luaL_ref(L, LUA_REGISTRYINDEX);
    outlet_pointer(dataPtr->io.outlets[0], reinterpret_cast<t_gpointer *>(&userDataRef));
    luaL_unref(L, LUA_REGISTRYINDEX, userDataRef);
}

void ofeliaLua::outletTable()
{
    lua_pushvalue(L, -1);
    lua_pushnil(L);
    int ac = 0;
    t_atom *av = static_cast<t_atom *>(getbytes(sizeof(t_atom) * ac));
    std::deque<int> userDataRef;
    while (lua_next(L, -2))
    {
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
        else if (lua_isuserdata(L, -1))
        {
            av[ac].a_type = A_POINTER;
            userDataRef.push_back(luaL_ref(L, LUA_REGISTRYINDEX));
            av[ac].a_w.w_gpointer = reinterpret_cast<t_gpointer *>(&userDataRef.back());
        }
        ac++;
    }
    lua_pop(L, 1);
    const ofeliaIO &io = dataPtr->io;
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

void ofeliaLua::outletRet()
{
    const ofeliaIO &io = dataPtr->io;
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
    lua_pop(L, 1);
}

void ofeliaLua::callFunction(int top)
{
    /* note: it currently passes only one argument */
    if (lua_pcall(L, 1, LUA_MULTRET, 0))
    {
        error("ofelia: %s", lua_tostring(L, -1));
        lua_pop(L, 1);
        return;
    }
    /* outlet return value if any exists */
    if (lua_gettop(L) - top)
        outletRet();
}

void ofeliaLua::setFunction(int top)
{
    lua_pushnil(L);
    callFunction(top);
    lua_pop(L, 1);
}

void ofeliaLua::setFunction(t_floatarg f, int top)
{
    lua_pushnumber(L, static_cast<lua_Number>(f));
    callFunction(top);
    lua_pop(L, 1);
}

void ofeliaLua::setFunction(t_symbol *s, int top)
{
    lua_pushstring(L, s->s_name);
    callFunction(top);
    lua_pop(L, 1);
}

void ofeliaLua::setFunction(t_gpointer *p, int top)
{
    pushUserData(p);
    callFunction(top);
    lua_pop(L, 1);
}

void ofeliaLua::setFunction(int argc, t_atom *argv, int top)
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

void ofeliaLua::doFunction(t_symbol *s)
{
    int top; if (!isFunction(s, top)) return;
    setFunction(top);
}

void ofeliaLua::doFunction(t_symbol *s, t_floatarg f)
{
    int top; if (!isFunction(s, top)) return;
    setFunction(static_cast<lua_Number>(f), top);
}

void ofeliaLua::doFunction(t_symbol *s, t_symbol *s2)
{
    int top; if (!isFunction(s, top)) return;
    setFunction(s2, top);
}

void ofeliaLua::doFunction(t_symbol *s, t_gpointer *p)
{
    int top; if (!isFunction(s, top)) return;
    setFunction(p, top);
}

void ofeliaLua::doFunction(t_symbol *s, int argc, t_atom *argv)
{
    int top; if (!isFunction(s, top)) return;
    setFunction(argc, argv, top);
}

void ofeliaLua::doNewFunction()
{
    isChunkRun = true;
    doFunction(gensym("new")); /* isChunkRun will set to false if fail */
}

void ofeliaLua::doFreeFunction()
{
    doFunction(gensym("free"));
    isChunkRun = false;
}

void ofeliaLua::realizeDollar(char **bufp, int *lengthp)
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
            while (first < *lengthp && isdigit((*bufp)[first]))
                dbuf[dlen++] = (*bufp)[first++];
            int tlen = dlen - 1; /* number of trailing digits */
            if (tlen)
            {
                dbuf[dlen] = '\0';
                t_symbol *s = canvas_realizedollar(dataPtr->canvas, gensym(dbuf));
                strcpy(dbuf, s->s_name);
                dlen = strlen(s->s_name);
                int newlength = length + dlen;
                buf = static_cast<char *>(resizebytes(buf, length, newlength));
                strcpy(buf + length, dbuf);
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

void ofeliaLua::doString(const char *s)
{
    lua_settop(L, 0); /* empty the stack */
    std::ostringstream ss;
    const char *name = dataPtr->sym->s_name;
    ss << "package.preload['" << name << "'] = nil package.loaded['" << name << "'] = nil\n"
    << "package.preload['" << name << "'] = function(this) local ofelia = {}\n"
    << s << "\nreturn ofelia end";
    
    /* run the lua chunk */
    const int ret = luaL_dostring(L, ss.str().c_str());
    if (ret)
    {
        if (ret == LUA_ERRSYNTAX)
            error("ofelia: %s", lua_tostring(L, -1));
        else if (ret == LUA_ERRMEM)
            error("ofelia: memory error");
        else
            error("ofelia: syntax error");
        return;
    }
    /* call the new function */
    doNewFunction();
}

void ofeliaLua::doArgs(int argc, t_atom *argv)
{
    dataPtr->initSym();
    dataPtr->bindSym();
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
    dataPtr->isDirectMode = true;
}

void ofeliaLua::doText()
{
    /* get the string from text editor gui */
    int ntxt;
    char *txt;
    binbuf_gettext(dataPtr->binbuf, &txt, &ntxt);
    realizeDollar(&txt, &ntxt);
    doString(txt);
    freebytes(txt, ntxt);
}

void ofeliaLua::getVariableByArgs(t_symbol *s, int argc, t_atom *argv)
{
    if (!require()) return;
    const ofeliaIO &io = dataPtr->io;
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

void ofeliaLua::setVariableByArgs(t_symbol *s, int argc, t_atom *argv)
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

