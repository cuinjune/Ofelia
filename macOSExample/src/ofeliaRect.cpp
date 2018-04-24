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

#include "ofeliaRect.h"

/* ________________________________________________________________________________
 * static variables
 */
const char *t_ofeliaLoadRect::objName = "ofLoadRect";
unsigned int t_ofeliaLoadRect::counter;
vector<t_ofeliaLoadRectData> t_ofeliaLoadRect::rectData;
const char *t_ofeliaEditRect::objName = "ofEditRect";
const char *t_ofeliaDoesRectNameExist::objName = "ofDoesRectNameExist";
const char *t_ofeliaGetRect::objName = "ofGetRect";
const char *t_ofeliaGetRects::objName = "ofGetRects";
const char *t_ofeliaIsPointInsideRect::objName = "ofIsPointInsideRect";
const char *t_ofeliaIsLineInsideRect::objName = "ofIsLineInsideRect";
const char *t_ofeliaIsRectInsideRect::objName = "ofIsRectInsideRect";
const char *t_ofeliaDoesLineIntersectRect::objName = "ofDoesLineIntersectRect";
const char *t_ofeliaDoesRectIntersectRect::objName = "ofDoesRectIntersectRect";
const char *t_ofeliaGetRectCenter::objName = "ofGetRectCenter";
const char *t_ofeliaGetRectArea::objName = "ofGetRectArea";
const char *t_ofeliaGetRectPerimeter::objName = "ofGetRectPerimeter";

/* ________________________________________________________________________________
 * ofLoadRect object methods
 */
void ofeliaLoadRect_vecSizeOut(t_ofeliaLoadRect *x);

int getPositionByRectObjID(const unsigned int objID)
{
    const auto it = find_if(begin(t_ofeliaLoadRect::rectData), end(t_ofeliaLoadRect::rectData),
                            [&objID](const t_ofeliaLoadRectData &rectData)
                            {
                                return rectData.objID == objID;
                            });
    return static_cast<int>(it - t_ofeliaLoadRect::rectData.begin());
}

bool getLoadRectFromArgs(const int argc, const t_atom *argv, ofRectangle &rect)
{
    if (argc) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT &&
            argv[3].a_type == A_FLOAT) {
            
            rect.set(ofRectangle(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float, argv[3].a_w.w_float));
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadRect::objName);
            return 0;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadRect::objName);
        return 0;
    }
    return 1;
}

bool getLoadRectsFromArgs(const int argc, const t_atom *argv, vector<ofRectangle> &rects)
{
    if (argc && argc%4 == 0) {
        
        for (int i=0; i<argc; i+=4) {
            
            if (argv[i].a_type == A_FLOAT &&
                argv[i+1].a_type == A_FLOAT &&
                argv[i+2].a_type == A_FLOAT &&
                argv[i+3].a_type == A_FLOAT) {
                
                rects.push_back(ofRectangle(argv[i].a_w.w_float, argv[i+1].a_w.w_float, argv[i+2].a_w.w_float, argv[i+3].a_w.w_float));
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaLoadRect::objName);
                return 0;
            }
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadRect::objName);
        return 0;
    }
    return 1;
}

bool getLoadRectCmdRangeFromArgs(const int argc, const t_atom *argv, t_ofeliaLoadRectCmdData &cmd)
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
                
                error("%s: wrong argument type", t_ofeliaLoadRect::objName);
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
                
                error("%s: wrong argument type", t_ofeliaLoadRect::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaLoadRect::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaLoadRect_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadRect *x = reinterpret_cast<t_ofeliaLoadRect*>(pd_new(ofeliaLoadRect_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    t_ofeliaLoadRectData rectData;
    getVarNameLocalPrefixes(rectData.varName);
    vector<ofRectangle> rects;
    
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            rectData.varName.name = argv[0].a_w.w_symbol->s_name;
            getVarNameLocalized(rectData.varName);
            
            if (--argc) {
                
                if (!getLoadRectsFromArgs(argc, argv+1, rects))
                    return 0;
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadRect::objName);
            return 0;
        }
    }
    x->objID = rectData.objID = t_ofeliaLoadRect::counter++;
    t_ofeliaLoadRect::rectData.push_back(rectData);
    x->vecSize = 0;
    x->vecSizeOutClock = clock_new(x, reinterpret_cast<t_method>(ofeliaLoadRect_vecSizeOut));
    outlet_new(&x->x_obj, &s_float);
    
    if (argc) {
        
        const int pos = getPositionByRectObjID(x->objID);
        t_ofeliaLoadRect::rectData[pos].rects = rects;
        x->vecSize = static_cast<int>(rects.size());
        clock_delay(x->vecSizeOutClock, 0.0);
    }
    return (x);
}

void ofeliaLoadRect_vecSizeOut(t_ofeliaLoadRect *x)
{
    outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(x->vecSize));
}

void ofeliaLoadRect_name(t_ofeliaLoadRect *x, t_symbol *s)
{
    const int pos = getPositionByRectObjID(x->objID);
    t_ofeliaLoadRect::rectData[pos].varName.name = s->s_name;
    getVarNameLocalized(t_ofeliaLoadRect::rectData[pos].varName);
}

void ofeliaLoadRect_load(t_ofeliaLoadRect *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<ofRectangle> rects;
    
    if (getLoadRectsFromArgs(argc, argv, rects)) {
        
        const int pos = getPositionByRectObjID(x->objID);
        t_ofeliaLoadRect::rectData[pos].rects = rects;
        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(rects.size()));
    }
}

void ofeliaLoadRect_add(t_ofeliaLoadRect *x, t_symbol *s, int argc, t_atom *argv)
{
    ofRectangle rect;
    
    if (getLoadRectFromArgs(argc, argv, rect)) {
        
        const int pos = getPositionByRectObjID(x->objID);
        t_ofeliaLoadRect::rectData[pos].rects.push_back(rect);
        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadRect::rectData[pos].rects.size()));
    }
}

