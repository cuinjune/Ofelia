/* terminal command to generate .cpp wrapper
swig -c++ -lua -fcompact -fvirtual -I../../../libs/openFrameworks ofeliaBindings.i && mv ofeliaBindings_wrap.cxx ofeliaBindings.cpp
*/

%module pd
%{
    #include "ofeliaBindings.h"
%}

%include <stl.i>
%include <exception.i>
%include <typemaps.i>

/* used to backport lua_len to Lua 5.1 */
%{
#if LUA_VERSION_NUM <= 501
static void lua_len(lua_State *L, int i) 
{
    switch (lua_type(L, i)) 
    {
        case LUA_TSTRING:
            lua_pushnumber(L, (lua_Number)lua_objlen(L, i));
            break;
        case LUA_TTABLE:
            if (!luaL_callmeta(L, i, "__len"))
            lua_pushnumber(L, (lua_Number)lua_objlen(L, i));
            break;
        case LUA_TUSERDATA:
            if (luaL_callmeta(L, i, "__len"))
            break;
        default:
        luaL_error(L, "attempt to get length of a %s value",
                   lua_typename(L, lua_type(L, i)));
    }
}
#endif
%}

/* convert the input lua_Number to t_floatarg */
%typemap(typecheck) t_floatarg 
{
    $1 = lua_isnumber(L, $input);
}
%typemap(in) t_floatarg 
{
    $1 = static_cast<t_float>(lua_tonumber(L, $input));
}

/* convert the input lua_String to t_symbol* */
%typemap(typecheck) t_symbol* 
{
    $1 = lua_isstring(L, $input);
}
%typemap(in) t_symbol* 
{
    $1 = gensym(lua_tostring(L, $input));
}

/* convert the input lua_UserData to t_gpointer* */
%typemap(typecheck) t_gpointer* 
{
    $1 = lua_isuserdata(L, $input);
}
%typemap(in) t_gpointer* 
{
    $1 = nullptr;
}

/* convert the input lua_Table to int, t_atom* */
%typemap(in) (int argc, t_atom *argv)
{
    if (!lua_istable(L, $input))
        SWIG_exception(SWIG_RuntimeError, "argument mismatch: table expected");
    lua_len(L, $input);
    $1 = static_cast<lua_Integer>(lua_tointeger(L, -1));
    if (!$1) SWIG_exception(SWIG_RuntimeError, "table is empty");
    $2 = static_cast<t_atom *>(getbytes($1 * sizeof(t_atom)));
    std::deque<int> userDataRef;
    for (int i = 0; i < $1; ++i) 
    {
        lua_pushinteger(L, i + 1);
        lua_gettable(L, $input);
        if (lua_isboolean(L, -1)) 
        {
	    $2[i].a_type = A_FLOAT;
	    $2[i].a_w.w_float = static_cast<t_float>(lua_toboolean(L, -1));
        }  
	else if (lua_isnumber(L, -1)) 
        {
	    $2[i].a_type = A_FLOAT;
	    $2[i].a_w.w_float = static_cast<t_float>(lua_tonumber(L, -1));
        }          
        else if (lua_isstring(L, -1)) 
        {
	    $2[i].a_type = A_SYMBOL;
	    $2[i].a_w.w_symbol = gensym(lua_tostring(L, -1));
        }
        else if (lua_isuserdata(L, -1)) 
        {
	    $2[i].a_type = A_POINTER;
	    userDataRef.push_back(luaL_ref(L, LUA_REGISTRYINDEX));
            $2[i].a_w.w_gpointer = reinterpret_cast<t_gpointer *>(&userDataRef.back());
        }
	else 
        {
	    SWIG_exception(SWIG_RuntimeError, "unhandled argument type");
        }
    }
    for (const int i : userDataRef)
        luaL_unref(L, LUA_REGISTRYINDEX, i);
}
%typemap(freearg) (int argc, t_atom *argv) 
{
    freebytes($2, $1 * sizeof(t_atom));
}

/* convert the output t_float to lua_Number */
%typemap(out) t_float 
{
    lua_pushnumber(L, static_cast<lua_Number>($1));
    ++SWIG_arg;
}

/* convert the output t_symbol* to lua_String */
%typemap(out) t_symbol* 
{
    lua_pushstring(L, ($1)->s_name);
    ++SWIG_arg;
}

/* convert the output t_gpointer* to lua_UserData */
%typemap(out) t_gpointer* 
{
    lua_rawgeti(L, LUA_REGISTRYINDEX, static_cast<lua_Integer>(*reinterpret_cast<int *>($1)));
    ++SWIG_arg;
}

