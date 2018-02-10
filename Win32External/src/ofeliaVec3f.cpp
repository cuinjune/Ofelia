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

#include "ofeliaVec3f.h"

/* ________________________________________________________________________________
 * static variables
 */
const char *t_ofeliaLoadVec3f::objName = "ofLoadVec3f";
unsigned int t_ofeliaLoadVec3f::counter;
vector<t_ofeliaLoadVec3fData> t_ofeliaLoadVec3f::vecData;
const char *t_ofeliaEditVec3f::objName = "ofEditVec3f";
const char *t_ofeliaDoesVec3fNameExist::objName = "ofDoesVec3fNameExist";
const char *t_ofeliaGetVec3f::objName = "ofGetVec3f";
const char *t_ofeliaGetVec3fs::objName = "ofGetVec3fs";
const char *t_ofeliaGetVec3fAverage::objName = "ofGetVec3fAverage";
const char *t_ofeliaGetVec3fAngle::objName = "ofGetVec3fAngle";
const char *t_ofeliaGetVec3fAngleRad::objName = "ofGetVec3fAngleRad";
const char *t_ofeliaGetVec3fDist::objName = "ofGetVec3fDist";
const char *t_ofeliaGetVec3fDistSquared::objName = "ofGetVec3fDistSquared";
const char *t_ofeliaGetVec3fDot::objName = "ofGetVec3fDot";
const char *t_ofeliaGetVec3fLength::objName = "ofGetVec3fLength";
const char *t_ofeliaGetVec3fLengthSquared::objName = "ofGetVec3fLengthSquared";

/* ________________________________________________________________________________
 * ofLoadVec3f object methods
 */
void ofeliaLoadVec3f_vecSizeOut(t_ofeliaLoadVec3f *x);

int getPositionByVec3fObjID(const unsigned int objID)
{
    const auto it = find_if(begin(t_ofeliaLoadVec3f::vecData), end(t_ofeliaLoadVec3f::vecData),
                            [&objID](const t_ofeliaLoadVec3fData &vecData)
                            {
                                return vecData.objID == objID;
                            });
    return static_cast<int>(it - t_ofeliaLoadVec3f::vecData.begin());
}

bool getLoadVec3fFloatFromArgs(const int argc, const t_atom *argv, ofVec3f &vecFloat)
{
    if (argc) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            vecFloat.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadVec3f::objName);
            return 0;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadVec3f::objName);
        return 0;
    }
    return 1;
}

bool getLoadVec3fFloatsFromArgs(const int argc, const t_atom *argv, vector<ofVec3f> &vecFloats)
{
    if (argc && argc%3 == 0) {
        
        for (int i=0; i<argc; i+=3) {
            
            if (argv[i].a_type == A_FLOAT &&
                argv[i+1].a_type == A_FLOAT &&
                argv[i+2].a_type == A_FLOAT) {
                
                vecFloats.push_back(ofVec3f(argv[i].a_w.w_float, argv[i+1].a_w.w_float, argv[i+2].a_w.w_float));
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaLoadVec3f::objName);
                return 0;
            }
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadVec3f::objName);
        return 0;
    }
    return 1;
}

bool getLoadVec3fCmdRangeFromArgs(const int argc, const t_atom *argv, t_ofeliaLoadVec3fCmdData &cmd)
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
                
                error("%s: wrong argument type", t_ofeliaLoadVec3f::objName);
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
                
                error("%s: wrong argument type", t_ofeliaLoadVec3f::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaLoadVec3f::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaLoadVec3f_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadVec3f *x = reinterpret_cast<t_ofeliaLoadVec3f*>(pd_new(ofeliaLoadVec3f_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    t_ofeliaLoadVec3fData vecData;
    getVarNameLocalPrefixes(vecData.varName);
    vector<ofVec3f> vecFloats;
    
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            vecData.varName.name = argv[0].a_w.w_symbol->s_name;
            getVarNameLocalized(vecData.varName);
            
            if (--argc) {
                
                if (!getLoadVec3fFloatsFromArgs(argc, argv+1, vecFloats))
                    return 0;
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadVec3f::objName);
            return 0;
        }
    }
    x->objID = vecData.objID = t_ofeliaLoadVec3f::counter++;
    t_ofeliaLoadVec3f::vecData.push_back(vecData);
    x->vecSize = 0;
    x->vecSizeOutClock = clock_new(x, reinterpret_cast<t_method>(ofeliaLoadVec3f_vecSizeOut));
    outlet_new(&x->x_obj, &s_float);
    
    if (argc) {
        
        const int pos = getPositionByVec3fObjID(x->objID);
        t_ofeliaLoadVec3f::vecData[pos].vecFloats = vecFloats;
        x->vecSize = static_cast<int>(vecFloats.size());
        clock_delay(x->vecSizeOutClock, 0.0);
    }
    return (x);
}

void ofeliaLoadVec3f_vecSizeOut(t_ofeliaLoadVec3f *x)
{
    outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(x->vecSize));
}

void ofeliaLoadVec3f_name(t_ofeliaLoadVec3f *x, t_symbol *s)
{    
    const int pos = getPositionByVec3fObjID(x->objID);
    t_ofeliaLoadVec3f::vecData[pos].varName.name = s->s_name;
    getVarNameLocalized(t_ofeliaLoadVec3f::vecData[pos].varName);
}

void ofeliaLoadVec3f_load(t_ofeliaLoadVec3f *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<ofVec3f> vecFloats;
    
    if (getLoadVec3fFloatsFromArgs(argc, argv, vecFloats)) {
        
        const int pos = getPositionByVec3fObjID(x->objID);
        t_ofeliaLoadVec3f::vecData[pos].vecFloats = vecFloats;
        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(vecFloats.size()));
    }
}

void ofeliaLoadVec3f_add(t_ofeliaLoadVec3f *x, t_symbol *s, int argc, t_atom *argv)
{
    ofVec3f vecFloat;
    
    if (getLoadVec3fFloatFromArgs(argc, argv, vecFloat)) {
        
        const int pos = getPositionByVec3fObjID(x->objID);
        t_ofeliaLoadVec3f::vecData[pos].vecFloats.push_back(vecFloat);
        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size()));
    }
}

void ofeliaLoadVec3f_append(t_ofeliaLoadVec3f *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<ofVec3f> vecFloats;
    
    if (getLoadVec3fFloatsFromArgs(argc, argv, vecFloats)) {
        
        const int pos = getPositionByVec3fObjID(x->objID);
        t_ofeliaLoadVec3f::vecData[pos].vecFloats.insert(t_ofeliaLoadVec3f::vecData[pos].vecFloats.end(),
                                                         vecFloats.begin(), vecFloats.end());
        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size()));
    }
}

