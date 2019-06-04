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
    const string &ofBindingsPath = ofPath + "addons/ofxLua/src/bindings/desktop/ofBindings.cpp";
    const string &ofeliaBindingsPath = ofPath + "addons/ofxOfelia/macOSExternal/src/ofeliaBindings.cpp";
    const string &glewPath = ofPath + "libs/glew/include/GL/glew.h";;
    const string &ofeliaAliasesPath = "ofeliaAliases.cpp";
    
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
    "/*==============================================================================\n"
    " ofelia: OpenFrameworks as an External Library for Interactive Applications\n"
    " \n"
    " Copyright (c) 2018 Zack Lee <cuinjune@gmail.com>\n"
    " \n"
    " This program is free software: you can redistribute it and/or modify\n"
    " it under the terms of the GNU General Public License as published by\n"
    " the Free Software Foundation, either version 3 of the License, or\n"
    " (at your option) any later version.\n"
    " \n"
    " This program is distributed in the hope that it will be useful,\n"
    " but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
    " MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the\n"
    " GNU General Public License for more details.\n"
    " \n"
    " You should have received a copy of the GNU General Public License\n"
    " along with this program. If not, see <http://www.gnu.org/licenses/>.\n"
    " \n"
    " See https://github.com/cuinjune/ofxOfelia for documentation\n"
    " ==============================================================================*/\n"
    "\n"
    "#include \"ofeliaAliases.h\"\n"
    "#include \"ofMain.h\"\n"
    "\n"
    "void ofeliaAliases::makeAliases(lua_State *L)\n"
    "{\n";
    
    ss << indent << "/* make aliases of classes and functions in of module */\n";
    ss << indent << "lua_getglobal(L, \"of\");\n";
    
    for(string line; getline(input, line); )
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
    input.open(ofeliaBindingsPath.c_str());
    if (!input.good())
    {
        cout << "Error: Failed to find \"" << ofeliaBindingsPath << '\n';
        cout << "\nProgram finished\n\n";
        return 0;
    }
    cout << "Processing \"" << ofeliaBindingsPath << "\"\n";
    
    ss << indent << "/* make aliases of classes and functions in pd module */\n";
    ss << indent << "lua_getglobal(L, \"pd\");\n";
    
    for(string line; getline(input, line); )
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
    
    for(string line; getline(input, line); )
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
    ss << "}";
    ss << "\n\n";
    
    ofstream output;
    output.open(ofeliaAliasesPath.c_str());
    output << ss.str();
    output.close();
    
    const clock_t clockEnd = clock();
    const double elapsedSec = double(clockEnd - clockBegin) / CLOCKS_PER_SEC;
    cout << "\nProgram finished in " << setprecision(2) << elapsedSec << " sec\n\n";
    return 0;
}

