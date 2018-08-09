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

#include "ofeliaSetup.h"
#include <string.h>
#include <stdio.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef TARGET_WIN32
#include <direct.h>
#include <io.h>
#define CHDIR _chdir
#else
#define CHDIR chdir
#endif
#include <errno.h>

lua_State *t_ofeliadata::L;

bool t_ofeliadata::ofelialua_require(t_ofeliadata *x)
{
    if (!x->run) return false;
    lua_getglobal(L, "require");
    lua_pushstring(L, x->sym->s_name);
    if (lua_pcall(L, 1, LUA_MULTRET, 0))
    {
        error("ofelia: %s", lua_tostring(L, -1));
        lua_pop(L, 1);
        return false;
    }
    return true;
}

int t_ofeliadata::ofelialua_gettype(t_ofeliadata *x, t_symbol *s)
{
    if (!ofelialua_require(x))
        return LUA_TNONE;
    lua_getfield(L, -1, s->s_name);
    return lua_type(L, -1);
}

int t_ofeliadata::ofelialua_gettype(t_ofeliadata *x, t_symbol *s, int &top)
{
    if (!ofelialua_require(x))
        return LUA_TNONE;
    top = lua_gettop(L);
    lua_getfield(L, -1, s->s_name);
    return lua_type(L, -1);
}

bool t_ofeliadata::ofelialua_isnil(t_ofeliadata *x, t_symbol *s)
{
    if (ofelialua_gettype(x, s) != LUA_TNIL)
    {
        lua_pop(L, 1);
        return false;
    }
    return true;
}

bool t_ofeliadata::ofelialua_isboolean(t_ofeliadata *x, t_symbol *s)
{
    if (ofelialua_gettype(x, s) != LUA_TBOOLEAN)
    {
        lua_pop(L, 1);
        return false;
    }
    return true;
}

bool t_ofeliadata::ofelialua_isnumber(t_ofeliadata *x, t_symbol *s)
{
    if (ofelialua_gettype(x, s) != LUA_TNUMBER)
    {
        lua_pop(L, 1);
        return false;
    }
    return true;
}

bool t_ofeliadata::ofelialua_isstring(t_ofeliadata *x, t_symbol *s)
{
    if (ofelialua_gettype(x, s) != LUA_TSTRING)
    {
        lua_pop(L, 1);
        return false;
    }
    return true;
}

bool t_ofeliadata::ofelialua_istable(t_ofeliadata *x, t_symbol *s)
{
    if (ofelialua_gettype(x, s) != LUA_TTABLE)
    {
        lua_pop(L, 1);
        return false;
    }
    return true;
}

bool t_ofeliadata::ofelialua_isfunction(t_ofeliadata *x, t_symbol *s, int &top)
{
    if (ofelialua_gettype(x, s, top) != LUA_TFUNCTION)
    {
        lua_pop(L, 1);
        return false;
    }
    return true;
}

void t_ofeliadata::ofelialua_setvariable(t_symbol *s)
{
    lua_pushnil(L);
    lua_setfield(L, -3, s->s_name);
}

void t_ofeliadata::ofelialua_setvariable(t_symbol *s, bool b)
{
    lua_pushboolean(L, static_cast<int>(b));
    lua_setfield(L, -3, s->s_name);
}

void t_ofeliadata::ofelialua_setvariable(t_symbol *s, t_floatarg f)
{
    lua_pushnumber(L, static_cast<lua_Number>(f));
    lua_setfield(L, -3, s->s_name);
}

void t_ofeliadata::ofelialua_setvariable(t_symbol *s, t_symbol *s2)
{
    lua_pushstring(L, s2->s_name);
    lua_setfield(L, -3, s->s_name);
}

void t_ofeliadata::ofelialua_setvariable(t_symbol *s, int argc, t_atom *argv)
{
    lua_newtable(L);
    for (int i = 0; i < argc; ++i)
    {
        lua_pushinteger(L, static_cast<lua_Integer>(i + 1));
        if (argv[i].a_type == A_FLOAT)
            lua_pushnumber(L, static_cast<lua_Number>(argv[i].a_w.w_float));
        else if (argv[i].a_type == A_SYMBOL)
            lua_pushstring(L, argv[i].a_w.w_symbol->s_name);
        lua_settable(L, -3);
    }
    lua_setfield(L, -3, s->s_name);
}

void t_ofeliadata::ofelialua_dovariable(t_ofeliadata *x, t_symbol *s)
{
    if (!ofelialua_isnil(x, s)) return;
    ofelialua_setvariable(s);
}

void t_ofeliadata::ofelialua_dovariable(t_ofeliadata *x, t_symbol *s, bool b)
{
    if (!ofelialua_isboolean(x, s)) return;
    ofelialua_setvariable(s, b);
}

void t_ofeliadata::ofelialua_dovariable(t_ofeliadata *x, t_symbol *s, t_floatarg f)
{
    if (!ofelialua_isnumber(x, s)) return;
    ofelialua_setvariable(s, f);
}

void t_ofeliadata::ofelialua_dovariable(t_ofeliadata *x, t_symbol *s, t_symbol *s2)
{
    if (!ofelialua_isstring(x, s)) return;
    ofelialua_setvariable(s, s2);
}

void t_ofeliadata::ofelialua_dovariable(t_ofeliadata *x, t_symbol *s, int argc, t_atom *argv)
{
    if (!ofelialua_istable(x, s)) return;
    ofelialua_setvariable(s, argc, argv);
}

