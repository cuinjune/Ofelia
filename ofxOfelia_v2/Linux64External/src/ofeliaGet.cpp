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

#include "ofeliaGet.h"
#include <new>

t_class *ofeliaGet::pdClass;

void *ofeliaGet::newMethod(t_symbol *s, int argc, t_atom *argv)
{
    client.data.argParse(argc, argv, gensym("get"), false);
    return this;
}

void ofeliaGet::bangMethod()
{
    client.bangMethod();
}

void ofeliaGet::floatMethod(t_floatarg f)
{
    client.floatMethod(f);
}

void ofeliaGet::symbolMethod(t_symbol *s)
{
    client.symbolMethod(s);
}

void ofeliaGet::pointerMethod(t_gpointer *p)
{
    client.pointerMethod(p);
}

void ofeliaGet::listMethod(t_symbol *s, int argc, t_atom *argv)
{
    client.listMethod(s, argc, argv);
}

void ofeliaGet::anythingMethod(t_symbol *s, int argc, t_atom *argv)
{
    client.getVariableByArgs(s, argc, argv);
}

void ofeliaGet::dspMethod(t_signal **sp)
{
    client.dspMethod(sp);
}

void ofeliaGet::freeMethod()
{
    client.freeMethod();
}

void *ofeliaGet::newWrapper(t_symbol *s, int argc, t_atom *argv)
{
    ofeliaGet *x = reinterpret_cast<ofeliaGet *>(pd_new(pdClass));
    new (x) ofeliaGet();
    return x->newMethod(s, argc, argv);
}

void ofeliaGet::bangWrapper(ofeliaGet *x)
{
    x->bangMethod();
}

void ofeliaGet::floatWrapper(ofeliaGet *x, t_floatarg f)
{
    x->floatMethod(f);
}

void ofeliaGet::symbolWrapper(ofeliaGet *x, t_symbol *s)
{
    x->symbolMethod(s);
}

void ofeliaGet::pointerWrapper(ofeliaGet *x, t_gpointer *p)
{
    x->pointerMethod(p);
}

void ofeliaGet::listWrapper(ofeliaGet *x, t_symbol *s, int argc, t_atom *argv)
{
    x->listMethod(s, argc, argv);
}

void ofeliaGet::anythingWrapper(ofeliaGet *x, t_symbol *s, int argc, t_atom *argv)
{
    x->anythingMethod(s, argc, argv);
}

void ofeliaGet::dspWrapper(ofeliaGet *x, t_signal **sp)
{
    x->dspMethod(sp);
}

void ofeliaGet::freeWrapper(ofeliaGet *x)
{
    x->freeMethod();
    x->~ofeliaGet();
}

void ofeliaGet::setup()
{
    pdClass = class_new(gensym("ofelia get"),
                        reinterpret_cast<t_newmethod>(newWrapper),
                        reinterpret_cast<t_method>(freeWrapper),
                        sizeof(ofeliaGet), 0, A_GIMME, 0);
    class_addmethod(pdClass, reinterpret_cast<t_method>(dspWrapper), gensym("dsp"), A_CANT, 0);
    CLASS_MAINSIGNALIN(pdClass, ofeliaGet, client.data.signal.f);
    class_addbang(pdClass, bangWrapper);
    class_addfloat(pdClass, floatWrapper);
    class_addsymbol(pdClass, symbolWrapper);
    class_addpointer(pdClass, pointerWrapper);
    class_addlist(pdClass, listWrapper);
    class_addanything(pdClass, anythingWrapper);
    class_sethelpsymbol(pdClass, gensym("ofelia-object"));
}

