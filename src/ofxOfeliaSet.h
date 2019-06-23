#pragma once

#include "m_pd.h"
#include "ofxOfeliaClient.h"

class ofxOfeliaSet
{
public:
    ofxOfeliaSet(){};
    ~ofxOfeliaSet(){};
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
    static void bangWrapper(ofxOfeliaSet *x);
    static void floatWrapper(ofxOfeliaSet *x, t_floatarg f);
    static void symbolWrapper(ofxOfeliaSet *x, t_symbol *s);
    static void pointerWrapper(ofxOfeliaSet *x, t_gpointer *p);
    static void listWrapper(ofxOfeliaSet *x, t_symbol *s, int argc, t_atom *argv);
    static void anythingWrapper(ofxOfeliaSet *x, t_symbol *s, int argc, t_atom *argv);
    static void dspWrapper(ofxOfeliaSet *x, t_signal **sp);
    static void freeWrapper(ofxOfeliaSet *x);
    static void setup();
    static t_class *pdClass;
    ofxOfeliaClient client;
private:
};