void t_ofeliadata::ofelialua_outlettable(t_ofeliadata *x)
{
    lua_pushvalue(L, -1);
    lua_pushnil(L);
    int ac = 0;
    t_atom *av = static_cast<t_atom *>(getbytes(ac * sizeof(t_atom)));
    while (lua_next(L, -2))
    {
        av = static_cast<t_atom *>(resizebytes(av, ac * sizeof(t_atom),
                                               (ac + 1) * sizeof(t_atom)));
        if (lua_isboolean(L, -1))
        {
            av[ac].a_type = A_FLOAT;
            av[ac].a_w.w_float = static_cast<t_float>(lua_toboolean(L, -1));
        }
        else if (lua_isnumber(L, -1))
        {
            av[ac].a_type = A_FLOAT;
            av[ac].a_w.w_float = static_cast<t_float>(lua_tonumber(L, -1));
        }
        else if (lua_isstring(L, -1))
        {
            av[ac].a_type = A_SYMBOL;
            av[ac].a_w.w_symbol = gensym(lua_tostring(L, -1));
        }
        ac++;
        lua_pop(L, 1);
    }
    lua_pop(L, 1);
    outlet_list(x->ob.ob_outlet, &s_list, ac, av);
    freebytes(av, ac * sizeof(t_atom));
}

void t_ofeliadata::ofelialua_outletret(t_ofeliadata *x)
{
    if (x->audio) return; /* ignore audio object */
    if (lua_isnil(L, -1))
        outlet_bang(x->ob.ob_outlet);
    else if (lua_isboolean(L, -1))
        outlet_float(x->ob.ob_outlet, static_cast<t_float>(lua_toboolean(L, -1)));
    else if (lua_isnumber(L, -1))
        outlet_float(x->ob.ob_outlet, static_cast<t_float>(lua_tonumber(L, -1)));
    else if (lua_isstring(L, -1))
        outlet_symbol(x->ob.ob_outlet, gensym(lua_tostring(L, -1)));
    else if (lua_istable(L, -1))
        ofelialua_outlettable(x);
}

void t_ofeliadata::ofelialua_callfunction(t_ofeliadata *x, int top)
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
        ofelialua_outletret(x);
}

void t_ofeliadata::ofelialua_setfunction(t_ofeliadata *x, int top)
{
    lua_pushnil(L);
    ofelialua_callfunction(x, top);
}

void t_ofeliadata::ofelialua_setfunction(t_ofeliadata *x, t_floatarg f, int top)
{
    lua_pushnumber(L, static_cast<lua_Number>(f));
    ofelialua_callfunction(x, top);
}

void t_ofeliadata::ofelialua_setfunction(t_ofeliadata *x, t_symbol *s, int top)
{
    lua_pushstring(L, s->s_name);
    ofelialua_callfunction(x, top);
}

void t_ofeliadata::ofelialua_setfunction(t_ofeliadata *x, int argc, t_atom *argv, int top)
{
    lua_newtable(L);
    for (int i = 0; i < argc; ++i)
    {
        lua_pushinteger(L, static_cast<lua_Integer>(i + 1));
        if (argv[i].a_type == A_FLOAT)
            lua_pushnumber(L, static_cast<lua_Number>(argv[i].a_w.w_float));
        else if (argv[i].a_type == A_SYMBOL)
            lua_pushstring(L, argv[i].a_w.w_symbol->s_name);
        lua_settable(L, -3);
    }
    ofelialua_callfunction(x, top);
}

void t_ofeliadata::ofelialua_dofunction(t_ofeliadata *x, t_symbol *s)
{
    int top; if (!ofelialua_isfunction(x, s, top)) return;
    ofelialua_setfunction(x, top);
}

void t_ofeliadata::ofelialua_dofunction(t_ofeliadata *x, t_symbol *s, t_floatarg f)
{
    int top; if (!ofelialua_isfunction(x, s, top)) return;
    ofelialua_setfunction(x, static_cast<lua_Number>(f), top);
}

void t_ofeliadata::ofelialua_dofunction(t_ofeliadata *x, t_symbol *s, t_symbol *s2)
{
    int top; if (!ofelialua_isfunction(x, s, top)) return;
    ofelialua_setfunction(x, s2, top);
}

void t_ofeliadata::ofelialua_dofunction(t_ofeliadata *x, t_symbol *s, int argc, t_atom *argv)
{
    int top; if (!ofelialua_isfunction(x, s, top)) return;
    ofelialua_setfunction(x, argc, argv, top);
}

void t_ofeliadata::ofelialua_realizedollar(t_ofeliadata *x, char **bufp, int *lengthp)
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
                t_symbol *s = canvas_realizedollar(x->canvas, gensym(dbuf));
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

void t_ofeliadata::ofelialua_run(t_ofeliadata *x)
{
    /* set the current working directory */
    CHDIR(canvas_getdir(x->canvas)->s_name);
    
    /* get the string from binbuf */
    int ntxt;
    char *txt;
    binbuf_gettext(x->binbuf, &txt, &ntxt);
    ofelialua_realizedollar(x, &txt, &ntxt);
    lua_settop(L, 0); /* empty the stack */
    ostringstream ss;
    const char *name = x->sym->s_name;
    ss << "package.preload['" << name << "'] = nil package.loaded['" << name << "'] = nil\n"
       << "package.preload['" << name << "'] = function(this) local ofelia = {}\n"
       << txt << "\nreturn ofelia end";
    
    /* run the lua chunk */
    const int ret = luaL_dostring(L, ss.str().c_str());
    freebytes(txt, ntxt);
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
    x->run = true;
    
    /* call the new function */
    t_ofeliadata::ofelialua_dofunction(x, gensym("new"));
}

