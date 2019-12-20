#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class ofxOfeliaMaps
{
public:
    static void init();
    typedef std::vector<int> Types;
    typedef std::vector<Types> TypeSet;
    struct FunctionData
    {
        TypeSet typeSet;
        bool hasReturn;
    };
    typedef std::vector<std::pair<std::string, FunctionData>> ClassData;
    static std::unordered_map<std::string, FunctionData> functionMap;
    static std::unordered_map<std::string, ClassData> classMap;
private:
};
