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

#include "ofeliaVec2f.h"

/* ________________________________________________________________________________
 * static variables
 */
const char *t_ofeliaLoadVec2f::objName = "ofLoadVec2f";
unsigned int t_ofeliaLoadVec2f::counter;
vector<t_ofeliaLoadVec2fData> t_ofeliaLoadVec2f::vecData;
const char *t_ofeliaEditVec2f::objName = "ofEditVec2f";
const char *t_ofeliaDoesVec2fNameExist::objName = "ofDoesVec2fNameExist";
const char *t_ofeliaGetVec2f::objName = "ofGetVec2f";
const char *t_ofeliaGetVec2fs::objName = "ofGetVec2fs";
const char *t_ofeliaGetVec2fAverage::objName = "ofGetVec2fAverage";
const char *t_ofeliaGetVec2fAngle::objName = "ofGetVec2fAngle";
const char *t_ofeliaGetVec2fAngleRad::objName = "ofGetVec2fAngleRad";
const char *t_ofeliaGetVec2fDist::objName = "ofGetVec2fDist";
const char *t_ofeliaGetVec2fDistSquared::objName = "ofGetVec2fDistSquared";
const char *t_ofeliaGetVec2fDot::objName = "ofGetVec2fDot";
const char *t_ofeliaGetVec2fLength::objName = "ofGetVec2fLength";
const char *t_ofeliaGetVec2fLengthSquared::objName = "ofGetVec2fLengthSquared";

/* ________________________________________________________________________________
 * ofLoadVec2f object methods
 */
void ofeliaLoadVec2f_vecSizeOut(t_ofeliaLoadVec2f *x);

int getPositionByVec2fObjID(const unsigned int objID)
{
    const auto it = find_if(begin(t_ofeliaLoadVec2f::vecData), end(t_ofeliaLoadVec2f::vecData),
                            [&objID](const t_ofeliaLoadVec2fData &vecData)
                            {
                                return vecData.objID == objID;
                            });
    return static_cast<int>(it - t_ofeliaLoadVec2f::vecData.begin());
}

bool getLoadVec2fFloatFromArgs(const int argc, const t_atom *argv, ofVec2f &vecFloat)
{
    if (argc) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            vecFloat.set(argv[0].a_w.w_float, argv[1].a_w.w_float);
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadVec2f::objName);
            return 0;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadVec2f::objName);
        return 0;
    }
    return 1;
}

bool getLoadVec2fFloatsFromArgs(const int argc, const t_atom *argv, vector<ofVec2f> &vecFloats)
{
    if (argc && argc%2 == 0) {
        
        for (int i=0; i<argc; i+=2) {
            
            if (argv[i].a_type == A_FLOAT &&
                argv[i+1].a_type == A_FLOAT) {
                
                vecFloats.push_back(ofVec2f(argv[i].a_w.w_float, argv[i+1].a_w.w_float));
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaLoadVec2f::objName);
                return 0;
            }
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadVec2f::objName);
        return 0;
    }
    return 1;
}

bool getLoadVec2fCmdRangeFromArgs(const int argc, const t_atom *argv, t_ofeliaLoadVec2fCmdData &cmd)
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
                
                error("%s: wrong argument type", t_ofeliaLoadVec2f::objName);
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
                
                error("%s: wrong argument type", t_ofeliaLoadVec2f::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaLoadVec2f::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaLoadVec2f_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadVec2f *x = reinterpret_cast<t_ofeliaLoadVec2f*>(pd_new(ofeliaLoadVec2f_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    t_ofeliaLoadVec2fData vecData;
    getVarNameLocalPrefixes(vecData.varName);
    vector<ofVec2f> vecFloats;
    
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            vecData.varName.name = argv[0].a_w.w_symbol->s_name;
            getVarNameLocalized(vecData.varName);
            
            if (--argc) {
                
                if (!getLoadVec2fFloatsFromArgs(argc, argv+1, vecFloats))
                    return 0;
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadVec2f::objName);
            return 0;
        }
    }
    x->objID = vecData.objID = t_ofeliaLoadVec2f::counter++;
    t_ofeliaLoadVec2f::vecData.push_back(vecData);
    x->vecSize = 0;
    x->vecSizeOutClock = clock_new(x, reinterpret_cast<t_method>(ofeliaLoadVec2f_vecSizeOut));
    outlet_new(&x->x_obj, &s_float);
    
    if (argc) {
        
        const int pos = getPositionByVec2fObjID(x->objID);
        t_ofeliaLoadVec2f::vecData[pos].vecFloats = vecFloats;
        x->vecSize = static_cast<int>(vecFloats.size());
        clock_delay(x->vecSizeOutClock, 0.0);
    }
    return (x);
}

void ofeliaLoadVec2f_vecSizeOut(t_ofeliaLoadVec2f *x)
{
    outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(x->vecSize));
}

void ofeliaLoadVec2f_name(t_ofeliaLoadVec2f *x, t_symbol *s)
{
    const int pos = getPositionByVec2fObjID(x->objID);
    t_ofeliaLoadVec2f::vecData[pos].varName.name = s->s_name;
    getVarNameLocalized(t_ofeliaLoadVec2f::vecData[pos].varName);
}

void ofeliaLoadVec2f_load(t_ofeliaLoadVec2f *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<ofVec2f> vecFloats;
    
    if (getLoadVec2fFloatsFromArgs(argc, argv, vecFloats)) {
        
        const int pos = getPositionByVec2fObjID(x->objID);
        t_ofeliaLoadVec2f::vecData[pos].vecFloats = vecFloats;
        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(vecFloats.size()));
    }
}

void ofeliaLoadVec2f_add(t_ofeliaLoadVec2f *x, t_symbol *s, int argc, t_atom *argv)
{
    ofVec2f vecFloat;
    
    if (getLoadVec2fFloatFromArgs(argc, argv, vecFloat)) {
        
        const int pos = getPositionByVec2fObjID(x->objID);
        t_ofeliaLoadVec2f::vecData[pos].vecFloats.push_back(vecFloat);
        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadVec2f::vecData[pos].vecFloats.size()));
    }
}