void t_ofeliadata::ofelialua_get(t_ofeliadata *x, t_symbol *s, int argc, t_atom *argv)
{
    int top; switch (ofelialua_gettype(x, s, top))
    {
        case LUA_TNIL:
            if (!x->audio)
                outlet_bang(x->ob.ob_outlet);
            break;
        case LUA_TBOOLEAN:
            if (!x->audio)
                outlet_float(x->ob.ob_outlet, static_cast<t_float>(lua_toboolean(L, -1)));
            break;
        case LUA_TNUMBER:
            if (!x->audio)
                outlet_float(x->ob.ob_outlet, static_cast<t_float>(lua_tonumber(L, -1)));
            break;
        case LUA_TSTRING:
            if (!x->audio)
                outlet_symbol(x->ob.ob_outlet, gensym(lua_tostring(L, -1)));
            break;
        case LUA_TTABLE:
            if (!x->audio)
                ofelialua_outlettable(x);
            break;
        case LUA_TFUNCTION:
            if (!argc)
                ofelialua_setfunction(x, top);
            else if (argc == 1)
            {
                if (argv[0].a_type == A_FLOAT)
                    ofelialua_setfunction(x, argv[0].a_w.w_float, top);
                else if (argv[0].a_type == A_SYMBOL)
                    ofelialua_setfunction(x, argv[0].a_w.w_symbol, top);
            }
            else
                ofelialua_setfunction(x, argc, argv, top);
            break;
        default:
            break;
    }
    lua_pop(L, 1);
}

void t_ofeliadata::ofelialua_set(t_ofeliadata *x, t_symbol *s, int argc, t_atom *argv)
{
    int top; switch (ofelialua_gettype(x, s, top))
    {
        case LUA_TNIL:
            if (!argc)
                ofelialua_setvariable(s);
            else if (argc == 1)
            {
                if (argv[0].a_type == A_FLOAT)
                    ofelialua_setvariable(s, argv[0].a_w.w_float);
                else if (argv[0].a_type == A_SYMBOL)
                    ofelialua_setvariable(s, argv[0].a_w.w_symbol->s_name);
            }
            else
                ofelialua_setvariable(s, argc, argv);
            break;
        case LUA_TBOOLEAN:
            if (!argc)
                ofelialua_setvariable(s);
            else if (argv[0].a_type == A_FLOAT)
                ofelialua_setvariable(s, argv[0].a_w.w_float != 0);
            else
                error("lua: failed to set the boolean variable '%s'", s->s_name);
            break;
        case LUA_TNUMBER:
            if (!argc)
                ofelialua_setvariable(s);
            else if (argv[0].a_type == A_FLOAT)
                ofelialua_setvariable(s, argv[0].a_w.w_float);
            else
                error("lua: failed to set the number variable '%s'", s->s_name);
            break;
        case LUA_TSTRING:
            if (!argc)
                ofelialua_setvariable(s);
            else if (argv[0].a_type == A_SYMBOL)
                ofelialua_setvariable(s, argv[0].a_w.w_symbol->s_name);
            else
                error("lua: failed to set the string variable '%s'", s->s_name);
            break;
        case LUA_TTABLE:
            if (!argc)
                ofelialua_setvariable(s);
            else
                ofelialua_setvariable(s, argc, argv);
            break;
        case LUA_TFUNCTION:
            if (!argc)
                ofelialua_setfunction(x, top);
            else if (argc == 1)
            {
                if (argv[0].a_type == A_FLOAT)
                    ofelialua_setfunction(x, argv[0].a_w.w_float, top);
                else if (argv[0].a_type == A_SYMBOL)
                    ofelialua_setfunction(x, argv[0].a_w.w_symbol, top);
            }
            else
                ofelialua_setfunction(x, argc, argv, top);
            break;
        default:
            break;
    }
    lua_pop(L, 1);
}

void t_ofeliadata::ofelialua_free(t_ofeliadata *x)
{
    ofelialua_dofunction(x, gensym("free"));
    x->run = false;
}

t_symbol *t_ofeliadata::ofeliabuf_getunique(t_ofeliadata *x)
{
    t_canvas *canvas = x->canvas;
    t_gobj *gobj = reinterpret_cast<t_gobj *>(x), *obj;
    t_glist *glist = static_cast<t_glist*>(canvas);
    int index = 0;
    for (obj = glist->gl_list; obj && obj != gobj; obj = obj->g_next) index++;
    char buf[MAXPDSTRING];
    snprintf(buf, MAXPDSTRING, ".x%lx%d", reinterpret_cast<long unsigned int>(canvas), index);
    return gensym(buf);
}

void t_ofeliadata::ofeliabuf_init(t_ofeliadata *x, t_symbol *s)
{
    x->binbuf = binbuf_new();
    x->canvas = canvas_getcurrent();
    x->sym = *s->s_name ? s : ofeliabuf_getunique(x);
}

void t_ofeliadata::ofeliabuf_senditup(t_ofeliadata *x)
{
    if (!x->guiconnect)
        return;
    int i, ntxt;
    char *txt;
    binbuf_gettext(x->binbuf, &txt, &ntxt);
    sys_vgui(const_cast<char *>("pdtk_textwindow_clear .x%lx\n"), x);
    for (i = 0; i < ntxt; )
    {
        char *j = strchr(txt + i, '\n');
        if (j == nullptr) j = txt + ntxt;
        sys_vgui(const_cast<char *>("pdtk_textwindow_append .x%lx {%.*s\n}\n"),
                 x, j - txt - i, txt + i);
        i = static_cast<int>((j - txt) + 1);
    }
    sys_vgui(const_cast<char *>("pdtk_textwindow_setdirty .x%lx 0\n"), x);
    freebytes(txt, ntxt);
}