void ofeliaLoadVec3f_prepend(t_ofeliaLoadVec3f *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<ofVec3f> vecFloats;
    
    if (getLoadVec3fFloatsFromArgs(argc, argv, vecFloats)) {
        
        const int pos = getPositionByVec3fObjID(x->objID);
        t_ofeliaLoadVec3f::vecData[pos].vecFloats.insert(t_ofeliaLoadVec3f::vecData[pos].vecFloats.begin(),
                                                         vecFloats.begin(), vecFloats.end());
        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size()));
    }
}

void ofeliaLoadVec3f_insert(t_ofeliaLoadVec3f *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadVec3fCmdData cmd;
    
    if (getLoadVec3fFloatFromArgs(argc, argv, cmd.vecFloat)) {
        
        if (getLoadVec3fCmdRangeFromArgs(argc-3, argv+3, cmd)) {
            
            const int pos = getPositionByVec3fObjID(x->objID);
            const int size = static_cast<int>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size());
            const auto vecFloatsBegin = t_ofeliaLoadVec3f::vecData[pos].vecFloats.begin();
            const int fromIndex = static_cast<int>(ofClamp(cmd.fromIndex, 0.0f, size));
            const int insertSize = static_cast<int>(max(0.0f, max(0.0f, cmd.toIndex) - max(0.0f, cmd.fromIndex))) + 1;
            t_ofeliaLoadVec3f::vecData[pos].vecFloats.insert(vecFloatsBegin + fromIndex,
                                                             insertSize, cmd.vecFloat);
            outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size()));
        }
    }
}

void ofeliaLoadVec3f_fill(t_ofeliaLoadVec3f *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadVec3fCmdData cmd;
    
    if (getLoadVec3fFloatFromArgs(argc, argv, cmd.vecFloat)) {
        
        if (getLoadVec3fCmdRangeFromArgs(argc-3, argv+3, cmd)) {
            
            const int pos = getPositionByVec3fObjID(x->objID);
            const int size = static_cast<int>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size());
            const auto vecFloatsBegin = t_ofeliaLoadVec3f::vecData[pos].vecFloats.begin();
            
            if (size) {
                
                const int maxIndex = size - 1;
                const int fromIndex = static_cast<int>(ofClamp(cmd.fromIndex, 0.0f, maxIndex));
                const int toIndex = static_cast<int>(ofClamp(cmd.toIndex, fromIndex, maxIndex)) + 1;
                fill(vecFloatsBegin + fromIndex, vecFloatsBegin + toIndex, cmd.vecFloat);
                outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size()));
            }
            else {
                
                outlet_float(x->x_obj.ob_outlet, 0.0f);
            }
        }
    }
}

void ofeliaLoadVec3f_erase(t_ofeliaLoadVec3f *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadVec3fCmdData cmd;
    
    if (getLoadVec3fCmdRangeFromArgs(argc, argv, cmd)) {
        
        const int pos = getPositionByVec3fObjID(x->objID);
        const int size = static_cast<int>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size());
        const auto vecFloatsBegin = t_ofeliaLoadVec3f::vecData[pos].vecFloats.begin();
        
        if (size) {
            
            const int maxIndex = size - 1;
            const int fromIndex = static_cast<int>(ofClamp(cmd.fromIndex, 0.0f, maxIndex));
            const int toIndex = static_cast<int>(ofClamp(cmd.toIndex, fromIndex, maxIndex)) + 1;
            t_ofeliaLoadVec3f::vecData[pos].vecFloats.erase(vecFloatsBegin + fromIndex,
                                                            vecFloatsBegin + toIndex);
            outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size()));
        }
        else {
            
            outlet_float(x->x_obj.ob_outlet, 0.0f);
        }
    }
}

void ofeliaLoadVec3f_clear(t_ofeliaLoadVec3f *x)
{
    const int pos = getPositionByVec3fObjID(x->objID);
    t_ofeliaLoadVec3f::vecData[pos].vecFloats.clear();
    outlet_float(x->x_obj.ob_outlet, 0.0f);
}

void ofeliaLoadVec3f_set(t_ofeliaLoadVec3f *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            ofeliaLoadVec3f_name(x, argv[0].a_w.w_symbol);
            
            if (--argc)
                ofeliaLoadVec3f_load(x, 0, argc, argv+1);
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadVec3f::objName);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadVec3f::objName);
    }
}

void ofeliaLoadVec3f_print(t_ofeliaLoadVec3f *x)
{
    post("\n[%s]", t_ofeliaLoadVec3f::objName);
    const int pos = getPositionByVec3fObjID(x->objID);
    post("name : %s", t_ofeliaLoadVec3f::vecData[pos].varName.name.c_str());
    post("load : ");
    
    for (size_t i=0; i<t_ofeliaLoadVec3f::vecData[pos].vecFloats.size(); ++i) {
        
        post(" %g %g %g", t_ofeliaLoadVec3f::vecData[pos].vecFloats[i].x, t_ofeliaLoadVec3f::vecData[pos].vecFloats[i].y, t_ofeliaLoadVec3f::vecData[pos].vecFloats[i].z);
    }
}

void ofeliaLoadVec3f_free(t_ofeliaLoadVec3f *x)
{
    clock_free(x->vecSizeOutClock);
    const int pos = getPositionByVec3fObjID(x->objID);
    t_ofeliaLoadVec3f::vecData.erase(t_ofeliaLoadVec3f::vecData.begin() + pos);
}

