#include "ofxOfeliaEmbBase.h"
#include "ofxOfeliaDefine.h"
#include "lua.hpp"
#include <utility>
#include <string>
#include <cstdio>
#include <cctype>
#include <algorithm>
#include <iterator>
#include <new>
#include <sstream>
#include <locale>

void *ofxOfeliaEmbBase::functionMapScript(t_symbol *s, int argc, t_atom *argv)
{
    definePtr->data.isEmbedded = true;
    definePtr->data.embName = s;
    t_symbol *funcName = s;
    const auto &functionData = ofxOfeliaMaps::functionMap[funcName->s_name];
    std::vector<t_atom> args; ofxOfeliaMaps::Types types; int numInlets;
    if (!getClassMethodCommonArgs(funcName, funcName, argc, argv, functionData, args, types, numInlets))
        return 0;
    addSymbolArg(args, getFunctionScript(funcName, types, numInlets));
    return definePtr->function.newMethod(funcName, args.size(), args.data());
}

void *ofxOfeliaEmbBase::classMapScript(t_symbol *s, int argc, t_atom *argv,
                                       getClassAdditionalScriptFuncPtr getClassAdditionalScriptFunc)
{
    definePtr->data.isEmbedded = true;
    definePtr->data.embName = s;
    t_symbol *className = s;
    t_symbol *varName = getClassVarName(className);
    const auto &classData = ofxOfeliaMaps::classMap[className->s_name];
    std::vector<t_atom> args;
    /* class object (creator) */
    if (argc < 2)
    {
        if (!getClassCommonArgs(className, argc, argv, classData, args))
        return 0;
        addSymbolArg(args, getClassScript(className, varName));
        const size_t numMethods = classData.size() - 1; /* number of class methods */
        for (size_t i = 0; i < numMethods; ++i)
            addSymbolArg(args, getClassMethodScript(gensym(classData[i].first.c_str())));
        addSymbolArg(args, getClassAdditionalScriptFunc(varName));
        return definePtr->newMethod(className, args.size(), args.data());
    }
    /* class method object (getter) */
    t_symbol *requireName = getClassRequireName(className, argc, argv);
    t_symbol *methodName = getClassMethodName(className, argc, argv);
    if (!requireName || !methodName) return 0;
    ofxOfeliaMaps::Types types; int numInlets;
    if (!getClassMethodCommonArgs(className, methodName, argc, argv, classData, args, types, numInlets))
        return 0;
    addSymbolArg(args, getClassMethodScript(varName, requireName, methodName, types, numInlets));
    return definePtr->function.newMethod(className, args.size(), args.data());
}

t_symbol *ofxOfeliaEmbBase::getParameterScript(const ofxOfeliaMaps::Types &types, const int numInlets)
{
    const size_t numArgs = types.size();
    std::ostringstream ss;
    ss << '(';
    if (numArgs)
    {
        ss << 'a';
        if (numArgs != 1 || numInlets != 1)
        ss << "[1]";
        if (types[0] == LUA_TBOOLEAN)
        ss << " ~= 0";
        for (size_t i = 1; i < numArgs; ++i)
        {
            ss << ", a[" << i + 1 << ']';
            if (types[i] == LUA_TBOOLEAN)
            ss << " ~= 0";
        }
    }
    ss << ')';
    return gensym(ss.str().c_str());
}

t_symbol *ofxOfeliaEmbBase::getFunctionScript(t_symbol *funcName,
                                              const ofxOfeliaMaps::Types &types,
                                              const int numInlets)
{
    t_symbol *params = ofxOfeliaEmbBase::getParameterScript(types, numInlets);
    std::ostringstream ss;
    ss <<
    "local r = " << funcName->s_name << params->s_name << "\n"
    "return r";
    return gensym(ss.str().c_str());
}

t_symbol *ofxOfeliaEmbBase::getClassScript(t_symbol *className, t_symbol *varName)
{
    std::ostringstream ss;
    ss <<
    "local o = pdOutlet(this)\n"
    "M." << varName->s_name << " = " << className->s_name << "()\n"
    "local function c(n, a)\n"
    "  o:outletAnything(0, {n, __classMethod(M." << varName->s_name << ", n, a)})\n"
    "end";
    return gensym(ss.str().c_str());
}

t_symbol *ofxOfeliaEmbBase::getClassMethodScript(t_symbol *methodName)
{
    std::ostringstream ss;
    ss <<
    "function M." << methodName->s_name << "(a)\n"
    "  c(\"" << methodName->s_name << "\", a)\n"
    "end";
    return gensym(ss.str().c_str());
}

