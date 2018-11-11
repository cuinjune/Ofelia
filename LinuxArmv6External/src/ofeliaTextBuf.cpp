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

#include "ofeliaTextBuf.h"
#include "ofeliaData.h"
#include "ofeliaDefine.h"
#include <string.h>

void ofeliaTextBuf::senditup()
{
    if (dataPtr->isDirectMode) return;
    if (!dataPtr->guiconnect) return;
    int i, ntxt;
    char *txt;
    binbuf_gettext(dataPtr->binbuf, &txt, &ntxt);
    sys_vgui(const_cast<char *>("pdtk_textwindow_clear .x%lx\n"), dataPtr);
    for (i = 0; i < ntxt; )
    {
        char *j = strchr(txt + i, '\n');
        if (j == nullptr) j = txt + ntxt;
        sys_vgui(const_cast<char *>("pdtk_textwindow_append .x%lx {%.*s\n}\n"),
                 dataPtr, j - txt - i, txt + i);
        i = static_cast<int>((j - txt) + 1);
    }
    sys_vgui(const_cast<char *>("pdtk_textwindow_setdirty .x%lx 0\n"), dataPtr);
    freebytes(txt, ntxt);
}

void ofeliaTextBuf::openMethod()
{
    if (dataPtr->isDirectMode) return;
    if (dataPtr->guiconnect)
    {
        sys_vgui(const_cast<char *>("wm deiconify .x%lx\n"), dataPtr);
        sys_vgui(const_cast<char *>("raise .x%lx\n"), dataPtr);
        sys_vgui(const_cast<char *>("focus .x%lx.text\n"), dataPtr);
    }
    else
    {
        char buf[40];
        sys_vgui(const_cast<char *>("pdtk_textwindow_open .x%lx %dx%d {%s} %d\n"),
                 dataPtr, 600, 340, dataPtr->sym->s_name,
                 sys_hostfontsize(glist_getfont(dataPtr->canvas),
                                  glist_getzoom(dataPtr->canvas)));
        sprintf(buf, ".x%lx", reinterpret_cast<unsigned long>(dataPtr));
        dataPtr->guiconnect = guiconnect_new(&dataPtr->ob.ob_pd, gensym(buf));
        senditup();
    }
}

void ofeliaTextBuf::closeMethod()
{
    if (dataPtr->isDirectMode) return;
    sys_vgui(const_cast<char *>("pdtk_textwindow_doclose .x%lx\n"), dataPtr);
    if (dataPtr->guiconnect)
    {
        guiconnect_notarget(dataPtr->guiconnect, 1000);
        dataPtr->guiconnect = 0;
    }
}

void ofeliaTextBuf::addLineMethod(t_symbol *s, int argc, t_atom *argv)
{
    if (dataPtr->isDirectMode) return;
    t_binbuf *z = binbuf_new();
    binbuf_restore(z, argc, argv);
    binbuf_add(dataPtr->binbuf, binbuf_getnatom(z), binbuf_getvec(z));
    binbuf_free(z);
}

void ofeliaTextBuf::readMethod(t_symbol *s, int argc, t_atom *argv)
{
    if (dataPtr->isDirectMode) return;
    dataPtr->lua.doFreeFunction();
    int cr = 0;
    t_symbol *filename;
    while (argc && argv->a_type == A_SYMBOL &&
           *argv->a_w.w_symbol->s_name == '-')
    {
        const char *flag = argv->a_w.w_symbol->s_name;
        if (!strcmp(flag, "-c"))
            cr = 1;
        else
            error("ofelia read: unknown flag '%s'", flag);
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
    if (binbuf_read_via_canvas(dataPtr->binbuf, filename->s_name, dataPtr->canvas, cr))
        error("%s: read failed", filename->s_name);
    senditup();
    dataPtr->lua.doText();
}

void ofeliaTextBuf::writeMethod(t_symbol *s, int argc, t_atom *argv)
{
    if (dataPtr->isDirectMode) return;
    int cr = 0;
    t_symbol *filename;
    char buf[MAXPDSTRING];
    while (argc && argv->a_type == A_SYMBOL &&
           *argv->a_w.w_symbol->s_name == '-')
    {
        const char *flag = argv->a_w.w_symbol->s_name;
        if (!strcmp(flag, "-c"))
            cr = 1;
        else
            error("ofelia write: unknown flag '%s'", flag);
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
    canvas_makefilename(dataPtr->canvas, filename->s_name,
                        buf, MAXPDSTRING);
    if (binbuf_write(dataPtr->binbuf, buf, const_cast<char *>(""), cr))
        error("%s: write failed", filename->s_name);
}

void ofeliaTextBuf::free()
{
    if (dataPtr->isDirectMode) return;
    t_pd *x2;
    binbuf_free(dataPtr->binbuf);
    if (dataPtr->guiconnect)
    {
        sys_vgui(const_cast<char *>("destroy .x%lx\n"), dataPtr);
        guiconnect_notarget(dataPtr->guiconnect, 1000);
    }
    /* just in case we're still bound to #A from loading... */
    while ((x2 = pd_findbyclass(gensym("#A"), ofeliaDefine::pdClass)))
        pd_unbind(x2, gensym("#A"));
}

