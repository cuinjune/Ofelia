#pragma once

#include "m_pd.h"
#include "g_canvas.h"
#include "lua.hpp"
#include "ofMain.h"
#include "ofxOfeliaLua.h"
#include "ofxOfeliaData.h"
#include "ofxOfeliaEvents.h"
#include "ofxOfeliaAudio.h"
#include "ofxOfeliaSetup.h"
#include "ofxJsonSettings.h"
#include "ofxOsc.h"
#include "ofxReverb.h"
#include "ofxZipPass.h"
#include "emscripten.h"
#include <cstdio>
#include <utility>
#include <vector>
#include <deque>

class pdWindow
{
public:
    pdWindow()
    :clock(clock_new(this, reinterpret_cast<t_method>(loop)))
    ,glVersionMajor(2)
    ,glVersionMinor(1)
    ,glesVersion(1)
    ,title(gensym(""))
    ,windowMode(0)
    ,width(1024)
    ,height(768)
    ,sizeSet(false)
    ,position(0, 0)
    ,positionSet(false)
    ,owner(false){};
    virtual ~pdWindow()
    {
        if (!owner || !exists) return;
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
    void setGLESVersion(int version)
    {
        this->glesVersion = version;
    }
    void getPosition(int **posp)
    {
        int *pos = static_cast<int *>(getbytes(2 * sizeof(int)));
        pos[0] = static_cast<int>(position.x);
        pos[1] = static_cast<int>(position.y);
        *posp = pos;
    }
    void setPosition(int xpos, int ypos)
    {
        this->position.x = static_cast<float>(xpos);
        this->position.y = static_cast<float>(ypos);
        this->positionSet = true;
    }
    bool isPositionSet()
    {
        return positionSet;
    }
    int getWidth()
    {
        return width;
    }
    int getHeight()
    {
        return height;
    }
    void setSize(int width, int height)
    {
        this->width = width;
        this->height = height;
        this->sizeSet = true;
    }
    bool isSizeSet()
    {
        return sizeSet;
    }
    void create()
    {
        if (owner || exists) return;
#if defined(TARGET_EXTERNAL)
#if defined(TARGET_OPENGLES)
        ofGLESWindowSettings settings;
        settings.setGLESVersion(glesVersion);
#else
        ofGLWindowSettings settings;
        settings.setGLVersion(glVersionMajor, glVersionMinor);
#endif
        settings.title = title->s_name;
        settings.windowMode = static_cast<ofWindowMode>(windowMode);
        if (positionSet) settings.setPosition(position);
        if (sizeSet) settings.setSize(width, height);
        ofCreateWindow(settings);
#elif defined(TARGET_STANDALONE)
        ofSetWindowTitle(title->s_name);
        ofSetWindowPosition(position.x, position.y);
        ofSetWindowShape(width, height);
#endif
        mainLoop = ofGetMainLoop().get();
        mainLoop->setEscapeQuitsLoop(false);
        windowPtr = ofGetWindowPtr();
        ofResetElapsedTimeCounter();
        ofSetFrameRate(60);
        ofDisableArbTex();
        owner = true;
        exists = true;
#if defined(TARGET_EXTERNAL)
        addWindowListeners();
        windowPtr->events().notifySetup();
        clock_delay(clock, 0.0);
#endif
    }
    void destroy()
    {
        if (!owner || !exists) return;
#if defined(TARGET_EXTERNAL)
        ofEventArgs e;
        exit(e);
        removeWindowListeners();
        windowPtr->setWindowShouldClose();
        owner = false;
        exists = false;
#elif defined(TARGET_STANDALONE)
        ofExit();
#endif
    };
    static void addListener(t_symbol *s, t_symbol *s2, t_floatarg f)
    {
        ofxOfeliaData *x = ofxOfeliaData::getPtr(s2);
        if (x == nullptr) return;
        ofxOfeliaEvents::DataPairVec *vecPtr = ofxOfeliaEvents::getTargetDataPairVec(s);
        if (vecPtr == nullptr) return;
        ofxOfeliaEvents::addDataPair(*vecPtr, x, f);
    }
    static void addListener(t_symbol *s, t_symbol *s2)
    {
        addListener(s, s2, 0);
    }
    static void removeListener(t_symbol *s, t_symbol *s2)
    {
        ofxOfeliaData *x = ofxOfeliaData::getPtr(s2);
        if (x == nullptr) return;
        ofxOfeliaEvents::DataPairVec *vecPtr = ofxOfeliaEvents::getTargetDataPairVec(s);
        if (vecPtr == nullptr) return;
        ofxOfeliaEvents::removeDataPair(*vecPtr, x);
    }
    static std::vector<std::pair<ofxOfeliaData *, t_float>> getListenerData(t_symbol *s)
    {
        ofxOfeliaEvents::DataPairVec *vecPtr = ofxOfeliaEvents::getTargetDataPairVec(s);
        if (vecPtr == nullptr) return {};
        ofxOfeliaEvents::sortDataPairVec(*vecPtr);
        return *vecPtr;
    }
    int glVersionMajor;
    int glVersionMinor;
    int glesVersion;
    t_symbol *title;
    int windowMode;
    static bool exists;
private:
    void addWindowListeners()
    {
        ofAddListener(windowPtr->events().setup, this, &pdWindow::setup);
        ofAddListener(windowPtr->events().update, this, &pdWindow::update);
        ofAddListener(windowPtr->events().draw, this, &pdWindow::draw);
        ofAddListener(windowPtr->events().exit, this, &pdWindow::exit);
        ofAddListener(windowPtr->events().keyPressed, this, &pdWindow::keyPressed);
        ofAddListener(windowPtr->events().keyReleased, this, &pdWindow::keyReleased);
        ofAddListener(windowPtr->events().mouseMoved, this, &pdWindow::mouseMoved);
        ofAddListener(windowPtr->events().mouseDragged, this, &pdWindow::mouseDragged);
        ofAddListener(windowPtr->events().mousePressed, this, &pdWindow::mousePressed);
        ofAddListener(windowPtr->events().mouseReleased, this, &pdWindow::mouseReleased);
        ofAddListener(windowPtr->events().mouseScrolled, this, &pdWindow::mouseScrolled);
        ofAddListener(windowPtr->events().mouseEntered, this, &pdWindow::mouseEntered);
        ofAddListener(windowPtr->events().mouseExited, this, &pdWindow::mouseExited);
        ofAddListener(windowPtr->events().windowResized, this, &pdWindow::windowResized);
        ofAddListener(windowPtr->events().messageEvent, this, &pdWindow::messageReceived);
        ofAddListener(windowPtr->events().fileDragEvent, this, &pdWindow::dragged);
        ofAddListener(windowPtr->events().touchCancelled, this, &pdWindow::touchCancelled);
        ofAddListener(windowPtr->events().touchDoubleTap, this, &pdWindow::touchDoubleTap);
        ofAddListener(windowPtr->events().touchDown, this, &pdWindow::touchDown);
        ofAddListener(windowPtr->events().touchMoved, this, &pdWindow::touchMoved);
        ofAddListener(windowPtr->events().touchUp, this, &pdWindow::touchUp);
    }
    void removeWindowListeners()
    {
        ofRemoveListener(windowPtr->events().setup, this, &pdWindow::setup);
        ofRemoveListener(windowPtr->events().update, this, &pdWindow::update);
        ofRemoveListener(windowPtr->events().draw, this, &pdWindow::draw);
        ofRemoveListener(windowPtr->events().exit, this, &pdWindow::exit);
        ofRemoveListener(windowPtr->events().keyPressed, this, &pdWindow::keyPressed);
        ofRemoveListener(windowPtr->events().keyReleased, this, &pdWindow::keyReleased);
        ofRemoveListener(windowPtr->events().mouseMoved, this, &pdWindow::mouseMoved);
        ofRemoveListener(windowPtr->events().mouseDragged, this, &pdWindow::mouseDragged);
        ofRemoveListener(windowPtr->events().mousePressed, this, &pdWindow::mousePressed);
        ofRemoveListener(windowPtr->events().mouseReleased, this, &pdWindow::mouseReleased);
        ofRemoveListener(windowPtr->events().mouseScrolled, this, &pdWindow::mouseScrolled);
        ofRemoveListener(windowPtr->events().mouseEntered, this, &pdWindow::mouseEntered);
        ofRemoveListener(windowPtr->events().mouseExited, this, &pdWindow::mouseExited);
        ofRemoveListener(windowPtr->events().windowResized, this, &pdWindow::windowResized);
        ofRemoveListener(windowPtr->events().messageEvent, this, &pdWindow::messageReceived);
        ofRemoveListener(windowPtr->events().fileDragEvent, this, &pdWindow::dragged);
        ofRemoveListener(windowPtr->events().touchCancelled, this, &pdWindow::touchCancelled);
        ofRemoveListener(windowPtr->events().touchDoubleTap, this, &pdWindow::touchDoubleTap);
        ofRemoveListener(windowPtr->events().touchDown, this, &pdWindow::touchDown);
        ofRemoveListener(windowPtr->events().touchMoved, this, &pdWindow::touchMoved);
        ofRemoveListener(windowPtr->events().touchUp, this, &pdWindow::touchUp);
    }
    void setup(ofEventArgs &e)
    {
        ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::setupVec, gensym("setup"));
    }
    void update(ofEventArgs &e)
    {
        ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::updateVec, gensym("update"));
    }
    void draw(ofEventArgs &e)
    {
        ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::drawVec, gensym("draw"));
    }
    void exit(ofEventArgs &e)
    {
        ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::exitVec, gensym("exit"));
        removeWindowListeners();
        owner = false;
        exists = false;
    }
    void keyPressed(ofKeyEventArgs &e)
    {
        ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::keyPressedVec, gensym("keyPressed"), e);
    }
    void keyReleased(ofKeyEventArgs &e)
    {
        ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::keyReleasedVec, gensym("keyReleased"), e);
    }
    void mouseMoved(ofMouseEventArgs &e)
    {
        ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::mouseMovedVec, gensym("mouseMoved"), e);
    }
    void mouseDragged(ofMouseEventArgs &e)
    {
        ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::mouseDraggedVec, gensym("mouseDragged"), e);
    }
    void mousePressed(ofMouseEventArgs &e)
    {
        ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::mousePressedVec, gensym("mousePressed"), e);
    }
    void mouseReleased(ofMouseEventArgs &e)
    {
        ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::mouseReleasedVec, gensym("mouseReleased"), e);
    }
    void mouseScrolled(ofMouseEventArgs &e)
    {
        ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::mouseScrolledVec, gensym("mouseScrolled"), e);
    }
    void mouseEntered(ofMouseEventArgs &e)
    {
        ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::mouseEnteredVec, gensym("mouseEntered"), e);
    }
    void mouseExited(ofMouseEventArgs &e)
    {
        ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::mouseExitedVec, gensym("mouseExited"), e);
    }
    void windowResized(ofResizeEventArgs &e)
    {
        ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::windowResizedVec, gensym("windowResized"), e);
    }
    void messageReceived(ofMessage &e)
    {
        ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::messageReceivedVec, gensym("messageReceived"), e);
    }
    void dragged(ofDragInfo &e)
    {
        ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::draggedVec, gensym("dragged"), e);
    }
    void touchCancelled(ofTouchEventArgs &e)
    {
        ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::touchCancelledVec, gensym("touchCancelled"), e);
    }
    void touchDoubleTap(ofTouchEventArgs &e)
    {
        ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::touchDoubleTapVec, gensym("touchDoubleTap"), e);
    }
    void touchDown(ofTouchEventArgs &e)
    {
        ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::touchDownVec, gensym("touchDown"), e);
    }
    void touchMoved(ofTouchEventArgs &e)
    {
        ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::touchMovedVec, gensym("touchMoved"), e);
    }
    void touchUp(ofTouchEventArgs &e)
    {
        ofxOfeliaEvents::callEventListeners(ofxOfeliaEvents::touchUpVec, gensym("touchUp"), e);
    }
    static void loop(pdWindow *window)
    {
        if (window->exists)
            clock_delay(window->clock, 1000.0 / static_cast<double>(ofGetTargetFrameRate()));
        window->mainLoop->loopOnce();
        window->mainLoop->pollEvents();
    };
    ofxOfeliaData *x;
    t_clock *clock;
    ofMainLoop *mainLoop;
    ofAppBaseWindow *windowPtr;
    glm::vec2 position;
    int width;
    int height;
    bool positionSet;
    bool sizeSet;
    bool owner;
};

