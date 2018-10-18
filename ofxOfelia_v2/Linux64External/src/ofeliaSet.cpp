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

#include "ofeliaSet.h"
#include <new>

t_class *ofeliaSet::pdClass;

void *ofeliaSet::newMethod(t_symbol *s, int argc, t_atom *argv)
{
    client.data.argParse(argc, argv, gensym("set"), false);
    return this;
}

void ofeliaSet::bangMethod()
{
    client.bangMethod();
}

void ofeliaSet::floatMethod(t_floatarg f)
{
    client.floatMethod(f);
}

void ofeliaSet::symbolMethod(t_symbol *s)
{
    client.symbolMethod(s);
}

void ofeliaSet::pointerMethod(t_gpointer *p)
{
    client.pointerMethod(p);
}

void ofeliaSet::listMethod(t_symbol *s, int argc, t_atom *argv)
{
    client.listMethod(s, argc, argv);
}

void ofeliaSet::anythingMethod(t_symbol *s, int argc, t_atom *argv)
{
    client.setVariableByArgs(s, argc, argv);
}

void ofeliaSet::dspMethod(t_signal **sp)
{
    client.dspMethod(sp);
}

void ofeliaSet::freeMethod()
{
    client.freeMethod();
}

void *ofeliaSet::newWrapper(t_symbol *s, int argc, t_atom *argv)
{
    ofeliaSet *x = reinterpret_cast<ofeliaSet *>(pd_new(pdClass));
    new (x) ofeliaSet();
    return x->newMethod(s, argc, argv);
}

void ofeliaSet::bangWrapper(ofeliaSet *x)
{
    x->bangMethod();
}

void ofeliaSet::floatWrapper(ofeliaSet *x, t_floatarg f)
{
    x->floatMethod(f);
}

void ofeliaSet::symbolWrapper(ofeliaSet *x, t_symbol *s)
{
    x->symbolMethod(s);
}

void ofeliaSet::pointerWrapper(ofeliaSet *x, t_gpointer *p)
{
    x->pointerMethod(p);
}

void ofeliaSet::listWrapper(ofeliaSet *x, t_symbol *s, int argc, t_atom *argv)
{
    x->listMethod(s, argc, argv);
}

void ofeliaSet::anythingWrapper(ofeliaSet *x, t_symbol *s, int argc, t_atom *argv)
{
    x->anythingMethod(s, argc, argv);
}

void ofeliaSet::dspWrapper(ofeliaSet *x, t_signal **sp)
{
    x->dspMethod(sp);
}

void ofeliaSet::freeWrapper(ofeliaSet *x)
{
    x->freeMethod();
    x->~ofeliaSet();
}

void ofeliaSet::setup()
{
    pdClass = class_new(gensym("ofelia set"),
                        reinterpret_cast<t_newmethod>(newWrapper),
                        reinterpret_cast<t_method>(freeWrapper),
                        sizeof(ofeliaSet), 0, A_GIMME, 0);
    class_addmethod(pdClass, reinterpret_cast<t_method>(dspWrapper), gensym("dsp"), A_CANT, 0);
    CLASS_MAINSIGNALIN(pdClass, ofeliaSet, client.data.signal.f);
    class_addbang(pdClass, bangWrapper);
    class_addfloat(pdClass, floatWrapper);
    class_addsymbol(pdClass, symbolWrapper);
    class_addpointer(pdClass, pointerWrapper);
    class_addlist(pdClass, listWrapper);
    class_addanything(pdClass, anythingWrapper);
    class_sethelpsymbol(pdClass, gensym("ofelia-object"));
}

