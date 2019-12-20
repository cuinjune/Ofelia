#include <iostream>
#include <algorithm>
#include <cctype>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

int main()
{
    string glewPath = "glew.h";
    string ofxOfeliaGLPath = "../../src/ofxOfeliaGL.cpp";
    ifstream input;
    input.open(glewPath.c_str());
    if (!input.good())
    {
        cout << "Error: Failed to find \"" << glewPath << '\n';
        cout << "\nProgram finished\n\n";
        return 0;
    }
    ostringstream ss;
    string indent = "    ";
    ss <<
    "#include \"ofxOfeliaGL.h\"\n"
    "\n"
    "void ofxOfeliaGL::addDefines(lua_State *L)\n"
    "{\n";
    for (string line; getline(input, line); )
    {
        size_t defineIndicator = line.find("#define GL_");
        if (defineIndicator != string::npos)
        {
            size_t defineNameFirstPos = defineIndicator + 8;
            size_t defineNameLastPos = line.find(' ', defineNameFirstPos + 3) - 1;
            string defineName = line.substr(defineNameFirstPos, defineNameLastPos - defineNameFirstPos + 1);
            if (std::any_of(defineName.begin(), defineName.end(), ::islower))
                continue;
            string defineValue = line.substr(defineNameLastPos + 2);
            if (!defineValue.empty() && !isspace(defineValue[0]) && isdigit(defineValue[0]))
            {
                ss << indent << "lua_pushnumber(L, " << defineValue << ");\n";
                ss << indent << "lua_setglobal(L, \"" << defineName << "\");\n";
            }
        }
    }
    ss << "}\n";
    ofstream output;
    output.open(ofxOfeliaGLPath.c_str());
    output << ss.str();
    output.close();
    return 0;
}
