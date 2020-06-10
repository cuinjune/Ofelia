#include "ofxOfeliaTextBuf.h"
#include "ofxOfeliaData.h"
#include "ofxOfeliaDefine.h"
#include "ofUtils.h"
#include <cstdio>
#include <cctype>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#ifdef _WIN32
#include <winsock2.h>
#endif

void ofxOfeliaTextBuf::loadScript()
{
    sys_gui(const_cast<char *>("foreach pathdir [concat $::sys_searchpath $::sys_temppath $::sys_staticpath] {\n"
                               "    set dir [file normalize $pathdir]\n"
                               "    if { ! [file isdirectory $dir]} {continue}\n"
                               "    foreach filename [glob -directory $dir -nocomplain -types {f} -- \\n"
                               "                          ofelia/ofelia_*.tcl ofelia_*.tcl] {\n"
                               "        set ::current_plugin_loadpath [file dirname $filename]\n"
                               "        load_plugin_script $filename }\n}\n"));
}

bool ofxOfeliaTextBuf::canvasOpen(const t_canvas *canvas, const std::string &fileName,
                                  std::string &dirResult, std::string &fileNameResult)
{
    if (canvas == nullptr) return false;
    int fileDesc;
    char buf[MAXPDSTRING], *bufPtr;
    if ((fileDesc = canvas_open(canvas, fileName.c_str(), "", buf, &bufPtr, MAXPDSTRING, 0)) < 0)
        return false;
#if defined(TARGET_EXTERNAL)
#ifdef _WIN32
    closesocket(fileDesc);
#else
    close(fileDesc);
#endif
#endif
    dirResult = buf;
    fileNameResult = bufPtr;
    return true;
}

void ofxOfeliaTextBuf::senditup()
{
    if (dataPtr->isDirectMode) return;
    if (!dataPtr->guiconnect) return;
    int i, ntxt;
    char *txt;
    binbuf_gettext(dataPtr->binbuf, &txt, &ntxt);
    sys_vgui(const_cast<char *>("ofelia_textwindow_clear .x%lx\n"), dataPtr);
    for (i = 0; i < ntxt; )
    {
        char *j = std::strchr(txt + i, '\n');
        if (j == nullptr) j = txt + ntxt;
        sys_vgui(const_cast<char *>("ofelia_textwindow_append .x%lx {%.*s\n}\n"),
                 dataPtr, j - txt - i, txt + i);
        i = static_cast<int>((j - txt) + 1);
    }
    sys_vgui(const_cast<char *>("ofelia_textwindow_setdirty .x%lx 0\n"), dataPtr);
    freebytes(txt, ntxt);
}

void ofxOfeliaTextBuf::openMethod()
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
        char title[MAXPDSTRING] = "Untitled";
        if (!dataPtr->hasUniqueSym)
            std::snprintf(title, MAXPDSTRING, "%s", dataPtr->sym->s_name);
        sys_vgui(const_cast<char *>("ofelia_textwindow_open .x%lx %dx%d {%s} %d\n"),
                 dataPtr, 600, 340, title,
                 sys_hostfontsize(glist_getfont(dataPtr->canvas),
                                  glist_getzoom(dataPtr->canvas)));
        char buf[40];
        std::sprintf(buf, ".x%lx", reinterpret_cast<unsigned long>(dataPtr));
        dataPtr->guiconnect = guiconnect_new(&dataPtr->ob.ob_pd, gensym(buf));
        senditup();
    }
}

void ofxOfeliaTextBuf::closeMethod()
{
    if (dataPtr->isDirectMode) return;
    sys_vgui(const_cast<char *>("ofelia_textwindow_doclose .x%lx\n"), dataPtr);
    if (dataPtr->guiconnect)
    {
        guiconnect_notarget(dataPtr->guiconnect, 1000);
        dataPtr->guiconnect = 0;
    }
}

void ofxOfeliaTextBuf::addLineMethod(t_symbol *s, int argc, t_atom *argv)
{
    if (dataPtr->isDirectMode) return;
    t_binbuf *z = binbuf_new();
    binbuf_restore(z, argc, argv);
    binbuf_add(dataPtr->binbuf, binbuf_getnatom(z), binbuf_getvec(z));
    binbuf_free(z);
}

void ofxOfeliaTextBuf::readMethod(t_symbol *s, int argc, t_atom *argv)
{
    if (dataPtr->isDirectMode) return;
    dataPtr->lua.doFreeFunction();
    int cr = 0;
    t_symbol *fileName;
    while (argc && argv->a_type == A_SYMBOL &&
           *argv->a_w.w_symbol->s_name == '-')
    {
        const char *flag = argv->a_w.w_symbol->s_name;
        if (!std::strcmp(flag, "-c"))
            cr = 1;
        else
            error("ofelia read: unknown flag '%s'", flag);
        argc--; argv++;
    }
    if (argc && argv->a_type == A_SYMBOL)
    {
        fileName = argv->a_w.w_symbol;
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
    if (binbuf_read_via_canvas(dataPtr->binbuf, fileName->s_name, dataPtr->canvas, cr))
        error("%s: read failed", fileName->s_name);
    senditup();
    dataPtr->lua.doText();
}

void ofxOfeliaTextBuf::writeMethod(t_symbol *s, int argc, t_atom *argv)
{
    if (dataPtr->isDirectMode) return;
    int cr = 0;
    t_symbol *fileName;
    char buf[MAXPDSTRING];
    while (argc && argv->a_type == A_SYMBOL &&
           *argv->a_w.w_symbol->s_name == '-')
    {
        const char *flag = argv->a_w.w_symbol->s_name;
        if (!std::strcmp(flag, "-c"))
            cr = 1;
        else
            error("ofelia write: unknown flag '%s'", flag);
        argc--; argv++;
    }
    if (argc && argv->a_type == A_SYMBOL)
    {
        fileName = argv->a_w.w_symbol;
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
    canvas_makefilename(dataPtr->canvas, fileName->s_name, buf, MAXPDSTRING);
    if (binbuf_write(dataPtr->binbuf, buf, const_cast<char *>(""), cr))
        error("%s: write failed", fileName->s_name);
}

void ofxOfeliaTextBuf::free()
{
    if (dataPtr->isDirectMode) return;
    t_pd *x2;
    if (dataPtr->binbuf)
        binbuf_free(dataPtr->binbuf);
    if (dataPtr->guiconnect)
    {
        sys_vgui(const_cast<char *>("destroy .x%lx\n"), dataPtr);
        guiconnect_notarget(dataPtr->guiconnect, 1000);
    }
    /* just in case we're still bound to #A from loading... */
    while ((x2 = pd_findbyclass(gensym("#A"), ofxOfeliaDefine::pdClass)))
        pd_unbind(x2, gensym("#A"));
}
