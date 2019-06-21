#include <iostream>
#include <string>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>
#include <chrono>

using namespace std;

int main(int argc, const char * argv[])
{
    cout << "\nProgram Started\n\n";
    const clock_t clockBegin = clock();
    
    const string &ofPath = "../../../../";
    const string &ofBindingsPath = ofPath + "addons/ofxOfelia/libs/ofxLua/src/bindings/desktop/ofBindings.cpp";
    const string &ofxOfeliaBindingsPath = ofPath + "addons/ofxOfelia/src/ofxOfeliaBindings.cpp";
    const string &glewPath = ofPath + "libs/glew/include/GL/glew.h";;
    const string &ofxOfeliaAliasesPath = "ofxOfeliaAliases.cpp";
    
    ifstream input;
    input.open(ofBindingsPath.c_str());
    if (!input.good())
    {
        cout << "Error: Failed to find \"" << ofBindingsPath << '\n';
        cout << "\nProgram finished\n\n";
        return 0;
    }
    cout << "Processing \"" << ofBindingsPath << "\"\n";
    ostringstream ss;
    string indent = "    ";
    ss <<
    "#include \"ofxOfeliaAliases.h\"\n"
    "\n"
    "void ofxOfeliaAliases::makeAliases(lua_State *L)\n"
    "{\n";
    
    ss << indent << "/* make aliases of classes and functions in of module */\n";
    ss << indent << "lua_getglobal(L, \"of\");\n";
    
    for (string line; getline(input, line); )
    {
        if (line.find("swig_SwigModule_attributes[]") != string::npos)
        {
            while (line.find("};") == string::npos)
            {
                const size_t first = line.find('"');
                if (first != string::npos)
                {
                    const size_t last = line.find('"', first + 1);
                    const string &name = line.substr(first + 1, last - first - 1);
                    ss << indent << "lua_getfield(L, -1, \"" << name << "\");\n";
                    ss << indent << "lua_setglobal(L, \"of" << name << "\");\n";
                }
                getline(input, line);
            }
        }
        if (line.find("swig_SwigModule_constants[]") != string::npos)
        {
            while (line.find("};") == string::npos)
            {
                const size_t first = line.find('"');
                if (first != string::npos)
                {
                    const size_t last = line.find('"', first + 1);
                    const string &name = line.substr(first + 1, last - first - 1);
                    ss << indent << "lua_getfield(L, -1, \"" << name << "\");\n";
                    if (line.find("OF_", last + 1) != string::npos)
                        ss << indent << "lua_setglobal(L, \"OF_" << name << "\");\n";
                    else if (line.find("of", last + 1) != string::npos)
                        ss << indent << "lua_setglobal(L, \"of" << name << "\");\n";
                    else
                        ss << indent << "lua_setglobal(L, \"" << name << "\");\n";
                }
                getline(input, line);
            }
        }
        if (line.find("swig_SwigModule_methods[]") != string::npos)
        {
            while (line.find("};") == string::npos)
            {
                const size_t first = line.find('"');
                if (first != string::npos)
                {
                    const size_t last = line.find('"', first + 1);
                    const string &name = line.substr(first + 1, last - first - 1);
                    const char upper = toupper(name[0]);
                    ss << indent << "lua_getfield(L, -1, \"" << name << "\");\n";
                    ss << indent << "lua_setglobal(L, \"of" << upper << name.substr(1) << "\");\n";
                }
                getline(input, line);
            }
        }
        if (line.find("swig_SwigModule_classes[]") != string::npos)
        {
            while (line.find("};") == string::npos)
            {
                const size_t first = line.find("&_wrap_class_");
                if (first != string::npos)
                {
                    const size_t last = line.rfind(',');
                    const string &name = line.substr(first + 13, last - first - 13);
                    ss << indent << "lua_getfield(L, -1, \"" << name << "\");\n";
                    ss << indent << "lua_setglobal(L, \"of" << name << "\");\n";
                }
                getline(input, line);
            }
        }
    }
    ss << indent << "lua_pop(L, 1);\n";
    input.close();
    
    ss << indent << "\n";
    input.open(ofxOfeliaBindingsPath.c_str());
    if (!input.good())
    {
        cout << "Error: Failed to find \"" << ofxOfeliaBindingsPath << '\n';
        cout << "\nProgram finished\n\n";
        return 0;
    }
    cout << "Processing \"" << ofxOfeliaBindingsPath << "\"\n";
    
    ss << indent << "/* make aliases of classes and functions in pd module */\n";
    ss << indent << "lua_getglobal(L, \"pd\");\n";
    
    for (string line; getline(input, line); )
    {
        if (line.find("swig_SwigModule_attributes[]") != string::npos)
        {
            while (line.find("};") == string::npos)
            {
                const size_t first = line.find('"');
                if (first != string::npos)
                {
                    const size_t last = line.find('"', first + 1);
                    string name = line.substr(first + 1, last - first - 1);
                    ss << indent << "lua_getfield(L, -1, \"" << name << "\");\n";
                    if (name.find("OF") != string::npos)
                    {
                        name.erase(0, 2);
                        ss << indent << "lua_setglobal(L, \"of" << name << "\");\n";
                    }
                    else
                        ss << indent << "lua_setglobal(L, \"pd" << name << "\");\n";
                }
                getline(input, line);
            }
        }
        if (line.find("swig_SwigModule_constants[]") != string::npos)
        {
            while (line.find("};") == string::npos)
            {
                const size_t first = line.find('"');
                if (first != string::npos)
                {
                    const size_t last = line.find('"', first + 1);
                    const string &name = line.substr(first + 1, last - first - 1);
                    ss << indent << "lua_getfield(L, -1, \"" << name << "\");\n";
                    if (line.find("PD_", last + 1) != string::npos)
                        ss << indent << "lua_setglobal(L, \"PD_" << name << "\");\n";
                    else if (line.find("pd", last + 1) != string::npos)
                        ss << indent << "lua_setglobal(L, \"pd" << name << "\");\n";
                    else
                        ss << indent << "lua_setglobal(L, \"" << name << "\");\n";
                }
                getline(input, line);
            }
        }
        if (line.find("swig_SwigModule_methods[]") != string::npos)
        {
            while (line.find("};") == string::npos)
            {
                const size_t first = line.find('"');
                if (first != string::npos)
                {
                    const size_t last = line.find('"', first + 1);
                    string name = line.substr(first + 1, last - first - 1);
                    const char upper = toupper(name[0]);
                    ss << indent << "lua_getfield(L, -1, \"" << name << "\");\n";
                    if (name.find("OF") != string::npos)
                    {
                        name.erase(0, 2);
                        ss << indent << "lua_setglobal(L, \"of" << name << "\");\n";
                    }
                    else
                        ss << indent << "lua_setglobal(L, \"pd" << upper << name.substr(1) << "\");\n";
                }
                getline(input, line);
            }
        }
        if (line.find("swig_SwigModule_classes[]") != string::npos)
        {
            while (line.find("};") == string::npos)
            {
                const size_t first = line.find("&_wrap_class_");
                if (first != string::npos)
                {
                    const size_t last = line.rfind(',');
                    string name = line.substr(first + 13, last - first - 13);
                    ss << indent << "lua_getfield(L, -1, \"" << name << "\");\n";
                    if (name.find("OF") != string::npos)
                    {
                        name.erase(0, 2);
                        ss << indent << "lua_setglobal(L, \"of" << name << "\");\n";
                    }
                    else
                        ss << indent << "lua_setglobal(L, \"pd" << name << "\");\n";
                }
                getline(input, line);
            }
        }
    }
    ss << indent << "lua_pop(L, 1);\n";
    input.close();
    
    ss << indent << "\n";
    input.open(glewPath.c_str());
    if (!input.good())
    {
        cout << "Error: Failed to find \"" << glewPath << '\n';
        cout << "\nProgram finished\n\n";
        return 0;
    }
    cout << "Processing \"" << glewPath << "\"\n";
    
    ss << indent << "/* make aliases of gl defines */\n";
    
    for (string line; getline(input, line); )
    {
        const size_t first = line.find("#define GL_");
        const size_t last = line.find(' ', first + 12);
        if (first != string::npos)
        {
            string name = line.substr(first + 11, last - first - 11);
            string value = line.substr(last + 1);
            if (isdigit(value[0]))
            {
                ss << indent << "lua_pushnumber(L, " << value << ");\n";
                ss << indent << "lua_setglobal(L, \"GL_" << name << "\");\n";
            }
        }
    }
    ss << "}\n";
    
    ofstream output;
    output.open(ofxOfeliaAliasesPath.c_str());
    output << ss.str();
    output.close();
    
    const clock_t clockEnd = clock();
    const double elapsedSec = double(clockEnd - clockBegin) / CLOCKS_PER_SEC;
    cout << "\nProgram finished in " << setprecision(2) << elapsedSec << " sec\n\n";
    return 0;
}

