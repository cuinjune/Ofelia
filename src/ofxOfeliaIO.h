#pragma once

#include "m_pd.h"

class ofxOfeliaData;
class ofxOfeliaIO
{
public:
    ofxOfeliaIO(ofxOfeliaData *dataPtr)
    :dataPtr(dataPtr)
    ,hasMultiControlInlets(false)
    ,hasMultiControlOutlets(false)
    ,hasControlOutlet(false){};
    void newControlIO(int numInlets, int numOutlets);
    void newSignalIO(int numInlets, int numOutlets);
    void freeControlIO();
    void freeSignalIO();
    void doList(int ac, t_atom *av);
    t_inlet **inlets;
    t_outlet **outlets;
    t_float *fv; /* variables for float inlets */
    int numInlets;
    int numOutlets;
    bool hasMultiControlInlets; /* whether an object has multiple control inlets */
    bool hasMultiControlOutlets; /* whether an object has multiple control outlets */
    bool hasControlOutlet; /* whether an object has any control outlet */
private:
    void newIO(int ninlets, int noutlets);
    void freeIO(int ninlets, int noutlets);
    ofxOfeliaData *dataPtr; /* pointer to data */
};
