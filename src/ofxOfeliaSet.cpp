#include "ofxOfeliaSet.h"
#include <new>

t_class *ofxOfeliaSet::pdClass;

void *ofxOfeliaSet::newMethod(t_symbol *s, int argc, t_atom *argv)
{
    client.data.argParse(argc, argv, gensym("set"), false);
    return this;
}

void ofxOfeliaSet::bangMethod()
{
    client.bangMethod();
}

void ofxOfeliaSet::floatMethod(t_floatarg f)
{
    client.floatMethod(f);
}

void ofxOfeliaSet::symbolMethod(t_symbol *s)
{
    client.symbolMethod(s);
}

void ofxOfeliaSet::pointerMethod(t_gpointer *p)
{
    client.pointerMethod(p);
}

void ofxOfeliaSet::listMethod(t_symbol *s, int argc, t_atom *argv)
{
    client.listMethod(s, argc, argv);
}

void ofxOfeliaSet::anythingMethod(t_symbol *s, int argc, t_atom *argv)
{
    client.setVariableByArgs(s, argc, argv);
}

void ofxOfeliaSet::dspMethod(t_signal **sp)
{
    client.dspMethod(sp);
}

void ofxOfeliaSet::freeMethod()
{
    client.freeMethod();
}

void *ofxOfeliaSet::newWrapper(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaSet *x = reinterpret_cast<ofxOfeliaSet *>(pd_new(pdClass));
    new (x) ofxOfeliaSet();
    return x->newMethod(s, argc, argv);
}

void ofxOfeliaSet::bangWrapper(ofxOfeliaSet *x)
{
    x->bangMethod();
}

void ofxOfeliaSet::floatWrapper(ofxOfeliaSet *x, t_floatarg f)
{
    x->floatMethod(f);
}

void ofxOfeliaSet::symbolWrapper(ofxOfeliaSet *x, t_symbol *s)
{
    x->symbolMethod(s);
}

void ofxOfeliaSet::pointerWrapper(ofxOfeliaSet *x, t_gpointer *p)
{
    x->pointerMethod(p);
}

void ofxOfeliaSet::listWrapper(ofxOfeliaSet *x, t_symbol *s, int argc, t_atom *argv)
{
    x->listMethod(s, argc, argv);
}

void ofxOfeliaSet::anythingWrapper(ofxOfeliaSet *x, t_symbol *s, int argc, t_atom *argv)
{
    x->anythingMethod(s, argc, argv);
}

void ofxOfeliaSet::dspWrapper(ofxOfeliaSet *x, t_signal **sp)
{
    x->dspMethod(sp);
}

void ofxOfeliaSet::freeWrapper(ofxOfeliaSet *x)
{
    x->freeMethod();
    x->~ofxOfeliaSet();
}

void ofxOfeliaSet::setup()
{
    pdClass = class_new(gensym("ofelia set"),
                        reinterpret_cast<t_newmethod>(newWrapper),
                        reinterpret_cast<t_method>(freeWrapper),
                        sizeof(ofxOfeliaSet), 0, A_GIMME, 0);
    class_addmethod(pdClass, reinterpret_cast<t_method>(dspWrapper), gensym("dsp"), A_CANT, 0);
    CLASS_MAINSIGNALIN(pdClass, ofxOfeliaSet, client.data.signal.f);
    class_addbang(pdClass, bangWrapper);
    class_addfloat(pdClass, floatWrapper);
    class_addsymbol(pdClass, symbolWrapper);
    class_addpointer(pdClass, pointerWrapper);
    class_addlist(pdClass, listWrapper);
    class_addanything(pdClass, anythingWrapper);
    class_sethelpsymbol(pdClass, gensym("ofelia-object"));
}