void t_ofeliadata::ofeliabuf_open(t_ofeliadata *x)
{
    if (x->guiconnect)
    {
        sys_vgui(const_cast<char *>("wm deiconify .x%lx\n"), x);
        sys_vgui(const_cast<char *>("raise .x%lx\n"), x);
        sys_vgui(const_cast<char *>("focus .x%lx.text\n"), x);
    }
    else
    {
        char buf[40];
        sys_vgui(const_cast<char *>("pdtk_textwindow_open .x%lx %dx%d {%s} %d\n"),
                 x, 600, 340, x->sym->s_name,
                 sys_hostfontsize(glist_getfont(x->canvas),
                                  glist_getzoom(x->canvas)));
        sprintf(buf, ".x%lx", reinterpret_cast<unsigned long>(x));
        x->guiconnect = guiconnect_new(&x->ob.ob_pd, gensym(buf));
        ofeliabuf_senditup(x);
    }
}

void t_ofeliadata::ofeliabuf_close(t_ofeliadata *x)
{
    sys_vgui(const_cast<char *>("pdtk_textwindow_doclose .x%lx\n"), x);
    if (x->guiconnect)
    {
        guiconnect_notarget(x->guiconnect, 1000);
        x->guiconnect = 0;
    }
}

void t_ofeliadata::ofeliabuf_addline(t_ofeliadata *x, t_symbol *s, int argc, t_atom *argv)
{
    t_binbuf *z = binbuf_new();
    binbuf_restore(z, argc, argv);
    binbuf_add(x->binbuf, binbuf_getnatom(z), binbuf_getvec(z));
    binbuf_free(z);
}

void t_ofeliadata::ofeliabuf_read(t_ofeliadata *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->run) ofelialua_free(x);
    int cr = 0;
    t_symbol *filename;
    while (argc && argv->a_type == A_SYMBOL &&
           *argv->a_w.w_symbol->s_name == '-')
    {
        if (!strcmp(argv->a_w.w_symbol->s_name, "-c"))
            cr = 1;
        else
        {
            error("ofelia read: unknown flag ...");
            postatom(argc, argv); endpost();
        }
        argc--; argv++;
    }
    if (argc && argv->a_type == A_SYMBOL)
    {
        filename = argv->a_w.w_symbol;
        argc--; argv++;
    }
    else
    {
        error("ofelia read: no file name given");
        return;
    }
    if (argc)
    {
        post("warning: ofelia define ignoring extra argument: ");
        postatom(argc, argv); endpost();
    }
    if (binbuf_read_via_canvas(x->binbuf, filename->s_name, x->canvas, cr))
        error("%s: read failed", filename->s_name);
    ofeliabuf_senditup(x);
    ofelialua_run(x);
}

void t_ofeliadata::ofeliabuf_write(t_ofeliadata *x, t_symbol *s, int argc, t_atom *argv)
{
    int cr = 0;
    t_symbol *filename;
    char buf[MAXPDSTRING];
    while (argc && argv->a_type == A_SYMBOL &&
           *argv->a_w.w_symbol->s_name == '-')
    {
        if (!strcmp(argv->a_w.w_symbol->s_name, "-c"))
            cr = 1;
        else
        {
            error("ofelia write: unknown flag ...");
            postatom(argc, argv); endpost();
        }
        argc--; argv++;
    }
    if (argc && argv->a_type == A_SYMBOL)
    {
        filename = argv->a_w.w_symbol;
        argc--; argv++;
    }
    else
    {
        error("ofelia write: no file name given");
        return;
    }
    if (argc)
    {
        post("warning: ofelia define ignoring extra argument: ");
        postatom(argc, argv); endpost();
    }
    canvas_makefilename(x->canvas, filename->s_name,
                        buf, MAXPDSTRING);
    if (binbuf_write(x->binbuf, buf, const_cast<char *>(""), cr))
        error("%s: write failed", filename->s_name);
}

void t_ofeliadata::ofeliabuf_free(t_ofeliadata *x)
{
    t_pd *x2;
    binbuf_free(x->binbuf);
    if (x->guiconnect)
    {
        sys_vgui(const_cast<char *>("destroy .x%lx\n"), x);
        guiconnect_notarget(x->guiconnect, 1000);
    }
    /* just in case we're still bound to #A from loading... */
    while ((x2 = pd_findbyclass(gensym("#A"), ofelia_define_class)))
        pd_unbind(x2, gensym("#A"));
}

t_int *t_ofeliadata::ofeliasig_perform(t_int *w)
{
    t_ofeliadata *x = reinterpret_cast<t_ofeliadata *>(w[1]);
    const int nsamples = static_cast<int>(w[2]);
    const int ninlets = x->ninlets;
    const int noutlets = x->noutlets;
    int top; if (ofelialua_isfunction(x, gensym("perform"), top))
    {
        for (int i = 0; i < ninlets; ++i)
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
        lua_call(L, ninlets, noutlets);
        for (int i = 0; i < noutlets; ++i)
        {
            t_float *out = reinterpret_cast<t_float *>(w[i + 3 + ninlets]);
            const int index = -noutlets - 1 + i;
            for (int j = 0; j < nsamples; ++j)
            {
                lua_pushinteger(L, static_cast<lua_Integer>(j + 1));
                lua_gettable(L, index);
                out[j] = static_cast<t_float>(lua_tonumber(L, -1));
                lua_pop(L, 1);
            }
        }
    }
    else
    {
        for (int i = 0; i < noutlets; ++i)
        {
            t_float *in = reinterpret_cast<t_float *>(w[3]);
            t_float *out = reinterpret_cast<t_float *>(w[i + 3 + ninlets]);
            for (int j = 0; j < nsamples; ++j)
                out[j] = in[j];
        }
        lua_pop(L, 1);
    }
    return (w + ninlets + noutlets + 3);
}

