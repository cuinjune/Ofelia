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

#include "ofeliaData.h"
#include "ofeliaDefine.h"
#include <iostream>
#include <cstring>
#ifdef _WIN32
#include <direct.h>
#include <io.h>
#define CHDIR _chdir
#else
#include <unistd.h>
#define CHDIR chdir
#endif
#include <errno.h>

t_symbol *ofeliaData::getUniqueSym()
{
    char buf[MAXPDSTRING];
    snprintf(buf, MAXPDSTRING, "#%lx", reinterpret_cast<unsigned long>(this));
    return gensym(buf);
}

void ofeliaData::initSym()
{
    if (*sym->s_name && pd_findbyclass(sym, ofeliaDefine::pdClass) == nullptr) return;
    sym = getUniqueSym();
}

void ofeliaData::bindSym()
{
    pd_bind(&ob.ob_pd, sym);
}

void ofeliaData::unbindSym()
{
    pd_unbind(&ob.ob_pd, sym);
}

void ofeliaData::argParse(int argc, t_atom *argv, t_symbol *s, bool define)
{
    if (define)
    {
        canvas = canvas_getcurrent();
        CHDIR(canvas_getdir(canvas)->s_name);
    }
    bool ioAdded = false;
    while (argc && argv->a_type == A_SYMBOL &&
           *argv->a_w.w_symbol->s_name == '-')
    {
        const char *flag = argv->a_w.w_symbol->s_name;
        if (define && !strcmp(flag, "-k"))
            shouldKeep = true;
        else if (!ioAdded && !strncmp(flag, "-c", 2) && (strlen(flag) == 4) &&
                 isdigit(flag[2]) && isdigit(flag[3]) && (flag[2] - '0'))
        {
            io.newControlIO(flag[2] - '0', flag[3] - '0');
            ioAdded = true;
        }
        else if (!ioAdded && !strncmp(flag, "-s", 2) && (strlen(flag) == 4) &&
                 isdigit(flag[2]) && isdigit(flag[3]) && (flag[2] - '0'))
        {
            io.newSignalIO(flag[2] - '0', flag[3] - '0');
            ioAdded = true;
        }
        else
            error("ofelia %s: unknown flag '%s'", s->s_name, flag);
        argc--; argv++;
    }
    if (!ioAdded) io.newControlIO(1, 1); /* default io setting (-c11) */
    if (argc && argv->a_type == A_SYMBOL)
    {
        if (define && *argv->a_w.w_symbol->s_name == ';')
        {
            lua.doArgs(--argc, ++argv);
            return;
        }
        sym = argv->a_w.w_symbol;
        argc--; argv++;
    }
    if (argc && argv->a_type == A_FLOAT)
    {
        if (isSignalObject)
        {
            signal.f = argv->a_w.w_float;
            argc--; argv++;
        }
        else
        {
            for (int i = 0; i < io.numInlets - 1; ++i)
            {
                if (argc && argv->a_type == A_FLOAT)
                {
                    io.fv[i] = argv->a_w.w_float;
                    argc--; argv++;
                }
                else
                    break;
            }
        }
    }
    if (argc)
    {
        if (define && argv->a_type == A_SYMBOL &&
            *argv->a_w.w_symbol->s_name == ';')
        {
            lua.doArgs(--argc, ++argv);
            return;
        }
        if (!(argv->a_type == A_FLOAT && argv->a_w.w_float == 0)) /* skip posting errors for dollar args */
        {
            post("warning: ofelia %s ignoring extra argument: ", s->s_name);
            postatom(argc, argv); endpost();
        }
    }
    if (!define && !ioAdded)
        symbolinlet_new(&ob, &sym);
    if (isDirectMode) return;
    binbuf = binbuf_new();
    if (define) initSym();
    bindSym();
}

ofeliaData *ofeliaData::getPtr(t_symbol *s)
{
    if (*s->s_name) /* named ofelia object */
    {
        ofeliaData *x = reinterpret_cast<ofeliaData *>(pd_findbyclass(s, ofeliaDefine::pdClass));
        if (x)
            return x;
        else
        {
            error("ofelia: couldn't find '%s'", s->s_name);
            return nullptr;
        }
    }
    return nullptr;
}