void ofeliaLoadRect_append(t_ofeliaLoadRect *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<ofRectangle> rects;
    
    if (getLoadRectsFromArgs(argc, argv, rects)) {
        
        const int pos = getPositionByRectObjID(x->objID);
        t_ofeliaLoadRect::rectData[pos].rects.insert(t_ofeliaLoadRect::rectData[pos].rects.end(), rects.begin(), rects.end());
        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadRect::rectData[pos].rects.size()));
    }
}

void ofeliaLoadRect_prepend(t_ofeliaLoadRect *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<ofRectangle> rects;
    
    if (getLoadRectsFromArgs(argc, argv, rects)) {
        
        const int pos = getPositionByRectObjID(x->objID);
        t_ofeliaLoadRect::rectData[pos].rects.insert(t_ofeliaLoadRect::rectData[pos].rects.begin(), rects.begin(), rects.end());
        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadRect::rectData[pos].rects.size()));
    }
}

void ofeliaLoadRect_insert(t_ofeliaLoadRect *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadRectCmdData cmd;
    
    if (getLoadRectFromArgs(argc, argv, cmd.rect)) {
        
        if (getLoadRectCmdRangeFromArgs(argc-4, argv+4, cmd)) {
            
            const int pos = getPositionByRectObjID(x->objID);
            const int size = static_cast<int>(t_ofeliaLoadRect::rectData[pos].rects.size());
            const auto rectsBegin = t_ofeliaLoadRect::rectData[pos].rects.begin();
            const int fromIndex = static_cast<int>(ofClamp(cmd.fromIndex, 0.0f, size));
            const int insertSize = static_cast<int>(max(0.0f, max(0.0f, cmd.toIndex) - max(0.0f, cmd.fromIndex))) + 1;
            t_ofeliaLoadRect::rectData[pos].rects.insert(rectsBegin + fromIndex,
                                                            insertSize, cmd.rect);
            outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadRect::rectData[pos].rects.size()));
        }
    }
}

void ofeliaLoadRect_fill(t_ofeliaLoadRect *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadRectCmdData cmd;
    
    if (getLoadRectFromArgs(argc, argv, cmd.rect)) {
        
        if (getLoadRectCmdRangeFromArgs(argc-4, argv+4, cmd)) {
            
            const int pos = getPositionByRectObjID(x->objID);
            const int size = static_cast<int>(t_ofeliaLoadRect::rectData[pos].rects.size());
            const auto rectsBegin = t_ofeliaLoadRect::rectData[pos].rects.begin();
            
            if (size) {
                
                const int maxIndex = size - 1;
                const int fromIndex = static_cast<int>(ofClamp(cmd.fromIndex, 0.0f, maxIndex));
                const int toIndex = static_cast<int>(ofClamp(cmd.toIndex, fromIndex, maxIndex)) + 1;
                fill(rectsBegin + fromIndex, rectsBegin + toIndex, cmd.rect);
                outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadRect::rectData[pos].rects.size()));
            }
            else {
                
                outlet_float(x->x_obj.ob_outlet, 0.0f);
            }
        }
    }
}

void ofeliaLoadRect_erase(t_ofeliaLoadRect *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadRectCmdData cmd;
    
    if (getLoadRectCmdRangeFromArgs(argc, argv, cmd)) {
        
        const int pos = getPositionByRectObjID(x->objID);
        const int size = static_cast<int>(t_ofeliaLoadRect::rectData[pos].rects.size());
        const auto rectsBegin = t_ofeliaLoadRect::rectData[pos].rects.begin();
        
        if (size) {
            
            const int maxIndex = size - 1;
            const int fromIndex = static_cast<int>(ofClamp(cmd.fromIndex, 0.0f, maxIndex));
            const int toIndex = static_cast<int>(ofClamp(cmd.toIndex, fromIndex, maxIndex)) + 1;
            t_ofeliaLoadRect::rectData[pos].rects.erase(rectsBegin + fromIndex,
                                                           rectsBegin + toIndex);
            outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadRect::rectData[pos].rects.size()));
        }
        else {
            
            outlet_float(x->x_obj.ob_outlet, 0.0f);
        }
    }
}

void ofeliaLoadRect_clear(t_ofeliaLoadRect *x)
{
    const int pos = getPositionByRectObjID(x->objID);
    t_ofeliaLoadRect::rectData[pos].rects.clear();
    outlet_float(x->x_obj.ob_outlet, 0.0f);
}

void ofeliaLoadRect_set(t_ofeliaLoadRect *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            ofeliaLoadRect_name(x, argv[0].a_w.w_symbol);
            
            if (--argc)
                ofeliaLoadRect_load(x, 0, argc, argv+1);
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadRect::objName);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadRect::objName);
    }
}

void ofeliaLoadRect_print(t_ofeliaLoadRect *x)
{
    post("\n[%s]", t_ofeliaLoadRect::objName);
    const int pos = getPositionByRectObjID(x->objID);
    post("name : %s", t_ofeliaLoadRect::rectData[pos].varName.name.c_str());
    post("load : ");
    
    for (size_t i=0; i<t_ofeliaLoadRect::rectData[pos].rects.size(); ++i) {
        
        post(" %g %g %g %g", t_ofeliaLoadRect::rectData[pos].rects[i].x, t_ofeliaLoadRect::rectData[pos].rects[i].y, t_ofeliaLoadRect::rectData[pos].rects[i].width, t_ofeliaLoadRect::rectData[pos].rects[i].height);
    }
}

void ofeliaLoadRect_free(t_ofeliaLoadRect *x)
{
    clock_free(x->vecSizeOutClock);
    const int pos = getPositionByRectObjID(x->objID);
    t_ofeliaLoadRect::rectData.erase(t_ofeliaLoadRect::rectData.begin() + pos);
}

