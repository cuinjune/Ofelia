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

#define _USE_MATH_DEFINES
#include "ofeliaWindow.h"
#include "ofeliaHead.h"
#include "ofeliaStyle.h"
#include "ofeliaListeners.h"
#include "ofeliaGetters.h"

/* object static variables */
const char *t_ofeliaWindow::objName = "ofWindow";
t_symbol *t_ofeliaWindow::firstLoopSym;
t_symbol *t_ofeliaWindow::firstLoopMess;
t_symbol *t_ofeliaWindow::initSym;
t_symbol *t_ofeliaWindow::initMess;
t_symbol *t_ofeliaWindow::updateSym;
t_symbol *t_ofeliaWindow::updateMess;
t_symbol *t_ofeliaWindow::drawSym;
t_symbol *t_ofeliaWindow::drawMess;
t_symbol *t_ofeliaWindow::exitSym;
t_symbol *t_ofeliaWindow::exitMess;
bool t_ofeliaWindow::bRenderGate;

/* public static variables */
ofAppBaseWindow *ofeliaWindow::window;
GLFWwindow *ofeliaWindow::GLFWwin;
bool ofeliaWindow::bWindowExists;
bool ofeliaWindow::bFirstLoop;
bool ofeliaWindow::bFirstUpdate;
int ofeliaWindow::retinaScale;
float ofeliaWindow::scaleFactor;
bool ofeliaWindow::bDepthTestEnabled;
bool ofeliaWindow::bFullscreenMode;
ofVec2f ofeliaWindow::fullscreenOffset;

/* private static variables */
int ofeliaWindow::windowPosX;
int ofeliaWindow::windowPosY;
int ofeliaWindow::windowWidth;
int ofeliaWindow::windowHeight;
float ofeliaWindow::windowScale;
int ofeliaWindow::widthScaled;
int ofeliaWindow::heightScaled;
int ofeliaWindow::windowResizeCount;
bool ofeliaWindow::bWindowShouldResize;
bool ofeliaWindow::bWindowResizeLocked;
bool ofeliaWindow::bScaleDirectionFixed;
bool ofeliaWindow::bAccelEnabled;
ofVec3f ofeliaWindow::accelForce;
t_clock *ofeliaWindow::pollEventsClock;
GLFWcursor *ofeliaWindow::blankCursor;
unique_ptr<ofPath> ofeliaWindow::fullscreenMaskPath;
unique_ptr<ofVboMesh> ofeliaWindow::fullscreenMaskMesh;

#if defined(TARGET_OSX)
const int ofeliaWindow::fnKey = OF_KEY_COMMAND;
#else
const int ofeliaWindow::fnKey = OF_KEY_CONTROL;
#endif

/* ________________________________________________________________________________
 * ofWindow object methods 
 */
void *ofeliaWindow_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaWindow *x = reinterpret_cast<t_ofeliaWindow*>(pd_new(ofeliaWindow_class));
    x->x_ofeliaWindow = new ofeliaWindow();
    ((ofeliaWindow*)x->x_ofeliaWindow)->x = x;
    if (!((ofeliaWindow*)x->x_ofeliaWindow)->_new(s, argc, argv))
        x = 0;
    return (x);
}

void ofeliaWindow_firstLoop(t_ofeliaWindow *x)
{
    ((ofeliaWindow*)x->x_ofeliaWindow)->_firstLoop();
}

void ofeliaWindow_create(t_ofeliaWindow *x)
{
    ((ofeliaWindow*)x->x_ofeliaWindow)->_create();
}

void ofeliaWindow_destroy(t_ofeliaWindow *x)
{
    ((ofeliaWindow*)x->x_ofeliaWindow)->_destroy();
}

void ofeliaWindow_reset(t_ofeliaWindow *x)
{
    ((ofeliaWindow*)x->x_ofeliaWindow)->_reset();
}

void ofeliaWindow_dimen(t_ofeliaWindow *x, const t_symbol *s, const int argc, const t_atom *argv)
{
    ((ofeliaWindow*)x->x_ofeliaWindow)->_dimen(s, argc, argv);
}

void ofeliaWindow_pos(t_ofeliaWindow *x, const t_symbol *s, const int argc, const t_atom *argv)
{
    ((ofeliaWindow*)x->x_ofeliaWindow)->_pos(s, argc, argv);
}

void ofeliaWindow_scale(t_ofeliaWindow *x, const t_floatarg f)
{
    ((ofeliaWindow*)x->x_ofeliaWindow)->_scale(f);
}

void ofeliaWindow_bgColor(t_ofeliaWindow *x, const t_symbol *s, const int argc, const t_atom *argv)
{
    ((ofeliaWindow*)x->x_ofeliaWindow)->_bgColor(s, argc, argv);
}

void ofeliaWindow_frameRate(t_ofeliaWindow *x, const t_floatarg f)
{
    ((ofeliaWindow*)x->x_ofeliaWindow)->_frameRate(f);
}

void ofeliaWindow_VSync(t_ofeliaWindow *x, const t_symbol *s, const int argc, const t_atom *argv)
{
    ((ofeliaWindow*)x->x_ofeliaWindow)->_VSync(s, argc, argv);
}

void ofeliaWindow_depthTest(t_ofeliaWindow *x, const t_symbol *s, const int argc, const t_atom *argv)
{
    ((ofeliaWindow*)x->x_ofeliaWindow)->_depthTest(s, argc, argv);
}

void ofeliaWindow_orien(t_ofeliaWindow *x, const t_symbol *s, const int argc, const t_atom *argv)
{
    ((ofeliaWindow*)x->x_ofeliaWindow)->_orien(s, argc, argv);
}

void ofeliaWindow_orienLock(t_ofeliaWindow *x, const t_symbol *s, const int argc, const t_atom *argv)
{
    ((ofeliaWindow*)x->x_ofeliaWindow)->_orienLock(s, argc, argv);
}

void ofeliaWindow_accel(t_ofeliaWindow *x, const t_symbol *s, const int argc, const t_atom *argv)
{
    ((ofeliaWindow*)x->x_ofeliaWindow)->_accel(s, argc, argv);
}

void ofeliaWindow_accelSmoothing(t_ofeliaWindow *x, const t_floatarg f)
{
    ((ofeliaWindow*)x->x_ofeliaWindow)->_accelSmoothing(f);
}

void ofeliaWindow_cursor(t_ofeliaWindow *x, const t_symbol *s, const int argc, const t_atom *argv)
{
    ((ofeliaWindow*)x->x_ofeliaWindow)->_cursor(s, argc, argv);
}

void ofeliaWindow_fullscreen(t_ofeliaWindow *x, const t_symbol *s, const int argc, const t_atom *argv)
{
    ((ofeliaWindow*)x->x_ofeliaWindow)->_fullscreen(s, argc, argv);
}

void ofeliaWindow_hotkeys(t_ofeliaWindow *x, const t_symbol *s, const int argc, const t_atom *argv)
{
    ((ofeliaWindow*)x->x_ofeliaWindow)->_hotkeys(s, argc, argv);
}

void ofeliaWindow_title(t_ofeliaWindow *x, const t_symbol *s, const int argc, const t_atom *argv)
{
    ((ofeliaWindow*)x->x_ofeliaWindow)->_title(s, argc, argv);
}

void ofeliaWindow_set(t_ofeliaWindow *x, const t_symbol *s, const int argc, const t_atom *argv)
{
    ((ofeliaWindow*)x->x_ofeliaWindow)->_set(s, argc, argv);
}

void ofeliaWindow_print(t_ofeliaWindow *x)
{
    ((ofeliaWindow*)x->x_ofeliaWindow)->_print();
}

void ofeliaWindow_free(t_ofeliaWindow *x)
{
    ((ofeliaWindow*)x->x_ofeliaWindow)->_free();
    delete ((ofeliaWindow*)x->x_ofeliaWindow);
}

