#pragma once

#include "m_pd.h"

class ofxOfeliaData;
class ofxOfeliaIO
{
public:
    ofxOfeliaIO(ofxOfeliaData *dataPtr)
    :hasMultiControlInlets(false)
    ,hasMultiControlOutlets(false)
    ,hasControlOutlet(false)
    ,dataPtr(dataPtr){};
    void newControlIO(int numInlets, int numOutlets);
    void newSignalIO(int numInlets, int numOutlets);
    void addControlIO();
    void addSignalIO();
    void freeControlIO();
    void freeSignalIO();
    void doList(int argc, t_atom *argv);
    t_inlet **inlets;
    t_outlet **outlets;
    t_atom *av; /* variables for passive inlets */
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
