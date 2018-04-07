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

#include "ofeliaShader.h"
#include "ofeliaWindow.h"
#include "ofeliaImage.h"
#include "ofeliaFbo.h"

/* ________________________________________________________________________________
 * static variables
 */
const char *t_ofeliaLoadShader::objName = "ofLoadShader";
unsigned int t_ofeliaLoadShader::counter;
vector<t_ofeliaLoadShaderData> t_ofeliaLoadShader::shaderData;
vector<vector<shared_ptr<ofShader>>> t_ofeliaLoadShader::shaders;
bool t_ofeliaLoadShader::bInited;
const char *t_ofeliaApplyShader::objName = "ofApplyShader";
ofShader *t_ofeliaApplyShader::currentShader;
const char *t_ofeliaDoesShaderNameExist::objName = "ofDoesShaderNameExist";
const char *t_ofeliaGetShaderPath::objName = "ofGetShaderPath";
const char *t_ofeliaIsShaderLoaded::objName = "ofIsShaderLoaded";
const char *t_ofeliaSetShaderUniform1i::objName = "ofSetShaderUniform1i";
const char *t_ofeliaSetShaderUniform2i::objName = "ofSetShaderUniform2i";
const char *t_ofeliaSetShaderUniform3i::objName = "ofSetShaderUniform3i";
const char *t_ofeliaSetShaderUniform4i::objName = "ofSetShaderUniform4i";
const char *t_ofeliaSetShaderUniform1f::objName = "ofSetShaderUniform1f";
const char *t_ofeliaSetShaderUniform2f::objName = "ofSetShaderUniform2f";
const char *t_ofeliaSetShaderUniform3f::objName = "ofSetShaderUniform3f";
const char *t_ofeliaSetShaderUniform4f::objName = "ofSetShaderUniform4f";
const char *t_ofeliaSetShaderUniform1iv::objName = "ofSetShaderUniform1iv";
const char *t_ofeliaSetShaderUniform2iv::objName = "ofSetShaderUniform2iv";
const char *t_ofeliaSetShaderUniform3iv::objName = "ofSetShaderUniform3iv";
const char *t_ofeliaSetShaderUniform4iv::objName = "ofSetShaderUniform4iv";
const char *t_ofeliaSetShaderUniform1fv::objName = "ofSetShaderUniform1fv";
const char *t_ofeliaSetShaderUniform2fv::objName = "ofSetShaderUniform2fv";
const char *t_ofeliaSetShaderUniform3fv::objName = "ofSetShaderUniform3fv";
const char *t_ofeliaSetShaderUniform4fv::objName = "ofSetShaderUniform4fv";
const char *t_ofeliaSetShaderUniformTex::objName = "ofSetShaderUniformTex";
const char *t_ofeliaSetShaderAttribute1f::objName = "ofSetShaderAttribute1f";
const char *t_ofeliaSetShaderAttribute2f::objName = "ofSetShaderAttribute2f";
const char *t_ofeliaSetShaderAttribute3f::objName = "ofSetShaderAttribute3f";
const char *t_ofeliaSetShaderAttribute4f::objName = "ofSetShaderAttribute4f";
const char *t_ofeliaSetShaderAttribute1fv::objName = "ofSetShaderAttribute1fv";
const char *t_ofeliaSetShaderAttribute2fv::objName = "ofSetShaderAttribute2fv";
const char *t_ofeliaSetShaderAttribute3fv::objName = "ofSetShaderAttribute3fv";
const char *t_ofeliaSetShaderAttribute4fv::objName = "ofSetShaderAttribute4fv";

/* ________________________________________________________________________________
 * ofLoadShader object methods
 */
void ofeliaLoadShader_clear(t_ofeliaLoadShader *x);
void ofeliaLoadShader_vecSizeOut(t_ofeliaLoadShader *x);

int getPositionByShaderObjID(const unsigned int objID)
{
    const auto it = find_if(begin(t_ofeliaLoadShader::shaderData), end(t_ofeliaLoadShader::shaderData),
                            [&objID](const t_ofeliaLoadShaderData &shaderData)
                            {
                                return shaderData.objID == objID;
                            });
    return static_cast<int>(it - t_ofeliaLoadShader::shaderData.begin());
}

bool getLoadShaderPathFromArgs(t_ofeliaLoadShader *x, const int argc, const t_atom *argv, t_string &path)
{
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            path = argv[0].a_w.w_symbol->s_name;
            
            if (doesFilenameHaveExtension(path)) {
                
                error("%s: wrong file format '%s'", t_ofeliaLoadShader::objName, path.c_str());
                return 0;
            }
            t_string vertPath = path + ".vert";
            
            if (!makePathAbsolute(vertPath, x->canvas)) {
                
                error("%s: failed to find '%s'", t_ofeliaLoadShader::objName, vertPath.c_str());
                return 0;
            }
            t_string fragPath = path + ".frag";
            
            if (!makePathAbsolute(fragPath, x->canvas)) {
                
                error("%s: failed to find '%s'", t_ofeliaLoadShader::objName, fragPath.c_str());
                return 0;
            }
            path = fragPath.substr(0, fragPath.length()-5);
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadShader::objName);
            return 0;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadShader::objName);
        return 0;
    }
    return 1;
}

bool getLoadShaderPathsFromArgs(t_ofeliaLoadShader *x, const int argc, const t_atom *argv, vector<t_string> &paths)
{
    if (argc) {
        
        for (int i=0; i<argc; ++i) {
            
            if (argv[i].a_type == A_SYMBOL) {
                
                t_string path = argv[i].a_w.w_symbol->s_name;
                
                if (doesFilenameHaveExtension(path)) {
                    
                    error("%s: wrong file format '%s'", t_ofeliaLoadShader::objName, path.c_str());
                    return 0;
                }
                t_string vertPath = path + ".vert";
                
                if (!makePathAbsolute(vertPath, x->canvas)) {
                    
                    error("%s: failed to find '%s'", t_ofeliaLoadShader::objName, vertPath.c_str());
                    return 0;
                }
                t_string fragPath = path + ".frag";
                
                if (!makePathAbsolute(fragPath, x->canvas)) {
                    
                    error("%s: failed to find '%s'", t_ofeliaLoadShader::objName, fragPath.c_str());
                    return 0;
                }
                path = fragPath.substr(0, fragPath.length()-5);
                paths.push_back(path);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaLoadShader::objName);
                return 0;
            }
        }
        return 1;
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadShader::objName);
        return 0;
    }
    return 1;
}

