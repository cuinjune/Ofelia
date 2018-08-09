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

#ifndef ofeliaBindings_h
#define ofeliaBindings_h

#pragma once

#include "ofeliaSetup.h"

static void print(t_symbol *s)
{
    post("%s", s->s_name);
}

static void error(t_symbol *s)
{
    error("%s", s->s_name);
}

static void sysGui(t_symbol *s)
{
    char buf[MAXPDSTRING];
    strcpy(buf, s->s_name);
    size_t size = strlen(s->s_name);
    buf[size] = '\n';
    buf[size + 1] = '\0';
    sys_gui(buf);
}

class Window
{
public:
    Window(t_symbol *s)
    :x(t_ofeliadata::ofeliadata_getdata(s))
    ,clock(clock_new(this, reinterpret_cast<t_method>(loop)))
    ,glVersionMajor(2)
    ,glVersionMinor(1)
    ,title("")
    ,windowMode(OF_WINDOW)
    ,width(1024)
    ,height(768)
    ,sizeSet(false)
    ,position(0, 0)
    ,positionSet(false)
    ,windowExists(false){};
    virtual ~Window()
    {
        if (!windowExists) return;
        removeWindowListeners();
        windowPtr->setWindowShouldClose();
        mainLoop->loopOnce();
        mainLoop->pollEvents();
        clock_free(clock);
    };
    void setGLVersion(int major, int minor)
    {
        this->glVersionMajor = major;
        this->glVersionMinor = minor;
    }
    void setPosition(float xpos, float ypos)
    {
        this->position.x = xpos;
        this->position.y = ypos;
        this->positionSet = true;
    }
    void setSize(int width, int height)
    {
        this->width = width;
        this->height = height;
        this->sizeSet = true;
    }
    void create()
    {
        if (windowExists) return;
        ofGLFWWindowSettings settings;
        settings.setGLVersion(glVersionMajor, glVersionMinor);
        settings.title = title;
        settings.windowMode = windowMode;
        if (positionSet) settings.setPosition(position);
        if (sizeSet) settings.setSize(width, height);
        settings.numSamples = numSamples;
        settings.doubleBuffering = doubleBuffering;
        settings.redBits = redBits;
        settings.greenBits = greenBits;
        settings.blueBits = blueBits;
        settings.alphaBits = alphaBits;
        settings.depthBits = depthBits;
        settings.stencilBits = stencilBits;
        settings.stereo = stereo;
        settings.visible = visible;
        settings.iconified = iconified;
        settings.decorated = decorated;
        settings.resizable = resizable;
        settings.monitor = monitor;
        settings.multiMonitorFullScreen = multiMonitorFullScreen;
        ofCreateWindow(settings);
        ofSetLogLevel(OF_LOG_SILENT);
        mainLoop = ofGetMainLoop().get();
        mainLoop->setEscapeQuitsLoop(false);
        windowPtr = ofGetWindowPtr();
        ofResetElapsedTimeCounter();
        ofSetFrameRate(60);
        windowExists = true;
        addWindowListeners();
        windowPtr->events().notifySetup();
        clock_delay(clock, 0.0);
    }
    void destroy()
    {
        if (!windowExists) return;
        ofEventArgs e;
        exit(e);
        removeWindowListeners();
        windowPtr->setWindowShouldClose();
        windowExists = false;
    };
    int glVersionMajor;
    int glVersionMinor;
    string title;
    ofWindowMode windowMode;
    int numSamples = 4;
    bool doubleBuffering = true;
    int redBits = 8;
    int greenBits = 8;
    int blueBits = 8;
    int alphaBits = 8;
    int depthBits = 24;
    int stencilBits = 0;
    bool stereo = false;
    bool visible = true;
    bool iconified = false;
    bool decorated = true;
    bool resizable = true;
    int monitor = 0;
    bool multiMonitorFullScreen = false;
private:
    static void loop(Window *window)
    {
        if (window->windowExists)
            clock_delay(window->clock, 1000.0 / static_cast<double>(ofGetTargetFrameRate()));
        window->mainLoop->loopOnce();
        window->mainLoop->pollEvents();
    };
    void addWindowListeners()
    {
        ofAddListener(windowPtr->events().setup, this, &Window::setup);
        ofAddListener(windowPtr->events().update, this, &Window::update);
        ofAddListener(windowPtr->events().draw, this, &Window::draw);
        ofAddListener(windowPtr->events().exit, this, &Window::exit);
        ofAddListener(windowPtr->events().keyPressed, this, &Window::keyPressed);
        ofAddListener(windowPtr->events().keyReleased, this, &Window::keyReleased);
        ofAddListener(windowPtr->events().mouseMoved, this, &Window::mouseMoved);
        ofAddListener(windowPtr->events().mouseDragged, this, &Window::mouseDragged);
        ofAddListener(windowPtr->events().mousePressed, this, &Window::mousePressed);
        ofAddListener(windowPtr->events().mouseReleased, this, &Window::mouseReleased);
        ofAddListener(windowPtr->events().mouseScrolled, this, &Window::mouseScrolled);
        ofAddListener(windowPtr->events().mouseEntered, this, &Window::mouseEntered);
        ofAddListener(windowPtr->events().mouseExited, this, &Window::mouseExited);
        ofAddListener(windowPtr->events().windowResized, this, &Window::windowResized);
        ofAddListener(windowPtr->events().messageEvent, this, &Window::messageReceived);
        ofAddListener(windowPtr->events().fileDragEvent, this, &Window::dragged);
        ofAddListener(windowPtr->events().touchCancelled, this, &Window::touchCancelled);
        ofAddListener(windowPtr->events().touchDoubleTap, this, &Window::touchDoubleTap);
        ofAddListener(windowPtr->events().touchDown, this, &Window::touchDown);
        ofAddListener(windowPtr->events().touchMoved, this, &Window::touchMoved);
        ofAddListener(windowPtr->events().touchUp, this, &Window::touchUp);
    }
    void removeWindowListeners()
    {
        ofRemoveListener(windowPtr->events().setup, this, &Window::setup);
        ofRemoveListener(windowPtr->events().update, this, &Window::update);
        ofRemoveListener(windowPtr->events().draw, this, &Window::draw);
        ofRemoveListener(windowPtr->events().exit, this, &Window::exit);
        ofRemoveListener(windowPtr->events().keyPressed, this, &Window::keyPressed);
        ofRemoveListener(windowPtr->events().keyReleased, this, &Window::keyReleased);
        ofRemoveListener(windowPtr->events().mouseMoved, this, &Window::mouseMoved);
        ofRemoveListener(windowPtr->events().mouseDragged, this, &Window::mouseDragged);
        ofRemoveListener(windowPtr->events().mousePressed, this, &Window::mousePressed);
        ofRemoveListener(windowPtr->events().mouseReleased, this, &Window::mouseReleased);
        ofRemoveListener(windowPtr->events().mouseScrolled, this, &Window::mouseScrolled);
        ofRemoveListener(windowPtr->events().mouseEntered, this, &Window::mouseEntered);
        ofRemoveListener(windowPtr->events().mouseExited, this, &Window::mouseExited);
        ofRemoveListener(windowPtr->events().windowResized, this, &Window::windowResized);
        ofRemoveListener(windowPtr->events().messageEvent, this, &Window::messageReceived);
        ofRemoveListener(windowPtr->events().fileDragEvent, this, &Window::dragged);
        ofRemoveListener(windowPtr->events().touchCancelled, this, &Window::touchCancelled);
        ofRemoveListener(windowPtr->events().touchDoubleTap, this, &Window::touchDoubleTap);
        ofRemoveListener(windowPtr->events().touchDown, this, &Window::touchDown);
        ofRemoveListener(windowPtr->events().touchMoved, this, &Window::touchMoved);
        ofRemoveListener(windowPtr->events().touchUp, this, &Window::touchUp);
    }
    void callLuaFunction(ofEventArgs &e, t_symbol *s)
    {
        if (x == nullptr) return;
        t_ofeliadata::ofelialua_dofunction(x, s);
    }
    void callLuaFunction(ofKeyEventArgs &e, t_symbol *s)
    {
        if (x == nullptr) return;
        int top; if (!t_ofeliadata::ofelialua_isfunction(x, s, top)) return;
        lua_State *L = x->L;
        lua_newtable(L);
        lua_pushinteger(L, static_cast<lua_Integer>(e.type));
        lua_setfield(L, -2, "type");
        lua_pushinteger(L, static_cast<lua_Integer>(e.key));
        lua_setfield(L, -2, "key");
        lua_pushinteger(L, static_cast<lua_Integer>(e.keycode));
        lua_setfield(L, -2, "keycode");
        lua_pushinteger(L, static_cast<lua_Integer>(e.scancode));
        lua_setfield(L, -2, "scancode");
        lua_pushinteger(L, static_cast<lua_Integer>(e.codepoint));
        lua_setfield(L, -2, "codepoint");
        lua_pushboolean(L, static_cast<int>(e.isRepeat));
        lua_setfield(L, -2, "isRepeat");
        lua_pushinteger(L, static_cast<lua_Integer>(e.modifiers));
        lua_setfield(L, -2, "modifiers");
        t_ofeliadata::ofelialua_callfunction(x, top);
    }
    void callLuaFunction(ofMouseEventArgs &e, t_symbol *s)
    {
        if (x == nullptr) return;
        int top; if (!t_ofeliadata::ofelialua_isfunction(x, s, top)) return;
        lua_State *L = x->L;
        lua_newtable(L);
        lua_pushinteger(L, static_cast<lua_Integer>(e.type));
        lua_setfield(L, -2, "type");
        lua_pushnumber(L, static_cast<lua_Number>(e.x));
        lua_setfield(L, -2, "x");
        lua_pushnumber(L, static_cast<lua_Number>(e.y));
        lua_setfield(L, -2, "y");
        lua_pushinteger(L, static_cast<lua_Integer>(e.button));
        lua_setfield(L, -2, "button");
        lua_pushnumber(L, static_cast<lua_Number>(e.scrollX));
        lua_setfield(L, -2, "scrollX");
        lua_pushnumber(L, static_cast<lua_Number>(e.scrollY));
        lua_setfield(L, -2, "scrollY");
        lua_pushinteger(L, static_cast<lua_Integer>(e.modifiers));
        lua_setfield(L, -2, "modifiers");
        t_ofeliadata::ofelialua_callfunction(x, top);
    }
    void callLuaFunction(ofResizeEventArgs &e, t_symbol *s)
    {
        if (x == nullptr) return;
        int top; if (!t_ofeliadata::ofelialua_isfunction(x, s, top)) return;
        lua_State *L = x->L;
        lua_newtable(L);
        lua_pushinteger(L, static_cast<lua_Integer>(e.width));
        lua_setfield(L, -2, "width");
        lua_pushinteger(L, static_cast<lua_Integer>(e.height));
        lua_setfield(L, -2, "height");
        t_ofeliadata::ofelialua_callfunction(x, top);
    }
    void callLuaFunction(ofMessage &e, t_symbol *s)
    {
        if (x == nullptr) return;
        int top; if (!t_ofeliadata::ofelialua_isfunction(x, s, top)) return;
        lua_State *L = x->L;
        lua_newtable(L);
        lua_pushstring(L, e.message.c_str());
        lua_setfield(L, -2, "message");
        t_ofeliadata::ofelialua_callfunction(x, top);
    }
    void callLuaFunction(ofDragInfo &e, t_symbol *s)
    {
        if (x == nullptr) return;
        int top; if (!t_ofeliadata::ofelialua_isfunction(x, s, top)) return;
        lua_State *L = x->L;
        lua_newtable(L); /* outermost table */
        lua_newtable(L); /* subtable "files" */
        for (size_t i = 0; i < e.files.size(); ++i)
        {
            lua_pushinteger(L, i + 1);
            lua_pushstring(L, e.files[i].c_str());
            lua_settable(L, -3);
        }
        lua_setfield(L, -2, "files");
        lua_newtable(L); /* subtable "position" */
        lua_pushnumber(L, e.position.x);
        lua_setfield(L, -2, "x");
        lua_pushnumber(L, e.position.y);
        lua_setfield(L, -2, "y");
        lua_setfield(L, -2, "position");
        t_ofeliadata::ofelialua_callfunction(x, top);
    }
    void callLuaFunction(ofTouchEventArgs &e, t_symbol *s)
    {
        if (x == nullptr) return;
        int top; if (!t_ofeliadata::ofelialua_isfunction(x, s, top)) return;
        lua_State *L = x->L;
        lua_newtable(L);
        lua_pushinteger(L, static_cast<lua_Integer>(e.type));
        lua_setfield(L, -2, "type");
        lua_pushnumber(L, static_cast<lua_Number>(e.x));
        lua_setfield(L, -2, "x");
        lua_pushnumber(L, static_cast<lua_Number>(e.y));
        lua_setfield(L, -2, "y");
        lua_pushinteger(L, static_cast<lua_Integer>(e.id));
        lua_setfield(L, -2, "id");
        lua_pushinteger(L, static_cast<lua_Integer>(e.time));
        lua_setfield(L, -2, "time");
        lua_pushinteger(L, static_cast<lua_Integer>(e.numTouches));
        lua_setfield(L, -2, "numTouches");
        lua_pushnumber(L, static_cast<lua_Number>(e.width));
        lua_setfield(L, -2, "width");
        lua_pushnumber(L, static_cast<lua_Number>(e.height));
        lua_setfield(L, -2, "height");
        lua_pushnumber(L, static_cast<lua_Number>(e.angle));
        lua_setfield(L, -2, "angle");
        lua_pushnumber(L, static_cast<lua_Number>(e.minoraxis));
        lua_setfield(L, -2, "minoraxis");
        lua_pushnumber(L, static_cast<lua_Number>(e.majoraxis));
        lua_setfield(L, -2, "majoraxis");
        lua_pushnumber(L, static_cast<lua_Number>(e.pressure));
        lua_setfield(L, -2, "pressure");
        lua_pushnumber(L, static_cast<lua_Number>(e.xspeed));
        lua_setfield(L, -2, "xspeed");
        lua_pushnumber(L, static_cast<lua_Number>(e.yspeed));
        lua_setfield(L, -2, "yspeed");
        lua_pushnumber(L, static_cast<lua_Number>(e.xaccel));
        lua_setfield(L, -2, "xaccel");
        lua_pushnumber(L, static_cast<lua_Number>(e.yaccel));
        lua_setfield(L, -2, "yaccel");
        t_ofeliadata::ofelialua_callfunction(x, top);
    }
    void setup(ofEventArgs &e)
    {
        callLuaFunction(e, gensym("setup"));
    }
    void update(ofEventArgs &e)
    {
        callLuaFunction(e, gensym("update"));
    }
    void draw(ofEventArgs &e)
    {
        callLuaFunction(e, gensym("draw"));
    }
    void exit(ofEventArgs &e)
    {
        callLuaFunction(e, gensym("exit"));
        removeWindowListeners();
        windowExists = false;
    }
    void keyPressed(ofKeyEventArgs &e)
    {
        callLuaFunction(e, gensym("keyPressed"));
    }
    void keyReleased(ofKeyEventArgs &e)
    {
        callLuaFunction(e, gensym("keyReleased"));
    }
    void mouseMoved(ofMouseEventArgs &e)
    {
        callLuaFunction(e, gensym("mouseMoved"));
    }
    void mouseDragged(ofMouseEventArgs &e)
    {
        callLuaFunction(e, gensym("mouseDragged"));
    }
    void mousePressed(ofMouseEventArgs &e)
    {
        callLuaFunction(e, gensym("mousePressed"));
    }
    void mouseReleased(ofMouseEventArgs &e)
    {
        callLuaFunction(e, gensym("mouseReleased"));
    }
    void mouseScrolled(ofMouseEventArgs &e)
    {
        callLuaFunction(e, gensym("mouseScrolled"));
    }
    void mouseEntered(ofMouseEventArgs &e)
    {
        callLuaFunction(e, gensym("mouseEntered"));
    }
    void mouseExited(ofMouseEventArgs &e)
    {
        callLuaFunction(e, gensym("mouseExited"));
    }
    void windowResized(ofResizeEventArgs &e)
    {
        callLuaFunction(e, gensym("windowResized"));
    }
    void messageReceived(ofMessage &e)
    {
        callLuaFunction(e, gensym("messageReceived"));
    }
    void dragged(ofDragInfo &e)
    {
        callLuaFunction(e, gensym("dragged"));
    }
    void touchCancelled(ofTouchEventArgs &e)
    {
        callLuaFunction(e, gensym("touchCancelled"));
    }
    void touchDoubleTap(ofTouchEventArgs &e)
    {
        callLuaFunction(e, gensym("touchDoubleTap"));
    }
    void touchDown(ofTouchEventArgs &e)
    {
        callLuaFunction(e, gensym("touchDown"));
    }
    void touchMoved(ofTouchEventArgs &e)
    {
        callLuaFunction(e, gensym("touchMoved"));
    }
    void touchUp(ofTouchEventArgs &e)
    {
        callLuaFunction(e, gensym("touchUp"));
    }
    t_ofeliadata *x;
    t_clock *clock;
    ofMainLoop *mainLoop;
    ofAppBaseWindow *windowPtr;
    glm::vec2 position;
    int width;
    int height;
    bool positionSet;
    bool sizeSet;
    bool windowExists;
};

