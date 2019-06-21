#pragma once

#include "m_pd.h"
#include "ofxOfeliaData.h"

class ofxOfeliaClient
{
public:
    void bangMethod();
    void floatMethod(t_floatarg f);
    void symbolMethod(t_symbol *s);
    void pointerMethod(t_gpointer *p);
    void listMethod(t_symbol *s, int argc, t_atom *argv);
    void getVariableByArgs(t_symbol *s, int argc, t_atom *argv);
    void setVariableByArgs(t_symbol *s, int argc, t_atom *argv);
    void dspMethod(t_signal **sp);
    void freeMethod();
    ofxOfeliaData data;
private:
    bool isDataValid();
};
