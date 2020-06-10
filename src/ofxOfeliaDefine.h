#pragma once

#include "m_pd.h"
#include "ofxOfeliaData.h"
#include "ofxOfeliaFunction.h"

class ofxOfeliaDefine
{
public:
    ofxOfeliaDefine()
    :function(this){};
    ~ofxOfeliaDefine(){};
    void *newMethod(t_symbol *s, int argc, t_atom *argv);
    void bangMethod();
    void floatMethod(t_floatarg f);
    void symbolMethod(t_symbol *s);
    void pointerMethod(t_gpointer *p);
    void listMethod(t_symbol *s, int argc, t_atom *argv);
    void anythingMethod(t_symbol *s, int argc, t_atom *argv);
    void clearMethod();
    void setMethod(t_symbol *s, int argc, t_atom *argv);
    void saveMethod(t_object *ob, t_binbuf *bb);
    void dspMethod(t_signal **sp);
    void notifyMethod();
    void freeMethod();
    static void *newWrapper(t_symbol *s, int argc, t_atom *argv);
    static void bangWrapper(ofxOfeliaDefine *x);
    static void floatWrapper(ofxOfeliaDefine *x, t_floatarg f);
    static void symbolWrapper(ofxOfeliaDefine *x, t_symbol *s);
    static void pointerWrapper(ofxOfeliaDefine *x, t_gpointer *p);
    static void listWrapper(ofxOfeliaDefine *x, t_symbol *s, int argc, t_atom *argv);
    static void anythingWrapper(ofxOfeliaDefine *x, t_symbol *s, int argc, t_atom *argv);
    static void clearWrapper(ofxOfeliaDefine *x);
    static void setWrapper(ofxOfeliaDefine *x, t_symbol *s, int argc, t_atom *argv);
    static void saveWrapper(t_gobj *z, t_binbuf *bb);
    static void dspWrapper(ofxOfeliaDefine *x, t_signal **sp);
    static void notifyWrapper(ofxOfeliaDefine *x);
    static void openWrapper(ofxOfeliaData *x);
    static void closeWrapper(ofxOfeliaData *x);
    static void addLineWrapper(ofxOfeliaData *x, t_symbol *s, int argc, t_atom *argv);
    static void readWrapper(ofxOfeliaData *x, t_symbol *s, int argc, t_atom *argv);
    static void writeWrapper(ofxOfeliaData *x, t_symbol *s, int argc, t_atom *argv);
    static void freeWrapper(ofxOfeliaDefine *x);
    static void setup();
    static t_class *pdClass;
    ofxOfeliaData data;
    ofxOfeliaFunction function;
private:
};
