#pragma once

#include "m_pd.h"
#include "ofxOfeliaMaps.h"
#include <vector>

class ofxOfeliaDefine;
class ofxOfeliaEmbBase
{
public:
    ofxOfeliaEmbBase(ofxOfeliaDefine *definePtr)
    :definePtr(definePtr){};
    typedef t_symbol *(*getClassAdditionalScriptFuncPtr)(t_symbol *varName);
    void *functionMapScript(t_symbol *s, int argc, t_atom *argv);
    void *classMapScript(t_symbol *s, int argc, t_atom *argv,
                         getClassAdditionalScriptFuncPtr getClassAdditionalScriptFunc);
private:
    t_symbol *getParameterScript(const ofxOfeliaMaps::Types &types, const int numInlets);
    t_symbol *getFunctionScript(t_symbol *funcName,
                                const ofxOfeliaMaps::Types &types, const int numInlets);
    t_symbol *getClassScript(t_symbol *className, t_symbol *varName);
    t_symbol *getClassMethodScript(t_symbol *methodName);
    t_symbol *getClassMethodScript(t_symbol *varName, t_symbol *requireName, t_symbol *methodName,
                                   const ofxOfeliaMaps::Types &types, const int numInlets);
    t_symbol *getClassVarName(t_symbol *className);
    t_symbol *getClassModuleName(t_symbol *className, int argc, t_atom *argv);
    t_symbol *getClassRequireName(t_symbol *className, int argc, t_atom *argv);
    t_symbol *getClassMethodName(t_symbol *className, int argc, t_atom *argv);
    void getDefaultFunctionArgs(std::vector<t_atom> &args, const ofxOfeliaMaps::TypeSet &typeSet);
    bool checkFunctionArgTypes(std::vector<t_atom> &args, const ofxOfeliaMaps::Types &types);
    void appendArgs(std::vector<t_atom> &args, int argc, t_atom *argv);
    void addFloatArg(std::vector<t_atom> &args, t_floatarg f);
    void addSymbolArg(std::vector<t_atom> &args, t_symbol *s);
    void setFloatArg(std::vector<t_atom> &args, const size_t index, t_floatarg f);
    void setSymbolArg(std::vector<t_atom> &args, const size_t index, t_symbol *s);
    void classMethodArgError(t_symbol *className, t_symbol *methodName,
                             const ofxOfeliaMaps::TypeSet &typeSet);
    void classArgTypeError(t_symbol *className, t_symbol *argType);
    void classMethodNotFoundError(t_symbol *className, t_symbol *methodName,
                                  const ofxOfeliaMaps::ClassData &classData);
    bool getClassCommonArgs(t_symbol *className, int argc, t_atom *argv,
                            const ofxOfeliaMaps::ClassData &classData, std::vector<t_atom> &args);
    bool getClassMethodCommonArgs(t_symbol *className, t_symbol *methodName, int argc, t_atom *argv,
                                  const ofxOfeliaMaps::FunctionData &functionData, std::vector<t_atom> &args,
                                  ofxOfeliaMaps::Types &types, int &numInlets);
    bool getClassMethodCommonArgs(t_symbol *className, t_symbol *methodName, int argc, t_atom *argv,
                                  const ofxOfeliaMaps::ClassData &classData, std::vector<t_atom> &args,
                                  ofxOfeliaMaps::Types &types, int &numInlets);
    ofxOfeliaDefine *definePtr; /* pointer to define */
};