void ofeliaWindow_setup()
{
    t_ofeliaWindow::firstLoopSym = gensym("{ofFirstLoop}");
    t_ofeliaWindow::firstLoopMess = t_ofeliaWindow::firstLoopSym;
    t_ofeliaWindow::initSym = gensym("{ofInit}");
    t_ofeliaWindow::initMess = t_ofeliaWindow::initSym;
    t_ofeliaWindow::updateSym = gensym("{ofUpdate}");
    t_ofeliaWindow::updateMess = t_ofeliaWindow::updateSym;
    t_ofeliaWindow::drawSym = gensym("{ofDraw}");
    t_ofeliaWindow::drawMess = t_ofeliaWindow::drawSym;
    t_ofeliaWindow::exitSym = gensym("{ofExit}");
    t_ofeliaWindow::exitMess = t_ofeliaWindow::exitSym;
    ofeliaWindow_class = class_new(gensym("ofWindow"),
                                   reinterpret_cast<t_newmethod>(ofeliaWindow_new),
                                   reinterpret_cast<t_method>(ofeliaWindow_free), sizeof(t_ofeliaWindow), 0,
                                   A_GIMME, 0);
    class_addmethod(ofeliaWindow_class, reinterpret_cast<t_method>(ofeliaWindow_firstLoop),
                    t_ofeliaWindow::firstLoopMess, A_NULL, 0);
    class_addmethod(ofeliaWindow_class, reinterpret_cast<t_method>(ofeliaWindow_create),
                    gensym("create"), A_NULL, 0);
    class_addmethod(ofeliaWindow_class, reinterpret_cast<t_method>(ofeliaWindow_destroy),
                    gensym("destroy"), A_NULL, 0);
    class_addmethod(ofeliaWindow_class, reinterpret_cast<t_method>(ofeliaWindow_reset),
                    gensym("reset"), A_NULL, 0);
    class_addmethod(ofeliaWindow_class, reinterpret_cast<t_method>(ofeliaWindow_dimen),
                    gensym("dimen"), A_GIMME, 0);
    class_addmethod(ofeliaWindow_class, reinterpret_cast<t_method>(ofeliaWindow_pos),
                    gensym("pos"), A_GIMME, 0);
    class_addmethod(ofeliaWindow_class, reinterpret_cast<t_method>(ofeliaWindow_scale),
                    gensym("scale"), A_FLOAT, 0);
    class_addmethod(ofeliaWindow_class, reinterpret_cast<t_method>(ofeliaWindow_bgColor),
                    gensym("bgColor"), A_GIMME, 0);
    class_addmethod(ofeliaWindow_class, reinterpret_cast<t_method>(ofeliaWindow_frameRate),
                    gensym("frameRate"), A_FLOAT, 0);
    class_addmethod(ofeliaWindow_class, reinterpret_cast<t_method>(ofeliaWindow_VSync),
                    gensym("VSync"), A_GIMME, 0);
    class_addmethod(ofeliaWindow_class, reinterpret_cast<t_method>(ofeliaWindow_depthTest),
                    gensym("depthTest"), A_GIMME, 0);
    class_addmethod(ofeliaWindow_class, reinterpret_cast<t_method>(ofeliaWindow_orien),
                    gensym("orien"), A_GIMME, 0);
    class_addmethod(ofeliaWindow_class, reinterpret_cast<t_method>(ofeliaWindow_orienLock),
                    gensym("orienLock"), A_GIMME, 0);
    class_addmethod(ofeliaWindow_class, reinterpret_cast<t_method>(ofeliaWindow_accel),
                    gensym("accel"), A_GIMME, 0);
    class_addmethod(ofeliaWindow_class, reinterpret_cast<t_method>(ofeliaWindow_accelSmoothing),
                    gensym("accelSmoothing"), A_FLOAT, 0);
    class_addmethod(ofeliaWindow_class, reinterpret_cast<t_method>(ofeliaWindow_cursor),
                    gensym("cursor"), A_GIMME, 0);
    class_addmethod(ofeliaWindow_class, reinterpret_cast<t_method>(ofeliaWindow_fullscreen),
                    gensym("fullscreen"), A_GIMME, 0);
    class_addmethod(ofeliaWindow_class, reinterpret_cast<t_method>(ofeliaWindow_hotkeys),
                    gensym("hotkeys"), A_GIMME, 0);
    class_addmethod(ofeliaWindow_class, reinterpret_cast<t_method>(ofeliaWindow_title),
                    gensym("title"), A_GIMME, 0);
    class_addmethod(ofeliaWindow_class, reinterpret_cast<t_method>(ofeliaWindow_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaWindow_class, reinterpret_cast<t_method>(ofeliaWindow_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * class methods
 */
bool ofeliaWindow::_new(t_symbol *s, int argc, t_atom *argv)
{
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getWindowElemFromArgs(argc, argv, argWindowWidth, argWindowHeight,
                               argWindowPosX, argWindowPosY, argWindowScale, argBackgroundColor))
        return 0;
    initVariables();
    bWindowOwner = false;
    myOrien.bWindowOwner = false;
    
    if (!bWindowExists)
        createWindow();
    
    /* bind getter symbols if the object is created */
    bindGettersValueSymbols();
    outlet_new(&x->x_obj, &s_bang);
    return 1;
}

void ofeliaWindow::_firstLoop() //this will be called once after the window is created and before update & draw
{
    /* set getter values */
    value_setfloat(t_ofeliaGetFrameNum::getFrameNumSym, static_cast<t_float>(0.0f));
    value_setfloat(t_ofeliaGetTargetFrameRate::getTargetFrameRateSym, static_cast<t_float>(frameRate));
    value_setfloat(t_ofeliaGetOrienLock::getOrienLockSym, static_cast<t_float>(lockMode));
    value_setfloat(t_ofeliaGetScreenWidth::getScreenWidthSym, static_cast<t_float>(screenWidth));
    value_setfloat(t_ofeliaGetScreenHeight::getScreenHeightSym, static_cast<t_float>(screenHeight));
    value_setfloat(t_ofeliaGetRetina::getRetinaSym, static_cast<t_float>(retinaScale));
    value_setfloat(t_ofeliaGetBgColorR::getBgColorRSym, static_cast<t_float>(backgroundColor.r));
    value_setfloat(t_ofeliaGetBgColorG::getBgColorGSym, static_cast<t_float>(backgroundColor.g));
    value_setfloat(t_ofeliaGetBgColorB::getBgColorBSym, static_cast<t_float>(backgroundColor.b));
    
    /* this is called when the object receives fullscreen message before the first loop */
    if (bFullscreenScheduled) {
        
        setFullscreenMode(true);
        bFullscreenScheduled = false;
    }
    /* send message to listeners */
    sendPosToPd(GLFWwin, initWindowPosX, initWindowPosY);
    sendScaleToPd(initWindowScale);
    myOrien.sendDimenToPd(myOrien.deviceWidth, myOrien.deviceHeight);
    myOrien.sendOrienToPd(initOrientation);
    sendFullscreenToPd(bFullscreenMode);
    sendInitToPd(); //no need to call this when running as a standalone app
    fullscreenMaskPath = make_unique<ofPath>();
    fullscreenMaskMesh = make_unique<ofVboMesh>();
    bFullscreenMaskPathSet = false;
    
    /* handle the rest */
    ofDisableArbTex(); //OFELIANOTE : default is changed for arbTex.
    ofResetElapsedTimeCounter();
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::firstLoopSym);
}

void ofeliaWindow::_create()
{
    if (!bWindowExists)
        createWindow();
    else
        error("%s: the window has already been created", t_ofeliaWindow::objName);
}

void ofeliaWindow::_destroy()
{
    if (bWindowExists)
        destroyWindow();
    else
        error("%s: the window has already been destroyed", t_ofeliaWindow::objName);
}

void ofeliaWindow::_reset()
{
    if (bWindowOwner && bFullscreenMode)
        setFullscreenMode(false);
    setDeviceOrientation(WINDOW_ORIENTATION_DEFAULT);
    initVariables();
    
    if (bWindowOwner)
        setWindowProperties();
}

void ofeliaWindow::_dimen(const t_symbol *s, const int argc, const t_atom *argv)
{
    if (argc == 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            if (bFullscreenMode) {
             
                error("%s: cannot change dimen in fullscreen mode", t_ofeliaWindow::objName);
                return;
            }
            int w = static_cast<int>(argv[0].a_w.w_float);
            int h = static_cast<int>(argv[1].a_w.w_float);
            
            if (orientation%2)
                swap(w, h);
            
            if(setDeviceDimensions(w, h))
                bWindowShouldResize = true;
        }
        else {
            
          error("%s: wrong argument type", t_ofeliaWindow::objName);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaWindow::objName);
    }
}

void ofeliaWindow::_pos(const t_symbol *s, const int argc, const t_atom *argv)
{
    if (argc == 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            if (bFullscreenMode) {
                
                error("%s: cannot change pos in fullscreen mode", t_ofeliaWindow::objName);
                return;
            }
            int posX = static_cast<int>(argv[0].a_w.w_float);
            int posY = static_cast<int>(argv[1].a_w.w_float);
            
            if (bWindowOwner) {
                
                setWindowPosition(posX, posY);
            }
            else {
                
                initWindowPosX = posX;
                initWindowPosY = posY;
            }
        }
        else if (argv[0].a_type == A_SYMBOL &&
                 argv[1].a_type == A_SYMBOL) {
            
            if (bFullscreenMode) {
                
                error("%s: cannot change pos in fullscreen mode", t_ofeliaWindow::objName);
                return;
            }
            int posX, posY;
            
            if (!getWindowPosFromAlignModes(argv[0].a_w.w_symbol->s_name,
                                            argv[1].a_w.w_symbol->s_name,
                                            posX,
                                            posY))
                return;
        
            if (bWindowOwner) {
                
                setWindowPosition(posX, posY);
            }
            else {
                
                initWindowPosX = posX;
                initWindowPosY = posY;
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaWindow::objName);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaWindow::objName);
    }
}

void ofeliaWindow::_scale(const t_floatarg f)
{
    if (bFullscreenMode) {
        
        error("%s: cannot change scale in fullscreen mode", t_ofeliaWindow::objName);
        return;
    }
    if (setWindowScale(f))
        bWindowShouldResize = true;
}

void ofeliaWindow::_bgColor(const t_symbol *s, const int argc, const t_atom *argv)
{
    if (!getColorFromArgs(argc, argv, backgroundColor, t_ofeliaWindow::objName))
        return;
    
    if (bWindowOwner) {
        
        ofGetCurrentRenderer()->setBackgroundColor(backgroundColor);
        value_setfloat(t_ofeliaGetBgColorR::getBgColorRSym,
                       static_cast<t_float>(backgroundColor.r));
        value_setfloat(t_ofeliaGetBgColorG::getBgColorGSym,
                       static_cast<t_float>(backgroundColor.g));
        value_setfloat(t_ofeliaGetBgColorB::getBgColorBSym,
                       static_cast<t_float>(backgroundColor.b));
    }
}

void ofeliaWindow::_frameRate(const t_floatarg f)
{
    setFrameRate(static_cast<int>(f));
}

void ofeliaWindow::_VSync(const t_symbol *s, const int argc, const t_atom *argv)
{
    if (!getToggleFromArgs(argc, argv, bVSyncEnabled, t_ofeliaWindow::objName))
        return;
    
    if (bWindowOwner)
        ofSetVerticalSync(bVSyncEnabled);
}