class Canvas
{
public:
    Canvas(t_symbol *s)
    :x(t_ofeliadata::ofeliadata_getdata(s))
    {
        initCanvas(s, 0);
    };
    Canvas(t_symbol *s, int level)
    :x(t_ofeliadata::ofeliadata_getdata(s))
    {
        initCanvas(s, level);
    };
    t_symbol *realizeDollar(t_symbol *s)
    {
        if (x == nullptr) return &s_;
        return canvas_realizedollar(canvas, s);
    }
    t_symbol *getName()
    {
        if (x == nullptr) return &s_;
        char buf[MAXPDSTRING];
        snprintf(buf, MAXPDSTRING, ".x%lx", reinterpret_cast<long unsigned int>(canvas));
        return gensym(buf);
    }
    int getIndex()
    {
        if (x == nullptr) return 0;
        t_glist *glist = static_cast<t_glist*>(canvas);
        t_gobj *obj;
        int index = 0;
        for (obj = glist->gl_list; obj && obj != gobj; obj = obj->g_next) index++;
        return index;
    }
    void getArgs(int *argcp, t_atom **argvp)
    {
        if (x == nullptr) return;
        t_binbuf *binbuf = canvas->gl_obj.te_binbuf;
        if (!binbuf) return;
        *argcp = binbuf_getnatom(binbuf) - 1;
        *argvp = binbuf_getvec(binbuf) + 1;
    }
    void setArgs(int argc, t_atom *argv)
    {
        if (x == nullptr) return;
        t_binbuf *binbuf = canvas->gl_obj.te_binbuf;
        if (!binbuf) return;
        t_atom av[1];
        av[0].a_type = A_SYMBOL;
        av[0].a_w.w_symbol = binbuf_getvec(binbuf)[0].a_w.w_symbol;
        binbuf_clear(binbuf);
        binbuf_add(binbuf, 1, av);
        binbuf_add(binbuf, argc, argv);
    }
    void getPosition(int **posp)
    {
        if (x == nullptr) return;
        int *pos = static_cast<int *>(getbytes(2 * sizeof(int)));
        pos[0] = static_cast<int>(canvas->gl_obj.te_xpix);
        pos[1] = static_cast<int>(canvas->gl_obj.te_ypix);
        *posp = pos;
    }
    void setPosition(int *pos)
    {
        if (x == nullptr) return;
        int dx = pos[0] - canvas->gl_obj.te_xpix;
        int dy = pos[1] - canvas->gl_obj.te_ypix;
        if (!dx && !dy) return;
        if (canvas->gl_owner && glist_isvisible(canvas->gl_owner))
        {
            gobj_displace(reinterpret_cast<t_gobj *>(canvas), canvas->gl_owner, dx, dy);
            canvas_fixlinesfor(canvas->gl_owner, reinterpret_cast<t_text *>(canvas));
        }
        else
        {
            canvas->gl_obj.te_xpix += dx;
            canvas->gl_obj.te_ypix += dy;
        }
    }
    t_symbol *getDir()
    {
        if (x == nullptr) return &s_;
        return canvas_getdir(canvas);
    }
    void remove()
    {
        if (x == nullptr) return;
        clock = clock_new(this, reinterpret_cast<t_method>(removeCanvas));
        clock_delay(clock, 0.0);
    }
private:
    void initCanvas(t_symbol *s, int level)
    {
        if (x == nullptr) return;
        canvas = x->canvas;
        gobj = reinterpret_cast<t_gobj *>(x);
        for (int i = 0; i < level; ++i)
        {
            if (!canvas->gl_owner) break;
            gobj = reinterpret_cast<t_gobj *>(canvas);
            canvas = canvas->gl_owner;
        }
    }
    static void removeCanvas(Canvas *canvas)
    {
        clock_free(canvas->clock);
        int dspstate = canvas_suspend_dsp();
        glist_delete(static_cast<t_glist*>(canvas->canvas), canvas->gobj);
        canvas_resume_dsp(dspstate);
    }
    t_ofeliadata *x;
    t_canvas *canvas;
    t_gobj *gobj;
    t_clock *clock;
};

