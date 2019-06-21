#include "ofxOfeliaCreator.h"
#include "ofxOfeliaDefine.h"
#include "ofxOfeliaGet.h"
#include "ofxOfeliaSet.h"
#include "ofxOfeliaExists.h"
#include <string.h>

void *ofxOfeliaCreator::newWrapper(t_symbol *s, int argc, t_atom *argv)
{
    if (!argc || argv[0].a_type != A_SYMBOL)
        pd_this->pd_newest = static_cast<t_pd *>(ofxOfeliaDefine::newWrapper(s, argc, argv));
    else
    {
        const char *str = argv[0].a_w.w_symbol->s_name;
        if (!strcmp(str, "d") || !strcmp(str, "define"))
            pd_this->pd_newest = static_cast<t_pd *>(ofxOfeliaDefine::newWrapper(s, argc - 1, argv + 1));
        else if (!strcmp(str, "f") || !strcmp(str, "function"))
            pd_this->pd_newest = static_cast<t_pd *>(ofxOfeliaDefine::newWrapper_function(s, argc - 1, argv + 1));
        else if (!strcmp(str, "get"))
            pd_this->pd_newest = static_cast<t_pd *>(ofxOfeliaGet::newWrapper(s, argc - 1, argv + 1));
        else if (!strcmp(str, "set"))
            pd_this->pd_newest = static_cast<t_pd *>(ofxOfeliaSet::newWrapper(s, argc - 1, argv + 1));
        else if (!strcmp(str, "e") || !strcmp(str, "exists"))
            pd_this->pd_newest = static_cast<t_pd *>(ofxOfeliaExists::newWrapper(s, argc - 1, argv + 1));
        else
        {
            error("list %s: unknown function", str);
            pd_this->pd_newest = 0;
        }
    }
    return pd_this->pd_newest;
}

void ofxOfeliaCreator::setup()
{
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper), gensym("ofelia"), A_GIMME, 0);
}
