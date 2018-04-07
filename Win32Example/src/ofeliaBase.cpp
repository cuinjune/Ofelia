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

#if defined(_WIN32)
#include <winsock2.h>
#endif
#include "ofeliaBase.h"
#include "ofeliaStyle.h"

/* ________________________________________________________________________________
 * common method
 */
void removeSemicolonFromArgs(int &argc, t_atom *argv)
{
    for (int i=0; i<argc; ++i) {
        
        if (argv[i].a_type == A_SYMBOL && argv[i].a_w.w_symbol->s_name[0] == ';')
            memmove(argv+i, argv+i+1, (--argc-i) * sizeof(t_atom));
    }
}

void getVarNameLocalPrefixes(t_ofeliaVarName &varName)
{
    t_canvas *canvas = canvas_getcurrent();
    char cp[MAXPDSTRING];
    snprintf(cp, MAXPDSTRING, "v%lx_", reinterpret_cast<long unsigned int>(canvas));
    varName.canvasPrefix = cp;
    varName.patchPrefix = canvas_realizedollar(canvas, gensym("v$0_"))->s_name;
}

void getVarNameLocalized(t_ofeliaVarName &varName)
{
    if (varName.name[0] == '@') /* patch local variable */
        varName.name.replace(0, 1, varName.patchPrefix);
    else if (varName.name[0] == '#') /* canvas local variable */
        varName.name.replace(0, 1, varName.canvasPrefix);
}

void getVarNameIndexed(t_ofeliaVarName &varName)
{
    if (varName.name.back() == ']') {
        
        const int pos = varName.name.rfind('[');
        
        if (pos != t_string::npos) {
            
            t_string sub = varName.name.substr(pos+1);
            sub.pop_back();
            
            if (isNumeric(sub.c_str())) {
                
                varName.name = varName.name.substr(0, pos);
                varName.index = max(0, ofToInt(sub.c_str()));
                return;
            }
        }
    }
    varName.index = 0;
}

void getCoordinatesFromRadAngle(float &posX, float &posY,
                                const float radius, const float angle)
{
    const float radian = angle / 360.0f * TWO_PI;
    posX = radius * cosf(radian);
    posY = radius * sinf(radian);
}

bool doesFilenameHaveExtension(t_string &path)
{
    if (path.rfind('.') != t_string::npos)
        return 1;
    else
        return 0;
}

bool makePathAbsolute(t_string &path, t_canvas *canvas)
{
    /* we should check library search paths as well as relative path */
    char buf[MAXPDSTRING], *bufptr;
    const char *dirname = canvas_getdir(canvas)->s_name;
    const char *filename = path.c_str();
    const int fd = open_via_path(dirname, filename, "", buf, &bufptr, MAXPDSTRING, 1);
    const bool fileExists = fd >= 0;
    
    /* if file exists, make path absolute */
    if (fileExists) {
        
        char *absPath = (char *)malloc(strlen(buf)+strlen(bufptr)+1+1);;
        strcpy(absPath, buf);
        char ps[2];
        ps[0] = OFELIA_PATH_SEPARATOR;
        ps[1] = '\0';
        strcat(absPath, ps);
        strcat(absPath, bufptr);
        path = absPath;
        free(absPath);
    }
#if defined(TARGET_WIN32)
    closesocket(fd);
#else
    close(fd);
#endif
    return fileExists;
}

int getClampIntValue(const int value, const int min, const int max)
{
    return value < min ? min : value > max ? max : value;
}

bool isNumeric(const char *str)
{
    while (*str) {
        
        if (isdigit(*str++) == 0)
            return 0;
    }
    return 1;
}

bool isFloat(const char *str)
{
    istringstream iss(str);
    float f;
    iss >> noskipws >> f; // noskipws considers leading whitespace invalid
    // Check the entire string was consumed and if either failbit or badbit is set
    return iss.eof() && !iss.fail();
}

bool endsWith(const char *str, const char *suffix)
{
    if (!str || !suffix)
        return 0;
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix >  lenstr)
        return 0;
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}