t_ofeliadata *t_ofeliadata::ofeliadata_getdata(t_symbol *s)
{
    if (s)       /* named ofelia object */
    {
        t_ofeliadata *y = reinterpret_cast<t_ofeliadata *>(pd_findbyclass(s, ofelia_define_class));
        if (y)
            return y;
        else
        {
            error("ofelia: couldn't find '%s'", s->s_name);
            return nullptr;
        }
    }
    return nullptr;
}

static void *ofelia_define_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofelia_define *x = reinterpret_cast<t_ofelia_define *>(pd_new(ofelia_define_class));
    t_symbol *asym = gensym("#A");
    x->data.keep = false;
    x->data.audio = false;
    x->data.run = false;
    x->bindsym = &s_;
    while (argc && argv->a_type == A_SYMBOL &&
           *argv->a_w.w_symbol->s_name == '-')
    {
        const char *flag = argv->a_w.w_symbol->s_name;
        if (!strcmp(flag, "-k"))
            x->data.keep = true;
        else if (!x->data.audio && !strncmp(flag, "-a", 2) && (strlen(flag) == 4) &&
                 isdigit(flag[2]) && isdigit(flag[3]) && (flag[2] - '0'))
        {
            int ninlets = flag[2] - '0';
            int noutlets = flag[3] - '0';
            x->data.ninlets = ninlets;
            x->data.noutlets = noutlets;
            x->data.w = static_cast<t_int **>(getbytes(sizeof(t_int *) * (ninlets + noutlets + 2)));
            ninlets--; /* no need to add the first inlet */
            x->data.inlets = static_cast<t_inlet **>(getbytes(sizeof(t_inlet *) * ninlets));
            x->data.outlets = static_cast<t_outlet **>(getbytes(sizeof(t_outlet *) * noutlets));
            for (int i = 0; i < ninlets; ++i)
                x->data.inlets[i] = inlet_new(&x->data.ob, &x->data.ob.ob_pd, &s_signal, &s_signal);
            for (int i = 0; i < noutlets; ++i)
                x->data.outlets[i] = outlet_new(&x->data.ob, &s_signal);
            x->data.f = 0;
            x->data.audio = true;
        }
        else
            error("ofelia define: invalid flag '%s'", flag);
        argc--; argv++;
    }
    if (argc && argv->a_type == A_SYMBOL)
    {
        pd_bind(&x->data.ob.ob_pd, argv->a_w.w_symbol);
        x->bindsym = argv->a_w.w_symbol;
        argc--; argv++;
    }
    if (argc)
    {
        post("warning: ofelia define ignoring extra argument: ");
        postatom(argc, argv); endpost();
    }
    t_ofeliadata::ofeliabuf_init(&x->data, x->bindsym);
    /* bashily unbind #A -- this would create garbage if #A were
     multiply bound but we believe in this context it's at most
     bound to whichever ofelia_define or array was created most recently */
    asym->s_thing = 0;
    /* and now bind #A to us to receive following messages in the
     saved file or copy buffer */
    pd_bind(&x->data.ob.ob_pd, asym);
    if (!x->data.audio) outlet_new(&x->data.ob, &s_list);
    return (x);
}

static void ofelia_define_bang(t_ofelia_define *x)
{
    t_ofeliadata::ofelialua_dofunction(&x->data, &s_bang);
}

static void ofelia_define_float(t_ofelia_define *x, t_floatarg f)
{
    t_ofeliadata::ofelialua_dofunction(&x->data, &s_float, f);
    if (x->data.audio) x->data.f = f;
}

static void ofelia_define_symbol(t_ofelia_define *x, t_symbol *s)
{
    t_ofeliadata::ofelialua_dofunction(&x->data, &s_symbol, s);
}

static void ofelia_define_list(t_ofelia_define *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliadata::ofelialua_dofunction(&x->data, &s_list, argc, argv);
}

static void ofelia_define_anything(t_ofelia_define *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliadata::ofelialua_set(&x->data, s, argc, argv);
}

static void ofelia_define_clear(t_ofelia_define *x)
{
    t_ofeliadata::ofelialua_free(&x->data);
    binbuf_clear(x->data.binbuf);
    t_ofeliadata::ofeliabuf_senditup(&x->data);
}

static void ofelia_define_set(t_ofelia_define *x, t_symbol *s, int argc, t_atom *argv)
{
    binbuf_restore(x->data.binbuf, argc, argv);
    t_ofeliadata::ofeliabuf_senditup(&x->data);
    t_ofeliadata::ofelialua_run(&x->data);
}

static void ofelia_define_save(t_gobj *z, t_binbuf *bb)
{
    t_ofelia_define *x = reinterpret_cast<t_ofelia_define *>(z);
    t_object *ob = &x->data.ob;
    binbuf_addv(bb, const_cast<char *>("ssff"), &s__X, gensym("obj"),
                static_cast<float>(ob->te_xpix),
                static_cast<float>(ob->te_ypix));
    binbuf_addbinbuf(bb, ob->ob_binbuf);
    binbuf_addsemi(bb);
    if (x->data.keep)
    {
        binbuf_addv(bb, const_cast<char *>("ss"), gensym("#A"), gensym("set"));
        binbuf_addbinbuf(bb, x->data.binbuf);
        binbuf_addsemi(bb);
    }
    obj_saveformat(ob, bb);
}

static void ofelia_define_dsp(t_ofelia_define *x, t_signal **sp)
{
    if (!x->data.audio) return;
    const int ninoutlets = x->data.ninlets + x->data.noutlets;
    t_int **w = x->data.w;
    w[0] = reinterpret_cast<t_int *>(&x->data);
    w[1] = reinterpret_cast<t_int *>(sp[0]->s_n);
    for (int i = 0; i < ninoutlets; i++)
        w[i+2] = reinterpret_cast<t_int *>(sp[i]->s_vec);
    dsp_addv(t_ofeliadata::ofeliasig_perform, ninoutlets + 2, reinterpret_cast<t_int *>(w));
}

