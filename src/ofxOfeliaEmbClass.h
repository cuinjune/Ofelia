#pragma once

#include "m_pd.h"
#include "ofxOfeliaEmbBase.h"

class ofxOfeliaDefine;
class ofxOfeliaEmbClass
{
public:
    ofxOfeliaEmbClass(ofxOfeliaDefine *definePtr)
    :definePtr(definePtr){};
    void *newMethod(t_symbol *s, int argc, t_atom *argv,
                    ofxOfeliaEmbBase::getClassAdditionalScriptFuncPtr getClassAdditionalScriptFunc);
    static void *newWrapper_default(t_symbol *s, int argc, t_atom *argv);
    static void *newWrapper_clear(t_symbol *s, int argc, t_atom *argv);
    static void *newWrapper_close(t_symbol *s, int argc, t_atom *argv);
    static void *newWrapper_unload(t_symbol *s, int argc, t_atom *argv);
    static void *newWrapper_destroy(t_symbol *s, int argc, t_atom *argv);
    static void setup();
private:
    static t_symbol *getClassAdditionalScript_default(t_symbol *varName);
    static t_symbol *getClassAdditionalScript_clear(t_symbol *varName);
    static t_symbol *getClassAdditionalScript_close(t_symbol *varName);
    static t_symbol *getClassAdditionalScript_unload(t_symbol *varName);
    static t_symbol *getClassAdditionalScript_destroy(t_symbol *varName);
    ofxOfeliaDefine *definePtr; /* pointer to define */
};