t_symbol *ofxOfeliaEmbBase::getClassMethodScript(t_symbol *varName, t_symbol *requireName, t_symbol *methodName,
                                                 const ofxOfeliaMaps::Types &types, const int numInlets)
{
    t_symbol *params = ofxOfeliaEmbBase::getParameterScript(types, numInlets);
    std::ostringstream ss;
    ss <<
    "local m = require(\"" << requireName->s_name << "\")\n"
    "local r = m." << varName->s_name << ':' << methodName->s_name << params->s_name << "\n"
    "return r";
    return gensym(ss.str().c_str());
}

t_symbol *ofxOfeliaEmbBase::getClassVarName(t_symbol *className)
{
    const std::string &_className(className->s_name);
    const auto it = std::find_if(_className.begin(), _className.end(),
                                 [](char c) {return std::isupper(c);});
    std::string _varName = _className.substr(std::distance(_className.begin(), it));
    _varName[0] = std::tolower(_varName[0]);
    return gensym(_varName.c_str());
}

t_symbol *ofxOfeliaEmbBase::getClassModuleName(t_symbol *className, int argc, t_atom *argv)
{
    if (argc)
    {
        if (argv[0].a_type == A_SYMBOL)
        definePtr->data.sym = argv[0].a_w.w_symbol;
        else if (!(argv[0].a_type == A_FLOAT && argv[0].a_w.w_float == 0)) /* skip unset $1 argument */
        {
            classArgTypeError(className, gensym("module"));
            return 0;
        }
    }
    definePtr->data.initSym();
    return definePtr->data.sym;
}

t_symbol *ofxOfeliaEmbBase::getClassRequireName(t_symbol *className, int argc, t_atom *argv)
{
    t_symbol *requireName = &s_;
    if (argv[0].a_type == A_SYMBOL)
    requireName = argv[0].a_w.w_symbol;
    else if (!(argv[0].a_type == A_FLOAT && argv[0].a_w.w_float == 0)) /* skip unset $1 argument */
    {
        classArgTypeError(className, gensym("module"));
        return 0;
    }
    return requireName;
}

t_symbol *ofxOfeliaEmbBase::getClassMethodName(t_symbol *className, int argc, t_atom *argv)
{
    if (argv[1].a_type == A_SYMBOL)
    return argv[1].a_w.w_symbol;
    classArgTypeError(className, gensym("method"));
    return 0;
}

void ofxOfeliaEmbBase::getDefaultFunctionArgs(std::vector<t_atom> &args, const ofxOfeliaMaps::TypeSet &typeSet)
{
    /* find the smallest number of valid argument types other than zero */
    const auto smallestArgTypes = std::min_element(typeSet.begin(), typeSet.end() - 1,
                                                   [](const std::vector<int> &a, const std::vector<int> &b)
                                                   {return a.size() < b.size();});
    const size_t numSmallestArgTypes = smallestArgTypes->size();
    for (size_t i = 0; i < numSmallestArgTypes; ++i)
    {
        if (smallestArgTypes->at(i) == LUA_TSTRING)
        addSymbolArg(args, &s_);
        else
        addFloatArg(args, 0);
    }
}

bool ofxOfeliaEmbBase::checkFunctionArgTypes(std::vector<t_atom> &args, const ofxOfeliaMaps::Types &types)
{
    bool isActiveArgFound = false; /* whether an active argument('?') is found */
    for (size_t i = args.size() - 1; i >= 2; --i) /* loop arguments backwards */
    {
        const int type = types[i - 2]; /* required type */
        if (args[i].a_type == A_FLOAT)
        {
            if (type != LUA_TBOOLEAN && type != LUA_TNUMBER)
            return false;
        }
        else if (args[i].a_type == A_SYMBOL)
        {
            if (*args[i].a_w.w_symbol->s_name == '?') /* active argument found */
            {
                args.erase(args.begin() + i); /* erase active arguments from vector */
                if (!isActiveArgFound)
                isActiveArgFound = true;
                continue;
            }
            else if (type != LUA_TSTRING)
            return false;
        }
        else return false;
        if (isActiveArgFound) /* active argument cannot be placed after passive argument */
        return false;
    }
    return true;
}

void ofxOfeliaEmbBase::appendArgs(std::vector<t_atom> &args, int argc, t_atom *argv)
{
    args.insert(args.end(), argv, argv + argc);
}

void ofxOfeliaEmbBase::addFloatArg(std::vector<t_atom> &args, t_floatarg f)
{
    t_atom arg;
    SETFLOAT(&arg, f);
    args.push_back(arg);
}

void ofxOfeliaEmbBase::addSymbolArg(std::vector<t_atom> &args, t_symbol *s)
{
    t_atom arg;
    SETSYMBOL(&arg, s);
    args.push_back(arg);
}

void ofxOfeliaEmbBase::setFloatArg(std::vector<t_atom> &args, const size_t index, t_floatarg f)
{
    SETFLOAT(args.data() + index, f);
}

void ofxOfeliaEmbBase::setSymbolArg(std::vector<t_atom> &args, const size_t index, t_symbol *s)
{
    SETSYMBOL(args.data() + index, s);
}

