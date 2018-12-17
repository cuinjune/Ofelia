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

#include "ofMain.h"
#include "ofeliaLua.h"
#include "ofeliaData.h"

class OFWindow
{
public:
    OFWindow()
    :clock(clock_new(this, reinterpret_cast<t_method>(loop)))
    ,glVersionMajor(2)
    ,glVersionMinor(1)
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
#if !defined( TARGET_OF_IOS ) & !defined(TARGET_ANDROID) & !defined(TARGET_EMSCRIPTEN) & !defined(TARGET_RASPBERRY_PI)
        ofGLFWWindowSettings settings;
        settings.setGLVersion(glVersionMajor, glVersionMinor);
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
#else
        ofWindowSettings settings;
#endif
        settings.title = title->s_name;
        settings.windowMode = static_cast<ofWindowMode>(windowMode);
        if (positionSet) settings.setPosition(position);
        if (sizeSet) settings.setSize(width, height);
        ofCreateWindow(settings);
        ofSetLogLevel(OF_LOG_SILENT);
        mainLoop = ofGetMainLoop().get();
        mainLoop->setEscapeQuitsLoop(false);
        windowPtr = ofGetWindowPtr();
#if !defined( TARGET_OF_IOS ) & !defined(TARGET_ANDROID) & !defined(TARGET_EMSCRIPTEN) & !defined(TARGET_RASPBERRY_PI)
        ofAppGLFWWindow *ofAppGLFWwin = static_cast<ofAppGLFWWindow *>(windowPtr);
        scale = ofAppGLFWwin->getPixelScreenCoordScale();
#endif
        ofResetElapsedTimeCounter();
        ofSetFrameRate(60);
        owner = true;
        exists = true;
        addWindowListeners();
        windowPtr->events().notifySetup();
        clock_delay(clock, 0.0);
    }
    void destroy()
    {
        if (!owner || !exists) return;
        ofEventArgs e;
        exit(e);
        removeWindowListeners();
        windowPtr->setWindowShouldClose();
        owner = false;
        exists = false;
    };
    static void addListener(t_symbol *s, t_symbol *s2, t_floatarg f)
    {
        ofeliaData *x = ofeliaData::getPtr(s2);
        if (x == nullptr) return;
        DataPairVec *vecPtr = getTargetDataPairVec(s);
        if (vecPtr == nullptr) return;
        addDataPair(*vecPtr, x, f);
    }
    static void addListener(t_symbol *s, t_symbol *s2)
    {
        addListener(s, s2, 0);
    }
    static void removeListener(t_symbol *s, t_symbol *s2)
    {
        ofeliaData *x = ofeliaData::getPtr(s2);
        if (x == nullptr) return;
        DataPairVec *vecPtr = getTargetDataPairVec(s);
        if (vecPtr == nullptr) return;
        removeDataPair(*vecPtr, x);
    }
    static vector<pair<ofeliaData *, t_float>> getListenerData(t_symbol *s)
    {
        DataPairVec *vecPtr = getTargetDataPairVec(s);
        if (vecPtr == nullptr) return {};
        sortDataPairVec(*vecPtr);
        return *vecPtr;
    }
    int glVersionMajor;
    int glVersionMinor;
    t_symbol *title;
    int windowMode;
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
    static bool exists;
    static int scale;
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
    void callLuaFunction(ofeliaData *x, ofEventArgs &e, t_symbol *s)
    {
        x->lua.doFunction(s);
    }
    void callLuaFunction(ofeliaData *x, ofKeyEventArgs &e, t_symbol *s)
    {
        int top; if (!x->lua.isFunction(s, top)) return;
        lua_State *L = x->lua.L;
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
        x->lua.callFunction(top);
    }
    void callLuaFunction(ofeliaData *x, ofMouseEventArgs &e, t_symbol *s)
    {
        int top; if (!x->lua.isFunction(s, top)) return;
        lua_State *L = x->lua.L;
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
        x->lua.callFunction(top);
    }
    void callLuaFunction(ofeliaData *x, ofResizeEventArgs &e, t_symbol *s)
    {
        int top; if (!x->lua.isFunction(s, top)) return;
        lua_State *L = x->lua.L;
        lua_newtable(L);
        lua_pushinteger(L, static_cast<lua_Integer>(e.width));
        lua_setfield(L, -2, "width");
        lua_pushinteger(L, static_cast<lua_Integer>(e.height));
        lua_setfield(L, -2, "height");
        x->lua.callFunction(top);
    }
    void callLuaFunction(ofeliaData *x, ofMessage &e, t_symbol *s)
    {
        int top; if (!x->lua.isFunction(s, top)) return;
        lua_State *L = x->lua.L;
        lua_newtable(L);
        lua_pushstring(L, e.message.c_str());
        lua_setfield(L, -2, "message");
        x->lua.callFunction(top);
    }
    void callLuaFunction(ofeliaData *x, ofDragInfo &e, t_symbol *s)
    {
        int top; if (!x->lua.isFunction(s, top)) return;
        lua_State *L = x->lua.L;
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
        x->lua.callFunction(top);
    }
    void callLuaFunction(ofeliaData *x, ofTouchEventArgs &e, t_symbol *s)
    {
        int top; if (!x->lua.isFunction(s, top)) return;
        lua_State *L = x->lua.L;
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
        x->lua.callFunction(top);
    }
    void setup(ofEventArgs &e)
    {
        callEventListeners(setupVec, e, gensym("setup"));
    }
    void update(ofEventArgs &e)
    {
        callEventListeners(updateVec, e, gensym("update"));
    }
    void draw(ofEventArgs &e)
    {
        callEventListeners(drawVec, e, gensym("draw"));
    }
    void exit(ofEventArgs &e)
    {
        callEventListeners(exitVec, e, gensym("exit"));
        removeWindowListeners();
        owner = false;
        exists = false;
    }
    void keyPressed(ofKeyEventArgs &e)
    {
        callEventListeners(keyPressedVec, e, gensym("keyPressed"));
    }
    void keyReleased(ofKeyEventArgs &e)
    {
        callEventListeners(keyReleasedVec, e, gensym("keyReleased"));
    }
    void mouseMoved(ofMouseEventArgs &e)
    {
        callEventListeners(mouseMovedVec, e, gensym("mouseMoved"));
    }
    void mouseDragged(ofMouseEventArgs &e)
    {
        callEventListeners(mouseDraggedVec, e, gensym("mouseDragged"));
    }
    void mousePressed(ofMouseEventArgs &e)
    {
        callEventListeners(mousePressedVec, e, gensym("mousePressed"));
    }
    void mouseReleased(ofMouseEventArgs &e)
    {
        callEventListeners(mouseReleasedVec, e, gensym("mouseReleased"));
    }
    void mouseScrolled(ofMouseEventArgs &e)
    {
        callEventListeners(mouseScrolledVec, e, gensym("mouseScrolled"));
    }
    void mouseEntered(ofMouseEventArgs &e)
    {
        callEventListeners(mouseEnteredVec, e, gensym("mouseEntered"));
    }
    void mouseExited(ofMouseEventArgs &e)
    {
        callEventListeners(mouseExitedVec, e, gensym("mouseExited"));
    }
    void windowResized(ofResizeEventArgs &e)
    {
        callEventListeners(windowResizedVec, e, gensym("windowResized"));
    }
    void messageReceived(ofMessage &e)
    {
        callEventListeners(messageReceivedVec, e, gensym("messageReceived"));
    }
    void dragged(ofDragInfo &e)
    {
        callEventListeners(draggedVec, e, gensym("dragged"));
    }
    void touchCancelled(ofTouchEventArgs &e)
    {
        callEventListeners(touchCancelledVec, e, gensym("touchCancelled"));
    }
    void touchDoubleTap(ofTouchEventArgs &e)
    {
        callEventListeners(touchDoubleTapVec, e, gensym("touchDoubleTap"));
    }
    void touchDown(ofTouchEventArgs &e)
    {
        callEventListeners(touchDownVec, e, gensym("touchDown"));
    }
    void touchMoved(ofTouchEventArgs &e)
    {
        callEventListeners(touchMovedVec, e, gensym("touchMoved"));
    }
    void touchUp(ofTouchEventArgs &e)
    {
        callEventListeners(touchUpVec, e, gensym("touchUp"));
    }
    static void loop(OFWindow *window)
    {
        if (window->exists)
            clock_delay(window->clock, 1000.0 / static_cast<double>(ofGetTargetFrameRate()));
        window->mainLoop->loopOnce();
        window->mainLoop->pollEvents();
    };
    typedef pair<ofeliaData *, t_float> DataPair;
    typedef vector<DataPair> DataPairVec;
    static DataPairVec *getTargetDataPairVec(t_symbol *s)
    {
        if (!strcmp(s->s_name, "setup"))
            return &setupVec;
        else if (!strcmp(s->s_name, "update"))
            return &updateVec;
        else if (!strcmp(s->s_name, "draw"))
            return &drawVec;
        else if (!strcmp(s->s_name, "exit"))
            return &exitVec;
        else if (!strcmp(s->s_name, "keyPressed"))
            return &keyPressedVec;
        else if (!strcmp(s->s_name, "keyReleased"))
            return &keyReleasedVec;
        else if (!strcmp(s->s_name, "mouseMoved"))
            return &mouseMovedVec;
        else if (!strcmp(s->s_name, "mouseDragged"))
            return &mouseDraggedVec;
        else if (!strcmp(s->s_name, "mousePressed"))
            return &mousePressedVec;
        else if (!strcmp(s->s_name, "mouseReleased"))
            return &mouseReleasedVec;
        else if (!strcmp(s->s_name, "mouseScrolled"))
            return &mouseScrolledVec;
        else if (!strcmp(s->s_name, "mouseEntered"))
            return &mouseEnteredVec;
        else if (!strcmp(s->s_name, "mouseExited"))
            return &mouseExitedVec;
        else if (!strcmp(s->s_name, "windowResized"))
            return &windowResizedVec;
        else if (!strcmp(s->s_name, "messageReceived"))
            return &messageReceivedVec;
        else if (!strcmp(s->s_name, "dragged"))
            return &draggedVec;
        else if (!strcmp(s->s_name, "touchCancelled"))
            return &touchCancelledVec;
        else if (!strcmp(s->s_name, "touchDoubleTap"))
            return &touchDoubleTapVec;
        else if (!strcmp(s->s_name, "touchDown"))
            return &touchDownVec;
        else if (!strcmp(s->s_name, "touchMoved"))
            return &touchMovedVec;
        else if (!strcmp(s->s_name, "touchUp"))
            return &touchUpVec;
        else
            error("ofelia: unknown listener method '%s'", s->s_name);
        return nullptr;
    }
    static DataPairVec::iterator findDataPair(DataPairVec &vec, ofeliaData *dataPtr)
    {
        return find_if(vec.begin(), vec.end(),
                       [&](const DataPair &ref) {return ref.first == dataPtr;});
    }
    static void addDataPair(DataPairVec &vec, ofeliaData *x, t_floatarg f)
    {
        auto it = findDataPair(vec, x);
        if (it != vec.end())
        {
            it->second = f;
            return;
        }
        vec.push_back(make_pair(x, f));
    }
    static void removeDataPair(DataPairVec &vec, ofeliaData *x)
    {
        auto it = findDataPair(vec, x);
        if (it != vec.end())
            vec.erase(it);
    }
    static void sortDataPairVec(DataPairVec &vec)
    {
        stable_sort(vec.begin(), vec.end(),
                    [](const auto &a, const auto &b) {return a.second < b.second;});
    }
    template<typename T>
    void callEventListeners(DataPairVec &vec, T &e, t_symbol *s)
    {
        sortDataPairVec(vec);
        for (const auto &i : vec)
            callLuaFunction(i.first, e, s);
    }
    ofeliaData *x;
    t_clock *clock;
    ofMainLoop *mainLoop;
    ofAppBaseWindow *windowPtr;
    glm::vec2 position;
    int width;
    int height;
    bool positionSet;
    bool sizeSet;
    bool owner;
    static DataPairVec setupVec;
    static DataPairVec updateVec;
    static DataPairVec drawVec;
    static DataPairVec exitVec;
    static DataPairVec keyPressedVec;
    static DataPairVec keyReleasedVec;
    static DataPairVec mouseMovedVec;
    static DataPairVec mouseDraggedVec;
    static DataPairVec mousePressedVec;
    static DataPairVec mouseReleasedVec;
    static DataPairVec mouseScrolledVec;
    static DataPairVec mouseEnteredVec;
    static DataPairVec mouseExitedVec;
    static DataPairVec windowResizedVec;
    static DataPairVec messageReceivedVec;
    static DataPairVec draggedVec;
    static DataPairVec touchCancelledVec;
    static DataPairVec touchDoubleTapVec;
    static DataPairVec touchDownVec;
    static DataPairVec touchMovedVec;
    static DataPairVec touchUpVec;
};

