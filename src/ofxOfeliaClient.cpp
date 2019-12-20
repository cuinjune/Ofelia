#include "ofxOfeliaClient.h"
#include <cstring>

bool ofxOfeliaClient::isDataValid()
{
    ofxOfeliaData *y = ofxOfeliaData::getPtr(data.sym);
    if (y == nullptr) return false;
    data.lua.isChunkRun = y->lua.isChunkRun;
    return true;
}

void ofxOfeliaClient::bangMethod()
{
    if (!isDataValid()) return;
    if (data.io.hasMultiControlInlets)
    {
        int ac = data.io.numInlets - 1;
        t_atom *av = static_cast<t_atom *>(getbytes(sizeof(t_atom) * ac));
        data.io.doList(ac, av);
        freebytes(av, sizeof(t_atom) * ac);
        return;
    }
    data.lua.doFunction(&s_bang);
}

void ofxOfeliaClient::floatMethod(t_floatarg f)
{
    if (!isDataValid()) return;
    if (data.io.hasMultiControlInlets)
    {
        int ac = data.io.numInlets;
        t_atom *av = static_cast<t_atom *>(getbytes(sizeof(t_atom) * ac));
        SETFLOAT(av, f);
        data.io.doList(ac, av);
        freebytes(av, sizeof(t_atom) * ac);
        return;
    }
    data.lua.doFunction(&s_float, f);
}

void ofxOfeliaClient::symbolMethod(t_symbol *s)
{
    if (!isDataValid()) return;
    if (data.io.hasMultiControlInlets)
    {
        int ac = data.io.numInlets;
        t_atom *av = static_cast<t_atom *>(getbytes(sizeof(t_atom) * ac));
        SETSYMBOL(av, s);
        data.io.doList(ac, av);
        freebytes(av, sizeof(t_atom) * ac);
        return;
    }
    data.lua.doFunction(&s_symbol, s);
}

void ofxOfeliaClient::pointerMethod(t_gpointer *p)
{
    if (!isDataValid()) return;
    if (data.io.hasMultiControlInlets)
    {
        int ac = data.io.numInlets;
        t_atom *av = static_cast<t_atom *>(getbytes(sizeof(t_atom) * ac));
        SETPOINTER(av, p);
        data.io.doList(ac, av);
        freebytes(av, sizeof(t_atom) * ac);
        return;
    }
    data.lua.doFunction(&s_pointer, p);
}

void ofxOfeliaClient::listMethod(t_symbol *s, int argc, t_atom *argv)
{
    if (!isDataValid()) return;
    if (!argc)
    {
        bangMethod();
        return;
    }
    if (argc == 1)
    {
        if (argv->a_type == A_FLOAT)
            floatMethod(argv->a_w.w_float);
        else if (argv->a_type == A_SYMBOL)
            symbolMethod(argv->a_w.w_symbol);
        else if (argv->a_type == A_POINTER)
            pointerMethod(argv->a_w.w_gpointer);
        return;
    }
    if (data.io.hasMultiControlInlets)
    {
        int ac = data.io.numInlets + argc - 1;
        t_atom *av = static_cast<t_atom *>(getbytes(sizeof(t_atom) * ac));
        std::memcpy(av, argv, sizeof(t_atom) * argc);
        data.io.doList(ac, av);
        freebytes(av, sizeof(t_atom) * ac);
        return;
    }
    data.lua.doFunction(&s_list, argc, argv);
}

void ofxOfeliaClient::getVariableByArgs(t_symbol *s, int argc, t_atom *argv)
{
    if (!isDataValid()) return;
    data.lua.getVariableByArgs(s, argc, argv);
}

void ofxOfeliaClient::setVariableByArgs(t_symbol *s, int argc, t_atom *argv)
{
    if (!isDataValid()) return;
    data.lua.setVariableByArgs(s, argc, argv);
}

void ofxOfeliaClient::dspMethod(t_signal **sp)
{
    if (!data.isSignalObject) return;
    if (!isDataValid()) return;
    data.signal.addDsp(sp);
}

void ofxOfeliaClient::freeMethod()
{
    if (!data.isSignalObject) data.io.freeControlIO();
    else data.io.freeSignalIO();
}
