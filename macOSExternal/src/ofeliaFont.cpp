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

#include "ofeliaFont.h"
#include "ofeliaWindow.h"

/* ________________________________________________________________________________
 * static variables
 */
const char *t_ofeliaLoadFont::objName = "ofLoadFont";
unsigned int t_ofeliaLoadFont::counter;
vector<t_ofeliaLoadFontData> t_ofeliaLoadFont::fontData;
vector<vector<shared_ptr<ofTrueTypeFont>>> t_ofeliaLoadFont::fonts;
bool t_ofeliaLoadFont::bInited;
const char *t_ofeliaEditFont::objName = "ofEditFont";
const char *t_ofeliaBindFontTex::objName = "ofBindFontTex";
const char *t_ofeliaDrawText::objName = "ofDrawText";
const char *t_ofeliaDrawTextAsShapes::objName = "ofDrawTextAsShapes";
const char *t_ofeliaDoesFontNameExist::objName = "ofDoesFontNameExist";
const char *t_ofeliaGetFontPath::objName = "ofGetFontPath";
const char *t_ofeliaGetFontSize::objName = "ofGetFontSize";
const char *t_ofeliaIsFontLoaded::objName = "ofIsFontLoaded";
const char *t_ofeliaGetFontLetterSpacing::objName = "ofGetFontLetterSpacing";
const char *t_ofeliaGetFontLineHeight::objName = "ofGetFontLineHeight";
const char *t_ofeliaGetFontSpaceSize::objName = "ofGetFontSpaceSize";
const char *t_ofeliaGetTextBoundingBox::objName = "ofGetTextBoundingBox";
const char *t_ofeliaGetTextMesh2dCommands::objName = "ofGetTextMesh2dCommands";
const char *t_ofeliaGetTextMesh3dCommands::objName = "ofGetTextMesh3dCommands";

/* ________________________________________________________________________________
 * ofLoadFont object methods
 */
void ofeliaLoadFont_clear(t_ofeliaLoadFont *x);
void ofeliaLoadFont_vecSizeOut(t_ofeliaLoadFont *x);

int getPositionByFontObjID(const unsigned int objID)
{
    const auto it = find_if(begin(t_ofeliaLoadFont::fontData), end(t_ofeliaLoadFont::fontData),
                            [&objID](const t_ofeliaLoadFontData &fontData)
                            {
                                return fontData.objID == objID;
                            });
    return static_cast<int>(it - t_ofeliaLoadFont::fontData.begin());
}

bool getLoadFontElemFromArgs(t_ofeliaLoadFont *x, const int argc, const t_atom *argv,
                             t_ofeliaLoadFontElem &elem)
{
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL &&
            argv[1].a_type == A_FLOAT) {
            
            elem.path = argv[0].a_w.w_symbol->s_name;
            elem.fontSize = static_cast<int>(argv[1].a_w.w_float);
            elem.fontEditID = 0;
            
            if (!doesFilenameHaveExtension(elem.path)) {
             
                error("%s: wrong file format '%s'", t_ofeliaLoadFont::objName, elem.path.c_str());
                return 0;
            }
            if (!makePathAbsolute(elem.path, x->canvas)) {
                
                error("%s: failed to find '%s'", t_ofeliaLoadFont::objName, elem.path.c_str());
                return 0;
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadFont::objName);
            return 0;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadFont::objName);
        return 0;
    }
    return 1;
}

bool getLoadFontElemsFromArgs(t_ofeliaLoadFont *x, const int argc, const t_atom *argv,
                              vector<t_ofeliaLoadFontElem> &elems)
{
    if (argc && argc%2 == 0) {
        
        for (int i=0; i<argc; i+=2) {
            
            if (argv[i].a_type == A_SYMBOL &&
                argv[i+1].a_type == A_FLOAT) {
                
                t_ofeliaLoadFontElem elem;
                elem.path = argv[i].a_w.w_symbol->s_name;
                elem.fontSize = static_cast<int>(argv[i+1].a_w.w_float);
                elem.fontEditID = 0;
                
                if (!doesFilenameHaveExtension(elem.path)) {
                    
                    error("%s: wrong file format '%s'", t_ofeliaLoadFont::objName, elem.path.c_str());
                    return 0;
                }
                if (!makePathAbsolute(elem.path, x->canvas)) {
                    
                    error("%s: failed to find '%s'", t_ofeliaLoadFont::objName, elem.path.c_str());
                    return 0;
                }
                elems.push_back(elem);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaLoadFont::objName);
                return 0;
            }
        }
        return 1;
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadFont::objName);
        return 0;
    }
    return 1;
}