/* notification from GUI that we've been updated */
static void ofelia_define_notify(t_ofelia_define *x)
{
    t_ofeliadata::ofeliabuf_senditup(&x->data);
    t_ofeliadata::ofelialua_run(&x->data);
}

static void ofelia_define_free(t_ofelia_define *x)
{
    t_ofeliadata::ofelialua_free(&x->data);
    t_ofeliadata::ofeliabuf_free(&x->data);
    if (x->bindsym != &s_)
        pd_unbind(&x->data.ob.ob_pd, x->bindsym);
    if (x->data.audio)
    {
        int ninlets = x->data.ninlets;
        int noutlets = x->data.noutlets;
        freebytes(x->data.w, sizeof(t_int *) * (ninlets + noutlets + 2));
        ninlets--; /* no need to free the first inlet */
        for (int i = 0; i < ninlets; ++i)
            inlet_free(x->data.inlets[i]);
        for (int i = 0; i < noutlets; ++i)
            outlet_free(x->data.outlets[i]);
    }
}

/* parse buffer-finding arguments */
static void ofeliaclient_argparse(t_ofeliaclient *x, int *argcp, t_atom **argvp, t_symbol *s)
{
    int argc = *argcp;
    t_atom *argv = *argvp;
    x->data.audio = false;
    x->data.run = false;
    x->data.sym = &s_;
    while (argc && argv->a_type == A_SYMBOL &&
           *argv->a_w.w_symbol->s_name == '-')
    {
        const char *flag = argv->a_w.w_symbol->s_name;
        if (!x->data.audio && !strncmp(flag, "-a", 2) && (strlen(flag) == 4) &&
                 isdigit(flag[2]) && isdigit(flag[3]) && (flag[2] - '0'))
        {
            int ninlets = flag[2] - '0';
            int noutlets = flag[3] - '0';
            x->data.ninlets = ninlets;
            x->data.noutlets = noutlets;
            x->data.w = static_cast<t_int **>(getbytes(sizeof(t_int *) * (ninlets + noutlets + 2)));
            ninlets--; /* no need to add the first inlet */
            x->data.inlets = static_cast<t_inlet **>(getbytes(sizeof(t_inlet *) * ninlets));
            x->data.outlets = static_cast<t_outlet **>(getbytes(sizeof(t_outlet *) * noutlets));
            for (int i = 0; i < ninlets; ++i)
                x->data.inlets[i] = inlet_new(&x->data.ob, &x->data.ob.ob_pd, &s_signal, &s_signal);
            for (int i = 0; i < noutlets; ++i)
                x->data.outlets[i] = outlet_new(&x->data.ob, &s_signal);
            x->data.f = 0;
            x->data.audio = true;
        }
        else
            error("ofelia %s: invalid flag '%s'", s->s_name, flag);
        argc--; argv++;
    }
    if (argc && argv->a_type == A_SYMBOL)
    {
        x->data.sym = argv->a_w.w_symbol;
        argc--; argv++;
    }
    if (argc)
    {
        post("warning: ofelia %s ignoring extra argument: ", s->s_name);
        postatom(argc, argv); endpost();
    }
    if (!x->data.audio)
    {
        symbolinlet_new(&x->data.ob, &x->data.sym);
        outlet_new(&x->data.ob, &s_list);
    }
    *argcp = argc;
    *argvp = argv;
}

/* ---  ofeliaclient - common code for objects that refer to ofelia data -- */
static void ofeliaclient_bang(t_ofeliaclient *x)
{
    t_ofeliadata *y = t_ofeliadata::ofeliadata_getdata(x->data.sym);
    if (y == nullptr) return;
    x->data.run = y->run;
    t_ofeliadata::ofelialua_dofunction(&x->data, &s_bang);
}

static void ofeliaclient_float(t_ofeliaclient *x, t_floatarg f)
{
    t_ofeliadata *y = t_ofeliadata::ofeliadata_getdata(x->data.sym);
    if (y == nullptr) return;
    x->data.run = y->run;
    t_ofeliadata::ofelialua_dofunction(&x->data, &s_float, f);
    if (x->data.audio) x->data.f = f;
}

static void ofeliaclient_symbol(t_ofeliaclient *x, t_symbol *s)
{
    t_ofeliadata *y = t_ofeliadata::ofeliadata_getdata(x->data.sym);
    if (y == nullptr) return;
    x->data.run = y->run;
    t_ofeliadata::ofelialua_dofunction(&x->data, &s_symbol, s);
}

static void ofeliaclient_list(t_ofeliaclient *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliadata *y = t_ofeliadata::ofeliadata_getdata(x->data.sym);
    if (y == nullptr) return;
    x->data.run = y->run;
    t_ofeliadata::ofelialua_dofunction(&x->data, &s_list, argc, argv);
}

static void ofeliaclient_anything_get(t_ofeliaclient *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliadata *y = t_ofeliadata::ofeliadata_getdata(x->data.sym);
    if (y == nullptr) return;
    x->data.run = y->run;
    t_ofeliadata::ofelialua_get(&x->data, s, argc, argv);
}

static void ofeliaclient_anything_set(t_ofeliaclient *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliadata *y = t_ofeliadata::ofeliadata_getdata(x->data.sym);
    if (y == nullptr) return;
    x->data.run = y->run;
    t_ofeliadata::ofelialua_set(&x->data, s, argc, argv);
}

