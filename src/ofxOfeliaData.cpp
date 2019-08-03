#include "ofxOfeliaData.h"
#include "ofxOfeliaDefine.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cctype>
#ifdef _WIN32
#include <direct.h>
#include <io.h>
#define CHDIR _chdir
#else
#include <unistd.h>
#define CHDIR chdir
#endif
#include <errno.h>

t_symbol *ofxOfeliaData::getUniqueSym()
{
    char buf[MAXPDSTRING];
    std::snprintf(buf, MAXPDSTRING, "__.x%lx.c", reinterpret_cast<unsigned long>(this));
    hasUniqueSym = true;
    return gensym(buf);
}

void ofxOfeliaData::initSym()
{
    if (*sym->s_name && pd_findbyclass(sym, ofxOfeliaDefine::pdClass) == nullptr) return;
    sym = getUniqueSym();
}

void ofxOfeliaData::bindSym()
{
    pd_bind(&ob.ob_pd, sym);
}

void ofxOfeliaData::unbindSym()
{
    pd_unbind(&ob.ob_pd, sym);
}

void ofxOfeliaData::argParse(t_symbol *s, int argc, t_atom *argv, bool define)
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
        if (define && !std::strcmp(flag, "-k"))
            shouldKeep = true;
        else if (!ioAdded && !std::strncmp(flag, "-c", 2) && (std::strlen(flag) == 4) &&
                 std::isdigit(flag[2]) && std::isdigit(flag[3]) && (flag[2] - '0'))
        {
            io.newControlIO(flag[2] - '0', flag[3] - '0');
            ioAdded = true;
        }
        else if (!ioAdded && !std::strncmp(flag, "-s", 2) && (std::strlen(flag) == 4) &&
                 std::isdigit(flag[2]) && std::isdigit(flag[3]) && (flag[2] - '0'))
        {
            io.newSignalIO(flag[2] - '0', flag[3] - '0');
            ioAdded = true;
        }
        else
            post("warning: ofelia %s: unknown flag '%s'", s->s_name, flag);
        argc--; argv++;
    }
    if (!ioAdded) io.newControlIO(1, 1); /* default io setting (-c11) */
    if (argc && argv->a_type == A_SYMBOL)
    {
        if (define && *argv->a_w.w_symbol->s_name == ';')
        {
            isDirectMode = true;
            goto directMode;
        }
        sym = argv->a_w.w_symbol;
        argc--; argv++;
    }
    if (argc) /* handle inlet arguments */
    {
        if (define && argv->a_type == A_SYMBOL &&
            *argv->a_w.w_symbol->s_name == ';')
        {
            isDirectMode = true;
            goto directMode;
        }
        if (isSignalObject)
        {
            signal.f = atom_getfloat(argv);
            argc--; argv++;
        }
        else
        {
            for (int i = 0; i < io.numInlets - 1; ++i)
            {
                if (!argc) break;
                if (argv->a_type == A_FLOAT)
                    io.av[i] = *argv;
                else if (argv->a_type == A_SYMBOL)
                {
                    if (define && *argv->a_w.w_symbol->s_name == ';')
                    {
                        isDirectMode = true;
                        goto directMode;
                    }
                    io.av[i] = *argv;
                }
                else break;
                argc--; argv++;
            }
        }
    }
    if (argc)
    {
        if (define && argv->a_type == A_SYMBOL &&
            *argv->a_w.w_symbol->s_name == ';')
        {
            isDirectMode = true;
            goto directMode;
        }
        /* skip posting the error when trying to set the module name with unset $1 argument */
        if (!(argv->a_type == A_FLOAT && argv->a_w.w_float == 0))
        {
            post("warning: ofelia %s: ignoring extra argument: ", s->s_name);
            while (argc)
            {
                if (argv->a_type == A_FLOAT)
                    postfloat(argv->a_w.w_float);
                else if (argv->a_type == A_SYMBOL)
                {
                    if (define && *argv->a_w.w_symbol->s_name == ';')
                    {
                        endpost();
                        isDirectMode = true;
                        goto directMode;
                    }
                    poststring(argv->a_w.w_symbol->s_name);
                }
                argc--; argv++;
            }
            endpost();
        }
    }
directMode:
    if (isSignalObject)
        io.addSignalIO();
    else
        io.addControlIO();
    if (!define && !ioAdded)
        symbolinlet_new(&ob, &sym);
    if (isDirectMode)
    {
        initSym();
        bindSym();
        lua.doArgs(--argc, ++argv);
        return;
    }
    binbuf = binbuf_new();
    if (define) initSym();
    bindSym();
}

ofxOfeliaData *ofxOfeliaData::getPtr(t_symbol *s)
{
    if (*s->s_name) /* named ofelia object */
    {
        ofxOfeliaData *x = reinterpret_cast<ofxOfeliaData *>(pd_findbyclass(s, ofxOfeliaDefine::pdClass));
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