void ofeliaLoadVec3f_setup()
{
    ofeliaLoadVec3f_class = class_new(gensym("ofLoadVec3f"),
                                      reinterpret_cast<t_newmethod>(ofeliaLoadVec3f_new),
                                      reinterpret_cast<t_method>(ofeliaLoadVec3f_free),
                                      sizeof(t_ofeliaLoadVec3f),
                                      CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaLoadVec3f_class, reinterpret_cast<t_method>(ofeliaLoadVec3f_name),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaLoadVec3f_class, reinterpret_cast<t_method>(ofeliaLoadVec3f_load),
                    gensym("load"), A_GIMME, 0);
    class_addmethod(ofeliaLoadVec3f_class, reinterpret_cast<t_method>(ofeliaLoadVec3f_add),
                    gensym("add"), A_GIMME, 0);
    class_addmethod(ofeliaLoadVec3f_class, reinterpret_cast<t_method>(ofeliaLoadVec3f_append),
                    gensym("append"), A_GIMME, 0);
    class_addmethod(ofeliaLoadVec3f_class, reinterpret_cast<t_method>(ofeliaLoadVec3f_prepend),
                    gensym("prepend"), A_GIMME, 0);
    class_addmethod(ofeliaLoadVec3f_class, reinterpret_cast<t_method>(ofeliaLoadVec3f_insert),
                    gensym("insert"), A_GIMME, 0);
    class_addmethod(ofeliaLoadVec3f_class, reinterpret_cast<t_method>(ofeliaLoadVec3f_fill),
                    gensym("fill"), A_GIMME, 0);
    class_addmethod(ofeliaLoadVec3f_class, reinterpret_cast<t_method>(ofeliaLoadVec3f_erase),
                    gensym("erase"), A_GIMME, 0);
    class_addmethod(ofeliaLoadVec3f_class, reinterpret_cast<t_method>(ofeliaLoadVec3f_clear),
                    gensym("clear"), A_NULL, 0);
    class_addmethod(ofeliaLoadVec3f_class, reinterpret_cast<t_method>(ofeliaLoadVec3f_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaLoadVec3f_class, reinterpret_cast<t_method>(ofeliaLoadVec3f_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * common methods
 */
int getPositionByVec3fName(const t_string &name)
{
    auto it = find_if(begin(t_ofeliaLoadVec3f::vecData), end(t_ofeliaLoadVec3f::vecData),
                      [&name] (const t_ofeliaLoadVec3fData &vecData) {
                          return (vecData.varName.name == name);
                      });
    if (it == end(t_ofeliaLoadVec3f::vecData))
        return -1;
    return static_cast<int>(it - begin(t_ofeliaLoadVec3f::vecData));
}

/* ________________________________________________________________________________
 * ofEditVec3f object methods
 */
void *ofeliaEditVec3f_new(t_symbol *s)
{
    t_ofeliaEditVec3f *x = reinterpret_cast<t_ofeliaEditVec3f*>(pd_new(ofeliaEditVec3f_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaEditVec3f_float(t_ofeliaEditVec3f *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaEditVec3f_assign(t_ofeliaEditVec3f *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec3fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec3f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size())-1);
                ofVec3f &vecFloat = t_ofeliaLoadVec3f::vecData[pos].vecFloats[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditVec3f::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            vecFloat = ofVec3f(argv[0].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec3f::objName);
                            return;
                        }
                        break;
                    }
                    case 2:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            vecFloat = ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec3f::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            vecFloat = ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec3f::objName);
                            return;
                        }
                        break;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec3f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec3f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec3f::objName);
    }
}

void ofeliaEditVec3f_assignX(t_ofeliaEditVec3f *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec3fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec3f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size())-1);
                ofVec3f &vecFloat = t_ofeliaLoadVec3f::vecData[pos].vecFloats[index];
                vecFloat.x = f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec3f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec3f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec3f::objName);
    }
}

void ofeliaEditVec3f_assignY(t_ofeliaEditVec3f *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec3fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec3f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size())-1);
                ofVec3f &vecFloat = t_ofeliaLoadVec3f::vecData[pos].vecFloats[index];
                vecFloat.y = f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec3f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec3f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec3f::objName);
    }
}

void ofeliaEditVec3f_assignZ(t_ofeliaEditVec3f *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec3fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec3f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size())-1);
                ofVec3f &vecFloat = t_ofeliaLoadVec3f::vecData[pos].vecFloats[index];
                vecFloat.z = f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec3f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec3f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec3f::objName);
    }
}

void ofeliaEditVec3f_add(t_ofeliaEditVec3f *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec3fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec3f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size())-1);
                ofVec3f &vecFloat = t_ofeliaLoadVec3f::vecData[pos].vecFloats[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditVec3f::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            vecFloat += argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec3f::objName);
                            return;
                        }
                        break;
                    }
                    case 2:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            vecFloat += ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec3f::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            vecFloat += ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec3f::objName);
                            return;
                        }
                        break;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec3f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec3f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec3f::objName);
    }
}

void ofeliaEditVec3f_sub(t_ofeliaEditVec3f *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec3fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec3f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size())-1);
                ofVec3f &vecFloat = t_ofeliaLoadVec3f::vecData[pos].vecFloats[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditVec3f::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            vecFloat -= argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec3f::objName);
                            return;
                        }
                        break;
                    }
                    case 2:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            vecFloat -= ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec3f::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            vecFloat -= ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec3f::objName);
                            return;
                        }
                        break;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec3f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec3f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec3f::objName);
    }
}

void ofeliaEditVec3f_mult(t_ofeliaEditVec3f *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec3fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec3f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size())-1);
                ofVec3f &vecFloat = t_ofeliaLoadVec3f::vecData[pos].vecFloats[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditVec3f::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            vecFloat *= argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec3f::objName);
                            return;
                        }
                        break;
                    }
                    case 2:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            vecFloat *= ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec3f::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            vecFloat *= ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec3f::objName);
                            return;
                        }
                        break;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec3f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec3f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec3f::objName);
    }
}

void ofeliaEditVec3f_div(t_ofeliaEditVec3f *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec3fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec3f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size())-1);
                ofVec3f &vecFloat = t_ofeliaLoadVec3f::vecData[pos].vecFloats[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditVec3f::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            vecFloat /= argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec3f::objName);
                            return;
                        }
                        break;
                    }
                    case 2:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            vecFloat /= ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec3f::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            vecFloat /= ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec3f::objName);
                            return;
                        }
                        break;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec3f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec3f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec3f::objName);
    }
}

void ofeliaEditVec3f_cross(t_ofeliaEditVec3f *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec3fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec3f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size())-1);
                ofVec3f &vecFloat = t_ofeliaLoadVec3f::vecData[pos].vecFloats[index];
                
                if (argc == 3) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT) {
                        
                        vecFloat.cross(ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditVec3f::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditVec3f::objName);
                    return;
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec3f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec3f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec3f::objName);
    }
}

void ofeliaEditVec3f_lerp(t_ofeliaEditVec3f *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec3fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec3f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size())-1);
                ofVec3f &vecFloat = t_ofeliaLoadVec3f::vecData[pos].vecFloats[index];
                
                if (argc == 4) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT &&
                        argv[3].a_type == A_FLOAT) {
                        
                        vecFloat.interpolate(ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float), argv[3].a_w.w_float);
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditVec3f::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditVec3f::objName);
                    return;
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec3f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec3f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec3f::objName);
    }
}

void ofeliaEditVec3f_limit(t_ofeliaEditVec3f *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec3fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec3f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size())-1);
                ofVec3f &vecFloat = t_ofeliaLoadVec3f::vecData[pos].vecFloats[index];
                vecFloat.limit(f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec3f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec3f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec3f::objName);
    }
}