static void ofeliaclient_dsp(t_ofeliaclient *x, t_signal **sp)
{
    if (!x->data.audio) return;
    t_ofeliadata *y = t_ofeliadata::ofeliadata_getdata(x->data.sym);
    if (y == nullptr) return;
    x->data.run = y->run;
    const int ninoutlets = x->data.ninlets + x->data.noutlets;
    t_int **w = x->data.w;
    w[0] = reinterpret_cast<t_int *>(y);
    w[1] = reinterpret_cast<t_int *>(sp[0]->s_n);
    for (int i = 0; i < ninoutlets; i++)
        w[i + 2] = reinterpret_cast<t_int *>(sp[i]->s_vec);
    dsp_addv(t_ofeliadata::ofeliasig_perform, ninoutlets + 2, reinterpret_cast<t_int *>(w));
}

static void ofeliaclient_free(t_ofeliaclient *x)
{
    if (x->data.audio)
    {
        int ninlets = x->data.ninlets;
        int noutlets = x->data.noutlets;
        freebytes(x->data.w, sizeof(t_int *) * (ninlets + noutlets + 2));
        ninlets--; /* no need to free the first inlet */
        for (int i = 0; i < ninlets; ++i)
            inlet_free(x->data.inlets[i]);
        for (int i = 0; i < noutlets; ++i)
            outlet_free(x->data.outlets[i]);
    }
}

/* --------- ofelia_get object ---------- */
static void *ofelia_get_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofelia_get *x = reinterpret_cast<t_ofelia_get *>(pd_new(ofelia_get_class));
    ofeliaclient_argparse(&x->client, &argc, &argv, gensym("get"));
    return (x);
}

static void ofelia_get_bang(t_ofelia_get *x)
{
    ofeliaclient_bang(&x->client);
}

static void ofelia_get_float(t_ofelia_get *x, t_floatarg f)
{
    ofeliaclient_float(&x->client, f);
}

static void ofelia_get_symbol(t_ofelia_get *x, t_symbol *s)
{
    ofeliaclient_symbol(&x->client, s);
}

static void ofelia_get_list(t_ofelia_get *x, t_symbol *s, int argc, t_atom *argv)
{
    ofeliaclient_list(&x->client, s, argc, argv);
}

static void ofelia_get_anything(t_ofelia_get *x, t_symbol *s, int argc, t_atom *argv)
{
    ofeliaclient_anything_get(&x->client, s, argc, argv);
}

static void ofelia_get_dsp(t_ofelia_get *x, t_signal **sp)
{
    ofeliaclient_dsp(&x->client, sp);
}

/* --------- ofelia_set object ---------- */
static void *ofelia_set_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofelia_set *x = reinterpret_cast<t_ofelia_set *>(pd_new(ofelia_set_class));
    ofeliaclient_argparse(&x->client, &argc, &argv, gensym("set"));
    return (x);
}

static void ofelia_set_bang(t_ofelia_set *x)
{
    ofeliaclient_bang(&x->client);
}

static void ofelia_set_float(t_ofelia_set *x, t_floatarg f)
{
    ofeliaclient_float(&x->client, f);
}

static void ofelia_set_symbol(t_ofelia_set *x, t_symbol *s)
{
    ofeliaclient_symbol(&x->client, s);
}

static void ofelia_set_list(t_ofelia_set *x, t_symbol *s, int argc, t_atom *argv)
{
    ofeliaclient_list(&x->client, s, argc, argv);
}

static void ofelia_set_anything(t_ofelia_set *x, t_symbol *s, int argc, t_atom *argv)
{
    ofeliaclient_anything_set(&x->client, s, argc, argv);
}

static void ofelia_set_dsp(t_ofelia_get *x, t_signal **sp)
{
    ofeliaclient_dsp(&x->client, sp);
}

/* --------- ofelia_exists object ---------- */
static void *ofelia_exists_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofelia_exists *x = reinterpret_cast<t_ofelia_exists *>(pd_new(ofelia_exists_class));
    x->client.data.sym = &s_;
    while (argc && argv->a_type == A_SYMBOL &&
           *argv->a_w.w_symbol->s_name == '-')
    {
        const char *flag = argv->a_w.w_symbol->s_name;
        error("ofelia exists: invalid flag '%s'", flag);
        argc--; argv++;
    }
    if (argc && argv->a_type == A_SYMBOL)
    {
        x->client.data.sym = argv->a_w.w_symbol;
        argc--; argv++;
    }
    if (argc)
    {
        post("warning: ofelia exists ignoring extra argument: ");
        postatom(argc, argv); endpost();
    }
    symbolinlet_new(&x->client.data.ob, &x->client.data.sym);
    outlet_new(&x->client.data.ob, &s_float);
    return (x);
}

static void ofelia_exists_bang(t_ofelia_exists *x)
{
    t_ofeliadata *y = reinterpret_cast<t_ofeliadata *>(pd_findbyclass(x->client.data.sym, ofelia_define_class));
    outlet_float(x->client.data.ob.ob_outlet, static_cast<t_float>(y != nullptr));
}

/* --- overall creator for "ofelia" objects: dispatch to "ofelia define" etc --- */
static void *ofelia_new(t_symbol *s, int argc, t_atom *argv)
{
    if (!argc || argv[0].a_type != A_SYMBOL)
        pd_this->pd_newest = static_cast<t_pd *>(ofelia_define_new(s, argc, argv));
    else
    {
        char *str = argv[0].a_w.w_symbol->s_name;
        if (!strcmp(str, "d") || !strcmp(str, "define"))
            pd_this->pd_newest = static_cast<t_pd *>(ofelia_define_new(s, argc - 1, argv + 1));
        else if (!strcmp(str, "get"))
            pd_this->pd_newest = static_cast<t_pd *>(ofelia_get_new(s, argc - 1, argv + 1));
        else if (!strcmp(str, "set"))
            pd_this->pd_newest = static_cast<t_pd *>(ofelia_set_new(s, argc - 1, argv + 1));
        else if (!strcmp(str, "e") || !strcmp(str, "exists"))
            pd_this->pd_newest = static_cast<t_pd *>(ofelia_exists_new(s, argc - 1, argv + 1));
        else
        {
            error("list %s: unknown function", str);
            pd_this->pd_newest = 0;
        }
    }
    return (pd_this->pd_newest);
}