void ofeliaWindow::_depthTest(const t_symbol *s, const int argc, const t_atom *argv)
{
    bool depthTest;
    
    if (!getToggleFromArgs(argc, argv, depthTest, t_ofeliaWindow::objName))
        return;
    
    if (bWindowOwner) {
        
        bDepthTestEnabled = depthTest;
        ofSetDepthTest(bDepthTestEnabled);
    }
}

void ofeliaWindow::_orien(const t_symbol *s, const int argc, const t_atom *argv)
{
    if (argc == 1) {
        
        if (bFullscreenMode) {
            
            error("%s: cannot change orien in fullscreen mode", t_ofeliaWindow::objName);
            return;
        }
        
        if (argv[0].a_type == A_FLOAT) {
            
            setDeviceOrientation(static_cast<int>(argv[0].a_w.w_float));
        }
        else if (argv[0].a_type == A_SYMBOL) {
            
            const char *mode = argv[0].a_w.w_symbol->s_name;
            
            if (!strcmp(mode, "DEFAULT"))
                setDeviceOrientation(0);
            else if (!strcmp(mode, "RIGHT"))
                setDeviceOrientation(1);
            else if (!strcmp(mode, "INVERT"))
                setDeviceOrientation(2);
            else if (!strcmp(mode, "LEFT"))
                setDeviceOrientation(3);
            else
                error("%s: no method for '%s'", t_ofeliaWindow::objName, mode);
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaWindow::objName);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaWindow::objName);
    }
}

void ofeliaWindow::_orienLock(const t_symbol *s, const int argc, const t_atom *argv)
{
    if (argc == 1) {
        
        if (bFullscreenMode) {
            
            error("%s: cannot change orienLock in fullscreen mode", t_ofeliaWindow::objName);
            return;
        }
        
        if (argv[0].a_type == A_FLOAT) {
            
            setDeviceRotationLockMode(static_cast<int>(argv[0].a_w.w_float), orientation);
        }
        else if (argv[0].a_type == A_SYMBOL) {
            
            const char *mode = argv[0].a_w.w_symbol->s_name;
            
            if (!strcmp(mode, "FREE"))
                setDeviceRotationLockMode(0, orientation);
            else if (!strcmp(mode, "SOFT"))
                setDeviceRotationLockMode(1, orientation);
            else if (!strcmp(mode, "HARD"))
                setDeviceRotationLockMode(2, orientation);
            else
                error("%s: no method for '%s'", t_ofeliaWindow::objName, mode);
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaWindow::objName);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaWindow::objName);
    }
}

void ofeliaWindow::_accel(const t_symbol *s, const int argc, const t_atom *argv)
{
    if (!getToggleFromArgs(argc, argv, bInitAccelEnabled, t_ofeliaWindow::objName))
        return;
    
    if (bWindowOwner)
        bAccelEnabled = bInitAccelEnabled;
}

void ofeliaWindow::_accelSmoothing(const t_floatarg f)
{
    if (f >= 0.0f && f <= 1.0f)
        accelSmoothing = f;
    else
        error("%s: accelSmoothing out of range", t_ofeliaWindow::objName);
}

void ofeliaWindow::_cursor(const t_symbol *s, const int argc, const t_atom *argv)
{
    if (!getToggleFromArgs(argc, argv, bCursorMode, t_ofeliaWindow::objName))
        return;
    
    if (bWindowOwner)
        setCursorMode(bCursorMode);
}

void ofeliaWindow::_fullscreen(const t_symbol *s, const int argc, const t_atom *argv)
{
    bool fullscreen;
    
    if (!getToggleFromArgs(argc, argv, fullscreen, t_ofeliaWindow::objName))
        return;
    
    if (bWindowOwner) {
        
        if (!bFirstLoop) {
            
            if(fullscreen != bFullscreenMode)
                setFullscreenMode(fullscreen);
        }
        else {
            
            /* this is to prevent flickering issue when called before the first loop */
            bFullscreenScheduled = fullscreen;
        }
    }
    else {
        
        bFullscreenMode = fullscreen;
    }
}

void ofeliaWindow::_hotkeys(const t_symbol *s, const int argc, const t_atom *argv)
{
    getToggleFromArgs(argc, argv, bHotkeyEnabled, t_ofeliaWindow::objName);
}

void ofeliaWindow::_title(const t_symbol *s, const int argc, const t_atom *argv)
{
    stringstream ss;
    const char space = ' ';
    
    for (int i=0; i<argc; ++i) {
        
        if (argv[i].a_type == A_FLOAT) {
            
            ss << argv[i].a_w.w_float;
        }
        else if (argv[i].a_type == A_SYMBOL) {
            
            ss << argv[i].a_w.w_symbol->s_name;
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaWindow::objName);
            return;
        }
        ss << space;
    }
    title = ss.str().c_str();
    title.pop_back();
    
    if (bWindowOwner)
        ofSetWindowTitle(title.c_str());
}

void ofeliaWindow::_set(const t_symbol *s, const int argc, const t_atom *argv)
{
    int width, height, posX, posY;
    float scale;
    ofColor bgColor;
    if (getWindowElemFromArgs(argc, argv, width, height, posX, posY, scale, bgColor)) {
        
        /* dimen */
        t_atom dimenArgv[2];
        dimenArgv[0].a_type = A_FLOAT;
        dimenArgv[0].a_w.w_float = static_cast<t_float>(width);
        dimenArgv[1].a_type = A_FLOAT;
        dimenArgv[1].a_w.w_float = static_cast<t_float>(height);
        _dimen(0, 2, dimenArgv);
        
        /* scale */
        _scale(scale);
        
        /* pos */
        t_atom posArgv[2];
        posArgv[0].a_type = A_FLOAT;
        posArgv[0].a_w.w_float = static_cast<t_float>(posX);
        posArgv[1].a_type = A_FLOAT;
        posArgv[1].a_w.w_float = static_cast<t_float>(posY);
        _pos(0, 2, posArgv);
        
        /* bgColor */
        t_atom bgColorArgv[3];
        bgColorArgv[0].a_type = A_FLOAT;
        bgColorArgv[0].a_w.w_float = static_cast<t_float>(bgColor.r);
        bgColorArgv[1].a_type = A_FLOAT;
        bgColorArgv[1].a_w.w_float = static_cast<t_float>(bgColor.g);
        bgColorArgv[2].a_type = A_FLOAT;
        bgColorArgv[2].a_w.w_float = static_cast<t_float>(bgColor.b);
        _bgColor(0, 3, bgColorArgv);
    }
}

void ofeliaWindow::_print()
{
    post("\n[%s]", t_ofeliaWindow::objName);
    
    if (orientation%2 && lockMode == WINDOW_ROTATION_LOCK_FREE)
        post("dimen : %d %d", initWindowHeight, initWindowWidth);
    else
        post("dimen : %d %d", initWindowWidth, initWindowHeight);
    
#if defined(TARGET_WIN32)
    const int titleBarHeight = 30;
#else
    const int titleBarHeight = 0;
#endif
    if (bWindowOwner)
        post("pos : %d %d", windowPosX, windowPosY - titleBarHeight);
    else
        post("pos : %d %d", initWindowPosX, initWindowPosY - titleBarHeight);
    
    if (bFullscreenMode)
        post("scale : %g", windowScale);
    else
        post("scale : %g", initWindowScale);
    post("bgColor : %d %d %d", backgroundColor.r, backgroundColor.g, backgroundColor.b);
    post("frameRate : %d", frameRate);
    printToggle("VSync", bVSyncEnabled);
    printToggle("depthTest", bDepthTestEnabled);
    
    switch (initOrientation) {
            
        case 0:
            post("orien : DEFAULT");
            break;
        case 1:
            post("orien : RIGHT");
            break;
        case 2:
            post("orien : INVERT");
            break;
        case 3:
            post("orien : LEFT");
            break;
        default:
            break;
    }
    switch (lockMode) {
            
        case 0:
            post("orienLock : FREE");
            break;
        case 1:
            post("orienLock : SOFT");
            break;
        case 2:
            post("orienLock : HARD");
            break;
        default:
            break;
    }
    printToggle("accel", bInitAccelEnabled);
    post("accelSmoothing : %g", accelSmoothing);
    printToggle("cursor", bCursorMode);
    printToggle("fullscreen", bFullscreenMode);
    printToggle("hotkeys", bHotkeyEnabled);
    post("title : %s", title.c_str());
}

void ofeliaWindow::_free()
{
    /* if the removed object is the one that created the window */
    if (bWindowOwner && bWindowExists)
        destroyWindow();
    
    /* unbind getter symbols if the object is deleted */
    unbindGettersValueSymbols();
}

/* ________________________________________________________________________________
 * class methods
 */
bool ofeliaWindow::getWindowPosFromAlignModes(const char *horMode, const char *verMode, int &posX, int &posY)
{
    if (!strcmp(horMode, "LEFT"))
        posX = 0;
    else if (!strcmp(horMode, "CENTER"))
        posX = -1;
    else if (!strcmp(horMode, "RIGHT"))
        posX = OFELIA_MAX_SCREENSIZE+1;
    else {
        
        error("%s: no method for '%s'", t_ofeliaWindow::objName, horMode);
        return 0;
    }
    if (!strcmp(verMode, "TOP"))
        posY = 0;
    else if (!strcmp(verMode, "MIDDLE"))
        posY = -1;
    else if (!strcmp(verMode, "BOTTOM"))
        posY = OFELIA_MAX_SCREENSIZE+1;
    else {
        
        error("%s: no method for '%s'", t_ofeliaWindow::objName, verMode);
        return 0;
    }
    return 1;
}