void ofeliaLoadRect_setup()
{
    ofeliaLoadRect_class = class_new(gensym("ofLoadRect"),
                                      reinterpret_cast<t_newmethod>(ofeliaLoadRect_new),
                                      reinterpret_cast<t_method>(ofeliaLoadRect_free),
                                      sizeof(t_ofeliaLoadRect),
                                      CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaLoadRect_class, reinterpret_cast<t_method>(ofeliaLoadRect_name),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaLoadRect_class, reinterpret_cast<t_method>(ofeliaLoadRect_load),
                    gensym("load"), A_GIMME, 0);
    class_addmethod(ofeliaLoadRect_class, reinterpret_cast<t_method>(ofeliaLoadRect_add),
                    gensym("add"), A_GIMME, 0);
    class_addmethod(ofeliaLoadRect_class, reinterpret_cast<t_method>(ofeliaLoadRect_append),
                    gensym("append"), A_GIMME, 0);
    class_addmethod(ofeliaLoadRect_class, reinterpret_cast<t_method>(ofeliaLoadRect_prepend),
                    gensym("prepend"), A_GIMME, 0);
    class_addmethod(ofeliaLoadRect_class, reinterpret_cast<t_method>(ofeliaLoadRect_insert),
                    gensym("insert"), A_GIMME, 0);
    class_addmethod(ofeliaLoadRect_class, reinterpret_cast<t_method>(ofeliaLoadRect_fill),
                    gensym("fill"), A_GIMME, 0);
    class_addmethod(ofeliaLoadRect_class, reinterpret_cast<t_method>(ofeliaLoadRect_erase),
                    gensym("erase"), A_GIMME, 0);
    class_addmethod(ofeliaLoadRect_class, reinterpret_cast<t_method>(ofeliaLoadRect_clear),
                    gensym("clear"), A_NULL, 0);
    class_addmethod(ofeliaLoadRect_class, reinterpret_cast<t_method>(ofeliaLoadRect_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaLoadRect_class, reinterpret_cast<t_method>(ofeliaLoadRect_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * common methods
 */
int getPositionByRectName(const t_string &name)
{
    auto it = find_if(begin(t_ofeliaLoadRect::rectData), end(t_ofeliaLoadRect::rectData),
                      [&name] (const t_ofeliaLoadRectData &rectData) {
                          return (rectData.varName.name == name);
                      });
    if (it == end(t_ofeliaLoadRect::rectData))
        return -1;
    return static_cast<int>(it - begin(t_ofeliaLoadRect::rectData));
}

/* ________________________________________________________________________________
 * ofEditRect object methods
 */
void *ofeliaEditRect_new(t_symbol *s)
{
    t_ofeliaEditRect *x = reinterpret_cast<t_ofeliaEditRect*>(pd_new(ofeliaEditRect_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaEditRect_float(t_ofeliaEditRect *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaEditRect_assign(t_ofeliaEditRect *x, t_floatarg posX, t_floatarg posY, t_floatarg width, t_floatarg height)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByRectName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadRect::rectData[pos].rects.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadRect::rectData[pos].rects.size())-1);
                ofRectangle &rect = t_ofeliaLoadRect::rectData[pos].rects[index];
                rect.set(posX, posY, width, height);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditRect::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditRect::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditRect::objName);
    }
}

void ofeliaEditRect_assignFromCenter(t_ofeliaEditRect *x, t_floatarg posX, t_floatarg posY, t_floatarg width, t_floatarg height)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByRectName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadRect::rectData[pos].rects.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadRect::rectData[pos].rects.size())-1);
                ofRectangle &rect = t_ofeliaLoadRect::rectData[pos].rects[index];
                rect.setFromCenter(posX, posY, width, height);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditRect::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditRect::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditRect::objName);
    }
}

void ofeliaEditRect_assignX(t_ofeliaEditRect *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByRectName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadRect::rectData[pos].rects.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadRect::rectData[pos].rects.size())-1);
                ofRectangle &rect = t_ofeliaLoadRect::rectData[pos].rects[index];
                rect.x = f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditRect::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditRect::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditRect::objName);
    }
}

void ofeliaEditRect_assignY(t_ofeliaEditRect *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByRectName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadRect::rectData[pos].rects.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadRect::rectData[pos].rects.size())-1);
                ofRectangle &rect = t_ofeliaLoadRect::rectData[pos].rects[index];
                rect.y = f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditRect::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditRect::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditRect::objName);
    }
}

void ofeliaEditRect_assignWidth(t_ofeliaEditRect *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByRectName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadRect::rectData[pos].rects.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadRect::rectData[pos].rects.size())-1);
                ofRectangle &rect = t_ofeliaLoadRect::rectData[pos].rects[index];
                rect.width = f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditRect::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditRect::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditRect::objName);
    }
}

void ofeliaEditRect_assignHeight(t_ofeliaEditRect *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByRectName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadRect::rectData[pos].rects.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadRect::rectData[pos].rects.size())-1);
                ofRectangle &rect = t_ofeliaLoadRect::rectData[pos].rects[index];
                rect.height = f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditRect::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditRect::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditRect::objName);
    }
}

void ofeliaEditRect_assignPos(t_ofeliaEditRect *x, t_floatarg posX, t_floatarg posY)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByRectName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadRect::rectData[pos].rects.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadRect::rectData[pos].rects.size())-1);
                ofRectangle &rect = t_ofeliaLoadRect::rectData[pos].rects[index];
                rect.setPosition(posX, posY);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditRect::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditRect::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditRect::objName);
    }
}

void ofeliaEditRect_assignDimen(t_ofeliaEditRect *x, t_floatarg width, t_floatarg height)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByRectName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadRect::rectData[pos].rects.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadRect::rectData[pos].rects.size())-1);
                ofRectangle &rect = t_ofeliaLoadRect::rectData[pos].rects[index];
                rect.setWidth(width);
                rect.setHeight(height);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditRect::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditRect::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditRect::objName);
    }
}