bool getCmdRangeFromArgs(const int argc, const t_atom *argv, t_ofeliaLoadShaderCmdData &cmd)
{
    switch (argc) {
            
        case 0:
        {
            cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                cmd.fromIndex = cmd.toIndex = truncf(argv[0].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaLoadShader::objName);
                return 0;
            }
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT) {
                
                cmd.fromIndex = truncf(argv[0].a_w.w_float);
                cmd.toIndex = truncf(argv[1].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaLoadShader::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaLoadShader::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaLoadShader_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadShader *x = reinterpret_cast<t_ofeliaLoadShader*>(pd_new(ofeliaLoadShader_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    t_ofeliaLoadShaderData shaderData;
    getVarNameLocalPrefixes(shaderData.varName);
    vector<t_string> paths;
    x->canvas = canvas_getcurrent();
    
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            shaderData.varName.name = argv[0].a_w.w_symbol->s_name;
            getVarNameLocalized(shaderData.varName);
            
            if (--argc) {
                
                if (!getLoadShaderPathsFromArgs(x, argc, argv+1, paths))
                    return 0;
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadShader::objName);
            return 0;
        }
    }
    x->failPathsShouldClear = false;
    x->vecSizesShouldClear = false;
    x->shouldOutlet = false;
    x->bInitGate = false;
    x->shdrID = 0;
    x->objID = shaderData.objID = t_ofeliaLoadShader::counter++;
    t_ofeliaLoadShader::shaderData.push_back(shaderData);
    t_ofeliaLoadShader::shaders.push_back(vector<shared_ptr<ofShader>>());
    x->shaderLoaded = make_unique<vector<char>>();
    x->shaderIDs = make_unique<vector<unsigned int>>();
    x->tempShaderIDs = make_unique<vector<unsigned int>>();
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::updateSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    x->jobDone_out = outlet_new(&x->x_obj, &s_bang);
    x->vecSize_out = outlet_new(&x->x_obj, &s_float);
    
    if (argc) {
        
        ofeliaLoadShader_clear(x);
        
        for (size_t i=0; i<paths.size(); ++i) {
            
            t_ofeliaLoadShaderCmdData cmd;
            cmd.path = paths[i];
            cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
            cmd.state = SHADER_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
    return (x);
}

void ofeliaLoadShader_init(t_ofeliaLoadShader *x)
{
    if (!t_ofeliaLoadShader::bInited && x->bInitGate) {
        
        t_ofeliaLoadShader::shaders.clear();
        
        for (size_t i=0; i<t_ofeliaLoadShader::shaderData.size(); ++i) {
            
            t_ofeliaLoadShader::shaders.push_back(vector<shared_ptr<ofShader>>());
            
            for (size_t j=0; j<t_ofeliaLoadShader::shaderData[i].paths.size(); ++j)
                t_ofeliaLoadShader::shaders[i].push_back(make_shared<ofShader>());
        }
        t_ofeliaLoadShader::bInited = true;
    }
}

void ofeliaLoadShader_update(t_ofeliaLoadShader *x)
{
    const int pos = getPositionByShaderObjID(x->objID);
    
    if (!x->cmdVec.empty()) {
        
        const size_t cmdVecSize = x->cmdVec.size();
        
        /* read all recorded commands */
        for (size_t i=0; i<cmdVecSize; ++i) {
            
            const int size = static_cast<int>(t_ofeliaLoadShader::shaderData[pos].paths.size());
            const auto pathsBegin = t_ofeliaLoadShader::shaderData[pos].paths.begin();
            const auto shadersBegin = t_ofeliaLoadShader::shaders[pos].begin();
            const auto shaderLoadedBegin = x->shaderLoaded->begin();
            const auto shaderIDsBegin = x->shaderIDs->begin();
            const t_string &path = x->cmdVec[i].path;
            
            switch (x->cmdVec[i].state) {
                    
                case SHADER_LOAD_CMD_INSERT:
                {
                    const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, size));
                    const int insertSize = static_cast<int>(max(0.0f, max(0.0f, x->cmdVec[i].toIndex) - max(0.0f, x->cmdVec[i].fromIndex))) + 1;
                    t_ofeliaLoadShader::shaderData[pos].paths.insert(pathsBegin + fromIndex, insertSize, path);
                    t_ofeliaLoadShader::shaders[pos].insert(shadersBegin + fromIndex, insertSize, make_shared<ofShader>());
                    x->shaderLoaded->insert(shaderLoadedBegin + fromIndex, insertSize, 0);
                    x->shaderIDs->insert(shaderIDsBegin + fromIndex, insertSize, 0);
                    break;
                }
                case SHADER_LOAD_CMD_ERASE:
                {
                    if (size) {
                        
                        const int maxIndex = size - 1;
                        const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, maxIndex));
                        const int toIndex = static_cast<int>(ofClamp(x->cmdVec[i].toIndex, fromIndex, maxIndex)) + 1;
                        t_ofeliaLoadShader::shaderData[pos].paths.erase(pathsBegin + fromIndex, pathsBegin + toIndex);
                        t_ofeliaLoadShader::shaders[pos].erase(shadersBegin + fromIndex, shadersBegin + toIndex);
                        x->shaderLoaded->erase(shaderLoadedBegin + fromIndex, shaderLoadedBegin + toIndex);
                        x->shaderIDs->erase(shaderIDsBegin + fromIndex, shaderIDsBegin + toIndex);
                        x->shouldOutlet = true;
                    }
                    break;
                }
                case SHADER_LOAD_CMD_FILL:
                {
                    if (size) {
                        
                        const int maxIndex = size - 1;
                        const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, maxIndex));
                        const int toIndex = static_cast<int>(ofClamp(x->cmdVec[i].toIndex, fromIndex, maxIndex)) + 1;
                        fill(pathsBegin + fromIndex, pathsBegin + toIndex, path);
                        fill(shaderIDsBegin + fromIndex, shaderIDsBegin + toIndex, 0);
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
        /* find shader elements to load. store IDs */
        for (size_t i=0; i<x->shaderIDs->size(); ++i) {
            
            if (x->shaderIDs->at(i) == 0) {
                
                x->shdrID++;
                x->shaderIDs->at(i) = x->shdrID;
                x->tempShaderIDs->push_back(x->shdrID);
            }
        }
        if (x->cmdVec.size() > cmdVecSize) {
            
            x->cmdVec.erase(x->cmdVec.begin(), x->cmdVec.begin() + cmdVecSize);
            return;
        }
        x->cmdVec.clear();
    }
    if (x->failPathsShouldClear) {
        
        x->failPaths.clear();
        x->failPathsShouldClear = false;
    }
    if (!x->tempShaderIDs->empty()) {
        
        /* load shaders based on stored IDs */
        for (size_t i=0; i<x->shaderIDs->size(); ++i) {
            
            if (x->shaderIDs->at(i) == x->tempShaderIDs->at(0)) {
                
                t_ofeliaLoadShader::shaders[pos][i]->load(t_ofeliaLoadShader::shaderData[pos].paths[i].c_str());
                
                if (!t_ofeliaLoadShader::shaders[pos][i]->isLoaded())
                    x->failPaths.push_back(t_ofeliaLoadShader::shaderData[pos].paths[i]);
                x->shaderLoaded->at(i) = 1;
                x->shouldOutlet = true;
                break;
            }
        }
        x->tempShaderIDs->erase(x->tempShaderIDs->begin());
    }
    /* output size when there's change. report error if loading fails */
    if (x->shouldOutlet) {
        
        if (x->vecSizesShouldClear) {
            
            x->vecSizes.clear();
            x->vecSizesShouldClear = false;
        }
        const int numLoadedShaders = count_if(x->shaderLoaded->begin(),
                                            x->shaderLoaded->end(),
                                            [] (char c)
                                            {
                                                return c != 0;
                                            });
        x->vecSizes.push_back(numLoadedShaders);
        ofeliaLoadShader_vecSizeOut(x);
        x->shouldOutlet = false;
    }
}

void ofeliaLoadShader_exit(t_ofeliaLoadShader *x)
{
    if (t_ofeliaLoadShader::bInited)
        t_ofeliaLoadShader::bInited = false;
    fill(x->shaderLoaded->begin(), x->shaderLoaded->end(), 0); /* mark unloaded */
    *x->tempShaderIDs = *x->shaderIDs; /* copy for reloading */
    x->vecSizes.clear();
    x->failPathsShouldClear = false;
    x->vecSizesShouldClear = false;
    x->shouldOutlet = false;
    x->bInitGate = true;
}

void ofeliaLoadShader_vecSizeOut(t_ofeliaLoadShader *x)
{
    if (!x->failPaths.empty()) {
        
        for (size_t i=0; i<x->failPaths.size(); ++i)
            error("%s: failed to load '%s'", t_ofeliaLoadShader::objName, x->failPaths[i].c_str());
        x->failPathsShouldClear = true;
    }
    if (!x->vecSizes.empty()) {
        
        for (size_t i=0; i<x->vecSizes.size(); ++i)
            outlet_float(x->vecSize_out, static_cast<t_float>(x->vecSizes[i]));
        x->vecSizesShouldClear = true;
    }
    if (x->tempShaderIDs->empty())
        outlet_bang(x->jobDone_out);
}

void ofeliaLoadShader_name(t_ofeliaLoadShader *x, t_symbol *s)
{
    const int pos = getPositionByShaderObjID(x->objID);
    t_ofeliaLoadShader::shaderData[pos].varName.name = s->s_name;
    getVarNameLocalized(t_ofeliaLoadShader::shaderData[pos].varName);
}

void ofeliaLoadShader_load(t_ofeliaLoadShader *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_string> paths;
    
    if (getLoadShaderPathsFromArgs(x, argc, argv, paths)) {
        
        ofeliaLoadShader_clear(x);
        
        for (size_t i=0; i<paths.size(); ++i) {
            
            t_ofeliaLoadShaderCmdData cmd;
            cmd.path = paths[i];
            cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
            cmd.state = SHADER_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadShader_add(t_ofeliaLoadShader *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadShaderCmdData cmd;
    
    if (getLoadShaderPathFromArgs(x, argc, argv, cmd.path)) {
        
        cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
        cmd.state = SHADER_LOAD_CMD_INSERT;
        x->cmdVec.push_back(cmd);
    }
}

void ofeliaLoadShader_append(t_ofeliaLoadShader *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_string> paths;
    
    if (getLoadShaderPathsFromArgs(x, argc, argv, paths)) {
        
        for (size_t i=0; i<paths.size(); ++i) {
            
            t_ofeliaLoadShaderCmdData cmd;
            cmd.path = paths[i];
            cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
            cmd.state = SHADER_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadShader_prepend(t_ofeliaLoadShader *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_string> paths;
    
    if (getLoadShaderPathsFromArgs(x, argc, argv, paths)) {
        
        for (size_t i=0; i<paths.size(); ++i) {
            
            t_ofeliaLoadShaderCmdData cmd;
            cmd.path = paths[i];
            cmd.fromIndex = cmd.toIndex = static_cast<t_float>(i);
            cmd.state = SHADER_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadShader_insert(t_ofeliaLoadShader *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadShaderCmdData cmd;
    
    if (getLoadShaderPathFromArgs(x, argc, argv, cmd.path)) {
        
        if (getCmdRangeFromArgs(argc-1, argv+1, cmd)) {
            
            cmd.state = SHADER_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadShader_fill(t_ofeliaLoadShader *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadShaderCmdData cmd;
    
    if (getLoadShaderPathFromArgs(x, argc, argv, cmd.path)) {
        
        if (getCmdRangeFromArgs(argc-1, argv+1, cmd)) {
            
            cmd.state = SHADER_LOAD_CMD_FILL;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadShader_erase(t_ofeliaLoadShader *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadShaderCmdData cmd;
    
    if (getCmdRangeFromArgs(argc, argv, cmd)) {
        
        cmd.state = SHADER_LOAD_CMD_ERASE;
        x->cmdVec.push_back(cmd);
    }
}

void ofeliaLoadShader_clear(t_ofeliaLoadShader *x)
{
    t_ofeliaLoadShaderCmdData cmd;
    cmd.fromIndex = 0.0f;
    cmd.toIndex = numeric_limits<t_float>::max();
    cmd.state = SHADER_LOAD_CMD_ERASE;
    x->cmdVec.push_back(cmd);
}

void ofeliaLoadShader_set(t_ofeliaLoadShader *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            ofeliaLoadShader_name(x, argv[0].a_w.w_symbol);
            
            if (--argc)
                ofeliaLoadShader_load(x, 0, argc, argv+1);
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadShader::objName);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadShader::objName);
    }
}

void ofeliaLoadShader_print(t_ofeliaLoadShader *x)
{
    post("\n[%s]", t_ofeliaLoadShader::objName);
    const int pos = getPositionByShaderObjID(x->objID);
    post("name : %s", t_ofeliaLoadShader::shaderData[pos].varName.name.c_str());
    post("load : ");
    
    for (size_t i=0; i<t_ofeliaLoadShader::shaderData[pos].paths.size(); ++i)
        post(" %s", t_ofeliaLoadShader::shaderData[pos].paths[i].c_str());
}

void ofeliaLoadShader_free(t_ofeliaLoadShader *x)
{
    const int pos = getPositionByShaderObjID(x->objID);
    t_ofeliaLoadShader::shaderData.erase(t_ofeliaLoadShader::shaderData.begin() + pos);
    t_ofeliaLoadShader::shaders.erase(t_ofeliaLoadShader::shaders.begin() + pos);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::updateSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    outlet_free(x->jobDone_out);
    outlet_free(x->vecSize_out);
}

void ofeliaLoadShader_setup()
{
    ofeliaLoadShader_class = class_new(gensym("ofLoadShader"),
                                     reinterpret_cast<t_newmethod>(ofeliaLoadShader_new),
                                     reinterpret_cast<t_method>(ofeliaLoadShader_free),
                                     sizeof(t_ofeliaLoadShader),
                                     CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaLoadShader_class, reinterpret_cast<t_method>(ofeliaLoadShader_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaLoadShader_class, reinterpret_cast<t_method>(ofeliaLoadShader_update),
                    t_ofeliaWindow::updateMess, A_NULL, 0);
    class_addmethod(ofeliaLoadShader_class, reinterpret_cast<t_method>(ofeliaLoadShader_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addmethod(ofeliaLoadShader_class, reinterpret_cast<t_method>(ofeliaLoadShader_name),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaLoadShader_class, reinterpret_cast<t_method>(ofeliaLoadShader_load),
                    gensym("load"), A_GIMME, 0);
    class_addmethod(ofeliaLoadShader_class, reinterpret_cast<t_method>(ofeliaLoadShader_add),
                    gensym("add"), A_GIMME, 0);
    class_addmethod(ofeliaLoadShader_class, reinterpret_cast<t_method>(ofeliaLoadShader_append),
                    gensym("append"), A_GIMME, 0);
    class_addmethod(ofeliaLoadShader_class, reinterpret_cast<t_method>(ofeliaLoadShader_prepend),
                    gensym("prepend"), A_GIMME, 0);
    class_addmethod(ofeliaLoadShader_class, reinterpret_cast<t_method>(ofeliaLoadShader_insert),
                    gensym("insert"), A_GIMME, 0);
    class_addmethod(ofeliaLoadShader_class, reinterpret_cast<t_method>(ofeliaLoadShader_fill),
                    gensym("fill"), A_GIMME, 0);
    class_addmethod(ofeliaLoadShader_class, reinterpret_cast<t_method>(ofeliaLoadShader_erase),
                    gensym("erase"), A_GIMME, 0);
    class_addmethod(ofeliaLoadShader_class, reinterpret_cast<t_method>(ofeliaLoadShader_clear),
                    gensym("clear"), A_NULL, 0);
    class_addmethod(ofeliaLoadShader_class, reinterpret_cast<t_method>(ofeliaLoadShader_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaLoadShader_class, reinterpret_cast<t_method>(ofeliaLoadShader_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * common methods
 */
int getPositionByShaderName(const t_string &name)
{
    auto it = find_if(begin(t_ofeliaLoadShader::shaderData), end(t_ofeliaLoadShader::shaderData),
                      [&name] (const t_ofeliaLoadShaderData &shaderData) {
                          return (shaderData.varName.name == name);
                      });
    if (it == end(t_ofeliaLoadShader::shaderData))
        return -1;
    return static_cast<int>(it - begin(t_ofeliaLoadShader::shaderData));
}

/* ________________________________________________________________________________
 * ofApplyShader object methods
 */
void *ofeliaApplyShader_new(t_symbol *s)
{
    t_ofeliaApplyShader *x = reinterpret_cast<t_ofeliaApplyShader*>(pd_new(ofeliaApplyShader_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaApplyShader_bang(t_ofeliaApplyShader *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const t_string &name = x->varName.name;
        
        if (!name.empty()) {
            
            const int pos = getPositionByShaderName(name);
            
            if (pos != -1 && !t_ofeliaLoadShader::shaderData[pos].paths.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadShader::shaderData[pos].paths.size())-1);
                
                if (!t_ofeliaLoadShader::shaders[pos].empty() &&
                    index < static_cast<int>(t_ofeliaLoadShader::shaders[pos].size())) {
         
                    if (t_ofeliaLoadShader::shaders[pos][index]->isLoaded()) {
                        
                        t_ofeliaLoadShader::shaders[pos][index]->begin();
                        t_ofeliaApplyShader::currentShader = t_ofeliaLoadShader::shaders[pos][index].get();
                        outlet_bang(x->x_obj.ob_outlet);
                        t_ofeliaApplyShader::currentShader = nullptr;
                        t_ofeliaLoadShader::shaders[pos][index]->end();
                        return;
                    }
                }
            }
        }
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaApplyShader::objName);
    }
}

void ofeliaApplyShader_float(t_ofeliaApplyShader *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaApplyShader_set(t_ofeliaApplyShader *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaApplyShader_print(t_ofeliaApplyShader *x)
{
    post("\n[%s]", t_ofeliaApplyShader::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaApplyShader_setup()
{
    ofeliaApplyShader_class = class_new(gensym("ofApplyShader"),
                                        reinterpret_cast<t_newmethod>(ofeliaApplyShader_new),
                                        0, sizeof(t_ofeliaApplyShader),
                                        CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaApplyShader_class, reinterpret_cast<t_method>(ofeliaApplyShader_bang));
    class_addfloat(ofeliaApplyShader_class, reinterpret_cast<t_method>(ofeliaApplyShader_float));
    class_addmethod(ofeliaApplyShader_class, reinterpret_cast<t_method>(ofeliaApplyShader_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaApplyShader_class, reinterpret_cast<t_method>(ofeliaApplyShader_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaApplyShader_class, reinterpret_cast<t_method>(ofeliaApplyShader_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofDoesShaderNameExist object methods
 */
void *ofeliaDoesShaderNameExist_new(t_symbol *s)
{
    t_ofeliaDoesShaderNameExist *x = reinterpret_cast<t_ofeliaDoesShaderNameExist*>(pd_new(ofeliaDoesShaderNameExist_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaDoesShaderNameExist_bang(t_ofeliaDoesShaderNameExist *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        if (getPositionByShaderName(name) != -1)
            outlet_float(x->x_obj.ob_outlet, 1.0f);
        else
            outlet_float(x->x_obj.ob_outlet, 0.0f);
    }
    else {
        
        error("%s: name not assigned", t_ofeliaDoesShaderNameExist::objName);
    }
}

void ofeliaDoesShaderNameExist_set(t_ofeliaDoesShaderNameExist *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaDoesShaderNameExist_print(t_ofeliaDoesShaderNameExist *x)
{
    post("\n[%s]", t_ofeliaDoesShaderNameExist::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaDoesShaderNameExist_setup()
{
    ofeliaDoesShaderNameExist_class = class_new(gensym("ofDoesShaderNameExist"),
                                                reinterpret_cast<t_newmethod>(ofeliaDoesShaderNameExist_new),
                                                0, sizeof(t_ofeliaDoesShaderNameExist),
                                                CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaDoesShaderNameExist_class, reinterpret_cast<t_method>(ofeliaDoesShaderNameExist_bang));
    class_addmethod(ofeliaDoesShaderNameExist_class, reinterpret_cast<t_method>(ofeliaDoesShaderNameExist_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesShaderNameExist_class, reinterpret_cast<t_method>(ofeliaDoesShaderNameExist_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesShaderNameExist_class, reinterpret_cast<t_method>(ofeliaDoesShaderNameExist_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetShaderPath object methods
 */
void *ofeliaGetShaderPath_new(t_symbol *s)
{
    t_ofeliaGetShaderPath *x = reinterpret_cast<t_ofeliaGetShaderPath*>(pd_new(ofeliaGetShaderPath_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_symbol);
    return (x);
}

void ofeliaGetShaderPath_bang(t_ofeliaGetShaderPath *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByShaderName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadShader::shaderData[pos].paths.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadShader::shaderData[pos].paths.size())-1);
                outlet_symbol(x->x_obj.ob_outlet, gensym(t_ofeliaLoadShader::shaderData[pos].paths[index].c_str()));
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetShaderPath::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetShaderPath::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetShaderPath::objName);
    }
}

void ofeliaGetShaderPath_float(t_ofeliaGetShaderPath *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetShaderPath_set(t_ofeliaGetShaderPath *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetShaderPath_print(t_ofeliaGetShaderPath *x)
{
    post("\n[%s]", t_ofeliaGetShaderPath::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetShaderPath_setup()
{
    ofeliaGetShaderPath_class = class_new(gensym("ofGetShaderPath"),
                                          reinterpret_cast<t_newmethod>(ofeliaGetShaderPath_new),
                                          0, sizeof(t_ofeliaGetShaderPath),
                                          CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetShaderPath_class, reinterpret_cast<t_method>(ofeliaGetShaderPath_bang));
    class_addfloat(ofeliaGetShaderPath_class, reinterpret_cast<t_method>(ofeliaGetShaderPath_float));
    class_addmethod(ofeliaGetShaderPath_class, reinterpret_cast<t_method>(ofeliaGetShaderPath_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetShaderPath_class, reinterpret_cast<t_method>(ofeliaGetShaderPath_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetShaderPath_class, reinterpret_cast<t_method>(ofeliaGetShaderPath_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofIsShaderLoaded object methods
 */
void *ofeliaIsShaderLoaded_new(t_symbol *s)
{
    t_ofeliaIsShaderLoaded *x = reinterpret_cast<t_ofeliaIsShaderLoaded*>(pd_new(ofeliaIsShaderLoaded_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaIsShaderLoaded_bang(t_ofeliaIsShaderLoaded *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByShaderName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadShader::shaderData[pos].paths.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadShader::shaderData[pos].paths.size())-1);
                
                if (!t_ofeliaLoadShader::shaders[pos].empty() &&
                    index < static_cast<int>(t_ofeliaLoadShader::shaders[pos].size())) {
                    
                    if (t_ofeliaLoadShader::shaders[pos][index]->isLoaded())
                        outlet_float(x->x_obj.ob_outlet, 1.0f);
                    else
                        outlet_float(x->x_obj.ob_outlet, 0.0f);
                }
                else {
                    
                    outlet_float(x->x_obj.ob_outlet, 0.0f);
                }
            }
            else {
                
                outlet_float(x->x_obj.ob_outlet, 0.0f);
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaIsShaderLoaded::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaIsShaderLoaded::objName);
    }
}

void ofeliaIsShaderLoaded_float(t_ofeliaIsShaderLoaded *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaIsShaderLoaded_set(t_ofeliaIsShaderLoaded *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaIsShaderLoaded_print(t_ofeliaIsShaderLoaded *x)
{
    post("\n[%s]", t_ofeliaIsShaderLoaded::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaIsShaderLoaded_setup()
{
    ofeliaIsShaderLoaded_class = class_new(gensym("ofIsShaderLoaded"),
                                           reinterpret_cast<t_newmethod>(ofeliaIsShaderLoaded_new),
                                           0, sizeof(t_ofeliaIsShaderLoaded),
                                           CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaIsShaderLoaded_class, reinterpret_cast<t_method>(ofeliaIsShaderLoaded_bang));
    class_addfloat(ofeliaIsShaderLoaded_class, reinterpret_cast<t_method>(ofeliaIsShaderLoaded_float));
    class_addmethod(ofeliaIsShaderLoaded_class, reinterpret_cast<t_method>(ofeliaIsShaderLoaded_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaIsShaderLoaded_class, reinterpret_cast<t_method>(ofeliaIsShaderLoaded_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaIsShaderLoaded_class, reinterpret_cast<t_method>(ofeliaIsShaderLoaded_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetShaderUniform1i object methods
 */
bool getSetShaderUniform1iElemFromArgs(const int argc, const t_atom *argv, t_ofeliaSetShaderUniform1iElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.uniformName = "";
            elem.value = 0;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_SYMBOL) {

                elem.uniformName = argv[0].a_w.w_symbol->s_name;
                elem.value = 0;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSetShaderUniform1i::objName);
                return 0;
            }
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_SYMBOL &&
                argv[1].a_type == A_FLOAT) {
                
                elem.uniformName = argv[0].a_w.w_symbol->s_name;
                elem.value = static_cast<int>(argv[1].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSetShaderUniform1i::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaSetShaderUniform1i::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaSetShaderUniform1i_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetShaderUniform1i *x = reinterpret_cast<t_ofeliaSetShaderUniform1i*>(pd_new(ofeliaSetShaderUniform1i_class));
   
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);

    if (!getSetShaderUniform1iElemFromArgs(argc, argv, x->elem))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("v1"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetShaderUniform1i_bang(t_ofeliaSetShaderUniform1i *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (t_ofeliaApplyShader::currentShader)
            t_ofeliaApplyShader::currentShader->setUniform1i(x->elem.uniformName.c_str(), x->elem.value);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetShaderUniform1i::objName);
    }
}

void ofeliaSetShaderUniform1i_uniformName(t_ofeliaSetShaderUniform1i *x, t_symbol *s)
{
    x->elem.uniformName = s->s_name;
}

void ofeliaSetShaderUniform1i_v1(t_ofeliaSetShaderUniform1i *x, t_floatarg f)
{
    x->elem.value = static_cast<int>(f);
}

void ofeliaSetShaderUniform1i_set(t_ofeliaSetShaderUniform1i *x, t_symbol *s, int argc, t_atom *argv)
{
    getSetShaderUniform1iElemFromArgs(argc, argv, x->elem);
}

void ofeliaSetShaderUniform1i_print(t_ofeliaSetShaderUniform1i *x)
{
    post("\n[%s]", t_ofeliaSetShaderUniform1i::objName);
    post("uniformName : %s", x->elem.uniformName.c_str());
    post("v1 : %d", x->elem.value);
}

void ofeliaSetShaderUniform1i_setup()
{
    ofeliaSetShaderUniform1i_class = class_new(gensym("ofSetShaderUniform1i"),
                                               reinterpret_cast<t_newmethod>(ofeliaSetShaderUniform1i_new),
                                               0, sizeof(t_ofeliaSetShaderUniform1i),
                                               CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetShaderUniform1i_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform1i_bang));
    class_addmethod(ofeliaSetShaderUniform1i_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform1i_uniformName),
                    gensym("uniformName"), A_SYMBOL, 0);
    class_addmethod(ofeliaSetShaderUniform1i_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform1i_v1),
                    gensym("v1"), A_FLOAT, 0);
    class_addmethod(ofeliaSetShaderUniform1i_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform1i_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderUniform1i_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform1i_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetShaderUniform2i object methods
 */
bool getSetShaderUniform2iElemFromArgs(const int argc, const t_atom *argv, t_ofeliaSetShaderUniform2iElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.uniformName = "";
            elem.value[0] = elem.value[1] = 0;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_SYMBOL) {
                
                elem.uniformName = argv[0].a_w.w_symbol->s_name;
                elem.value[0] = elem.value[1] = 0;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSetShaderUniform2i::objName);
                return 0;
            }
            break;
        }
        case 3:
        {
            if (argv[0].a_type == A_SYMBOL &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT) {
                
                elem.uniformName = argv[0].a_w.w_symbol->s_name;
                elem.value[0] = static_cast<int>(argv[1].a_w.w_float);
                elem.value[1] = static_cast<int>(argv[2].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSetShaderUniform2i::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaSetShaderUniform2i::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaSetShaderUniform2i_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetShaderUniform2i *x = reinterpret_cast<t_ofeliaSetShaderUniform2i*>(pd_new(ofeliaSetShaderUniform2i_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getSetShaderUniform2iElemFromArgs(argc, argv, x->elem))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("v1"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("v2"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetShaderUniform2i_bang(t_ofeliaSetShaderUniform2i *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (t_ofeliaApplyShader::currentShader)
            t_ofeliaApplyShader::currentShader->setUniform2i(x->elem.uniformName.c_str(), x->elem.value[0], x->elem.value[1]);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetShaderUniform2i::objName);
    }
}

void ofeliaSetShaderUniform2i_uniformName(t_ofeliaSetShaderUniform2i *x, t_symbol *s)
{
    x->elem.uniformName = s->s_name;
}

void ofeliaSetShaderUniform2i_v1(t_ofeliaSetShaderUniform2i *x, t_floatarg f)
{
    x->elem.value[0] = static_cast<int>(f);
}

void ofeliaSetShaderUniform2i_v2(t_ofeliaSetShaderUniform2i *x, t_floatarg f)
{
    x->elem.value[1] = static_cast<int>(f);
}

void ofeliaSetShaderUniform2i_set(t_ofeliaSetShaderUniform2i *x, t_symbol *s, int argc, t_atom *argv)
{
    getSetShaderUniform2iElemFromArgs(argc, argv, x->elem);
}

void ofeliaSetShaderUniform2i_print(t_ofeliaSetShaderUniform2i *x)
{
    post("\n[%s]", t_ofeliaSetShaderUniform2i::objName);
    post("uniformName : %s", x->elem.uniformName.c_str());
    post("v1 : %d", x->elem.value[0]);
    post("v2 : %d", x->elem.value[1]);
}

void ofeliaSetShaderUniform2i_setup()
{
    ofeliaSetShaderUniform2i_class = class_new(gensym("ofSetShaderUniform2i"),
                                               reinterpret_cast<t_newmethod>(ofeliaSetShaderUniform2i_new),
                                               0, sizeof(t_ofeliaSetShaderUniform2i),
                                               CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetShaderUniform2i_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform2i_bang));
    class_addmethod(ofeliaSetShaderUniform2i_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform2i_uniformName),
                    gensym("uniformName"), A_SYMBOL, 0);
    class_addmethod(ofeliaSetShaderUniform2i_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform2i_v1),
                    gensym("v1"), A_FLOAT, 0);
    class_addmethod(ofeliaSetShaderUniform2i_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform2i_v2),
                    gensym("v2"), A_FLOAT, 0);
    class_addmethod(ofeliaSetShaderUniform2i_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform2i_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderUniform2i_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform2i_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetShaderUniform3i object methods
 */
bool getSetShaderUniform3iElemFromArgs(const int argc, const t_atom *argv, t_ofeliaSetShaderUniform3iElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.uniformName = "";
            elem.value[0] = elem.value[1] = elem.value[2] = 0;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_SYMBOL) {

                elem.uniformName = argv[0].a_w.w_symbol->s_name;
                elem.value[0] = elem.value[1] = elem.value[2] = 0;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSetShaderUniform3i::objName);
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
                
                elem.uniformName = argv[0].a_w.w_symbol->s_name;
                elem.value[0] = static_cast<int>(argv[1].a_w.w_float);
                elem.value[1] = static_cast<int>(argv[2].a_w.w_float);
                elem.value[2] = static_cast<int>(argv[3].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSetShaderUniform3i::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaSetShaderUniform3i::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaSetShaderUniform3i_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetShaderUniform3i *x = reinterpret_cast<t_ofeliaSetShaderUniform3i*>(pd_new(ofeliaSetShaderUniform3i_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getSetShaderUniform3iElemFromArgs(argc, argv, x->elem))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("v1"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("v2"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("v3"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetShaderUniform3i_bang(t_ofeliaSetShaderUniform3i *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (t_ofeliaApplyShader::currentShader)
            t_ofeliaApplyShader::currentShader->setUniform3i(x->elem.uniformName.c_str(), x->elem.value[0], x->elem.value[1], x->elem.value[2]);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetShaderUniform3i::objName);
    }
}

void ofeliaSetShaderUniform3i_uniformName(t_ofeliaSetShaderUniform3i *x, t_symbol *s)
{
    x->elem.uniformName = s->s_name;
}

void ofeliaSetShaderUniform3i_v1(t_ofeliaSetShaderUniform3i *x, t_floatarg f)
{
    x->elem.value[0] = static_cast<int>(f);
}

void ofeliaSetShaderUniform3i_v2(t_ofeliaSetShaderUniform3i *x, t_floatarg f)
{
    x->elem.value[1] = static_cast<int>(f);
}

void ofeliaSetShaderUniform3i_v3(t_ofeliaSetShaderUniform3i *x, t_floatarg f)
{
    x->elem.value[2] = static_cast<int>(f);
}

void ofeliaSetShaderUniform3i_set(t_ofeliaSetShaderUniform3i *x, t_symbol *s, int argc, t_atom *argv)
{
    getSetShaderUniform3iElemFromArgs(argc, argv, x->elem);
}

void ofeliaSetShaderUniform3i_print(t_ofeliaSetShaderUniform3i *x)
{
    post("\n[%s]", t_ofeliaSetShaderUniform3i::objName);
    post("uniformName : %s", x->elem.uniformName.c_str());
    post("v1 : %d", x->elem.value[0]);
    post("v2 : %d", x->elem.value[1]);
    post("v3 : %d", x->elem.value[2]);
}

void ofeliaSetShaderUniform3i_setup()
{
    ofeliaSetShaderUniform3i_class = class_new(gensym("ofSetShaderUniform3i"),
                                               reinterpret_cast<t_newmethod>(ofeliaSetShaderUniform3i_new),
                                               0, sizeof(t_ofeliaSetShaderUniform3i),
                                               CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetShaderUniform3i_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform3i_bang));
    class_addmethod(ofeliaSetShaderUniform3i_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform3i_uniformName),
                    gensym("uniformName"), A_SYMBOL, 0);
    class_addmethod(ofeliaSetShaderUniform3i_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform3i_v1),
                    gensym("v1"), A_FLOAT, 0);
    class_addmethod(ofeliaSetShaderUniform3i_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform3i_v2),
                    gensym("v2"), A_FLOAT, 0);
    class_addmethod(ofeliaSetShaderUniform3i_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform3i_v3),
                    gensym("v3"), A_FLOAT, 0);
    class_addmethod(ofeliaSetShaderUniform3i_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform3i_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderUniform3i_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform3i_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetShaderUniform4i object methods
 */
bool getSetShaderUniform4iElemFromArgs(const int argc, const t_atom *argv, t_ofeliaSetShaderUniform4iElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.uniformName = "";
            elem.value[0] = elem.value[1] = elem.value[2] = elem.value[3] = 0;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_SYMBOL) {
                
                elem.uniformName = argv[0].a_w.w_symbol->s_name;
                elem.value[0] = elem.value[1] = elem.value[2] = elem.value[3] = 0;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSetShaderUniform4i::objName);
                return 0;
            }
            break;
        }
        case 5:
        {
            if (argv[0].a_type == A_SYMBOL &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT &&
                argv[4].a_type == A_FLOAT) {
                
                elem.uniformName = argv[0].a_w.w_symbol->s_name;
                elem.value[0] = static_cast<int>(argv[1].a_w.w_float);
                elem.value[1] = static_cast<int>(argv[2].a_w.w_float);
                elem.value[2] = static_cast<int>(argv[3].a_w.w_float);
                elem.value[3] = static_cast<int>(argv[4].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSetShaderUniform4i::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaSetShaderUniform4i::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaSetShaderUniform4i_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetShaderUniform4i *x = reinterpret_cast<t_ofeliaSetShaderUniform4i*>(pd_new(ofeliaSetShaderUniform4i_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getSetShaderUniform4iElemFromArgs(argc, argv, x->elem))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("v1"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("v2"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("v3"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("v4"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetShaderUniform4i_bang(t_ofeliaSetShaderUniform4i *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (t_ofeliaApplyShader::currentShader)
            t_ofeliaApplyShader::currentShader->setUniform4i(x->elem.uniformName.c_str(), x->elem.value[0], x->elem.value[1], x->elem.value[2], x->elem.value[3]);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetShaderUniform4i::objName);
    }
}

void ofeliaSetShaderUniform4i_uniformName(t_ofeliaSetShaderUniform4i *x, t_symbol *s)
{
    x->elem.uniformName = s->s_name;
}

void ofeliaSetShaderUniform4i_v1(t_ofeliaSetShaderUniform4i *x, t_floatarg f)
{
    x->elem.value[0] = static_cast<int>(f);
}

void ofeliaSetShaderUniform4i_v2(t_ofeliaSetShaderUniform4i *x, t_floatarg f)
{
    x->elem.value[1] = static_cast<int>(f);
}

void ofeliaSetShaderUniform4i_v3(t_ofeliaSetShaderUniform4i *x, t_floatarg f)
{
    x->elem.value[2] = static_cast<int>(f);
}

void ofeliaSetShaderUniform4i_v4(t_ofeliaSetShaderUniform4i *x, t_floatarg f)
{
    x->elem.value[3] = static_cast<int>(f);
}

void ofeliaSetShaderUniform4i_set(t_ofeliaSetShaderUniform4i *x, t_symbol *s, int argc, t_atom *argv)
{
    getSetShaderUniform4iElemFromArgs(argc, argv, x->elem);
}

void ofeliaSetShaderUniform4i_print(t_ofeliaSetShaderUniform4i *x)
{
    post("\n[%s]", t_ofeliaSetShaderUniform4i::objName);
    post("uniformName : %s", x->elem.uniformName.c_str());
    post("v1 : %d", x->elem.value[0]);
    post("v2 : %d", x->elem.value[1]);
    post("v3 : %d", x->elem.value[2]);
    post("v4 : %d", x->elem.value[3]);
}

void ofeliaSetShaderUniform4i_setup()
{
    ofeliaSetShaderUniform4i_class = class_new(gensym("ofSetShaderUniform4i"),
                                               reinterpret_cast<t_newmethod>(ofeliaSetShaderUniform4i_new),
                                               0, sizeof(t_ofeliaSetShaderUniform4i),
                                               CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetShaderUniform4i_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform4i_bang));
    class_addmethod(ofeliaSetShaderUniform4i_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform4i_uniformName),
                    gensym("uniformName"), A_SYMBOL, 0);
    class_addmethod(ofeliaSetShaderUniform4i_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform4i_v1),
                    gensym("v1"), A_FLOAT, 0);
    class_addmethod(ofeliaSetShaderUniform4i_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform4i_v2),
                    gensym("v2"), A_FLOAT, 0);
    class_addmethod(ofeliaSetShaderUniform4i_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform4i_v3),
                    gensym("v3"), A_FLOAT, 0);
    class_addmethod(ofeliaSetShaderUniform4i_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform4i_v4),
                    gensym("v4"), A_FLOAT, 0);
    class_addmethod(ofeliaSetShaderUniform4i_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform4i_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderUniform4i_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform4i_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetShaderUniform1f object methods
 */
bool getSetShaderUniform1fElemFromArgs(const int argc, const t_atom *argv, t_ofeliaSetShaderUniform1fElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.uniformName = "";
            elem.value = 0.0f;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_SYMBOL) {
                
                elem.uniformName = argv[0].a_w.w_symbol->s_name;
                elem.value = 0.0f;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSetShaderUniform1f::objName);
                return 0;
            }
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_SYMBOL &&
                argv[1].a_type == A_FLOAT) {
                
                elem.uniformName = argv[0].a_w.w_symbol->s_name;
                elem.value = argv[1].a_w.w_float;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSetShaderUniform1f::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaSetShaderUniform1f::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaSetShaderUniform1f_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetShaderUniform1f *x = reinterpret_cast<t_ofeliaSetShaderUniform1f*>(pd_new(ofeliaSetShaderUniform1f_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getSetShaderUniform1fElemFromArgs(argc, argv, x->elem))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("v1"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetShaderUniform1f_bang(t_ofeliaSetShaderUniform1f *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (t_ofeliaApplyShader::currentShader)
            t_ofeliaApplyShader::currentShader->setUniform1f(x->elem.uniformName.c_str(), x->elem.value);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetShaderUniform1f::objName);
    }
}

void ofeliaSetShaderUniform1f_uniformName(t_ofeliaSetShaderUniform1f *x, t_symbol *s)
{
    x->elem.uniformName = s->s_name;
}

void ofeliaSetShaderUniform1f_v1(t_ofeliaSetShaderUniform1f *x, t_floatarg f)
{
    x->elem.value = f;
}

void ofeliaSetShaderUniform1f_set(t_ofeliaSetShaderUniform1f *x, t_symbol *s, int argc, t_atom *argv)
{
    getSetShaderUniform1fElemFromArgs(argc, argv, x->elem);
}

void ofeliaSetShaderUniform1f_print(t_ofeliaSetShaderUniform1f *x)
{
    post("\n[%s]", t_ofeliaSetShaderUniform1f::objName);
    post("uniformName : %s", x->elem.uniformName.c_str());
    post("v1 : %g", x->elem.value);
}

void ofeliaSetShaderUniform1f_setup()
{
    ofeliaSetShaderUniform1f_class = class_new(gensym("ofSetShaderUniform1f"),
                                               reinterpret_cast<t_newmethod>(ofeliaSetShaderUniform1f_new),
                                               0, sizeof(t_ofeliaSetShaderUniform1f),
                                               CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetShaderUniform1f_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform1f_bang));
    class_addmethod(ofeliaSetShaderUniform1f_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform1f_uniformName),
                    gensym("uniformName"), A_SYMBOL, 0);
    class_addmethod(ofeliaSetShaderUniform1f_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform1f_v1),
                    gensym("v1"), A_FLOAT, 0);
    class_addmethod(ofeliaSetShaderUniform1f_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform1f_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderUniform1f_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform1f_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetShaderUniform2f object methods
 */
bool getSetShaderUniform2fElemFromArgs(const int argc, const t_atom *argv, t_ofeliaSetShaderUniform2fElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.uniformName = "";
            elem.value[0] = elem.value[1] = 0.0f;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_SYMBOL) {
                
                elem.uniformName = argv[0].a_w.w_symbol->s_name;
                elem.value[0] = elem.value[1] = 0.0f;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSetShaderUniform2f::objName);
                return 0;
            }
            break;
        }
        case 3:
        {
            if (argv[0].a_type == A_SYMBOL &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT) {
                
                elem.uniformName = argv[0].a_w.w_symbol->s_name;
                elem.value[0] = argv[1].a_w.w_float;
                elem.value[1] = argv[2].a_w.w_float;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSetShaderUniform2f::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaSetShaderUniform2f::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaSetShaderUniform2f_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetShaderUniform2f *x = reinterpret_cast<t_ofeliaSetShaderUniform2f*>(pd_new(ofeliaSetShaderUniform2f_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getSetShaderUniform2fElemFromArgs(argc, argv, x->elem))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("v1"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("v2"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetShaderUniform2f_bang(t_ofeliaSetShaderUniform2f *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (t_ofeliaApplyShader::currentShader)
            t_ofeliaApplyShader::currentShader->setUniform2f(x->elem.uniformName.c_str(), x->elem.value[0], x->elem.value[1]);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetShaderUniform2f::objName);
    }
}

void ofeliaSetShaderUniform2f_uniformName(t_ofeliaSetShaderUniform2f *x, t_symbol *s)
{
    x->elem.uniformName = s->s_name;
}

void ofeliaSetShaderUniform2f_v1(t_ofeliaSetShaderUniform2f *x, t_floatarg f)
{
    x->elem.value[0] = f;
}

void ofeliaSetShaderUniform2f_v2(t_ofeliaSetShaderUniform2f *x, t_floatarg f)
{
    x->elem.value[1] = f;
}

void ofeliaSetShaderUniform2f_set(t_ofeliaSetShaderUniform2f *x, t_symbol *s, int argc, t_atom *argv)
{
    getSetShaderUniform2fElemFromArgs(argc, argv, x->elem);
}

void ofeliaSetShaderUniform2f_print(t_ofeliaSetShaderUniform2f *x)
{
    post("\n[%s]", t_ofeliaSetShaderUniform2f::objName);
    post("uniformName : %s", x->elem.uniformName.c_str());
    post("v1 : %g", x->elem.value[0]);
    post("v2 : %g", x->elem.value[1]);
}

void ofeliaSetShaderUniform2f_setup()
{
    ofeliaSetShaderUniform2f_class = class_new(gensym("ofSetShaderUniform2f"),
                                               reinterpret_cast<t_newmethod>(ofeliaSetShaderUniform2f_new),
                                               0, sizeof(t_ofeliaSetShaderUniform2f),
                                               CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetShaderUniform2f_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform2f_bang));
    class_addmethod(ofeliaSetShaderUniform2f_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform2f_uniformName),
                    gensym("uniformName"), A_SYMBOL, 0);
    class_addmethod(ofeliaSetShaderUniform2f_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform2f_v1),
                    gensym("v1"), A_FLOAT, 0);
    class_addmethod(ofeliaSetShaderUniform2f_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform2f_v2),
                    gensym("v2"), A_FLOAT, 0);
    class_addmethod(ofeliaSetShaderUniform2f_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform2f_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderUniform2f_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform2f_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetShaderUniform3f object methods
 */
bool getSetShaderUniform3fElemFromArgs(const int argc, const t_atom *argv, t_ofeliaSetShaderUniform3fElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.uniformName = "";
            elem.value[0] = elem.value[1] = elem.value[2] = 0.0f;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_SYMBOL) {
                
                elem.uniformName = argv[0].a_w.w_symbol->s_name;
                elem.value[0] = elem.value[1] = elem.value[2] = 0.0f;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSetShaderUniform3f::objName);
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
                
                elem.uniformName = argv[0].a_w.w_symbol->s_name;
                elem.value[0] = argv[1].a_w.w_float;
                elem.value[1] = argv[2].a_w.w_float;
                elem.value[2] = argv[3].a_w.w_float;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSetShaderUniform3f::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaSetShaderUniform3f::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaSetShaderUniform3f_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetShaderUniform3f *x = reinterpret_cast<t_ofeliaSetShaderUniform3f*>(pd_new(ofeliaSetShaderUniform3f_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getSetShaderUniform3fElemFromArgs(argc, argv, x->elem))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("v1"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("v2"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("v3"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetShaderUniform3f_bang(t_ofeliaSetShaderUniform3f *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (t_ofeliaApplyShader::currentShader)
            t_ofeliaApplyShader::currentShader->setUniform3f(x->elem.uniformName.c_str(), x->elem.value[0], x->elem.value[1], x->elem.value[2]);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetShaderUniform3f::objName);
    }
}

void ofeliaSetShaderUniform3f_uniformName(t_ofeliaSetShaderUniform3f *x, t_symbol *s)
{
    x->elem.uniformName = s->s_name;
}

void ofeliaSetShaderUniform3f_v1(t_ofeliaSetShaderUniform3f *x, t_floatarg f)
{
    x->elem.value[0] = f;
}

void ofeliaSetShaderUniform3f_v2(t_ofeliaSetShaderUniform3f *x, t_floatarg f)
{
    x->elem.value[1] = f;
}

void ofeliaSetShaderUniform3f_v3(t_ofeliaSetShaderUniform3f *x, t_floatarg f)
{
    x->elem.value[2] = f;
}

void ofeliaSetShaderUniform3f_set(t_ofeliaSetShaderUniform3f *x, t_symbol *s, int argc, t_atom *argv)
{
    getSetShaderUniform3fElemFromArgs(argc, argv, x->elem);
}

void ofeliaSetShaderUniform3f_print(t_ofeliaSetShaderUniform3f *x)
{
    post("\n[%s]", t_ofeliaSetShaderUniform3f::objName);
    post("uniformName : %s", x->elem.uniformName.c_str());
    post("v1 : %g", x->elem.value[0]);
    post("v2 : %g", x->elem.value[1]);
    post("v3 : %g", x->elem.value[2]);
}

void ofeliaSetShaderUniform3f_setup()
{
    ofeliaSetShaderUniform3f_class = class_new(gensym("ofSetShaderUniform3f"),
                                               reinterpret_cast<t_newmethod>(ofeliaSetShaderUniform3f_new),
                                               0, sizeof(t_ofeliaSetShaderUniform3f),
                                               CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetShaderUniform3f_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform3f_bang));
    class_addmethod(ofeliaSetShaderUniform3f_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform3f_uniformName),
                    gensym("uniformName"), A_SYMBOL, 0);
    class_addmethod(ofeliaSetShaderUniform3f_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform3f_v1),
                    gensym("v1"), A_FLOAT, 0);
    class_addmethod(ofeliaSetShaderUniform3f_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform3f_v2),
                    gensym("v2"), A_FLOAT, 0);
    class_addmethod(ofeliaSetShaderUniform3f_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform3f_v3),
                    gensym("v3"), A_FLOAT, 0);
    class_addmethod(ofeliaSetShaderUniform3f_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform3f_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderUniform3f_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform3f_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetShaderUniform4f object methods
 */
bool getSetShaderUniform4fElemFromArgs(const int argc, const t_atom *argv, t_ofeliaSetShaderUniform4fElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.uniformName = "";
            elem.value[0] = elem.value[1] = elem.value[2] = elem.value[3] = 0.0f;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_SYMBOL) {
                
                elem.uniformName = argv[0].a_w.w_symbol->s_name;
                elem.value[0] = elem.value[1] = elem.value[2] = elem.value[3] = 0.0f;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSetShaderUniform4f::objName);
                return 0;
            }
            break;
        }
        case 5:
        {
            if (argv[0].a_type == A_SYMBOL &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT &&
                argv[4].a_type == A_FLOAT) {
                
                elem.uniformName = argv[0].a_w.w_symbol->s_name;
                elem.value[0] = argv[1].a_w.w_float;
                elem.value[1] = argv[2].a_w.w_float;
                elem.value[2] = argv[3].a_w.w_float;
                elem.value[3] = argv[4].a_w.w_float;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSetShaderUniform4f::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaSetShaderUniform4f::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaSetShaderUniform4f_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetShaderUniform4f *x = reinterpret_cast<t_ofeliaSetShaderUniform4f*>(pd_new(ofeliaSetShaderUniform4f_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);

    if (!getSetShaderUniform4fElemFromArgs(argc, argv, x->elem))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("v1"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("v2"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("v3"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("v4"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetShaderUniform4f_bang(t_ofeliaSetShaderUniform4f *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (t_ofeliaApplyShader::currentShader)
            t_ofeliaApplyShader::currentShader->setUniform4f(x->elem.uniformName.c_str(), x->elem.value[0], x->elem.value[1], x->elem.value[2], x->elem.value[3]);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetShaderUniform4f::objName);
    }
}

void ofeliaSetShaderUniform4f_uniformName(t_ofeliaSetShaderUniform4f *x, t_symbol *s)
{
    x->elem.uniformName = s->s_name;
}

void ofeliaSetShaderUniform4f_v1(t_ofeliaSetShaderUniform4f *x, t_floatarg f)
{
    x->elem.value[0] = f;
}

void ofeliaSetShaderUniform4f_v2(t_ofeliaSetShaderUniform4f *x, t_floatarg f)
{
    x->elem.value[1] = f;
}

void ofeliaSetShaderUniform4f_v3(t_ofeliaSetShaderUniform4f *x, t_floatarg f)
{
    x->elem.value[2] = f;
}

void ofeliaSetShaderUniform4f_v4(t_ofeliaSetShaderUniform4f *x, t_floatarg f)
{
    x->elem.value[3] = f;
}

void ofeliaSetShaderUniform4f_set(t_ofeliaSetShaderUniform4f *x, t_symbol *s, int argc, t_atom *argv)
{
    getSetShaderUniform4fElemFromArgs(argc, argv, x->elem);
}

void ofeliaSetShaderUniform4f_print(t_ofeliaSetShaderUniform4f *x)
{
    post("\n[%s]", t_ofeliaSetShaderUniform4f::objName);
    post("uniformName : %s", x->elem.uniformName.c_str());
    post("v1 : %g", x->elem.value[0]);
    post("v2 : %g", x->elem.value[1]);
    post("v3 : %g", x->elem.value[2]);
    post("v4 : %g", x->elem.value[3]);
}

void ofeliaSetShaderUniform4f_setup()
{
    ofeliaSetShaderUniform4f_class = class_new(gensym("ofSetShaderUniform4f"),
                                               reinterpret_cast<t_newmethod>(ofeliaSetShaderUniform4f_new),
                                               0, sizeof(t_ofeliaSetShaderUniform4f),
                                               CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetShaderUniform4f_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform4f_bang));
    class_addmethod(ofeliaSetShaderUniform4f_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform4f_uniformName),
                    gensym("uniformName"), A_SYMBOL, 0);
    class_addmethod(ofeliaSetShaderUniform4f_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform4f_v1),
                    gensym("v1"), A_FLOAT, 0);
    class_addmethod(ofeliaSetShaderUniform4f_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform4f_v2),
                    gensym("v2"), A_FLOAT, 0);
    class_addmethod(ofeliaSetShaderUniform4f_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform4f_v3),
                    gensym("v3"), A_FLOAT, 0);
    class_addmethod(ofeliaSetShaderUniform4f_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform4f_v4),
                    gensym("v4"), A_FLOAT, 0);
    class_addmethod(ofeliaSetShaderUniform4f_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform4f_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderUniform4f_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform4f_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * common methods
 */
bool getSetShaderUniformivElemFromArgs(const int argc, const t_atom *argv, t_ofeliaSetShaderUniformivElem &elem, const int numVec, const char *objName)
{
    if (!argc) {
        
        elem.uniformName = "";
    }
    else {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            elem.uniformName = argv[0].a_w.w_symbol->s_name;
            
            if ((argc-1)%numVec) {
                
                error("%s: invalid number of arguments", objName);
                return 0;
            }
            elem.values.clear();
            
            for (int i=1; i<argc; ++i) {
                
                if (argv[i].a_type == A_FLOAT) {
                    
                    elem.values.push_back(static_cast<int>(argv[i].a_w.w_float));
                }
                else {
                    
                    error("%s: wrong argument type", objName);
                    return 0;
                }
            }
        }
        else {
            
            error("%s: wrong argument type", objName);
            return 0;
        }
    }
    return 1;
}

bool getSetShaderUniformfvElemFromArgs(const int argc, const t_atom *argv, t_ofeliaSetShaderUniformfvElem &elem, const int numVec, const char *objName)
{
    if (!argc) {
        
        elem.uniformName = "";
    }
    else {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            elem.uniformName = argv[0].a_w.w_symbol->s_name;
            
            if ((argc-1)%numVec) {
                
                error("%s: invalid number of arguments", objName);
                return 0;
            }
            elem.values.clear();
            
            for (int i=1; i<argc; ++i) {
                
                if (argv[i].a_type == A_FLOAT) {
                    
                    elem.values.push_back(argv[i].a_w.w_float);
                }
                else {
                    
                    error("%s: wrong argument type", objName);
                    return 0;
                }
            }
        }
        else {
            
            error("%s: wrong argument type", objName);
            return 0;
        }
    }
    return 1;
}

bool getSetShaderAttributefvElemFromArgs(const int argc, const t_atom *argv, t_ofeliaSetShaderAttributefvElem &elem, const int numVec, const char *objName)
{
    if (!argc) {
        
        elem.attributeName = "";
    }
    else {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            elem.attributeName = argv[0].a_w.w_symbol->s_name;
            
            if ((argc-1)%numVec) {
                
                error("%s: invalid number of arguments", objName);
                return 0;
            }
            elem.values.clear();
            
            for (int i=1; i<argc; ++i) {
                
                if (argv[i].a_type == A_FLOAT) {
                    
                    elem.values.push_back(argv[i].a_w.w_float);
                }
                else {
                    
                    error("%s: wrong argument type", objName);
                    return 0;
                }
            }
        }
        else {
            
            error("%s: wrong argument type", objName);
            return 0;
        }
    }
    return 1;
}

/* ________________________________________________________________________________
 * ofSetShaderUniform1iv object methods
 */
void *ofeliaSetShaderUniform1iv_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetShaderUniform1iv *x = reinterpret_cast<t_ofeliaSetShaderUniform1iv*>(pd_new(ofeliaSetShaderUniform1iv_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getSetShaderUniformivElemFromArgs(argc, argv, x->elem, 1, t_ofeliaSetShaderUniform1iv::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("v"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetShaderUniform1iv_bang(t_ofeliaSetShaderUniform1iv *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (t_ofeliaApplyShader::currentShader)
            t_ofeliaApplyShader::currentShader->setUniform1iv(x->elem.uniformName.c_str(), x->elem.values.data(), x->elem.values.size());
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetShaderUniform1iv::objName);
    }
}

void ofeliaSetShaderUniform1iv_uniformName(t_ofeliaSetShaderUniform1iv *x, t_symbol *s)
{
    x->elem.uniformName = s->s_name;
}

void ofeliaSetShaderUniform1iv_v(t_ofeliaSetShaderUniform1iv *x, t_symbol *s, int argc, t_atom *argv)
{
    x->elem.values.clear();
    
    for (int i=0; i<argc; ++i) {
        
        if (argv[i].a_type == A_FLOAT) {
            
            x->elem.values.push_back(static_cast<int>(argv[i].a_w.w_float));
        }
        else {
            
            x->elem.values.clear();
            error("%s: wrong argument type", t_ofeliaSetShaderUniform1iv::objName);
            return;
        }
    }
}

void ofeliaSetShaderUniform1iv_set(t_ofeliaSetShaderUniform1iv *x, t_symbol *s, int argc, t_atom *argv)
{
    getSetShaderUniformivElemFromArgs(argc, argv, x->elem, 1, t_ofeliaSetShaderUniform1iv::objName);
}

void ofeliaSetShaderUniform1iv_print(t_ofeliaSetShaderUniform1iv *x)
{
    post("\n[%s]", t_ofeliaSetShaderUniform1iv::objName);
    post("uniformName : %s", x->elem.uniformName.c_str());
    post("v : ");
    
    for (size_t i=0; i<x->elem.values.size(); ++i)
        post(" %d", x->elem.values[i]);
}

void ofeliaSetShaderUniform1iv_setup()
{
    ofeliaSetShaderUniform1iv_class = class_new(gensym("ofSetShaderUniform1iv"),
                                                reinterpret_cast<t_newmethod>(ofeliaSetShaderUniform1iv_new),
                                                0, sizeof(t_ofeliaSetShaderUniform1iv),
                                                CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetShaderUniform1iv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform1iv_bang));
    class_addmethod(ofeliaSetShaderUniform1iv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform1iv_uniformName),
                    gensym("uniformName"), A_SYMBOL, 0);
    class_addmethod(ofeliaSetShaderUniform1iv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform1iv_v),
                    gensym("v"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderUniform1iv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform1iv_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderUniform1iv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform1iv_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetShaderUniform2iv object methods
 */
void *ofeliaSetShaderUniform2iv_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetShaderUniform2iv *x = reinterpret_cast<t_ofeliaSetShaderUniform2iv*>(pd_new(ofeliaSetShaderUniform2iv_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getSetShaderUniformivElemFromArgs(argc, argv, x->elem, 2, t_ofeliaSetShaderUniform2iv::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("v"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetShaderUniform2iv_bang(t_ofeliaSetShaderUniform2iv *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (t_ofeliaApplyShader::currentShader)
            t_ofeliaApplyShader::currentShader->setUniform2iv(x->elem.uniformName.c_str(), x->elem.values.data(), x->elem.values.size());
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetShaderUniform2iv::objName);
    }
}

void ofeliaSetShaderUniform2iv_uniformName(t_ofeliaSetShaderUniform2iv *x, t_symbol *s)
{
    x->elem.uniformName = s->s_name;
}

void ofeliaSetShaderUniform2iv_v(t_ofeliaSetShaderUniform2iv *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc%2) {
        
        error("%s: invalid number of arguments", t_ofeliaSetShaderUniform2iv::objName);
        return;
    }
    x->elem.values.clear();
    
    for (int i=0; i<argc; ++i) {
        
        if (argv[i].a_type == A_FLOAT) {
            
            x->elem.values.push_back(static_cast<int>(argv[i].a_w.w_float));
        }
        else {
            
            x->elem.values.clear();
            error("%s: wrong argument type", t_ofeliaSetShaderUniform2iv::objName);
            return;
        }
    }
}

void ofeliaSetShaderUniform2iv_set(t_ofeliaSetShaderUniform2iv *x, t_symbol *s, int argc, t_atom *argv)
{
    getSetShaderUniformivElemFromArgs(argc, argv, x->elem, 2, t_ofeliaSetShaderUniform2iv::objName);
}

void ofeliaSetShaderUniform2iv_print(t_ofeliaSetShaderUniform2iv *x)
{
    post("\n[%s]", t_ofeliaSetShaderUniform2iv::objName);
    post("uniformName : %s", x->elem.uniformName.c_str());
    post("v : ");
    
    for (size_t i=0; i<x->elem.values.size(); i+=2)
        post(" %d %d", x->elem.values[i], x->elem.values[i+1]);
}

void ofeliaSetShaderUniform2iv_setup()
{
    ofeliaSetShaderUniform2iv_class = class_new(gensym("ofSetShaderUniform2iv"),
                                                reinterpret_cast<t_newmethod>(ofeliaSetShaderUniform2iv_new),
                                                0, sizeof(t_ofeliaSetShaderUniform2iv),
                                                CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetShaderUniform2iv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform2iv_bang));
    class_addmethod(ofeliaSetShaderUniform2iv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform2iv_uniformName),
                    gensym("uniformName"), A_SYMBOL, 0);
    class_addmethod(ofeliaSetShaderUniform2iv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform2iv_v),
                    gensym("v"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderUniform2iv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform2iv_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderUniform2iv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform2iv_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetShaderUniform3iv object methods
 */
void *ofeliaSetShaderUniform3iv_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetShaderUniform3iv *x = reinterpret_cast<t_ofeliaSetShaderUniform3iv*>(pd_new(ofeliaSetShaderUniform3iv_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getSetShaderUniformivElemFromArgs(argc, argv, x->elem, 3, t_ofeliaSetShaderUniform3iv::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("v"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetShaderUniform3iv_bang(t_ofeliaSetShaderUniform3iv *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (t_ofeliaApplyShader::currentShader)
            t_ofeliaApplyShader::currentShader->setUniform3iv(x->elem.uniformName.c_str(), x->elem.values.data(), x->elem.values.size());
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetShaderUniform3iv::objName);
    }
}

void ofeliaSetShaderUniform3iv_uniformName(t_ofeliaSetShaderUniform3iv *x, t_symbol *s)
{
    x->elem.uniformName = s->s_name;
}

void ofeliaSetShaderUniform3iv_v(t_ofeliaSetShaderUniform3iv *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc%3) {
        
        error("%s: invalid number of arguments", t_ofeliaSetShaderUniform3iv::objName);
        return;
    }
    x->elem.values.clear();
    
    for (int i=0; i<argc; ++i) {
        
        if (argv[i].a_type == A_FLOAT) {
            
            x->elem.values.push_back(static_cast<int>(argv[i].a_w.w_float));
        }
        else {
            
            x->elem.values.clear();
            error("%s: wrong argument type", t_ofeliaSetShaderUniform3iv::objName);
            return;
        }
    }
}

void ofeliaSetShaderUniform3iv_set(t_ofeliaSetShaderUniform3iv *x, t_symbol *s, int argc, t_atom *argv)
{
    getSetShaderUniformivElemFromArgs(argc, argv, x->elem, 3, t_ofeliaSetShaderUniform3iv::objName);
}

void ofeliaSetShaderUniform3iv_print(t_ofeliaSetShaderUniform3iv *x)
{
    post("\n[%s]", t_ofeliaSetShaderUniform3iv::objName);
    post("uniformName : %s", x->elem.uniformName.c_str());
    post("v : ");
    
    for (size_t i=0; i<x->elem.values.size(); i+=3)
        post(" %d %d %d", x->elem.values[i], x->elem.values[i+1], x->elem.values[i+2]);
}

void ofeliaSetShaderUniform3iv_setup()
{
    ofeliaSetShaderUniform3iv_class = class_new(gensym("ofSetShaderUniform3iv"),
                                                reinterpret_cast<t_newmethod>(ofeliaSetShaderUniform3iv_new),
                                                0, sizeof(t_ofeliaSetShaderUniform3iv),
                                                CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetShaderUniform3iv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform3iv_bang));
    class_addmethod(ofeliaSetShaderUniform3iv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform3iv_uniformName),
                    gensym("uniformName"), A_SYMBOL, 0);
    class_addmethod(ofeliaSetShaderUniform3iv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform3iv_v),
                    gensym("v"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderUniform3iv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform3iv_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderUniform3iv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform3iv_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetShaderUniform4iv object methods
 */
void *ofeliaSetShaderUniform4iv_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetShaderUniform4iv *x = reinterpret_cast<t_ofeliaSetShaderUniform4iv*>(pd_new(ofeliaSetShaderUniform4iv_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);

    if (!getSetShaderUniformivElemFromArgs(argc, argv, x->elem, 4, t_ofeliaSetShaderUniform4iv::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("v"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetShaderUniform4iv_bang(t_ofeliaSetShaderUniform4iv *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (t_ofeliaApplyShader::currentShader)
            t_ofeliaApplyShader::currentShader->setUniform4iv(x->elem.uniformName.c_str(), x->elem.values.data(), x->elem.values.size());
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetShaderUniform4iv::objName);
    }
}

void ofeliaSetShaderUniform4iv_uniformName(t_ofeliaSetShaderUniform4iv *x, t_symbol *s)
{
    x->elem.uniformName = s->s_name;
}

void ofeliaSetShaderUniform4iv_v(t_ofeliaSetShaderUniform4iv *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc%4) {
        
        error("%s: invalid number of arguments", t_ofeliaSetShaderUniform4iv::objName);
        return;
    }
    x->elem.values.clear();
    
    for (int i=0; i<argc; ++i) {
        
        if (argv[i].a_type == A_FLOAT) {
            
            x->elem.values.push_back(static_cast<int>(argv[i].a_w.w_float));
        }
        else {
            
            x->elem.values.clear();
            error("%s: wrong argument type", t_ofeliaSetShaderUniform4iv::objName);
            return;
        }
    }
}

void ofeliaSetShaderUniform4iv_set(t_ofeliaSetShaderUniform4iv *x, t_symbol *s, int argc, t_atom *argv)
{
    getSetShaderUniformivElemFromArgs(argc, argv, x->elem, 4, t_ofeliaSetShaderUniform4iv::objName);
}

void ofeliaSetShaderUniform4iv_print(t_ofeliaSetShaderUniform4iv *x)
{
    post("\n[%s]", t_ofeliaSetShaderUniform4iv::objName);
    post("uniformName : %s", x->elem.uniformName.c_str());
    post("v : ");
    
    for (size_t i=0; i<x->elem.values.size(); i+=4)
        post(" %d %d %d %d", x->elem.values[i], x->elem.values[i+1], x->elem.values[i+2], x->elem.values[i+3]);
}

void ofeliaSetShaderUniform4iv_setup()
{
    ofeliaSetShaderUniform4iv_class = class_new(gensym("ofSetShaderUniform4iv"),
                                                reinterpret_cast<t_newmethod>(ofeliaSetShaderUniform4iv_new),
                                                0, sizeof(t_ofeliaSetShaderUniform4iv),
                                                CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetShaderUniform4iv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform4iv_bang));
    class_addmethod(ofeliaSetShaderUniform4iv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform4iv_uniformName),
                    gensym("uniformName"), A_SYMBOL, 0);
    class_addmethod(ofeliaSetShaderUniform4iv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform4iv_v),
                    gensym("v"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderUniform4iv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform4iv_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderUniform4iv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform4iv_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetShaderUniform1fv object methods
 */
void *ofeliaSetShaderUniform1fv_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetShaderUniform1fv *x = reinterpret_cast<t_ofeliaSetShaderUniform1fv*>(pd_new(ofeliaSetShaderUniform1fv_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getSetShaderUniformfvElemFromArgs(argc, argv, x->elem, 1, t_ofeliaSetShaderUniform1fv::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("v"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetShaderUniform1fv_bang(t_ofeliaSetShaderUniform1fv *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (t_ofeliaApplyShader::currentShader)
            t_ofeliaApplyShader::currentShader->setUniform1fv(x->elem.uniformName.c_str(), x->elem.values.data(), x->elem.values.size());
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetShaderUniform1fv::objName);
    }
}

void ofeliaSetShaderUniform1fv_uniformName(t_ofeliaSetShaderUniform1fv *x, t_symbol *s)
{
    x->elem.uniformName = s->s_name;
}

void ofeliaSetShaderUniform1fv_v(t_ofeliaSetShaderUniform1fv *x, t_symbol *s, int argc, t_atom *argv)
{
    x->elem.values.clear();
    
    for (int i=0; i<argc; ++i) {
        
        if (argv[i].a_type == A_FLOAT) {
            
            x->elem.values.push_back(argv[i].a_w.w_float);
        }
        else {
            
            x->elem.values.clear();
            error("%s: wrong argument type", t_ofeliaSetShaderUniform1fv::objName);
            return;
        }
    }
}

void ofeliaSetShaderUniform1fv_set(t_ofeliaSetShaderUniform1fv *x, t_symbol *s, int argc, t_atom *argv)
{
    getSetShaderUniformfvElemFromArgs(argc, argv, x->elem, 1, t_ofeliaSetShaderUniform1fv::objName);
}

void ofeliaSetShaderUniform1fv_print(t_ofeliaSetShaderUniform1fv *x)
{
    post("\n[%s]", t_ofeliaSetShaderUniform1fv::objName);
    post("uniformName : %s", x->elem.uniformName.c_str());
    post("v : ");
    
    for (size_t i=0; i<x->elem.values.size(); ++i)
        post(" %g", x->elem.values[i]);
}

void ofeliaSetShaderUniform1fv_setup()
{
    ofeliaSetShaderUniform1fv_class = class_new(gensym("ofSetShaderUniform1fv"),
                                                reinterpret_cast<t_newmethod>(ofeliaSetShaderUniform1fv_new),
                                                0, sizeof(t_ofeliaSetShaderUniform1fv),
                                                CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetShaderUniform1fv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform1fv_bang));
    class_addmethod(ofeliaSetShaderUniform1fv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform1fv_uniformName),
                    gensym("uniformName"), A_SYMBOL, 0);
    class_addmethod(ofeliaSetShaderUniform1fv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform1fv_v),
                    gensym("v"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderUniform1fv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform1fv_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderUniform1fv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform1fv_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetShaderUniform2fv object methods
 */
void *ofeliaSetShaderUniform2fv_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetShaderUniform2fv *x = reinterpret_cast<t_ofeliaSetShaderUniform2fv*>(pd_new(ofeliaSetShaderUniform2fv_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getSetShaderUniformfvElemFromArgs(argc, argv, x->elem, 2, t_ofeliaSetShaderUniform2fv::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("v"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetShaderUniform2fv_bang(t_ofeliaSetShaderUniform2fv *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (t_ofeliaApplyShader::currentShader)
            t_ofeliaApplyShader::currentShader->setUniform2fv(x->elem.uniformName.c_str(), x->elem.values.data(), x->elem.values.size());
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetShaderUniform2fv::objName);
    }
}

void ofeliaSetShaderUniform2fv_uniformName(t_ofeliaSetShaderUniform2fv *x, t_symbol *s)
{
    x->elem.uniformName = s->s_name;
}

void ofeliaSetShaderUniform2fv_v(t_ofeliaSetShaderUniform2fv *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc%2) {
        
        error("%s: invalid number of arguments", t_ofeliaSetShaderUniform2fv::objName);
        return;
    }
    x->elem.values.clear();
    
    for (int i=0; i<argc; ++i) {
        
        if (argv[i].a_type == A_FLOAT) {
            
            x->elem.values.push_back(argv[i].a_w.w_float);
        }
        else {
            
            x->elem.values.clear();
            error("%s: wrong argument type", t_ofeliaSetShaderUniform2fv::objName);
            return;
        }
    }
}

void ofeliaSetShaderUniform2fv_set(t_ofeliaSetShaderUniform2fv *x, t_symbol *s, int argc, t_atom *argv)
{
    getSetShaderUniformfvElemFromArgs(argc, argv, x->elem, 2, t_ofeliaSetShaderUniform2fv::objName);
}

void ofeliaSetShaderUniform2fv_print(t_ofeliaSetShaderUniform2fv *x)
{
    post("\n[%s]", t_ofeliaSetShaderUniform2fv::objName);
    post("uniformName : %s", x->elem.uniformName.c_str());
    post("v : ");
    
    for (size_t i=0; i<x->elem.values.size(); i+=2)
        post(" %g %g", x->elem.values[i], x->elem.values[i+1]);
}

void ofeliaSetShaderUniform2fv_setup()
{
    ofeliaSetShaderUniform2fv_class = class_new(gensym("ofSetShaderUniform2fv"),
                                                reinterpret_cast<t_newmethod>(ofeliaSetShaderUniform2fv_new),
                                                0, sizeof(t_ofeliaSetShaderUniform2fv),
                                                CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetShaderUniform2fv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform2fv_bang));
    class_addmethod(ofeliaSetShaderUniform2fv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform2fv_uniformName),
                    gensym("uniformName"), A_SYMBOL, 0);
    class_addmethod(ofeliaSetShaderUniform2fv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform2fv_v),
                    gensym("v"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderUniform2fv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform2fv_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderUniform2fv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform2fv_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetShaderUniform3fv object methods
 */
void *ofeliaSetShaderUniform3fv_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetShaderUniform3fv *x = reinterpret_cast<t_ofeliaSetShaderUniform3fv*>(pd_new(ofeliaSetShaderUniform3fv_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getSetShaderUniformfvElemFromArgs(argc, argv, x->elem, 3, t_ofeliaSetShaderUniform3fv::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("v"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetShaderUniform3fv_bang(t_ofeliaSetShaderUniform3fv *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (t_ofeliaApplyShader::currentShader)
            t_ofeliaApplyShader::currentShader->setUniform3fv(x->elem.uniformName.c_str(), x->elem.values.data(), x->elem.values.size());
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetShaderUniform3fv::objName);
    }
}

void ofeliaSetShaderUniform3fv_uniformName(t_ofeliaSetShaderUniform3fv *x, t_symbol *s)
{
    x->elem.uniformName = s->s_name;
}

void ofeliaSetShaderUniform3fv_v(t_ofeliaSetShaderUniform3fv *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc%3) {
        
        error("%s: invalid number of arguments", t_ofeliaSetShaderUniform3fv::objName);
        return;
    }
    x->elem.values.clear();
    
    for (int i=0; i<argc; ++i) {
        
        if (argv[i].a_type == A_FLOAT) {
            
            x->elem.values.push_back(argv[i].a_w.w_float);
        }
        else {
            
            x->elem.values.clear();
            error("%s: wrong argument type", t_ofeliaSetShaderUniform3fv::objName);
            return;
        }
    }
}

void ofeliaSetShaderUniform3fv_set(t_ofeliaSetShaderUniform3fv *x, t_symbol *s, int argc, t_atom *argv)
{
    getSetShaderUniformfvElemFromArgs(argc, argv, x->elem, 3, t_ofeliaSetShaderUniform3fv::objName);
}

void ofeliaSetShaderUniform3fv_print(t_ofeliaSetShaderUniform3fv *x)
{
    post("\n[%s]", t_ofeliaSetShaderUniform3fv::objName);
    post("uniformName : %s", x->elem.uniformName.c_str());
    post("v : ");
    
    for (size_t i=0; i<x->elem.values.size(); i+=3)
        post(" %g %g %g", x->elem.values[i], x->elem.values[i+1], x->elem.values[i+2]);
}

void ofeliaSetShaderUniform3fv_setup()
{
    ofeliaSetShaderUniform3fv_class = class_new(gensym("ofSetShaderUniform3fv"),
                                                reinterpret_cast<t_newmethod>(ofeliaSetShaderUniform3fv_new),
                                                0, sizeof(t_ofeliaSetShaderUniform3fv),
                                                CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetShaderUniform3fv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform3fv_bang));
    class_addmethod(ofeliaSetShaderUniform3fv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform3fv_uniformName),
                    gensym("uniformName"), A_SYMBOL, 0);
    class_addmethod(ofeliaSetShaderUniform3fv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform3fv_v),
                    gensym("v"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderUniform3fv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform3fv_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderUniform3fv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform3fv_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetShaderUniform4fv object methods
 */
void *ofeliaSetShaderUniform4fv_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetShaderUniform4fv *x = reinterpret_cast<t_ofeliaSetShaderUniform4fv*>(pd_new(ofeliaSetShaderUniform4fv_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getSetShaderUniformfvElemFromArgs(argc, argv, x->elem, 4, t_ofeliaSetShaderUniform4fv::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("v"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetShaderUniform4fv_bang(t_ofeliaSetShaderUniform4fv *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (t_ofeliaApplyShader::currentShader)
            t_ofeliaApplyShader::currentShader->setUniform4fv(x->elem.uniformName.c_str(), x->elem.values.data(), x->elem.values.size());
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetShaderUniform4fv::objName);
    }
}

void ofeliaSetShaderUniform4fv_uniformName(t_ofeliaSetShaderUniform4fv *x, t_symbol *s)
{
    x->elem.uniformName = s->s_name;
}

void ofeliaSetShaderUniform4fv_v(t_ofeliaSetShaderUniform4fv *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc%4) {
        
        error("%s: invalid number of arguments", t_ofeliaSetShaderUniform4fv::objName);
        return;
    }
    x->elem.values.clear();
    
    for (int i=0; i<argc; ++i) {
        
        if (argv[i].a_type == A_FLOAT) {
            
            x->elem.values.push_back(argv[i].a_w.w_float);
        }
        else {
            
            x->elem.values.clear();
            error("%s: wrong argument type", t_ofeliaSetShaderUniform4fv::objName);
            return;
        }
    }
}

void ofeliaSetShaderUniform4fv_set(t_ofeliaSetShaderUniform4fv *x, t_symbol *s, int argc, t_atom *argv)
{
    getSetShaderUniformfvElemFromArgs(argc, argv, x->elem, 4, t_ofeliaSetShaderUniform4fv::objName);
}

void ofeliaSetShaderUniform4fv_print(t_ofeliaSetShaderUniform4fv *x)
{
    post("\n[%s]", t_ofeliaSetShaderUniform4fv::objName);
    post("uniformName : %s", x->elem.uniformName.c_str());
    post("v : ");
    
    for (size_t i=0; i<x->elem.values.size(); i+=4)
        post(" %g %g %g %g", x->elem.values[i], x->elem.values[i+1], x->elem.values[i+2], x->elem.values[i+3]);
}

void ofeliaSetShaderUniform4fv_setup()
{
    ofeliaSetShaderUniform4fv_class = class_new(gensym("ofSetShaderUniform4fv"),
                                                reinterpret_cast<t_newmethod>(ofeliaSetShaderUniform4fv_new),
                                                0, sizeof(t_ofeliaSetShaderUniform4fv),
                                                CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetShaderUniform4fv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform4fv_bang));
    class_addmethod(ofeliaSetShaderUniform4fv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform4fv_uniformName),
                    gensym("uniformName"), A_SYMBOL, 0);
    class_addmethod(ofeliaSetShaderUniform4fv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform4fv_v),
                    gensym("v"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderUniform4fv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform4fv_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderUniform4fv_class, reinterpret_cast<t_method>(ofeliaSetShaderUniform4fv_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetShaderUniformTex object methods
 */
bool getSetShaderUniformTexElemFromArgs(const int argc, const t_atom *argv, t_ofeliaSetShaderUniformTexElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.uniformName = "";
            elem.imageName.name = "";
            elem.textureLocation = 0;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_SYMBOL) {
                
                elem.uniformName = argv[0].a_w.w_symbol->s_name;
                elem.imageName.name = "";
                elem.textureLocation = 0;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSetShaderUniformTex::objName);
                return 0;
            }
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_SYMBOL &&
                argv[1].a_type == A_SYMBOL) {
                
                elem.uniformName = argv[0].a_w.w_symbol->s_name;
                elem.imageName.name = argv[1].a_w.w_symbol->s_name;
                getVarNameLocalized(elem.imageName);
                getVarNameIndexed(elem.imageName);
                elem.textureLocation = 0;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSetShaderUniformTex::objName);
                return 0;
            }
            break;
        }
        case 3:
        {
            if (argv[0].a_type == A_SYMBOL &&
                argv[1].a_type == A_SYMBOL &&
                argv[2].a_type == A_FLOAT) {
                
                elem.uniformName = argv[0].a_w.w_symbol->s_name;
                elem.imageName.name = argv[1].a_w.w_symbol->s_name;
                getVarNameLocalized(elem.imageName);
                getVarNameIndexed(elem.imageName);
                elem.textureLocation = static_cast<int>(argv[2].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSetShaderUniformTex::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaSetShaderUniformTex::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaSetShaderUniformTex_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetShaderUniformTex *x = reinterpret_cast<t_ofeliaSetShaderUniformTex*>(pd_new(ofeliaSetShaderUniformTex_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    getVarNameLocalPrefixes(x->elem.imageName);
    
    if (!getSetShaderUniformTexElemFromArgs(argc, argv, x->elem))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("textureLocation"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetShaderUniformTex_bang(t_ofeliaSetShaderUniformTex *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (t_ofeliaApplyShader::currentShader) {
            
            const t_string &cName = x->elem.imageName.name;
            bool cIsLoadedImage, cIsEmpty;
            int cPos = getPositionByImageName(cName, cIsLoadedImage);
            
            if (cPos != -1) {
                
                ofImage *cImg = nullptr;
                
                if (!cIsLoadedImage) {
                    
                    cIsEmpty = false;
                    cImg = t_ofeliaCreateImage::images[cPos].get();
                }
                else {
                    
                    const int cIndex = min(x->elem.imageName.index, static_cast<int>(t_ofeliaLoadImage::imageData[cPos].paths.size())-1);
                    cIsEmpty = t_ofeliaLoadImage::imageData[cPos].paths.empty();
                    
                    if (!cIsEmpty) {
                        
                        if (!t_ofeliaLoadImage::images[cPos].empty() &&
                            cIndex < static_cast<int>(t_ofeliaLoadImage::images[cPos].size())) {
                            
                            cImg = t_ofeliaLoadImage::images[cPos][cIndex].get();
                        }
                    }
                }
                if (!cIsEmpty) {
                    
                    if (cImg) {
                        
                        if (cImg->isAllocated()) {
                            
                            t_ofeliaApplyShader::currentShader->setUniformTexture(x->elem.uniformName.c_str(), cImg->getTexture(), x->elem.textureLocation);
                        }
                    }
                }
            }
            else {
                
                cPos = getPositionByFboName(cName);
                
                if (cPos != -1) {
                    
                    if (t_ofeliaCreateFbo::fbos[cPos]->isAllocated()) {
                        
                        t_ofeliaApplyShader::currentShader->setUniformTexture(x->elem.uniformName.c_str(), t_ofeliaCreateFbo::fbos[cPos]->getTexture(), x->elem.textureLocation);
                    }
                }
            }
        }
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetShaderUniformTex::objName);
    }
}

void ofeliaSetShaderUniformTex_uniformName(t_ofeliaSetShaderUniformTex *x, t_symbol *s)
{
    x->elem.uniformName = s->s_name;
}

void ofeliaSetShaderUniformTex_imageName(t_ofeliaSetShaderUniformTex *x, t_symbol *s)
{
    x->elem.imageName.name = s->s_name;
    getVarNameLocalized(x->elem.imageName);
    getVarNameIndexed(x->elem.imageName);
}

void ofeliaSetShaderUniformTex_textureLocation(t_ofeliaSetShaderUniformTex *x, t_floatarg f)
{
    x->elem.textureLocation = static_cast<int>(f);
}

void ofeliaSetShaderUniformTex_set(t_ofeliaSetShaderUniformTex *x, t_symbol *s, int argc, t_atom *argv)
{
    getSetShaderUniformTexElemFromArgs(argc, argv, x->elem);
}

void ofeliaSetShaderUniformTex_print(t_ofeliaSetShaderUniformTex *x)
{
    post("\n[%s]", t_ofeliaSetShaderUniformTex::objName);
    post("uniformName : %s", x->elem.uniformName.c_str());
    post("imageName : %s[%d]", x->elem.imageName.name.c_str(), x->elem.imageName.index);
    post("textureLocation : %d", x->elem.textureLocation);
}

void ofeliaSetShaderUniformTex_setup()
{
    ofeliaSetShaderUniformTex_class = class_new(gensym("ofSetShaderUniformTex"),
                                                reinterpret_cast<t_newmethod>(ofeliaSetShaderUniformTex_new),
                                                0, sizeof(t_ofeliaSetShaderUniformTex),
                                                CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetShaderUniformTex_class, reinterpret_cast<t_method>(ofeliaSetShaderUniformTex_bang));
    class_addmethod(ofeliaSetShaderUniformTex_class, reinterpret_cast<t_method>(ofeliaSetShaderUniformTex_uniformName),
                    gensym("uniformName"), A_SYMBOL, 0);
    class_addmethod(ofeliaSetShaderUniformTex_class, reinterpret_cast<t_method>(ofeliaSetShaderUniformTex_imageName),
                    gensym("imageName"), A_SYMBOL, 0);
    class_addmethod(ofeliaSetShaderUniformTex_class, reinterpret_cast<t_method>(ofeliaSetShaderUniformTex_textureLocation),
                    gensym("textureLocation"), A_FLOAT, 0);
    class_addmethod(ofeliaSetShaderUniformTex_class, reinterpret_cast<t_method>(ofeliaSetShaderUniformTex_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderUniformTex_class, reinterpret_cast<t_method>(ofeliaSetShaderUniformTex_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetShaderAttribute1f object methods
 */
bool getSetShaderAttribute1fElemFromArgs(const int argc, const t_atom *argv, t_ofeliaSetShaderAttribute1fElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.attributeName = "";
            elem.value = 0.0f;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_SYMBOL) {
                
                elem.attributeName = argv[0].a_w.w_symbol->s_name;
                elem.value = 0.0f;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSetShaderAttribute1f::objName);
                return 0;
            }
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_SYMBOL &&
                argv[1].a_type == A_FLOAT) {
                
                elem.attributeName = argv[0].a_w.w_symbol->s_name;
                elem.value = argv[1].a_w.w_float;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSetShaderAttribute1f::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaSetShaderAttribute1f::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaSetShaderAttribute1f_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetShaderAttribute1f *x = reinterpret_cast<t_ofeliaSetShaderAttribute1f*>(pd_new(ofeliaSetShaderAttribute1f_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getSetShaderAttribute1fElemFromArgs(argc, argv, x->elem))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("v1"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetShaderAttribute1f_bang(t_ofeliaSetShaderAttribute1f *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (t_ofeliaApplyShader::currentShader)
            t_ofeliaApplyShader::currentShader->setAttribute1f(t_ofeliaApplyShader::currentShader->getAttributeLocation(x->elem.attributeName.c_str()), x->elem.value);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetShaderAttribute1f::objName);
    }
}

void ofeliaSetShaderAttribute1f_attributeName(t_ofeliaSetShaderAttribute1f *x, t_symbol *s)
{
    x->elem.attributeName = s->s_name;
}

void ofeliaSetShaderAttribute1f_v1(t_ofeliaSetShaderAttribute1f *x, t_floatarg f)
{
    x->elem.value = f;
}

void ofeliaSetShaderAttribute1f_set(t_ofeliaSetShaderAttribute1f *x, t_symbol *s, int argc, t_atom *argv)
{
    getSetShaderAttribute1fElemFromArgs(argc, argv, x->elem);
}

void ofeliaSetShaderAttribute1f_print(t_ofeliaSetShaderAttribute1f *x)
{
    post("\n[%s]", t_ofeliaSetShaderAttribute1f::objName);
    post("attributeName : %s", x->elem.attributeName.c_str());
    post("v1 : %g", x->elem.value);
}

void ofeliaSetShaderAttribute1f_setup()
{
    ofeliaSetShaderAttribute1f_class = class_new(gensym("ofSetShaderAttribute1f"),
                                                 reinterpret_cast<t_newmethod>(ofeliaSetShaderAttribute1f_new),
                                                 0, sizeof(t_ofeliaSetShaderAttribute1f),
                                                 CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetShaderAttribute1f_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute1f_bang));
    class_addmethod(ofeliaSetShaderAttribute1f_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute1f_attributeName),
                    gensym("attributeName"), A_SYMBOL, 0);
    class_addmethod(ofeliaSetShaderAttribute1f_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute1f_v1),
                    gensym("v1"), A_FLOAT, 0);
    class_addmethod(ofeliaSetShaderAttribute1f_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute1f_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderAttribute1f_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute1f_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetShaderAttribute2f object methods
 */
bool getSetShaderAttribute2fElemFromArgs(const int argc, const t_atom *argv, t_ofeliaSetShaderAttribute2fElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.attributeName = "";
            elem.value[0] = elem.value[1] = 0.0f;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_SYMBOL) {
                
                elem.attributeName = argv[0].a_w.w_symbol->s_name;
                elem.value[0] = elem.value[1] = 0.0f;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSetShaderAttribute2f::objName);
                return 0;
            }
            break;
        }
        case 3:
        {
            if (argv[0].a_type == A_SYMBOL &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT) {
                
                elem.attributeName = argv[0].a_w.w_symbol->s_name;
                elem.value[0] = argv[1].a_w.w_float;
                elem.value[1] = argv[2].a_w.w_float;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSetShaderAttribute2f::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaSetShaderAttribute2f::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaSetShaderAttribute2f_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetShaderAttribute2f *x = reinterpret_cast<t_ofeliaSetShaderAttribute2f*>(pd_new(ofeliaSetShaderAttribute2f_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getSetShaderAttribute2fElemFromArgs(argc, argv, x->elem))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("v1"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("v2"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetShaderAttribute2f_bang(t_ofeliaSetShaderAttribute2f *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (t_ofeliaApplyShader::currentShader)
            t_ofeliaApplyShader::currentShader->setAttribute2f(t_ofeliaApplyShader::currentShader->getAttributeLocation(x->elem.attributeName.c_str()), x->elem.value[0], x->elem.value[1]);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetShaderAttribute2f::objName);
    }
}

void ofeliaSetShaderAttribute2f_attributeName(t_ofeliaSetShaderAttribute2f *x, t_symbol *s)
{
    x->elem.attributeName = s->s_name;
}

void ofeliaSetShaderAttribute2f_v1(t_ofeliaSetShaderAttribute2f *x, t_floatarg f)
{
    x->elem.value[0] = f;
}

void ofeliaSetShaderAttribute2f_v2(t_ofeliaSetShaderAttribute2f *x, t_floatarg f)
{
    x->elem.value[1] = f;
}

void ofeliaSetShaderAttribute2f_set(t_ofeliaSetShaderAttribute2f *x, t_symbol *s, int argc, t_atom *argv)
{
    getSetShaderAttribute2fElemFromArgs(argc, argv, x->elem);
}

void ofeliaSetShaderAttribute2f_print(t_ofeliaSetShaderAttribute2f *x)
{
    post("\n[%s]", t_ofeliaSetShaderAttribute2f::objName);
    post("attributeName : %s", x->elem.attributeName.c_str());
    post("v1 : %g", x->elem.value[0]);
    post("v2 : %g", x->elem.value[1]);
}

void ofeliaSetShaderAttribute2f_setup()
{
    ofeliaSetShaderAttribute2f_class = class_new(gensym("ofSetShaderAttribute2f"),
                                                 reinterpret_cast<t_newmethod>(ofeliaSetShaderAttribute2f_new),
                                                 0, sizeof(t_ofeliaSetShaderAttribute2f),
                                                 CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetShaderAttribute2f_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute2f_bang));
    class_addmethod(ofeliaSetShaderAttribute2f_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute2f_attributeName),
                    gensym("attributeName"), A_SYMBOL, 0);
    class_addmethod(ofeliaSetShaderAttribute2f_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute2f_v1),
                    gensym("v1"), A_FLOAT, 0);
    class_addmethod(ofeliaSetShaderAttribute2f_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute2f_v2),
                    gensym("v2"), A_FLOAT, 0);
    class_addmethod(ofeliaSetShaderAttribute2f_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute2f_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderAttribute2f_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute2f_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetShaderAttribute3f object methods
 */
bool getSetShaderAttribute3fElemFromArgs(const int argc, const t_atom *argv, t_ofeliaSetShaderAttribute3fElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.attributeName = "";
            elem.value[0] = elem.value[1] = elem.value[2] = 0.0f;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_SYMBOL) {
                
                elem.attributeName = argv[0].a_w.w_symbol->s_name;
                elem.value[0] = elem.value[1] = elem.value[2] = 0.0f;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSetShaderAttribute3f::objName);
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
                
                elem.attributeName = argv[0].a_w.w_symbol->s_name;
                elem.value[0] = argv[1].a_w.w_float;
                elem.value[1] = argv[2].a_w.w_float;
                elem.value[2] = argv[3].a_w.w_float;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSetShaderAttribute3f::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaSetShaderAttribute3f::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaSetShaderAttribute3f_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetShaderAttribute3f *x = reinterpret_cast<t_ofeliaSetShaderAttribute3f*>(pd_new(ofeliaSetShaderAttribute3f_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getSetShaderAttribute3fElemFromArgs(argc, argv, x->elem))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("v1"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("v2"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("v3"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetShaderAttribute3f_bang(t_ofeliaSetShaderAttribute3f *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (t_ofeliaApplyShader::currentShader)
            t_ofeliaApplyShader::currentShader->setAttribute3f(t_ofeliaApplyShader::currentShader->getAttributeLocation(x->elem.attributeName.c_str()), x->elem.value[0], x->elem.value[1], x->elem.value[2]);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetShaderAttribute3f::objName);
    }
}

void ofeliaSetShaderAttribute3f_attributeName(t_ofeliaSetShaderAttribute3f *x, t_symbol *s)
{
    x->elem.attributeName = s->s_name;
}

void ofeliaSetShaderAttribute3f_v1(t_ofeliaSetShaderAttribute3f *x, t_floatarg f)
{
    x->elem.value[0] = f;
}

void ofeliaSetShaderAttribute3f_v2(t_ofeliaSetShaderAttribute3f *x, t_floatarg f)
{
    x->elem.value[1] = f;
}

void ofeliaSetShaderAttribute3f_v3(t_ofeliaSetShaderAttribute3f *x, t_floatarg f)
{
    x->elem.value[2] = f;
}

void ofeliaSetShaderAttribute3f_set(t_ofeliaSetShaderAttribute3f *x, t_symbol *s, int argc, t_atom *argv)
{
    getSetShaderAttribute3fElemFromArgs(argc, argv, x->elem);
}

void ofeliaSetShaderAttribute3f_print(t_ofeliaSetShaderAttribute3f *x)
{
    post("\n[%s]", t_ofeliaSetShaderAttribute3f::objName);
    post("attributeName : %s", x->elem.attributeName.c_str());
    post("v1 : %g", x->elem.value[0]);
    post("v2 : %g", x->elem.value[1]);
    post("v3 : %g", x->elem.value[2]);
}

void ofeliaSetShaderAttribute3f_setup()
{
    ofeliaSetShaderAttribute3f_class = class_new(gensym("ofSetShaderAttribute3f"),
                                                 reinterpret_cast<t_newmethod>(ofeliaSetShaderAttribute3f_new),
                                                 0, sizeof(t_ofeliaSetShaderAttribute3f),
                                                 CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetShaderAttribute3f_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute3f_bang));
    class_addmethod(ofeliaSetShaderAttribute3f_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute3f_attributeName),
                    gensym("attributeName"), A_SYMBOL, 0);
    class_addmethod(ofeliaSetShaderAttribute3f_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute3f_v1),
                    gensym("v1"), A_FLOAT, 0);
    class_addmethod(ofeliaSetShaderAttribute3f_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute3f_v2),
                    gensym("v2"), A_FLOAT, 0);
    class_addmethod(ofeliaSetShaderAttribute3f_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute3f_v3),
                    gensym("v3"), A_FLOAT, 0);
    class_addmethod(ofeliaSetShaderAttribute3f_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute3f_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderAttribute3f_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute3f_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetShaderAttribute4f object methods
 */
bool getSetShaderAttribute4fElemFromArgs(const int argc, const t_atom *argv, t_ofeliaSetShaderAttribute4fElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.attributeName = "";
            elem.value[0] = elem.value[1] = elem.value[2] = elem.value[3] = 0.0f;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_SYMBOL) {
                
                elem.attributeName = argv[0].a_w.w_symbol->s_name;
                elem.value[0] = elem.value[1] = elem.value[2] = elem.value[3] = 0.0f;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSetShaderAttribute4f::objName);
                return 0;
            }
            break;
        }
        case 5:
        {
            if (argv[0].a_type == A_SYMBOL &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT &&
                argv[4].a_type == A_FLOAT) {
                
                elem.attributeName = argv[0].a_w.w_symbol->s_name;
                elem.value[0] = argv[1].a_w.w_float;
                elem.value[1] = argv[2].a_w.w_float;
                elem.value[2] = argv[3].a_w.w_float;
                elem.value[3] = argv[4].a_w.w_float;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSetShaderAttribute4f::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaSetShaderAttribute4f::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaSetShaderAttribute4f_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetShaderAttribute4f *x = reinterpret_cast<t_ofeliaSetShaderAttribute4f*>(pd_new(ofeliaSetShaderAttribute4f_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getSetShaderAttribute4fElemFromArgs(argc, argv, x->elem))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("v1"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("v2"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("v3"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("v4"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetShaderAttribute4f_bang(t_ofeliaSetShaderAttribute4f *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (t_ofeliaApplyShader::currentShader)
            t_ofeliaApplyShader::currentShader->setAttribute4f(t_ofeliaApplyShader::currentShader->getAttributeLocation(x->elem.attributeName.c_str()), x->elem.value[0], x->elem.value[1], x->elem.value[2], x->elem.value[3]);
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetShaderAttribute4f::objName);
    }
}

void ofeliaSetShaderAttribute4f_attributeName(t_ofeliaSetShaderAttribute4f *x, t_symbol *s)
{
    x->elem.attributeName = s->s_name;
}

void ofeliaSetShaderAttribute4f_v1(t_ofeliaSetShaderAttribute4f *x, t_floatarg f)
{
    x->elem.value[0] = f;
}

void ofeliaSetShaderAttribute4f_v2(t_ofeliaSetShaderAttribute4f *x, t_floatarg f)
{
    x->elem.value[1] = f;
}

void ofeliaSetShaderAttribute4f_v3(t_ofeliaSetShaderAttribute4f *x, t_floatarg f)
{
    x->elem.value[2] = f;
}

void ofeliaSetShaderAttribute4f_v4(t_ofeliaSetShaderAttribute4f *x, t_floatarg f)
{
    x->elem.value[3] = f;
}

void ofeliaSetShaderAttribute4f_set(t_ofeliaSetShaderAttribute4f *x, t_symbol *s, int argc, t_atom *argv)
{
    getSetShaderAttribute4fElemFromArgs(argc, argv, x->elem);
}

void ofeliaSetShaderAttribute4f_print(t_ofeliaSetShaderAttribute4f *x)
{
    post("\n[%s]", t_ofeliaSetShaderAttribute4f::objName);
    post("attributeName : %s", x->elem.attributeName.c_str());
    post("v1 : %g", x->elem.value[0]);
    post("v2 : %g", x->elem.value[1]);
    post("v3 : %g", x->elem.value[2]);
    post("v4 : %g", x->elem.value[3]);
}

void ofeliaSetShaderAttribute4f_setup()
{
    ofeliaSetShaderAttribute4f_class = class_new(gensym("ofSetShaderAttribute4f"),
                                                 reinterpret_cast<t_newmethod>(ofeliaSetShaderAttribute4f_new),
                                                 0, sizeof(t_ofeliaSetShaderAttribute4f),
                                                 CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetShaderAttribute4f_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute4f_bang));
    class_addmethod(ofeliaSetShaderAttribute4f_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute4f_attributeName),
                    gensym("attributeName"), A_SYMBOL, 0);
    class_addmethod(ofeliaSetShaderAttribute4f_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute4f_v1),
                    gensym("v1"), A_FLOAT, 0);
    class_addmethod(ofeliaSetShaderAttribute4f_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute4f_v2),
                    gensym("v2"), A_FLOAT, 0);
    class_addmethod(ofeliaSetShaderAttribute4f_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute4f_v3),
                    gensym("v3"), A_FLOAT, 0);
    class_addmethod(ofeliaSetShaderAttribute4f_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute4f_v4),
                    gensym("v4"), A_FLOAT, 0);
    class_addmethod(ofeliaSetShaderAttribute4f_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute4f_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderAttribute4f_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute4f_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetShaderAttribute1fv object methods
 */
void *ofeliaSetShaderAttribute1fv_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetShaderAttribute1fv *x = reinterpret_cast<t_ofeliaSetShaderAttribute1fv*>(pd_new(ofeliaSetShaderAttribute1fv_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getSetShaderAttributefvElemFromArgs(argc, argv, x->elem, 1, t_ofeliaSetShaderAttribute1fv::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("v"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetShaderAttribute1fv_bang(t_ofeliaSetShaderAttribute1fv *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (t_ofeliaApplyShader::currentShader)
            t_ofeliaApplyShader::currentShader->setAttribute1fv(x->elem.attributeName.c_str(), x->elem.values.data(), x->elem.values.size());
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetShaderAttribute1fv::objName);
    }
}

void ofeliaSetShaderAttribute1fv_attributeName(t_ofeliaSetShaderAttribute1fv *x, t_symbol *s)
{
    x->elem.attributeName = s->s_name;
}

void ofeliaSetShaderAttribute1fv_v(t_ofeliaSetShaderAttribute1fv *x, t_symbol *s, int argc, t_atom *argv)
{
    x->elem.values.clear();
    
    for (int i=0; i<argc; ++i) {
        
        if (argv[i].a_type == A_FLOAT) {
            
            x->elem.values.push_back(argv[i].a_w.w_float);
        }
        else {
            
            x->elem.values.clear();
            error("%s: wrong argument type", t_ofeliaSetShaderAttribute1fv::objName);
            return;
        }
    }
}

void ofeliaSetShaderAttribute1fv_set(t_ofeliaSetShaderAttribute1fv *x, t_symbol *s, int argc, t_atom *argv)
{
    getSetShaderAttributefvElemFromArgs(argc, argv, x->elem, 1, t_ofeliaSetShaderAttribute1fv::objName);
}

void ofeliaSetShaderAttribute1fv_print(t_ofeliaSetShaderAttribute1fv *x)
{
    post("\n[%s]", t_ofeliaSetShaderAttribute1fv::objName);
    post("attributeName : %s", x->elem.attributeName.c_str());
    post("v : ");
    
    for (size_t i=0; i<x->elem.values.size(); ++i)
        post(" %g", x->elem.values[i]);
}

void ofeliaSetShaderAttribute1fv_setup()
{
    ofeliaSetShaderAttribute1fv_class = class_new(gensym("ofSetShaderAttribute1fv"),
                                                  reinterpret_cast<t_newmethod>(ofeliaSetShaderAttribute1fv_new),
                                                  0, sizeof(t_ofeliaSetShaderAttribute1fv),
                                                  CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetShaderAttribute1fv_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute1fv_bang));
    class_addmethod(ofeliaSetShaderAttribute1fv_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute1fv_attributeName),
                    gensym("attributeName"), A_SYMBOL, 0);
    class_addmethod(ofeliaSetShaderAttribute1fv_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute1fv_v),
                    gensym("v"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderAttribute1fv_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute1fv_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderAttribute1fv_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute1fv_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetShaderAttribute2fv object methods
 */
void *ofeliaSetShaderAttribute2fv_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetShaderAttribute2fv *x = reinterpret_cast<t_ofeliaSetShaderAttribute2fv*>(pd_new(ofeliaSetShaderAttribute2fv_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getSetShaderAttributefvElemFromArgs(argc, argv, x->elem, 2, t_ofeliaSetShaderAttribute2fv::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("v"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetShaderAttribute2fv_bang(t_ofeliaSetShaderAttribute2fv *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (t_ofeliaApplyShader::currentShader)
            t_ofeliaApplyShader::currentShader->setAttribute2fv(x->elem.attributeName.c_str(), x->elem.values.data(), x->elem.values.size());
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetShaderAttribute2fv::objName);
    }
}

void ofeliaSetShaderAttribute2fv_attributeName(t_ofeliaSetShaderAttribute2fv *x, t_symbol *s)
{
    x->elem.attributeName = s->s_name;
}

void ofeliaSetShaderAttribute2fv_v(t_ofeliaSetShaderAttribute2fv *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc%2) {
        
        error("%s: invalid number of arguments", t_ofeliaSetShaderAttribute2fv::objName);
        return;
    }
    x->elem.values.clear();
    
    for (int i=0; i<argc; ++i) {
        
        if (argv[i].a_type == A_FLOAT) {
            
            x->elem.values.push_back(argv[i].a_w.w_float);
        }
        else {
            
            x->elem.values.clear();
            error("%s: wrong argument type", t_ofeliaSetShaderAttribute2fv::objName);
            return;
        }
    }
}

void ofeliaSetShaderAttribute2fv_set(t_ofeliaSetShaderAttribute2fv *x, t_symbol *s, int argc, t_atom *argv)
{
    getSetShaderAttributefvElemFromArgs(argc, argv, x->elem, 2, t_ofeliaSetShaderAttribute2fv::objName);
}

void ofeliaSetShaderAttribute2fv_print(t_ofeliaSetShaderAttribute2fv *x)
{
    post("\n[%s]", t_ofeliaSetShaderAttribute2fv::objName);
    post("attributeName : %s", x->elem.attributeName.c_str());
    post("v : ");
    
    for (size_t i=0; i<x->elem.values.size(); i+=2)
        post(" %g %g", x->elem.values[i], x->elem.values[i+1]);
}

void ofeliaSetShaderAttribute2fv_setup()
{
    ofeliaSetShaderAttribute2fv_class = class_new(gensym("ofSetShaderAttribute2fv"),
                                                  reinterpret_cast<t_newmethod>(ofeliaSetShaderAttribute2fv_new),
                                                  0, sizeof(t_ofeliaSetShaderAttribute2fv),
                                                  CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetShaderAttribute2fv_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute2fv_bang));
    class_addmethod(ofeliaSetShaderAttribute2fv_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute2fv_attributeName),
                    gensym("attributeName"), A_SYMBOL, 0);
    class_addmethod(ofeliaSetShaderAttribute2fv_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute2fv_v),
                    gensym("v"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderAttribute2fv_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute2fv_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderAttribute2fv_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute2fv_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetShaderAttribute3fv object methods
 */
void *ofeliaSetShaderAttribute3fv_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetShaderAttribute3fv *x = reinterpret_cast<t_ofeliaSetShaderAttribute3fv*>(pd_new(ofeliaSetShaderAttribute3fv_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getSetShaderAttributefvElemFromArgs(argc, argv, x->elem, 3, t_ofeliaSetShaderAttribute3fv::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("v"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetShaderAttribute3fv_bang(t_ofeliaSetShaderAttribute3fv *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (t_ofeliaApplyShader::currentShader)
            t_ofeliaApplyShader::currentShader->setAttribute3fv(x->elem.attributeName.c_str(), x->elem.values.data(), x->elem.values.size());
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetShaderAttribute3fv::objName);
    }
}

void ofeliaSetShaderAttribute3fv_attributeName(t_ofeliaSetShaderAttribute3fv *x, t_symbol *s)
{
    x->elem.attributeName = s->s_name;
}

void ofeliaSetShaderAttribute3fv_v(t_ofeliaSetShaderAttribute3fv *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc%3) {
        
        error("%s: invalid number of arguments", t_ofeliaSetShaderAttribute3fv::objName);
        return;
    }
    x->elem.values.clear();
    
    for (int i=0; i<argc; ++i) {
        
        if (argv[i].a_type == A_FLOAT) {
            
            x->elem.values.push_back(argv[i].a_w.w_float);
        }
        else {
            
            x->elem.values.clear();
            error("%s: wrong argument type", t_ofeliaSetShaderAttribute3fv::objName);
            return;
        }
    }
}

void ofeliaSetShaderAttribute3fv_set(t_ofeliaSetShaderAttribute3fv *x, t_symbol *s, int argc, t_atom *argv)
{
    getSetShaderAttributefvElemFromArgs(argc, argv, x->elem, 3, t_ofeliaSetShaderAttribute3fv::objName);
}

void ofeliaSetShaderAttribute3fv_print(t_ofeliaSetShaderAttribute3fv *x)
{
    post("\n[%s]", t_ofeliaSetShaderAttribute3fv::objName);
    post("attributeName : %s", x->elem.attributeName.c_str());
    post("v : ");
    
    for (size_t i=0; i<x->elem.values.size(); i+=3)
        post(" %g %g %g", x->elem.values[i], x->elem.values[i+1], x->elem.values[i+2]);
}

void ofeliaSetShaderAttribute3fv_setup()
{
    ofeliaSetShaderAttribute3fv_class = class_new(gensym("ofSetShaderAttribute3fv"),
                                                  reinterpret_cast<t_newmethod>(ofeliaSetShaderAttribute3fv_new),
                                                  0, sizeof(t_ofeliaSetShaderAttribute3fv),
                                                  CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetShaderAttribute3fv_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute3fv_bang));
    class_addmethod(ofeliaSetShaderAttribute3fv_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute3fv_attributeName),
                    gensym("attributeName"), A_SYMBOL, 0);
    class_addmethod(ofeliaSetShaderAttribute3fv_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute3fv_v),
                    gensym("v"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderAttribute3fv_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute3fv_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderAttribute3fv_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute3fv_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSetShaderAttribute4fv object methods
 */
void *ofeliaSetShaderAttribute4fv_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSetShaderAttribute4fv *x = reinterpret_cast<t_ofeliaSetShaderAttribute4fv*>(pd_new(ofeliaSetShaderAttribute4fv_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getSetShaderAttributefvElemFromArgs(argc, argv, x->elem, 4, t_ofeliaSetShaderAttribute4fv::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("v"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSetShaderAttribute4fv_bang(t_ofeliaSetShaderAttribute4fv *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        if (t_ofeliaApplyShader::currentShader)
            t_ofeliaApplyShader::currentShader->setAttribute4fv(x->elem.attributeName.c_str(), x->elem.values.data(), x->elem.values.size());
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSetShaderAttribute4fv::objName);
    }
}

void ofeliaSetShaderAttribute4fv_attributeName(t_ofeliaSetShaderAttribute4fv *x, t_symbol *s)
{
    x->elem.attributeName = s->s_name;
}

void ofeliaSetShaderAttribute4fv_v(t_ofeliaSetShaderAttribute4fv *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc%4) {
        
        error("%s: invalid number of arguments", t_ofeliaSetShaderAttribute4fv::objName);
        return;
    }
    x->elem.values.clear();
    
    for (int i=0; i<argc; ++i) {
        
        if (argv[i].a_type == A_FLOAT) {
            
            x->elem.values.push_back(argv[i].a_w.w_float);
        }
        else {
            
            x->elem.values.clear();
            error("%s: wrong argument type", t_ofeliaSetShaderAttribute4fv::objName);
            return;
        }
    }
}

void ofeliaSetShaderAttribute4fv_set(t_ofeliaSetShaderAttribute4fv *x, t_symbol *s, int argc, t_atom *argv)
{
    getSetShaderAttributefvElemFromArgs(argc, argv, x->elem, 4, t_ofeliaSetShaderAttribute4fv::objName);
}

void ofeliaSetShaderAttribute4fv_print(t_ofeliaSetShaderAttribute4fv *x)
{
    post("\n[%s]", t_ofeliaSetShaderAttribute4fv::objName);
    post("attributeName : %s", x->elem.attributeName.c_str());
    post("v : ");
    
    for (size_t i=0; i<x->elem.values.size(); i+=4)
        post(" %g %g %g %g", x->elem.values[i], x->elem.values[i+1], x->elem.values[i+2], x->elem.values[i+3]);
}

void ofeliaSetShaderAttribute4fv_setup()
{
    ofeliaSetShaderAttribute4fv_class = class_new(gensym("ofSetShaderAttribute4fv"),
                                                  reinterpret_cast<t_newmethod>(ofeliaSetShaderAttribute4fv_new),
                                                  0, sizeof(t_ofeliaSetShaderAttribute4fv),
                                                  CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSetShaderAttribute4fv_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute4fv_bang));
    class_addmethod(ofeliaSetShaderAttribute4fv_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute4fv_attributeName),
                    gensym("attributeName"), A_SYMBOL, 0);
    class_addmethod(ofeliaSetShaderAttribute4fv_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute4fv_v),
                    gensym("v"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderAttribute4fv_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute4fv_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSetShaderAttribute4fv_class, reinterpret_cast<t_method>(ofeliaSetShaderAttribute4fv_print),
                    gensym("print"), A_NULL, 0);
}


/* ________________________________________________________________________________
 * setup methods
 */
void ofeliaShader_setup()
{
    ofeliaLoadShader_setup();
    ofeliaApplyShader_setup();
    ofeliaDoesShaderNameExist_setup();
    ofeliaGetShaderPath_setup();
    ofeliaIsShaderLoaded_setup();
    ofeliaSetShaderUniform1i_setup();
    ofeliaSetShaderUniform2i_setup();
    ofeliaSetShaderUniform3i_setup();
    ofeliaSetShaderUniform4i_setup();
    ofeliaSetShaderUniform1f_setup();
    ofeliaSetShaderUniform2f_setup();
    ofeliaSetShaderUniform3f_setup();
    ofeliaSetShaderUniform4f_setup();
    ofeliaSetShaderUniform1iv_setup();
    ofeliaSetShaderUniform2iv_setup();
    ofeliaSetShaderUniform3iv_setup();
    ofeliaSetShaderUniform4iv_setup();
    ofeliaSetShaderUniform1fv_setup();
    ofeliaSetShaderUniform2fv_setup();
    ofeliaSetShaderUniform3fv_setup();
    ofeliaSetShaderUniform4fv_setup();
    ofeliaSetShaderUniformTex_setup();
    ofeliaSetShaderAttribute1f_setup();
    ofeliaSetShaderAttribute2f_setup();
    ofeliaSetShaderAttribute3f_setup();
    ofeliaSetShaderAttribute4f_setup();
    ofeliaSetShaderAttribute1fv_setup();
    ofeliaSetShaderAttribute2fv_setup();
    ofeliaSetShaderAttribute3fv_setup();
    ofeliaSetShaderAttribute4fv_setup();
}

