#pragma once

#include "m_pd.h"
#include "g_canvas.h"
#include "lua.hpp"
#include "ofMain.h"
#include "ofxOfeliaLua.h"
#include "ofxOfeliaData.h"
#include "ofxOfeliaEvents.h"
#include "ofxOfeliaAudio.h"

class OFWindow
{
public:
    OFWindow()
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
    virtual ~OFWindow()
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
    static vector<pair<ofxOfeliaData *, t_float>> getListenerData(t_symbol *s)
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
        ofAddListener(windowPtr->events().setup, this, &OFWindow::setup);
        ofAddListener(windowPtr->events().update, this, &OFWindow::update);
        ofAddListener(windowPtr->events().draw, this, &OFWindow::draw);
        ofAddListener(windowPtr->events().exit, this, &OFWindow::exit);
        ofAddListener(windowPtr->events().keyPressed, this, &OFWindow::keyPressed);
        ofAddListener(windowPtr->events().keyReleased, this, &OFWindow::keyReleased);
        ofAddListener(windowPtr->events().mouseMoved, this, &OFWindow::mouseMoved);
        ofAddListener(windowPtr->events().mouseDragged, this, &OFWindow::mouseDragged);
        ofAddListener(windowPtr->events().mousePressed, this, &OFWindow::mousePressed);
        ofAddListener(windowPtr->events().mouseReleased, this, &OFWindow::mouseReleased);
        ofAddListener(windowPtr->events().mouseScrolled, this, &OFWindow::mouseScrolled);
        ofAddListener(windowPtr->events().mouseEntered, this, &OFWindow::mouseEntered);
        ofAddListener(windowPtr->events().mouseExited, this, &OFWindow::mouseExited);
        ofAddListener(windowPtr->events().windowResized, this, &OFWindow::windowResized);
        ofAddListener(windowPtr->events().messageEvent, this, &OFWindow::messageReceived);
        ofAddListener(windowPtr->events().fileDragEvent, this, &OFWindow::dragged);
        ofAddListener(windowPtr->events().touchCancelled, this, &OFWindow::touchCancelled);
        ofAddListener(windowPtr->events().touchDoubleTap, this, &OFWindow::touchDoubleTap);
        ofAddListener(windowPtr->events().touchDown, this, &OFWindow::touchDown);
        ofAddListener(windowPtr->events().touchMoved, this, &OFWindow::touchMoved);
        ofAddListener(windowPtr->events().touchUp, this, &OFWindow::touchUp);
    }
    void removeWindowListeners()
    {
        ofRemoveListener(windowPtr->events().setup, this, &OFWindow::setup);
        ofRemoveListener(windowPtr->events().update, this, &OFWindow::update);
        ofRemoveListener(windowPtr->events().draw, this, &OFWindow::draw);
        ofRemoveListener(windowPtr->events().exit, this, &OFWindow::exit);
        ofRemoveListener(windowPtr->events().keyPressed, this, &OFWindow::keyPressed);
        ofRemoveListener(windowPtr->events().keyReleased, this, &OFWindow::keyReleased);
        ofRemoveListener(windowPtr->events().mouseMoved, this, &OFWindow::mouseMoved);
        ofRemoveListener(windowPtr->events().mouseDragged, this, &OFWindow::mouseDragged);
        ofRemoveListener(windowPtr->events().mousePressed, this, &OFWindow::mousePressed);
        ofRemoveListener(windowPtr->events().mouseReleased, this, &OFWindow::mouseReleased);
        ofRemoveListener(windowPtr->events().mouseScrolled, this, &OFWindow::mouseScrolled);
        ofRemoveListener(windowPtr->events().mouseEntered, this, &OFWindow::mouseEntered);
        ofRemoveListener(windowPtr->events().mouseExited, this, &OFWindow::mouseExited);
        ofRemoveListener(windowPtr->events().windowResized, this, &OFWindow::windowResized);
        ofRemoveListener(windowPtr->events().messageEvent, this, &OFWindow::messageReceived);
        ofRemoveListener(windowPtr->events().fileDragEvent, this, &OFWindow::dragged);
        ofRemoveListener(windowPtr->events().touchCancelled, this, &OFWindow::touchCancelled);
        ofRemoveListener(windowPtr->events().touchDoubleTap, this, &OFWindow::touchDoubleTap);
        ofRemoveListener(windowPtr->events().touchDown, this, &OFWindow::touchDown);
        ofRemoveListener(windowPtr->events().touchMoved, this, &OFWindow::touchMoved);
        ofRemoveListener(windowPtr->events().touchUp, this, &OFWindow::touchUp);
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
    static void loop(OFWindow *window)
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

bool OFWindow::exists = false;

class Canvas
{
public:
    Canvas(t_symbol *s)
    :x(ofxOfeliaData::getPtr(s))
    {
        initCanvas(s, 0);
    };
    Canvas(t_symbol *s, t_floatarg f)
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
        snprintf(buf, MAXPDSTRING, ".x%lx.c", reinterpret_cast<unsigned long>(canvas));
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
    static void removeCanvas(Canvas *canvas)
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

class Inlet
{
public:
    Inlet(t_symbol *s)
    :x(ofxOfeliaData::getPtr(s)){};
    void setFloatInlet(t_floatarg f)
    {
        if (!doesFloatInletExist()) return;
        x->io.fv[0] = f;
    }
    void setFloatInlets(int n, t_floatarg *f)
    {
        if (!doesFloatInletExist()) return;
        int numFloatInlets = x->io.numInlets - 1;
        int last = n > numFloatInlets ? numFloatInlets : n;
        for (int i = 0; i < last; ++i)
            x->io.fv[i] = f[i];
    }
    void setSignalInlet(t_floatarg f)
    {
        if (!doesSignalInletExist()) return;
        x->signal.f = f;
    }
private:
    bool doesFloatInletExist()
    {
        if (x == nullptr) return false;
        if (!x->io.hasMultiControlInlets)
        {
            error("ofelia: float inlet does not exist");
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
    ofxOfeliaData *x;
};

class Outlet
{
public:
    Outlet(t_symbol *s)
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
    :x(ofxOfeliaData::getPtr(s))
    ,sym(gensym("delayed"))
    ,clock(clock_new(this, reinterpret_cast<t_method>(delayedFunction))){};
    Clock(t_symbol *s, t_symbol *s2)
    :x(ofxOfeliaData::getPtr(s))
    ,sym(s2)
    ,clock(clock_new(this, reinterpret_cast<t_method>(delayedFunction))){};
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
    static void delayedFunction(Clock *clock)
    {
        if (clock->x == nullptr) return;
        clock->x->lua.doFunction(clock->sym);
    };
    ofxOfeliaData *x;
    t_symbol *sym;
    t_clock *clock;
};

class Osc
{
public:
    Osc(){};
    void setPhase(t_floatarg f)
    {
        audio.setPhase(f);
    }
    t_float getPhase()
    {
        return audio.getPhase();
    }
    void sine(float *in1, int n1)
    {
        audio.sine(in1, n1);
    }
    void triangle(float *in1, int n1)
    {
        audio.triangle(in1, n1);
    }
    void saw(float *in1, int n1)
    {
        audio.saw(in1, n1);
    }
    void square(float *in1, int n1)
    {
        audio.square(in1, n1);
    }
    void pulse(float *in1, int n1, float *in2, int n2)
    {
        audio.pulse(in1, n1, in2, n2);
    }
private:
    ofxOfeliaAudio audio;
};

class BlOsc
{
public:
    BlOsc(){};
    void setPhase(t_floatarg f)
    {
        audio.setPhase(f);
    }
    t_float getPhase()
    {
        return audio.getPhase();
    }
    void triangle(float *in1, int n1)
    {
        audio.blTriangle(in1, n1);
    }
    void saw(float *in1, int n1)
    {
        audio.blSaw(in1, n1);
    }
    void square(float *in1, int n1)
    {
        audio.blSquare(in1, n1);
    }
    void pulse(float *in1, int n1, float *in2, int n2)
    {
        audio.blPulse(in1, n1, in2, n2);
    }
private:
    ofxOfeliaAudio audio;
};

class Filter
{
public:
    Filter(){};
    void clear() /* clear internal state to zero */
    {
        audio.clear();
    }
    void lowPass(float *in1, int n1, float *in2, int n2, float *in3, int n3)
    {
        audio.lowPass(in1, n1, in2, n2, in3, n3);
    }
    void highPass(float *in1, int n1, float *in2, int n2, float *in3, int n3)
    {
        audio.highPass(in1, n1, in2, n2, in3, n3);
    }
    void bandPass(float *in1, int n1, float *in2, int n2, float *in3, int n3)
    {
        audio.bandPass(in1, n1, in2, n2, in3, n3);
    }
    void notch(float *in1, int n1, float *in2, int n2, float *in3, int n3)
    {
        audio.notch(in1, n1, in2, n2, in3, n3);
    }
    void peaking(float *in1, int n1, float *in2, int n2,
                 float *in3, int n3, float *in4, int n4)
    {
        audio.peaking(in1, n1, in2, n2, in3, n3, in4, n4);
    }
    void lowShelf(float *in1, int n1, float *in2, int n2,
                  float *in3, int n3, float *in4, int n4)
    {
        audio.lowShelf(in1, n1, in2, n2, in3, n3, in4, n4);
    }
    void highShelf(float *in1, int n1, float *in2, int n2,
                   float *in3, int n3, float *in4, int n4)
    {
        audio.highShelf(in1, n1, in2, n2, in3, n3, in4, n4);
    }
    void allPass(float *in1, int n1, float *in2, int n2, float *in3, int n3)
    {
        audio.allPass(in1, n1, in2, n2, in3, n3);
    }
private:
    ofxOfeliaAudio audio;
};

class Log
{
public:
    Log()
    :sym(&s_){};
    Log(t_symbol *s)
    :sym(s){};
    void post(const char *s, int level)
    {
        if (*sym->s_name)
            logpost(NULL, level, "%s: %s", sym->s_name, s);
        else
            logpost(NULL, level, "%s", s);
    }
    void post(const char *s)
    {
        post(s, 2);
    }
    void error(const char *s)
    {
        post(s, 1);
    }
private:
    t_symbol *sym; /* module name */
};

static void sysGui(t_symbol *s)
{
    char buf[MAXPDSTRING];
    snprintf(buf, MAXPDSTRING, "%s\n", s->s_name);
    sys_gui(buf);
}

static int getBlockSize()
{
    return sys_getblksize();
}

static t_float getSampleRate()
{
    return sys_getsr();
}

static int getInChannels()
{
    return sys_get_inchannels();
}

static int getOutChannels()
{
    return sys_get_outchannels();
}

static bool getDspState()
{
    return pd_getdspstate() != 0;
}

static int getMaxString()
{
    return MAXPDSTRING;
}

static int getFloatSize()
{
    return PD_FLOATSIZE;
}

static t_float getMinFloat()
{
    return numeric_limits<t_float>::lowest();
}

static t_float getMaxFloat()
{
    return numeric_limits<t_float>::max();
}

static bool isBadFloat(t_floatarg f)
{
    return PD_BADFLOAT(f) != 0;
}

static bool isBigOrSmall(t_floatarg f)
{
    return PD_BIGORSMALL(f) != 0;
}

static tuple<int, int, int> getVersion()
{
    int major, minor, bugfix;
    sys_getversion(&major, &minor, &bugfix);
    return make_tuple(major, minor, bugfix);
}