bool OFWindow::exists = false;
int OFWindow::scale = 1;
OFWindow::DataPairVec OFWindow::setupVec;
OFWindow::DataPairVec OFWindow::updateVec;
OFWindow::DataPairVec OFWindow::drawVec;
OFWindow::DataPairVec OFWindow::exitVec;
OFWindow::DataPairVec OFWindow::keyPressedVec;
OFWindow::DataPairVec OFWindow::keyReleasedVec;
OFWindow::DataPairVec OFWindow::mouseMovedVec;
OFWindow::DataPairVec OFWindow::mouseDraggedVec;
OFWindow::DataPairVec OFWindow::mousePressedVec;
OFWindow::DataPairVec OFWindow::mouseReleasedVec;
OFWindow::DataPairVec OFWindow::mouseScrolledVec;
OFWindow::DataPairVec OFWindow::mouseEnteredVec;
OFWindow::DataPairVec OFWindow::mouseExitedVec;
OFWindow::DataPairVec OFWindow::windowResizedVec;
OFWindow::DataPairVec OFWindow::messageReceivedVec;
OFWindow::DataPairVec OFWindow::draggedVec;
OFWindow::DataPairVec OFWindow::touchCancelledVec;
OFWindow::DataPairVec OFWindow::touchDoubleTapVec;
OFWindow::DataPairVec OFWindow::touchDownVec;
OFWindow::DataPairVec OFWindow::touchMovedVec;
OFWindow::DataPairVec OFWindow::touchUpVec;

