/*==============================================================================
 ofelia: OpenFrameworks as an External Library for Interactive Applications
 
 Copyright (c) 2018 Zack Lee <cuinjune@gmail.com>
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
 
 See https://github.com/cuinjune/ofxOfelia for documentation
 ==============================================================================*/

#include "ofeliaDefine.h"
#include <new>
#include <string.h>

t_class *ofeliaDefine::pdClass;

void *ofeliaDefine::newMethod(t_symbol *s, int argc, t_atom *argv)
{
    t_symbol *asym = gensym("#A");
    data.initCanvas();
    data.argParse(argc, argv, gensym("define"), true);
    data.initTextBuf();
    data.initSym();
    pd_bind(&data.ob.ob_pd, data.sym);
    /* bashily unbind #A -- this would create garbage if #A were
     multiply bound but we believe in this context it's at most
     bound to whichever ofelia_define or array was created most recently */
    asym->s_thing = 0;
    /* and now bind #A to us to receive following messages in the
     saved file or copy buffer */
    pd_bind(&data.ob.ob_pd, asym);
    return this;
}

void ofeliaDefine::bangMethod()
{
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

void ofeliaDefine::floatMethod(t_floatarg f)
{
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
    if (data.isSignalObject) data.signal.f = f;
}

void ofeliaDefine::symbolMethod(t_symbol *s)
{
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

void ofeliaDefine::pointerMethod(t_gpointer *p)
{
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

void ofeliaDefine::listMethod(t_symbol *s, int argc, t_atom *argv)
{
    if (data.io.hasMultiControlInlets)
    {
        int ac = data.io.numInlets + argc - 1;
        t_atom *av = static_cast<t_atom *>(getbytes(sizeof(t_atom) * ac));
        memcpy(av, argv, sizeof(t_atom) * argc);
        data.io.doList(ac, av);
        freebytes(av, sizeof(t_atom) * ac);
        return;
    }
    data.lua.doFunction(&s_list, argc, argv);
}

void ofeliaDefine::anythingMethod(t_symbol *s, int argc, t_atom *argv)
{
    data.lua.setVariableByArgs(s, argc, argv);
}

void ofeliaDefine::clearMethod()
{
    if (data.isDirectMode) return;
    data.lua.doFreeFunction();
    binbuf_clear(data.binbuf);
    data.textBuf.senditup();
}

void ofeliaDefine::setMethod(t_symbol *s, int argc, t_atom *argv)
{
    if (data.isDirectMode) return;
    binbuf_restore(data.binbuf, argc, argv);
    data.textBuf.senditup();
    data.lua.doText();
}

void ofeliaDefine::saveMethod(t_object *ob, t_binbuf *bb)
{
    binbuf_addv(bb, const_cast<char *>("ssff"), &s__X, gensym("obj"),
                static_cast<float>(ob->te_xpix),
                static_cast<float>(ob->te_ypix));
    binbuf_addbinbuf(bb, ob->ob_binbuf);
    binbuf_addsemi(bb);
    if (data.shouldKeep)
    {
        binbuf_addv(bb, const_cast<char *>("ss"), gensym("#A"), gensym("set"));
        binbuf_addbinbuf(bb, data.binbuf);
        binbuf_addsemi(bb);
    }
    obj_saveformat(ob, bb);
}

void ofeliaDefine::dspMethod(t_signal **sp)
{
    if (!data.isSignalObject) return;
    data.signal.addDsp(sp);
}

/* notification from GUI that we've been updated */
void ofeliaDefine::notifyMethod()
{
    if (data.isDirectMode) return;
    data.textBuf.senditup();
    data.lua.doText();
}

void ofeliaDefine::freeMethod()
{
    data.lua.doFreeFunction();
    data.textBuf.free();
    pd_unbind(&data.ob.ob_pd, data.sym);
    if (!data.isSignalObject) data.io.freeControlIO();
    else data.io.freeSignalIO();
}

void *ofeliaDefine::newWrapper(t_symbol *s, int argc, t_atom *argv)
{
    ofeliaDefine *x = reinterpret_cast<ofeliaDefine *>(pd_new(pdClass));
    new (x) ofeliaDefine();
    return x->newMethod(s, argc, argv);
}

void ofeliaDefine::bangWrapper(ofeliaDefine *x)
{
    x->bangMethod();
}

void ofeliaDefine::floatWrapper(ofeliaDefine *x, t_floatarg f)
{
    x->floatMethod(f);
}

void ofeliaDefine::symbolWrapper(ofeliaDefine *x, t_symbol *s)
{
    x->symbolMethod(s);
}

void ofeliaDefine::pointerWrapper(ofeliaDefine *x, t_gpointer *p)
{
    x->pointerMethod(p);
}

void ofeliaDefine::listWrapper(ofeliaDefine *x, t_symbol *s, int argc, t_atom *argv)
{
    x->listMethod(s, argc, argv);
}

void ofeliaDefine::anythingWrapper(ofeliaDefine *x, t_symbol *s, int argc, t_atom *argv)
{
    x->anythingMethod(s, argc, argv);
}

void ofeliaDefine::clearWrapper(ofeliaDefine *x)
{
    x->clearMethod();
}

void ofeliaDefine::setWrapper(ofeliaDefine *x, t_symbol *s, int argc, t_atom *argv)
{
    x->setMethod(s, argc, argv);
}

void ofeliaDefine::saveWrapper(t_gobj *z, t_binbuf *bb)
{
    ofeliaDefine *x = reinterpret_cast<ofeliaDefine *>(z);
    x->saveMethod(&x->data.ob, bb);
}

void ofeliaDefine::dspWrapper(ofeliaDefine *x, t_signal **sp)
{
    x->dspMethod(sp);
}

void ofeliaDefine::notifyWrapper(ofeliaDefine *x)
{
    x->notifyMethod();
}

void ofeliaDefine::openWrapper(ofeliaData *x)
{
    x->textBuf.openMethod();
}

void ofeliaDefine::closeWrapper(ofeliaData *x)
{
    x->textBuf.closeMethod();
}

void ofeliaDefine::addLineWrapper(ofeliaData *x, t_symbol *s, int argc, t_atom *argv)
{
    x->textBuf.addLineMethod(s, argc, argv);
}

void ofeliaDefine::readWrapper(ofeliaData *x, t_symbol *s, int argc, t_atom *argv)
{
    x->textBuf.readMethod(s, argc, argv);
}

void ofeliaDefine::writeWrapper(ofeliaData *x, t_symbol *s, int argc, t_atom *argv)
{
    x->textBuf.writeMethod(s, argc, argv);
}

void ofeliaDefine::freeWrapper(ofeliaDefine *x)
{
    x->freeMethod();
    x->~ofeliaDefine();
}

void ofeliaDefine::setup()
{
    pdClass = class_new(gensym("ofelia define"),
                        reinterpret_cast<t_newmethod>(newWrapper),
                        reinterpret_cast<t_method>(freeWrapper),
                        sizeof(ofeliaDefine), 0, A_GIMME, 0);
    class_addmethod(pdClass, reinterpret_cast<t_method>(openWrapper),
                    gensym("click"), A_NULL, 0);
    class_addmethod(pdClass, reinterpret_cast<t_method>(closeWrapper),
                    gensym("close"), A_NULL, 0);
    class_addmethod(pdClass, reinterpret_cast<t_method>(addLineWrapper),
                    gensym("addline"), A_GIMME, 0);
    class_addmethod(pdClass, reinterpret_cast<t_method>(dspWrapper), gensym("dsp"), A_CANT, 0);
    class_addmethod(pdClass, reinterpret_cast<t_method>(notifyWrapper),
                    gensym("notify"), A_NULL, 0);
    class_addmethod(pdClass, reinterpret_cast<t_method>(setWrapper),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(pdClass, reinterpret_cast<t_method>(clearWrapper),
                    gensym("clear"), A_NULL, 0);
    class_addmethod(pdClass, reinterpret_cast<t_method>(writeWrapper),
                    gensym("write"), A_GIMME, 0);
    class_addmethod(pdClass, reinterpret_cast<t_method>(readWrapper),
                    gensym("read"), A_GIMME, 0);
    class_setsavefn(pdClass, saveWrapper);
    CLASS_MAINSIGNALIN(pdClass, ofeliaDefine, data.signal.f);
    class_addbang(pdClass, bangWrapper);
    class_addfloat(pdClass, floatWrapper);
    class_addsymbol(pdClass, symbolWrapper);
    class_addpointer(pdClass, pointerWrapper);
    class_addlist(pdClass, listWrapper);
    class_addanything(pdClass, anythingWrapper);
    class_sethelpsymbol(pdClass, gensym("ofelia-object"));
}