bool ofeliaWindow::getWindowElemFromArgs(const int argc, const t_atom *argv, int &width, int &height,
                                      int &posX, int &posY, float &scale, ofColor &bgColor)
{
    /* initialize variables from arguments */
    switch (argc) {
            
        case 0:
        {
            width = OFELIA_DEFAULT_WINDOWWIDTH;
            height = OFELIA_DEFAULT_WINDOWHEIGHT;
            posX = posY = 0;
            scale = 1.0f;
            bgColor.set(OFELIA_DEFAULT_BGCOLOR);
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT) {
                
                width = static_cast<int>(argv[0].a_w.w_float);
                height = static_cast<int>(argv[1].a_w.w_float);
                posX = posY = 0;
                scale = 1.0f;
                bgColor.set(OFELIA_DEFAULT_BGCOLOR);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaWindow::objName);
                return 0;
            }
            break;
        }
        case 4:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT) {
                
                width = static_cast<int>(argv[0].a_w.w_float);
                height = static_cast<int>(argv[1].a_w.w_float);
                posX = static_cast<int>(argv[2].a_w.w_float);
                posY = static_cast<int>(argv[3].a_w.w_float);
                scale = 1.0f;
                bgColor.set(OFELIA_DEFAULT_BGCOLOR);
            }
            else if (argv[0].a_type == A_FLOAT &&
                     argv[1].a_type == A_FLOAT &&
                     argv[2].a_type == A_SYMBOL &&
                     argv[3].a_type == A_SYMBOL) {
                
                width = static_cast<int>(argv[0].a_w.w_float);
                height = static_cast<int>(argv[1].a_w.w_float);
                
                if (!getWindowPosFromAlignModes(argv[2].a_w.w_symbol->s_name,
                                                argv[3].a_w.w_symbol->s_name,
                                                posX,
                                                posY))
                    return 0;
                scale = 1.0f;
                bgColor.set(OFELIA_DEFAULT_BGCOLOR);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaWindow::objName);
                return 0;
            }
            break;
        }
        case 5:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT &&
                argv[4].a_type == A_FLOAT) {
                
                width = static_cast<int>(argv[0].a_w.w_float);
                height = static_cast<int>(argv[1].a_w.w_float);
                posX = static_cast<int>(argv[2].a_w.w_float);
                posY = static_cast<int>(argv[3].a_w.w_float);
                scale = argv[4].a_w.w_float;
                bgColor.set(OFELIA_DEFAULT_BGCOLOR);
            }
            else if (argv[0].a_type == A_FLOAT &&
                     argv[1].a_type == A_FLOAT &&
                     argv[2].a_type == A_SYMBOL &&
                     argv[3].a_type == A_SYMBOL &&
                     argv[4].a_type == A_FLOAT) {
                
                width = static_cast<int>(argv[0].a_w.w_float);
                height = static_cast<int>(argv[1].a_w.w_float);
                
                if (!getWindowPosFromAlignModes(argv[2].a_w.w_symbol->s_name,
                                                argv[3].a_w.w_symbol->s_name,
                                                posX,
                                                posY))
                    return 0;
                scale = argv[4].a_w.w_float;
                bgColor.set(OFELIA_DEFAULT_BGCOLOR);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaWindow::objName);
                return 0;
            }
            break;
        }
        case 6:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT &&
                argv[4].a_type == A_FLOAT &&
                argv[5].a_type == A_FLOAT) {
                
                width = static_cast<int>(argv[0].a_w.w_float);
                height = static_cast<int>(argv[1].a_w.w_float);
                posX = static_cast<int>(argv[2].a_w.w_float);
                posY = static_cast<int>(argv[3].a_w.w_float);
                scale = argv[4].a_w.w_float;
                bgColor.set(ofColor(static_cast<unsigned char>(argv[5].a_w.w_float)));
            }
            else if (argv[0].a_type == A_FLOAT &&
                     argv[1].a_type == A_FLOAT &&
                     argv[2].a_type == A_FLOAT &&
                     argv[3].a_type == A_FLOAT &&
                     argv[4].a_type == A_FLOAT &&
                     argv[5].a_type == A_SYMBOL) {
                
                width = static_cast<int>(argv[0].a_w.w_float);
                height = static_cast<int>(argv[1].a_w.w_float);
                posX = static_cast<int>(argv[2].a_w.w_float);
                posY = static_cast<int>(argv[3].a_w.w_float);
                scale = argv[4].a_w.w_float;
                const char *colorName = argv[5].a_w.w_symbol->s_name;
                
                if (!t_ofeliaSetColor::getColorByName(colorName, bgColor))
                    return 0;
            }
            else if (argv[0].a_type == A_FLOAT &&
                     argv[1].a_type == A_FLOAT &&
                     argv[2].a_type == A_SYMBOL &&
                     argv[3].a_type == A_SYMBOL &&
                     argv[4].a_type == A_FLOAT &&
                     argv[5].a_type == A_FLOAT) {
                
                width = static_cast<int>(argv[0].a_w.w_float);
                height = static_cast<int>(argv[1].a_w.w_float);
                
                if (!getWindowPosFromAlignModes(argv[2].a_w.w_symbol->s_name,
                                                argv[3].a_w.w_symbol->s_name,
                                                posX,
                                                posY))
                    return 0;
                scale = argv[4].a_w.w_float;
                bgColor.set(ofColor(static_cast<unsigned char>(argv[5].a_w.w_float)));
            }
            else if (argv[0].a_type == A_FLOAT &&
                     argv[1].a_type == A_FLOAT &&
                     argv[2].a_type == A_SYMBOL &&
                     argv[3].a_type == A_SYMBOL &&
                     argv[4].a_type == A_FLOAT &&
                     argv[5].a_type == A_SYMBOL) {
                
                width = static_cast<int>(argv[0].a_w.w_float);
                height = static_cast<int>(argv[1].a_w.w_float);
                
                if (!getWindowPosFromAlignModes(argv[2].a_w.w_symbol->s_name,
                                                argv[3].a_w.w_symbol->s_name,
                                                posX,
                                                posY))
                    return 0;
                scale = argv[4].a_w.w_float;
                const char *colorName = argv[5].a_w.w_symbol->s_name;
                
                if (!t_ofeliaSetColor::getColorByName(colorName, bgColor))
                    return 0;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaWindow::objName);
                return 0;
            }
            break;
        }
        case 8:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT &&
                argv[4].a_type == A_FLOAT &&
                argv[5].a_type == A_FLOAT &&
                argv[6].a_type == A_FLOAT &&
                argv[7].a_type == A_FLOAT) {
                
                width = static_cast<int>(argv[0].a_w.w_float);
                height = static_cast<int>(argv[1].a_w.w_float);
                posX = static_cast<int>(argv[2].a_w.w_float);
                posY = static_cast<int>(argv[3].a_w.w_float);
                scale = argv[4].a_w.w_float;
                bgColor.set(ofColor(static_cast<unsigned char>(argv[5].a_w.w_float),
                                    static_cast<unsigned char>(argv[6].a_w.w_float),
                                    static_cast<unsigned char>(argv[7].a_w.w_float)));
            }
            else if (argv[0].a_type == A_FLOAT &&
                     argv[1].a_type == A_FLOAT &&
                     argv[2].a_type == A_SYMBOL &&
                     argv[3].a_type == A_SYMBOL &&
                     argv[4].a_type == A_FLOAT &&
                     argv[5].a_type == A_FLOAT &&
                     argv[6].a_type == A_FLOAT &&
                     argv[7].a_type == A_FLOAT) {
                
                width = static_cast<int>(argv[0].a_w.w_float);
                height = static_cast<int>(argv[1].a_w.w_float);
                
                if (!getWindowPosFromAlignModes(argv[2].a_w.w_symbol->s_name,
                                                argv[3].a_w.w_symbol->s_name,
                                                posX,
                                                posY))
                    return 0;
                scale = argv[4].a_w.w_float;
                bgColor.set(ofColor(static_cast<unsigned char>(argv[5].a_w.w_float),
                                    static_cast<unsigned char>(argv[6].a_w.w_float),
                                    static_cast<unsigned char>(argv[7].a_w.w_float)));
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaWindow::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaWindow::objName);
            return 0;
        }
    }
    if (width <= 0 || width > OFELIA_MAX_SCREENSIZE ||
        height <= 0 || height > OFELIA_MAX_SCREENSIZE) {
        
        error("%s: dimen out of range", t_ofeliaWindow::objName);
        return 0;
    }
    if (scale <= 0.0f || scale > OFELIA_MAX_WINDOWSCALE) {
        
        error("%s: scale out of range", t_ofeliaWindow::objName);
        return 0;
    }
    return 1;
}

void ofeliaWindow::initVariables()
{
    /* initialize variables by the default settings */
    initWindowWidth = argWindowWidth;
    initWindowHeight = argWindowHeight;
    initWindowPosX = argWindowPosX;
    initWindowPosY = argWindowPosY;
    initWindowScale = argWindowScale;
    backgroundColor.set(argBackgroundColor);
    frameRate = 60;
    bVSyncEnabled = true;
    bDepthTestEnabled = false;
    initOrientation = WINDOW_ORIENTATION_DEFAULT;
    lockMode = WINDOW_ROTATION_LOCK_FREE;
    bInitAccelEnabled = false;
    accelSmoothing = 0.0f;
    initAccelForce.set(0.0f, 0.0f, -1.0f);
    bCursorMode = true;
    bFullscreenMode = false;
    bHotkeyEnabled = true;
    title = "";
}

void ofeliaWindow::setWindowProperties()
{
    bAccelEnabled = bInitAccelEnabled;
    accelForce = initAccelForce;
    setDeviceDimensions(initWindowWidth,
                        initWindowHeight);
    setWindowScale(initWindowScale);
    setWindowPosition(initWindowPosX, initWindowPosY);
    setFrameRate(frameRate);
    setDeviceRotationLockMode(lockMode, initOrientation);
    setDeviceOrientation(initOrientation);
    setCursorMode(bCursorMode);
    ofSetVerticalSync(bVSyncEnabled);
    ofSetDepthTest(bDepthTestEnabled);
    ofSetBackgroundColor(backgroundColor);
    ofSetWindowTitle(title.c_str());
    resizeWindow();
}