bool pdWindow::exists = false;

class pdCanvas
{
public:
    pdCanvas(t_symbol *s)
    :x(ofxOfeliaData::getPtr(s))
    {
        initCanvas(s, 0);
    };
    pdCanvas(t_symbol *s, t_floatarg f)
    :x(ofxOfeliaData::getPtr(s))
    {
        initCanvas(s, f);
    };
    t_symbol *getDollarZero()
    {
        if (x == nullptr) return &s_;
        return canvas_realizedollar(canvas, gensym("$0"));
    }
    t_symbol *getName()
    {
        if (x == nullptr) return &s_;
        char buf[MAXPDSTRING];
        std::snprintf(buf, MAXPDSTRING, ".x%lx.c", reinterpret_cast<unsigned long>(canvas));
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
    void getArgs(int *argcp, t_atom **argvp, t_canvas **canvasp)
    {
        if (x == nullptr) return;
        t_binbuf *binbuf = canvas->gl_obj.te_binbuf;
        if (!binbuf)
        {
            canvas_setcurrent(canvas);
            canvas_getargs(argcp, argvp);
            canvas_unsetcurrent(canvas);
            return;
        }
        *argcp = binbuf_getnatom(binbuf) - 1;
        *argvp = binbuf_getvec(binbuf) + 1;
        *canvasp = canvas->gl_owner ? canvas->gl_owner : canvas;
    }
    void setArgs(int argc, t_atom *argv, std::deque<int> userDataRef)
    {
        if (x == nullptr) return;
        t_binbuf *binbuf = canvas->gl_obj.te_binbuf;
        if (!binbuf) return;
        t_atom av[1];
        SETSYMBOL(av, binbuf_getvec(binbuf)[0].a_w.w_symbol);
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
    void setPosition(int xpos, int ypos)
    {
        if (x == nullptr) return;
        int dx = xpos - canvas->gl_obj.te_xpix;
        int dy = ypos - canvas->gl_obj.te_ypix;
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
    t_symbol *makeFileName(t_symbol *s)
    {
        if (x == nullptr) return &s_;
        char buf[MAXPDSTRING];
        canvas_makefilename(canvas, s->s_name, buf, MAXPDSTRING);
        return gensym(buf);
    }
    void remove()
    {
        if (x == nullptr) return;
        clock = clock_new(this, reinterpret_cast<t_method>(removeCanvas));
        clock_delay(clock, 0.0);
    }
private:
    void initCanvas(t_symbol *s, t_floatarg f)
    {
        if (x == nullptr) return;
        canvas = x->canvas;
        gobj = reinterpret_cast<t_gobj *>(x);
        while (f)
        {
            if (!canvas->gl_owner) break;
            gobj = reinterpret_cast<t_gobj *>(canvas);
            canvas = canvas->gl_owner;
            f--;
        }
    }
    static void removeCanvas(pdCanvas *canvas)
    {
        clock_free(canvas->clock);
        int dspstate = canvas_suspend_dsp();
        glist_delete(static_cast<t_glist*>(canvas->canvas), canvas->gobj);
        canvas_resume_dsp(dspstate);
    }
    ofxOfeliaData *x;
    t_canvas *canvas;
    t_gobj *gobj;
    t_clock *clock;
};

class pdSend
{
public:
    pdSend(t_symbol *s)
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
    void sendPointer(t_gpointer *p)
    {
        int userDataRef = luaL_ref(ofxOfeliaLua::L, LUA_REGISTRYINDEX);
        if (sym->s_thing) pd_pointer(sym->s_thing, reinterpret_cast<t_gpointer *>(&userDataRef));
        luaL_unref(ofxOfeliaLua::L, LUA_REGISTRYINDEX, userDataRef);
    }
    void sendList(int argc, t_atom *argv, std::deque<int> userDataRef)
    {
        if (sym->s_thing) pd_list(sym->s_thing, &s_list, argc, argv);
    }
    void sendAnything(int argc, t_atom *argv, std::deque<int> userDataRef)
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

class pdInlet
{
public:
    pdInlet(t_symbol *s)
    :x(ofxOfeliaData::getPtr(s)){};
    void setFloatInlet(int n, t_floatarg f)
    {
        if (!doesPassiveInletExist()) return;
        n = ofClamp(n, 0, x->io.numInlets - 2);
        if (x->io.av[n].a_type != A_FLOAT)
        {
            postWrongPassiveInletArgTypeError();
            return;
        }
        x->io.av[n].a_w.w_float = f;
    }
    void setSymbolInlet(int n, t_symbol *s)
    {
        if (!doesPassiveInletExist()) return;
        n = ofClamp(n, 0, x->io.numInlets - 2);
        if (x->io.av[n].a_type != A_SYMBOL)
        {
            postWrongPassiveInletArgTypeError();
            return;
        }
        x->io.av[n].a_w.w_symbol = s;
    }
    void setInlets(int argc, t_atom *argv, std::deque<int> userDataRef)
    {
        if (!doesPassiveInletExist()) return;
        const int numPassiveInlets = x->io.numInlets - 1;
        if (argc > numPassiveInlets)
            argc = numPassiveInlets;
        for (int i = 0; i < argc; ++i)
        {
            if (x->io.av[i].a_type != argv[i].a_type)
            {
                postWrongPassiveInletArgTypeError();
                return;
            }
            x->io.av[i].a_w = argv[i].a_w;
        }
    }
    void setSignalInlet(t_floatarg f)
    {
        if (!doesSignalInletExist()) return;
        x->signal.f = f;
    }
private:
    bool doesPassiveInletExist()
    {
        if (x == nullptr) return false;
        if (!x->io.hasMultiControlInlets)
        {
            error("ofelia: passive inlet does not exist");
            return false;
        }
        return true;
    }
    bool doesSignalInletExist()
    {
        if (x == nullptr) return false;
        if (!x->isSignalObject)
        {
            error("ofelia: signal inlet does not exist");
            return false;
        }
        return true;
    }
    void postWrongPassiveInletArgTypeError()
    {
        error("ofelia: wrong passive inlet argument type to set");
    }
    ofxOfeliaData *x;
};

class pdOutlet
{
public:
    pdOutlet(t_symbol *s)
    :x(ofxOfeliaData::getPtr(s)){};
    void outletBang(int index)
    {
        if (!isOutletValid(index)) return;
        outlet_bang(x->io.outlets[index]);
    }
    void outletFloat(int index, t_floatarg f)
    {
        if (!isOutletValid(index)) return;
        outlet_float(x->io.outlets[index], f);
    }
    void outletSymbol(int index, t_symbol *s)
    {
        if (!isOutletValid(index)) return;
        outlet_symbol(x->io.outlets[index], s);
    }
    void outletPointer(int index, t_gpointer *p)
    {
        if (isOutletValid(index))
        {
            int userDataRef = luaL_ref(ofxOfeliaLua::L, LUA_REGISTRYINDEX);
            outlet_pointer(x->io.outlets[index], reinterpret_cast<t_gpointer *>(&userDataRef));
            luaL_unref(ofxOfeliaLua::L, LUA_REGISTRYINDEX, userDataRef);
        }
    }
    void outletList(int index, int argc, t_atom *argv, std::deque<int> userDataRef)
    {
        if (!isOutletValid(index)) return;
        outlet_list(x->io.outlets[index], &s_list, argc, argv);
    }
    void outletAnything(int index, int argc, t_atom *argv, std::deque<int> userDataRef)
    {
        if (!isOutletValid(index)) return;
        if (argv[0].a_type == A_SYMBOL)
            outlet_anything(x->io.outlets[index], argv[0].a_w.w_symbol, argc - 1, argv + 1);
        else if (argv[0].a_type == A_FLOAT)
            outlet_list(x->io.outlets[index], &s_list, argc, argv);
    }
private:
    bool isOutletValid(int index)
    {
        if (x == nullptr) return false;
        if (!x->io.hasControlOutlet)
        {
            error("ofelia: control outlet does not exist");
            return false;
        }
        if (index < 0 || index >= x->io.numOutlets)
        {
            error("ofelia: invalid outlet index '%d'", index);
            return false;
        }
        return true;
    }
    ofxOfeliaData *x;
};

class pdValue
{
public:
    pdValue(t_symbol *s)
    :sym(s)
    ,floatstar(value_get(sym)){};
    virtual ~pdValue()
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

class pdArray
{
public:
    pdArray(t_symbol *s)
    :sym(s){};
    t_float getAt(int n)
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
    t_float __getitem__(int n)
    {
        return getAt(n);
    }
    void __setitem__(int n, t_floatarg f)
    {
        setAt(n, f);
    }
    void get(t_word **vecp, int *sizep, int onset)
    {
        t_garray *a; int size; t_word *vec;
        if (exists(&a) && getData(a, &size, &vec))
        {
            if (onset < 0) onset = 0;
            *vecp = vec + onset;
            *sizep = size - onset;
        }
    }
    void set(int n, t_floatarg *f, int onset)
    {
        t_garray *a; int size; t_word *vec;
        if (exists(&a) && getData(a, &size, &vec))
        {
            if (onset < 0) onset = 0;
            for (int i = 0; i < n; ++i)
            {
                int io = i + onset;
                if (io < size) vec[io].w_float = f[i];
                else break;
            }
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
    bool exists(t_garray **a)
    {
        *a = reinterpret_cast<t_garray *>(pd_findbyclass(sym, garray_class));
        return (*a != nullptr);
    }
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

class pdClock
{
public:
    pdClock(t_symbol *s)
    :x(ofxOfeliaData::getPtr(s))
    ,sym(gensym("delayed"))
    ,clock(clock_new(this, reinterpret_cast<t_method>(delayedFunction))){};
    pdClock(t_symbol *s, t_symbol *s2)
    :x(ofxOfeliaData::getPtr(s))
    ,sym(s2)
    ,clock(clock_new(this, reinterpret_cast<t_method>(delayedFunction))){};
    virtual ~pdClock()
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
    static void delayedFunction(pdClock *clock)
    {
        if (clock->x == nullptr) return;
        clock->x->lua.doFunction(clock->sym);
    };
    ofxOfeliaData *x;
    t_symbol *sym;
    t_clock *clock;
};

class pdOsc
{
public:
    pdOsc(){};
    void setPhase(t_floatarg f)
    {
        osc.setPhase(f);
    }
    t_float getPhase()
    {
        return osc.getPhase();
    }
    void sine(float *io1, int n1)
    {
        osc.sine(io1, n1);
    }
    void triangle(float *io1, int n1)
    {
        osc.triangle(io1, n1);
    }
    void saw(float *io1, int n1)
    {
        osc.saw(io1, n1);
    }
    void square(float *io1, int n1)
    {
        osc.square(io1, n1);
    }
    void pulse(float *io1, int n1, float *in2, int n2)
    {
        osc.pulse(io1, n1, in2, n2);
    }
private:
    ofxOfeliaOsc osc;
};

class pdBlOsc
{
public:
    pdBlOsc(){};
    void setPhase(t_floatarg f)
    {
        blOsc.setPhase(f);
    }
    t_float getPhase()
    {
        return blOsc.getPhase();
    }
    void triangle(float *io1, int n1)
    {
        blOsc.triangle(io1, n1);
    }
    void saw(float *io1, int n1)
    {
        blOsc.saw(io1, n1);
    }
    void square(float *io1, int n1)
    {
        blOsc.square(io1, n1);
    }
    void pulse(float *io1, int n1, float *in2, int n2)
    {
        blOsc.pulse(io1, n1, in2, n2);
    }
private:
    ofxOfeliaBlOsc blOsc;
};

class pdFilter
{
public:
    pdFilter(){};
    void clear() /* clear internal state to zero */
    {
        filter.clear();
    }
    void lowPass(float *io1, int n1, float *in2, int n2, float *in3, int n3)
    {
        filter.lowPass(io1, n1, in2, n2, in3, n3);
    }
    void highPass(float *io1, int n1, float *in2, int n2, float *in3, int n3)
    {
        filter.highPass(io1, n1, in2, n2, in3, n3);
    }
    void bandPass(float *io1, int n1, float *in2, int n2, float *in3, int n3)
    {
        filter.bandPass(io1, n1, in2, n2, in3, n3);
    }
    void notch(float *io1, int n1, float *in2, int n2, float *in3, int n3)
    {
        filter.notch(io1, n1, in2, n2, in3, n3);
    }
    void peaking(float *io1, int n1, float *in2, int n2,
                 float *in3, int n3, float *in4, int n4)
    {
        filter.peaking(io1, n1, in2, n2, in3, n3, in4, n4);
    }
    void lowShelf(float *io1, int n1, float *in2, int n2,
                  float *in3, int n3, float *in4, int n4)
    {
        filter.lowShelf(io1, n1, in2, n2, in3, n3, in4, n4);
    }
    void highShelf(float *io1, int n1, float *in2, int n2,
                   float *in3, int n3, float *in4, int n4)
    {
        filter.highShelf(io1, n1, in2, n2, in3, n3, in4, n4);
    }
    void allPass(float *io1, int n1, float *in2, int n2, float *in3, int n3)
    {
        filter.allPass(io1, n1, in2, n2, in3, n3);
    }
private:
    ofxOfeliaFilter filter;
};

class pdReverb
{
public:
    pdReverb()
    {
        setRoomSize(0.5f);
        setDamp(0.5f);
        setWidth(0.5f);
        setWet(0.5f);
        setDry(0.5f);
    };
    void setRoomSize(t_floatarg f)
    {
        reverb.setroomsize(f);
    }
    void setDamp(t_floatarg f)
    {
        reverb.setdamp(f);
    }
    void setWidth(t_floatarg f)
    {
        reverb.setwidth(f);
    }
    void setWet(t_floatarg f)
    {
        reverb.setwet(f);
    }
    void setDry(t_floatarg f)
    {
        reverb.setdry(f);
    }
    void setMode(t_floatarg f)
    {
        reverb.setmode(f);
    }
    t_float getRoomSize()
    {
        return reverb.getroomsize();
    }
    t_float getDamp()
    {
        return reverb.getdamp();
    }
    t_float getWidth()
    {
        return reverb.getwidth();
    }
    t_float getWet()
    {
        return reverb.getwet();
    }
    t_float getDry()
    {
        return reverb.getdry();
    }
    t_float getMode()
    {
        return reverb.getmode();
    }
    void process(float *io1, int n1, float *io2, int n2)
    {
        reverb.process(io1, io2, n1);
    }
private:
    ofxReverb reverb;
};

class pdLog
{
public:
    pdLog()
    :sym(&s_){};
    pdLog(t_symbol *s)
    :sym(s){};
    void post(const std::string &str, int level = 2)
    {
        if (*sym->s_name)
            logpost(NULL, level, "%s: %s", sym->s_name, str.c_str());
        else
            logpost(NULL, level, "%s", str.c_str());
    }
    void error(const std::string &str)
    {
        post(str, 1);
    }
private:
    t_symbol *sym; /* module name */
};

class pdJson
{
public:
    pdJson(){};
    void setDelimiter(const std::string &delim)
    {
        Settings::get().setDelimiter(delim);
    }
    bool load(const std::string &file)
    {
        return Settings::get().load(file);
    }
    bool save(const std::string &file, bool prettyPrint = true)
    {
        return Settings::get().save(file);
    }
    std::string getAsJsonString()
    {
        return Settings::get().getAsJsonString();
    }
    std::string getString(const std::string &key)
    {
        return Settings::get().getString(key);
    }
    bool getBool(const std::string &key)
    {
        return Settings::get().getBool(key);
    }
    int getInt(const std::string &key)
    {
        return Settings::get().getInt(key);
    }
    float getFloat(const std::string &key)
    {
        return Settings::get().getFloat(key);
    }
    double getDouble(const std::string &key)
    {
        return Settings::get().getDouble(key);
    }
    void setString(const std::string &key, const std::string &value)
    {
        Settings::get().getString(key) = value;
    }
    void setBool(const std::string &key, bool value)
    {
        Settings::get().getBool(key) = value;
    }
    void setInt(const std::string &key, int value)
    {
        Settings::get().getInt(key) = value;
    }
    void setFloat(const std::string &key, float value)
    {
        Settings::get().getFloat(key) = value;
    }
    void setDouble(const std::string &key, double value)
    {
        Settings::get().getDouble(key) = value;
    }
    bool exists(const std::string &key)
    {
        return Settings::get().exists(key);
    }
    bool remove(const std::string &key)
    {
        return Settings::get().remove(key);
    }
private:
};

class pdOscSender
{
public:
    pdOscSender(){};
    virtual ~pdOscSender()
    {
        clear();
    };
    bool setup(const std::string &host, int port)
    {
        return sender.setup(host, port);
    }
    void clear()
    {
        sender.clear();
    }
    void send(int argc, t_atom *argv, std::deque<int> userDataRef)
    {
        if (argv[0].a_type != A_SYMBOL)
        {
            error("ofelia: wrong type for OSC address");
            return;
        }
        ofxOscMessage m;
        m.setAddress(argv[0].a_w.w_symbol->s_name);
        for (int i = 1; i < argc; ++i)
        {
            if (argv[i].a_type == A_FLOAT)
                m.addFloatArg(argv[i].a_w.w_float);
            else if (argv[i].a_type == A_SYMBOL)
                m.addStringArg(argv[i].a_w.w_symbol->s_name);
        }
        sender.sendMessage(m);
    }
    std::string getHost()
    {
        return sender.getHost();
    }
    int getPort()
    {
        return sender.getPort();
    }
private:
    ofxOscSender sender;
};

class pdOscReceiver
{
public:
    pdOscReceiver(t_symbol *s)
    :x(ofxOfeliaData::getPtr(s))
    ,sym(gensym("received"))
    ,clock(clock_new(this, reinterpret_cast<t_method>(receivedFunction)))
    ,pollingInterval(10.0){};
    pdOscReceiver(t_symbol *s, t_symbol *s2)
    :x(ofxOfeliaData::getPtr(s))
    ,sym(s2)
    ,clock(clock_new(this, reinterpret_cast<t_method>(receivedFunction)))
    ,pollingInterval(10.0){};
    virtual ~pdOscReceiver()
    {
        clock_free(clock);
        receiver.stop();
    };
    bool setup(int port)
    {
        clock_delay(clock, 0.0);
        return receiver.setup(port);
    }
    bool start()
    {
        clock_delay(clock, 0.0);
        return receiver.start();
    }
    void stop() /* must be called on ofelia.free() */
    {
        clock_unset(clock);
        receiver.stop();
    }
    void setPollingInterval(double d)
    {
        pollingInterval = d;
    }
    bool isListening()
    {
        return receiver.isListening();
    }
    bool hasWaitingMessages()
    {
        return receiver.hasWaitingMessages();
    }
    int getPort()
    {
        return receiver.getPort();
    }
private:
    static void receivedFunction(pdOscReceiver *oscReceiver)
    {
        if (oscReceiver->x == nullptr) return;
        while (oscReceiver->receiver.hasWaitingMessages())
        {
            ofxOscMessage m;
            oscReceiver->receiver.getNextMessage(m);
            int ac = m.getNumArgs() + 1;
            t_atom *av = static_cast<t_atom *>(getbytes(sizeof(t_atom) * ac));
            SETSYMBOL(av, gensym(m.getAddress().c_str()));
            for (size_t i = 0; i < m.getNumArgs(); ++i)
            {
                switch (m.getArgType(i))
                {
                    case OFXOSC_TYPE_INT32:
                        SETFLOAT(av + 1 + i, static_cast<t_float>(m.getArgAsInt32(i)));
                        break;
                    case OFXOSC_TYPE_INT64:
                        SETFLOAT(av + 1 + i, static_cast<t_float>(m.getArgAsInt64(i)));
                        break;
                    case OFXOSC_TYPE_FLOAT:
                        SETFLOAT(av + 1 + i, static_cast<t_float>(m.getArgAsFloat(i)));
                        break;
                    case OFXOSC_TYPE_DOUBLE:
                        SETFLOAT(av + 1 + i, static_cast<t_float>(m.getArgAsDouble(i)));
                        break;
                    case OFXOSC_TYPE_STRING:
                        SETSYMBOL(av + 1 + i, gensym(m.getArgAsString(i).c_str()));
                        break;
                    case OFXOSC_TYPE_SYMBOL:
                        SETSYMBOL(av + 1 + i, gensym(m.getArgAsSymbol(i).c_str()));
                        break;
                    case OFXOSC_TYPE_CHAR:
                        SETFLOAT(av + 1 + i, static_cast<t_float>(m.getArgAsChar(i)));
                        break;
                    case OFXOSC_TYPE_MIDI_MESSAGE:
                        SETFLOAT(av + 1 + i, static_cast<t_float>(m.getArgAsMidiMessage(i)));
                        break;
                    case OFXOSC_TYPE_TRUE:
                        SETFLOAT(av + 1 + i, 1.0f);
                        break;
                    case OFXOSC_TYPE_FALSE:
                        SETFLOAT(av + 1 + i, 0.0f);
                        break;
                    default:
                        break;
                }
            }
            oscReceiver->x->lua.doFunction(oscReceiver->sym, ac, av);
            freebytes(av, sizeof(t_atom) * ac);
        }
        clock_delay(oscReceiver->clock, oscReceiver->pollingInterval);
    };
    ofxOfeliaData *x;
    t_symbol *sym;
    t_clock *clock;
    double pollingInterval;
    ofxOscReceiver receiver;
};

static void pdSysGui(std::string str)
{
    str += '\n';
    sys_gui(const_cast<char*>(str.c_str()));
}

static void pdEmscriptenRunScript(std::string str)
{
    emscripten_run_script(const_cast<char*>(str.c_str()));
}

class pdEM_ASM
{
public:
    pdEM_ASM(){};
    void elementSendFloatToText(std::string str, float number)
    {    
        EM_ASM_(document.getElementById(UTF8ToString($0)).textContent = Number($1).toFixed(2), str.c_str(), number);
    }
    int elementReceiveInt(std::string str)
    {
        return EM_ASM_INT(return document.getElementById(UTF8ToString($0)).value, str.c_str());
    } 
    int sendInt(std::string str, int number)
    {
        EM_ASM_INT(return window[UTF8ToString($0)] = $1, str.c_str(), number);
        return number;
    }
    float sendFloatArray(std::string str, float note, float velocity, float pitch)
    {
        EM_ASM_DOUBLE(return window[UTF8ToString($0)] = ([$1,$2,$3]), str.c_str(), note, velocity, pitch);
    }
    float receiveFloatArray(std::string str, float note, float velocity, float pitch)
    {
        return EM_ASM_DOUBLE(return window[UTF8ToString($0)] = ([$1,$2,$3]), str.c_str(), note, velocity, pitch);
    }
    int sendIntArray(std::string str, int note, int velocity, int pitch)
    {
        EM_ASM_INT(return window[UTF8ToString($0)] = ([$1,$2,$3]), str.c_str(), note, velocity, pitch);
    }
    int receiveIntArray(std::string str, int note, int velocity, int pitch)
    {
        return EM_ASM_INT(return window[UTF8ToString($0)] = ([$1,$2,$3]), str.c_str(), note, velocity, pitch);
    }
    int receiveInt(std::string str)

    {
        return EM_ASM_INT(return window[UTF8ToString($0)], str.c_str());
    }
    float sendFloat(std::string str, float number)
    {    
        EM_ASM_DOUBLE(return window[UTF8ToString($0)] = $1, str.c_str(), number);
        return number;
    }
    float receiveFloat(std::string str)
    {
        return EM_ASM_DOUBLE(return window[UTF8ToString($0)], str.c_str());
    }
    void sendSymbol(std::string str, std::string str2)
    {    
        EM_ASM_(return window[UTF8ToString($0)] = UTF8ToString($1), str.c_str(), str2.c_str());
    }
private:
};

static int pdGetBlockSize()
{
    return sys_getblksize();
}

static t_float pdGetSampleRate()
{
    return sys_getsr();
}

static int pdGetNumInChannels()
{
    return sys_get_inchannels();
}

static int pdGetNumOutChannels()
{
    return sys_get_outchannels();
}

static bool pdGetDspState()
{
    return pd_getdspstate() != 0;
}

static int pdGetMaxString()
{
    return MAXPDSTRING;
}

static int pdGetFloatSize()
{
    return PD_FLOATSIZE;
}

static t_float pdGetMinFloat()
{
    return std::numeric_limits<t_float>::lowest();
}

static t_float pdGetMaxFloat()
{
    return std::numeric_limits<t_float>::max();
}

static bool pdIsBadFloat(t_floatarg f)
{
    return PD_BADFLOAT(f) != 0;
}

static bool pdIsBigOrSmall(t_floatarg f)
{
    return PD_BIGORSMALL(f) != 0;
}

static int pdGetPdVersionMajor()
{
    int major, minor, bugfix;
    sys_getversion(&major, &minor, &bugfix);
    return major;
}

static int pdGetPdVersionMinor()
{
    int major, minor, bugfix;
    sys_getversion(&major, &minor, &bugfix);
    return minor;
}

static int pdGetPdVersionBugFix()
{
    int major, minor, bugfix;
    sys_getversion(&major, &minor, &bugfix);
    return bugfix;
}

static int pdGetOfeliaVersionMajor()
{
    return ofeliaVersionMajor;
}

static int pdGetOfeliaVersionMinor()
{
    return ofeliaVersionMinor;
}

static int pdGetOfeliaVersionBugFix()
{
    return ofeliaVersionBugFix;
}

static bool pdUnZipPass(const std::string &fileName, const std::string &passWord, std::string dirName = "")
{
    ofFile file(fileName);
    if (!file.exists())
    {
        error("ofelia: the file '%s' does not exist", fileName.c_str());
        return false;
    }
    if (dirName == "")
        dirName = file.getEnclosingDirectory();
    ofxUnzipPass zip(fileName, passWord);
    if (!zip.isOk())
    {
        error("ofelia: failed to open '%s'", fileName.c_str());
        return false;
    }
    return zip.unzipTo(dirName);
}

static bool pdUnZip(const std::string &fileName, std::string dirName = "")
{
    return pdUnZipPass(fileName, "", dirName);
}
