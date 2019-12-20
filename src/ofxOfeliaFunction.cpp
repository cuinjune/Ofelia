#include "ofxOfeliaFunction.h"
#include "ofxOfeliaDefine.h"
#include <new>

void *ofxOfeliaFunction::newMethod(t_symbol *s, int argc, t_atom *argv)
{
    definePtr->data.isFunctionMode = true;
    return definePtr->newMethod(s, argc, argv);
}

void *ofxOfeliaFunction::newWrapper(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->function.newMethod(s, argc, argv);
}

void ofxOfeliaFunction::setup()
{
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper),
                     gensym("ofelia function"), A_GIMME, 0);
}