void ofxOfeliaEmbBase::classMethodArgError(t_symbol *className, t_symbol *methodName,
                                           const ofxOfeliaMaps::TypeSet &typeSet)
{
    const size_t numTypeSets = typeSet.size(); /* number of valid argument cases */
    error("%s: wrong arguments given to '%s'", className->s_name, methodName->s_name);
    error("  possible prototype(s):");
    for (size_t i = 0; i < numTypeSets; ++i)
    {
        std::ostringstream ss;
        const size_t numTypes = typeSet[i].size();
        for (size_t j = 0; j < numTypes; ++j)
        {
            if (j) ss << ", ";
            switch (typeSet[i][j])
            {
                case LUA_TBOOLEAN:
                    ss << "boolean";
                    break;
                case LUA_TNUMBER:
                    ss << "number";
                    break;
                case LUA_TSTRING:
                    ss << "string";
                    break;
                default:
                    break;
            }
        }
        error("    %s(%s)", methodName->s_name, ss.str().c_str());
    }
}

void ofxOfeliaEmbBase::classArgTypeError(t_symbol *className, t_symbol *argType)
{
    error("%s: wrong argument type for %s name", className->s_name, argType->s_name);
}

void ofxOfeliaEmbBase::classMethodNotFoundError(t_symbol *className, t_symbol *methodName,
                                                const ofxOfeliaMaps::ClassData &classData)
{
    error("%s: cannot find the method name '%s'", className->s_name, methodName->s_name);
    error("  available class method(s):");
    const size_t numMethods = classData.size() - 1; /* number of class methods */
    for (size_t i = 0; i < numMethods; ++i)
    error("    %s::%s()", className->s_name, classData[i].first.c_str());
}

bool ofxOfeliaEmbBase::getClassCommonArgs(t_symbol *className, int argc, t_atom *argv,
                                          const ofxOfeliaMaps::ClassData &classData, std::vector<t_atom> &args)
{
    t_symbol *moduleName = getClassModuleName(className, argc, argv);
    if (!moduleName) return false;
    const size_t numMethods = classData.size() - 1; /* number of class methods */
    args.reserve(4 + numMethods); /* module name, semi-colons, script, methods, additional script */
    addSymbolArg(args, moduleName);
    addSymbolArg(args, gensym(";"));
    return true;
}

bool ofxOfeliaEmbBase::getClassMethodCommonArgs(t_symbol *className, t_symbol *methodName, int argc, t_atom *argv,
                                                const ofxOfeliaMaps::FunctionData &functionData, std::vector<t_atom> &args,
                                                ofxOfeliaMaps::Types &types, int &numInlets)
{
    const auto &typeSet = functionData.typeSet;
    args.reserve(12); /* -c flag, module name, max numArgs(8), semi-colons, script */
    addSymbolArg(args, gensym("-c11"));
    addSymbolArg(args, definePtr->data.getUniqueSym());
    appendArgs(args, argc, argv); /* append the incoming arguments to vector */
    const size_t numTypeSets = typeSet.size(); /* number of valid argument type set */
    if (!argc && numTypeSets > 1) /* if no argument found although required */
    getDefaultFunctionArgs(args, typeSet); /* get the smallest number of valid arguments */
    const size_t numArgs = args.size() - 2;
    const auto it = std::find_if(typeSet.begin(), typeSet.end(),
                                 [&](const std::vector<int> &ref)
                                 {return ref.size() == numArgs;}); /* check if valid number of arguments exists */
    if (it == typeSet.end() || !checkFunctionArgTypes(args, *it))
    {
        classMethodArgError(className, methodName, typeSet);
        return false;
    }
    numInlets = args.size() - 2 + 1;
    char buf[5];
    std::snprintf(buf, 5, "-c%d1", numInlets);
    setSymbolArg(args, 0, gensym(buf));
    addSymbolArg(args, gensym(";"));
    types = *it;
    return true;
}

bool ofxOfeliaEmbBase::getClassMethodCommonArgs(t_symbol *className, t_symbol *methodName, int argc, t_atom *argv,
                                                const ofxOfeliaMaps::ClassData &classData, std::vector<t_atom> &args,
                                                ofxOfeliaMaps::Types &types, int &numInlets)
{
    const auto it = std::find_if(classData.begin(), classData.end(),
                                 [&](const std::pair<std::string, ofxOfeliaMaps::FunctionData> &ref)
                                 {return ref.first == methodName->s_name;});
    if (it == classData.end())
    {
        classMethodNotFoundError(className, methodName, classData);
        return false;
    }
    argc -= 2;
    argv += 2;
    const auto &functionData = it->second;
    return getClassMethodCommonArgs(className, methodName, argc, argv, functionData, args, types, numInlets);
}
