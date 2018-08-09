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

/* convert the input lua_Table to int, t_atom* */
%typemap(in) (int argc, t_atom *argv)
{
    if (!lua_istable(L, $input))
        SWIG_exception(SWIG_RuntimeError, "argument mismatch: table expected");
    lua_len(L, $input);
    $1 = static_cast<lua_Integer>(lua_tointeger(L, -1));
    if (!$1) SWIG_exception(SWIG_RuntimeError, "table is empty");
    $2 = static_cast<t_atom *>(getbytes($1 * sizeof(t_atom)));
    for (int i = 0; i < $1; ++i) 
    {
        lua_pushinteger(L, i + 1);
        lua_gettable(L, $input);
	if (lua_isnumber(L, -1)) 
        {
	    $2[i].a_type = A_FLOAT;
	    $2[i].a_w.w_float = static_cast<t_float>(lua_tonumber(L, -1));
        }          
        else if (lua_isstring(L, -1)) 
        {
	    $2[i].a_type = A_SYMBOL;
	    $2[i].a_w.w_symbol = gensym(lua_tostring(L, -1));
        }
	else 
        {
	    SWIG_exception(SWIG_RuntimeError, "unhandled argument type");
        }
    }
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

/* convert the output int *argcp, t_atom **argvp to lua_Table */
%typemap(in, numinputs = 0) (int *argcp, t_atom **argvp) 
(int targc = 0, t_atom *targv = nullptr) 
%{
    $1 = &targc;
    $2 = &targv;
%}
%typemap(argout) (int *argcp, t_atom **argvp) 
{
    lua_newtable(L);
    for (int i = 0; i < *$1; ++i)
    {
        if ((*$2)[i].a_type == A_FLOAT)
            lua_pushnumber(L, static_cast<lua_Number>((*$2)[i].a_w.w_float));
        else if ((*$2)[i].a_type == A_SYMBOL)
            lua_pushstring(L, (*$2)[i].a_w.w_symbol->s_name);
        lua_rawseti(L, -2, i + 1);
    }
    SWIG_arg++;
}

/* used for Canvas:getPosition() */
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

/* used for Canvas:setPosition() */
%typemap(in) (int *pos)
{
    if (!lua_istable(L, $input))
        SWIG_exception(SWIG_RuntimeError, "argument mismatch: table expected");
    lua_len(L, $input);
    int len = static_cast<lua_Integer>(lua_tointeger(L, -1));
    if (!len) 
        SWIG_exception(SWIG_RuntimeError, "table is empty");
    if (len != 2) 
        SWIG_exception(SWIG_RuntimeError, "invalid number of table elements");
    $1 = static_cast<int *>(getbytes(len * sizeof(int)));
    for (int i = 0; i < 2; ++i) 
    {
        lua_pushinteger(L, i + 1);
        lua_gettable(L, $input);
	if (lua_isnumber(L, -1))
	    $1[i] = static_cast<int>(lua_tonumber(L, -1));       
	else
	    SWIG_exception(SWIG_RuntimeError, "unhandled argument type");
    }
}
%typemap(freearg) (int *pos) 
{
    freebytes($1, 2 * sizeof(int));
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

/* used for Array:setTable() */
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