void ofeliaLoadVec2f_append(t_ofeliaLoadVec2f *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<ofVec2f> vecFloats;
    
    if (getLoadVec2fFloatsFromArgs(argc, argv, vecFloats)) {
        
        const int pos = getPositionByVec2fObjID(x->objID);
        t_ofeliaLoadVec2f::vecData[pos].vecFloats.insert(t_ofeliaLoadVec2f::vecData[pos].vecFloats.end(),
                                                         vecFloats.begin(), vecFloats.end());
        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadVec2f::vecData[pos].vecFloats.size()));
    }
}

void ofeliaLoadVec2f_prepend(t_ofeliaLoadVec2f *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<ofVec2f> vecFloats;
    
    if (getLoadVec2fFloatsFromArgs(argc, argv, vecFloats)) {
        
        const int pos = getPositionByVec2fObjID(x->objID);
        t_ofeliaLoadVec2f::vecData[pos].vecFloats.insert(t_ofeliaLoadVec2f::vecData[pos].vecFloats.begin(),
                                                         vecFloats.begin(), vecFloats.end());
        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadVec2f::vecData[pos].vecFloats.size()));
    }
}

void ofeliaLoadVec2f_insert(t_ofeliaLoadVec2f *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadVec2fCmdData cmd;
    
    if (getLoadVec2fFloatFromArgs(argc, argv, cmd.vecFloat)) {
        
        if (getLoadVec2fCmdRangeFromArgs(argc-2, argv+2, cmd)) {
            
            const int pos = getPositionByVec2fObjID(x->objID);
            const int size = static_cast<int>(t_ofeliaLoadVec2f::vecData[pos].vecFloats.size());
            const auto vecFloatsBegin = t_ofeliaLoadVec2f::vecData[pos].vecFloats.begin();
            const int fromIndex = static_cast<int>(ofClamp(cmd.fromIndex, 0.0f, size));
            const int insertSize = static_cast<int>(max(0.0f, max(0.0f, cmd.toIndex) - max(0.0f, cmd.fromIndex))) + 1;
            t_ofeliaLoadVec2f::vecData[pos].vecFloats.insert(vecFloatsBegin + fromIndex,
                                                             insertSize, cmd.vecFloat);
            outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadVec2f::vecData[pos].vecFloats.size()));
        }
    }
}

void ofeliaLoadVec2f_fill(t_ofeliaLoadVec2f *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadVec2fCmdData cmd;
    
    if (getLoadVec2fFloatFromArgs(argc, argv, cmd.vecFloat)) {
        
        if (getLoadVec2fCmdRangeFromArgs(argc-2, argv+2, cmd)) {
            
            const int pos = getPositionByVec2fObjID(x->objID);
            const int size = static_cast<int>(t_ofeliaLoadVec2f::vecData[pos].vecFloats.size());
            const auto vecFloatsBegin = t_ofeliaLoadVec2f::vecData[pos].vecFloats.begin();
            
            if (size) {
                
                const int maxIndex = size - 1;
                const int fromIndex = static_cast<int>(ofClamp(cmd.fromIndex, 0.0f, maxIndex));
                const int toIndex = static_cast<int>(ofClamp(cmd.toIndex, fromIndex, maxIndex)) + 1;
                fill(vecFloatsBegin + fromIndex, vecFloatsBegin + toIndex, cmd.vecFloat);
                outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadVec2f::vecData[pos].vecFloats.size()));
            }
            else {
                
                outlet_float(x->x_obj.ob_outlet, 0.0f);
            }
        }
    }
}

void ofeliaLoadVec2f_erase(t_ofeliaLoadVec2f *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadVec2fCmdData cmd;
    
    if (getLoadVec2fCmdRangeFromArgs(argc, argv, cmd)) {
        
        const int pos = getPositionByVec2fObjID(x->objID);
        const int size = static_cast<int>(t_ofeliaLoadVec2f::vecData[pos].vecFloats.size());
        const auto vecFloatsBegin = t_ofeliaLoadVec2f::vecData[pos].vecFloats.begin();
        
        if (size) {
            
            const int maxIndex = size - 1;
            const int fromIndex = static_cast<int>(ofClamp(cmd.fromIndex, 0.0f, maxIndex));
            const int toIndex = static_cast<int>(ofClamp(cmd.toIndex, fromIndex, maxIndex)) + 1;
            t_ofeliaLoadVec2f::vecData[pos].vecFloats.erase(vecFloatsBegin + fromIndex,
                                                            vecFloatsBegin + toIndex);
            outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadVec2f::vecData[pos].vecFloats.size()));
        }
        else {
            
            outlet_float(x->x_obj.ob_outlet, 0.0f);
        }
    }
}

void ofeliaLoadVec2f_clear(t_ofeliaLoadVec2f *x)
{
    const int pos = getPositionByVec2fObjID(x->objID);
    t_ofeliaLoadVec2f::vecData[pos].vecFloats.clear();
    outlet_float(x->x_obj.ob_outlet, 0.0f);
}

void ofeliaLoadVec2f_set(t_ofeliaLoadVec2f *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            ofeliaLoadVec2f_name(x, argv[0].a_w.w_symbol);
            
            if (--argc)
                ofeliaLoadVec2f_load(x, 0, argc, argv+1);
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadVec2f::objName);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadVec2f::objName);
    }
}

void ofeliaLoadVec2f_print(t_ofeliaLoadVec2f *x)
{
    post("\n[%s]", t_ofeliaLoadVec2f::objName);
    const int pos = getPositionByVec2fObjID(x->objID);
    post("name : %s", t_ofeliaLoadVec2f::vecData[pos].varName.name.c_str());
    post("load : ");
    
    for (size_t i=0; i<t_ofeliaLoadVec2f::vecData[pos].vecFloats.size(); ++i) {
        
        post(" %g %g", t_ofeliaLoadVec2f::vecData[pos].vecFloats[i].x, t_ofeliaLoadVec2f::vecData[pos].vecFloats[i].y);
    }
}

void ofeliaLoadVec2f_free(t_ofeliaLoadVec2f *x)
{
    clock_free(x->vecSizeOutClock);
    const int pos = getPositionByVec2fObjID(x->objID);
    t_ofeliaLoadVec2f::vecData.erase(t_ofeliaLoadVec2f::vecData.begin() + pos);
}