class Send
{
public:
    Send(t_symbol *s)
    :sym(s){};
    void sendBang()
    {
        if (sym->s_thing) pd_bang(sym->s_thing);
    }
    void sendFloat(t_floatarg f)
    {
        if (sym->s_thing) pd_float(sym->s_thing, f);
    }
    void sendSymbol(t_symbol *s)
    {
        if (sym->s_thing) pd_symbol(sym->s_thing, s);
    }
    void sendList(int argc, t_atom *argv)
    {
        if (sym->s_thing) pd_list(sym->s_thing, &s_list, argc, argv);
    }
    void sendAnything(int argc, t_atom *argv)
    {
        if (sym->s_thing)
        {
            if (argv[0].a_type == A_SYMBOL)
                typedmess(sym->s_thing, argv[0].a_w.w_symbol, argc - 1, argv + 1);
            else if (argv[0].a_type == A_FLOAT)
                pd_list(sym->s_thing, &s_list, argc, argv);
        }
    }
private:
    t_symbol *sym;
};

class Value
{
public:
    Value(t_symbol *s)
    :sym(s)
    ,floatstar(value_get(sym)){};
    virtual ~Value()
    {
        value_release(sym);
    };
    t_float get()
    {
        return *floatstar;
    }
    void set(t_floatarg f)
    {
        *floatstar = f;
    }
private:
    t_symbol *sym;
    t_float *floatstar;
};

