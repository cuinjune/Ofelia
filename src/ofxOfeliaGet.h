#pragma once

#include "m_pd.h"
#include "ofxOfeliaClient.h"

class ofxOfeliaGet
{
public:
    ofxOfeliaGet(){};
    ~ofxOfeliaGet(){};
    void *newMethod(t_symbol *s, int argc, t_atom *argv);
    void bangMethod();
    void floatMethod(t_floatarg f);
    void symbolMethod(t_symbol *s);
    void pointerMethod(t_gpointer *p);
    void listMethod(t_symbol *s, int argc, t_atom *argv);
    void anythingMethod(t_symbol *s, int argc, t_atom *argv);
    void dspMethod(t_signal **sp);
    void freeMethod();
    static void *newWrapper(t_symbol *s, int argc, t_atom *argv);
    static void bangWrapper(ofxOfeliaGet *x);
    static void floatWrapper(ofxOfeliaGet *x, t_floatarg f);
    static void symbolWrapper(ofxOfeliaGet *x, t_symbol *s);
    static void pointerWrapper(ofxOfeliaGet *x, t_gpointer *p);
    static void listWrapper(ofxOfeliaGet *x, t_symbol *s, int argc, t_atom *argv);
    static void anythingWrapper(ofxOfeliaGet *x, t_symbol *s, int argc, t_atom *argv);
    static void dspWrapper(ofxOfeliaGet *x, t_signal **sp);
    static void freeWrapper(ofxOfeliaGet *x);
    static void setup();
    static t_class *pdClass;
    ofxOfeliaClient client;
private:
};
