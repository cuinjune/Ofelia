#include <iostream>
#include <utility>
#include <algorithm>
#include <cctype>
#include <string>
#include <cstring>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;

class Mapper
{
public:
    typedef pair<string, string> StringPair;
    typedef pair<string, vector<string>> StringVecStringPair;
    typedef tuple<string, string, string> StringTuple;
    
    bool addMapsToStream(const string &fileName, /* SWIG bindings cpp file name */
                         const string &m, /* module name */
                         ostringstream &ss, /* string stream to add maps */
                         bool bLogSkipped = false) /* whether to log skipped */
    {
        ifstream input;
        input.open(fileName.c_str()); /* open a SWIG bindings cpp file */
        if (!input.good())
        {
            cout << "Error: Failed to find \"" << fileName << '\n';
            cout << "\nProgram finished\n\n";
            return false;
        }
        string indent = "    ";
        const string indent2 = indent + indent;
        const string indent3 = indent2 + indent;
        
        /* create a function map */
        ss << indent << "const std::unordered_map<std::string, FunctionData> " << m << "FunctionMap =\n";
        ss << indent << "{";
        string funcNameAlias = "";
        bool hasReturnAlias = false;
        for (string line; getline(input, line);)
        {
            string objectIndicator = "SWIG_check_num_args(\"" + m;
            size_t objectPos = line.find(objectIndicator);
            if (objectPos != string::npos)
            {
                size_t funcNameFirstPos = objectPos + 21;
                if (line.find(':', funcNameFirstPos + 1) < line.find('"', funcNameFirstPos + 1) ||
                    line.find('_', funcNameFirstPos + 1) < line.find('"', funcNameFirstPos + 1))
                    continue; /* skip class members */
                size_t funcNameLastPos = line.find('"', funcNameFirstPos + 1) - 1;
                size_t numArgsFirstPos = funcNameLastPos + 3;
                size_t numArgsLastPos = line.find(',', numArgsFirstPos + 1) - 1;
                string funcName = line.substr(funcNameFirstPos, funcNameLastPos - funcNameFirstPos + 1);
                if (find(functionsToSkip.begin(), functionsToSkip.end(), funcName) != functionsToSkip.end())
                    continue;
                auto it = find_if(functionsToRename.begin(), functionsToRename.end(),
                                  [&](StringPair &element){return element.first == funcName;});
                if (it != functionsToRename.end())
                    funcName = it->second;
                int numArgs = stoi(line.substr(numArgsFirstPos, numArgsLastPos - numArgsFirstPos + 1));
                vector<int> argTypes;
                argTypes.reserve(numArgs);
                bool hasReturn = false;
                while (line.rfind("return SWIG_arg; }") == string::npos)
                {
                    for (size_t typeNamePos = line.find("(!lua_is", 0);
                         typeNamePos != string::npos; typeNamePos = line.find("(!lua_is", typeNamePos + 8))
                    {
                        size_t typeNameFirstPos = typeNamePos + 8;
                        string typeName = line.substr(typeNameFirstPos, 3);
                        if (typeName == "boo")
                            argTypes.push_back(1);
                        else if (typeName == "num")
                            argTypes.push_back(3);
                        else if (typeName == "str")
                            argTypes.push_back(4);
                    }
                    if (!hasReturn && line.find("result") != string::npos)
                        hasReturn = true;
                    getline(input, line);
                }
                if (argTypes.size() != numArgs || numArgs > 8)
                {
                    if (bLogSkipped)
                        cout << "Skipped: " << funcName << '\n';
                    continue; /* skip functions with unsupported type argument */
                }
                if (funcName != funcNameAlias)
                {
                    if (funcNameAlias != "")
                    {
                        ss << "{}},"; /* add an empty data */
                        if (hasReturnAlias) ss << "1}},";
                        else ss << "0}},";
                    }
                    ss << '\n' << indent2 << "{\"" << funcName << "\",{{";
                    funcNameAlias = funcName;
                    hasReturnAlias = hasReturn;
                }
                if (argTypes.size())
                {
                    ss << '{';
                    ss << argTypes[0];
                    for (size_t i = 1; i < argTypes.size(); ++i)
                    {
                        ss << ",";
                        ss << argTypes[i];
                    }
                    ss << "},";
                }
            }
        }
        ss << "{}},"; /* add an empty data */
        if (hasReturnAlias) ss << "1}},";
        else ss << "0}},";
        ss << '\n' << indent2 << "{\"\",{{},0}}\n"; /* add an empty data */
        ss << indent << "};\n";
        
        /* rewind the input file */
        input.clear();
        input.seekg(0);
        
        /* create a class map */
        ss << indent << "const std::unordered_map<std::string, ClassData> " << m << "ClassMap =\n";
        ss << indent << "{";
        string classNameAlias = "";
        string classTypeAlias = "";
        string methodNameAlias = "";
        hasReturnAlias = false;
        for (string line; getline(input, line);)
        {
            string objectIndicator = "SWIG_check_num_args(\"" + m;
            size_t objectPos = line.find(objectIndicator);
            if (objectPos != string::npos)
            {
                size_t classNameFirstPos = objectPos + 21;
                size_t classIndicatorPos = line.find("::", classNameFirstPos + 1);
                size_t classNameLastPos = classIndicatorPos - 1;
                size_t doubleQuotePos = line.find('"', classNameFirstPos + 1);
                string className, classType, methodName;
                if (classIndicatorPos < doubleQuotePos)
                {
                    className = line.substr(classNameFirstPos, classNameLastPos - classNameFirstPos + 1);
                    size_t underScorePos = className.find('_');
                    if (underScorePos != string::npos)
                    {
                        size_t classTypeFirstPos = underScorePos + 1;
                        size_t classTypeLastPos = classIndicatorPos - 1;
                        std::string classType = className.substr(classTypeFirstPos, classTypeLastPos - classTypeFirstPos + 1);
                        className.erase(underScorePos);
                        if (classType != classTypeAlias && className == classNameAlias)
                            continue;
                        classTypeAlias = classType;
                    }
                    if (find(classesToSkip.begin(), classesToSkip.end(), className) != classesToSkip.end())
                        continue;
                    auto it = find_if(classesToRename.begin(), classesToRename.end(),
                                      [&](StringPair &element){return element.first == className;});
                    if (it != classesToRename.end())
                        className = it->second;
                    size_t methodNameFirstPos = classIndicatorPos + 2;
                    size_t methodNameLastPos = doubleQuotePos - 1;
                    methodName = line.substr(methodNameFirstPos, methodNameLastPos - methodNameFirstPos + 1);
                    if (methodName[0] == '_' || methodName.find(' ') != string::npos)
                        continue;
                    if (methodName.back() == '_')
                        methodName.pop_back();
                    if (find(classMethodsToSkip.begin(), classMethodsToSkip.end(), make_pair(className, methodName)) != classMethodsToSkip.end())
                        continue;
                    auto it2 = find_if(classMethodsToRename.begin(), classMethodsToRename.end(),
                                       [&](StringTuple &element){return get<0>(element) == className &&
                                           get<1>(element) == methodName;});
                    if (it2 != classMethodsToRename.end())
                        methodName = get<2>(*it2);
                    size_t numArgsFirstPos = methodNameLastPos + 3;
                    size_t numArgsLastPos = line.find(',', numArgsFirstPos + 1) - 1;
                    int numArgs = stoi(line.substr(numArgsFirstPos, numArgsLastPos - numArgsFirstPos + 1));
                    if (!numArgs) continue;
                    if (className != classNameAlias)
                    {
                        if (classNameAlias != "")
                        {
                            if (methodNameAlias != "")
                            {
                                ss << "{}},"; /* add an empty data */
                                if (hasReturnAlias) ss << "1}},";
                                else ss << "0}},";
                            }
                            ss << '\n' << indent3 << "{\"\",{{},0}}}"; /* add an empty data */
                            ss << '\n' << indent2 << "},";
                        }
                        ss << '\n' << indent2 << "{\"" << className << "\",{";
                        classNameAlias = className;
                        methodNameAlias = "";
                        auto it3 = find_if(classMethodsToAdd.begin(), classMethodsToAdd.end(),
                                           [&](StringVecStringPair &element){return element.first == className;});
                        if (it3 != classMethodsToAdd.end())
                        {
                            const auto &methodsVec = (*it3).second;
                            for (size_t i = 0; i < methodsVec.size(); ++i)
                                ss << '\n' << indent3 << methodsVec[i];
                        }
                    }
                    if (className == methodName) continue; /* skip constructors */
                    vector<int> argTypes;
                    argTypes.reserve(numArgs - 1); /* first argument is a pointer to class */
                    bool isClassMember = false;
                    bool hasReturn = false;
                    while (line.rfind("return SWIG_arg; }") == string::npos)
                    {
                        if (!isClassMember && line.find("(!SWIG_isptrtype(L,1)") != string::npos)
                            isClassMember = true;
                        if (isClassMember)
                        {
                            for (size_t typeNamePos = line.find("(!lua_is", 0);
                                 typeNamePos != string::npos; typeNamePos = line.find("(!lua_is", typeNamePos + 8))
                            {
                                size_t typeNameFirstPos = typeNamePos + 8;
                                string typeName = line.substr(typeNameFirstPos, 3);
                                if (typeName == "boo")
                                    argTypes.push_back(1);
                                else if (typeName == "num")
                                    argTypes.push_back(3);
                                else if (typeName == "str")
                                    argTypes.push_back(4);
                            }
                        }
                        if (!hasReturn && line.find("result") != string::npos)
                            hasReturn = true;
                        getline(input, line);
                    }
                    if (!isClassMember || argTypes.size() != numArgs - 1 || numArgs - 1 > 8)
                    {
                        if (bLogSkipped)
                            cout << "Skipped: " << className << "::" << methodName << '\n';
                        continue; /* skip static methods or methods with unsupported type argument */
                    }
                    if (methodName != methodNameAlias)
                    {
                        if (methodNameAlias != "")
                        {
                            ss << "{}},"; /* add an empty data */
                            if (hasReturnAlias) ss << "1}},";
                            else ss << "0}},";
                        }
                        ss << '\n' << indent3 << "{\"" << methodName << "\",{{";
                        methodNameAlias = methodName;
                        hasReturnAlias = hasReturn;
                    }
                    if (argTypes.size())
                    {
                        ss << '{';
                        ss << argTypes[0];
                        for (size_t i = 1; i < argTypes.size(); ++i)
                        {
                            ss << ",";
                            ss << argTypes[i];
                        }
                        ss << "},";
                    }
                }
            }
        }
        ss << "{}},"; /* add an empty data */
        if (hasReturnAlias) ss << "1}},";
        else ss << "0}},";
        ss << '\n' << indent3 << "{\"\", {{},0}}}"; /* add an empty data */
        ss << '\n' << indent2 << "}\n";
        ss << indent << "};\n";
        ss << indent << "functionMap.insert(" << m << "FunctionMap.begin(), " << m << "FunctionMap.end());\n";
        ss << indent << "classMap.insert(" << m << "ClassMap.begin(), " << m << "ClassMap.end());\n";
        input.close();
        return true;
    }
    void addFunctionToSkip(const string &name)
    {
        functionsToSkip.push_back(name);
    }
    void addClassToSkip(const string &name)
    {
        classesToSkip.push_back(name);
    }
    void addClassMethodToSkip(const StringPair &names)
    {
        classMethodsToSkip.push_back(names);
    }
    void addFunctionToRename(const StringPair &names)
    {
        functionsToRename.push_back(names);
    }
    void addClassToRename(const StringPair &names)
    {
        classesToRename.push_back(names);
    }
    void addClassMethodToRename(const StringTuple &names)
    {
        classMethodsToRename.push_back(names);
    }
    void addClassMethodToAdd(const StringVecStringPair &names)
    {
        classMethodsToAdd.push_back(names);
    }
private:
    vector<string> functionsToSkip;
    vector<string> classesToSkip;
    vector<StringPair> classMethodsToSkip;
    vector<StringPair> functionsToRename;
    vector<StringPair> classesToRename;
    vector<StringTuple> classMethodsToRename;
    vector<StringVecStringPair> classMethodsToAdd;
};