class Array
{
public:
    Array(t_symbol *s)
    :sym(s){};
    bool exists(t_garray **a)
    {
        *a = reinterpret_cast<t_garray *>(pd_findbyclass(sym, garray_class));
        return (*a != nullptr);
    }
    float getAt(int n)
    {
        t_garray *a; int size; t_word *vec;
        if (exists(&a) && getData(a, &size, &vec))
        {
            if (n < 0) n = 0;
            else if (n >= size) n = size - 1;
            return size ? vec[n].w_float : 0;
        }
        return 0;
    }
    void setAt(int n, t_floatarg f)
    {
        t_garray *a; int size; t_word *vec;
        if (exists(&a) && getData(a, &size, &vec))
        {
            if (n < 0) n = 0;
            else if (n >= size) n = size - 1;
            vec[n].w_float = f;
            garray_redraw(a);
        }
    }
    void getTable(t_word **vecp, int *sizep)
    {
        t_garray *a; int size; t_word *vec;
        if (exists(&a) && getData(a, &size, &vec))
        {
            *vecp = vec;
            *sizep = size;
        }
    }
    void setTable(int n, t_floatarg *f)
    {
        t_garray *a; int size; t_word *vec;
        if (exists(&a) && getData(a, &size, &vec))
        {
            garray_resize_long(a, long(n));
            for (int i = 0; i < n; ++i)
                vec[i].w_float = f[i];
            garray_redraw(a);
        }
    }
    int getSize()
    {
        t_garray *a; int size; t_word *vec;
        if (exists(&a) && getData(a, &size, &vec))
            return size;
        return 0;
    }
    void setSize(long n)
    {
        t_garray *a; int size; t_word *vec;
        if (exists(&a) && getData(a, &size, &vec))
        {
            garray_resize_long(a, n);
            garray_redraw(a);
        }
    }
private:
    bool getData(t_garray *a, int *size, t_word **vec)
    {
        if (!garray_getfloatwords(a, size, vec))
        {
            error("ofelia: bad template for array '%s'", sym->s_name);
            return false;
        }
        return true;
    }
    t_symbol *sym;
};