void ofeliaWindow::createWindow()
{
    ofGLFWWindowSettings settings;
    settings.setGLVersion(2, 1);
    settings.width = 1;
    settings.height = 1;
    ofCreateWindow(settings);
    window = ofGetWindowPtr();
    ofAppGLFWWindow *ofAppGLFWwin = static_cast<ofAppGLFWWindow*>(window);
    retinaScale = ofAppGLFWwin->getPixelScreenCoordScale();
    GLFWwin = ofAppGLFWwin->getGLFWWindow();
    ofGetMainLoop()->setEscapeQuitsLoop(false);
    bFirstLoop = true;
    bFirstUpdate = true;
    bWindowOwner = true;
    myOrien.bWindowOwner = true;
    ofSetLogLevel(OF_LOG_SILENT);
    
    /* get non-retina screen dimensions */
    screenWidth = ofGetScreenWidth() / retinaScale;
    screenHeight = ofGetScreenHeight() / retinaScale;
#if defined(TARGET_WIN32)
    const int menuBarHeight = 0;
#else
    const int menuBarHeight = 25;
#endif
    
    /* reduce scale while the window is larger than the screen size */
    if (initWindowWidth*initWindowScale > screenWidth ||
        initWindowHeight*initWindowScale > screenHeight - menuBarHeight * retinaScale) {
        
        while (initWindowWidth*initWindowScale > screenWidth ||
               initWindowHeight*initWindowScale > screenHeight - menuBarHeight * retinaScale)
            initWindowScale-=0.001f;
        
        argWindowScale = initWindowScale;
        error("%s: scale has been changed to %g", t_ofeliaWindow::objName, argWindowScale);
    }
    /* create a blank cursor */
    if (!blankCursor) {
        
        unsigned char blankPixels[4];
        memset(blankPixels, 0x00, sizeof(blankPixels));
        GLFWimage blankImage;
        blankImage.width = 1;
        blankImage.height = 1;
        blankImage.pixels = blankPixels;
        blankCursor = glfwCreateCursor(&blankImage, 0, 0);
    }
    setWindowProperties();
    addWindowListeners();
    glfwSetWindowFocusCallback(GLFWwin, sendFocusToPd);
    glfwSetWindowPosCallback(GLFWwin, sendPosToPd);
    
    if (bFullscreenMode)
        setFullscreenMode(true);
    mouseX = mouseY = OFELIA_MAX_SCREENSIZE;
    touchState = WINDOW_TOUCH_UP;
    touchID = -1;
    bHoldingFnKey = false;
    bAccelSimEnabled = false;
    bMultiTouchSimEnabled = false;
    bMultiTouchSimScheduled = false;
    bWindowExists = true;
    bFullscreenScheduled = false;
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::firstLoopSym);
    pollEventsClock = clock_new(this, reinterpret_cast<t_method>(pollEventsMethod));
    
    /* start events polling */
    clock_delay(pollEventsClock, 0.0);
}

void ofeliaWindow::destroyWindow()
{
    window->setWindowShouldClose();
    
    if (bFirstLoop)
        pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::firstLoopSym);
}

void ofeliaWindow::bindGettersValueSymbols()
{
    value_get(t_ofeliaGetWidth::getWidthSym);
    value_get(t_ofeliaGetHeight::getHeightSym);
    value_get(t_ofeliaGetScale::getScaleSym);
    value_get(t_ofeliaGetFrameNum::getFrameNumSym);
    value_get(t_ofeliaGetFrameRate::getFrameRateSym);
    value_get(t_ofeliaGetTargetFrameRate::getTargetFrameRateSym);
    value_get(t_ofeliaGetElapsedTime::getElapsedTimeSym);
    value_get(t_ofeliaGetLastFrameTime::getLastFrameTimeSym);
    value_get(t_ofeliaGetOrienLock::getOrienLockSym);
    value_get(t_ofeliaGetOrien::getOrienSym);
    value_get(t_ofeliaGetFullscreen::getFullscreenSym);
    value_get(t_ofeliaGetFocus::getFocusSym);
    value_get(t_ofeliaGetPosX::getPosXSym);
    value_get(t_ofeliaGetPosY::getPosYSym);
    value_get(t_ofeliaGetScreenWidth::getScreenWidthSym);
    value_get(t_ofeliaGetScreenHeight::getScreenHeightSym);
    value_get(t_ofeliaGetRetina::getRetinaSym);
    value_get(t_ofeliaGetBgColorR::getBgColorRSym);
    value_get(t_ofeliaGetBgColorG::getBgColorGSym);
    value_get(t_ofeliaGetBgColorB::getBgColorBSym);
    value_get(t_ofeliaGetWindow::getWindowSym);
}

void ofeliaWindow::unbindGettersValueSymbols()
{
    value_release(t_ofeliaGetWidth::getWidthSym);
    value_release(t_ofeliaGetHeight::getHeightSym);
    value_release(t_ofeliaGetScale::getScaleSym);
    value_release(t_ofeliaGetFrameNum::getFrameNumSym);
    value_release(t_ofeliaGetFrameRate::getFrameRateSym);
    value_release(t_ofeliaGetTargetFrameRate::getTargetFrameRateSym);
    value_release(t_ofeliaGetElapsedTime::getElapsedTimeSym);
    value_release(t_ofeliaGetLastFrameTime::getLastFrameTimeSym);
    value_release(t_ofeliaGetOrienLock::getOrienLockSym);
    value_release(t_ofeliaGetOrien::getOrienSym);
    value_release(t_ofeliaGetFullscreen::getFullscreenSym);
    value_release(t_ofeliaGetFocus::getFocusSym);
    value_release(t_ofeliaGetPosX::getPosXSym);
    value_release(t_ofeliaGetPosY::getPosYSym);
    value_release(t_ofeliaGetScreenWidth::getScreenWidthSym);
    value_release(t_ofeliaGetScreenHeight::getScreenHeightSym);
    value_release(t_ofeliaGetRetina::getRetinaSym);
    value_release(t_ofeliaGetBgColorR::getBgColorRSym);
    value_release(t_ofeliaGetBgColorG::getBgColorGSym);
    value_release(t_ofeliaGetBgColorB::getBgColorBSym);
    value_release(t_ofeliaGetWindow::getWindowSym);
}

bool ofeliaWindow::setDeviceDimensions(const int dWidth, const int dHeight)
{
    if (dWidth > 0 && dWidth <= OFELIA_MAX_SCREENSIZE &&
        dHeight > 0 && dHeight <= OFELIA_MAX_SCREENSIZE) {
    
        myOrien.deviceWidth = initWindowWidth = dWidth;
        myOrien.deviceHeight = initWindowHeight = dHeight;
        
        if (bWindowOwner) {
            
            windowWidth = initWindowWidth;
            windowHeight = initWindowHeight;
        }
        if (initOrientation != WINDOW_ORIENTATION_DEFAULT && lockMode != WINDOW_ROTATION_LOCK_FREE) {
            
            if (initOrientation%2)
                swap(myOrien.deviceWidth, myOrien.deviceHeight);
            myOrien.blockSendingOrienToPd = true;
            int o = WINDOW_ORIENTATION_DEFAULT;
            myOrien.setDeviceOrientation(o);
            setDeviceOrientation(orientation);
            myOrien.blockSendingOrienToPd = false;
        }
        if (!bFirstLoop)
            myOrien.sendDimenToPd(myOrien.deviceWidth, myOrien.deviceHeight);
        
        if (bWindowOwner)
            return 1;
        else
            return 0;
    }
    else {
        
        error("%s: dimen out of range", t_ofeliaWindow::objName);
        return 0;
    }
}

bool ofeliaWindow::setWindowScale(const float wScale)
{
    if (wScale > 0.0f && wScale <= OFELIA_MAX_WINDOWSCALE) {
        
        initWindowScale = wScale;
        
        if (bWindowOwner)
            windowScale = initWindowScale;
        else
            return 0;
    }
    else {
        
        error("%s: scale out of range", t_ofeliaWindow::objName);
        return 0;
    }
    return 1;
}

void ofeliaWindow::setWindowPosition(const int posX, const int posY)
{
    if (posX < 0)
        initWindowPosX = (screenWidth - initWindowWidth * initWindowScale) * 0.5f;
    else if (posX > screenWidth - initWindowWidth * initWindowScale)
        initWindowPosX = screenWidth - initWindowWidth * initWindowScale;
    else
        initWindowPosX = posX;
    
    if (posY < 0)
        initWindowPosY = (screenHeight - initWindowHeight * initWindowScale) * 0.5f;
    else if (posY > screenHeight - initWindowHeight * initWindowScale)
        initWindowPosY = screenHeight - initWindowHeight * initWindowScale;
    else
        initWindowPosY = posY;
#if defined(TARGET_WIN32)
    const int titleBarHeight = 30;
#else
    const int titleBarHeight = 0;
#endif
    glfwSetWindowPos(GLFWwin, initWindowPosX, initWindowPosY + titleBarHeight);
}

void ofeliaWindow::setFrameRate(const int fRate)
{
    if (fRate >= 20 && fRate <= 300) {
        
        frameRate = fRate;
        
        if (bWindowOwner) {
            
            ofSetFrameRate(frameRate);
            value_setfloat(t_ofeliaGetTargetFrameRate::getTargetFrameRateSym,
                           static_cast<t_float>(frameRate));
        }
    }
    else {
        
        error("%s: frameRate out of range", t_ofeliaWindow::objName);
    }
}

