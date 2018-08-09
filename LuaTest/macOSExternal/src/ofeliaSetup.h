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

#ifndef ofeliaSetup_h
#define ofeliaSetup_h

#pragma once

#include "m_pd.h"
#include "g_canvas.h"
#include "ofMain.h"
#include "ofxLua.h"

extern "C"
{
    /* ofelia define object - ofelia data, accessible by other accessor objects */
    static t_class *ofelia_define_class;
    typedef struct _ofeliadata t_ofeliadata;
    typedef struct _ofeliadata
    {
        /* lua */
        static lua_State *L;
        static bool ofelialua_require(t_ofeliadata *x);
        static int  ofelialua_gettype(t_ofeliadata *x, t_symbol *s);
        static int  ofelialua_gettype(t_ofeliadata *x, t_symbol *s, int &top);
        static bool ofelialua_isnil(t_ofeliadata *x, t_symbol *s);
        static bool ofelialua_isboolean(t_ofeliadata *x, t_symbol *s);
        static bool ofelialua_isnumber(t_ofeliadata *x, t_symbol *s);
        static bool ofelialua_isstring(t_ofeliadata *x, t_symbol *s);
        static bool ofelialua_istable(t_ofeliadata *x, t_symbol *s);
        static bool ofelialua_isfunction(t_ofeliadata *x, t_symbol *s, int &top);
        static void ofelialua_setvariable(t_symbol *s);
        static void ofelialua_setvariable(t_symbol *s, bool b);
        static void ofelialua_setvariable(t_symbol *s, t_floatarg f);
        static void ofelialua_setvariable(t_symbol *s, t_symbol *s2);
        static void ofelialua_setvariable(t_symbol *s, int argc, t_atom *argv);
        static void ofelialua_dovariable(t_ofeliadata *x, t_symbol *s);
        static void ofelialua_dovariable(t_ofeliadata *x, t_symbol *s, bool b);
        static void ofelialua_dovariable(t_ofeliadata *x, t_symbol *s, t_floatarg f);
        static void ofelialua_dovariable(t_ofeliadata *x, t_symbol *s, t_symbol *s2);
        static void ofelialua_dovariable(t_ofeliadata *x, t_symbol *s, int argc, t_atom *argv);
        static void ofelialua_outlettable(t_ofeliadata *x);
        static void ofelialua_outletret(t_ofeliadata *x);
        static void ofelialua_callfunction(t_ofeliadata *x, int top);
        static void ofelialua_setfunction(t_ofeliadata *x, int top);
        static void ofelialua_setfunction(t_ofeliadata *x, t_floatarg f, int top);
        static void ofelialua_setfunction(t_ofeliadata *x, t_symbol *s, int top);
        static void ofelialua_setfunction(t_ofeliadata *x, int argc, t_atom *argv,int top);
        static void ofelialua_dofunction(t_ofeliadata *x, t_symbol *s);
        static void ofelialua_dofunction(t_ofeliadata *x, t_symbol *s, t_floatarg f);
        static void ofelialua_dofunction(t_ofeliadata *x, t_symbol *s, t_symbol *s2);
        static void ofelialua_dofunction(t_ofeliadata *x, t_symbol *s, int argc, t_atom *argv);
        static void ofelialua_realizedollar(t_ofeliadata *x, char **bufp, int *lengthp);
        static void ofelialua_run(t_ofeliadata *x);
        static void ofelialua_get(t_ofeliadata *x, t_symbol *s, int argc, t_atom *argv);
        static void ofelialua_set(t_ofeliadata *x, t_symbol *s, int argc, t_atom *argv);
        static void ofelialua_free(t_ofeliadata *x);
        
        /* buf */
        t_object ob;
        t_binbuf *binbuf;
        t_canvas *canvas;
        t_guiconnect *guiconnect;
        t_symbol *sym;
        static t_symbol *ofeliabuf_getunique(t_ofeliadata *x);
        static void ofeliabuf_init(t_ofeliadata *x, t_symbol *s);
        static void ofeliabuf_senditup(t_ofeliadata *x);
        static void ofeliabuf_open(t_ofeliadata *x);
        static void ofeliabuf_close(t_ofeliadata *x);
        static void ofeliabuf_addline(t_ofeliadata *x, t_symbol *s, int argc, t_atom *argv);
        static void ofeliabuf_read(t_ofeliadata *x, t_symbol *s, int argc, t_atom *argv);
        static void ofeliabuf_write(t_ofeliadata *x, t_symbol *s, int argc, t_atom *argv);
        static void ofeliabuf_free(t_ofeliadata *x);
        
        /* sig */
        t_inlet **inlets;
        t_outlet **outlets;
        int ninlets;
        int noutlets;
        t_int **w;
        t_float f;
        static t_int *ofeliasig_perform(t_int *w);
        
        /* data */
        static t_ofeliadata *ofeliadata_getdata(t_symbol *s);
        
        /* states */
        bool keep;  /* whether to embed contents in patch on save */
        bool audio; /* whether to use audio inlets and outlets */
        bool run; /* whether the lua chunk is run or not */
    } t_ofeliadata;
    
    typedef struct _ofelia_define
    {
        t_ofeliadata data;
        t_symbol *bindsym;
    } t_ofelia_define;
    
    /* ---  ofeliaclient - common code for objects that refer to ofelia data -- */
    typedef struct _ofeliaclient
    {
        t_ofeliadata data;
    } t_ofeliaclient;
    
    /* --------- ofelia get object ---------- */
    static t_class *ofelia_get_class;
    
    typedef struct _ofelia_get
    {
        t_ofeliaclient client;
    } t_ofelia_get;
    
    /* --------- ofelia set object ---------- */
    static t_class *ofelia_set_class;
    
    typedef struct _ofelia_set
    {
        t_ofeliaclient client;
    } t_ofelia_set;
    
    /* --------- ofelia exists object ---------- */
    static t_class *ofelia_exists_class;
    
    typedef struct _ofelia_exists
    {
        t_ofeliaclient client;
    } t_ofelia_exists;
    
    int luaopen_of(lua_State *L);
    int luaopen_pd(lua_State *L);
    void ofelia_setup();
}

#endif /* ofeliaSetup_h */