void ofelia_setup()
{
    /* initialize lua */
    t_ofeliadata::L = luaL_newstate();
    if (t_ofeliadata::L == nullptr)
    {
        error("ofelia: failed initializing lua");
        return;
    }
    /* open libs */
    luaL_openlibs(t_ofeliadata::L);
    luaopen_of(t_ofeliadata::L);
    luaopen_pd(t_ofeliadata::L);
    
    /* clear stack since opening libs leaves tables on the stack */
    lua_settop(t_ofeliadata::L, 0);
    
    ofelia_define_class = class_new(gensym("ofelia define"),
                                    reinterpret_cast<t_newmethod>(ofelia_define_new),
                                    reinterpret_cast<t_method>(ofelia_define_free),
                                    sizeof(t_ofelia_define), 0, A_GIMME, 0);
    class_addmethod(ofelia_define_class, reinterpret_cast<t_method>(t_ofeliadata::ofeliabuf_open),
                    gensym("click"), A_NULL, 0);
    class_addmethod(ofelia_define_class, reinterpret_cast<t_method>(t_ofeliadata::ofeliabuf_close),
                    gensym("close"), A_NULL, 0);
    class_addmethod(ofelia_define_class, reinterpret_cast<t_method>(t_ofeliadata::ofeliabuf_addline),
                    gensym("addline"), A_GIMME, 0);
    class_addmethod(ofelia_define_class, reinterpret_cast<t_method>(ofelia_define_dsp), gensym("dsp"), A_CANT, 0);
    class_addmethod(ofelia_define_class, reinterpret_cast<t_method>(ofelia_define_notify),
                    gensym("notify"), A_NULL, 0);
    class_addmethod(ofelia_define_class, reinterpret_cast<t_method>(ofelia_define_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofelia_define_class, reinterpret_cast<t_method>(ofelia_define_clear),
                    gensym("clear"), A_NULL, 0);
    class_addmethod(ofelia_define_class, reinterpret_cast<t_method>(t_ofeliadata::ofeliabuf_write),
                    gensym("write"), A_GIMME, 0);
    class_addmethod(ofelia_define_class, reinterpret_cast<t_method>(t_ofeliadata::ofeliabuf_read),
                    gensym("read"), A_GIMME, 0);
    class_setsavefn(ofelia_define_class, ofelia_define_save);
    CLASS_MAINSIGNALIN(ofelia_define_class, t_ofelia_define, data.f);
    class_addbang(ofelia_define_class, ofelia_define_bang);
    class_addfloat(ofelia_define_class, ofelia_define_float);
    class_addsymbol(ofelia_define_class, ofelia_define_symbol);
    class_addlist(ofelia_define_class, ofelia_define_list);
    class_addanything(ofelia_define_class, ofelia_define_anything);
    class_sethelpsymbol(ofelia_define_class, gensym("ofelia-object"));
    class_addcreator(reinterpret_cast<t_newmethod>(ofelia_new), gensym("ofelia"), A_GIMME, 0);
    ofelia_get_class = class_new(gensym("ofelia get"),
                                 reinterpret_cast<t_newmethod>(ofelia_get_new),
                                 reinterpret_cast<t_method>(ofeliaclient_free),
                                 sizeof(t_ofelia_get), 0, A_GIMME, 0);
    class_addmethod(ofelia_get_class, reinterpret_cast<t_method>(ofelia_get_dsp), gensym("dsp"), A_CANT, 0);
    CLASS_MAINSIGNALIN(ofelia_get_class, t_ofelia_get, client.data.f);
    class_addbang(ofelia_get_class, ofelia_get_bang);
    class_addfloat(ofelia_get_class, ofelia_get_float);
    class_addsymbol(ofelia_get_class, ofelia_get_symbol);
    class_addlist(ofelia_get_class, ofelia_get_list);
    class_addanything(ofelia_get_class, ofelia_get_anything);
    class_sethelpsymbol(ofelia_get_class, gensym("ofelia-object"));
    ofelia_set_class = class_new(gensym("ofelia set"),
                                 reinterpret_cast<t_newmethod>(ofelia_set_new),
                                 reinterpret_cast<t_method>(ofeliaclient_free),
                                 sizeof(t_ofelia_set), 0, A_GIMME, 0);
    class_addmethod(ofelia_set_class, reinterpret_cast<t_method>(ofelia_set_dsp), gensym("dsp"), A_CANT, 0);
    CLASS_MAINSIGNALIN(ofelia_set_class, t_ofelia_set, client.data.f);
    class_addbang(ofelia_set_class, ofelia_set_bang);
    class_addfloat(ofelia_set_class, ofelia_set_float);
    class_addsymbol(ofelia_set_class, ofelia_set_symbol);
    class_addlist(ofelia_set_class, ofelia_set_list);
    class_addanything(ofelia_set_class, ofelia_set_anything);
    class_sethelpsymbol(ofelia_set_class, gensym("ofelia-object"));
    ofelia_exists_class = class_new(gensym("ofelia exists"),
                                    reinterpret_cast<t_newmethod>(ofelia_exists_new),
                                    0, sizeof(t_ofelia_exists), 0, A_GIMME, 0);
    class_addbang(ofelia_exists_class, ofelia_exists_bang);
    class_sethelpsymbol(ofelia_exists_class, gensym("ofelia-object"));
}