void ofeliaWindow::setDeviceRotationLockMode(const int newLockMode,
                                          int &currentOrientation)
{
    if (newLockMode >= 0 && newLockMode <= 2) {
        
        myOrien.setDeviceRotationLockMode(newLockMode, currentOrientation);
        lockMode = newLockMode;
        initOrientation = currentOrientation;
        
        if (bWindowOwner)
            value_setfloat(t_ofeliaGetOrienLock::getOrienLockSym,
                           static_cast<t_float>(lockMode));
    }
    else {
        
        error("%s: orienLock out of range", t_ofeliaWindow::objName);
    }
}

void ofeliaWindow::setDeviceOrientation(int newOrientation)
{
    if (newOrientation >= 0 && newOrientation <= 3) {
        
        myOrien.setDeviceOrientation(newOrientation);
        
        if (orientation%2 != newOrientation%2)
            swapWindowDimensions();
        orientation = newOrientation;
        initOrientation = orientation;
    }
    else {
        
        error("%s: orien out of range", t_ofeliaWindow::objName);
    }
}

void ofeliaWindow::setCursorMode(const bool cMode)
{
    if (cMode)
        glfwSetCursor(GLFWwin, NULL);
    else
        glfwSetCursor(GLFWwin, blankCursor);
}

void ofeliaWindow::setFullscreenMode(const bool fMode)
{
    if (fMode) {
        
        bWindowResizeLocked = true;
        const float stored = windowScale;
        ofSetFullscreen(true);
        windowScaleStored = stored;
    }
    else {
        
        ofSetFullscreen(false);
        ofSetWindowTitle(title.c_str());
        windowScale = windowScaleStored;
        sendScaleToPd(windowScale);
        sendFullscreenToPd(false);
        resizeWindow();
        bFullscreenMode = false;
    }
}

void ofeliaWindow::swapWindowDimensions()
{
    if (bWindowOwner) {
        
        /* relocate window to center */
        const int width = ofGetWidth();
        const int height = ofGetHeight();
        const int windowOffsetX = (width - height) * 0.5f;
        const int windowOffsetY = (height - width) * 0.5f;
        const int windowPosX = ofGetWindowPositionX() + windowOffsetX;
        const int windowPosY = ofGetWindowPositionY() + windowOffsetY;
        ofSetWindowPosition(windowPosX, windowPosY);
        
        /* swap window dimensions then resize */
        swap(windowWidth, windowHeight);
        initWindowWidth = windowWidth;
        initWindowHeight = windowHeight;
        resizeWindow();
    }
    else
        swap(initWindowWidth, initWindowHeight);
}

void ofeliaWindow::addWindowListeners()
{
    ofAddListener(window->events().update, this,
                  &ofeliaWindow::windowUpdate);
    ofAddListener(window->events().draw, this,
                  &ofeliaWindow::windowDraw);
    ofAddListener(window->events().mousePressed, this,
                  &ofeliaWindow::windowMousePressed);
    ofAddListener(window->events().mouseMoved, this,
                  &ofeliaWindow::windowMouseMoved);
    ofAddListener(window->events().mouseDragged, this,
                  &ofeliaWindow::windowMouseDragged);
    ofAddListener(window->events().mouseReleased, this,
                  &ofeliaWindow::windowMouseReleased);
    ofAddListener(window->events().mouseScrolled, this,
                  &ofeliaWindow::windowMouseScrolled);
    ofAddListener(window->events().keyPressed, this,
                  &ofeliaWindow::windowKeyPressed);
    ofAddListener(window->events().keyReleased, this,
                  &ofeliaWindow::windowKeyReleased);
    ofAddListener(window->events().windowResized, this,
                  &ofeliaWindow::windowResized);
    ofAddListener(window->events().exit, this,
                  &ofeliaWindow::windowDestroyed);
}

void ofeliaWindow::removeWindowListeners()
{
    ofRemoveListener(window->events().update, this,
                     &ofeliaWindow::windowUpdate);
    ofRemoveListener(window->events().draw, this,
                     &ofeliaWindow::windowDraw);
    ofRemoveListener(window->events().mousePressed, this,
                     &ofeliaWindow::windowMousePressed);
    ofRemoveListener(window->events().mouseMoved, this,
                     &ofeliaWindow::windowMouseMoved);
    ofRemoveListener(window->events().mouseDragged, this,
                     &ofeliaWindow::windowMouseDragged);
    ofRemoveListener(window->events().mouseReleased, this,
                     &ofeliaWindow::windowMouseReleased);
    ofRemoveListener(window->events().mouseScrolled, this,
                     &ofeliaWindow::windowMouseScrolled);
    ofRemoveListener(window->events().keyPressed, this,
                     &ofeliaWindow::windowKeyPressed);
    ofRemoveListener(window->events().keyReleased, this,
                     &ofeliaWindow::windowKeyReleased);
    ofRemoveListener(window->events().windowResized, this,
                     &ofeliaWindow::windowResized);
    ofRemoveListener(window->events().exit, this,
                     &ofeliaWindow::windowDestroyed);
}

/* ________________________________________________________________________________
 * event listeners
 */
void ofeliaWindow::windowUpdate(ofEventArgs &e)
{
    if (!bWindowExists || bFirstLoop)
        return;
    
    /* set default styles */
    ofGetCurrentRenderer()->setColor(255, 255, 255, 255);
    ofGetCurrentRenderer()->setRectMode(OF_RECTMODE_CORNER);
    t_ofeliaSetTextMode::textModeVec = {{TEXT_ALIGN_LEFT, TEXT_ALIGN_TOP}};
    ofGetCurrentRenderer()->setFillMode(OF_FILLED);
    ofGetCurrentRenderer()->setBlendMode(OF_BLENDMODE_ALPHA);
    ofGetCurrentRenderer()->setPolyMode(OF_POLY_WINDING_ODD);
    ofGetCurrentRenderer()->setLineWidth(scaleFactor);
    ofGetCurrentRenderer()->setLineSmoothing(false);
    ofGetCurrentRenderer()->setCircleResolution(OFELIA_DEFAULT_RESOLUTION);
    ofGetCurrentRenderer()->setCurveResolution(OFELIA_DEFAULT_RESOLUTION);
    ofGetCurrentRenderer()->setSphereResolution(OFELIA_DEFAULT_RESOLUTION);
    
    if (bAccelEnabled) {
        
        if (bAccelSimEnabled) {
            
            float rawX = (mouseX / static_cast<float>(myOrien.deviceWidth)) * 2.0f - 1.0f;
            float rawY = (1.0f - (mouseY / static_cast<float>(myOrien.deviceHeight))) * 2.0f - 1.0f;
            float rawZ = max(abs(rawX), abs(rawY));
            float accX = sinf(rawX * M_PI_2);
            float accY = sinf(rawY * M_PI_2);
            float accZ = cosf(rawZ * M_PI_2) * -1;
            const ofVec3f accelReal(accX, accY, accZ);
            
            if(accelSmoothing) {
                
                float lerpFactor = 1.0f - accelSmoothing;
                
                if (lerpFactor>1.0f) lerpFactor = 1.0f;
                else if (lerpFactor < 0.01f) lerpFactor = 0.01f;
                accelForce += (accelReal - accelForce) * lerpFactor;
            }
            else {
                
                accelForce.set(accX, accY, accZ);
            }
        }
        sendAccelToPd(accelForce.x, accelForce.y, accelForce.z);
    }
    myOrien.updateRotation();
    
    /* values that need to be updated before each frame */
    value_setfloat(t_ofeliaGetFrameNum::getFrameNumSym, static_cast<t_float>(ofGetFrameNum()));
    value_setfloat(t_ofeliaGetFrameRate::getFrameRateSym, ofGetFrameRate());
    value_setfloat(t_ofeliaGetElapsedTime::getElapsedTimeSym, ofGetElapsedTimef()*1000.0f);
    value_setfloat(t_ofeliaGetLastFrameTime::getLastFrameTimeSym, static_cast<t_float>(ofGetLastFrameTime()*1000.0));
    
    /* send update message to objects that listen to update */
    if (t_ofeliaWindow::updateSym->s_thing)
        typedmess(t_ofeliaWindow::updateSym->s_thing, t_ofeliaWindow::updateMess, 0, 0);
    
    if (bFirstUpdate)
        bFirstUpdate = false;
}

void ofeliaWindow::windowDraw(ofEventArgs &e)
{
    if (!bWindowExists || bFirstLoop || bFirstUpdate)
        return;
    
    if (bFullscreenMode) {
        
        ofGetCurrentRenderer()->pushView();
        ofGetCurrentRenderer()->viewport(fullscreenOffset.x,
                                         fullscreenOffset.y,
                                         (screenWidth * retinaScale)-fullscreenOffset.x*2.0f,
                                         (screenHeight * retinaScale)-fullscreenOffset.y*2.0f);
        ofGetCurrentRenderer()->setupScreen();
    }
    ofGetCurrentRenderer()->scale(scaleFactor, scaleFactor, scaleFactor);
    myOrien.drawRotation();
    t_ofeliaWindow::bRenderGate = true;
    
    /* send bang message through outlet */
    outlet_bang(x->x_obj.ob_outlet);
    
    /* send draw message to head objects */
    if (t_ofeliaWindow::drawSym->s_thing) {
        
        sort(t_ofeliaHead::vec.begin(),t_ofeliaHead::vec.end());
        
        for (size_t i=0; i<t_ofeliaHead::vec.size(); ++i) {
            
            t_atom av[1];
            av[0].a_type = A_FLOAT;
            av[0].a_w.w_float = static_cast<t_float>(t_ofeliaHead::vec[i].second);
            typedmess(t_ofeliaWindow::drawSym->s_thing, t_ofeliaWindow::drawMess, 1, av);
        }
    }
    t_ofeliaWindow::bRenderGate = false;
    
    if (bDepthTestEnabled)
        ofGetCurrentRenderer()->setDepthTest(false); //turn off depthTest
    myOrien.drawMasking();

    /* draw masking area on fullscreen mode */
    if (bFullscreenMode) {
        
        ofGetCurrentRenderer()->popView();
        
        if (!bFullscreenMaskPathSet) {
            
            fullscreenMaskPath->clear();
            fullscreenMaskPath->rectangle(0.0f, 0.0f, screenWidth * retinaScale, screenHeight * retinaScale);
            fullscreenMaskPath->rectangle(fullscreenOffset, widthScaled, heightScaled);
            fullscreenMaskMesh->operator=(fullscreenMaskPath->getTessellation());
            bFullscreenMaskPathSet = true;
        }
        ofGetCurrentRenderer()->setColor(ofColor(0));
        fullscreenMaskMesh->draw();
    }
    else {
        
        if (bFullscreenMaskPathSet)
            bFullscreenMaskPathSet = false;
    }
    if (bDepthTestEnabled)
        ofGetCurrentRenderer()->setDepthTest(true); //restore depthTest
}

