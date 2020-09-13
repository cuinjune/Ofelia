// app folder bindings
// 2017 Dan Wilcox <danomatika@gmail.com>

// ----- ofWindowSettings.h -----

// DIFF: ofWindowSettings.h:
// DIFF:   ignoring api-specific window settings classes
%ignore ofGLWindowSettings;
%ignore ofGLESWindowSettings;

%include "app/ofWindowSettings.h"

// ----- ofMainLoop.h -----

// not needed

// ----- ofAppNoWindow.h -----

// not needed

// ----- ofAppGLFWWindow.h -----

// not needed

// ----- ofAppBaseWindow.h -----

// not needed

// ----- ofAppRunner.h -----

// DIFF: ofAppRunner.h:
// DIFF:   ofInit, ofSetupOpenGL, & ofCreateWindow not applicable in target language
%ignore ofInit;
%ignore ofSetupOpenGL;
%ignore ofCreateWindow;

// DIFF:   get/set main loop not applicable to target language
%ignore ofGetMainLoop;
%ignore ofSetMainLoop;

// DIFF:   run app & main loop not applicable to target language
%ignore noopDeleter;
%ignore ofRunApp;
%ignore ofRunMainLoop;

// DIFF:   ofGetAppPtr not applicable in a target language
%ignore ofGetAppPtr;

// DIFF:   ofGetWindowPtr and ofGetCurrentWindow not applicable in a target language
%ignore ofGetWindowPtr;
%ignore ofGetCurrentWindow;

// DIFF:   get/set current renderer not applicable to target language
%ignore ofSetCurrentRenderer;
%ignore ofGetCurrentRenderer;

// DIFF:   ignoring api-specific window objects: display, window, context, surface
%ignore ofGetX11Display;
%ignore ofGetX11Window;
%ignore ofGetGLXContext;
%ignore ofGetEGLDisplay;
%ignore ofGetEGLContext;
%ignore ofGetEGLSurface;
%ignore ofGetNSGLContext;
%ignore ofGetCocoaWindow;
%ignore ofGetWGLContext;
%ignore ofGetWin32Window;

%include "app/ofAppRunner.h"

// ----- ofBaseApp.h -----

// not needed
