#include "ofxOfeliaGet.h"
#include <new>

t_class *ofxOfeliaGet::pdClass;

void *ofxOfeliaGet::newMethod(t_symbol *s, int argc, t_atom *argv)
{
    client.data.argParse(argc, argv, gensym("get"), false);
    return this;
}

void ofxOfeliaGet::bangMethod()
{
    client.bangMethod();
}

void ofxOfeliaGet::floatMethod(t_floatarg f)
{
    client.floatMethod(f);
}

void ofxOfeliaGet::symbolMethod(t_symbol *s)
{
    client.symbolMethod(s);
}

void ofxOfeliaGet::pointerMethod(t_gpointer *p)
{
    client.pointerMethod(p);
}

void ofxOfeliaGet::listMethod(t_symbol *s, int argc, t_atom *argv)
{
    client.listMethod(s, argc, argv);
}

void ofxOfeliaGet::anythingMethod(t_symbol *s, int argc, t_atom *argv)
{
    client.getVariableByArgs(s, argc, argv);
}

void ofxOfeliaGet::dspMethod(t_signal **sp)
{
    client.dspMethod(sp);
}

void ofxOfeliaGet::freeMethod()
{
    client.freeMethod();
}

void *ofxOfeliaGet::newWrapper(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaGet *x = reinterpret_cast<ofxOfeliaGet *>(pd_new(pdClass));
    new (x) ofxOfeliaGet();
    return x->newMethod(s, argc, argv);
}

void ofxOfeliaGet::bangWrapper(ofxOfeliaGet *x)
{
    x->bangMethod();
}

void ofxOfeliaGet::floatWrapper(ofxOfeliaGet *x, t_floatarg f)
{
    x->floatMethod(f);
}

void ofxOfeliaGet::symbolWrapper(ofxOfeliaGet *x, t_symbol *s)
{
    x->symbolMethod(s);
}

void ofxOfeliaGet::pointerWrapper(ofxOfeliaGet *x, t_gpointer *p)
{
    x->pointerMethod(p);
}

void ofxOfeliaGet::listWrapper(ofxOfeliaGet *x, t_symbol *s, int argc, t_atom *argv)
{
    x->listMethod(s, argc, argv);
}

void ofxOfeliaGet::anythingWrapper(ofxOfeliaGet *x, t_symbol *s, int argc, t_atom *argv)
{
    x->anythingMethod(s, argc, argv);
}

void ofxOfeliaGet::dspWrapper(ofxOfeliaGet *x, t_signal **sp)
{
    x->dspMethod(sp);
}

void ofxOfeliaGet::freeWrapper(ofxOfeliaGet *x)
{
    x->freeMethod();
    x->~ofxOfeliaGet();
}

void ofxOfeliaGet::setup()
{
    pdClass = class_new(gensym("ofelia get"),
                        reinterpret_cast<t_newmethod>(newWrapper),
                        reinterpret_cast<t_method>(freeWrapper),
                        sizeof(ofxOfeliaGet), 0, A_GIMME, 0);
    class_addmethod(pdClass, reinterpret_cast<t_method>(dspWrapper), gensym("dsp"), A_CANT, 0);
    CLASS_MAINSIGNALIN(pdClass, ofxOfeliaGet, client.data.signal.f);
    class_addbang(pdClass, bangWrapper);
    class_addfloat(pdClass, floatWrapper);
    class_addsymbol(pdClass, symbolWrapper);
    class_addpointer(pdClass, pointerWrapper);
    class_addlist(pdClass, listWrapper);
    class_addanything(pdClass, anythingWrapper);
    class_sethelpsymbol(pdClass, gensym("ofelia-object"));
}