bool getCmdRangeFromArgs(const int argc, const t_atom *argv, t_ofeliaLoadFontCmdData &cmd)
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
                
                error("%s: wrong argument type", t_ofeliaLoadFont::objName);
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
                
                error("%s: wrong argument type", t_ofeliaLoadFont::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaLoadFont::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaLoadFont_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadFont *x = reinterpret_cast<t_ofeliaLoadFont*>(pd_new(ofeliaLoadFont_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    t_ofeliaLoadFontData fontData;
    getVarNameLocalPrefixes(fontData.varName);
    vector<t_ofeliaLoadFontElem> elems;
    x->canvas = canvas_getcurrent();
    
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            fontData.varName.name = argv[0].a_w.w_symbol->s_name;
            getVarNameLocalized(fontData.varName);
            
            if (--argc) {
                
                if (!getLoadFontElemsFromArgs(x, argc, argv+1, elems))
                    return 0;
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadFont::objName);
            return 0;
        }
    }
    x->failPathsShouldClear = false;
    x->vecSizesShouldClear = false;
    x->shouldOutlet = false;
    x->bInitGate = false;
    x->ftID = 0;
    x->objID = fontData.objID = t_ofeliaLoadFont::counter++;
    t_ofeliaLoadFont::fontData.push_back(fontData);
    t_ofeliaLoadFont::fonts.push_back(vector<shared_ptr<ofTrueTypeFont>>());
    x->fontLoaded = make_unique<vector<char>>();
    x->fontIDs = make_unique<vector<unsigned int>>();
    x->tempFontIDs = make_unique<vector<unsigned int>>();
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::updateSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    x->jobDone_out = outlet_new(&x->x_obj, &s_bang);
    x->vecSize_out = outlet_new(&x->x_obj, &s_float);
    
    if (argc) {
        
        ofeliaLoadFont_clear(x);
        
        for (size_t i=0; i<elems.size(); ++i) {
            
            t_ofeliaLoadFontCmdData cmd;
            cmd.elem = elems[i];
            cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
            cmd.state = FONT_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
    return (x);
}

void ofeliaLoadFont_init(t_ofeliaLoadFont *x)
{
    if (!t_ofeliaLoadFont::bInited && x->bInitGate) {
        
        t_ofeliaLoadFont::fonts.clear();

        for (size_t i=0; i<t_ofeliaLoadFont::fontData.size(); ++i) {
            
            t_ofeliaLoadFont::fonts.push_back(vector<shared_ptr<ofTrueTypeFont>>());
            
            for (size_t j=0; j<t_ofeliaLoadFont::fontData[i].elems.size(); ++j)
                t_ofeliaLoadFont::fonts[i].push_back(make_shared<ofTrueTypeFont>());
        }
        t_ofeliaLoadFont::bInited = true;
    }
}

void ofeliaLoadFont_update(t_ofeliaLoadFont *x)
{
    const int pos = getPositionByFontObjID(x->objID);
    
    if (!x->cmdVec.empty()) {
        
        const size_t cmdVecSize = x->cmdVec.size();
        
        /* read all recorded commands */
        for (size_t i=0; i<cmdVecSize; ++i) {
            
            const int size = static_cast<int>(t_ofeliaLoadFont::fontData[pos].elems.size());
            const auto elemsBegin = t_ofeliaLoadFont::fontData[pos].elems.begin();
            const auto fontsBegin = t_ofeliaLoadFont::fonts[pos].begin();
            const auto fontLoadedBegin = x->fontLoaded->begin();
            const auto fontIDsBegin = x->fontIDs->begin();
            const t_ofeliaLoadFontElem &elem = x->cmdVec[i].elem;
            
            switch (x->cmdVec[i].state) {
                    
                case FONT_LOAD_CMD_INSERT:
                {
                    const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, size));
                    const int insertSize = static_cast<int>(max(0.0f, max(0.0f, x->cmdVec[i].toIndex) - max(0.0f, x->cmdVec[i].fromIndex))) + 1;
                    t_ofeliaLoadFont::fontData[pos].elems.insert(elemsBegin + fromIndex,
                                                              insertSize, elem);
                    t_ofeliaLoadFont::fonts[pos].insert(fontsBegin + fromIndex,
                                                     insertSize, make_shared<ofTrueTypeFont>());
                    x->fontLoaded->insert(fontLoadedBegin + fromIndex, insertSize, 0);
                    x->fontIDs->insert(fontIDsBegin + fromIndex, insertSize, 0);
                    break;
                }
                case FONT_LOAD_CMD_ERASE:
                {
                    if (size) {
                        
                        const int maxIndex = size - 1;
                        const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, maxIndex));
                        const int toIndex = static_cast<int>(ofClamp(x->cmdVec[i].toIndex, fromIndex, maxIndex)) + 1;
                        t_ofeliaLoadFont::fontData[pos].elems.erase(elemsBegin + fromIndex,
                                                                 elemsBegin + toIndex);
                        t_ofeliaLoadFont::fonts[pos].erase(fontsBegin + fromIndex,
                                                        fontsBegin + toIndex);
                        x->fontLoaded->erase(fontLoadedBegin + fromIndex, fontLoadedBegin + toIndex);
                        x->fontIDs->erase(fontIDsBegin + fromIndex, fontIDsBegin + toIndex);
                        x->shouldOutlet = true;
                    }
                    break;
                }
                case FONT_LOAD_CMD_FILL:
                {
                    if (size) {
                        
                        const int maxIndex = size - 1;
                        const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, maxIndex));
                        const int toIndex = static_cast<int>(ofClamp(x->cmdVec[i].toIndex, fromIndex, maxIndex)) + 1;
                        fill(elemsBegin + fromIndex, elemsBegin + toIndex, elem);
                        fill(fontIDsBegin + fromIndex, fontIDsBegin + toIndex, 0);
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
        /* find font elements to load. store IDs */
        for (size_t i=0; i<x->fontIDs->size(); ++i) {
            
            if (x->fontIDs->at(i) == 0) {
                
                x->ftID++;
                x->fontIDs->at(i) = x->ftID;
                x->tempFontIDs->push_back(x->ftID);
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
    if (!x->tempFontIDs->empty()) {
        
        /* load fonts based on stored IDs */
        for (size_t i=0; i<x->fontIDs->size(); ++i) {
            
            if (x->fontIDs->at(i) == x->tempFontIDs->at(0)) {
                
                t_ofeliaLoadFont::fonts[pos][i]->load(t_ofeliaLoadFont::fontData[pos].elems[i].path.c_str(),
                                                      t_ofeliaLoadFont::fontData[pos].elems[i].fontSize*ofeliaWindow::retinaScale,
                                                   true, true, true, 0.1);
                
                if (!t_ofeliaLoadFont::fonts[pos][i]->isLoaded())
                    x->failPaths.push_back(t_ofeliaLoadFont::fontData[pos].elems[i].path);
                x->fontLoaded->at(i) = 1;
                x->shouldOutlet = true;
                break;
            }
        }
        x->tempFontIDs->erase(x->tempFontIDs->begin());
    }
    /* output size when there's change. report error if loading fails */
    if (x->shouldOutlet) {
        
        if (x->vecSizesShouldClear) {
            
            x->vecSizes.clear();
            x->vecSizesShouldClear = false;
        }
        const int numLoadedFonts = count_if(x->fontLoaded->begin(),
                                            x->fontLoaded->end(),
                                            [] (char c)
                                            {
                                                return c != 0;
                                            });
        x->vecSizes.push_back(numLoadedFonts);
        ofeliaLoadFont_vecSizeOut(x);
        x->shouldOutlet = false;
    }
}

void ofeliaLoadFont_exit(t_ofeliaLoadFont *x)
{
    if (t_ofeliaLoadFont::bInited)
        t_ofeliaLoadFont::bInited = false;
    fill(x->fontLoaded->begin(), x->fontLoaded->end(), 0); /* mark unloaded */
    *x->tempFontIDs = *x->fontIDs; /* copy for reloading */
    x->vecSizes.clear();
    x->failPathsShouldClear = false;
    x->vecSizesShouldClear = false;
    x->shouldOutlet = false;
    x->bInitGate = true;
}

void ofeliaLoadFont_vecSizeOut(t_ofeliaLoadFont *x)
{
    if (!x->failPaths.empty()) {
        
        for (size_t i=0; i<x->failPaths.size(); ++i)
            error("%s: failed to load '%s'", t_ofeliaLoadFont::objName, x->failPaths[i].c_str());
        x->failPathsShouldClear = true;
    }
    if (!x->vecSizes.empty()) {
        
        for (size_t i=0; i<x->vecSizes.size(); ++i)
            outlet_float(x->vecSize_out, static_cast<t_float>(x->vecSizes[i]));
        x->vecSizesShouldClear = true;
    }
    if (x->tempFontIDs->empty())
        outlet_bang(x->jobDone_out);
}

void ofeliaLoadFont_name(t_ofeliaLoadFont *x, t_symbol *s)
{
    const int pos = getPositionByFontObjID(x->objID);
    t_ofeliaLoadFont::fontData[pos].varName.name = s->s_name;
    getVarNameLocalized(t_ofeliaLoadFont::fontData[pos].varName);
}

void ofeliaLoadFont_load(t_ofeliaLoadFont *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaLoadFontElem> elems;
    
    if (getLoadFontElemsFromArgs(x, argc, argv, elems)) {
        
        ofeliaLoadFont_clear(x);
        
        for (size_t i=0; i<elems.size(); ++i) {
            
            t_ofeliaLoadFontCmdData cmd;
            cmd.elem = elems[i];
            cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
            cmd.state = FONT_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadFont_add(t_ofeliaLoadFont *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadFontCmdData cmd;
    
    if (getLoadFontElemFromArgs(x, argc, argv, cmd.elem)) {
        
        cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
        cmd.state = FONT_LOAD_CMD_INSERT;
        x->cmdVec.push_back(cmd);
    }
}

void ofeliaLoadFont_append(t_ofeliaLoadFont *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaLoadFontElem> elems;
    
    if (getLoadFontElemsFromArgs(x, argc, argv, elems)) {
        
        for (size_t i=0; i<elems.size(); ++i) {
            
            t_ofeliaLoadFontCmdData cmd;
            cmd.elem = elems[i];
            cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
            cmd.state = FONT_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadFont_prepend(t_ofeliaLoadFont *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaLoadFontElem> elems;
    
    if (getLoadFontElemsFromArgs(x, argc, argv, elems)) {
        
        for (size_t i=0; i<elems.size(); ++i) {
            
            t_ofeliaLoadFontCmdData cmd;
            cmd.elem = elems[i];
            cmd.fromIndex = cmd.toIndex = static_cast<t_float>(i);
            cmd.state = FONT_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadFont_insert(t_ofeliaLoadFont *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadFontCmdData cmd;
    
    if (getLoadFontElemFromArgs(x, argc, argv, cmd.elem)) {
        
        if (getCmdRangeFromArgs(argc-2, argv+2, cmd)) {
            
            cmd.state = FONT_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadFont_fill(t_ofeliaLoadFont *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadFontCmdData cmd;
    
    if (getLoadFontElemFromArgs(x, argc, argv, cmd.elem)) {
        
        if (getCmdRangeFromArgs(argc-2, argv+2, cmd)) {
            
            cmd.state = FONT_LOAD_CMD_FILL;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadFont_erase(t_ofeliaLoadFont *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadFontCmdData cmd;
    
    if (getCmdRangeFromArgs(argc, argv, cmd)) {
        
        cmd.state = FONT_LOAD_CMD_ERASE;
        x->cmdVec.push_back(cmd);
    }
}

void ofeliaLoadFont_clear(t_ofeliaLoadFont *x)
{
    t_ofeliaLoadFontCmdData cmd;
    cmd.fromIndex = 0.0f;
    cmd.toIndex = numeric_limits<t_float>::max();
    cmd.state = FONT_LOAD_CMD_ERASE;
    x->cmdVec.push_back(cmd);
}

void ofeliaLoadFont_set(t_ofeliaLoadFont *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            ofeliaLoadFont_name(x, argv[0].a_w.w_symbol);
            
            if (--argc)
                ofeliaLoadFont_load(x, 0, argc, argv+1);
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadFont::objName);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadFont::objName);
    }
}

void ofeliaLoadFont_print(t_ofeliaLoadFont *x)
{
    post("\n[%s]", t_ofeliaLoadFont::objName);
    const int pos = getPositionByFontObjID(x->objID);
    post("name : %s", t_ofeliaLoadFont::fontData[pos].varName.name.c_str());
    post("load : ");
    
    for (size_t i=0; i<t_ofeliaLoadFont::fontData[pos].elems.size(); ++i)
        post(" %s %d", t_ofeliaLoadFont::fontData[pos].elems[i].path.c_str(), t_ofeliaLoadFont::fontData[pos].elems[i].fontSize);
}

void ofeliaLoadFont_free(t_ofeliaLoadFont *x)
{
    const int pos = getPositionByFontObjID(x->objID);
    t_ofeliaLoadFont::fontData.erase(t_ofeliaLoadFont::fontData.begin() + pos);
    t_ofeliaLoadFont::fonts.erase(t_ofeliaLoadFont::fonts.begin() + pos);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::updateSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    outlet_free(x->jobDone_out);
    outlet_free(x->vecSize_out);
}

void ofeliaLoadFont_setup()
{
    ofeliaLoadFont_class = class_new(gensym("ofLoadFont"),
                                     reinterpret_cast<t_newmethod>(ofeliaLoadFont_new),
                                     reinterpret_cast<t_method>(ofeliaLoadFont_free),
                                     sizeof(t_ofeliaLoadFont),
                                     CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaLoadFont_class, reinterpret_cast<t_method>(ofeliaLoadFont_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaLoadFont_class, reinterpret_cast<t_method>(ofeliaLoadFont_update),
                    t_ofeliaWindow::updateMess, A_NULL, 0);
    class_addmethod(ofeliaLoadFont_class, reinterpret_cast<t_method>(ofeliaLoadFont_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addmethod(ofeliaLoadFont_class, reinterpret_cast<t_method>(ofeliaLoadFont_name),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaLoadFont_class, reinterpret_cast<t_method>(ofeliaLoadFont_load),
                    gensym("load"), A_GIMME, 0);
    class_addmethod(ofeliaLoadFont_class, reinterpret_cast<t_method>(ofeliaLoadFont_add),
                    gensym("add"), A_GIMME, 0);
    class_addmethod(ofeliaLoadFont_class, reinterpret_cast<t_method>(ofeliaLoadFont_append),
                    gensym("append"), A_GIMME, 0);
    class_addmethod(ofeliaLoadFont_class, reinterpret_cast<t_method>(ofeliaLoadFont_prepend),
                    gensym("prepend"), A_GIMME, 0);
    class_addmethod(ofeliaLoadFont_class, reinterpret_cast<t_method>(ofeliaLoadFont_insert),
                    gensym("insert"), A_GIMME, 0);
    class_addmethod(ofeliaLoadFont_class, reinterpret_cast<t_method>(ofeliaLoadFont_fill),
                    gensym("fill"), A_GIMME, 0);
    class_addmethod(ofeliaLoadFont_class, reinterpret_cast<t_method>(ofeliaLoadFont_erase),
                    gensym("erase"), A_GIMME, 0);
    class_addmethod(ofeliaLoadFont_class, reinterpret_cast<t_method>(ofeliaLoadFont_clear),
                    gensym("clear"), A_NULL, 0);
    class_addmethod(ofeliaLoadFont_class, reinterpret_cast<t_method>(ofeliaLoadFont_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaLoadFont_class, reinterpret_cast<t_method>(ofeliaLoadFont_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * common methods
 */
int getPositionByFontName(const t_string &name)
{
    auto it = find_if(begin(t_ofeliaLoadFont::fontData), end(t_ofeliaLoadFont::fontData),
                      [&name] (const t_ofeliaLoadFontData &fontData) {
                          return (fontData.varName.name == name);
                      });
    if (it == end(t_ofeliaLoadFont::fontData))
        return -1;
    return static_cast<int>(it - begin(t_ofeliaLoadFont::fontData));
}

/* ________________________________________________________________________________
 * ofEditFont object methods
 */
void ofeliaEditFont_jobDoneOut(t_ofeliaEditFont *x);

void *ofeliaEditFont_new(t_symbol *s)
{
    t_ofeliaEditFont *x = reinterpret_cast<t_ofeliaEditFont*>(pd_new(ofeliaEditFont_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    x->bInitGate = false;
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::updateSym);
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaEditFont_float(t_ofeliaEditFont *x, t_floatarg f)
{
    if (x->cmdVec.empty())
        x->varName.index = max(0, static_cast<int>(f));
    else
        error("%s: cannot change index while editing", t_ofeliaEditFont::objName);
}

void ofeliaEditFont_update(t_ofeliaEditFont *x)
{
    if (!x->cmdVec.empty()) {
        
        x->errorType = FONT_EDIT_ERROR_NOERROR;
        const t_string &name = x->varName.name;
        const size_t cmdVecSize = x->cmdVec.size();
        
        if (!name.empty()) {
            
            const int pos = getPositionByFontName(name);
            
            if (pos != -1) {
                
                if (!t_ofeliaLoadFont::fontData[pos].elems.empty()) {
                    
                    const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadFont::fontData[pos].elems.size())-1);
                    
                    if (!t_ofeliaLoadFont::fonts[pos].empty() &&
                        index < static_cast<int>(t_ofeliaLoadFont::fonts[pos].size())) {
                        
                        if (!t_ofeliaLoadFont::fonts[pos][index]->isLoaded())
                            return;
                        
                        for (size_t i=0; i<cmdVecSize; ++i) {
                            
                            switch (x->cmdVec[i].state) {
                                    
                                case FONT_EDIT_CMD_LETTERSPACING:
                                {
                                    t_ofeliaLoadFont::fonts[pos][index]->setLetterSpacing(x->cmdVec[i].arg);
                                    break;
                                }
                                case FONT_EDIT_CMD_LINEHEIGHT:
                                {
                                    t_ofeliaLoadFont::fonts[pos][index]->setLineHeight(x->cmdVec[i].arg);
                                    break;
                                }
                                case FONT_EDIT_CMD_SPACESIZE:
                                {
                                    t_ofeliaLoadFont::fonts[pos][index]->setSpaceSize(x->cmdVec[i].arg);
                                    break;
                                }
                                case FONT_EDIT_CMD_CLONE:
                                {
                                    const t_string &fontName = x->cmdVec[i].varName.name;
                                    const size_t fontPos = getPositionByFontName(fontName);
                                    
                                    if (fontPos != -1) {
                                        
                                        if (!t_ofeliaLoadFont::fontData[fontPos].elems.empty()) {
                                            
                                            const int fontIndex = min(x->cmdVec[i].varName.index, static_cast<int>(t_ofeliaLoadFont::fontData[fontPos].elems.size())-1);
                                            t_ofeliaLoadFont::fonts[pos][index] = t_ofeliaLoadFont::fonts[fontPos][fontIndex];
                                        }
                                        else {
                                            
                                            x->errorType = FONT_EDIT_ERROR_EMPTY;
                                            x->errorName = fontName;
                                        }
                                    }
                                    else {
                                        
                                        x->errorType = FONT_EDIT_ERROR_FAILTOFIND;
                                        x->errorName = fontName;
                                    }
                                    break;
                                }
                                default:
                                {
                                    break;
                                }
                            }
                        }
                        ++t_ofeliaLoadFont::fontData[pos].elems[index].fontEditID;
                    }
                    else {
                        
                        return;
                    }
                }
                else {
                    
                    x->errorType = FONT_EDIT_ERROR_EMPTY;
                    x->errorName = name;
                }
            }
            else {
                
                x->errorType = FONT_EDIT_ERROR_FAILTOFIND;
                x->errorName = name;
            }
        }
        else {
            
            x->errorType = FONT_EDIT_ERROR_NOTASSIGNED;
        }
        if (x->cmdVec.size() > cmdVecSize) {
            
            x->cmdVec.erase(x->cmdVec.begin(), x->cmdVec.begin() + cmdVecSize);
            return;
        }
        x->cmdVec.clear();
        ofeliaEditFont_jobDoneOut(x);
    }
}

void ofeliaEditFont_jobDoneOut(t_ofeliaEditFont *x)
{
    if (x->errorType == FONT_EDIT_ERROR_NOERROR) {
        
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        switch (x->errorType) {
                
            case FONT_EDIT_ERROR_NOTASSIGNED:
                error("%s: name not assigned", t_ofeliaEditFont::objName);
                break;
            case FONT_EDIT_ERROR_FAILTOFIND:
                error("%s: failed to find '%s'", t_ofeliaEditFont::objName, x->errorName.c_str());
                break;
            case FONT_EDIT_ERROR_EMPTY:
                error("%s: '%s' is empty", t_ofeliaEditFont::objName, x->errorName.c_str());
                break;
            default:
                break;
        }
    }
}

void ofeliaEditFont_letterSpacing(t_ofeliaEditFont *x, t_floatarg f)
{
    t_ofeliaEditFontCmdData cmd;
    cmd.arg = f;
    cmd.state = FONT_EDIT_CMD_LETTERSPACING;
    x->cmdVec.push_back(cmd);
}

void ofeliaEditFont_lineHeight(t_ofeliaEditFont *x, t_floatarg f)
{
    t_ofeliaEditFontCmdData cmd;
    cmd.arg = f;
    cmd.state = FONT_EDIT_CMD_LINEHEIGHT;
    x->cmdVec.push_back(cmd);
}

void ofeliaEditFont_spaceSize(t_ofeliaEditFont *x, t_floatarg f)
{
    if (f < 0.0f) {
        
        error("%s: spaceSize out of range", t_ofeliaEditFont::objName);
        return;
    }
    t_ofeliaEditFontCmdData cmd;
    cmd.arg = f;
    cmd.state = FONT_EDIT_CMD_SPACESIZE;
    x->cmdVec.push_back(cmd);
}

void ofeliaEditFont_clone(t_ofeliaEditFont *x, t_symbol *s)
{
    t_ofeliaEditFontCmdData cmd;
    cmd.varName = {s->s_name, x->varName.patchPrefix, x->varName.canvasPrefix, 0};
    getVarNameLocalized(cmd.varName);
    getVarNameIndexed(cmd.varName);
    cmd.state = FONT_EDIT_CMD_CLONE;
    x->cmdVec.push_back(cmd);
}

void ofeliaEditFont_set(t_ofeliaEditFont *x, t_symbol *s)
{
    if (x->cmdVec.empty()) {
        
        x->varName.name = s->s_name;
        getVarNameLocalized(x->varName);
        getVarNameIndexed(x->varName);
    }
    else {
        
        error("%s: cannot change name while editing", t_ofeliaEditFont::objName);
    }
}

void ofeliaEditFont_print(t_ofeliaEditFont *x)
{
    post("\n[%s]", t_ofeliaEditFont::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaEditFont_free(t_ofeliaEditFont *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::updateSym);
}

void ofeliaEditFont_setup()
{
    ofeliaEditFont_class = class_new(gensym("ofEditFont"),
                                     reinterpret_cast<t_newmethod>(ofeliaEditFont_new),
                                     reinterpret_cast<t_method>(ofeliaEditFont_free),
                                     sizeof(t_ofeliaEditFont),
                                     CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaEditFont_class, reinterpret_cast<t_method>(ofeliaEditFont_float));
    class_addmethod(ofeliaEditFont_class, reinterpret_cast<t_method>(ofeliaEditFont_update),
                    t_ofeliaWindow::updateMess, A_NULL, 0);
    class_addmethod(ofeliaEditFont_class, reinterpret_cast<t_method>(ofeliaEditFont_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditFont_class, reinterpret_cast<t_method>(ofeliaEditFont_letterSpacing),
                    gensym("letterSpacing"), A_FLOAT, 0);
    class_addmethod(ofeliaEditFont_class, reinterpret_cast<t_method>(ofeliaEditFont_lineHeight),
                    gensym("lineHeight"), A_FLOAT, 0);
    class_addmethod(ofeliaEditFont_class, reinterpret_cast<t_method>(ofeliaEditFont_spaceSize),
                    gensym("spaceSize"), A_FLOAT, 0);
    class_addmethod(ofeliaEditFont_class, reinterpret_cast<t_method>(ofeliaEditFont_clone),
                    gensym("clone"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditFont_class, reinterpret_cast<t_method>(ofeliaEditFont_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditFont_class, reinterpret_cast<t_method>(ofeliaEditFont_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofBindFontTex object methods
 */
void *ofeliaBindFontTex_new(t_symbol *s)
{
    t_ofeliaBindFontTex *x = reinterpret_cast<t_ofeliaBindFontTex*>(pd_new(ofeliaBindFontTex_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaBindFontTex_bang(t_ofeliaBindFontTex *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const t_string &name = x->varName.name;
        
        if (!name.empty()) {
            
            const int pos = getPositionByFontName(name);
            
            if (pos != -1 && !t_ofeliaLoadFont::fontData[pos].elems.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadFont::fontData[pos].elems.size())-1);
                
                if (!t_ofeliaLoadFont::fonts[pos].empty() &&
                    index < static_cast<int>(t_ofeliaLoadFont::fonts[pos].size())) {
                    
                    t_ofeliaLoadFont::fonts[pos][index]->getFontTexture().bind();
                    outlet_bang(x->x_obj.ob_outlet);
                    t_ofeliaLoadFont::fonts[pos][index]->getFontTexture().unbind();
                }
                else {
                    
                    outlet_bang(x->x_obj.ob_outlet);
                }
            }
            else {
                
                outlet_bang(x->x_obj.ob_outlet);
            }
        }
        else {
            
            outlet_bang(x->x_obj.ob_outlet);
        }
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaBindFontTex::objName);
    }
}

void ofeliaBindFontTex_float(t_ofeliaBindFontTex *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaBindFontTex_set(t_ofeliaBindFontTex *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaBindFontTex_print(t_ofeliaBindFontTex *x)
{
    post("\n[%s]", t_ofeliaBindFontTex::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaBindFontTex_setup()
{
    ofeliaBindFontTex_class = class_new(gensym("ofBindFontTex"),
                                        reinterpret_cast<t_newmethod>(ofeliaBindFontTex_new),
                                        0, sizeof(t_ofeliaBindFontTex),
                                        CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaBindFontTex_class, reinterpret_cast<t_method>(ofeliaBindFontTex_bang));
    class_addfloat(ofeliaBindFontTex_class, reinterpret_cast<t_method>(ofeliaBindFontTex_float));
    class_addmethod(ofeliaBindFontTex_class, reinterpret_cast<t_method>(ofeliaBindFontTex_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaBindFontTex_class, reinterpret_cast<t_method>(ofeliaBindFontTex_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaBindFontTex_class, reinterpret_cast<t_method>(ofeliaBindFontTex_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * common methods
 */
void getTextLinesFromArgs(const int argc, const t_atom *argv, vector<TextLineElem> &textLines)
{
    textLines.clear();
    TextLineElem textLine;
    
    for (int i=0; i<argc; ++i) {
        
        if (argv[i].a_type == A_SYMBOL) {
            
            t_string word = argv[i].a_w.w_symbol->s_name;
            
            for (int j=0; j<word.length(); ++j) {
                
                if (word[j] == '\n') { /* detect new line */

                    textLines.push_back(textLine);
                    textLine.text.clear();
                }
                else if (word[j] == '\t') { /* detect tab */
                    
                    textLine.text += "    ";
                }
                else {
                    
                    textLine.text += word[j];
                }
            }
        }
        else if (argv[i].a_type == A_FLOAT) {
         
            textLine.text += ofToString(argv[i].a_w.w_float).c_str();
        }
        textLine.text += ' ';
    }
    textLine.text.pop_back();
    textLines.push_back(textLine);
}

bool getDrawTextElemFromArgs(int argc, t_atom *argv, t_ofeliaDrawTextElem &elem, const char *objName)
{
    switch (argc) {
            
        case 0:
        {
            elem.varName.name = "";
            elem.textLines = {{"", 0.0f, 0.0f}};
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_SYMBOL) {
                
                elem.varName.name = argv[0].a_w.w_symbol->s_name;
                getVarNameLocalized(elem.varName);
                getVarNameIndexed(elem.varName);
                elem.textLines = {{"", 0.0f, 0.0f}};
            }
            else {
                
                error("%s: wrong argument type", objName);
                return 0;
            }
            break;
        }
        default:
        {
            if (argv[0].a_type == A_SYMBOL) {
                
                elem.varName.name = argv[0].a_w.w_symbol->s_name;
                getVarNameLocalized(elem.varName);
                getVarNameIndexed(elem.varName);
                getTextLinesFromArgs(argc-1, argv+1, elem.textLines);
            }
            else {
                
                error("%s: wrong argument type", objName);
                return 0;
            }
            break;
        }
    }
    return 1;
}

void getTextLinePosition(const shared_ptr<ofTrueTypeFont> font,
                         const t_ofeliaTextModeElem &textMode,
                         vector<TextLineElem> &textLines)
{
    const int size = static_cast<int>(textLines.size());
    const float stringHeight = font->stringHeight("I");
    const float lineHeight = font->getLineHeight();
    const float linesHeight = lineHeight * (size - 1) + stringHeight;
    
    for (int i=0; i<size; ++i) {
        
        const ofRectangle &rect = font->getStringBoundingBox(textLines[i].text.c_str(), 0.0f, 0.0f);
        const float baseY = i * lineHeight;
        
        switch (textMode.horMode) {
                
            case TEXT_ALIGN_LEFT:
                textLines[i].posX = -rect.x;
                break;
                
            case TEXT_ALIGN_CENTER:
                textLines[i].posX = -rect.x - rect.width * 0.5f;
                break;
                
            case TEXT_ALIGN_RIGHT:
                textLines[i].posX = -rect.x - rect.width;
                break;
                
            default:
                break;
        }
        switch (textMode.verMode) {
                
            case TEXT_ALIGN_TOP:
                textLines[i].posY = baseY + stringHeight;
                break;
                
            case TEXT_ALIGN_MIDDLE:
                textLines[i].posY = baseY + stringHeight - linesHeight * 0.5f;
                break;
                
            case TEXT_ALIGN_BOTTOM:
                textLines[i].posY = baseY + stringHeight - linesHeight;
                break;
                
            default:
                break;
        }
    }
}

/* ________________________________________________________________________________
 * ofDrawText object methods
 */
void *ofeliaDrawText_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaDrawText *x = reinterpret_cast<t_ofeliaDrawText*>(pd_new(ofeliaDrawText_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    getVarNameLocalPrefixes(x->elem.varName);

    if (!getDrawTextElemFromArgs(argc, argv, x->elem, t_ofeliaDrawText::objName))
        return 0;
    x->fontElem = {"", 0, 0};
    x->shouldUpdateTextLines = true;
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaDrawText_bang(t_ofeliaDrawText *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const t_string &name = x->elem.varName.name;
        
        if (!name.empty()) {
            
            const int pos = getPositionByFontName(name);
            
            if (pos != -1 && !t_ofeliaLoadFont::fontData[pos].elems.empty()) {
                
                const int index = min(x->elem.varName.index, static_cast<int>(t_ofeliaLoadFont::fontData[pos].elems.size())-1);
                
                if (!t_ofeliaLoadFont::fonts[pos].empty() &&
                    index < static_cast<int>(t_ofeliaLoadFont::fonts[pos].size())) {

                    const t_ofeliaTextModeElem &textMode = t_ofeliaSetTextMode::textModeVec.back();
                
                    if (x->textMode.horMode != textMode.horMode ||
                        x->textMode.verMode != textMode.verMode) {
                        
                        x->shouldUpdateTextLines = true;
                        x->textMode = textMode;
                    }
                    const t_ofeliaLoadFontElem &fontElem = t_ofeliaLoadFont::fontData[pos].elems[index];
                    
                    if (x->fontElem.fontSize != fontElem.fontSize ||
                        x->fontElem.path != fontElem.path ||
                        x->fontElem.fontEditID != fontElem.fontEditID) {
                        
                        x->shouldUpdateTextLines = true;
                        x->fontElem = fontElem;
                    }
                    if (x->shouldUpdateTextLines) {
                        
                        if (t_ofeliaLoadFont::fonts[pos][index]->isLoaded()) {
                            
                            getTextLinePosition(t_ofeliaLoadFont::fonts[pos][index],
                                                x->textMode, x->elem.textLines);
                            x->shouldUpdateTextLines = false;
                        }
                    }
                    ofGetCurrentRenderer()->pushMatrix();
                    const float scaleAmt = 1.0f / ofeliaWindow::retinaScale;
                    ofGetCurrentRenderer()->scale(scaleAmt, scaleAmt, 1.0f);
                    
                    for (size_t i=0; i<x->elem.textLines.size(); ++i) {
                        
                        t_ofeliaLoadFont::fonts[pos][index]->drawString(x->elem.textLines[i].text.c_str(),
                                                                        x->elem.textLines[i].posX,
                                                                        x->elem.textLines[i].posY);
                    }
                    ofGetCurrentRenderer()->popMatrix();
                }
            }
        }
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaDrawText::objName);
    }
}

void ofeliaDrawText_float(t_ofeliaDrawText *x, t_floatarg f)
{
    x->elem.varName.index = max(0, static_cast<int>(f));
}

void ofeliaDrawText_name(t_ofeliaDrawText *x, t_symbol *s)
{
    x->elem.varName.name = s->s_name;
    getVarNameLocalized(x->elem.varName);
    getVarNameIndexed(x->elem.varName);
    x->shouldUpdateTextLines = true;
}

void ofeliaDrawText_text(t_ofeliaDrawText *x, t_symbol *s, int argc, t_atom *argv)
{
    getTextLinesFromArgs(argc, argv, x->elem.textLines);
    x->shouldUpdateTextLines = true;
}

void ofeliaDrawText_set(t_ofeliaDrawText *x, t_symbol *s, int argc, t_atom *argv)
{
    getDrawTextElemFromArgs(argc, argv, x->elem, t_ofeliaDrawText::objName);
    x->shouldUpdateTextLines = true;
}

void ofeliaDrawText_print(t_ofeliaDrawText *x)
{
    post("\n[%s]", t_ofeliaDrawText::objName);
    post("name : %s[%d]", x->elem.varName.name.c_str(), x->elem.varName.index);
    post("text : %s", x->elem.textLines[0].text.c_str());
    
    for (size_t i=1; i<x->elem.textLines.size(); ++i)
        post("       %s", x->elem.textLines[i].text.c_str());
}

void ofeliaDrawText_setup()
{
    ofeliaDrawText_class = class_new(gensym("ofDrawText"),
                                     reinterpret_cast<t_newmethod>(ofeliaDrawText_new),
                                     0, sizeof(t_ofeliaDrawText),
                                     CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaDrawText_class, reinterpret_cast<t_method>(ofeliaDrawText_bang));
    class_addfloat(ofeliaDrawText_class, reinterpret_cast<t_method>(ofeliaDrawText_float));
    class_addmethod(ofeliaDrawText_class, reinterpret_cast<t_method>(ofeliaDrawText_name),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaDrawText_class, reinterpret_cast<t_method>(ofeliaDrawText_text),
                    gensym("text"), A_GIMME, 0);
    class_addmethod(ofeliaDrawText_class, reinterpret_cast<t_method>(ofeliaDrawText_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaDrawText_class, reinterpret_cast<t_method>(ofeliaDrawText_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofDrawTextAsShapes object methods
 */
bool getDrawTextAsShapesElemFromArgs(int argc, t_atom *argv, t_ofeliaDrawTextAsShapesElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.varName.name = "";
            elem.size = 0.0f;
            elem.textLines = {{"", 0.0f, 0.0f}};
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_SYMBOL) {
                
                elem.varName.name = argv[0].a_w.w_symbol->s_name;
                getVarNameLocalized(elem.varName);
                getVarNameIndexed(elem.varName);
                elem.size = 0.0f;
                elem.textLines = {{"", 0.0f, 0.0f}};
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaDrawTextAsShapes::objName);
                return 0;
            }
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_SYMBOL &&
                argv[1].a_type == A_FLOAT) {
                
                elem.varName.name = argv[0].a_w.w_symbol->s_name;
                getVarNameLocalized(elem.varName);
                getVarNameIndexed(elem.varName);
                elem.size = argv[1].a_w.w_float;
                elem.textLines = {{"", 0.0f, 0.0f}};
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaDrawTextAsShapes::objName);
                return 0;
            }
            break;
        }
        default:
        {
            if (argv[0].a_type == A_SYMBOL &&
                argv[1].a_type == A_FLOAT) {
                
                elem.varName.name = argv[0].a_w.w_symbol->s_name;
                getVarNameLocalized(elem.varName);
                getVarNameIndexed(elem.varName);
                elem.size = argv[1].a_w.w_float;
                getTextLinesFromArgs(argc-2, argv+2, elem.textLines);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaDrawTextAsShapes::objName);
                return 0;
            }
            break;
        }
    }
    return 1;
}

void *ofeliaDrawTextAsShapes_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaDrawTextAsShapes *x = reinterpret_cast<t_ofeliaDrawTextAsShapes*>(pd_new(ofeliaDrawTextAsShapes_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    getVarNameLocalPrefixes(x->elem.varName);
    
    if (!getDrawTextAsShapesElemFromArgs(argc, argv, x->elem))
        return 0;
    x->fontElem = {"", 0, 0};
    x->shouldUpdateTextLines = true;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("size"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaDrawTextAsShapes_bang(t_ofeliaDrawTextAsShapes *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const t_string &name = x->elem.varName.name;
        
        if (!name.empty()) {
            
            const int pos = getPositionByFontName(name);
            
            if (pos != -1 && !t_ofeliaLoadFont::fontData[pos].elems.empty()) {
                
                const int index = min(x->elem.varName.index, static_cast<int>(t_ofeliaLoadFont::fontData[pos].elems.size())-1);
                
                if (!t_ofeliaLoadFont::fonts[pos].empty() &&
                    index < static_cast<int>(t_ofeliaLoadFont::fonts[pos].size())) {
                    
                    const t_ofeliaTextModeElem &textMode = t_ofeliaSetTextMode::textModeVec.back();
                    
                    if (x->textMode.horMode != textMode.horMode ||
                        x->textMode.verMode != textMode.verMode) {
                        
                        x->shouldUpdateTextLines = true;
                        x->textMode = textMode;
                    }
                    const t_ofeliaLoadFontElem &fontElem = t_ofeliaLoadFont::fontData[pos].elems[index];
                    
                    if (x->fontElem.fontSize != fontElem.fontSize ||
                        x->fontElem.path != fontElem.path ||
                        x->fontElem.fontEditID != fontElem.fontEditID) {
                        
                        x->shouldUpdateTextLines = true;
                        x->fontElem = fontElem;
                    }
                    if (x->shouldUpdateTextLines) {
                        
                        if (t_ofeliaLoadFont::fonts[pos][index]->isLoaded()) {

                            getTextLinePosition(t_ofeliaLoadFont::fonts[pos][index],
                                                x->textMode, x->elem.textLines);
                            x->shouldUpdateTextLines = false;
                        }
                    }
                    ofGetCurrentRenderer()->pushMatrix();
                    float scaleAmt = 1.0f / ofeliaWindow::retinaScale;
                    
                    if (x->elem.size)
                        scaleAmt *= x->elem.size / (t_ofeliaLoadFont::fonts[pos][index]->getSize() * scaleAmt);
                    ofGetCurrentRenderer()->scale(scaleAmt, scaleAmt, 1.0f);
                    
                    for (size_t i=0; i<x->elem.textLines.size(); ++i)
                        t_ofeliaLoadFont::fonts[pos][index]->drawStringAsShapes(x->elem.textLines[i].text.c_str(), x->elem.textLines[i].posX, x->elem.textLines[i].posY);
                    ofGetCurrentRenderer()->popMatrix();
                }
            }
        }
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaDrawTextAsShapes::objName);
    }
}

void ofeliaDrawTextAsShapes_float(t_ofeliaDrawTextAsShapes *x, t_floatarg f)
{
    x->elem.varName.index = max(0, static_cast<int>(f));
}

void ofeliaDrawTextAsShapes_name(t_ofeliaDrawTextAsShapes *x, t_symbol *s)
{
    x->elem.varName.name = s->s_name;
    getVarNameLocalized(x->elem.varName);
    getVarNameIndexed(x->elem.varName);
    x->shouldUpdateTextLines = true;
}

void ofeliaDrawTextAsShapes_size(t_ofeliaDrawTextAsShapes *x, t_floatarg f)
{
    x->elem.size = f;
    x->shouldUpdateTextLines = true;
}

void ofeliaDrawTextAsShapes_text(t_ofeliaDrawTextAsShapes *x, t_symbol *s, int argc, t_atom *argv)
{
    getTextLinesFromArgs(argc, argv, x->elem.textLines);
    x->shouldUpdateTextLines = true;
}

void ofeliaDrawTextAsShapes_set(t_ofeliaDrawTextAsShapes *x, t_symbol *s, int argc, t_atom *argv)
{
    getDrawTextAsShapesElemFromArgs(argc, argv, x->elem);
    x->shouldUpdateTextLines = true;
}

void ofeliaDrawTextAsShapes_print(t_ofeliaDrawTextAsShapes *x)
{
    post("\n[%s]", t_ofeliaDrawTextAsShapes::objName);
    const t_string &name = x->elem.varName.name;
    float size;
    
    if (!x->elem.size) {
        
        if (!name.empty()) {
            
            const int pos = getPositionByFontName(name);
            
            if (pos != -1) {
                
                const int index = min(x->elem.varName.index, static_cast<int>(t_ofeliaLoadFont::fontData[pos].elems.size())-1);
                const float scaleAmt = 1.0f / ofeliaWindow::retinaScale;
                size = (t_ofeliaLoadFont::fonts[pos][index]->getSize() * scaleAmt);
            }
            else {
                
                size = 0.0f;
            }
        }
        else {
            
            size = 0.0f;
        }
    }
    else {
        
        size = x->elem.size;
    }
    post("name : %s[%d]", name.c_str(), x->elem.varName.index);
    post("size : %g", size);
    post("text : %s", x->elem.textLines[0].text.c_str());
    
    for (size_t i=1; i<x->elem.textLines.size(); ++i)
        post("       %s", x->elem.textLines[i].text.c_str());
}

void ofeliaDrawTextAsShapes_setup()
{
    ofeliaDrawTextAsShapes_class = class_new(gensym("ofDrawTextAsShapes"),
                                             reinterpret_cast<t_newmethod>(ofeliaDrawTextAsShapes_new),
                                             0, sizeof(t_ofeliaDrawTextAsShapes),
                                             CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaDrawTextAsShapes_class, reinterpret_cast<t_method>(ofeliaDrawTextAsShapes_bang));
    class_addfloat(ofeliaDrawTextAsShapes_class, reinterpret_cast<t_method>(ofeliaDrawTextAsShapes_float));
    class_addmethod(ofeliaDrawTextAsShapes_class, reinterpret_cast<t_method>(ofeliaDrawTextAsShapes_name),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaDrawTextAsShapes_class, reinterpret_cast<t_method>(ofeliaDrawTextAsShapes_size),
                    gensym("size"), A_FLOAT, 0);
    class_addmethod(ofeliaDrawTextAsShapes_class, reinterpret_cast<t_method>(ofeliaDrawTextAsShapes_text),
                    gensym("text"), A_GIMME, 0);
    class_addmethod(ofeliaDrawTextAsShapes_class, reinterpret_cast<t_method>(ofeliaDrawTextAsShapes_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaDrawTextAsShapes_class, reinterpret_cast<t_method>(ofeliaDrawTextAsShapes_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofDoesFontNameExist object methods
 */
void *ofeliaDoesFontNameExist_new(t_symbol *s)
{
    t_ofeliaDoesFontNameExist *x = reinterpret_cast<t_ofeliaDoesFontNameExist*>(pd_new(ofeliaDoesFontNameExist_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaDoesFontNameExist_bang(t_ofeliaDoesFontNameExist *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        if (getPositionByFontName(name) != -1)
            outlet_float(x->x_obj.ob_outlet, 1.0f);
        else
            outlet_float(x->x_obj.ob_outlet, 0.0f);
    }
    else {
        
        error("%s: name not assigned", t_ofeliaDoesFontNameExist::objName);
    }
}

void ofeliaDoesFontNameExist_set(t_ofeliaDoesFontNameExist *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaDoesFontNameExist_print(t_ofeliaDoesFontNameExist *x)
{
    post("\n[%s]", t_ofeliaDoesFontNameExist::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaDoesFontNameExist_setup()
{
    ofeliaDoesFontNameExist_class = class_new(gensym("ofDoesFontNameExist"),
                                             reinterpret_cast<t_newmethod>(ofeliaDoesFontNameExist_new),
                                             0, sizeof(t_ofeliaDoesFontNameExist),
                                             CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaDoesFontNameExist_class, reinterpret_cast<t_method>(ofeliaDoesFontNameExist_bang));
    class_addmethod(ofeliaDoesFontNameExist_class, reinterpret_cast<t_method>(ofeliaDoesFontNameExist_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesFontNameExist_class, reinterpret_cast<t_method>(ofeliaDoesFontNameExist_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesFontNameExist_class, reinterpret_cast<t_method>(ofeliaDoesFontNameExist_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetFontPath object methods
 */
void *ofeliaGetFontPath_new(t_symbol *s)
{
    t_ofeliaGetFontPath *x = reinterpret_cast<t_ofeliaGetFontPath*>(pd_new(ofeliaGetFontPath_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_symbol);
    return (x);
}

void ofeliaGetFontPath_bang(t_ofeliaGetFontPath *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByFontName(name);

        if (pos != -1) {
            
            if (!t_ofeliaLoadFont::fontData[pos].elems.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadFont::fontData[pos].elems.size())-1);
                outlet_symbol(x->x_obj.ob_outlet, gensym(t_ofeliaLoadFont::fontData[pos].elems[index].path.c_str()));
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetFontPath::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetFontPath::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetFontPath::objName);
    }
}

void ofeliaGetFontPath_float(t_ofeliaGetFontPath *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetFontPath_set(t_ofeliaGetFontPath *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetFontPath_print(t_ofeliaGetFontPath *x)
{
    post("\n[%s]", t_ofeliaGetFontPath::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetFontPath_setup()
{
    ofeliaGetFontPath_class = class_new(gensym("ofGetFontPath"),
                                        reinterpret_cast<t_newmethod>(ofeliaGetFontPath_new),
                                        0, sizeof(t_ofeliaGetFontPath),
                                        CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetFontPath_class, reinterpret_cast<t_method>(ofeliaGetFontPath_bang));
    class_addfloat(ofeliaGetFontPath_class, reinterpret_cast<t_method>(ofeliaGetFontPath_float));
    class_addmethod(ofeliaGetFontPath_class, reinterpret_cast<t_method>(ofeliaGetFontPath_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetFontPath_class, reinterpret_cast<t_method>(ofeliaGetFontPath_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetFontPath_class, reinterpret_cast<t_method>(ofeliaGetFontPath_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetFontSize object methods
 */
void *ofeliaGetFontSize_new(t_symbol *s)
{
    t_ofeliaGetFontSize *x = reinterpret_cast<t_ofeliaGetFontSize*>(pd_new(ofeliaGetFontSize_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetFontSize_bang(t_ofeliaGetFontSize *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByFontName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadFont::fontData[pos].elems.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadFont::fontData[pos].elems.size())-1);
                outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadFont::fontData[pos].elems[index].fontSize));
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetFontSize::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetFontSize::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetFontSize::objName);
    }
}

void ofeliaGetFontSize_float(t_ofeliaGetFontSize *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetFontSize_set(t_ofeliaGetFontSize *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetFontSize_print(t_ofeliaGetFontSize *x)
{
    post("\n[%s]", t_ofeliaGetFontSize::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetFontSize_setup()
{
    ofeliaGetFontSize_class = class_new(gensym("ofGetFontSize"),
                                        reinterpret_cast<t_newmethod>(ofeliaGetFontSize_new),
                                        0, sizeof(t_ofeliaGetFontSize),
                                        CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetFontSize_class, reinterpret_cast<t_method>(ofeliaGetFontSize_bang));
    class_addfloat(ofeliaGetFontSize_class, reinterpret_cast<t_method>(ofeliaGetFontSize_float));
    class_addmethod(ofeliaGetFontSize_class, reinterpret_cast<t_method>(ofeliaGetFontSize_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetFontSize_class, reinterpret_cast<t_method>(ofeliaGetFontSize_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetFontSize_class, reinterpret_cast<t_method>(ofeliaGetFontSize_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofIsFontLoaded object methods
 */
void *ofeliaIsFontLoaded_new(t_symbol *s)
{
    t_ofeliaIsFontLoaded *x = reinterpret_cast<t_ofeliaIsFontLoaded*>(pd_new(ofeliaIsFontLoaded_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaIsFontLoaded_bang(t_ofeliaIsFontLoaded *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByFontName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadFont::fontData[pos].elems.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadFont::fontData[pos].elems.size())-1);
                
                if (!t_ofeliaLoadFont::fonts[pos].empty() &&
                    index < static_cast<int>(t_ofeliaLoadFont::fonts[pos].size())) {
                    
                    if (t_ofeliaLoadFont::fonts[pos][index]->isLoaded())
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
            
            error("%s: failed to find '%s'", t_ofeliaIsFontLoaded::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaIsFontLoaded::objName);
    }
}

void ofeliaIsFontLoaded_float(t_ofeliaIsFontLoaded *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaIsFontLoaded_set(t_ofeliaIsFontLoaded *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaIsFontLoaded_print(t_ofeliaIsFontLoaded *x)
{
    post("\n[%s]", t_ofeliaIsFontLoaded::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaIsFontLoaded_setup()
{
    ofeliaIsFontLoaded_class = class_new(gensym("ofIsFontLoaded"),
                                          reinterpret_cast<t_newmethod>(ofeliaIsFontLoaded_new),
                                          0, sizeof(t_ofeliaIsFontLoaded),
                                          CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaIsFontLoaded_class, reinterpret_cast<t_method>(ofeliaIsFontLoaded_bang));
    class_addfloat(ofeliaIsFontLoaded_class, reinterpret_cast<t_method>(ofeliaIsFontLoaded_float));
    class_addmethod(ofeliaIsFontLoaded_class, reinterpret_cast<t_method>(ofeliaIsFontLoaded_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaIsFontLoaded_class, reinterpret_cast<t_method>(ofeliaIsFontLoaded_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaIsFontLoaded_class, reinterpret_cast<t_method>(ofeliaIsFontLoaded_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetFontLetterSpacing object methods
 */
void *ofeliaGetFontLetterSpacing_new(t_symbol *s)
{
    t_ofeliaGetFontLetterSpacing *x = reinterpret_cast<t_ofeliaGetFontLetterSpacing*>(pd_new(ofeliaGetFontLetterSpacing_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetFontLetterSpacing_bang(t_ofeliaGetFontLetterSpacing *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByFontName(name);

        if (pos != -1) {
            
            if (!t_ofeliaLoadFont::fontData[pos].elems.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadFont::fontData[pos].elems.size())-1);
                
                if (!t_ofeliaLoadFont::fonts[pos].empty() &&
                    index < static_cast<int>(t_ofeliaLoadFont::fonts[pos].size())) {
                    
                    if (t_ofeliaLoadFont::fonts[pos][index]->isLoaded()) {
                        
                        outlet_float(x->x_obj.ob_outlet, t_ofeliaLoadFont::fonts[pos][index]->getLetterSpacing());
                    }
                    else {
                        
                        error("%s: '%s' is not loaded", t_ofeliaGetFontLetterSpacing::objName, name.c_str());
                    }
                }
                else {
                    
                    error("%s: '%s' is not loaded", t_ofeliaGetFontLetterSpacing::objName, name.c_str());
                }
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetFontLetterSpacing::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetFontLetterSpacing::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetFontLetterSpacing::objName);
    }
}

void ofeliaGetFontLetterSpacing_float(t_ofeliaGetFontLetterSpacing *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetFontLetterSpacing_set(t_ofeliaGetFontLetterSpacing *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetFontLetterSpacing_print(t_ofeliaGetFontLetterSpacing *x)
{
    post("\n[%s]", t_ofeliaGetFontLetterSpacing::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetFontLetterSpacing_setup()
{
    ofeliaGetFontLetterSpacing_class = class_new(gensym("ofGetFontLetterSpacing"),
                                                 reinterpret_cast<t_newmethod>(ofeliaGetFontLetterSpacing_new),
                                                 0, sizeof(t_ofeliaGetFontLetterSpacing),
                                                 CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetFontLetterSpacing_class, reinterpret_cast<t_method>(ofeliaGetFontLetterSpacing_bang));
    class_addfloat(ofeliaGetFontLetterSpacing_class, reinterpret_cast<t_method>(ofeliaGetFontLetterSpacing_float));
    class_addmethod(ofeliaGetFontLetterSpacing_class, reinterpret_cast<t_method>(ofeliaGetFontLetterSpacing_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetFontLetterSpacing_class, reinterpret_cast<t_method>(ofeliaGetFontLetterSpacing_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetFontLetterSpacing_class, reinterpret_cast<t_method>(ofeliaGetFontLetterSpacing_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetFontLineHeight object methods
 */
void *ofeliaGetFontLineHeight_new(t_symbol *s)
{
    t_ofeliaGetFontLineHeight *x = reinterpret_cast<t_ofeliaGetFontLineHeight*>(pd_new(ofeliaGetFontLineHeight_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetFontLineHeight_bang(t_ofeliaGetFontLineHeight *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByFontName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadFont::fontData[pos].elems.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadFont::fontData[pos].elems.size())-1);
                
                if (!t_ofeliaLoadFont::fonts[pos].empty() &&
                    index < static_cast<int>(t_ofeliaLoadFont::fonts[pos].size())) {
                    
                    if (t_ofeliaLoadFont::fonts[pos][index]->isLoaded()) {
                        
                        const float scaleAmt = 1.0f / ofeliaWindow::retinaScale;
                        outlet_float(x->x_obj.ob_outlet, t_ofeliaLoadFont::fonts[pos][index]->getLineHeight() * scaleAmt);
                    }
                    else {
                        
                        error("%s: '%s' is not loaded", t_ofeliaGetFontLineHeight::objName, name.c_str());
                    }
                }
                else {
                    
                    error("%s: '%s' is not loaded", t_ofeliaGetFontLineHeight::objName, name.c_str());
                }
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetFontLineHeight::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetFontLineHeight::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetFontLineHeight::objName);
    }
}

void ofeliaGetFontLineHeight_float(t_ofeliaGetFontLineHeight *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetFontLineHeight_set(t_ofeliaGetFontLineHeight *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetFontLineHeight_print(t_ofeliaGetFontLineHeight *x)
{
    post("\n[%s]", t_ofeliaGetFontLineHeight::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetFontLineHeight_setup()
{
    ofeliaGetFontLineHeight_class = class_new(gensym("ofGetFontLineHeight"),
                                              reinterpret_cast<t_newmethod>(ofeliaGetFontLineHeight_new),
                                              0, sizeof(t_ofeliaGetFontLineHeight),
                                              CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetFontLineHeight_class, reinterpret_cast<t_method>(ofeliaGetFontLineHeight_bang));
    class_addfloat(ofeliaGetFontLineHeight_class, reinterpret_cast<t_method>(ofeliaGetFontLineHeight_float));
    class_addmethod(ofeliaGetFontLineHeight_class, reinterpret_cast<t_method>(ofeliaGetFontLineHeight_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetFontLineHeight_class, reinterpret_cast<t_method>(ofeliaGetFontLineHeight_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetFontLineHeight_class, reinterpret_cast<t_method>(ofeliaGetFontLineHeight_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetFontSpaceSize object methods
 */
void *ofeliaGetFontSpaceSize_new(t_symbol *s)
{
    t_ofeliaGetFontSpaceSize *x = reinterpret_cast<t_ofeliaGetFontSpaceSize*>(pd_new(ofeliaGetFontSpaceSize_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetFontSpaceSize_bang(t_ofeliaGetFontSpaceSize *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByFontName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadFont::fontData[pos].elems.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadFont::fontData[pos].elems.size())-1);
                
                if (!t_ofeliaLoadFont::fonts[pos].empty() &&
                    index < static_cast<int>(t_ofeliaLoadFont::fonts[pos].size())) {
                    
                    if (t_ofeliaLoadFont::fonts[pos][index]->isLoaded()) {
                        
                        /* OFELIANOTE : getSpaceSize() doesn't work before manual setting (bug) */
                        outlet_float(x->x_obj.ob_outlet, t_ofeliaLoadFont::fonts[pos][index]->getSpaceSize());
                    }
                    else {
                        
                        error("%s: '%s' is not loaded", t_ofeliaGetFontSpaceSize::objName, name.c_str());
                    }
                }
                else {
                    
                    error("%s: '%s' is not loaded", t_ofeliaGetFontSpaceSize::objName, name.c_str());
                }
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetFontSpaceSize::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetFontSpaceSize::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetFontSpaceSize::objName);
    }
}

void ofeliaGetFontSpaceSize_float(t_ofeliaGetFontSpaceSize *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetFontSpaceSize_set(t_ofeliaGetFontSpaceSize *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetFontSpaceSize_print(t_ofeliaGetFontSpaceSize *x)
{
    post("\n[%s]", t_ofeliaGetFontSpaceSize::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetFontSpaceSize_setup()
{
    ofeliaGetFontSpaceSize_class = class_new(gensym("ofGetFontSpaceSize"),
                                             reinterpret_cast<t_newmethod>(ofeliaGetFontSpaceSize_new),
                                             0, sizeof(t_ofeliaGetFontSpaceSize),
                                             CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetFontSpaceSize_class, reinterpret_cast<t_method>(ofeliaGetFontSpaceSize_bang));
    class_addfloat(ofeliaGetFontSpaceSize_class, reinterpret_cast<t_method>(ofeliaGetFontSpaceSize_float));
    class_addmethod(ofeliaGetFontSpaceSize_class, reinterpret_cast<t_method>(ofeliaGetFontSpaceSize_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetFontSpaceSize_class, reinterpret_cast<t_method>(ofeliaGetFontSpaceSize_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetFontSpaceSize_class, reinterpret_cast<t_method>(ofeliaGetFontSpaceSize_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetTextBoundingBox object methods
 */
void *ofeliaGetTextBoundingBox_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaGetTextBoundingBox *x = reinterpret_cast<t_ofeliaGetTextBoundingBox*>(pd_new(ofeliaGetTextBoundingBox_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    getVarNameLocalPrefixes(x->elem.varName);
    
    if (!getDrawTextElemFromArgs(argc, argv, x->elem, t_ofeliaGetTextBoundingBox::objName))
        return 0;
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetTextBoundingBox_bang(t_ofeliaGetTextBoundingBox *x)
{
    const t_string &name = x->elem.varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByFontName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadFont::fontData[pos].elems.empty()) {
                
                const int index = min(x->elem.varName.index, static_cast<int>(t_ofeliaLoadFont::fontData[pos].elems.size())-1);
                
                if (!t_ofeliaLoadFont::fonts[pos].empty() &&
                    index < static_cast<int>(t_ofeliaLoadFont::fonts[pos].size())) {
                    
                    if (t_ofeliaLoadFont::fonts[pos][index]->isLoaded()) {
                        
                        float boxWidth = 0.0f;
                        
                        for (auto &v : x->elem.textLines) {
                            
                            const float lineWidth = t_ofeliaLoadFont::fonts[pos][index]->stringWidth(v.text.c_str());
                            
                            if (lineWidth > boxWidth)
                                boxWidth = lineWidth;
                        }
                        const float stringHeight = t_ofeliaLoadFont::fonts[pos][index]->stringHeight("I");
                        const float lineHeight = t_ofeliaLoadFont::fonts[pos][index]->getLineHeight();
                        const float boxHeight = lineHeight * (x->elem.textLines.size() - 1) + stringHeight;
                        const float scaleAmt = 1.0f / ofeliaWindow::retinaScale;
                        t_atom av[2];
                        av[0].a_type = A_FLOAT;
                        av[0].a_w.w_float = boxWidth * scaleAmt;
                        av[1].a_type = A_FLOAT;
                        av[1].a_w.w_float = boxHeight * scaleAmt;
                        outlet_list(x->x_obj.ob_outlet, &s_list, 2, av);
                    }
                    else {
                        
                        error("%s: '%s' is not loaded", t_ofeliaGetTextBoundingBox::objName, name.c_str());
                    }
                }
                else {
                    
                    error("%s: '%s' is not loaded", t_ofeliaGetTextBoundingBox::objName, name.c_str());
                }
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetTextBoundingBox::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetTextBoundingBox::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetTextBoundingBox::objName);
    }
}

void ofeliaGetTextBoundingBox_float(t_ofeliaGetTextBoundingBox *x, t_floatarg f)
{
    x->elem.varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetTextBoundingBox_name(t_ofeliaGetTextBoundingBox *x, t_symbol *s)
{
    x->elem.varName.name = s->s_name;
    getVarNameLocalized(x->elem.varName);
    getVarNameIndexed(x->elem.varName);
}

void ofeliaGetTextBoundingBox_set(t_ofeliaGetTextBoundingBox *x, t_symbol *s, int argc, t_atom *argv)
{
    getDrawTextElemFromArgs(argc, argv, x->elem, t_ofeliaGetTextBoundingBox::objName);
}

void ofeliaGetTextBoundingBox_text(t_ofeliaGetTextBoundingBox *x, t_symbol *s, int argc, t_atom *argv)
{
    getTextLinesFromArgs(argc, argv, x->elem.textLines);
}

void ofeliaGetTextBoundingBox_print(t_ofeliaGetTextBoundingBox *x)
{
    post("\n[%s]", t_ofeliaGetTextBoundingBox::objName);
    post("name : %s[%d]", x->elem.varName.name.c_str(), x->elem.varName.index);
    post("text : %s", x->elem.textLines[0].text.c_str());
    
    for (size_t i=1; i<x->elem.textLines.size(); ++i)
        post("       %s", x->elem.textLines[i].text.c_str());
}

void ofeliaGetTextBoundingBox_setup()
{
    ofeliaGetTextBoundingBox_class = class_new(gensym("ofGetTextBoundingBox"),
                                               reinterpret_cast<t_newmethod>(ofeliaGetTextBoundingBox_new),
                                               0, sizeof(t_ofeliaGetTextBoundingBox),
                                               CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaGetTextBoundingBox_class, reinterpret_cast<t_method>(ofeliaGetTextBoundingBox_bang));
    class_addfloat(ofeliaGetTextBoundingBox_class, reinterpret_cast<t_method>(ofeliaGetTextBoundingBox_float));
    class_addmethod(ofeliaGetTextBoundingBox_class, reinterpret_cast<t_method>(ofeliaGetTextBoundingBox_name),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetTextBoundingBox_class, reinterpret_cast<t_method>(ofeliaGetTextBoundingBox_text),
                    gensym("text"), A_GIMME, 0);
    class_addmethod(ofeliaGetTextBoundingBox_class, reinterpret_cast<t_method>(ofeliaGetTextBoundingBox_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaGetTextBoundingBox_class, reinterpret_cast<t_method>(ofeliaGetTextBoundingBox_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetTextMesh2dCommands object methods
 */
void *ofeliaGetTextMesh2dCommands_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaGetTextMesh2dCommands *x = reinterpret_cast<t_ofeliaGetTextMesh2dCommands*>(pd_new(ofeliaGetTextMesh2dCommands_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    getVarNameLocalPrefixes(x->elem.varName);
    
    if (!getDrawTextElemFromArgs(argc, argv, x->elem, t_ofeliaGetTextMesh2dCommands::objName))
        return 0;
    x->textMode = {TEXT_ALIGN_LEFT, TEXT_ALIGN_TOP};
    outlet_new(&x->x_obj, &s_anything);
    return (x);
}

void ofeliaGetTextMesh2dCommands_bang(t_ofeliaGetTextMesh2dCommands *x)
{
    const t_string &name = x->elem.varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByFontName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadFont::fontData[pos].elems.empty()) {
                
                const int index = min(x->elem.varName.index, static_cast<int>(t_ofeliaLoadFont::fontData[pos].elems.size())-1);
                
                if (!t_ofeliaLoadFont::fonts[pos].empty() &&
                    index < static_cast<int>(t_ofeliaLoadFont::fonts[pos].size())) {
                    
                    if (t_ofeliaLoadFont::fonts[pos][index]->isLoaded()) {
                        
                        getTextLinePosition(t_ofeliaLoadFont::fonts[pos][index],
                                            x->textMode, x->elem.textLines);
                        ofMesh mesh;
                        
                        for (size_t i=0; i<x->elem.textLines.size(); ++i) {
                            
                            mesh.append(t_ofeliaLoadFont::fonts[pos][index]->
                                        getStringMesh(x->elem.textLines[i].text.c_str(),
                                                      x->elem.textLines[i].posX,
                                                      x->elem.textLines[i].posY));
                        }
                        vector<t_ofeliaAtomElem> elems;
                        const float scaleAmt = 1.0f / ofeliaWindow::retinaScale;
                        const size_t numVertices = mesh.getNumVertices();
                        const size_t numIndices = mesh.getNumIndices();
                        const size_t numTexCoords = mesh.getNumTexCoords();
                        
                        if (numVertices) {
                            
                            elems.push_back({ATOM_ELEM_TYPE_SYMBOL, 0.0f, "vertex"});
                            
                            for (size_t i=0; i<numVertices; ++i) {
                                
                                const ofVec2f &vertex = mesh.getVertex(i);
                                elems.push_back({ATOM_ELEM_TYPE_FLOAT, vertex.x * scaleAmt});
                                elems.push_back({ATOM_ELEM_TYPE_FLOAT, vertex.y * scaleAmt});
                            }
                        }
                        if (numIndices) {
                            
                            elems.push_back({ATOM_ELEM_TYPE_SYMBOL, 0.0f, "index"});
                            
                            for (size_t i=0; i<numIndices; ++i) {
                                
                                const ofIndexType index = mesh.getIndex(i);
                                elems.push_back({ATOM_ELEM_TYPE_FLOAT, static_cast<float>(index)});
                            }
                        }
                        if (numTexCoords) {
                            
                            elems.push_back({ATOM_ELEM_TYPE_SYMBOL, 0.0f, "texCoord"});
                            
                            for (size_t i=0; i<numTexCoords; ++i) {
                                
                                const ofVec2f &texCoord = mesh.getTexCoord(i);
                                elems.push_back({ATOM_ELEM_TYPE_FLOAT, texCoord.x});
                                elems.push_back({ATOM_ELEM_TYPE_FLOAT, texCoord.y});
                            }
                        }
                        const int ac = static_cast<int>(elems.size());
                        t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
                        
                        for (int i=0; i<ac; ++i) {
                            
                            if (elems[i].type == ATOM_ELEM_TYPE_FLOAT) {
                                
                                av[i].a_type = A_FLOAT;
                                av[i].a_w.w_float = elems[i].value;
                            }
                            else if (elems[i].type == ATOM_ELEM_TYPE_SYMBOL) {
                                
                                av[i].a_type = A_SYMBOL;
                                av[i].a_w.w_symbol = gensym(elems[i].name.c_str());
                            }
                        }
                        outlet_anything(x->x_obj.ob_outlet, av[0].a_w.w_symbol, ac-1, av+1);
                        freebytes(av, ac * sizeof(t_atom));
                    }
                    else {
                        
                        error("%s: '%s' is not loaded", t_ofeliaGetTextMesh2dCommands::objName, name.c_str());
                    }
                }
                else {
                    
                    error("%s: '%s' is not loaded", t_ofeliaGetTextMesh2dCommands::objName, name.c_str());
                }
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetTextMesh2dCommands::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetTextMesh2dCommands::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetTextMesh2dCommands::objName);
    }
}

void ofeliaGetTextMesh2dCommands_float(t_ofeliaGetTextMesh2dCommands *x, t_floatarg f)
{
    x->elem.varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetTextMesh2dCommands_name(t_ofeliaGetTextMesh2dCommands *x, t_symbol *s)
{
    x->elem.varName.name = s->s_name;
    getVarNameLocalized(x->elem.varName);
    getVarNameIndexed(x->elem.varName);
}

void ofeliaGetTextMesh2dCommands_text(t_ofeliaGetTextMesh2dCommands *x, t_symbol *s, int argc, t_atom *argv)
{
    getTextLinesFromArgs(argc, argv, x->elem.textLines);
}

void ofeliaGetTextMesh2dCommands_textMode(t_ofeliaGetTextMesh2dCommands *x, t_symbol *s, int argc, t_atom *argv)
{
    getTextModeFromArgs(argc, argv, x->textMode, t_ofeliaGetTextMesh2dCommands::objName);
}

void ofeliaGetTextMesh2dCommands_set(t_ofeliaGetTextMesh2dCommands *x, t_symbol *s, int argc, t_atom *argv)
{
    getDrawTextElemFromArgs(argc, argv, x->elem, t_ofeliaGetTextMesh2dCommands::objName);
}

void ofeliaGetTextMesh2dCommands_print(t_ofeliaGetTextMesh2dCommands *x)
{
    post("\n[%s]", t_ofeliaGetTextMesh2dCommands::objName);
    post("name : %s[%d]", x->elem.varName.name.c_str(), x->elem.varName.index);
    post("text : %s", x->elem.textLines[0].text.c_str());
    
    for (size_t i=1; i<x->elem.textLines.size(); ++i)
        post("       %s", x->elem.textLines[i].text.c_str());
    t_string horMode, verMode;
    
    switch (x->textMode.horMode) {
            
        case TEXT_ALIGN_LEFT:
            horMode = "LEFT";
            break;
        case TEXT_ALIGN_CENTER:
            horMode = "CENTER";
            break;
        case TEXT_ALIGN_RIGHT:
            horMode = "RIGHT";
            break;
        default:
            break;
    }
    switch (x->textMode.verMode) {
            
        case TEXT_ALIGN_TOP:
            verMode = "TOP";
            break;
        case TEXT_ALIGN_MIDDLE:
            verMode = "MIDDLE";
            break;
        case TEXT_ALIGN_BOTTOM:
            verMode = "BOTTOM";
            break;
        default:
            break;
    }
    post("textMode : %s %s", horMode.c_str(), verMode.c_str());
}

void ofeliaGetTextMesh2dCommands_setup()
{
    ofeliaGetTextMesh2dCommands_class = class_new(gensym("ofGetTextMesh2dCommands"),
                                                  reinterpret_cast<t_newmethod>(ofeliaGetTextMesh2dCommands_new),
                                                  0, sizeof(t_ofeliaGetTextMesh2dCommands),
                                                  CLASS_DEFAULT, A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaGetTextMesh2dCommands_new),
                     gensym("ofGetTextMesh2dCmds"), A_GIMME, 0);
    class_addbang(ofeliaGetTextMesh2dCommands_class, reinterpret_cast<t_method>(ofeliaGetTextMesh2dCommands_bang));
    class_addfloat(ofeliaGetTextMesh2dCommands_class, reinterpret_cast<t_method>(ofeliaGetTextMesh2dCommands_float));
    class_addmethod(ofeliaGetTextMesh2dCommands_class, reinterpret_cast<t_method>(ofeliaGetTextMesh2dCommands_name),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetTextMesh2dCommands_class, reinterpret_cast<t_method>(ofeliaGetTextMesh2dCommands_text),
                    gensym("text"), A_GIMME, 0);
    class_addmethod(ofeliaGetTextMesh2dCommands_class, reinterpret_cast<t_method>(ofeliaGetTextMesh2dCommands_textMode),
                    gensym("textMode"), A_GIMME, 0);
    class_addmethod(ofeliaGetTextMesh2dCommands_class, reinterpret_cast<t_method>(ofeliaGetTextMesh2dCommands_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaGetTextMesh2dCommands_class, reinterpret_cast<t_method>(ofeliaGetTextMesh2dCommands_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetTextMesh3dCommands object methods
 */
void *ofeliaGetTextMesh3dCommands_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaGetTextMesh3dCommands *x = reinterpret_cast<t_ofeliaGetTextMesh3dCommands*>(pd_new(ofeliaGetTextMesh3dCommands_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    getVarNameLocalPrefixes(x->elem.varName);
    
    if (!getDrawTextElemFromArgs(argc, argv, x->elem, t_ofeliaGetTextMesh3dCommands::objName))
        return 0;
    x->textMode = {TEXT_ALIGN_LEFT, TEXT_ALIGN_TOP};
    outlet_new(&x->x_obj, &s_anything);
    return (x);
}

void ofeliaGetTextMesh3dCommands_bang(t_ofeliaGetTextMesh3dCommands *x)
{
    const t_string &name = x->elem.varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByFontName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadFont::fontData[pos].elems.empty()) {
                
                const int index = min(x->elem.varName.index, static_cast<int>(t_ofeliaLoadFont::fontData[pos].elems.size())-1);
                
                if (!t_ofeliaLoadFont::fonts[pos].empty() &&
                    index < static_cast<int>(t_ofeliaLoadFont::fonts[pos].size())) {
                    
                    if (t_ofeliaLoadFont::fonts[pos][index]->isLoaded()) {
                        
                        getTextLinePosition(t_ofeliaLoadFont::fonts[pos][index],
                                            x->textMode, x->elem.textLines);
                        ofMesh mesh;
                        
                        for (size_t i=0; i<x->elem.textLines.size(); ++i) {
                            
                            mesh.append(t_ofeliaLoadFont::fonts[pos][index]->
                                        getStringMesh(x->elem.textLines[i].text.c_str(),
                                                      x->elem.textLines[i].posX,
                                                      x->elem.textLines[i].posY));
                        }
                        vector<t_ofeliaAtomElem> elems;
                        const float scaleAmt = 1.0f / ofeliaWindow::retinaScale;
                        const size_t numVertices = mesh.getNumVertices();
                        const size_t numIndices = mesh.getNumIndices();
                        const size_t numTexCoords = mesh.getNumTexCoords();
                        
                        if (numVertices) {
                            
                            elems.push_back({ATOM_ELEM_TYPE_SYMBOL, 0.0f, "vertex"});
                            
                            for (size_t i=0; i<numVertices; ++i) {
                                
                                const ofVec3f &vertex = mesh.getVertex(i);
                                elems.push_back({ATOM_ELEM_TYPE_FLOAT, vertex.x * scaleAmt});
                                elems.push_back({ATOM_ELEM_TYPE_FLOAT, vertex.y * scaleAmt});
                                elems.push_back({ATOM_ELEM_TYPE_FLOAT, vertex.z});
                            }
                        }
                        if (numIndices) {
                            
                            elems.push_back({ATOM_ELEM_TYPE_SYMBOL, 0.0f, "index"});
                            
                            for (size_t i=0; i<numIndices; ++i) {
                                
                                const ofIndexType index = mesh.getIndex(i);
                                elems.push_back({ATOM_ELEM_TYPE_FLOAT, static_cast<float>(index)});
                            }
                        }
                        if (numTexCoords) {
                            
                            elems.push_back({ATOM_ELEM_TYPE_SYMBOL, 0.0f, "texCoord"});
                            
                            for (size_t i=0; i<numTexCoords; ++i) {
                                
                                const ofVec2f &texCoord = mesh.getTexCoord(i);
                                elems.push_back({ATOM_ELEM_TYPE_FLOAT, texCoord.x});
                                elems.push_back({ATOM_ELEM_TYPE_FLOAT, texCoord.y});
                            }
                        }
                        const int ac = static_cast<int>(elems.size());
                        t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
                        
                        for (int i=0; i<ac; ++i) {
                            
                            if (elems[i].type == ATOM_ELEM_TYPE_FLOAT) {
                                
                                av[i].a_type = A_FLOAT;
                                av[i].a_w.w_float = elems[i].value;
                            }
                            else if (elems[i].type == ATOM_ELEM_TYPE_SYMBOL) {
                                
                                av[i].a_type = A_SYMBOL;
                                av[i].a_w.w_symbol = gensym(elems[i].name.c_str());
                            }
                        }
                        outlet_anything(x->x_obj.ob_outlet, av[0].a_w.w_symbol, ac-1, av+1);
                        freebytes(av, ac * sizeof(t_atom));
                    }
                    else {
                        
                        error("%s: '%s' is not loaded", t_ofeliaGetTextMesh3dCommands::objName, name.c_str());
                    }
                }
                else {
                    
                    error("%s: '%s' is not loaded", t_ofeliaGetTextMesh3dCommands::objName, name.c_str());
                }
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetTextMesh3dCommands::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetTextMesh3dCommands::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetTextMesh3dCommands::objName);
    }
}

void ofeliaGetTextMesh3dCommands_float(t_ofeliaGetTextMesh3dCommands *x, t_floatarg f)
{
    x->elem.varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetTextMesh3dCommands_name(t_ofeliaGetTextMesh3dCommands *x, t_symbol *s)
{
    x->elem.varName.name = s->s_name;
    getVarNameLocalized(x->elem.varName);
    getVarNameIndexed(x->elem.varName);
}

void ofeliaGetTextMesh3dCommands_text(t_ofeliaGetTextMesh3dCommands *x, t_symbol *s, int argc, t_atom *argv)
{
    getTextLinesFromArgs(argc, argv, x->elem.textLines);
}

void ofeliaGetTextMesh3dCommands_textMode(t_ofeliaGetTextMesh3dCommands *x, t_symbol *s, int argc, t_atom *argv)
{
    getTextModeFromArgs(argc, argv, x->textMode, t_ofeliaGetTextMesh3dCommands::objName);
}

void ofeliaGetTextMesh3dCommands_set(t_ofeliaGetTextMesh3dCommands *x, t_symbol *s, int argc, t_atom *argv)
{
    getDrawTextElemFromArgs(argc, argv, x->elem, t_ofeliaGetTextMesh3dCommands::objName);
}

void ofeliaGetTextMesh3dCommands_print(t_ofeliaGetTextMesh3dCommands *x)
{
    post("\n[%s]", t_ofeliaGetTextMesh3dCommands::objName);
    post("name : %s[%d]", x->elem.varName.name.c_str(), x->elem.varName.index);
    post("text : %s", x->elem.textLines[0].text.c_str());
    
    for (size_t i=1; i<x->elem.textLines.size(); ++i)
        post("       %s", x->elem.textLines[i].text.c_str());
    t_string horMode, verMode;
    
    switch (x->textMode.horMode) {
            
        case TEXT_ALIGN_LEFT:
            horMode = "LEFT";
            break;
        case TEXT_ALIGN_CENTER:
            horMode = "CENTER";
            break;
        case TEXT_ALIGN_RIGHT:
            horMode = "RIGHT";
            break;
        default:
            break;
    }
    switch (x->textMode.verMode) {
            
        case TEXT_ALIGN_TOP:
            verMode = "TOP";
            break;
        case TEXT_ALIGN_MIDDLE:
            verMode = "MIDDLE";
            break;
        case TEXT_ALIGN_BOTTOM:
            verMode = "BOTTOM";
            break;
        default:
            break;
    }
    post("textMode : %s %s", horMode.c_str(), verMode.c_str());
}

void ofeliaGetTextMesh3dCommands_setup()
{
    ofeliaGetTextMesh3dCommands_class = class_new(gensym("ofGetTextMesh3dCommands"),
                                                  reinterpret_cast<t_newmethod>(ofeliaGetTextMesh3dCommands_new),
                                                  0, sizeof(t_ofeliaGetTextMesh3dCommands),
                                                  CLASS_DEFAULT, A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaGetTextMesh3dCommands_new),
                     gensym("ofGetTextMesh3dCmds"), A_GIMME, 0);
    class_addbang(ofeliaGetTextMesh3dCommands_class, reinterpret_cast<t_method>(ofeliaGetTextMesh3dCommands_bang));
    class_addfloat(ofeliaGetTextMesh3dCommands_class, reinterpret_cast<t_method>(ofeliaGetTextMesh3dCommands_float));
    class_addmethod(ofeliaGetTextMesh3dCommands_class, reinterpret_cast<t_method>(ofeliaGetTextMesh3dCommands_name),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetTextMesh3dCommands_class, reinterpret_cast<t_method>(ofeliaGetTextMesh3dCommands_text),
                    gensym("text"), A_GIMME, 0);
    class_addmethod(ofeliaGetTextMesh3dCommands_class, reinterpret_cast<t_method>(ofeliaGetTextMesh3dCommands_textMode),
                    gensym("textMode"), A_GIMME, 0);
    class_addmethod(ofeliaGetTextMesh3dCommands_class, reinterpret_cast<t_method>(ofeliaGetTextMesh3dCommands_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaGetTextMesh3dCommands_class, reinterpret_cast<t_method>(ofeliaGetTextMesh3dCommands_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * setup methods
 */
void ofeliaFont_setup()
{
    ofeliaLoadFont_setup();
    ofeliaEditFont_setup();
    ofeliaBindFontTex_setup();
    ofeliaDrawText_setup();
    ofeliaDrawTextAsShapes_setup();
    ofeliaDoesFontNameExist_setup();
    ofeliaGetFontPath_setup();
    ofeliaGetFontSize_setup();
    ofeliaIsFontLoaded_setup();
    ofeliaGetFontLetterSpacing_setup();
    ofeliaGetFontLineHeight_setup();
    ofeliaGetFontSpaceSize_setup();
    ofeliaGetTextBoundingBox_setup();
    ofeliaGetTextMesh2dCommands_setup();
    ofeliaGetTextMesh3dCommands_setup();
}