void ofeliaLoadVec2f_setup()
{
    ofeliaLoadVec2f_class = class_new(gensym("ofLoadVec2f"),
                                      reinterpret_cast<t_newmethod>(ofeliaLoadVec2f_new),
                                      reinterpret_cast<t_method>(ofeliaLoadVec2f_free),
                                      sizeof(t_ofeliaLoadVec2f),
                                      CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaLoadVec2f_class, reinterpret_cast<t_method>(ofeliaLoadVec2f_name),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaLoadVec2f_class, reinterpret_cast<t_method>(ofeliaLoadVec2f_load),
                    gensym("load"), A_GIMME, 0);
    class_addmethod(ofeliaLoadVec2f_class, reinterpret_cast<t_method>(ofeliaLoadVec2f_add),
                    gensym("add"), A_GIMME, 0);
    class_addmethod(ofeliaLoadVec2f_class, reinterpret_cast<t_method>(ofeliaLoadVec2f_append),
                    gensym("append"), A_GIMME, 0);
    class_addmethod(ofeliaLoadVec2f_class, reinterpret_cast<t_method>(ofeliaLoadVec2f_prepend),
                    gensym("prepend"), A_GIMME, 0);
    class_addmethod(ofeliaLoadVec2f_class, reinterpret_cast<t_method>(ofeliaLoadVec2f_insert),
                    gensym("insert"), A_GIMME, 0);
    class_addmethod(ofeliaLoadVec2f_class, reinterpret_cast<t_method>(ofeliaLoadVec2f_fill),
                    gensym("fill"), A_GIMME, 0);
    class_addmethod(ofeliaLoadVec2f_class, reinterpret_cast<t_method>(ofeliaLoadVec2f_erase),
                    gensym("erase"), A_GIMME, 0);
    class_addmethod(ofeliaLoadVec2f_class, reinterpret_cast<t_method>(ofeliaLoadVec2f_clear),
                    gensym("clear"), A_NULL, 0);
    class_addmethod(ofeliaLoadVec2f_class, reinterpret_cast<t_method>(ofeliaLoadVec2f_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaLoadVec2f_class, reinterpret_cast<t_method>(ofeliaLoadVec2f_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * common methods
 */
int getPositionByVec2fName(const t_string &name)
{
    auto it = find_if(begin(t_ofeliaLoadVec2f::vecData), end(t_ofeliaLoadVec2f::vecData),
                      [&name] (const t_ofeliaLoadVec2fData &vecData) {
                          return (vecData.varName.name == name);
                      });
    if (it == end(t_ofeliaLoadVec2f::vecData))
        return -1;
    return static_cast<int>(it - begin(t_ofeliaLoadVec2f::vecData));
}

/* ________________________________________________________________________________
 * ofEditVec2f object methods
 */
void *ofeliaEditVec2f_new(t_symbol *s)
{
    t_ofeliaEditVec2f *x = reinterpret_cast<t_ofeliaEditVec2f*>(pd_new(ofeliaEditVec2f_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaEditVec2f_float(t_ofeliaEditVec2f *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaEditVec2f_assign(t_ofeliaEditVec2f *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec2fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec2f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec2f::vecData[pos].vecFloats.size())-1);
                ofVec2f &vecFloat = t_ofeliaLoadVec2f::vecData[pos].vecFloats[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditVec2f::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            vecFloat = ofVec2f(argv[0].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec2f::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            vecFloat = ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec2f::objName);
                            return;
                        }
                        break;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec2f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec2f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec2f::objName);
    }
}

void ofeliaEditVec2f_assignX(t_ofeliaEditVec2f *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {

        const int pos = getPositionByVec2fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec2f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec2f::vecData[pos].vecFloats.size())-1);
                ofVec2f &vecFloat = t_ofeliaLoadVec2f::vecData[pos].vecFloats[index];
                vecFloat.x = f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec2f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec2f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec2f::objName);
    }
}

void ofeliaEditVec2f_assignY(t_ofeliaEditVec2f *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec2fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec2f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec2f::vecData[pos].vecFloats.size())-1);
                ofVec2f &vecFloat = t_ofeliaLoadVec2f::vecData[pos].vecFloats[index];
                vecFloat.y = f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec2f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec2f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec2f::objName);
    }
}

void ofeliaEditVec2f_add(t_ofeliaEditVec2f *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec2fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec2f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec2f::vecData[pos].vecFloats.size())-1);
                ofVec2f &vecFloat = t_ofeliaLoadVec2f::vecData[pos].vecFloats[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditVec2f::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            vecFloat += argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec2f::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            vecFloat += ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec2f::objName);
                            return;
                        }
                        break;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec2f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec2f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec2f::objName);
    }
}

void ofeliaEditVec2f_sub(t_ofeliaEditVec2f *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec2fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec2f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec2f::vecData[pos].vecFloats.size())-1);
                ofVec2f &vecFloat = t_ofeliaLoadVec2f::vecData[pos].vecFloats[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditVec2f::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            vecFloat -= argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec2f::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            vecFloat -= ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec2f::objName);
                            return;
                        }
                        break;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec2f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec2f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec2f::objName);
    }
}

void ofeliaEditVec2f_mult(t_ofeliaEditVec2f *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec2fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec2f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec2f::vecData[pos].vecFloats.size())-1);
                ofVec2f &vecFloat = t_ofeliaLoadVec2f::vecData[pos].vecFloats[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditVec2f::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            vecFloat *= argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec2f::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            vecFloat *= ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec2f::objName);
                            return;
                        }
                        break;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec2f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec2f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec2f::objName);
    }
}

void ofeliaEditVec2f_div(t_ofeliaEditVec2f *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec2fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec2f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec2f::vecData[pos].vecFloats.size())-1);
                ofVec2f &vecFloat = t_ofeliaLoadVec2f::vecData[pos].vecFloats[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditVec2f::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            vecFloat /= argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec2f::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            vecFloat /= ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec2f::objName);
                            return;
                        }
                        break;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec2f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec2f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec2f::objName);
    }
}

void ofeliaEditVec2f_lerp(t_ofeliaEditVec2f *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec2fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec2f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec2f::vecData[pos].vecFloats.size())-1);
                ofVec2f &vecFloat = t_ofeliaLoadVec2f::vecData[pos].vecFloats[index];
                
                if (argc == 3) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT) {
                        
                        vecFloat.interpolate(ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float), argv[2].a_w.w_float);
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditVec2f::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditVec2f::objName);
                    return;
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec2f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec2f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec2f::objName);
    }
}

