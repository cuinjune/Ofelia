#pragma once

#include "m_pd.h"
#include "ofxOfeliaEmbBase.h"

class ofxOfeliaDefine;
class ofxOfeliaEmbFunction
{
public:
    ofxOfeliaEmbFunction(ofxOfeliaDefine *definePtr)
    :definePtr(definePtr){};
    void *newMethod(t_symbol *s, int argc, t_atom *argv);
    static void *newWrapper(t_symbol *s, int argc, t_atom *argv);
    static void setup();
private:
    ofxOfeliaDefine *definePtr; /* pointer to define */
};