void ofeliaEditRect_growToInclude(t_ofeliaEditRect *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByRectName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadRect::rectData[pos].rects.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadRect::rectData[pos].rects.size())-1);
                ofRectangle &rect = t_ofeliaLoadRect::rectData[pos].rects[index];
                
                switch (argc) {
                        
                    case 2:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            rect.growToInclude(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditRect::objName);
                            return;
                        }
                        break;
                    }
                    case 4:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT &&
                            argv[3].a_type == A_FLOAT) {
                            
                            rect.growToInclude(ofRectangle(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float, argv[3].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditRect::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditRect::objName);
                        return;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditRect::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditRect::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditRect::objName);
    }
}

void ofeliaEditRect_scale(t_ofeliaEditRect *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByRectName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadRect::rectData[pos].rects.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadRect::rectData[pos].rects.size())-1);
                ofRectangle &rect = t_ofeliaLoadRect::rectData[pos].rects[index];
                
                switch (argc) {
                        
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            rect.scale(argv[0].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditRect::objName);
                            return;
                        }
                        break;
                    }
                    case 2:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            rect.scale(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditRect::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditRect::objName);
                        return;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditRect::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditRect::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditRect::objName);
    }
}

void ofeliaEditRect_scaleFromCenter(t_ofeliaEditRect *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByRectName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadRect::rectData[pos].rects.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadRect::rectData[pos].rects.size())-1);
                ofRectangle &rect = t_ofeliaLoadRect::rectData[pos].rects[index];
                
                switch (argc) {
                        
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            rect.scaleFromCenter(argv[0].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditRect::objName);
                            return;
                        }
                        break;
                    }
                    case 2:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            rect.scaleFromCenter(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditRect::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditRect::objName);
                        return;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditRect::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditRect::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditRect::objName);
    }
}

void ofeliaEditRect_standardize(t_ofeliaEditRect *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByRectName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadRect::rectData[pos].rects.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadRect::rectData[pos].rects.size())-1);
                ofRectangle &rect = t_ofeliaLoadRect::rectData[pos].rects[index];
                rect.standardize();
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditRect::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditRect::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditRect::objName);
    }
}

void ofeliaEditRect_translate(t_ofeliaEditRect *x, t_floatarg dx, t_floatarg dy)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByRectName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadRect::rectData[pos].rects.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadRect::rectData[pos].rects.size())-1);
                ofRectangle &rect = t_ofeliaLoadRect::rectData[pos].rects[index];
                rect.translate(dx, dy);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditRect::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditRect::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditRect::objName);
    }
}