void ofeliaEditVec3f_map(t_ofeliaEditVec3f *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec3fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec3f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size())-1);
                ofVec3f &vecFloat = t_ofeliaLoadVec3f::vecData[pos].vecFloats[index];
                
                if (argc == 12) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT &&
                        argv[3].a_type == A_FLOAT &&
                        argv[4].a_type == A_FLOAT &&
                        argv[5].a_type == A_FLOAT &&
                        argv[6].a_type == A_FLOAT &&
                        argv[7].a_type == A_FLOAT &&
                        argv[8].a_type == A_FLOAT &&
                        argv[9].a_type == A_FLOAT &&
                        argv[10].a_type == A_FLOAT &&
                        argv[11].a_type == A_FLOAT) {
                        
                        vecFloat.map(ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float),
                                     ofVec3f(argv[3].a_w.w_float, argv[4].a_w.w_float, argv[5].a_w.w_float),
                                     ofVec3f(argv[6].a_w.w_float, argv[7].a_w.w_float, argv[8].a_w.w_float),
                                     ofVec3f(argv[9].a_w.w_float, argv[10].a_w.w_float, argv[11].a_w.w_float));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditVec3f::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditVec3f::objName);
                    return;
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec3f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec3f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec3f::objName);
    }
}

void ofeliaEditVec3f_middle(t_ofeliaEditVec3f *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec3fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec3f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size())-1);
                ofVec3f &vecFloat = t_ofeliaLoadVec3f::vecData[pos].vecFloats[index];
                
                if (argc == 3) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT) {
                        
                        vecFloat.middle(ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditVec3f::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditVec3f::objName);
                    return;
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec3f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec3f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec3f::objName);
    }
}

void ofeliaEditVec3f_normalize(t_ofeliaEditVec3f *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec3fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec3f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size())-1);
                ofVec3f &vecFloat = t_ofeliaLoadVec3f::vecData[pos].vecFloats[index];
                vecFloat.normalize();
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec3f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec3f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec3f::objName);
    }
}

void ofeliaEditVec3f_perpendicular(t_ofeliaEditVec3f *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec3fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec3f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size())-1);
                ofVec3f &vecFloat = t_ofeliaLoadVec3f::vecData[pos].vecFloats[index];
                
                if (argc == 3) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT) {
                        
                        vecFloat.perpendicular(ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditVec3f::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditVec3f::objName);
                    return;
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec3f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec3f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec3f::objName);
    }
}

void ofeliaEditVec3f_rotate(t_ofeliaEditVec3f *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec3fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec3f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size())-1);
                ofVec3f &vecFloat = t_ofeliaLoadVec3f::vecData[pos].vecFloats[index];
                
                switch (argc) {
                        
                    case 3:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            vecFloat.rotate(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec3f::objName);
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
                            
                            vecFloat.rotate(argv[0].a_w.w_float, ofVec3f(argv[1].a_w.w_float,
                                                                         argv[2].a_w.w_float,
                                                                         argv[3].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec3f::objName);
                            return;
                        }
                        break;
                    }
                    case 7:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT &&
                            argv[3].a_type == A_FLOAT &&
                            argv[4].a_type == A_FLOAT &&
                            argv[5].a_type == A_FLOAT &&
                            argv[6].a_type == A_FLOAT) {
                            
                            vecFloat.rotate(argv[0].a_w.w_float,
                                            ofVec3f(argv[1].a_w.w_float,
                                                    argv[2].a_w.w_float,
                                                    argv[3].a_w.w_float),
                                            ofVec3f(argv[4].a_w.w_float,
                                                    argv[5].a_w.w_float,
                                                    argv[6].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec3f::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditVec3f::objName);
                        return;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec3f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec3f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec3f::objName);
    }
}

void ofeliaEditVec3f_rotateRad(t_ofeliaEditVec3f *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec3fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec3f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size())-1);
                ofVec3f &vecFloat = t_ofeliaLoadVec3f::vecData[pos].vecFloats[index];
                
                switch (argc) {
                        
                    case 3:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            vecFloat.rotateRad(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec3f::objName);
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
                            
                            vecFloat.rotateRad(argv[0].a_w.w_float, ofVec3f(argv[1].a_w.w_float,
                                                                            argv[2].a_w.w_float,
                                                                            argv[3].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec3f::objName);
                            return;
                        }
                        break;
                    }
                    case 7:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT &&
                            argv[3].a_type == A_FLOAT &&
                            argv[4].a_type == A_FLOAT &&
                            argv[5].a_type == A_FLOAT &&
                            argv[6].a_type == A_FLOAT) {
                            
                            vecFloat.rotateRad(argv[0].a_w.w_float,
                                               ofVec3f(argv[1].a_w.w_float,
                                                       argv[2].a_w.w_float,
                                                       argv[3].a_w.w_float),
                                               ofVec3f(argv[4].a_w.w_float,
                                                       argv[5].a_w.w_float,
                                                       argv[6].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec3f::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditVec3f::objName);
                        return;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec3f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec3f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec3f::objName);
    }
}

void ofeliaEditVec3f_scale(t_ofeliaEditVec3f *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec3fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec3f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size())-1);
                ofVec3f &vecFloat = t_ofeliaLoadVec3f::vecData[pos].vecFloats[index];
                vecFloat.scale(f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec3f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec3f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec3f::objName);
    }
}

