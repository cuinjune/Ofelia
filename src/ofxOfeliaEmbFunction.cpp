#include "ofxOfeliaEmbFunction.h"
#include "ofxOfeliaDefine.h"
#include <new>

void *ofxOfeliaEmbFunction::newMethod(t_symbol *s, int argc, t_atom *argv)
{
    return definePtr->embBase.functionMapScript(s, argc, argv);
}

void *ofxOfeliaEmbFunction::newWrapper(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embFunction.newMethod(s, argc, argv);
}

void ofxOfeliaEmbFunction::setup()
{
    for (auto it = ofxOfeliaMaps::functionMap.begin(); it != ofxOfeliaMaps::functionMap.end(); ++it)
        class_addcreator(reinterpret_cast<t_newmethod>(newWrapper),
                         gensym(it->first.c_str()), A_GIMME, 0);
}
