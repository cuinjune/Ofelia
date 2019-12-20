#pragma once

#include "m_pd.h"

class ofxOfeliaData;
class ofxOfeliaSignal
{
public:
    ofxOfeliaSignal(ofxOfeliaData *dataPtr)
    :dataPtr(dataPtr){};
    void addDsp(t_signal **sp);
    static t_int *perform(t_int *w);
    t_int **w;
    t_float f; /* variable for main signal inlet */
private:
    ofxOfeliaData *dataPtr; /* pointer to data */
};