void ofeliaWindow::windowMousePressed(ofMouseEventArgs &e)
{
    sendMouseToPd(WINDOW_MOUSE_PRESSED, e.button+1, mouseX, mouseY);
    
    if (e.button != 0) /* touch only works if the left mouse button is used */
        return;
    touchState = WINDOW_TOUCH_DOWN;
    touchID++;
    
    if (touchID < 0)
        touchID = 0;
    else if (touchID > OFELIA_MAX_TOUCHES-1)
             touchID = OFELIA_MAX_TOUCHES-1;
    sendTouchToPd(touchState, touchID, mouseX, mouseY);
    
    if (bMultiTouchSimEnabled)
        touches[touchID].set(mouseX, mouseY);
}

/* detect mouse movement while not pressed */
void ofeliaWindow::windowMouseMoved(ofMouseEventArgs &e)
{
    calibrateMousePosition(e);
    sendMouseToPd(WINDOW_MOUSE_MOVED, 0, e.x, e.y);
    mouseX = e.x;
    mouseY = e.y;
}

/* detect mouse movement while pressed */
void ofeliaWindow::windowMouseDragged(ofMouseEventArgs &e)
{
    calibrateMousePosition(e);
    sendMouseToPd(WINDOW_MOUSE_MOVED, e.button+1, e.x, e.y);
    mouseX = e.x;
    mouseY = e.y;
    
    if (e.button != 0) /* touch only works if the left mouse button is used */
        return;
    touchState = WINDOW_TOUCH_MOVED;
    
    if (touchID < 0)
        return;
    sendTouchToPd(touchState, touchID, mouseX, mouseY);
    
    if (bMultiTouchSimEnabled)
        touches[touchID].set(mouseX, mouseY);
}

void ofeliaWindow::windowMouseReleased(ofMouseEventArgs &e)
{
    sendMouseToPd(WINDOW_MOUSE_RELEASED, e.button+1, mouseX, mouseY);
    
    if (e.button != 0) /* touch only works if the left mouse button is used */
        return;
    touchState = WINDOW_TOUCH_UP;
    
    if (bMultiTouchSimEnabled || touchID < 0)
        return;
    sendTouchToPd(touchState, touchID, mouseX, mouseY);
    --touchID;
    
    if (bMultiTouchSimScheduled) {
        
        bMultiTouchSimEnabled = true;
        bMultiTouchSimScheduled = false;
    }
}

void ofeliaWindow::windowMouseScrolled(ofMouseEventArgs &e)
{
    sendScrollToPd(e.scrollX, e.scrollY);
}

void ofeliaWindow::windowKeyPressed(ofKeyEventArgs& e)
{
    sendKeyToPd(true, e.key);
    
    if (!bHotkeyEnabled)
        return;
    
    if (bHoldingFnKey) {
        
        switch (e.keycode) {
                
            case 45: /* key '-' : scale down */
            {
                if (bFullscreenMode)
                    return;
                if (setWindowScale(windowScale - 0.1f))
                    bWindowShouldResize = true;
                break;
            }
            case 61: /* key '+' : scale up */
            {
                if (bFullscreenMode)
                    return;
                if (setWindowScale(windowScale + 0.1f))
                    bWindowShouldResize = true;
                break;
            }
            case 65: /* key 'a' : enable accelerometer simulation mode */
            {
                if (!bAccelSimEnabled)
                    bAccelSimEnabled = true;
                break;
            }
            case 67: /* key 'c' : show/hide the cursor */
            {
                bCursorMode = bCursorMode == false;
                setCursorMode(bCursorMode);
                break;
            }
            case 70: /* key 'f' : toggle fullscreen */
            {
                setFullscreenMode(bFullscreenMode == false);
                break;
            }
            case 80: /* key 'p' : print all property values */
            {
                _print();
                break;
            }
            case 82: /* key 'r' : reset window */
            {
                if (bFullscreenMode)
                    setFullscreenMode(false);
                setDeviceOrientation(WINDOW_ORIENTATION_DEFAULT);
                initVariables();
                setWindowProperties();
                break;
            }
            case 84: /* key 't' : enable multitouch simulation mode */
            {
                if (!bMultiTouchSimEnabled) {
                    
                    if (touchState == WINDOW_TOUCH_UP)
                        bMultiTouchSimEnabled = true;
                    else
                        bMultiTouchSimScheduled = true;
                }
                break;
            }
            case 263: /* key 'left' : rotate left */
            {
                if (bFullscreenMode)
                    return;
                orientation = (orientation - 1) % 4;
                if (orientation < 0)
                    orientation += 4;
                myOrien.setDeviceOrientation(orientation);
                initOrientation = orientation;
                swapWindowDimensions();
                break;
            }
            case 262: /* key 'right' : rotate right */
            {
                if (bFullscreenMode)
                    return;
                orientation = (orientation + 1) % 4;
                myOrien.setDeviceOrientation(orientation);
                initOrientation = orientation;
                swapWindowDimensions();
                break;
            }
            case 265: /* key 'up' : rotate upside down */
            {
                if (bFullscreenMode)
                    return;
                orientation = (orientation + 2) % 4;
                myOrien.setDeviceOrientation(orientation);
                initOrientation = orientation;
                break;
            }
            case 264: /* key 'down' : rotate to DEFAULT */
            {
                if (bFullscreenMode)
                    return;
                bool dimenShouldSwap = false;
                
                if (orientation % 2)
                    dimenShouldSwap = true;
                orientation = WINDOW_ORIENTATION_DEFAULT;
                myOrien.setDeviceOrientation(orientation);
                initOrientation = orientation;
                
                if (dimenShouldSwap)
                    swapWindowDimensions();
                break;
            }
        }
    }
    else if (e.key == fnKey) {
        
        bHoldingFnKey = true;
    }
}

void ofeliaWindow::windowKeyReleased(ofKeyEventArgs& e)
{
    sendKeyToPd(false, e.key);
    
    if (!bHotkeyEnabled)
        return;
    
    switch (e.keycode) {
            
        case 65: /* key 'a' : disable accelerometer simulation mode */
        {
            if (bAccelSimEnabled)
                bAccelSimEnabled = false;
            break;
        }
        case 84: /* key 't' : disable multitouch simulation mode */
        {
            if (bMultiTouchSimEnabled) {
                
                touchState = WINDOW_TOUCH_UP;
                
                /* release all stored touches */
                for (int i = 0; i <= touchID; ++i)
                    sendTouchToPd(touchState, i, touches[i].x, touches[i].y);
                touchID = -1;
                bMultiTouchSimEnabled = false;
            }
            else if (bMultiTouchSimScheduled) {
                
                bMultiTouchSimScheduled = false;
            }
            break;
        }
    }
    if (e.key == fnKey) {
        
        bHoldingFnKey = false;
    }
}

void ofeliaWindow::windowResized(ofResizeEventArgs& e)
{
    /* if the window enters a fullscreen mode */
    if (e.width == screenWidth * retinaScale && e.height == screenHeight * retinaScale) {

        if (!bFullscreenMode) {
            
            windowScaleStored = windowScale;
            
            float deviceRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
            float screeenRatio = static_cast<float>(e.width) / static_cast<float>(e.height);
            
            if (deviceRatio < screeenRatio) {
                
                windowScale = static_cast<float>(e.height) / static_cast<float>(windowHeight * retinaScale);
                scaleFactor = retinaScale * windowScale;
                widthScaled = windowWidth * scaleFactor;
                heightScaled = e.height;
                fullscreenOffset.set((e.width - widthScaled) * 0.5f, 0.0f);
            }
            else {
                
                windowScale = static_cast<float>(e.width) / static_cast<float>(windowWidth * retinaScale);
                scaleFactor = retinaScale * windowScale;
                widthScaled = e.width;
                heightScaled = windowHeight * scaleFactor;
                fullscreenOffset.set(0.0f, (e.height - heightScaled) * 0.5f);
            }
            bWindowResizeLocked = true;
            bFullscreenMode = true;
        }
        else {
            
            windowScale = windowScaleStored;
            resizeWindow();
            bFullscreenMode = false;
        }
        sendScaleToPd(windowScale);
        sendFullscreenToPd(bFullscreenMode);
    }
    else if (!bWindowShouldResize && !bWindowResizeLocked) {
        
        if (!bScaleDirectionFixed) {
            
            int gapW = abs(widthScaled - e.width);
            int gapH = abs(heightScaled - e.height);
            
            if (gapW >= gapH)
                bScaleDirection = WINDOW_SCALE_DIR_HORIZONTAL;
            else
                bScaleDirection = WINDOW_SCALE_DIR_VERTICAL;
            bScaleDirectionFixed = true;
        }
        float ratio;
        int w, h;
        
        if (bScaleDirection == WINDOW_SCALE_DIR_HORIZONTAL) {
            
            ratio = static_cast<float>(windowHeight) / static_cast<float>(windowWidth);
            w = e.width;
            h = e.width * ratio;
            windowScale = static_cast<float>(w) / static_cast<float>(windowWidth * retinaScale);
        }
        else if (bScaleDirection == WINDOW_SCALE_DIR_VERTICAL) {
            
            ratio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
            w = e.height * ratio;
            h = e.height;
            windowScale = static_cast<float>(h) / static_cast<float>(windowHeight * retinaScale);
        }
#if defined(TARGET_LINUX)
        /* OFELIANOTE : disable the manual window scaling as it is buggy on Linux */
        windowScale = initWindowScale;
#else
        initWindowScale = windowScale;
#endif
        resizeWindow();
        bFullscreenMode = false;
    }
}

