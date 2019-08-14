#pragma once

#include "m_pd.h"

class ofxOfeliaDefine;
class ofxOfeliaFunction
{
public:
    ofxOfeliaFunction(ofxOfeliaDefine *definePtr)
    :definePtr(definePtr){};
    void *newMethod(t_symbol *s, int argc, t_atom *argv);
    static void *newWrapper(t_symbol *s, int argc, t_atom *argv);
    static void setup();
private:
    ofxOfeliaDefine *definePtr; /* pointer to define */
};