class Clock
{
public:
    Clock(t_symbol *s)
    :x(t_ofeliadata::ofeliadata_getdata(s))
    ,sym(gensym("delayed"))
    ,clock(clock_new(this, reinterpret_cast<t_method>(delayedFunction)))
    {
        initClock(s);
    };
    Clock(t_symbol *s, t_symbol *s2)
    :x(t_ofeliadata::ofeliadata_getdata(s))
    ,sym(s2)
    ,clock(clock_new(this, reinterpret_cast<t_method>(delayedFunction)))
    {
        initClock(s);
    };
    virtual ~Clock()
    {
        clock_free(clock);
    };
    void delay(double delayTime)
    {
        clock_delay(clock, delayTime);
    };
    void unset()
    {
        clock_unset(clock);
    };
private:
    void initClock(t_symbol *s)
    {
        if (x == nullptr) return;
    }
    static void delayedFunction(Clock *clock)
    {
        if (clock->x == nullptr) return;
        t_ofeliadata::ofelialua_dofunction(clock->x, clock->sym);
    };
    t_ofeliadata *x;
    t_symbol *sym;
    t_clock *clock;
};

class Audio
{
public:
    int getBlockSize()
    {
        return sys_getblksize();
    }
    t_float getSampleRate()
    {
        return sys_getsr();
    }
    int getInChannels()
    {
        return sys_get_inchannels();
    }
    int getOutChannels()
    {
        return sys_get_outchannels();
    }
private:
};

#endif /* ofeliaBindings_h */
