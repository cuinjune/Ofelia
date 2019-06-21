#include "ofxOfeliaData.h"
#include "ofxOfeliaDefine.h"
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

t_symbol *ofxOfeliaData::getUniqueSym()
{
    char buf[MAXPDSTRING];
    snprintf(buf, MAXPDSTRING, "#%lx", reinterpret_cast<unsigned long>(this));
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

void ofxOfeliaData::argParse(int argc, t_atom *argv, t_symbol *s, bool define)
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
