#pragma once

#include "m_pd.h"
#include "ofxOfeliaClient.h"

class ofxOfeliaExists
{
public:
    ofxOfeliaExists(){};
    ~ofxOfeliaExists(){};
    void *newMethod(t_symbol *s, int argc, t_atom *argv);
    void bangMethod();
    static void *newWrapper(t_symbol *s, int argc, t_atom *argv);
    static void bangWrapper(ofxOfeliaExists *x);
    static void freeWrapper(ofxOfeliaExists *x);
    static void setup();
    static t_class *pdClass;
    ofxOfeliaClient client;
private:
};