class Canvas
{
public:
    Canvas(t_symbol *s)
    :x(ofeliaData::getPtr(s))
    {
        initCanvas(s, 0);
    };
    Canvas(t_symbol *s, t_floatarg f)
    :x(ofeliaData::getPtr(s))
    {
        initCanvas(s, f);
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
    void setArgs(int argc, t_atom *argv)
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
    ofeliaData *x;
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
        int userDataRef = luaL_ref(ofeliaLua::L, LUA_REGISTRYINDEX);
        if (sym->s_thing) pd_pointer(sym->s_thing, reinterpret_cast<t_gpointer *>(&userDataRef));
        luaL_unref(ofeliaLua::L, LUA_REGISTRYINDEX, userDataRef);
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

class Inlet
{
public:
    Inlet(t_symbol *s)
    :x(ofeliaData::getPtr(s)){};
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
    ofeliaData *x;
};

class Outlet
{
public:
    Outlet(t_symbol *s)
    :x(ofeliaData::getPtr(s)){};
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
            int userDataRef = luaL_ref(ofeliaLua::L, LUA_REGISTRYINDEX);
            outlet_pointer(x->io.outlets[index], reinterpret_cast<t_gpointer *>(&userDataRef));
            luaL_unref(ofeliaLua::L, LUA_REGISTRYINDEX, userDataRef);
        }
    }
    void outletList(int index, int argc, t_atom *argv)
    {
        if (!isOutletValid(index)) return;
        outlet_list(x->io.outlets[index], &s_list, argc, argv);
    }
    void outletAnything(int index, int argc, t_atom *argv)
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
    ofeliaData *x;
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
    :x(ofeliaData::getPtr(s))
    ,sym(gensym("delayed"))
    ,clock(clock_new(this, reinterpret_cast<t_method>(delayedFunction))){};
    Clock(t_symbol *s, t_symbol *s2)
    :x(ofeliaData::getPtr(s))
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
    ofeliaData *x;
    t_symbol *sym;
    t_clock *clock;
};

