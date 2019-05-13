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

#include "ofeliaClient.h"
#include <string.h>

bool ofeliaClient::isDataValid()
{
    ofeliaData *y = ofeliaData::getPtr(data.sym);
    if (y == nullptr) return false;
    data.lua.isChunkRun = y->lua.isChunkRun;
    return true;
}

void ofeliaClient::bangMethod()
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

void ofeliaClient::floatMethod(t_floatarg f)
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

void ofeliaClient::symbolMethod(t_symbol *s)
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

void ofeliaClient::pointerMethod(t_gpointer *p)
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

void ofeliaClient::listMethod(t_symbol *s, int argc, t_atom *argv)
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
        memcpy(av, argv, sizeof(t_atom) * argc);
        data.io.doList(ac, av);
        freebytes(av, sizeof(t_atom) * ac);
        return;
    }
    data.lua.doFunction(&s_list, argc, argv);
}

void ofeliaClient::getVariableByArgs(t_symbol *s, int argc, t_atom *argv)
{
    if (!isDataValid()) return;
    data.lua.getVariableByArgs(s, argc, argv);
}

void ofeliaClient::setVariableByArgs(t_symbol *s, int argc, t_atom *argv)
{
    if (!isDataValid()) return;
    data.lua.setVariableByArgs(s, argc, argv);
}

void ofeliaClient::dspMethod(t_signal **sp)
{
    if (!data.isSignalObject) return;
    if (!isDataValid()) return;
    data.signal.addDsp(sp);
}

void ofeliaClient::freeMethod()
{
    if (!data.isSignalObject) data.io.freeControlIO();
    else data.io.freeSignalIO();
}

