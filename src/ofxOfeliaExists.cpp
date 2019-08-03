#include "ofxOfeliaExists.h"
#include "ofxOfeliaDefine.h"
#include <new>

t_class *ofxOfeliaExists::pdClass;

void *ofxOfeliaExists::newMethod(t_symbol *s, int argc, t_atom *argv)
{
    client.data.sym = &s_;
    while (argc && argv->a_type == A_SYMBOL &&
           *argv->a_w.w_symbol->s_name == '-')
    {
        const char *flag = argv->a_w.w_symbol->s_name;
        post("warning: ofelia %s: unknown flag '%s'", s->s_name, flag);
        argc--; argv++;
    }
    if (argc && argv->a_type == A_SYMBOL)
    {
        client.data.sym = argv->a_w.w_symbol;
        argc--; argv++;
    }
    if (argc)
    {
        post("warning: ofelia %s ignoring extra argument: ", s->s_name);
        postatom(argc, argv); endpost();
    }
    symbolinlet_new(&client.data.ob, &client.data.sym);
    outlet_new(&client.data.ob, &s_float);
    return this;
}

void ofxOfeliaExists::bangMethod()
{
    ofxOfeliaData *y = reinterpret_cast<ofxOfeliaData *>(pd_findbyclass(client.data.sym, ofxOfeliaDefine::pdClass));
    outlet_float(client.data.ob.ob_outlet, static_cast<t_float>(y != nullptr));
}

void *ofxOfeliaExists::newWrapper(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaExists *x = reinterpret_cast<ofxOfeliaExists *>(pd_new(pdClass));
    new (x) ofxOfeliaExists();
    return x->newMethod(s, argc, argv);
}

void ofxOfeliaExists::bangWrapper(ofxOfeliaExists *x)
{
    x->bangMethod();
}

void ofxOfeliaExists::freeWrapper(ofxOfeliaExists *x)
{
    x->~ofxOfeliaExists();
}

void ofxOfeliaExists::setup()
{
    pdClass = class_new(gensym("ofelia exists"),
                        reinterpret_cast<t_newmethod>(newWrapper),
                        reinterpret_cast<t_method>(freeWrapper),
                        sizeof(ofxOfeliaExists), 0, A_GIMME, 0);
    class_addbang(pdClass, bangWrapper);
    class_sethelpsymbol(pdClass, gensym("ofelia-object"));
}
