#include "ofxOfeliaEmbClass.h"
#include "ofxOfeliaDefine.h"
#include <new>
#include <sstream>

#define FORMAT_RELEASE_SCRIPT(methodName) \
ss << \
"function M.new()\n" \
"  ofWindow.addListener(\"exit\", this)\n" \
"end\n" \
"function M.free()\n" \
"  M." << varName->s_name << ':' << methodName << "()\n" \
"  M." << varName->s_name << " = nil\n" \
"  ofWindow.removeListener(\"exit\", this)\n" \
"end\n" \
"function M.exit()\n" \
"  M." << varName->s_name << ':' << methodName << "()\n" \
"end\n"; \

t_symbol *ofxOfeliaEmbClass::getClassAdditionalScript_default(t_symbol *varName)
{
    std::ostringstream ss;
    ss <<
    "function M.free()\n"
    "  M." << varName->s_name << " = nil\n"
    "end";
    return gensym(ss.str().c_str());
}

t_symbol *ofxOfeliaEmbClass::getClassAdditionalScript_clear(t_symbol *varName)
{
    std::ostringstream ss;
    FORMAT_RELEASE_SCRIPT("clear");
    return gensym(ss.str().c_str());
}

t_symbol *ofxOfeliaEmbClass::getClassAdditionalScript_close(t_symbol *varName)
{
    std::ostringstream ss;
    FORMAT_RELEASE_SCRIPT("close");
    return gensym(ss.str().c_str());
}

t_symbol *ofxOfeliaEmbClass::getClassAdditionalScript_unload(t_symbol *varName)
{
    std::ostringstream ss;
    FORMAT_RELEASE_SCRIPT("unload");
    return gensym(ss.str().c_str());
}

t_symbol *ofxOfeliaEmbClass::getClassAdditionalScript_destroy(t_symbol *varName)
{
    std::ostringstream ss;
    ss <<
    "function M.free()\n"
    "  c(\"destroy\", a)\n"
    "end";
    return gensym(ss.str().c_str());
}

void *ofxOfeliaEmbClass::newMethod(t_symbol *s, int argc, t_atom *argv,
                                   ofxOfeliaEmbBase::getClassAdditionalScriptFuncPtr getClassAdditionalScriptFunc)
{
    return definePtr->embBase.classMapScript(s, argc, argv, getClassAdditionalScriptFunc);
}

void *ofxOfeliaEmbClass::newWrapper_default(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embClass.newMethod(s, argc, argv, getClassAdditionalScript_default);
}

void *ofxOfeliaEmbClass::newWrapper_clear(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embClass.newMethod(s, argc, argv, getClassAdditionalScript_clear);
}

void *ofxOfeliaEmbClass::newWrapper_close(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embClass.newMethod(s, argc, argv, getClassAdditionalScript_close);
}

void *ofxOfeliaEmbClass::newWrapper_unload(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embClass.newMethod(s, argc, argv, getClassAdditionalScript_unload);
}

void *ofxOfeliaEmbClass::newWrapper_destroy(t_symbol *s, int argc, t_atom *argv)
{
    ofxOfeliaDefine *x = reinterpret_cast<ofxOfeliaDefine *>(pd_new(ofxOfeliaDefine::pdClass));
    new (x) ofxOfeliaDefine();
    return x->embClass.newMethod(s, argc, argv, getClassAdditionalScript_destroy);
}

void ofxOfeliaEmbClass::setup()
{
    for (auto it = ofxOfeliaMaps::classMap.begin(); it != ofxOfeliaMaps::classMap.end(); ++it)
        class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_default),
                         gensym(it->first.c_str()), A_GIMME, 0);
    /* exceptions */
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_clear),
                     gensym("ofFbo"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_clear),
                     gensym("ofTexture"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_clear),
                     gensym("ofImage"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_clear),
                     gensym("ofMesh"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_clear),
                     gensym("ofVbo"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_clear),
                     gensym("ofPixels"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_clear),
                     gensym("ofPath"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_clear),
                     gensym("ofPolyline"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_clear),
                     gensym("ofXml"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_clear),
                     gensym("ofBuffer"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_clear),
                     gensym("ofURLFileLoader"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_close),
                     gensym("ofBaseSoundStream"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_close),
                     gensym("ofSerial"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_close),
                     gensym("ofSoundStream"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_close),
                     gensym("ofFile"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_close),
                     gensym("ofDirectory"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_close),
                     gensym("ofVideoGrabber"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_close),
                     gensym("ofVideoPlayer"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_unload),
                     gensym("ofBaseSoundPlayer"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_unload),
                     gensym("ofShader"), A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(newWrapper_destroy),
                     gensym("ofWindow"), A_GIMME, 0);
}