class Sys
{
public:
    double getRealTime()
    {
        return sys_getrealtime();
    }
    void lock()
    {
        sys_lock();
    }
    void unlock()
    {
        sys_unlock();
    }
    int tryLock()
    {
        return sys_trylock();
    }
    void gui(t_symbol *s)
    {
        char buf[MAXPDSTRING];
        snprintf(buf, MAXPDSTRING, "%s\n", s->s_name);
        sys_gui(buf);
    }
private:
};

class Signal
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
    bool getDspState()
    {
        return pd_getdspstate() != 0;
    }
private:
};

class PD
{
public:
    PD()
    :maxString(MAXPDSTRING)
    ,floatSize(PD_FLOATSIZE)
    ,minFloat(numeric_limits<t_float>::lowest())
    ,maxFloat(numeric_limits<t_float>::max())
    {
        int major, minor, bugfix;
        sys_getversion(&major, &minor, &bugfix);
        version = make_tuple(major, minor, bugfix);
    };
    int getMaxString()
    {
        return maxString;
    }
    int getFloatSize()
    {
        return floatSize;
    }
    t_float getMinFloat()
    {
        return minFloat;
    }
    t_float getMaxFloat()
    {
        return maxFloat;
    }
    bool isBadFloat(t_floatarg f)
    {
        return PD_BADFLOAT(f) != 0;
    }
    bool isBigOrSmall(t_floatarg f)
    {
        return PD_BIGORSMALL(f) != 0;
    }
    tuple<int, int, int> getVersion()
    {
        return version;
    }
    int maxString;
    int floatSize;
    t_float minFloat;
    t_float maxFloat;
    tuple<int, int, int> version;
private:
};

class Log
{
public:
    void post(const char *s)
    {
        logpost(NULL, 2, "%s", s);
    }
    void post(const char *s, int level)
    {
        logpost(NULL, level, "%s", s);
    }
    void startPost(const char *s)
    {
        startpost("%s", s);
    }
    void postString(const char *s)
    {
        poststring(s);
    }
    void postFloat(t_floatarg f)
    {
        postfloat(f);
    }
    void postAtom(int argc, t_atom *argv)
    {
        postatom(argc, argv);
    }
    void endPost()
    {
        endpost();
    }
    void error(const char *s)
    {
        logpost(NULL, 1, "%s", s);
    }
private:
};

#endif /* ofeliaBindings_h */