void ofeliaEditVec2f_limit(t_ofeliaEditVec2f *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec2fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec2f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec2f::vecData[pos].vecFloats.size())-1);
                ofVec2f &vecFloat = t_ofeliaLoadVec2f::vecData[pos].vecFloats[index];
                vecFloat.limit(f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec2f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec2f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec2f::objName);
    }
}

void ofeliaEditVec2f_map(t_ofeliaEditVec2f *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec2fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec2f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec2f::vecData[pos].vecFloats.size())-1);
                ofVec2f &vecFloat = t_ofeliaLoadVec2f::vecData[pos].vecFloats[index];
                
                if (argc == 6) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT &&
                        argv[3].a_type == A_FLOAT &&
                        argv[4].a_type == A_FLOAT &&
                        argv[5].a_type == A_FLOAT) {
                        
                        vecFloat.map(ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float),
                                     ofVec2f(argv[2].a_w.w_float, argv[3].a_w.w_float),
                                     ofVec2f(argv[4].a_w.w_float, argv[5].a_w.w_float));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditVec2f::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditVec2f::objName);
                    return;
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec2f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec2f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec2f::objName);
    }
}

void ofeliaEditVec2f_middle(t_ofeliaEditVec2f *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec2fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec2f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec2f::vecData[pos].vecFloats.size())-1);
                ofVec2f &vecFloat = t_ofeliaLoadVec2f::vecData[pos].vecFloats[index];
                
                if (argc == 2) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT) {
                        
                        vecFloat.middle(ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditVec2f::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditVec2f::objName);
                    return;
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec2f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec2f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec2f::objName);
    }
}

void ofeliaEditVec2f_normalize(t_ofeliaEditVec2f *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec2fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec2f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec2f::vecData[pos].vecFloats.size())-1);
                ofVec2f &vecFloat = t_ofeliaLoadVec2f::vecData[pos].vecFloats[index];
                vecFloat.normalize();
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec2f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec2f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec2f::objName);
    }
}

void ofeliaEditVec2f_perpendicular(t_ofeliaEditVec2f *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec2fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec2f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec2f::vecData[pos].vecFloats.size())-1);
                ofVec2f &vecFloat = t_ofeliaLoadVec2f::vecData[pos].vecFloats[index];
                vecFloat.perpendicular();
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec2f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec2f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec2f::objName);
    }
}

void ofeliaEditVec2f_rotate(t_ofeliaEditVec2f *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec2fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec2f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec2f::vecData[pos].vecFloats.size())-1);
                ofVec2f &vecFloat = t_ofeliaLoadVec2f::vecData[pos].vecFloats[index];
                
                switch (argc) {
                        
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            vecFloat.rotate(argv[0].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec2f::objName);
                            return;
                        }
                        break;
                    }
                    case 3:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            vecFloat.rotate(argv[0].a_w.w_float, ofVec2f(argv[1].a_w.w_float,
                                                                         argv[2].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec2f::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditVec2f::objName);
                        return;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec2f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec2f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec2f::objName);
    }
}

void ofeliaEditVec2f_rotateRad(t_ofeliaEditVec2f *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec2fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec2f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec2f::vecData[pos].vecFloats.size())-1);
                ofVec2f &vecFloat = t_ofeliaLoadVec2f::vecData[pos].vecFloats[index];
                
                switch (argc) {
                        
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            vecFloat.rotateRad(argv[0].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec2f::objName);
                            return;
                        }
                        break;
                    }
                    case 3:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            vecFloat.rotateRad(argv[0].a_w.w_float, ofVec2f(argv[1].a_w.w_float,
                                                                            argv[2].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec2f::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditVec2f::objName);
                        return;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec2f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec2f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec2f::objName);
    }
}

void ofeliaEditVec2f_scale(t_ofeliaEditVec2f *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec2fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec2f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec2f::vecData[pos].vecFloats.size())-1);
                ofVec2f &vecFloat = t_ofeliaLoadVec2f::vecData[pos].vecFloats[index];
                vecFloat.scale(f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec2f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec2f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec2f::objName);
    }
}