int main(int argc, char *argv[])
{
    bool debug = false;
    if (argc > 1 && !strcmp(argv[1], "-d"))
        debug = true;
    string ofBindingsPath = "../../libs/ofxLua/src/bindings/desktop/ofBindings.cpp";
    string ofxOfeliaPdBindingsPath = "../../src/ofxOfeliaPdBindings.cpp";
    string ofxOfeliaMapsPath = "../../src/ofxOfeliaMaps.cpp";
    Mapper mapper;
    
    /* of bindings */
    mapper.addFunctionToSkip("ofExit");
    mapper.addFunctionToSkip("ofToHex");
    mapper.addFunctionToSkip("ofToBinary");
    mapper.addFunctionToSkip("ofLoadMatrix");
    mapper.addFunctionToSkip("ofMultMatrix");
    mapper.addClassToSkip("ofFilePath");
    mapper.addClassToSkip("ofMatrixStack");
    mapper.addClassToSkip("ofTrueTypeFontSettings");
    mapper.addClassToSkip("ofBaseSoundPlayer");
    mapper.addClassToSkip("ofBaseSoundStream");
    mapper.addClassToSkip("ofVideoDevice");
    mapper.addClassToSkip("ofVideoFormat");
    mapper.addClassMethodToRename({"ofFbo", "begin", "beginFbo"});
    mapper.addClassMethodToRename({"ofFbo", "end", "endFbo"});
    mapper.addClassMethodToRename({"ofCamera", "begin", "beginCamera"});
    mapper.addClassMethodToRename({"ofCamera", "end", "endCamera"});
    mapper.addClassMethodToRename({"ofEasyCam", "begin", "beginCamera"});
    mapper.addClassMethodToRename({"ofMaterial", "begin", "beginMaterial"});
    mapper.addClassMethodToRename({"ofMaterial", "end", "endMaterial"});
    mapper.addClassMethodToRename({"ofShader", "begin", "beginShader"});
    mapper.addClassMethodToRename({"ofShader", "end", "endShader"});
    mapper.addClassMethodToAdd({"ofEasyCam", {"{\"endCamera\",{{{}},0}},"}});
    mapper.addClassMethodToAdd({"ofSoundPlayer", {"{\"getPan\",{{{}},1}},",
        "{\"getPosition\",{{{}},1}},",
        "{\"getSpeed\",{{{}},1}},",
        "{\"getVolume\",{{{}},1}},",
        "{\"isLoaded\",{{{}},1}},",
        "{\"isPlaying\",{{{}},1}},",
        "{\"play\",{{{}},0}},",
        "{\"setLoop\",{{{1},{}},0}},",
        "{\"setMultiPlay\",{{{1},{}},0}},",
        "{\"setPan\",{{{3},{}},0}},",
        "{\"setPaused\",{{{1},{}},0}},",
        "{\"setPosition\",{{{3},{}},0}},",
        "{\"setPositionMS\",{{{3},{}},0}},",
        "{\"setSpeed\",{{{3},{}},0}},",
        "{\"setVolume\",{{{3},{}},0}},",
        "{\"stop\",{{{}},0}},",
        "{\"unload\",{{{}},0}},"}});
    
    /* pd bindings */
    mapper.addClassToRename({"pdWindow", "ofWindow"});
    mapper.addClassToSkip("pdCanvas");
    mapper.addClassToSkip("pdSend");
    mapper.addClassToSkip("pdInlet");
    mapper.addClassToSkip("pdOutlet");
    mapper.addClassToSkip("pdValue");
    mapper.addClassToSkip("pdArray");
    mapper.addClassToSkip("pdClock");
    mapper.addClassToSkip("pdOsc");
    mapper.addClassToSkip("pdBlOsc");
    mapper.addClassToSkip("pdFilter");
    mapper.addClassToSkip("pdReverb");
    mapper.addClassToSkip("pdLog");
    mapper.addClassToRename({"pdJson", "ofJson"});
    mapper.addClassToSkip("pdOscSender");
    mapper.addClassToSkip("pdOscReceiver");
    mapper.addFunctionToRename({"pdSysGui", "ofSysGui"});
    mapper.addFunctionToRename({"pdGetBlockSize", "ofGetBlockSize"});
    mapper.addFunctionToRename({"pdGetSampleRate", "ofGetSampleRate"});
    mapper.addFunctionToRename({"pdGetNumInChannels", "ofGetNumInChannels"});
    mapper.addFunctionToRename({"pdGetNumOutChannels", "ofGetNumOutChannels"});
    mapper.addFunctionToRename({"pdGetDspState", "ofGetDspState"});
    mapper.addFunctionToRename({"pdGetMaxString", "ofGetMaxString"});
    mapper.addFunctionToRename({"pdGetFloatSize", "ofGetFloatSize"});
    mapper.addFunctionToRename({"pdGetMinFloat", "ofGetMinFloat"});
    mapper.addFunctionToRename({"pdGetMaxFloat", "ofGetMaxFloat"});
    mapper.addFunctionToRename({"pdIsBadFloat", "ofIsBadFloat"});
    mapper.addFunctionToRename({"pdIsBigOrSmall", "ofIsBigOrSmall"});
    mapper.addFunctionToRename({"pdGetPdVersionMajor", "ofGetPdVersionMajor"});
    mapper.addFunctionToRename({"pdGetPdVersionMinor", "ofGetPdVersionMinor"});
    mapper.addFunctionToRename({"pdGetPdVersionBugFix", "ofGetPdVersionBugFix"});
    mapper.addFunctionToRename({"pdGetOfeliaVersionMajor", "ofGetOfeliaVersionMajor"});
    mapper.addFunctionToRename({"pdGetOfeliaVersionMinor", "ofGetOfeliaVersionMinor"});
    mapper.addFunctionToRename({"pdGetOfeliaVersionBugFix", "ofGetOfeliaVersionBugFix"});
    mapper.addFunctionToRename({"pdUnZipPass", "ofUnZipPass"});
    mapper.addFunctionToRename({"pdUnZip", "ofUnZip"});
    
    /* start writing the file */
    ostringstream ss;
    ss <<
    "#include \"ofxOfeliaMaps.h\"\n"
    "\n"
    "std::unordered_map<std::string, ofxOfeliaMaps::FunctionData> ofxOfeliaMaps::functionMap;\n"
    "std::unordered_map<std::string, ofxOfeliaMaps::ClassData> ofxOfeliaMaps::classMap;\n"
    "\n"
    "void ofxOfeliaMaps::init()\n"
    "{\n";
    if (!mapper.addMapsToStream(ofBindingsPath, "of", ss, debug)) return 0;
    if (!mapper.addMapsToStream(ofxOfeliaPdBindingsPath, "pd", ss, debug)) return 0;
    ss <<
    "}\n";
    ofstream output;
    output.open(ofxOfeliaMapsPath.c_str());
    output << ss.str();
    output.close();
    return 0;
}