void ofeliaEditVec3f_set(t_ofeliaEditVec3f *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaEditVec3f_print(t_ofeliaEditVec3f *x)
{
    post("\n[%s]", t_ofeliaEditVec3f::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaEditVec3f_setup()
{
    ofeliaEditVec3f_class = class_new(gensym("ofEditVec3f"),
                                      reinterpret_cast<t_newmethod>(ofeliaEditVec3f_new),
                                      0, sizeof(t_ofeliaEditVec3f),
                                      CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaEditVec3f_class, reinterpret_cast<t_method>(ofeliaEditVec3f_float));
    class_addmethod(ofeliaEditVec3f_class, reinterpret_cast<t_method>(ofeliaEditVec3f_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditVec3f_class, reinterpret_cast<t_method>(ofeliaEditVec3f_assign),
                    gensym("assign"), A_GIMME, 0);
    class_addmethod(ofeliaEditVec3f_class, reinterpret_cast<t_method>(ofeliaEditVec3f_assignX),
                    gensym("assignX"), A_FLOAT, 0);
    class_addmethod(ofeliaEditVec3f_class, reinterpret_cast<t_method>(ofeliaEditVec3f_assignY),
                    gensym("assignY"), A_FLOAT, 0);
    class_addmethod(ofeliaEditVec3f_class, reinterpret_cast<t_method>(ofeliaEditVec3f_assignZ),
                    gensym("assignZ"), A_FLOAT, 0);
    class_addmethod(ofeliaEditVec3f_class, reinterpret_cast<t_method>(ofeliaEditVec3f_add),
                    gensym("add"), A_GIMME, 0);
    class_addmethod(ofeliaEditVec3f_class, reinterpret_cast<t_method>(ofeliaEditVec3f_sub),
                    gensym("sub"), A_GIMME, 0);
    class_addmethod(ofeliaEditVec3f_class, reinterpret_cast<t_method>(ofeliaEditVec3f_mult),
                    gensym("mult"), A_GIMME, 0);
    class_addmethod(ofeliaEditVec3f_class, reinterpret_cast<t_method>(ofeliaEditVec3f_div),
                    gensym("div"), A_GIMME, 0);
    class_addmethod(ofeliaEditVec3f_class, reinterpret_cast<t_method>(ofeliaEditVec3f_cross),
                    gensym("cross"), A_GIMME, 0);
    class_addmethod(ofeliaEditVec3f_class, reinterpret_cast<t_method>(ofeliaEditVec3f_lerp),
                    gensym("lerp"), A_GIMME, 0);
    class_addmethod(ofeliaEditVec3f_class, reinterpret_cast<t_method>(ofeliaEditVec3f_limit),
                    gensym("limit"), A_FLOAT, 0);
    class_addmethod(ofeliaEditVec3f_class, reinterpret_cast<t_method>(ofeliaEditVec3f_map),
                    gensym("map"), A_GIMME, 0);
    class_addmethod(ofeliaEditVec3f_class, reinterpret_cast<t_method>(ofeliaEditVec3f_middle),
                    gensym("middle"), A_GIMME, 0);
    class_addmethod(ofeliaEditVec3f_class, reinterpret_cast<t_method>(ofeliaEditVec3f_normalize),
                    gensym("normalize"), A_NULL, 0);
    class_addmethod(ofeliaEditVec3f_class, reinterpret_cast<t_method>(ofeliaEditVec3f_perpendicular),
                    gensym("perpendicular"), A_GIMME, 0);
    class_addmethod(ofeliaEditVec3f_class, reinterpret_cast<t_method>(ofeliaEditVec3f_rotate),
                    gensym("rotate"), A_GIMME, 0);
    class_addmethod(ofeliaEditVec3f_class, reinterpret_cast<t_method>(ofeliaEditVec3f_rotateRad),
                    gensym("rotateRad"), A_GIMME, 0);
    class_addmethod(ofeliaEditVec3f_class, reinterpret_cast<t_method>(ofeliaEditVec3f_scale),
                    gensym("scale"), A_FLOAT, 0);
    class_addmethod(ofeliaEditVec3f_class, reinterpret_cast<t_method>(ofeliaEditVec3f_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditVec3f_class, reinterpret_cast<t_method>(ofeliaEditVec3f_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofDoesVec3fNameExist object methods
 */
void *ofeliaDoesVec3fNameExist_new(t_symbol *s)
{
    t_ofeliaDoesVec3fNameExist *x = reinterpret_cast<t_ofeliaDoesVec3fNameExist*>(pd_new(ofeliaDoesVec3fNameExist_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaDoesVec3fNameExist_bang(t_ofeliaDoesVec3fNameExist *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        if (getPositionByVec3fName(name) != -1)
            outlet_float(x->x_obj.ob_outlet, 1.0f);
        else
            outlet_float(x->x_obj.ob_outlet, 0.0f);
    }
    else {
        
        error("%s: name not assigned", t_ofeliaDoesVec3fNameExist::objName);
    }
}

void ofeliaDoesVec3fNameExist_set(t_ofeliaDoesVec3fNameExist *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaDoesVec3fNameExist_print(t_ofeliaDoesVec3fNameExist *x)
{
    post("\n[%s]", t_ofeliaDoesVec3fNameExist::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaDoesVec3fNameExist_setup()
{
    ofeliaDoesVec3fNameExist_class = class_new(gensym("ofDoesVec3fNameExist"),
                                               reinterpret_cast<t_newmethod>(ofeliaDoesVec3fNameExist_new),
                                               0, sizeof(t_ofeliaDoesVec3fNameExist),
                                               CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaDoesVec3fNameExist_class, reinterpret_cast<t_method>(ofeliaDoesVec3fNameExist_bang));
    class_addmethod(ofeliaDoesVec3fNameExist_class, reinterpret_cast<t_method>(ofeliaDoesVec3fNameExist_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesVec3fNameExist_class, reinterpret_cast<t_method>(ofeliaDoesVec3fNameExist_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesVec3fNameExist_class, reinterpret_cast<t_method>(ofeliaDoesVec3fNameExist_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetVec3f object methods
 */
void *ofeliaGetVec3f_new(t_symbol *s)
{
    t_ofeliaGetVec3f *x = reinterpret_cast<t_ofeliaGetVec3f*>(pd_new(ofeliaGetVec3f_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetVec3f_bang(t_ofeliaGetVec3f *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec3fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec3f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size())-1);
                const ofVec3f &vecFloat = t_ofeliaLoadVec3f::vecData[pos].vecFloats[index];
                t_atom av[3];
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = vecFloat.x;
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = vecFloat.y;
                av[2].a_type = A_FLOAT;
                av[2].a_w.w_float = vecFloat.z;
                outlet_list(x->x_obj.ob_outlet, &s_list, 3, av);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetVec3f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetVec3f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetVec3f::objName);
    }
}

void ofeliaGetVec3f_float(t_ofeliaGetVec3f *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetVec3f_set(t_ofeliaGetVec3f *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetVec3f_print(t_ofeliaGetVec3f *x)
{
    post("\n[%s]", t_ofeliaGetVec3f::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetVec3f_setup()
{
    ofeliaGetVec3f_class = class_new(gensym("ofGetVec3f"),
                                     reinterpret_cast<t_newmethod>(ofeliaGetVec3f_new),
                                     0, sizeof(t_ofeliaGetVec3f),
                                     CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetVec3f_class, reinterpret_cast<t_method>(ofeliaGetVec3f_bang));
    class_addfloat(ofeliaGetVec3f_class, reinterpret_cast<t_method>(ofeliaGetVec3f_float));
    class_addmethod(ofeliaGetVec3f_class, reinterpret_cast<t_method>(ofeliaGetVec3f_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec3f_class, reinterpret_cast<t_method>(ofeliaGetVec3f_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec3f_class, reinterpret_cast<t_method>(ofeliaGetVec3f_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetVec3fs object methods
 */
void *ofeliaGetVec3fs_new(t_symbol *s)
{
    t_ofeliaGetVec3fs *x = reinterpret_cast<t_ofeliaGetVec3fs*>(pd_new(ofeliaGetVec3fs_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetVec3fs_bang(t_ofeliaGetVec3fs *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec3fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec3f::vecData[pos].vecFloats.empty()) {
                
                vector<float> vecFloats;
                
                for (size_t i=0; i<t_ofeliaLoadVec3f::vecData[pos].vecFloats.size(); ++i) {
                    
                    vecFloats.push_back(t_ofeliaLoadVec3f::vecData[pos].vecFloats[i].x);
                    vecFloats.push_back(t_ofeliaLoadVec3f::vecData[pos].vecFloats[i].y);
                    vecFloats.push_back(t_ofeliaLoadVec3f::vecData[pos].vecFloats[i].z);
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
                
                error("%s: '%s' is empty", t_ofeliaGetVec3fs::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetVec3fs::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetVec3fs::objName);
    }
}

void ofeliaGetVec3fs_set(t_ofeliaGetVec3fs *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetVec3fs_print(t_ofeliaGetVec3fs *x)
{
    post("\n[%s]", t_ofeliaGetVec3fs::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetVec3fs_setup()
{
    ofeliaGetVec3fs_class = class_new(gensym("ofGetVec3fs"),
                                      reinterpret_cast<t_newmethod>(ofeliaGetVec3fs_new),
                                      0, sizeof(t_ofeliaGetVec3fs),
                                      CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetVec3fs_class, reinterpret_cast<t_method>(ofeliaGetVec3fs_bang));
    class_addmethod(ofeliaGetVec3fs_class, reinterpret_cast<t_method>(ofeliaGetVec3fs_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec3fs_class, reinterpret_cast<t_method>(ofeliaGetVec3fs_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec3fs_class, reinterpret_cast<t_method>(ofeliaGetVec3fs_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetVec3fAverage object methods
 */
void *ofeliaGetVec3fAverage_new(t_symbol *s)
{
    t_ofeliaGetVec3fAverage *x = reinterpret_cast<t_ofeliaGetVec3fAverage*>(pd_new(ofeliaGetVec3fAverage_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetVec3fAverage_bang(t_ofeliaGetVec3fAverage *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec3fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec3f::vecData[pos].vecFloats.empty()) {
                
                ofVec3f avg;
                avg.average(t_ofeliaLoadVec3f::vecData[pos].vecFloats.data(),
                            t_ofeliaLoadVec3f::vecData[pos].vecFloats.size());
                t_atom av[3];
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = avg.x;
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = avg.y;
                av[2].a_type = A_FLOAT;
                av[2].a_w.w_float = avg.z;
                outlet_list(x->x_obj.ob_outlet, &s_list, 3, av);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetVec3fAverage::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetVec3fAverage::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetVec3fAverage::objName);
    }
}

void ofeliaGetVec3fAverage_set(t_ofeliaGetVec3fAverage *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetVec3fAverage_print(t_ofeliaGetVec3fAverage *x)
{
    post("\n[%s]", t_ofeliaGetVec3fAverage::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetVec3fAverage_setup()
{
    ofeliaGetVec3fAverage_class = class_new(gensym("ofGetVec3fAverage"),
                                            reinterpret_cast<t_newmethod>(ofeliaGetVec3fAverage_new),
                                            0, sizeof(t_ofeliaGetVec3fAverage),
                                            CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetVec3fAverage_class, reinterpret_cast<t_method>(ofeliaGetVec3fAverage_bang));
    class_addmethod(ofeliaGetVec3fAverage_class, reinterpret_cast<t_method>(ofeliaGetVec3fAverage_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec3fAverage_class, reinterpret_cast<t_method>(ofeliaGetVec3fAverage_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec3fAverage_class, reinterpret_cast<t_method>(ofeliaGetVec3fAverage_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetVec3fAngle object methods
 */
void *ofeliaGetVec3fAngle_new(t_symbol *s)
{
    t_ofeliaGetVec3fAngle *x = reinterpret_cast<t_ofeliaGetVec3fAngle*>(pd_new(ofeliaGetVec3fAngle_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetVec3fAngle_float(t_ofeliaGetVec3fAngle *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetVec3fAngle_list(t_ofeliaGetVec3fAngle *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec3fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec3f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size())-1);
                const ofVec3f &vecFloat = t_ofeliaLoadVec3f::vecData[pos].vecFloats[index];
                
                if (argc >= 3) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT) {
                        
                        outlet_float(x->x_obj.ob_outlet, vecFloat.angle(ofVec3f(argv[0].a_w.w_float,
                                                                                argv[1].a_w.w_float,
                                                                                argv[2].a_w.w_float)));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaGetVec3fAngle::objName);
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaGetVec3fAngle::objName);
                }
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetVec3fAngle::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetVec3fAngle::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetVec3fAngle::objName);
    }
}

void ofeliaGetVec3fAngle_set(t_ofeliaGetVec3fAngle *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetVec3fAngle_print(t_ofeliaGetVec3fAngle *x)
{
    post("\n[%s]", t_ofeliaGetVec3fAngle::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetVec3fAngle_setup()
{
    ofeliaGetVec3fAngle_class = class_new(gensym("ofGetVec3fAngle"),
                                          reinterpret_cast<t_newmethod>(ofeliaGetVec3fAngle_new),
                                          0, sizeof(t_ofeliaGetVec3fAngle),
                                          CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaGetVec3fAngle_class, reinterpret_cast<t_method>(ofeliaGetVec3fAngle_float));
    class_addlist(ofeliaGetVec3fAngle_class, reinterpret_cast<t_method>(ofeliaGetVec3fAngle_list));
    class_addmethod(ofeliaGetVec3fAngle_class, reinterpret_cast<t_method>(ofeliaGetVec3fAngle_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec3fAngle_class, reinterpret_cast<t_method>(ofeliaGetVec3fAngle_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec3fAngle_class, reinterpret_cast<t_method>(ofeliaGetVec3fAngle_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetVec3fAngleRad object methods
 */
void *ofeliaGetVec3fAngleRad_new(t_symbol *s)
{
    t_ofeliaGetVec3fAngleRad *x = reinterpret_cast<t_ofeliaGetVec3fAngleRad*>(pd_new(ofeliaGetVec3fAngleRad_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetVec3fAngleRad_float(t_ofeliaGetVec3fAngleRad *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetVec3fAngleRad_list(t_ofeliaGetVec3fAngleRad *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {

        const int pos = getPositionByVec3fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec3f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size())-1);
                const ofVec3f &vecFloat = t_ofeliaLoadVec3f::vecData[pos].vecFloats[index];
                
                if (argc >= 3) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT) {
                        
                        outlet_float(x->x_obj.ob_outlet, vecFloat.angleRad(ofVec3f(argv[0].a_w.w_float,
                                                                                   argv[1].a_w.w_float,
                                                                                   argv[2].a_w.w_float)));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaGetVec3fAngleRad::objName);
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaGetVec3fAngleRad::objName);
                }
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetVec3fAngleRad::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetVec3fAngleRad::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetVec3fAngleRad::objName);
    }
}

void ofeliaGetVec3fAngleRad_set(t_ofeliaGetVec3fAngleRad *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetVec3fAngleRad_print(t_ofeliaGetVec3fAngleRad *x)
{
    post("\n[%s]", t_ofeliaGetVec3fAngleRad::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetVec3fAngleRad_setup()
{
    ofeliaGetVec3fAngleRad_class = class_new(gensym("ofGetVec3fAngleRad"),
                                             reinterpret_cast<t_newmethod>(ofeliaGetVec3fAngleRad_new),
                                             0, sizeof(t_ofeliaGetVec3fAngleRad),
                                             CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaGetVec3fAngleRad_class, reinterpret_cast<t_method>(ofeliaGetVec3fAngleRad_float));
    class_addlist(ofeliaGetVec3fAngleRad_class, reinterpret_cast<t_method>(ofeliaGetVec3fAngleRad_list));
    class_addmethod(ofeliaGetVec3fAngleRad_class, reinterpret_cast<t_method>(ofeliaGetVec3fAngleRad_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec3fAngleRad_class, reinterpret_cast<t_method>(ofeliaGetVec3fAngleRad_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec3fAngleRad_class, reinterpret_cast<t_method>(ofeliaGetVec3fAngleRad_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetVec3fDist object methods
 */
void *ofeliaGetVec3fDist_new(t_symbol *s)
{
    t_ofeliaGetVec3fDist *x = reinterpret_cast<t_ofeliaGetVec3fDist*>(pd_new(ofeliaGetVec3fDist_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetVec3fDist_float(t_ofeliaGetVec3fDist *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetVec3fDist_list(t_ofeliaGetVec3fDist *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec3fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec3f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size())-1);
                const ofVec3f &vecFloat = t_ofeliaLoadVec3f::vecData[pos].vecFloats[index];
                
                if (argc >= 3) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT) {
                        
                        outlet_float(x->x_obj.ob_outlet, vecFloat.distance(ofVec3f(argv[0].a_w.w_float,
                                                                                   argv[1].a_w.w_float,
                                                                                   argv[2].a_w.w_float)));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaGetVec3fDist::objName);
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaGetVec3fDist::objName);
                }
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetVec3fDist::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetVec3fDist::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetVec3fDist::objName);
    }
}

void ofeliaGetVec3fDist_set(t_ofeliaGetVec3fDist *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetVec3fDist_print(t_ofeliaGetVec3fDist *x)
{
    post("\n[%s]", t_ofeliaGetVec3fDist::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetVec3fDist_setup()
{
    ofeliaGetVec3fDist_class = class_new(gensym("ofGetVec3fDist"),
                                         reinterpret_cast<t_newmethod>(ofeliaGetVec3fDist_new),
                                         0, sizeof(t_ofeliaGetVec3fDist),
                                         CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaGetVec3fDist_class, reinterpret_cast<t_method>(ofeliaGetVec3fDist_float));
    class_addlist(ofeliaGetVec3fDist_class, reinterpret_cast<t_method>(ofeliaGetVec3fDist_list));
    class_addmethod(ofeliaGetVec3fDist_class, reinterpret_cast<t_method>(ofeliaGetVec3fDist_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec3fDist_class, reinterpret_cast<t_method>(ofeliaGetVec3fDist_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec3fDist_class, reinterpret_cast<t_method>(ofeliaGetVec3fDist_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetVec3fDistSquared object methods
 */
void *ofeliaGetVec3fDistSquared_new(t_symbol *s)
{
    t_ofeliaGetVec3fDistSquared *x = reinterpret_cast<t_ofeliaGetVec3fDistSquared*>(pd_new(ofeliaGetVec3fDistSquared_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetVec3fDistSquared_float(t_ofeliaGetVec3fDistSquared *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetVec3fDistSquared_list(t_ofeliaGetVec3fDistSquared *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec3fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec3f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size())-1);
                const ofVec3f &vecFloat = t_ofeliaLoadVec3f::vecData[pos].vecFloats[index];
                
                if (argc >= 3) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT) {
                        
                        outlet_float(x->x_obj.ob_outlet, vecFloat.squareDistance(ofVec3f(argv[0].a_w.w_float,
                                                                                         argv[1].a_w.w_float,
                                                                                         argv[2].a_w.w_float)));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaGetVec3fDistSquared::objName);
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaGetVec3fDistSquared::objName);
                }
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetVec3fDistSquared::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetVec3fDistSquared::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetVec3fDistSquared::objName);
    }
}

void ofeliaGetVec3fDistSquared_set(t_ofeliaGetVec3fDistSquared *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetVec3fDistSquared_print(t_ofeliaGetVec3fDistSquared *x)
{
    post("\n[%s]", t_ofeliaGetVec3fDistSquared::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetVec3fDistSquared_setup()
{
    ofeliaGetVec3fDistSquared_class = class_new(gensym("ofGetVec3fDistSquared"),
                                                reinterpret_cast<t_newmethod>(ofeliaGetVec3fDistSquared_new),
                                                0, sizeof(t_ofeliaGetVec3fDistSquared),
                                                CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaGetVec3fDistSquared_class, reinterpret_cast<t_method>(ofeliaGetVec3fDistSquared_float));
    class_addlist(ofeliaGetVec3fDistSquared_class, reinterpret_cast<t_method>(ofeliaGetVec3fDistSquared_list));
    class_addmethod(ofeliaGetVec3fDistSquared_class, reinterpret_cast<t_method>(ofeliaGetVec3fDistSquared_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec3fDistSquared_class, reinterpret_cast<t_method>(ofeliaGetVec3fDistSquared_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec3fDistSquared_class, reinterpret_cast<t_method>(ofeliaGetVec3fDistSquared_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetVec3fDot object methods
 */
void *ofeliaGetVec3fDot_new(t_symbol *s)
{
    t_ofeliaGetVec3fDot *x = reinterpret_cast<t_ofeliaGetVec3fDot*>(pd_new(ofeliaGetVec3fDot_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetVec3fDot_float(t_ofeliaGetVec3fDot *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetVec3fDot_list(t_ofeliaGetVec3fDot *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec3fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec3f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size())-1);
                const ofVec3f &vecFloat = t_ofeliaLoadVec3f::vecData[pos].vecFloats[index];
                
                if (argc >= 3) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT) {
                        
                        outlet_float(x->x_obj.ob_outlet, vecFloat.dot(ofVec3f(argv[0].a_w.w_float,
                                                                              argv[1].a_w.w_float,
                                                                              argv[2].a_w.w_float)));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaGetVec3fDot::objName);
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaGetVec3fDot::objName);
                }
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetVec3fDot::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetVec3fDot::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetVec3fDot::objName);
    }
}

void ofeliaGetVec3fDot_set(t_ofeliaGetVec3fDot *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetVec3fDot_print(t_ofeliaGetVec3fDot *x)
{
    post("\n[%s]", t_ofeliaGetVec3fDot::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetVec3fDot_setup()
{
    ofeliaGetVec3fDot_class = class_new(gensym("ofGetVec3fDot"),
                                        reinterpret_cast<t_newmethod>(ofeliaGetVec3fDot_new),
                                        0, sizeof(t_ofeliaGetVec3fDot),
                                        CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaGetVec3fDot_class, reinterpret_cast<t_method>(ofeliaGetVec3fDot_float));
    class_addlist(ofeliaGetVec3fDot_class, reinterpret_cast<t_method>(ofeliaGetVec3fDot_list));
    class_addmethod(ofeliaGetVec3fDot_class, reinterpret_cast<t_method>(ofeliaGetVec3fDot_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec3fDot_class, reinterpret_cast<t_method>(ofeliaGetVec3fDot_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec3fDot_class, reinterpret_cast<t_method>(ofeliaGetVec3fDot_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetVec3fLength object methods
 */
void *ofeliaGetVec3fLength_new(t_symbol *s)
{
    t_ofeliaGetVec3fLength *x = reinterpret_cast<t_ofeliaGetVec3fLength*>(pd_new(ofeliaGetVec3fLength_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetVec3fLength_bang(t_ofeliaGetVec3fLength *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec3fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec3f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size())-1);
                const ofVec3f &vecFloat = t_ofeliaLoadVec3f::vecData[pos].vecFloats[index];
                outlet_float(x->x_obj.ob_outlet, vecFloat.length());
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetVec3fLength::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetVec3fLength::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetVec3fLength::objName);
    }
}

void ofeliaGetVec3fLength_float(t_ofeliaGetVec3fLength *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetVec3fLength_set(t_ofeliaGetVec3fLength *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetVec3fLength_print(t_ofeliaGetVec3fLength *x)
{
    post("\n[%s]", t_ofeliaGetVec3fLength::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetVec3fLength_setup()
{
    ofeliaGetVec3fLength_class = class_new(gensym("ofGetVec3fLength"),
                                           reinterpret_cast<t_newmethod>(ofeliaGetVec3fLength_new),
                                           0, sizeof(t_ofeliaGetVec3fLength),
                                           CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetVec3fLength_class, reinterpret_cast<t_method>(ofeliaGetVec3fLength_bang));
    class_addfloat(ofeliaGetVec3fLength_class, reinterpret_cast<t_method>(ofeliaGetVec3fLength_float));
    class_addmethod(ofeliaGetVec3fLength_class, reinterpret_cast<t_method>(ofeliaGetVec3fLength_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec3fLength_class, reinterpret_cast<t_method>(ofeliaGetVec3fLength_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec3fLength_class, reinterpret_cast<t_method>(ofeliaGetVec3fLength_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetVec3fLengthSquared object methods
 */
void *ofeliaGetVec3fLengthSquared_new(t_symbol *s)
{
    t_ofeliaGetVec3fLengthSquared *x = reinterpret_cast<t_ofeliaGetVec3fLengthSquared*>(pd_new(ofeliaGetVec3fLengthSquared_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetVec3fLengthSquared_bang(t_ofeliaGetVec3fLengthSquared *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {

        const int pos = getPositionByVec3fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec3f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec3f::vecData[pos].vecFloats.size())-1);
                const ofVec3f &vecFloat = t_ofeliaLoadVec3f::vecData[pos].vecFloats[index];
                outlet_float(x->x_obj.ob_outlet, vecFloat.lengthSquared());
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetVec3fLengthSquared::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetVec3fLengthSquared::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetVec3fLengthSquared::objName);
    }
}

void ofeliaGetVec3fLengthSquared_float(t_ofeliaGetVec3fLengthSquared *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetVec3fLengthSquared_set(t_ofeliaGetVec3fLengthSquared *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetVec3fLengthSquared_print(t_ofeliaGetVec3fLengthSquared *x)
{
    post("\n[%s]", t_ofeliaGetVec3fLengthSquared::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetVec3fLengthSquared_setup()
{
    ofeliaGetVec3fLengthSquared_class = class_new(gensym("ofGetVec3fLengthSquared"),
                                                  reinterpret_cast<t_newmethod>(ofeliaGetVec3fLengthSquared_new),
                                                  0, sizeof(t_ofeliaGetVec3fLengthSquared),
                                                  CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetVec3fLengthSquared_class, reinterpret_cast<t_method>(ofeliaGetVec3fLengthSquared_bang));
    class_addfloat(ofeliaGetVec3fLengthSquared_class, reinterpret_cast<t_method>(ofeliaGetVec3fLengthSquared_float));
    class_addmethod(ofeliaGetVec3fLengthSquared_class, reinterpret_cast<t_method>(ofeliaGetVec3fLengthSquared_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec3fLengthSquared_class, reinterpret_cast<t_method>(ofeliaGetVec3fLengthSquared_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec3fLengthSquared_class, reinterpret_cast<t_method>(ofeliaGetVec3fLengthSquared_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * setup methods
 */
void ofeliaVec3f_setup()
{
    ofeliaLoadVec3f_setup();
    ofeliaEditVec3f_setup();
    ofeliaDoesVec3fNameExist_setup();
    ofeliaGetVec3f_setup();
    ofeliaGetVec3fs_setup();
    ofeliaGetVec3fAverage_setup();
    ofeliaGetVec3fAngle_setup();
    ofeliaGetVec3fAngleRad_setup();
    ofeliaGetVec3fDist_setup();
    ofeliaGetVec3fDistSquared_setup();
    ofeliaGetVec3fDot_setup();
    ofeliaGetVec3fLength_setup();
    ofeliaGetVec3fLengthSquared_setup();
}