void ofeliaEditVec2f_set(t_ofeliaEditVec2f *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaEditVec2f_print(t_ofeliaEditVec2f *x)
{
    post("\n[%s]", t_ofeliaEditVec2f::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaEditVec2f_setup()
{
    ofeliaEditVec2f_class = class_new(gensym("ofEditVec2f"),
                                      reinterpret_cast<t_newmethod>(ofeliaEditVec2f_new),
                                      0, sizeof(t_ofeliaEditVec2f),
                                      CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaEditVec2f_class, reinterpret_cast<t_method>(ofeliaEditVec2f_float));
    class_addmethod(ofeliaEditVec2f_class, reinterpret_cast<t_method>(ofeliaEditVec2f_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditVec2f_class, reinterpret_cast<t_method>(ofeliaEditVec2f_assign),
                    gensym("assign"), A_GIMME, 0);
    class_addmethod(ofeliaEditVec2f_class, reinterpret_cast<t_method>(ofeliaEditVec2f_assignX),
                    gensym("assignX"), A_FLOAT, 0);
    class_addmethod(ofeliaEditVec2f_class, reinterpret_cast<t_method>(ofeliaEditVec2f_assignY),
                    gensym("assignY"), A_FLOAT, 0);
    class_addmethod(ofeliaEditVec2f_class, reinterpret_cast<t_method>(ofeliaEditVec2f_add),
                    gensym("add"), A_GIMME, 0);
    class_addmethod(ofeliaEditVec2f_class, reinterpret_cast<t_method>(ofeliaEditVec2f_sub),
                    gensym("sub"), A_GIMME, 0);
    class_addmethod(ofeliaEditVec2f_class, reinterpret_cast<t_method>(ofeliaEditVec2f_mult),
                    gensym("mult"), A_GIMME, 0);
    class_addmethod(ofeliaEditVec2f_class, reinterpret_cast<t_method>(ofeliaEditVec2f_div),
                    gensym("div"), A_GIMME, 0);
    class_addmethod(ofeliaEditVec2f_class, reinterpret_cast<t_method>(ofeliaEditVec2f_lerp),
                    gensym("lerp"), A_GIMME, 0);
    class_addmethod(ofeliaEditVec2f_class, reinterpret_cast<t_method>(ofeliaEditVec2f_limit),
                    gensym("limit"), A_FLOAT, 0);
    class_addmethod(ofeliaEditVec2f_class, reinterpret_cast<t_method>(ofeliaEditVec2f_map),
                    gensym("map"), A_GIMME, 0);
    class_addmethod(ofeliaEditVec2f_class, reinterpret_cast<t_method>(ofeliaEditVec2f_middle),
                    gensym("middle"), A_GIMME, 0);
    class_addmethod(ofeliaEditVec2f_class, reinterpret_cast<t_method>(ofeliaEditVec2f_normalize),
                    gensym("normalize"), A_NULL, 0);
    class_addmethod(ofeliaEditVec2f_class, reinterpret_cast<t_method>(ofeliaEditVec2f_perpendicular),
                    gensym("perpendicular"), A_NULL, 0);
    class_addmethod(ofeliaEditVec2f_class, reinterpret_cast<t_method>(ofeliaEditVec2f_rotate),
                    gensym("rotate"), A_GIMME, 0);
    class_addmethod(ofeliaEditVec2f_class, reinterpret_cast<t_method>(ofeliaEditVec2f_rotateRad),
                    gensym("rotateRad"), A_GIMME, 0);
    class_addmethod(ofeliaEditVec2f_class, reinterpret_cast<t_method>(ofeliaEditVec2f_scale),
                    gensym("scale"), A_FLOAT, 0);
    class_addmethod(ofeliaEditVec2f_class, reinterpret_cast<t_method>(ofeliaEditVec2f_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditVec2f_class, reinterpret_cast<t_method>(ofeliaEditVec2f_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofDoesVec2fNameExist object methods
 */
void *ofeliaDoesVec2fNameExist_new(t_symbol *s)
{
    t_ofeliaDoesVec2fNameExist *x = reinterpret_cast<t_ofeliaDoesVec2fNameExist*>(pd_new(ofeliaDoesVec2fNameExist_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaDoesVec2fNameExist_bang(t_ofeliaDoesVec2fNameExist *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        if (getPositionByVec2fName(name) != -1)
            outlet_float(x->x_obj.ob_outlet, 1.0f);
        else
            outlet_float(x->x_obj.ob_outlet, 0.0f);
    }
    else {
        
        error("%s: name not assigned", t_ofeliaDoesVec2fNameExist::objName);
    }
}

void ofeliaDoesVec2fNameExist_set(t_ofeliaDoesVec2fNameExist *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaDoesVec2fNameExist_print(t_ofeliaDoesVec2fNameExist *x)
{
    post("\n[%s]", t_ofeliaDoesVec2fNameExist::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaDoesVec2fNameExist_setup()
{
    ofeliaDoesVec2fNameExist_class = class_new(gensym("ofDoesVec2fNameExist"),
                                               reinterpret_cast<t_newmethod>(ofeliaDoesVec2fNameExist_new),
                                               0, sizeof(t_ofeliaDoesVec2fNameExist),
                                               CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaDoesVec2fNameExist_class, reinterpret_cast<t_method>(ofeliaDoesVec2fNameExist_bang));
    class_addmethod(ofeliaDoesVec2fNameExist_class, reinterpret_cast<t_method>(ofeliaDoesVec2fNameExist_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesVec2fNameExist_class, reinterpret_cast<t_method>(ofeliaDoesVec2fNameExist_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesVec2fNameExist_class, reinterpret_cast<t_method>(ofeliaDoesVec2fNameExist_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetVec2f object methods
 */
void *ofeliaGetVec2f_new(t_symbol *s)
{
    t_ofeliaGetVec2f *x = reinterpret_cast<t_ofeliaGetVec2f*>(pd_new(ofeliaGetVec2f_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetVec2f_bang(t_ofeliaGetVec2f *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec2fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec2f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec2f::vecData[pos].vecFloats.size())-1);
                const ofVec2f &vecFloat = t_ofeliaLoadVec2f::vecData[pos].vecFloats[index];
                t_atom av[2];
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = vecFloat.x;
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = vecFloat.y;
                outlet_list(x->x_obj.ob_outlet, &s_list, 2, av);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetVec2f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetVec2f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetVec2f::objName);
    }
}

void ofeliaGetVec2f_float(t_ofeliaGetVec2f *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetVec2f_set(t_ofeliaGetVec2f *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetVec2f_print(t_ofeliaGetVec2f *x)
{
    post("\n[%s]", t_ofeliaGetVec2f::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetVec2f_setup()
{
    ofeliaGetVec2f_class = class_new(gensym("ofGetVec2f"),
                                     reinterpret_cast<t_newmethod>(ofeliaGetVec2f_new),
                                     0, sizeof(t_ofeliaGetVec2f),
                                     CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetVec2f_class, reinterpret_cast<t_method>(ofeliaGetVec2f_bang));
    class_addfloat(ofeliaGetVec2f_class, reinterpret_cast<t_method>(ofeliaGetVec2f_float));
    class_addmethod(ofeliaGetVec2f_class, reinterpret_cast<t_method>(ofeliaGetVec2f_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec2f_class, reinterpret_cast<t_method>(ofeliaGetVec2f_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec2f_class, reinterpret_cast<t_method>(ofeliaGetVec2f_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetVec2fs object methods
 */
void *ofeliaGetVec2fs_new(t_symbol *s)
{
    t_ofeliaGetVec2fs *x = reinterpret_cast<t_ofeliaGetVec2fs*>(pd_new(ofeliaGetVec2fs_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetVec2fs_bang(t_ofeliaGetVec2fs *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec2fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec2f::vecData[pos].vecFloats.empty()) {
                
                vector<float> vecFloats;
                
                for (size_t i=0; i<t_ofeliaLoadVec2f::vecData[pos].vecFloats.size(); ++i) {
                    
                    vecFloats.push_back(t_ofeliaLoadVec2f::vecData[pos].vecFloats[i].x);
                    vecFloats.push_back(t_ofeliaLoadVec2f::vecData[pos].vecFloats[i].y);
                }
                const int ac = static_cast<int>(vecFloats.size());
                t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
                
                for (int i=0; i<ac; ++i) {
                    
                    av[i].a_type = A_FLOAT;
                    av[i].a_w.w_float = vecFloats[i];
                }
                outlet_list(x->x_obj.ob_outlet, &s_list, ac, av);
                freebytes(av, ac * sizeof(t_atom));
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetVec2fs::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetVec2fs::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetVec2fs::objName);
    }
}

void ofeliaGetVec2fs_set(t_ofeliaGetVec2fs *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetVec2fs_print(t_ofeliaGetVec2fs *x)
{
    post("\n[%s]", t_ofeliaGetVec2fs::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetVec2fs_setup()
{
    ofeliaGetVec2fs_class = class_new(gensym("ofGetVec2fs"),
                                      reinterpret_cast<t_newmethod>(ofeliaGetVec2fs_new),
                                      0, sizeof(t_ofeliaGetVec2fs),
                                      CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetVec2fs_class, reinterpret_cast<t_method>(ofeliaGetVec2fs_bang));
    class_addmethod(ofeliaGetVec2fs_class, reinterpret_cast<t_method>(ofeliaGetVec2fs_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec2fs_class, reinterpret_cast<t_method>(ofeliaGetVec2fs_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec2fs_class, reinterpret_cast<t_method>(ofeliaGetVec2fs_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetVec2fAverage object methods
 */
void *ofeliaGetVec2fAverage_new(t_symbol *s)
{
    t_ofeliaGetVec2fAverage *x = reinterpret_cast<t_ofeliaGetVec2fAverage*>(pd_new(ofeliaGetVec2fAverage_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetVec2fAverage_bang(t_ofeliaGetVec2fAverage *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec2fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec2f::vecData[pos].vecFloats.empty()) {
                
                ofVec2f avg;
                avg.average(t_ofeliaLoadVec2f::vecData[pos].vecFloats.data(),
                            t_ofeliaLoadVec2f::vecData[pos].vecFloats.size());
                t_atom av[2];
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = avg.x;
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = avg.y;
                outlet_list(x->x_obj.ob_outlet, &s_list, 2, av);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetVec2fAverage::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetVec2fAverage::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetVec2fAverage::objName);
    }
}

void ofeliaGetVec2fAverage_set(t_ofeliaGetVec2fAverage *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetVec2fAverage_print(t_ofeliaGetVec2fAverage *x)
{
    post("\n[%s]", t_ofeliaGetVec2fAverage::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetVec2fAverage_setup()
{
    ofeliaGetVec2fAverage_class = class_new(gensym("ofGetVec2fAverage"),
                                            reinterpret_cast<t_newmethod>(ofeliaGetVec2fAverage_new),
                                            0, sizeof(t_ofeliaGetVec2fAverage),
                                            CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetVec2fAverage_class, reinterpret_cast<t_method>(ofeliaGetVec2fAverage_bang));
    class_addmethod(ofeliaGetVec2fAverage_class, reinterpret_cast<t_method>(ofeliaGetVec2fAverage_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec2fAverage_class, reinterpret_cast<t_method>(ofeliaGetVec2fAverage_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec2fAverage_class, reinterpret_cast<t_method>(ofeliaGetVec2fAverage_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetVec2fAngle object methods
 */
void *ofeliaGetVec2fAngle_new(t_symbol *s)
{
    t_ofeliaGetVec2fAngle *x = reinterpret_cast<t_ofeliaGetVec2fAngle*>(pd_new(ofeliaGetVec2fAngle_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetVec2fAngle_float(t_ofeliaGetVec2fAngle *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetVec2fAngle_list(t_ofeliaGetVec2fAngle *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec2fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec2f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec2f::vecData[pos].vecFloats.size())-1);
                const ofVec2f &vecFloat = t_ofeliaLoadVec2f::vecData[pos].vecFloats[index];
                
                if (argc >= 2) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT) {
                        
                        outlet_float(x->x_obj.ob_outlet, vecFloat.angle(ofVec2f(argv[0].a_w.w_float,
                                                                                argv[1].a_w.w_float)));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaGetVec2fAngle::objName);
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaGetVec2fAngle::objName);
                }
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetVec2fAngle::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetVec2fAngle::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetVec2fAngle::objName);
    }
}

void ofeliaGetVec2fAngle_set(t_ofeliaGetVec2fAngle *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetVec2fAngle_print(t_ofeliaGetVec2fAngle *x)
{
    post("\n[%s]", t_ofeliaGetVec2fAngle::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetVec2fAngle_setup()
{
    ofeliaGetVec2fAngle_class = class_new(gensym("ofGetVec2fAngle"),
                                          reinterpret_cast<t_newmethod>(ofeliaGetVec2fAngle_new),
                                          0, sizeof(t_ofeliaGetVec2fAngle),
                                          CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaGetVec2fAngle_class, reinterpret_cast<t_method>(ofeliaGetVec2fAngle_float));
    class_addlist(ofeliaGetVec2fAngle_class, reinterpret_cast<t_method>(ofeliaGetVec2fAngle_list));
    class_addmethod(ofeliaGetVec2fAngle_class, reinterpret_cast<t_method>(ofeliaGetVec2fAngle_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec2fAngle_class, reinterpret_cast<t_method>(ofeliaGetVec2fAngle_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec2fAngle_class, reinterpret_cast<t_method>(ofeliaGetVec2fAngle_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetVec2fAngleRad object methods
 */
void *ofeliaGetVec2fAngleRad_new(t_symbol *s)
{
    t_ofeliaGetVec2fAngleRad *x = reinterpret_cast<t_ofeliaGetVec2fAngleRad*>(pd_new(ofeliaGetVec2fAngleRad_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetVec2fAngleRad_float(t_ofeliaGetVec2fAngleRad *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetVec2fAngleRad_list(t_ofeliaGetVec2fAngleRad *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec2fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec2f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec2f::vecData[pos].vecFloats.size())-1);
                const ofVec2f &vecFloat = t_ofeliaLoadVec2f::vecData[pos].vecFloats[index];
                
                if (argc >= 2) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT) {
                        
                        outlet_float(x->x_obj.ob_outlet, vecFloat.angleRad(ofVec2f(argv[0].a_w.w_float,
                                                                                   argv[1].a_w.w_float)));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaGetVec2fAngleRad::objName);
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaGetVec2fAngleRad::objName);
                }
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetVec2fAngleRad::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetVec2fAngleRad::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetVec2fAngleRad::objName);
    }
}

void ofeliaGetVec2fAngleRad_set(t_ofeliaGetVec2fAngleRad *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetVec2fAngleRad_print(t_ofeliaGetVec2fAngleRad *x)
{
    post("\n[%s]", t_ofeliaGetVec2fAngleRad::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetVec2fAngleRad_setup()
{
    ofeliaGetVec2fAngleRad_class = class_new(gensym("ofGetVec2fAngleRad"),
                                             reinterpret_cast<t_newmethod>(ofeliaGetVec2fAngleRad_new),
                                             0, sizeof(t_ofeliaGetVec2fAngleRad),
                                             CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaGetVec2fAngleRad_class, reinterpret_cast<t_method>(ofeliaGetVec2fAngleRad_float));
    class_addlist(ofeliaGetVec2fAngleRad_class, reinterpret_cast<t_method>(ofeliaGetVec2fAngleRad_list));
    class_addmethod(ofeliaGetVec2fAngleRad_class, reinterpret_cast<t_method>(ofeliaGetVec2fAngleRad_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec2fAngleRad_class, reinterpret_cast<t_method>(ofeliaGetVec2fAngleRad_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec2fAngleRad_class, reinterpret_cast<t_method>(ofeliaGetVec2fAngleRad_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetVec2fDist object methods
 */
void *ofeliaGetVec2fDist_new(t_symbol *s)
{
    t_ofeliaGetVec2fDist *x = reinterpret_cast<t_ofeliaGetVec2fDist*>(pd_new(ofeliaGetVec2fDist_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetVec2fDist_float(t_ofeliaGetVec2fDist *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetVec2fDist_list(t_ofeliaGetVec2fDist *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec2fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec2f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec2f::vecData[pos].vecFloats.size())-1);
                const ofVec2f &vecFloat = t_ofeliaLoadVec2f::vecData[pos].vecFloats[index];
                
                if (argc >= 2) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT) {
                        
                        outlet_float(x->x_obj.ob_outlet, vecFloat.distance(ofVec2f(argv[0].a_w.w_float,
                                                                                   argv[1].a_w.w_float)));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaGetVec2fDist::objName);
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaGetVec2fDist::objName);
                }
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetVec2fDist::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetVec2fDist::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetVec2fDist::objName);
    }
}

void ofeliaGetVec2fDist_set(t_ofeliaGetVec2fDist *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetVec2fDist_print(t_ofeliaGetVec2fDist *x)
{
    post("\n[%s]", t_ofeliaGetVec2fDist::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetVec2fDist_setup()
{
    ofeliaGetVec2fDist_class = class_new(gensym("ofGetVec2fDist"),
                                         reinterpret_cast<t_newmethod>(ofeliaGetVec2fDist_new),
                                         0, sizeof(t_ofeliaGetVec2fDist),
                                         CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaGetVec2fDist_class, reinterpret_cast<t_method>(ofeliaGetVec2fDist_float));
    class_addlist(ofeliaGetVec2fDist_class, reinterpret_cast<t_method>(ofeliaGetVec2fDist_list));
    class_addmethod(ofeliaGetVec2fDist_class, reinterpret_cast<t_method>(ofeliaGetVec2fDist_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec2fDist_class, reinterpret_cast<t_method>(ofeliaGetVec2fDist_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec2fDist_class, reinterpret_cast<t_method>(ofeliaGetVec2fDist_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetVec2fDistSquared object methods
 */
void *ofeliaGetVec2fDistSquared_new(t_symbol *s)
{
    t_ofeliaGetVec2fDistSquared *x = reinterpret_cast<t_ofeliaGetVec2fDistSquared*>(pd_new(ofeliaGetVec2fDistSquared_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetVec2fDistSquared_float(t_ofeliaGetVec2fDistSquared *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetVec2fDistSquared_list(t_ofeliaGetVec2fDistSquared *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec2fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec2f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec2f::vecData[pos].vecFloats.size())-1);
                const ofVec2f &vecFloat = t_ofeliaLoadVec2f::vecData[pos].vecFloats[index];
                
                if (argc >= 2) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT) {
                        
                        outlet_float(x->x_obj.ob_outlet, vecFloat.squareDistance(ofVec2f(argv[0].a_w.w_float,
                                                                                         argv[1].a_w.w_float)));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaGetVec2fDistSquared::objName);
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaGetVec2fDistSquared::objName);
                }
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetVec2fDistSquared::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetVec2fDistSquared::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetVec2fDistSquared::objName);
    }
}

void ofeliaGetVec2fDistSquared_set(t_ofeliaGetVec2fDistSquared *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetVec2fDistSquared_print(t_ofeliaGetVec2fDistSquared *x)
{
    post("\n[%s]", t_ofeliaGetVec2fDistSquared::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetVec2fDistSquared_setup()
{
    ofeliaGetVec2fDistSquared_class = class_new(gensym("ofGetVec2fDistSquared"),
                                                reinterpret_cast<t_newmethod>(ofeliaGetVec2fDistSquared_new),
                                                0, sizeof(t_ofeliaGetVec2fDistSquared),
                                                CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaGetVec2fDistSquared_class, reinterpret_cast<t_method>(ofeliaGetVec2fDistSquared_float));
    class_addlist(ofeliaGetVec2fDistSquared_class, reinterpret_cast<t_method>(ofeliaGetVec2fDistSquared_list));
    class_addmethod(ofeliaGetVec2fDistSquared_class, reinterpret_cast<t_method>(ofeliaGetVec2fDistSquared_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec2fDistSquared_class, reinterpret_cast<t_method>(ofeliaGetVec2fDistSquared_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec2fDistSquared_class, reinterpret_cast<t_method>(ofeliaGetVec2fDistSquared_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetVec2fDot object methods
 */
void *ofeliaGetVec2fDot_new(t_symbol *s)
{
    t_ofeliaGetVec2fDot *x = reinterpret_cast<t_ofeliaGetVec2fDot*>(pd_new(ofeliaGetVec2fDot_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetVec2fDot_float(t_ofeliaGetVec2fDot *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetVec2fDot_list(t_ofeliaGetVec2fDot *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec2fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec2f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec2f::vecData[pos].vecFloats.size())-1);
                const ofVec2f &vecFloat = t_ofeliaLoadVec2f::vecData[pos].vecFloats[index];
                
                if (argc >= 2) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT) {
                        
                        outlet_float(x->x_obj.ob_outlet, vecFloat.dot(ofVec2f(argv[0].a_w.w_float,
                                                                              argv[1].a_w.w_float)));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaGetVec2fDot::objName);
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaGetVec2fDot::objName);
                }
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetVec2fDot::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetVec2fDot::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetVec2fDot::objName);
    }
}

void ofeliaGetVec2fDot_set(t_ofeliaGetVec2fDot *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetVec2fDot_print(t_ofeliaGetVec2fDot *x)
{
    post("\n[%s]", t_ofeliaGetVec2fDot::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetVec2fDot_setup()
{
    ofeliaGetVec2fDot_class = class_new(gensym("ofGetVec2fDot"),
                                        reinterpret_cast<t_newmethod>(ofeliaGetVec2fDot_new),
                                        0, sizeof(t_ofeliaGetVec2fDot),
                                        CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaGetVec2fDot_class, reinterpret_cast<t_method>(ofeliaGetVec2fDot_float));
    class_addlist(ofeliaGetVec2fDot_class, reinterpret_cast<t_method>(ofeliaGetVec2fDot_list));
    class_addmethod(ofeliaGetVec2fDot_class, reinterpret_cast<t_method>(ofeliaGetVec2fDot_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec2fDot_class, reinterpret_cast<t_method>(ofeliaGetVec2fDot_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec2fDot_class, reinterpret_cast<t_method>(ofeliaGetVec2fDot_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetVec2fLength object methods
 */
void *ofeliaGetVec2fLength_new(t_symbol *s)
{
    t_ofeliaGetVec2fLength *x = reinterpret_cast<t_ofeliaGetVec2fLength*>(pd_new(ofeliaGetVec2fLength_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetVec2fLength_bang(t_ofeliaGetVec2fLength *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec2fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec2f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec2f::vecData[pos].vecFloats.size())-1);
                const ofVec2f &vecFloat = t_ofeliaLoadVec2f::vecData[pos].vecFloats[index];
                outlet_float(x->x_obj.ob_outlet, vecFloat.length());
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetVec2fLength::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetVec2fLength::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetVec2fLength::objName);
    }
}

void ofeliaGetVec2fLength_float(t_ofeliaGetVec2fLength *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetVec2fLength_set(t_ofeliaGetVec2fLength *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetVec2fLength_print(t_ofeliaGetVec2fLength *x)
{
    post("\n[%s]", t_ofeliaGetVec2fLength::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetVec2fLength_setup()
{
    ofeliaGetVec2fLength_class = class_new(gensym("ofGetVec2fLength"),
                                           reinterpret_cast<t_newmethod>(ofeliaGetVec2fLength_new),
                                           0, sizeof(t_ofeliaGetVec2fLength),
                                           CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetVec2fLength_class, reinterpret_cast<t_method>(ofeliaGetVec2fLength_bang));
    class_addfloat(ofeliaGetVec2fLength_class, reinterpret_cast<t_method>(ofeliaGetVec2fLength_float));
    class_addmethod(ofeliaGetVec2fLength_class, reinterpret_cast<t_method>(ofeliaGetVec2fLength_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec2fLength_class, reinterpret_cast<t_method>(ofeliaGetVec2fLength_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec2fLength_class, reinterpret_cast<t_method>(ofeliaGetVec2fLength_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetVec2fLengthSquared object methods
 */
void *ofeliaGetVec2fLengthSquared_new(t_symbol *s)
{
    t_ofeliaGetVec2fLengthSquared *x = reinterpret_cast<t_ofeliaGetVec2fLengthSquared*>(pd_new(ofeliaGetVec2fLengthSquared_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetVec2fLengthSquared_bang(t_ofeliaGetVec2fLengthSquared *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec2fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec2f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec2f::vecData[pos].vecFloats.size())-1);
                const ofVec2f &vecFloat = t_ofeliaLoadVec2f::vecData[pos].vecFloats[index];
                outlet_float(x->x_obj.ob_outlet, vecFloat.lengthSquared());
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetVec2fLengthSquared::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetVec2fLengthSquared::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetVec2fLengthSquared::objName);
    }
}

void ofeliaGetVec2fLengthSquared_float(t_ofeliaGetVec2fLengthSquared *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));;
}

void ofeliaGetVec2fLengthSquared_set(t_ofeliaGetVec2fLengthSquared *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetVec2fLengthSquared_print(t_ofeliaGetVec2fLengthSquared *x)
{
    post("\n[%s]", t_ofeliaGetVec2fLengthSquared::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetVec2fLengthSquared_setup()
{
    ofeliaGetVec2fLengthSquared_class = class_new(gensym("ofGetVec2fLengthSquared"),
                                                  reinterpret_cast<t_newmethod>(ofeliaGetVec2fLengthSquared_new),
                                                  0, sizeof(t_ofeliaGetVec2fLengthSquared),
                                                  CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetVec2fLengthSquared_class, reinterpret_cast<t_method>(ofeliaGetVec2fLengthSquared_bang));
    class_addfloat(ofeliaGetVec2fLengthSquared_class, reinterpret_cast<t_method>(ofeliaGetVec2fLengthSquared_float));
    class_addmethod(ofeliaGetVec2fLengthSquared_class, reinterpret_cast<t_method>(ofeliaGetVec2fLengthSquared_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec2fLengthSquared_class, reinterpret_cast<t_method>(ofeliaGetVec2fLengthSquared_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec2fLengthSquared_class, reinterpret_cast<t_method>(ofeliaGetVec2fLengthSquared_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * setup methods
 */
void ofeliaVec2f_setup()
{
    ofeliaLoadVec2f_setup();
    ofeliaEditVec2f_setup();
    ofeliaDoesVec2fNameExist_setup();
    ofeliaGetVec2f_setup();
    ofeliaGetVec2fs_setup();
    ofeliaGetVec2fAverage_setup();
    ofeliaGetVec2fAngle_setup();
    ofeliaGetVec2fAngleRad_setup();
    ofeliaGetVec2fDist_setup();
    ofeliaGetVec2fDistSquared_setup();
    ofeliaGetVec2fDot_setup();
    ofeliaGetVec2fLength_setup();
    ofeliaGetVec2fLengthSquared_setup();
}

