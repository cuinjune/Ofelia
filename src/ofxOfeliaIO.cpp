#include "ofxOfeliaIO.h"
#include "ofxOfeliaData.h"

void ofxOfeliaIO::newIO(int numInlets, int numOutlets)
{
    inlets = static_cast<t_inlet **>(getbytes(sizeof(t_inlet *) * numInlets));
    outlets = static_cast<t_outlet **>(getbytes(sizeof(t_outlet *) * numOutlets));
}

void ofxOfeliaIO::freeIO(int numInlets, int numOutlets)
{
    for (int i = 0; i < numInlets; ++i)
        inlet_free(inlets[i]);
    for (int i = 0; i < numOutlets; ++i)
        outlet_free(outlets[i]);
}

void ofxOfeliaIO::newControlIO(int numInlets, int numOutlets)
{
    this->numInlets = numInlets;
    this->numOutlets = numOutlets;
    hasMultiControlInlets = numInlets > 1;
    hasMultiControlOutlets = numOutlets > 1;
    hasControlOutlet = numOutlets > 0;
    newIO(--numInlets, numOutlets);
    av = static_cast<t_atom *>(getbytes(sizeof(t_atom) * numInlets));
    for (int i = 0; i < numInlets; ++i)
        SETFLOAT(av + i, 0);
}

void ofxOfeliaIO::newSignalIO(int numInlets, int numOutlets)
{
    this->numInlets = numInlets;
    this->numOutlets = numOutlets;
    dataPtr->signal.w = static_cast<t_int **>(getbytes(sizeof(t_int *) * (numInlets + numOutlets + 2)));
    newIO(--numInlets, numOutlets);
    dataPtr->signal.f = 0;
    dataPtr->isSignalObject = true;
}

void ofxOfeliaIO::addControlIO()
{
    for (int i = 0; i < numInlets - 1; ++i)
    {
        if (av[i].a_type == A_FLOAT)
            inlets[i] = floatinlet_new(&dataPtr->ob, &av[i].a_w.w_float);
        else if (av[i].a_type == A_SYMBOL)
            inlets[i] = symbolinlet_new(&dataPtr->ob, &av[i].a_w.w_symbol);
    }
    for (int i = 0; i < numOutlets; ++i)
        outlets[i] = outlet_new(&dataPtr->ob, &s_list);
}

void ofxOfeliaIO::addSignalIO()
{
    for (int i = 0; i < numInlets - 1; ++i)
        inlets[i] = inlet_new(&dataPtr->ob, &dataPtr->ob.ob_pd, &s_signal, &s_signal);
    for (int i = 0; i < numOutlets; ++i)
        outlets[i] = outlet_new(&dataPtr->ob, &s_signal);
}

void ofxOfeliaIO::freeControlIO()
{
    int numInlets = this->numInlets;
    int numOutlets = this->numOutlets;
    freeIO(--numInlets, numOutlets);
    freebytes(av, sizeof(t_atom) * numInlets);
}

void ofxOfeliaIO::freeSignalIO()
{
    int numInlets = this->numInlets;
    int numOutlets = this->numOutlets;
    freebytes(dataPtr->signal.w, sizeof(t_int *) * (numInlets + numOutlets + 2));
    freeIO(--numInlets, numOutlets);
}

void ofxOfeliaIO::doList(int argc, t_atom *argv)
{
    /* copies the control inlet values from the first index */
    int first = argc - numInlets + 1;
    for (int i = first; i < argc; ++i)
        argv[i] = av[i - first];
    dataPtr->lua.doFunction(&s_list, argc, argv);
}
