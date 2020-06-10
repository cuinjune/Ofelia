#pragma once

#include "m_pd.h"
#include "g_canvas.h"
#include "ofxOfeliaLua.h"
#include "ofxOfeliaSignal.h"
#include "ofxOfeliaIO.h"
#include "ofxOfeliaTextBuf.h"

class ofxOfeliaData
{
public:
    ofxOfeliaData()
    :binbuf(nullptr)
    ,canvas(nullptr)
    ,guiconnect(nullptr)
    ,sym(&s_)
    ,embName(&s_)
    ,isFunctionMode(false)
    ,shouldKeep(false)
    ,isSignalObject(false)
    ,isDirectMode(false)
    ,hasUniqueSym(false)
    ,lua(this)
    ,signal(this)
    ,io(this)
    ,textBuf(this){};
    
    void initSym();
    void bindSym();
    void unbindSym();
    void argParse(t_symbol *s, int argc, t_atom *argv, bool define);
    static ofxOfeliaData *getPtr(t_symbol *s);
    
    t_object ob;
    t_binbuf *binbuf;
    t_canvas *canvas;
    t_guiconnect *guiconnect;
    t_symbol *sym;
    t_symbol *embName;
    t_symbol *getUniqueSym();
    
    /* states */
    bool isFunctionMode; /* whether to automatically wrap the script in a function */
    bool shouldKeep; /* whether to embed contents in patch on save */
    bool isSignalObject; /* whether the object is a signal object or not */
    bool isDirectMode; /* whether to write script directly on object */
    bool hasUniqueSym; /* whether the object has a unique symbol */
    
    ofxOfeliaLua lua;
    ofxOfeliaSignal signal;
    ofxOfeliaIO io;
    ofxOfeliaTextBuf textBuf;
private:
};