void ofeliaEditRect_set(t_ofeliaEditRect *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaEditRect_print(t_ofeliaEditRect *x)
{
    post("\n[%s]", t_ofeliaEditRect::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaEditRect_setup()
{
    ofeliaEditRect_class = class_new(gensym("ofEditRect"),
                                      reinterpret_cast<t_newmethod>(ofeliaEditRect_new),
                                      0, sizeof(t_ofeliaEditRect),
                                      CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaEditRect_class, reinterpret_cast<t_method>(ofeliaEditRect_float));
    class_addmethod(ofeliaEditRect_class, reinterpret_cast<t_method>(ofeliaEditRect_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditRect_class, reinterpret_cast<t_method>(ofeliaEditRect_assign),
                    gensym("assign"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, 0);
    class_addmethod(ofeliaEditRect_class, reinterpret_cast<t_method>(ofeliaEditRect_assignFromCenter),
                    gensym("assignFromCenter"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, 0);
    class_addmethod(ofeliaEditRect_class, reinterpret_cast<t_method>(ofeliaEditRect_assignX),
                    gensym("assignX"), A_FLOAT, 0);
    class_addmethod(ofeliaEditRect_class, reinterpret_cast<t_method>(ofeliaEditRect_assignY),
                    gensym("assignY"), A_FLOAT, 0);
    class_addmethod(ofeliaEditRect_class, reinterpret_cast<t_method>(ofeliaEditRect_assignWidth),
                    gensym("assignWidth"), A_FLOAT, 0);
    class_addmethod(ofeliaEditRect_class, reinterpret_cast<t_method>(ofeliaEditRect_assignHeight),
                    gensym("assignHeight"), A_FLOAT, 0);
    class_addmethod(ofeliaEditRect_class, reinterpret_cast<t_method>(ofeliaEditRect_assignPos),
                    gensym("assignPos"), A_FLOAT, A_FLOAT, 0);
    class_addmethod(ofeliaEditRect_class, reinterpret_cast<t_method>(ofeliaEditRect_assignDimen),
                    gensym("assignDimen"), A_FLOAT, A_FLOAT, 0);
    class_addmethod(ofeliaEditRect_class, reinterpret_cast<t_method>(ofeliaEditRect_growToInclude),
                    gensym("growToInclude"), A_GIMME, 0);
    class_addmethod(ofeliaEditRect_class, reinterpret_cast<t_method>(ofeliaEditRect_scale),
                    gensym("scale"), A_GIMME, 0);
    class_addmethod(ofeliaEditRect_class, reinterpret_cast<t_method>(ofeliaEditRect_scaleFromCenter),
                    gensym("scaleFromCenter"), A_GIMME, 0);
    class_addmethod(ofeliaEditRect_class, reinterpret_cast<t_method>(ofeliaEditRect_standardize),
                    gensym("standardize"), A_NULL, 0);
    class_addmethod(ofeliaEditRect_class, reinterpret_cast<t_method>(ofeliaEditRect_translate),
                    gensym("translate"), A_FLOAT, A_FLOAT, 0);
    class_addmethod(ofeliaEditRect_class, reinterpret_cast<t_method>(ofeliaEditRect_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditRect_class, reinterpret_cast<t_method>(ofeliaEditRect_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofDoesRectNameExist object methods
 */
void *ofeliaDoesRectNameExist_new(t_symbol *s)
{
    t_ofeliaDoesRectNameExist *x = reinterpret_cast<t_ofeliaDoesRectNameExist*>(pd_new(ofeliaDoesRectNameExist_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaDoesRectNameExist_bang(t_ofeliaDoesRectNameExist *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        if (getPositionByRectName(name) != -1)
            outlet_float(x->x_obj.ob_outlet, 1.0f);
        else
            outlet_float(x->x_obj.ob_outlet, 0.0f);
    }
    else {
        
        error("%s: name not assigned", t_ofeliaDoesRectNameExist::objName);
    }
}

void ofeliaDoesRectNameExist_set(t_ofeliaDoesRectNameExist *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaDoesRectNameExist_print(t_ofeliaDoesRectNameExist *x)
{
    post("\n[%s]", t_ofeliaDoesRectNameExist::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaDoesRectNameExist_setup()
{
    ofeliaDoesRectNameExist_class = class_new(gensym("ofDoesRectNameExist"),
                                               reinterpret_cast<t_newmethod>(ofeliaDoesRectNameExist_new),
                                               0, sizeof(t_ofeliaDoesRectNameExist),
                                               CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaDoesRectNameExist_class, reinterpret_cast<t_method>(ofeliaDoesRectNameExist_bang));
    class_addmethod(ofeliaDoesRectNameExist_class, reinterpret_cast<t_method>(ofeliaDoesRectNameExist_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesRectNameExist_class, reinterpret_cast<t_method>(ofeliaDoesRectNameExist_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesRectNameExist_class, reinterpret_cast<t_method>(ofeliaDoesRectNameExist_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetRect object methods
 */
void *ofeliaGetRect_new(t_symbol *s)
{
    t_ofeliaGetRect *x = reinterpret_cast<t_ofeliaGetRect*>(pd_new(ofeliaGetRect_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetRect_bang(t_ofeliaGetRect *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByRectName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadRect::rectData[pos].rects.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadRect::rectData[pos].rects.size())-1);
                const ofRectangle &rect = t_ofeliaLoadRect::rectData[pos].rects[index];
                t_atom av[4];
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = rect.x;
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = rect.y;
                av[2].a_type = A_FLOAT;
                av[2].a_w.w_float = rect.width;
                av[3].a_type = A_FLOAT;
                av[3].a_w.w_float = rect.height;
                outlet_list(x->x_obj.ob_outlet, &s_list, 4, av);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetRect::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetRect::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetRect::objName);
    }
}

void ofeliaGetRect_float(t_ofeliaGetRect *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetRect_set(t_ofeliaGetRect *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetRect_print(t_ofeliaGetRect *x)
{
    post("\n[%s]", t_ofeliaGetRect::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetRect_setup()
{
    ofeliaGetRect_class = class_new(gensym("ofGetRect"),
                                     reinterpret_cast<t_newmethod>(ofeliaGetRect_new),
                                     0, sizeof(t_ofeliaGetRect),
                                     CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetRect_class, reinterpret_cast<t_method>(ofeliaGetRect_bang));
    class_addfloat(ofeliaGetRect_class, reinterpret_cast<t_method>(ofeliaGetRect_float));
    class_addmethod(ofeliaGetRect_class, reinterpret_cast<t_method>(ofeliaGetRect_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetRect_class, reinterpret_cast<t_method>(ofeliaGetRect_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetRect_class, reinterpret_cast<t_method>(ofeliaGetRect_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetRects object methods
 */
void *ofeliaGetRects_new(t_symbol *s)
{
    t_ofeliaGetRects *x = reinterpret_cast<t_ofeliaGetRects*>(pd_new(ofeliaGetRects_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetRects_bang(t_ofeliaGetRects *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByRectName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadRect::rectData[pos].rects.empty()) {
                
                const vector<ofRectangle> &rects = t_ofeliaLoadRect::rectData[pos].rects;
                const int ac = static_cast<int>(rects.size()*4);
                t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
                
                for (int i=0; i<ac; i+=4) {
                    
                    const int idx = i/4;
                    av[i].a_type = A_FLOAT;
                    av[i].a_w.w_float = rects[idx].x;
                    av[i+1].a_type = A_FLOAT;
                    av[i+1].a_w.w_float = rects[idx].y;
                    av[i+2].a_type = A_FLOAT;
                    av[i+2].a_w.w_float = rects[idx].width;
                    av[i+3].a_type = A_FLOAT;
                    av[i+3].a_w.w_float = rects[idx].height;
                }
                outlet_list(x->x_obj.ob_outlet, &s_list, ac, av);
                freebytes(av, ac * sizeof(t_atom));
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetRects::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetRects::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetRects::objName);
    }
}

void ofeliaGetRects_set(t_ofeliaGetRects *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetRects_print(t_ofeliaGetRects *x)
{
    post("\n[%s]", t_ofeliaGetRects::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetRects_setup()
{
    ofeliaGetRects_class = class_new(gensym("ofGetRects"),
                                      reinterpret_cast<t_newmethod>(ofeliaGetRects_new),
                                      0, sizeof(t_ofeliaGetRects),
                                      CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetRects_class, reinterpret_cast<t_method>(ofeliaGetRects_bang));
    class_addmethod(ofeliaGetRects_class, reinterpret_cast<t_method>(ofeliaGetRects_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetRects_class, reinterpret_cast<t_method>(ofeliaGetRects_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetRects_class, reinterpret_cast<t_method>(ofeliaGetRects_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofIsPointInsideRect object methods
 */
void *ofeliaIsPointInsideRect_new(t_symbol *s)
{
    t_ofeliaIsPointInsideRect *x = reinterpret_cast<t_ofeliaIsPointInsideRect*>(pd_new(ofeliaIsPointInsideRect_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaIsPointInsideRect_float(t_ofeliaIsPointInsideRect *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaIsPointInsideRect_list(t_ofeliaIsPointInsideRect *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByRectName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadRect::rectData[pos].rects.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadRect::rectData[pos].rects.size())-1);
                const ofRectangle &rect = t_ofeliaLoadRect::rectData[pos].rects[index];
                
                if (argc >= 2) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT) {
                        
                        const bool inside = rect.inside(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(inside));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaIsPointInsideRect::objName);
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaIsPointInsideRect::objName);
                }
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaIsPointInsideRect::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaIsPointInsideRect::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaIsPointInsideRect::objName);
    }
}

void ofeliaIsPointInsideRect_set(t_ofeliaIsPointInsideRect *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaIsPointInsideRect_print(t_ofeliaIsPointInsideRect *x)
{
    post("\n[%s]", t_ofeliaIsPointInsideRect::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaIsPointInsideRect_setup()
{
    ofeliaIsPointInsideRect_class = class_new(gensym("ofIsPointInsideRect"),
                                              reinterpret_cast<t_newmethod>(ofeliaIsPointInsideRect_new),
                                              0, sizeof(t_ofeliaIsPointInsideRect),
                                              CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaIsPointInsideRect_class, reinterpret_cast<t_method>(ofeliaIsPointInsideRect_float));
    class_addlist(ofeliaIsPointInsideRect_class, reinterpret_cast<t_method>(ofeliaIsPointInsideRect_list));
    class_addmethod(ofeliaIsPointInsideRect_class, reinterpret_cast<t_method>(ofeliaIsPointInsideRect_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaIsPointInsideRect_class, reinterpret_cast<t_method>(ofeliaIsPointInsideRect_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaIsPointInsideRect_class, reinterpret_cast<t_method>(ofeliaIsPointInsideRect_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofIsLineInsideRect object methods
 */
void *ofeliaIsLineInsideRect_new(t_symbol *s)
{
    t_ofeliaIsLineInsideRect *x = reinterpret_cast<t_ofeliaIsLineInsideRect*>(pd_new(ofeliaIsLineInsideRect_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaIsLineInsideRect_float(t_ofeliaIsLineInsideRect *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaIsLineInsideRect_list(t_ofeliaIsLineInsideRect *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByRectName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadRect::rectData[pos].rects.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadRect::rectData[pos].rects.size())-1);
                const ofRectangle &rect = t_ofeliaLoadRect::rectData[pos].rects[index];
                
                if (argc == 4) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT &&
                        argv[3].a_type == A_FLOAT) {

                        const bool inside = rect.inside(ofPoint(argv[0].a_w.w_float, argv[1].a_w.w_float),
                                                        ofPoint(argv[2].a_w.w_float, argv[3].a_w.w_float));
                        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(inside));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaIsLineInsideRect::objName);
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaIsLineInsideRect::objName);
                }
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaIsLineInsideRect::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaIsLineInsideRect::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaIsLineInsideRect::objName);
    }
}

void ofeliaIsLineInsideRect_set(t_ofeliaIsLineInsideRect *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaIsLineInsideRect_print(t_ofeliaIsLineInsideRect *x)
{
    post("\n[%s]", t_ofeliaIsLineInsideRect::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaIsLineInsideRect_setup()
{
    ofeliaIsLineInsideRect_class = class_new(gensym("ofIsLineInsideRect"),
                                             reinterpret_cast<t_newmethod>(ofeliaIsLineInsideRect_new),
                                             0, sizeof(t_ofeliaIsLineInsideRect),
                                             CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaIsLineInsideRect_class, reinterpret_cast<t_method>(ofeliaIsLineInsideRect_float));
    class_addlist(ofeliaIsLineInsideRect_class, reinterpret_cast<t_method>(ofeliaIsLineInsideRect_list));
    class_addmethod(ofeliaIsLineInsideRect_class, reinterpret_cast<t_method>(ofeliaIsLineInsideRect_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaIsLineInsideRect_class, reinterpret_cast<t_method>(ofeliaIsLineInsideRect_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaIsLineInsideRect_class, reinterpret_cast<t_method>(ofeliaIsLineInsideRect_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofIsRectInsideRect object methods
 */
void *ofeliaIsRectInsideRect_new(t_symbol *s)
{
    t_ofeliaIsRectInsideRect *x = reinterpret_cast<t_ofeliaIsRectInsideRect*>(pd_new(ofeliaIsRectInsideRect_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaIsRectInsideRect_float(t_ofeliaIsRectInsideRect *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaIsRectInsideRect_list(t_ofeliaIsRectInsideRect *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByRectName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadRect::rectData[pos].rects.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadRect::rectData[pos].rects.size())-1);
                const ofRectangle &rect = t_ofeliaLoadRect::rectData[pos].rects[index];
                
                if (argc == 4) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT &&
                        argv[3].a_type == A_FLOAT) {
                        
                        const bool inside = rect.inside(ofRectangle(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float, argv[3].a_w.w_float));
                        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(inside));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaIsRectInsideRect::objName);
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaIsRectInsideRect::objName);
                }
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaIsRectInsideRect::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaIsRectInsideRect::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaIsRectInsideRect::objName);
    }
}

void ofeliaIsRectInsideRect_set(t_ofeliaIsRectInsideRect *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaIsRectInsideRect_print(t_ofeliaIsRectInsideRect *x)
{
    post("\n[%s]", t_ofeliaIsRectInsideRect::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaIsRectInsideRect_setup()
{
    ofeliaIsRectInsideRect_class = class_new(gensym("ofIsRectInsideRect"),
                                             reinterpret_cast<t_newmethod>(ofeliaIsRectInsideRect_new),
                                             0, sizeof(t_ofeliaIsRectInsideRect),
                                             CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaIsRectInsideRect_class, reinterpret_cast<t_method>(ofeliaIsRectInsideRect_float));
    class_addlist(ofeliaIsRectInsideRect_class, reinterpret_cast<t_method>(ofeliaIsRectInsideRect_list));
    class_addmethod(ofeliaIsRectInsideRect_class, reinterpret_cast<t_method>(ofeliaIsRectInsideRect_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaIsRectInsideRect_class, reinterpret_cast<t_method>(ofeliaIsRectInsideRect_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaIsRectInsideRect_class, reinterpret_cast<t_method>(ofeliaIsRectInsideRect_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofDoesLineIntersectRect object methods
 */
void *ofeliaDoesLineIntersectRect_new(t_symbol *s)
{
    t_ofeliaDoesLineIntersectRect *x = reinterpret_cast<t_ofeliaDoesLineIntersectRect*>(pd_new(ofeliaDoesLineIntersectRect_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaDoesLineIntersectRect_float(t_ofeliaDoesLineIntersectRect *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaDoesLineIntersectRect_list(t_ofeliaDoesLineIntersectRect *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByRectName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadRect::rectData[pos].rects.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadRect::rectData[pos].rects.size())-1);
                const ofRectangle &rect = t_ofeliaLoadRect::rectData[pos].rects[index];
                
                if (argc == 4) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT &&
                        argv[3].a_type == A_FLOAT) {
                        
                        const bool inside = rect.intersects(ofPoint(argv[0].a_w.w_float, argv[1].a_w.w_float),
                                                            ofPoint(argv[2].a_w.w_float, argv[3].a_w.w_float));
                        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(inside));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaDoesLineIntersectRect::objName);
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaDoesLineIntersectRect::objName);
                }
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaDoesLineIntersectRect::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaDoesLineIntersectRect::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaDoesLineIntersectRect::objName);
    }
}

void ofeliaDoesLineIntersectRect_set(t_ofeliaDoesLineIntersectRect *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaDoesLineIntersectRect_print(t_ofeliaDoesLineIntersectRect *x)
{
    post("\n[%s]", t_ofeliaDoesLineIntersectRect::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaDoesLineIntersectRect_setup()
{
    ofeliaDoesLineIntersectRect_class = class_new(gensym("ofDoesLineIntersectRect"),
                                                  reinterpret_cast<t_newmethod>(ofeliaDoesLineIntersectRect_new),
                                                  0, sizeof(t_ofeliaDoesLineIntersectRect),
                                                  CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaDoesLineIntersectRect_class, reinterpret_cast<t_method>(ofeliaDoesLineIntersectRect_float));
    class_addlist(ofeliaDoesLineIntersectRect_class, reinterpret_cast<t_method>(ofeliaDoesLineIntersectRect_list));
    class_addmethod(ofeliaDoesLineIntersectRect_class, reinterpret_cast<t_method>(ofeliaDoesLineIntersectRect_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesLineIntersectRect_class, reinterpret_cast<t_method>(ofeliaDoesLineIntersectRect_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesLineIntersectRect_class, reinterpret_cast<t_method>(ofeliaDoesLineIntersectRect_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofDoesRectIntersectRect object methods
 */
void *ofeliaDoesRectIntersectRect_new(t_symbol *s)
{
    t_ofeliaDoesRectIntersectRect *x = reinterpret_cast<t_ofeliaDoesRectIntersectRect*>(pd_new(ofeliaDoesRectIntersectRect_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaDoesRectIntersectRect_float(t_ofeliaDoesRectIntersectRect *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaDoesRectIntersectRect_list(t_ofeliaDoesRectIntersectRect *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByRectName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadRect::rectData[pos].rects.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadRect::rectData[pos].rects.size())-1);
                const ofRectangle &rect = t_ofeliaLoadRect::rectData[pos].rects[index];
                
                if (argc == 4) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT &&
                        argv[3].a_type == A_FLOAT) {
                        
                        const bool inside = rect.intersects(ofRectangle(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float, argv[3].a_w.w_float));
                        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(inside));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaDoesRectIntersectRect::objName);
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaDoesRectIntersectRect::objName);
                }
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaDoesRectIntersectRect::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaDoesRectIntersectRect::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaDoesRectIntersectRect::objName);
    }
}

void ofeliaDoesRectIntersectRect_set(t_ofeliaDoesRectIntersectRect *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaDoesRectIntersectRect_print(t_ofeliaDoesRectIntersectRect *x)
{
    post("\n[%s]", t_ofeliaDoesRectIntersectRect::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaDoesRectIntersectRect_setup()
{
    ofeliaDoesRectIntersectRect_class = class_new(gensym("ofDoesRectIntersectRect"),
                                                  reinterpret_cast<t_newmethod>(ofeliaDoesRectIntersectRect_new),
                                                  0, sizeof(t_ofeliaDoesRectIntersectRect),
                                                  CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaDoesRectIntersectRect_class, reinterpret_cast<t_method>(ofeliaDoesRectIntersectRect_float));
    class_addlist(ofeliaDoesRectIntersectRect_class, reinterpret_cast<t_method>(ofeliaDoesRectIntersectRect_list));
    class_addmethod(ofeliaDoesRectIntersectRect_class, reinterpret_cast<t_method>(ofeliaDoesRectIntersectRect_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesRectIntersectRect_class, reinterpret_cast<t_method>(ofeliaDoesRectIntersectRect_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesRectIntersectRect_class, reinterpret_cast<t_method>(ofeliaDoesRectIntersectRect_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetRectCenter object methods
 */
void *ofeliaGetRectCenter_new(t_symbol *s)
{
    t_ofeliaGetRectCenter *x = reinterpret_cast<t_ofeliaGetRectCenter*>(pd_new(ofeliaGetRectCenter_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetRectCenter_bang(t_ofeliaGetRectCenter *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByRectName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadRect::rectData[pos].rects.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadRect::rectData[pos].rects.size())-1);
                const ofRectangle &rect = t_ofeliaLoadRect::rectData[pos].rects[index];
                t_atom av[2];
                const ofPoint &center = rect.getCenter();
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = center.x;
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = center.y;
                outlet_list(x->x_obj.ob_outlet, &s_list, 2, av);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetRectCenter::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetRectCenter::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetRectCenter::objName);
    }
}

void ofeliaGetRectCenter_float(t_ofeliaGetRectCenter *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetRectCenter_set(t_ofeliaGetRectCenter *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetRectCenter_print(t_ofeliaGetRectCenter *x)
{
    post("\n[%s]", t_ofeliaGetRectCenter::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetRectCenter_setup()
{
    ofeliaGetRectCenter_class = class_new(gensym("ofGetRectCenter"),
                                          reinterpret_cast<t_newmethod>(ofeliaGetRectCenter_new),
                                          0, sizeof(t_ofeliaGetRectCenter),
                                          CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetRectCenter_class, reinterpret_cast<t_method>(ofeliaGetRectCenter_bang));
    class_addfloat(ofeliaGetRectCenter_class, reinterpret_cast<t_method>(ofeliaGetRectCenter_float));
    class_addmethod(ofeliaGetRectCenter_class, reinterpret_cast<t_method>(ofeliaGetRectCenter_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetRectCenter_class, reinterpret_cast<t_method>(ofeliaGetRectCenter_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetRectCenter_class, reinterpret_cast<t_method>(ofeliaGetRectCenter_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetRectArea object methods
 */
void *ofeliaGetRectArea_new(t_symbol *s)
{
    t_ofeliaGetRectArea *x = reinterpret_cast<t_ofeliaGetRectArea*>(pd_new(ofeliaGetRectArea_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetRectArea_bang(t_ofeliaGetRectArea *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByRectName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadRect::rectData[pos].rects.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadRect::rectData[pos].rects.size())-1);
                const ofRectangle &rect = t_ofeliaLoadRect::rectData[pos].rects[index];
                outlet_float(x->x_obj.ob_outlet, rect.getArea());
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetRectArea::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetRectArea::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetRectArea::objName);
    }
}

void ofeliaGetRectArea_float(t_ofeliaGetRectArea *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetRectArea_set(t_ofeliaGetRectArea *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetRectArea_print(t_ofeliaGetRectArea *x)
{
    post("\n[%s]", t_ofeliaGetRectArea::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetRectArea_setup()
{
    ofeliaGetRectArea_class = class_new(gensym("ofGetRectArea"),
                                        reinterpret_cast<t_newmethod>(ofeliaGetRectArea_new),
                                        0, sizeof(t_ofeliaGetRectArea),
                                        CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetRectArea_class, reinterpret_cast<t_method>(ofeliaGetRectArea_bang));
    class_addfloat(ofeliaGetRectArea_class, reinterpret_cast<t_method>(ofeliaGetRectArea_float));
    class_addmethod(ofeliaGetRectArea_class, reinterpret_cast<t_method>(ofeliaGetRectArea_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetRectArea_class, reinterpret_cast<t_method>(ofeliaGetRectArea_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetRectArea_class, reinterpret_cast<t_method>(ofeliaGetRectArea_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetRectPerimeter object methods
 */
void *ofeliaGetRectPerimeter_new(t_symbol *s)
{
    t_ofeliaGetRectPerimeter *x = reinterpret_cast<t_ofeliaGetRectPerimeter*>(pd_new(ofeliaGetRectPerimeter_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetRectPerimeter_bang(t_ofeliaGetRectPerimeter *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByRectName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadRect::rectData[pos].rects.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadRect::rectData[pos].rects.size())-1);
                const ofRectangle &rect = t_ofeliaLoadRect::rectData[pos].rects[index];
                outlet_float(x->x_obj.ob_outlet, rect.getPerimeter());
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetRectPerimeter::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetRectPerimeter::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetRectPerimeter::objName);
    }
}

void ofeliaGetRectPerimeter_float(t_ofeliaGetRectPerimeter *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetRectPerimeter_set(t_ofeliaGetRectPerimeter *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetRectPerimeter_print(t_ofeliaGetRectPerimeter *x)
{
    post("\n[%s]", t_ofeliaGetRectPerimeter::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetRectPerimeter_setup()
{
    ofeliaGetRectPerimeter_class = class_new(gensym("ofGetRectPerimeter"),
                                             reinterpret_cast<t_newmethod>(ofeliaGetRectPerimeter_new),
                                             0, sizeof(t_ofeliaGetRectPerimeter),
                                             CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetRectPerimeter_class, reinterpret_cast<t_method>(ofeliaGetRectPerimeter_bang));
    class_addfloat(ofeliaGetRectPerimeter_class, reinterpret_cast<t_method>(ofeliaGetRectPerimeter_float));
    class_addmethod(ofeliaGetRectPerimeter_class, reinterpret_cast<t_method>(ofeliaGetRectPerimeter_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetRectPerimeter_class, reinterpret_cast<t_method>(ofeliaGetRectPerimeter_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetRectPerimeter_class, reinterpret_cast<t_method>(ofeliaGetRectPerimeter_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * setup methods
 */
void ofeliaRect_setup()
{
    ofeliaLoadRect_setup();
    ofeliaEditRect_setup();
    ofeliaDoesRectNameExist_setup();
    ofeliaGetRect_setup();
    ofeliaGetRects_setup();
    ofeliaIsPointInsideRect_setup();
    ofeliaIsLineInsideRect_setup();
    ofeliaIsRectInsideRect_setup();
    ofeliaDoesLineIntersectRect_setup();
    ofeliaDoesRectIntersectRect_setup();
    ofeliaGetRectCenter_setup();
    ofeliaGetRectArea_setup();
    ofeliaGetRectPerimeter_setup();
}