bool getColorFromArgs(const int argc, const t_atom *argv,
                      ofColor &color, const char *objName)
{
    switch (argc) {
            
        case 0:
        {
            color.set(OFELIA_DEFAULT_BGCOLOR);
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                color.set(ofColor(static_cast<unsigned char>(argv[0].a_w.w_float)));
            }
            else if (argv[0].a_type == A_SYMBOL) {
                
                const char *colorName = argv[0].a_w.w_symbol->s_name;
                
                if (!t_ofeliaSetColor::getColorByName(colorName, color))
                    return 0;
            }
            else {
                
                error("%s: wrong argument type", objName);
                return 0;
            }
            break;
        }
        case 3:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT) {
                
                color.set(ofColor(static_cast<unsigned char>(argv[0].a_w.w_float),
                                  static_cast<unsigned char>(argv[1].a_w.w_float),
                                  static_cast<unsigned char>(argv[2].a_w.w_float)));
            }
            else {
                
                error("%s: wrong argument type", objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", objName);
            return 0;
        }
    }
    return 1;
}

bool getColorAlphaFromArgs(const int argc, const t_atom *argv,
                           ofColor &color, const char *objName)
{
    switch (argc) {
            
        case 0:
        {
            color.set(ofColor::white);
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                color.set(ofColor(static_cast<unsigned char>(argv[0].a_w.w_float)));
            }
            else if (argv[0].a_type == A_SYMBOL) {
                
                const char *colorName = argv[0].a_w.w_symbol->s_name;
                
                if (!t_ofeliaSetColor::getColorByName(colorName, color))
                    return 0;
            }
            else {
                
                error("%s: wrong argument type", objName);
                return 0;
            }
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT) {
                
                color.set(ofColor(static_cast<unsigned char>(argv[0].a_w.w_float),
                                  static_cast<unsigned char>(argv[1].a_w.w_float)));
            }
            else if (argv[0].a_type == A_SYMBOL &&
                     argv[1].a_type == A_FLOAT) {
                
                const char *colorName = argv[0].a_w.w_symbol->s_name;
                
                if (!t_ofeliaSetColor::getColorByName(colorName, color))
                    return 0;
                else
                    color.a = argv[1].a_w.w_float;
            }
            else {
                
                error("%s: wrong argument type", objName);
                return 0;
            }
            break;
        }
        case 3:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT) {
                
                color.set(ofColor(static_cast<unsigned char>(argv[0].a_w.w_float),
                                  static_cast<unsigned char>(argv[1].a_w.w_float),
                                  static_cast<unsigned char>(argv[2].a_w.w_float)));
            }
            else {
                
                error("%s: wrong argument type", objName);
                return 0;
            }
            break;
        }
        case 4:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT) {
                
                color.set(ofColor(static_cast<unsigned char>(argv[0].a_w.w_float),
                                  static_cast<unsigned char>(argv[1].a_w.w_float),
                                  static_cast<unsigned char>(argv[2].a_w.w_float),
                                  static_cast<unsigned char>(argv[3].a_w.w_float)));
            }
            else {
                
                error("%s: wrong argument type", objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", objName);
            return 0;
        }
    }
    return 1;
}

bool getFloatColorFromArgs(const int argc, const t_atom *argv,
                           ofFloatColor &color, const char *objName)
{
    ofColor c;
    
    if (!getColorFromArgs(argc, argv, c, objName))
        return 0;
    color = c;
    return 1;
}

bool getFloatColorAlphaFromArgs(const int argc, const t_atom *argv,
                                ofFloatColor &color, const char *objName)
{
    ofColor c;
    
    if (!getColorAlphaFromArgs(argc, argv, c, objName))
        return 0;
    color = c;
    return 1;
}