/* used for Canvas:getArgs() */
%typemap(in, numinputs = 0) (int *argcp, t_atom **argvp, t_canvas **canvasp) 
(int targc = 0, t_atom *targv = nullptr, t_canvas *tcanvas = nullptr) 
%{
    $1 = &targc;
    $2 = &targv;
    $3 = &tcanvas;
%}
%typemap(argout) (int *argcp, t_atom **argvp, t_canvas **canvasp) 
{
    lua_newtable(L);
    for (int i = 0; i < *$1; ++i)
    {
        char tbuf[MAXPDSTRING];
        switch ((*$2)[i].a_type)
        {
            case A_FLOAT:
                lua_pushnumber(L, static_cast<lua_Number>((*$2)[i].a_w.w_float));
                break;
            case A_SYMBOL:
                lua_pushstring(L, (*$2)[i].a_w.w_symbol->s_name);
                break;
            case A_POINTER:
                lua_rawgeti(L, LUA_REGISTRYINDEX, static_cast<lua_Integer>(*reinterpret_cast<int *>($2)));
                break;
            case A_SEMI:
                lua_pushstring(L, ";");
                break;
            case A_COMMA:
                lua_pushstring(L, ",");
                break;
            case A_DOLLAR:
            {
                sprintf(tbuf, "$%d", (*$2)[i].a_w.w_index);
                const char *s = canvas_realizedollar(*$3, gensym(tbuf))->s_name;
                char *ep = nullptr;
                double f = strtod(s, &ep);
                if (!ep  ||  *ep)
                {
                    size_t len = strlen(s);
                    if (s[0] == '$' && len > 1)
                    {
                        bool isStr = false;
                        for (int j = 1; j < len; ++j)
                        {
                            if (!isdigit(s[j]))
                            {
                                isStr = true;
                                break;
                            }
                        }
                        if (isStr) lua_pushstring(L, s);
                        else lua_pushnumber(L, 0);
                    }
                    else
                        lua_pushstring(L, s);
                }
                else
                    lua_pushnumber(L, static_cast<lua_Number>(f));
                break;
            }
            case A_DOLLSYM:
            {
                atom_string(*$2 + i, tbuf, MAXPDSTRING);
                const char *s = canvas_realizedollar(*$3, gensym(tbuf))->s_name;
                lua_pushstring(L, s);
                break;
            }
            default:
                break;
        }
        lua_rawseti(L, -2, i + 1);
    }
    SWIG_arg++;
}

/* convert the output vector<pair<ofeliaData *, t_float>> to lua_Table */
%typemap(out) vector<pair<ofeliaData *, t_float>>
{
    lua_newtable(L);
    for (size_t i = 0; i < $1.size(); ++i)
    {
        lua_newtable(L);
        lua_pushstring(L, $1.at(i).first->sym->s_name);
        lua_setfield(L, -2, "name");
        lua_pushnumber(L, static_cast<lua_Number>($1.at(i).second));
        lua_setfield(L, -2, "order");
        lua_rawseti(L, -2, i + 1);
    }
    SWIG_arg++;
}

/* convert the output tuple<int, int, int> to lua_Table */
%typemap(out) tuple<int, int, int>
{
    lua_newtable(L);
    lua_pushinteger(L, static_cast<lua_Number>(get<0>($1)));
    lua_setfield(L, -2, "major");
    lua_pushinteger(L, static_cast<lua_Number>(get<1>($1)));
    lua_setfield(L, -2, "minor");
    lua_pushinteger(L, static_cast<lua_Number>(get<2>($1)));
    lua_setfield(L, -2, "bugfix");
    SWIG_arg++;
}

/* used for OFWindow:getPosition() and Canvas:getPosition() */
%typemap(in, numinputs = 0) (int **posp) 
(int *tmp = nullptr) 
%{
    $1 = &tmp;
%}
%typemap(argout) (int **posp) 
{
    lua_newtable(L);
    for (int i = 0; i < 2; ++i)
    {
        lua_pushnumber(L, static_cast<lua_Number>((*$1)[i]));
        lua_rawseti(L, -2, i + 1);
    }
    SWIG_arg++;
}
%typemap(freearg) (int **posp) 
{
    freebytes(*$1, 2 * sizeof(int));
}

/* used for Array:getTable() */
%typemap(in, numinputs = 0) (t_word **vecp, int *sizep) 
(t_word *tmp = nullptr, int tsize = 0) 
%{
    $1 = &tmp;
    $2 = &tsize;
%}
%typemap(argout) (t_word **vecp, int *sizep) 
{
    lua_newtable(L);
    for (int i = 0; i < *$2; ++i)
    {
        lua_pushnumber(L, static_cast<lua_Number>((*$1)[i].w_float));
        lua_rawseti(L, -2, i + 1);
    }
    SWIG_arg++;
}

/* used for Inlet:setFloatInlets() and Array:setTable() */
%typemap(in) (int n, t_floatarg *f)
{
    if (!lua_istable(L, $input))
        SWIG_exception(SWIG_RuntimeError, "argument mismatch: table expected");
    lua_len(L, $input);
    $1 = static_cast<lua_Integer>(lua_tointeger(L, -1));
    if (!$1) SWIG_exception(SWIG_RuntimeError, "table is empty");
    $2 = static_cast<t_float *>(getbytes($1 * sizeof(t_float)));
    for (int i = 0; i < $1; ++i) 
    {
        lua_pushinteger(L, i + 1);
        lua_gettable(L, $input);
	if (lua_isnumber(L, -1))
	    $2[i] = static_cast<t_float>(lua_tonumber(L, -1));       
	else
	    SWIG_exception(SWIG_RuntimeError, "unhandled argument type");
    }
}
%typemap(freearg) (int n, t_floatarg *f) 
{
    freebytes($2, $1 * sizeof(t_float));
}

%include "ofeliaBindings.h"