void ofeliaWindow::windowDestroyed(ofEventArgs& e)
{
    initWindowPosX = windowPosX;
    initWindowPosY = windowPosY;
    windowPosX = windowPosY = 0;
    initAccelForce = accelForce;
    removeWindowListeners();
    t_ofeliaWindow::bRenderGate = false;
    bWindowExists = false;
    bWindowOwner = false;
    myOrien.lastDeviceWidth = 0;
    myOrien.lastDeviceHeight = 0;
    myOrien.bWindowOwner = false;
    sendWindowToPd(false);
    sendExitToPd(); //no need to call this when running as a standalone app
}

/* ________________________________________________________________________________
 * static methods
 */
void ofeliaWindow::resizeWindow()
{
    bWindowResizeLocked = true;
    scaleFactor = retinaScale * windowScale;
    widthScaled = windowWidth * scaleFactor;
    heightScaled = windowHeight * scaleFactor;
    ofSetWindowShape(widthScaled, heightScaled);
#if defined(TARGET_LINUX)
    if (!glfwGetWindowAttrib(GLFWwin, GLFW_RESIZABLE))
        ofSetWindowPosition(windowPosX, windowPosY);
#endif
    bWindowResizeLocked = false;
}

void ofeliaWindow::pollEventsMethod(void *nul)
{
    ofGetMainLoop()->loopOnce();
    ofGetMainLoop()->pollEvents();
    
    if (bFirstLoop) {

        if (t_ofeliaWindow::firstLoopSym->s_thing)
            typedmess(t_ofeliaWindow::firstLoopSym->s_thing, t_ofeliaWindow::firstLoopMess, 0, 0);
        bFirstLoop = false;
        sendWindowToPd(true);
    }
    else if (!bWindowExists) {
        
        clock_free(pollEventsClock);
        
        if (window) {
            
            ofGetMainLoop()->loopOnce();
            window = NULL;
            GLFWwin = NULL;
        }
        ofGetMainLoop()->pollEvents();
        return;
    }
    else if (bScaleDirectionFixed) {
        
        sendScaleToPd(windowScale);
        bScaleDirectionFixed = false;
    }
    else if (bWindowShouldResize) {
        
        windowResizeCount++;
        
        if (windowResizeCount > 2) {
            
            resizeWindow();
            sendScaleToPd(windowScale);
            windowResizeCount = 0;
            bWindowShouldResize = false;
        }
    }
    clock_delay(pollEventsClock, 1000.0 / static_cast<double>(ofGetTargetFrameRate()));
}

void ofeliaWindow::sendTouchToPd(const int touchState, const int touchID, const float posX, const float posY)
{
    t_atom av[4];
    av[0].a_type = A_FLOAT;
    av[0].a_w.w_float = static_cast<t_float>(touchState);
    av[1].a_type = A_FLOAT;
    av[1].a_w.w_float = static_cast<t_float>(touchID);
    av[2].a_type = A_FLOAT;
    av[2].a_w.w_float = posX;
    av[3].a_type = A_FLOAT;
    av[3].a_w.w_float = posY;
    
    if (t_ofeliaTouchListener::touchListenerSym->s_thing)
        pd_list(t_ofeliaTouchListener::touchListenerSym->s_thing, 0, 4, av);
}

void ofeliaWindow::sendMouseToPd(const int mouseState, const int button, const float posX, const float posY)
{
    t_atom av[4];
    av[0].a_type = A_FLOAT;
    av[0].a_w.w_float = static_cast<t_float>(mouseState);
    av[1].a_type = A_FLOAT;
    av[1].a_w.w_float = static_cast<t_float>(button);
    av[2].a_type = A_FLOAT;
    av[2].a_w.w_float = posX;
    av[3].a_type = A_FLOAT;
    av[3].a_w.w_float = posY;
    
    if (t_ofeliaMouseListener::mouseListenerSym->s_thing)
        pd_list(t_ofeliaMouseListener::mouseListenerSym->s_thing, 0, 4, av);
}

void ofeliaWindow::sendScrollToPd(const float scrollX, const float scrollY)
{
    t_atom av[2];
    av[0].a_type = A_FLOAT;
    av[0].a_w.w_float = scrollX;
    av[1].a_type = A_FLOAT;
    av[1].a_w.w_float = scrollY;
    
    if (t_ofeliaScrollListener::scrollListenerSym->s_thing)
        pd_list(t_ofeliaScrollListener::scrollListenerSym->s_thing, 0, 2, av);
}

void ofeliaWindow::sendKeyToPd(const bool pressed, const int key)
{
    t_atom av[2];
    av[0].a_type = A_FLOAT;
    av[0].a_w.w_float = static_cast<t_float>(pressed);
    av[1].a_type = A_FLOAT;
    av[1].a_w.w_float = static_cast<t_float>(key);
    
    if (t_ofeliaKeyListener::keyListenerSym->s_thing)
        pd_list(t_ofeliaKeyListener::keyListenerSym->s_thing, 0, 2, av);
}

void ofeliaWindow::sendAccelToPd(const float accelX, const float accelY, const float accelZ)
{
    t_atom av[3];
    av[0].a_type = A_FLOAT;
    av[0].a_w.w_float = accelX;
    av[1].a_type = A_FLOAT;
    av[1].a_w.w_float = accelY;
    av[2].a_type = A_FLOAT;
    av[2].a_w.w_float = accelZ;
    
    if (t_ofeliaAccelListener::accelListenerSym->s_thing)
        pd_list(t_ofeliaAccelListener::accelListenerSym->s_thing, 0, 3, av);
}

void ofeliaWindow::sendScaleToPd(const float wScale)
{
    value_setfloat(t_ofeliaGetScale::getScaleSym, wScale);
    
    if (t_ofeliaScaleListener::scaleListenerSym->s_thing)
        pd_float(t_ofeliaScaleListener::scaleListenerSym->s_thing, wScale);
}

void ofeliaWindow::sendFullscreenToPd(const bool fMode)
{
    value_setfloat(t_ofeliaGetFullscreen::getFullscreenSym, static_cast<t_float>(fMode));
    
    if (t_ofeliaFullscreenListener::fullscreenListenerSym->s_thing)
        pd_float(t_ofeliaFullscreenListener::fullscreenListenerSym->s_thing, static_cast<t_float>(fMode));
}

void ofeliaWindow::sendFocusToPd(GLFWwindow *window, const int focused)
{
    value_setfloat(t_ofeliaGetFocus::getFocusSym, static_cast<t_float>(focused));
    
    if (t_ofeliaFocusListener::focusListenerSym->s_thing)
        pd_float(t_ofeliaFocusListener::focusListenerSym->s_thing, static_cast<t_float>(focused));
}

void ofeliaWindow::sendPosToPd(GLFWwindow* window, const int posX, const int posY)
{
    if (posX == windowPosX && posY == windowPosY)
        return;
    value_setfloat(t_ofeliaGetPosX::getPosXSym, static_cast<t_float>(posX));
    value_setfloat(t_ofeliaGetPosY::getPosYSym, static_cast<t_float>(posY));
    t_atom av[2];
    av[0].a_type = A_FLOAT;
    av[0].a_w.w_float = static_cast<t_float>(posX);
    av[1].a_type = A_FLOAT;
    av[1].a_w.w_float = static_cast<t_float>(posY);
    
    if (t_ofeliaPosListener::posListenerSym->s_thing)
        pd_list(t_ofeliaPosListener::posListenerSym->s_thing, 0, 2, av);
    windowPosX = posX;
    windowPosY = posY;
}

void ofeliaWindow::sendWindowToPd(const bool wMode)
{
    value_setfloat(t_ofeliaGetWindow::getWindowSym, static_cast<t_float>(wMode));
    
    if (t_ofeliaWindowListener::windowListenerSym->s_thing)
        pd_float(t_ofeliaWindowListener::windowListenerSym->s_thing, static_cast<t_float>(wMode));
}

void ofeliaWindow::sendBackToPd()
{
    if (t_ofeliaBackListener::backListenerSym->s_thing)
        pd_bang(t_ofeliaBackListener::backListenerSym->s_thing);
}

void ofeliaWindow::sendInitToPd()
{
    if (t_ofeliaWindow::initSym->s_thing)
        typedmess(t_ofeliaWindow::initSym->s_thing, t_ofeliaWindow::initMess, 0, 0);
}

void ofeliaWindow::sendExitToPd()
{
    if (t_ofeliaWindow::exitSym->s_thing)
        typedmess(t_ofeliaWindow::exitSym->s_thing, t_ofeliaWindow::exitMess, 0, 0);
}