bool getRectModeFromArgs(const int argc, const t_atom *argv,
                         ofRectMode &rectMode, const char *objName)
{
    switch (argc) {
            
        case 0:
        {
            rectMode = OF_RECTMODE_CORNER;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                rectMode = static_cast<ofRectMode>(argv[0].a_w.w_float != 0.0f);
            }
            else if (argv[0].a_type == A_SYMBOL) {
                
                const char *mode = argv[0].a_w.w_symbol->s_name;
                
                if (!strcmp(mode, "CORNER"))
                    rectMode = OF_RECTMODE_CORNER;
                else if (!strcmp(mode, "CENTER"))
                    rectMode = OF_RECTMODE_CENTER;
                else {
                    
                    error("%s: no method for '%s'", objName, mode);
                    return 0;
                }
            }
            else {
                
                error("%s: wrong argument type", objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", objName);
            return 0;
        }
    }
    return 1;
}

bool getTextModeFromArgs(const int argc, const t_atom *argv,
                         t_ofeliaTextModeElem &textMode, const char *objName)
{
    switch (argc) {
            
        case 0:
        {
            textMode.horMode = TEXT_ALIGN_LEFT;
            textMode.verMode = TEXT_ALIGN_TOP;
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT) {
                
                const int horMode = static_cast<int>(argv[0].a_w.w_float);
                const int verMode = static_cast<int>(argv[1].a_w.w_float);
                
                if (horMode < 0 || horMode > 2) {
                    
                    error("%s: horMode out of range", objName);
                    return 0;
                }
                if (verMode < 0 || verMode > 2) {
                    
                    error("%s: verMode out of range", objName);
                    return 0;
                }
                textMode.horMode = static_cast<TextAlignHorMode>(horMode);
                textMode.verMode = static_cast<TextAlignVerMode>(verMode);
            }
            else if (argv[0].a_type == A_SYMBOL &&
                     argv[1].a_type == A_SYMBOL) {
                
                const char *horMode = argv[0].a_w.w_symbol->s_name;
                
                if (!strcmp(horMode, "LEFT"))
                    textMode.horMode = TEXT_ALIGN_LEFT;
                else if (!strcmp(horMode, "CENTER"))
                    textMode.horMode = TEXT_ALIGN_CENTER;
                else if (!strcmp(horMode, "RIGHT"))
                    textMode.horMode = TEXT_ALIGN_RIGHT;
                else {
                    
                    error("%s: no method for '%s'", objName, horMode);
                    return 0;
                }
                const char *verMode = argv[1].a_w.w_symbol->s_name;
                
                if (!strcmp(verMode, "TOP"))
                    textMode.verMode = TEXT_ALIGN_TOP;
                else if (!strcmp(verMode, "MIDDLE"))
                    textMode.verMode = TEXT_ALIGN_MIDDLE;
                else if (!strcmp(verMode, "BOTTOM"))
                    textMode.verMode = TEXT_ALIGN_BOTTOM;
                else {
                    
                    error("%s: no method for '%s'", objName, verMode);
                    return 0;
                }
            }
            else {
                
                error("%s: wrong argument type", objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", objName);
            return 0;
        }
    }
    return 1;
}

bool getPolyModeFromArgs(const int argc, const t_atom *argv,
                         ofPolyWindingMode &polyMode, const char *objName)
{
    switch (argc) {
            
        case 0:
        {
            polyMode = OF_POLY_WINDING_ODD;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                const int mode = static_cast<int>(argv[0].a_w.w_float);
                
                if (mode < 0 || mode > 4) {
                    
                    error("%s: polyMode out of range", objName);
                    return 0;
                }
                polyMode = static_cast<ofPolyWindingMode>(mode);
            }
            else if (argv[0].a_type == A_SYMBOL) {
                
                const char *mode = argv[0].a_w.w_symbol->s_name;
                
                if (!strcmp(mode, "ODD"))
                    polyMode = OF_POLY_WINDING_ODD;
                else if (!strcmp(mode, "NONZERO"))
                    polyMode = OF_POLY_WINDING_NONZERO;
                else if (!strcmp(mode, "POSITIVE"))
                    polyMode = OF_POLY_WINDING_POSITIVE;
                else if (!strcmp(mode, "NEGATIVE"))
                    polyMode = OF_POLY_WINDING_NEGATIVE;
                else if (!strcmp(mode, "ABS_GEQ_TWO"))
                    polyMode = OF_POLY_WINDING_ABS_GEQ_TWO;
                else {
                    
                    error("%s: no method for '%s'", objName, mode);
                    return 0;
                }
            }
            else {
                
                error("%s: wrong argument type", objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", objName);
            return 0;
        }
    }
    return 1;
}

void getBoundingBoxDimen2d(ofVec2f &boxDimen, const vector<ofVec3f> &vertices)
{
    if (vertices.empty())
        return;
    ofVec2f boxMin(vertices[0]);
    ofVec2f boxMax(vertices[0]);
    
    for (auto &v : vertices) {
        
        if (v.x < boxMin.x) boxMin.x = v.x;
        if (v.y < boxMin.y) boxMin.y = v.y;
        if (v.x > boxMax.x) boxMax.x = v.x;
        if (v.y > boxMax.y) boxMax.y = v.y;
    }
    boxDimen = boxMax - boxMin;
}

void getBoundingBoxDimen3d(ofVec3f &boxDimen, const vector<ofVec3f> &vertices) {
    
    if (vertices.empty())
        return;
    ofVec3f boxMin(vertices[0]);
    ofVec3f boxMax(vertices[0]);
    
    for (auto &v : vertices) {
        
        if (v.x < boxMin.x) boxMin.x = v.x;
        if (v.y < boxMin.y) boxMin.y = v.y;
        if (v.z < boxMin.z) boxMin.z = v.z;
        if (v.x > boxMax.x) boxMax.x = v.x;
        if (v.y > boxMax.y) boxMax.y = v.y;
        if (v.z > boxMax.z) boxMax.z = v.z;
    }
    boxDimen = boxMax - boxMin;
}

bool getRotateElemFromArgs(const int argc, const t_atom *argv,
                           t_ofeliaRotateElem &elem, const char *objName)
{
    switch (argc) {
            
        case 0:
        {
            elem.angle = 0.0f;
            elem.axis.set(0.0f, 0.0f, 0.0f);
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                elem.angle = argv[0].a_w.w_float;
                elem.axis.set(0.0f, 0.0f, 0.0f);
            }
            else {
                
                error("%s: wrong argument type", objName);
                return 0;
            }
            break;
        }
        case 4:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT) {
                
                elem.angle = argv[0].a_w.w_float;
                elem.axis.set(argv[1].a_w.w_float,
                              argv[2].a_w.w_float,
                              argv[3].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", objName);
            return 0;
        }
    }
    return 1;
}

bool getImageDataFromArgs(const int argc, const t_atom *argv,
                          t_ofeliaImageData &data, const char *objName)
{
    if (!argc) {
        
        data.width = 0;
        data.height = 0;
        data.type = OF_IMAGE_GRAYSCALE;
        data.color.set(ofColor::white);
    }
    else if (argc >= 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            data.width = static_cast<int>(argv[0].a_w.w_float);
            data.height = static_cast<int>(argv[1].a_w.w_float);
            const int type = static_cast<int>(argv[2].a_w.w_float);
            
            if (type == 0)
                data.type = OF_IMAGE_GRAYSCALE;
            else if (type == 1)
                data.type = OF_IMAGE_COLOR;
            else if (type == 2)
                data.type = OF_IMAGE_COLOR_ALPHA;
            else {
                
                error("%s: type out of range", objName);
                return 0;
            }
            if (!getColorAlphaFromArgs(argc-3, argv+3, data.color, objName))
                return 0;
        }
        else if (argv[0].a_type == A_FLOAT &&
                 argv[1].a_type == A_FLOAT &&
                 argv[2].a_type == A_SYMBOL) {
            
            data.width = static_cast<int>(argv[0].a_w.w_float);
            data.height = static_cast<int>(argv[1].a_w.w_float);
            const char *type = argv[2].a_w.w_symbol->s_name;
            
            if (!strcmp(type, "GRAY"))
                data.type = OF_IMAGE_GRAYSCALE;
            else if (!strcmp(type, "RGB"))
                data.type = OF_IMAGE_COLOR;
            else if (!strcmp(type, "RGBA"))
                data.type = OF_IMAGE_COLOR_ALPHA;
            else {
                
                error("%s: no method for '%s'", objName, type);
                return 0;
            }
            if (!getColorAlphaFromArgs(argc-3, argv+3, data.color, objName))
                return 0;
        }
        else {
            
            error("%s: wrong argument type", objName);
            return 0;
        }
    }
    else {
        
        error("%s: invalid number of arguments", objName);
        return 0;
    }
    if (data.width < 0) {
        
        error("%s: width out of range", objName);
        return 0;
    }
    if (data.height < 0) {
        
        error("%s: height out of range", objName);
        return 0;
    }
    return 1;
}

bool getNameDimen2dElemFromArgs(const int argc, const t_atom *argv,
                                t_ofeliaNameDimen2dElem &elem, const char *objName)
{
    switch (argc) {
            
        case 0:
        {
            elem.varName.name = "";
            elem.width = 0.0f;
            elem.height = 0.0f;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_SYMBOL) {
                
                elem.varName.name = argv[0].a_w.w_symbol->s_name;
                getVarNameLocalized(elem.varName);
                getVarNameIndexed(elem.varName);
                elem.width = 0.0f;
                elem.height = 0.0f;
            }
            else {
                
                error("%s: wrong argument type", objName);
                return 0;
            }
            break;
        }
        case 3:
        {
            if (argv[0].a_type == A_SYMBOL &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT) {
                
                elem.varName.name = argv[0].a_w.w_symbol->s_name;
                getVarNameLocalized(elem.varName);
                getVarNameIndexed(elem.varName);
                elem.width = argv[1].a_w.w_float;
                elem.height = argv[2].a_w.w_float;
            }
            else {
                
                error("%s: wrong argument type", objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", objName);
            return 0;
        }
    }
    return 1;
}

bool getNameDimen3dElemFromArgs(const int argc, const t_atom *argv,
                                t_ofeliaNameDimen3dElem &elem, const char *objName)
{
    switch (argc) {
            
        case 0:
        {
            elem.varName.name = "";
            elem.width = 0.0f;
            elem.height = 0.0f;
            elem.depth = 0.0f;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_SYMBOL) {
                
                elem.varName.name = argv[0].a_w.w_symbol->s_name;
                getVarNameLocalized(elem.varName);
                getVarNameIndexed(elem.varName);
                elem.width = 0.0f;
                elem.height = 0.0f;
                elem.depth = 0.0f;
            }
            else {
                
                error("%s: wrong argument type", objName);
                return 0;
            }
            break;
        }
        case 4:
        {
            if (argv[0].a_type == A_SYMBOL &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT) {
                
                elem.varName.name = argv[0].a_w.w_symbol->s_name;
                getVarNameLocalized(elem.varName);
                getVarNameIndexed(elem.varName);
                elem.width = argv[1].a_w.w_float;
                elem.height = argv[2].a_w.w_float;
                elem.depth = argv[3].a_w.w_float;
            }
            else {
                
                error("%s: wrong argument type", objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", objName);
            return 0;
        }
    }
    return 1;
}

bool getVec2fFromArgs(const int argc, const t_atom *argv,
                      ofVec2f &vec, const char *objName)
{
    switch (argc) {
            
        case 0:
        {
            vec.set(0.0f, 0.0f);
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT) {
                
                vec.set(argv[0].a_w.w_float,
                        argv[1].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", objName);
            return 0;
        }
    }
    return 1;
}

bool getVec3fFromArgs(const int argc, const t_atom *argv,
                      ofVec3f &vec, const char *objName)
{
    switch (argc) {
            
        case 0:
        {
            vec.set(0.0f, 0.0f, 0.0f);
            break;
        }
        case 3:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT) {
                
                vec.set(argv[0].a_w.w_float,
                        argv[1].a_w.w_float,
                        argv[2].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", objName);
            return 0;
        }
    }
    return 1;
}

bool getToggleFromSym(const t_symbol *s, bool &toggle, const char *objName)
{
    const char *mode = s->s_name;
    
    if (!strcmp(mode, "OFF"))
        toggle = false;
    else if (!strcmp(mode, "ON"))
        toggle = true;
    else {
        
        error("%s: no method for '%s'", objName, mode);
        return 0;
    }
    return 1;
}

bool getToggleFromArgs(const int argc, const t_atom *argv,
                       bool &toggle, const char *objName)
{
    if (argc == 1) {
        
        if (argv[0].a_type == A_FLOAT) {
            
            toggle = argv[0].a_w.w_float != 0.0f;
        }
        else if (argv[0].a_type == A_SYMBOL) {
            
            const char *mode = argv[0].a_w.w_symbol->s_name;
            
            if (!strcmp(mode, "OFF"))
                toggle = false;
            else if (!strcmp(mode, "ON"))
                toggle = true;
            else {
             
                error("%s: no method for '%s'", objName, mode);
                return 0;
            }
        }
        else {
            
            error("%s: wrong argument type", objName);
            return 0;
        }
    }
    else {
        
        error("%s: invalid number of arguments", objName);
        return 0;
    }
    return 1;
}

void printToggle(const char *name, const bool state)
{
    if (!state)
        post("%s : OFF", name);
    else
        post("%s : ON", name);
}

