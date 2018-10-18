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

#include "ofeliaExists.h"
#include "ofeliaDefine.h"
#include <new>

t_class *ofeliaExists::pdClass;

void *ofeliaExists::newMethod(t_symbol *s, int argc, t_atom *argv)
{
    client.data.sym = &s_;
    while (argc && argv->a_type == A_SYMBOL &&
           *argv->a_w.w_symbol->s_name == '-')
    {
        const char *flag = argv->a_w.w_symbol->s_name;
        error("ofelia exists: unknown flag '%s'", flag);
        argc--; argv++;
    }
    if (argc && argv->a_type == A_SYMBOL)
    {
        client.data.sym = argv->a_w.w_symbol;
        argc--; argv++;
    }
    if (argc)
    {
        post("warning: ofelia exists ignoring extra argument: ");
        postatom(argc, argv); endpost();
    }
    symbolinlet_new(&client.data.ob, &client.data.sym);
    outlet_new(&client.data.ob, &s_float);
    return this;
}

void ofeliaExists::bangMethod()
{
    ofeliaData *y = reinterpret_cast<ofeliaData *>(pd_findbyclass(client.data.sym, ofeliaDefine::pdClass));
    outlet_float(client.data.ob.ob_outlet, static_cast<t_float>(y != nullptr));
}

void *ofeliaExists::newWrapper(t_symbol *s, int argc, t_atom *argv)
{
    ofeliaExists *x = reinterpret_cast<ofeliaExists *>(pd_new(pdClass));
    new (x) ofeliaExists();
    return x->newMethod(s, argc, argv);
}

void ofeliaExists::bangWrapper(ofeliaExists *x)
{
    x->bangMethod();
}

void ofeliaExists::freeWrapper(ofeliaExists *x)
{
    x->~ofeliaExists();
}

void ofeliaExists::setup()
{
    pdClass = class_new(gensym("ofelia exists"),
                        reinterpret_cast<t_newmethod>(newWrapper),
                        reinterpret_cast<t_method>(freeWrapper),
                        sizeof(ofeliaExists), 0, A_GIMME, 0);
    class_addbang(pdClass, bangWrapper);
    class_sethelpsymbol(pdClass, gensym("ofelia-object"));
}

