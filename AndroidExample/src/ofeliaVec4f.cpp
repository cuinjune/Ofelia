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

#include "ofeliaVec4f.h"

/* ________________________________________________________________________________
 * static variables
 */
const char *t_ofeliaLoadVec4f::objName = "ofLoadVec4f";
unsigned int t_ofeliaLoadVec4f::counter;
vector<t_ofeliaLoadVec4fData> t_ofeliaLoadVec4f::vecData;
const char *t_ofeliaEditVec4f::objName = "ofEditVec4f";
const char *t_ofeliaDoesVec4fNameExist::objName = "ofDoesVec4fNameExist";
const char *t_ofeliaGetVec4f::objName = "ofGetVec4f";
const char *t_ofeliaGetVec4fs::objName = "ofGetVec4fs";
const char *t_ofeliaGetVec4fAverage::objName = "ofGetVec4fAverage";
const char *t_ofeliaGetVec4fAngle::objName = "ofGetVec4fAngle";
const char *t_ofeliaGetVec4fAngleRad::objName = "ofGetVec4fAngleRad";
const char *t_ofeliaGetVec4fDist::objName = "ofGetVec4fDist";
const char *t_ofeliaGetVec4fDistSquared::objName = "ofGetVec4fDistSquared";
const char *t_ofeliaGetVec4fDot::objName = "ofGetVec4fDot";
const char *t_ofeliaGetVec4fLength::objName = "ofGetVec4fLength";
const char *t_ofeliaGetVec4fLengthSquared::objName = "ofGetVec4fLengthSquared";

/* ________________________________________________________________________________
 * ofLoadVec4f object methods
 */
void ofeliaLoadVec4f_vecSizeOut(t_ofeliaLoadVec4f *x);

int getPositionByVec4fObjID(const unsigned int objID)
{
    const auto it = find_if(begin(t_ofeliaLoadVec4f::vecData), end(t_ofeliaLoadVec4f::vecData),
                            [&objID](const t_ofeliaLoadVec4fData &vecData)
                            {
                                return vecData.objID == objID;
                            });
    return static_cast<int>(it - t_ofeliaLoadVec4f::vecData.begin());
}

bool getLoadVec4fFloatFromArgs(const int argc, const t_atom *argv, ofVec4f &vecFloat)
{
    if (argc) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT &&
            argv[3].a_type == A_FLOAT) {
            
            vecFloat.set(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float, argv[3].a_w.w_float);
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadVec4f::objName);
            return 0;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadVec4f::objName);
        return 0;
    }
    return 1;
}

bool getLoadVec4fFloatsFromArgs(const int argc, const t_atom *argv, vector<ofVec4f> &vecFloats)
{
    if (argc && argc%4 == 0) {
        
        for (int i=0; i<argc; i+=4) {
            
            if (argv[i].a_type == A_FLOAT &&
                argv[i+1].a_type == A_FLOAT &&
                argv[i+2].a_type == A_FLOAT &&
                argv[i+3].a_type == A_FLOAT) {
                
                vecFloats.push_back(ofVec4f(argv[i].a_w.w_float,
                                            argv[i+1].a_w.w_float,
                                            argv[i+2].a_w.w_float,
                                            argv[i+3].a_w.w_float));
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaLoadVec4f::objName);
                return 0;
            }
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadVec4f::objName);
        return 0;
    }
    return 1;
}

bool getLoadVec4fCmdRangeFromArgs(const int argc, const t_atom *argv, t_ofeliaLoadVec4fCmdData &cmd)
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
                
                error("%s: wrong argument type", t_ofeliaLoadVec4f::objName);
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
                
                error("%s: wrong argument type", t_ofeliaLoadVec4f::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaLoadVec4f::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaLoadVec4f_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadVec4f *x = reinterpret_cast<t_ofeliaLoadVec4f*>(pd_new(ofeliaLoadVec4f_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    t_ofeliaLoadVec4fData vecData;
    getVarNameLocalPrefixes(vecData.varName);
    vector<ofVec4f> vecFloats;
    
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            vecData.varName.name = argv[0].a_w.w_symbol->s_name;
            getVarNameLocalized(vecData.varName);
            
            if (--argc) {
                
                if (!getLoadVec4fFloatsFromArgs(argc, argv+1, vecFloats))
                    return 0;
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadVec4f::objName);
            return 0;
        }
    }
    x->objID = vecData.objID = t_ofeliaLoadVec4f::counter++;
    t_ofeliaLoadVec4f::vecData.push_back(vecData);
    x->vecSize = 0;
    x->vecSizeOutClock = clock_new(x, reinterpret_cast<t_method>(ofeliaLoadVec4f_vecSizeOut));
    outlet_new(&x->x_obj, &s_float);
    
    if (argc) {
        
        const int pos = getPositionByVec4fObjID(x->objID);
        t_ofeliaLoadVec4f::vecData[pos].vecFloats = vecFloats;
        x->vecSize = static_cast<int>(vecFloats.size());
        clock_delay(x->vecSizeOutClock, 0.0);
    }
    return (x);
}

void ofeliaLoadVec4f_vecSizeOut(t_ofeliaLoadVec4f *x)
{
    outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(x->vecSize));
}

void ofeliaLoadVec4f_name(t_ofeliaLoadVec4f *x, t_symbol *s)
{
    const int pos = getPositionByVec4fObjID(x->objID);
    t_ofeliaLoadVec4f::vecData[pos].varName.name = s->s_name;
    getVarNameLocalized(t_ofeliaLoadVec4f::vecData[pos].varName);
}

void ofeliaLoadVec4f_load(t_ofeliaLoadVec4f *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<ofVec4f> vecFloats;
    
    if (getLoadVec4fFloatsFromArgs(argc, argv, vecFloats)) {
        
        const int pos = getPositionByVec4fObjID(x->objID);
        t_ofeliaLoadVec4f::vecData[pos].vecFloats = vecFloats;
        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(vecFloats.size()));
    }
}

void ofeliaLoadVec4f_add(t_ofeliaLoadVec4f *x, t_symbol *s, int argc, t_atom *argv)
{
    ofVec4f vecFloat;
    
    if (getLoadVec4fFloatFromArgs(argc, argv, vecFloat)) {
        
        const int pos = getPositionByVec4fObjID(x->objID);
        t_ofeliaLoadVec4f::vecData[pos].vecFloats.push_back(vecFloat);
        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadVec4f::vecData[pos].vecFloats.size()));
    }
}

void ofeliaLoadVec4f_append(t_ofeliaLoadVec4f *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<ofVec4f> vecFloats;
    
    if (getLoadVec4fFloatsFromArgs(argc, argv, vecFloats)) {
        
        const int pos = getPositionByVec4fObjID(x->objID);
        t_ofeliaLoadVec4f::vecData[pos].vecFloats.insert(t_ofeliaLoadVec4f::vecData[pos].vecFloats.end(),
                                                         vecFloats.begin(), vecFloats.end());
        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadVec4f::vecData[pos].vecFloats.size()));
    }
}

void ofeliaLoadVec4f_prepend(t_ofeliaLoadVec4f *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<ofVec4f> vecFloats;
    
    if (getLoadVec4fFloatsFromArgs(argc, argv, vecFloats)) {
        
        const int pos = getPositionByVec4fObjID(x->objID);
        t_ofeliaLoadVec4f::vecData[pos].vecFloats.insert(t_ofeliaLoadVec4f::vecData[pos].vecFloats.begin(),
                                                         vecFloats.begin(), vecFloats.end());
        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadVec4f::vecData[pos].vecFloats.size()));
    }
}

void ofeliaLoadVec4f_insert(t_ofeliaLoadVec4f *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadVec4fCmdData cmd;
    
    if (getLoadVec4fFloatFromArgs(argc, argv, cmd.vecFloat)) {
        
        if (getLoadVec4fCmdRangeFromArgs(argc-4, argv+4, cmd)) {
            
            const int pos = getPositionByVec4fObjID(x->objID);
            const int size = static_cast<int>(t_ofeliaLoadVec4f::vecData[pos].vecFloats.size());
            const auto vecFloatsBegin = t_ofeliaLoadVec4f::vecData[pos].vecFloats.begin();
            const int fromIndex = static_cast<int>(ofClamp(cmd.fromIndex, 0.0f, size));
            const int insertSize = static_cast<int>(max(0.0f, max(0.0f, cmd.toIndex) - max(0.0f, cmd.fromIndex))) + 1;
            t_ofeliaLoadVec4f::vecData[pos].vecFloats.insert(vecFloatsBegin + fromIndex,
                                                             insertSize, cmd.vecFloat);
            outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadVec4f::vecData[pos].vecFloats.size()));
        }
    }
}

void ofeliaLoadVec4f_fill(t_ofeliaLoadVec4f *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadVec4fCmdData cmd;
    
    if (getLoadVec4fFloatFromArgs(argc, argv, cmd.vecFloat)) {
        
        if (getLoadVec4fCmdRangeFromArgs(argc-4, argv+4, cmd)) {
            
            const int pos = getPositionByVec4fObjID(x->objID);
            const int size = static_cast<int>(t_ofeliaLoadVec4f::vecData[pos].vecFloats.size());
            const auto vecFloatsBegin = t_ofeliaLoadVec4f::vecData[pos].vecFloats.begin();
            
            if (size) {
                
                const int maxIndex = size - 1;
                const int fromIndex = static_cast<int>(ofClamp(cmd.fromIndex, 0.0f, maxIndex));
                const int toIndex = static_cast<int>(ofClamp(cmd.toIndex, fromIndex, maxIndex)) + 1;
                fill(vecFloatsBegin + fromIndex, vecFloatsBegin + toIndex, cmd.vecFloat);
                outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadVec4f::vecData[pos].vecFloats.size()));
            }
            else {
                
                outlet_float(x->x_obj.ob_outlet, 0.0f);
            }
        }
    }
}

void ofeliaLoadVec4f_erase(t_ofeliaLoadVec4f *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadVec4fCmdData cmd;
    
    if (getLoadVec4fCmdRangeFromArgs(argc, argv, cmd)) {
        
        const int pos = getPositionByVec4fObjID(x->objID);
        const int size = static_cast<int>(t_ofeliaLoadVec4f::vecData[pos].vecFloats.size());
        const auto vecFloatsBegin = t_ofeliaLoadVec4f::vecData[pos].vecFloats.begin();
        
        if (size) {
            
            const int maxIndex = size - 1;
            const int fromIndex = static_cast<int>(ofClamp(cmd.fromIndex, 0.0f, maxIndex));
            const int toIndex = static_cast<int>(ofClamp(cmd.toIndex, fromIndex, maxIndex)) + 1;
            t_ofeliaLoadVec4f::vecData[pos].vecFloats.erase(vecFloatsBegin + fromIndex,
                                                            vecFloatsBegin + toIndex);
            outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadVec4f::vecData[pos].vecFloats.size()));
        }
        else {
            
            outlet_float(x->x_obj.ob_outlet, 0.0f);
        }
    }
}

void ofeliaLoadVec4f_clear(t_ofeliaLoadVec4f *x)
{
    const int pos = getPositionByVec4fObjID(x->objID);
    t_ofeliaLoadVec4f::vecData[pos].vecFloats.clear();
    outlet_float(x->x_obj.ob_outlet, 0.0f);
}

void ofeliaLoadVec4f_set(t_ofeliaLoadVec4f *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            ofeliaLoadVec4f_name(x, argv[0].a_w.w_symbol);
            
            if (--argc)
                ofeliaLoadVec4f_load(x, 0, argc, argv+1);
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadVec4f::objName);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadVec4f::objName);
    }
}

void ofeliaLoadVec4f_print(t_ofeliaLoadVec4f *x)
{
    post("\n[%s]", t_ofeliaLoadVec4f::objName);
    const int pos = getPositionByVec4fObjID(x->objID);
    post("name : %s", t_ofeliaLoadVec4f::vecData[pos].varName.name.c_str());
    post("load : ");
    
    for (size_t i=0; i<t_ofeliaLoadVec4f::vecData[pos].vecFloats.size(); ++i) {
        
        post(" %g %g %g %g", t_ofeliaLoadVec4f::vecData[pos].vecFloats[i].x, t_ofeliaLoadVec4f::vecData[pos].vecFloats[i].y, t_ofeliaLoadVec4f::vecData[pos].vecFloats[i].z, t_ofeliaLoadVec4f::vecData[pos].vecFloats[i].w);
    }
}

void ofeliaLoadVec4f_free(t_ofeliaLoadVec4f *x)
{
    clock_free(x->vecSizeOutClock);
    const int pos = getPositionByVec4fObjID(x->objID);
    t_ofeliaLoadVec4f::vecData.erase(t_ofeliaLoadVec4f::vecData.begin() + pos);
}

void ofeliaLoadVec4f_setup()
{
    ofeliaLoadVec4f_class = class_new(gensym("ofLoadVec4f"),
                                      reinterpret_cast<t_newmethod>(ofeliaLoadVec4f_new),
                                      reinterpret_cast<t_method>(ofeliaLoadVec4f_free),
                                      sizeof(t_ofeliaLoadVec4f),
                                      CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaLoadVec4f_class, reinterpret_cast<t_method>(ofeliaLoadVec4f_name),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaLoadVec4f_class, reinterpret_cast<t_method>(ofeliaLoadVec4f_load),
                    gensym("load"), A_GIMME, 0);
    class_addmethod(ofeliaLoadVec4f_class, reinterpret_cast<t_method>(ofeliaLoadVec4f_add),
                    gensym("add"), A_GIMME, 0);
    class_addmethod(ofeliaLoadVec4f_class, reinterpret_cast<t_method>(ofeliaLoadVec4f_append),
                    gensym("append"), A_GIMME, 0);
    class_addmethod(ofeliaLoadVec4f_class, reinterpret_cast<t_method>(ofeliaLoadVec4f_prepend),
                    gensym("prepend"), A_GIMME, 0);
    class_addmethod(ofeliaLoadVec4f_class, reinterpret_cast<t_method>(ofeliaLoadVec4f_insert),
                    gensym("insert"), A_GIMME, 0);
    class_addmethod(ofeliaLoadVec4f_class, reinterpret_cast<t_method>(ofeliaLoadVec4f_fill),
                    gensym("fill"), A_GIMME, 0);
    class_addmethod(ofeliaLoadVec4f_class, reinterpret_cast<t_method>(ofeliaLoadVec4f_erase),
                    gensym("erase"), A_GIMME, 0);
    class_addmethod(ofeliaLoadVec4f_class, reinterpret_cast<t_method>(ofeliaLoadVec4f_clear),
                    gensym("clear"), A_NULL, 0);
    class_addmethod(ofeliaLoadVec4f_class, reinterpret_cast<t_method>(ofeliaLoadVec4f_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaLoadVec4f_class, reinterpret_cast<t_method>(ofeliaLoadVec4f_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * common methods
 */
int getPositionByVec4fName(const t_string &name)
{
    auto it = find_if(begin(t_ofeliaLoadVec4f::vecData), end(t_ofeliaLoadVec4f::vecData),
                      [&name] (const t_ofeliaLoadVec4fData &vecData) {
                          return (vecData.varName.name == name);
                      });
    if (it == end(t_ofeliaLoadVec4f::vecData))
        return -1;
    return static_cast<int>(it - begin(t_ofeliaLoadVec4f::vecData));
}

/* ________________________________________________________________________________
 * ofEditVec4f object methods
 */
void *ofeliaEditVec4f_new(t_symbol *s)
{
    t_ofeliaEditVec4f *x = reinterpret_cast<t_ofeliaEditVec4f*>(pd_new(ofeliaEditVec4f_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaEditVec4f_float(t_ofeliaEditVec4f *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaEditVec4f_assign(t_ofeliaEditVec4f *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {

        const int pos = getPositionByVec4fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec4f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec4f::vecData[pos].vecFloats.size())-1);
                ofVec4f &vecFloat = t_ofeliaLoadVec4f::vecData[pos].vecFloats[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditVec4f::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            vecFloat = ofVec4f(argv[0].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec4f::objName);
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
                            
                            error("%s: wrong argument type", t_ofeliaEditVec4f::objName);
                            return;
                        }
                        break;
                    }
                    case 3:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            vecFloat = ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec4f::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT &&
                            argv[3].a_type == A_FLOAT) {
                            
                            vecFloat = ofVec4f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float, argv[3].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec4f::objName);
                            return;
                        }
                        break;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec4f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec4f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec4f::objName);
    }
}

void ofeliaEditVec4f_assignX(t_ofeliaEditVec4f *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec4fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec4f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec4f::vecData[pos].vecFloats.size())-1);
                ofVec4f &vecFloat = t_ofeliaLoadVec4f::vecData[pos].vecFloats[index];
                vecFloat.x = f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec4f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec4f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec4f::objName);
    }
}

void ofeliaEditVec4f_assignY(t_ofeliaEditVec4f *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec4fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec4f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec4f::vecData[pos].vecFloats.size())-1);
                ofVec4f &vecFloat = t_ofeliaLoadVec4f::vecData[pos].vecFloats[index];
                vecFloat.y = f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec4f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec4f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec4f::objName);
    }
}

void ofeliaEditVec4f_assignZ(t_ofeliaEditVec4f *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec4fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec4f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec4f::vecData[pos].vecFloats.size())-1);
                ofVec4f &vecFloat = t_ofeliaLoadVec4f::vecData[pos].vecFloats[index];
                vecFloat.z = f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec4f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec4f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec4f::objName);
    }
}

void ofeliaEditVec4f_assignW(t_ofeliaEditVec4f *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec4fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec4f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec4f::vecData[pos].vecFloats.size())-1);
                ofVec4f &vecFloat = t_ofeliaLoadVec4f::vecData[pos].vecFloats[index];
                vecFloat.w = f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec4f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec4f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec4f::objName);
    }
}

void ofeliaEditVec4f_add(t_ofeliaEditVec4f *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {

        const int pos = getPositionByVec4fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec4f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec4f::vecData[pos].vecFloats.size())-1);
                ofVec4f &vecFloat = t_ofeliaLoadVec4f::vecData[pos].vecFloats[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditVec4f::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            vecFloat += argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec4f::objName);
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
                            
                            error("%s: wrong argument type", t_ofeliaEditVec4f::objName);
                            return;
                        }
                        break;
                    }
                    case 3:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            vecFloat += ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec4f::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT &&
                            argv[3].a_type == A_FLOAT) {
                            
                            vecFloat += ofVec4f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float, argv[3].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec4f::objName);
                            return;
                        }
                        break;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec4f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec4f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec4f::objName);
    }
}

void ofeliaEditVec4f_sub(t_ofeliaEditVec4f *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec4fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec4f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec4f::vecData[pos].vecFloats.size())-1);
                ofVec4f &vecFloat = t_ofeliaLoadVec4f::vecData[pos].vecFloats[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditVec4f::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            vecFloat -= argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec4f::objName);
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
                            
                            error("%s: wrong argument type", t_ofeliaEditVec4f::objName);
                            return;
                        }
                        break;
                    }
                    case 3:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            vecFloat -= ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec4f::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT &&
                            argv[3].a_type == A_FLOAT) {
                            
                            vecFloat -= ofVec4f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float, argv[3].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec4f::objName);
                            return;
                        }
                        break;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec4f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec4f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec4f::objName);
    }
}

void ofeliaEditVec4f_mult(t_ofeliaEditVec4f *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec4fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec4f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec4f::vecData[pos].vecFloats.size())-1);
                ofVec4f &vecFloat = t_ofeliaLoadVec4f::vecData[pos].vecFloats[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditVec4f::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            vecFloat *= argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec4f::objName);
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
                            
                            error("%s: wrong argument type", t_ofeliaEditVec4f::objName);
                            return;
                        }
                        break;
                    }
                    case 3:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            vecFloat *= ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec4f::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT &&
                            argv[3].a_type == A_FLOAT) {
                            
                            vecFloat *= ofVec4f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float, argv[3].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec4f::objName);
                            return;
                        }
                        break;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec4f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec4f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec4f::objName);
    }
}

void ofeliaEditVec4f_div(t_ofeliaEditVec4f *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec4fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec4f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec4f::vecData[pos].vecFloats.size())-1);
                ofVec4f &vecFloat = t_ofeliaLoadVec4f::vecData[pos].vecFloats[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditVec4f::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            vecFloat /= argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec4f::objName);
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
                            
                            error("%s: wrong argument type", t_ofeliaEditVec4f::objName);
                            return;
                        }
                        break;
                    }
                    case 3:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            vecFloat /= ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec4f::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT &&
                            argv[3].a_type == A_FLOAT) {
                            
                            vecFloat /= ofVec4f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float, argv[3].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditVec4f::objName);
                            return;
                        }
                        break;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec4f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec4f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec4f::objName);
    }
}

void ofeliaEditVec4f_lerp(t_ofeliaEditVec4f *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec4fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec4f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec4f::vecData[pos].vecFloats.size())-1);
                ofVec4f &vecFloat = t_ofeliaLoadVec4f::vecData[pos].vecFloats[index];
                
                if (argc == 5) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT &&
                        argv[3].a_type == A_FLOAT &&
                        argv[4].a_type == A_FLOAT) {
                        
                        vecFloat.interpolate(ofVec4f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float, argv[3].a_w.w_float), argv[4].a_w.w_float);
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditVec4f::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditVec4f::objName);
                    return;
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec4f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec4f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec4f::objName);
    }
}

void ofeliaEditVec4f_limit(t_ofeliaEditVec4f *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec4fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec4f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec4f::vecData[pos].vecFloats.size())-1);
                ofVec4f &vecFloat = t_ofeliaLoadVec4f::vecData[pos].vecFloats[index];
                vecFloat.limit(f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec4f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec4f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec4f::objName);
    }
}

void ofeliaEditVec4f_middle(t_ofeliaEditVec4f *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec4fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec4f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec4f::vecData[pos].vecFloats.size())-1);
                ofVec4f &vecFloat = t_ofeliaLoadVec4f::vecData[pos].vecFloats[index];
                
                if (argc == 4) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT &&
                        argv[3].a_type == A_FLOAT) {
                        
                        vecFloat.middle(ofVec4f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float, argv[3].a_w.w_float));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditVec4f::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditVec4f::objName);
                    return;
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec4f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec4f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec4f::objName);
    }
}

void ofeliaEditVec4f_normalize(t_ofeliaEditVec4f *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec4fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec4f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec4f::vecData[pos].vecFloats.size())-1);
                ofVec4f &vecFloat = t_ofeliaLoadVec4f::vecData[pos].vecFloats[index];
                vecFloat.normalize();
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec4f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec4f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec4f::objName);
    }
}

void ofeliaEditVec4f_scale(t_ofeliaEditVec4f *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec4fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec4f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec4f::vecData[pos].vecFloats.size())-1);
                ofVec4f &vecFloat = t_ofeliaLoadVec4f::vecData[pos].vecFloats[index];
                vecFloat.scale(f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditVec4f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditVec4f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditVec4f::objName);
    }
}

void ofeliaEditVec4f_set(t_ofeliaEditVec4f *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaEditVec4f_print(t_ofeliaEditVec4f *x)
{
    post("\n[%s]", t_ofeliaEditVec4f::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaEditVec4f_setup()
{
    ofeliaEditVec4f_class = class_new(gensym("ofEditVec4f"),
                                      reinterpret_cast<t_newmethod>(ofeliaEditVec4f_new),
                                      0, sizeof(t_ofeliaEditVec4f),
                                      CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaEditVec4f_class, reinterpret_cast<t_method>(ofeliaEditVec4f_float));
    class_addmethod(ofeliaEditVec4f_class, reinterpret_cast<t_method>(ofeliaEditVec4f_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditVec4f_class, reinterpret_cast<t_method>(ofeliaEditVec4f_assign),
                    gensym("assign"), A_GIMME, 0);
    class_addmethod(ofeliaEditVec4f_class, reinterpret_cast<t_method>(ofeliaEditVec4f_assignX),
                    gensym("assignX"), A_FLOAT, 0);
    class_addmethod(ofeliaEditVec4f_class, reinterpret_cast<t_method>(ofeliaEditVec4f_assignY),
                    gensym("assignY"), A_FLOAT, 0);
    class_addmethod(ofeliaEditVec4f_class, reinterpret_cast<t_method>(ofeliaEditVec4f_assignZ),
                    gensym("assignZ"), A_FLOAT, 0);
    class_addmethod(ofeliaEditVec4f_class, reinterpret_cast<t_method>(ofeliaEditVec4f_assignW),
                    gensym("assignW"), A_FLOAT, 0);
    class_addmethod(ofeliaEditVec4f_class, reinterpret_cast<t_method>(ofeliaEditVec4f_add),
                    gensym("add"), A_GIMME, 0);
    class_addmethod(ofeliaEditVec4f_class, reinterpret_cast<t_method>(ofeliaEditVec4f_sub),
                    gensym("sub"), A_GIMME, 0);
    class_addmethod(ofeliaEditVec4f_class, reinterpret_cast<t_method>(ofeliaEditVec4f_mult),
                    gensym("mult"), A_GIMME, 0);
    class_addmethod(ofeliaEditVec4f_class, reinterpret_cast<t_method>(ofeliaEditVec4f_div),
                    gensym("div"), A_GIMME, 0);
    class_addmethod(ofeliaEditVec4f_class, reinterpret_cast<t_method>(ofeliaEditVec4f_lerp),
                    gensym("lerp"), A_GIMME, 0);
    class_addmethod(ofeliaEditVec4f_class, reinterpret_cast<t_method>(ofeliaEditVec4f_limit),
                    gensym("limit"), A_FLOAT, 0);
    class_addmethod(ofeliaEditVec4f_class, reinterpret_cast<t_method>(ofeliaEditVec4f_middle),
                    gensym("middle"), A_GIMME, 0);
    class_addmethod(ofeliaEditVec4f_class, reinterpret_cast<t_method>(ofeliaEditVec4f_normalize),
                    gensym("normalize"), A_NULL, 0);
    class_addmethod(ofeliaEditVec4f_class, reinterpret_cast<t_method>(ofeliaEditVec4f_scale),
                    gensym("scale"), A_FLOAT, 0);
    class_addmethod(ofeliaEditVec4f_class, reinterpret_cast<t_method>(ofeliaEditVec4f_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditVec4f_class, reinterpret_cast<t_method>(ofeliaEditVec4f_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofDoesVec4fNameExist object methods
 */
void *ofeliaDoesVec4fNameExist_new(t_symbol *s)
{
    t_ofeliaDoesVec4fNameExist *x = reinterpret_cast<t_ofeliaDoesVec4fNameExist*>(pd_new(ofeliaDoesVec4fNameExist_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaDoesVec4fNameExist_bang(t_ofeliaDoesVec4fNameExist *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        if (getPositionByVec4fName(name) != -1)
            outlet_float(x->x_obj.ob_outlet, 1.0f);
        else
            outlet_float(x->x_obj.ob_outlet, 0.0f);
    }
    else {
        
        error("%s: name not assigned", t_ofeliaDoesVec4fNameExist::objName);
    }
}

void ofeliaDoesVec4fNameExist_set(t_ofeliaDoesVec4fNameExist *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaDoesVec4fNameExist_print(t_ofeliaDoesVec4fNameExist *x)
{
    post("\n[%s]", t_ofeliaDoesVec4fNameExist::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaDoesVec4fNameExist_setup()
{
    ofeliaDoesVec4fNameExist_class = class_new(gensym("ofDoesVec4fNameExist"),
                                               reinterpret_cast<t_newmethod>(ofeliaDoesVec4fNameExist_new),
                                               0, sizeof(t_ofeliaDoesVec4fNameExist),
                                               CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaDoesVec4fNameExist_class, reinterpret_cast<t_method>(ofeliaDoesVec4fNameExist_bang));
    class_addmethod(ofeliaDoesVec4fNameExist_class, reinterpret_cast<t_method>(ofeliaDoesVec4fNameExist_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesVec4fNameExist_class, reinterpret_cast<t_method>(ofeliaDoesVec4fNameExist_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesVec4fNameExist_class, reinterpret_cast<t_method>(ofeliaDoesVec4fNameExist_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetVec4f object methods
 */
void *ofeliaGetVec4f_new(t_symbol *s)
{
    t_ofeliaGetVec4f *x = reinterpret_cast<t_ofeliaGetVec4f*>(pd_new(ofeliaGetVec4f_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetVec4f_bang(t_ofeliaGetVec4f *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec4fName(name);

        if (pos != -1) {
            
            if (!t_ofeliaLoadVec4f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec4f::vecData[pos].vecFloats.size())-1);
                const ofVec4f &vecFloat = t_ofeliaLoadVec4f::vecData[pos].vecFloats[index];
                t_atom av[4];
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = vecFloat.x;
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = vecFloat.y;
                av[2].a_type = A_FLOAT;
                av[2].a_w.w_float = vecFloat.z;
                av[3].a_type = A_FLOAT;
                av[3].a_w.w_float = vecFloat.w;
                outlet_list(x->x_obj.ob_outlet, &s_list, 4, av);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetVec4f::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetVec4f::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetVec4f::objName);
    }
}

void ofeliaGetVec4f_float(t_ofeliaGetVec4f *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetVec4f_set(t_ofeliaGetVec4f *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetVec4f_print(t_ofeliaGetVec4f *x)
{
    post("\n[%s]", t_ofeliaGetVec4f::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetVec4f_setup()
{
    ofeliaGetVec4f_class = class_new(gensym("ofGetVec4f"),
                                     reinterpret_cast<t_newmethod>(ofeliaGetVec4f_new),
                                     0, sizeof(t_ofeliaGetVec4f),
                                     CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetVec4f_class, reinterpret_cast<t_method>(ofeliaGetVec4f_bang));
    class_addfloat(ofeliaGetVec4f_class, reinterpret_cast<t_method>(ofeliaGetVec4f_float));
    class_addmethod(ofeliaGetVec4f_class, reinterpret_cast<t_method>(ofeliaGetVec4f_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec4f_class, reinterpret_cast<t_method>(ofeliaGetVec4f_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec4f_class, reinterpret_cast<t_method>(ofeliaGetVec4f_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetVec4fs object methods
 */
void *ofeliaGetVec4fs_new(t_symbol *s)
{
    t_ofeliaGetVec4fs *x = reinterpret_cast<t_ofeliaGetVec4fs*>(pd_new(ofeliaGetVec4fs_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetVec4fs_bang(t_ofeliaGetVec4fs *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec4fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec4f::vecData[pos].vecFloats.empty()) {
                
                vector<float> vecFloats;
                
                for (size_t i=0; i<t_ofeliaLoadVec4f::vecData[pos].vecFloats.size(); ++i) {
                    
                    vecFloats.push_back(t_ofeliaLoadVec4f::vecData[pos].vecFloats[i].x);
                    vecFloats.push_back(t_ofeliaLoadVec4f::vecData[pos].vecFloats[i].y);
                    vecFloats.push_back(t_ofeliaLoadVec4f::vecData[pos].vecFloats[i].z);
                    vecFloats.push_back(t_ofeliaLoadVec4f::vecData[pos].vecFloats[i].w);
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
                
                error("%s: '%s' is empty", t_ofeliaGetVec4fs::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetVec4fs::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetVec4fs::objName);
    }
}

void ofeliaGetVec4fs_set(t_ofeliaGetVec4fs *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetVec4fs_print(t_ofeliaGetVec4fs *x)
{
    post("\n[%s]", t_ofeliaGetVec4fs::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetVec4fs_setup()
{
    ofeliaGetVec4fs_class = class_new(gensym("ofGetVec4fs"),
                                      reinterpret_cast<t_newmethod>(ofeliaGetVec4fs_new),
                                      0, sizeof(t_ofeliaGetVec4fs),
                                      CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetVec4fs_class, reinterpret_cast<t_method>(ofeliaGetVec4fs_bang));
    class_addmethod(ofeliaGetVec4fs_class, reinterpret_cast<t_method>(ofeliaGetVec4fs_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec4fs_class, reinterpret_cast<t_method>(ofeliaGetVec4fs_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec4fs_class, reinterpret_cast<t_method>(ofeliaGetVec4fs_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetVec4fAverage object methods
 */
void *ofeliaGetVec4fAverage_new(t_symbol *s)
{
    t_ofeliaGetVec4fAverage *x = reinterpret_cast<t_ofeliaGetVec4fAverage*>(pd_new(ofeliaGetVec4fAverage_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetVec4fAverage_bang(t_ofeliaGetVec4fAverage *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec4fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec4f::vecData[pos].vecFloats.empty()) {
                
                ofVec4f avg;
                avg.average(t_ofeliaLoadVec4f::vecData[pos].vecFloats.data(),
                            t_ofeliaLoadVec4f::vecData[pos].vecFloats.size());
                t_atom av[4];
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = avg.x;
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = avg.y;
                av[2].a_type = A_FLOAT;
                av[2].a_w.w_float = avg.z;
                av[3].a_type = A_FLOAT;
                av[3].a_w.w_float = avg.w;
                outlet_list(x->x_obj.ob_outlet, &s_list, 4, av);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetVec4fAverage::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetVec4fAverage::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetVec4fAverage::objName);
    }
}

void ofeliaGetVec4fAverage_set(t_ofeliaGetVec4fAverage *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetVec4fAverage_print(t_ofeliaGetVec4fAverage *x)
{
    post("\n[%s]", t_ofeliaGetVec4fAverage::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetVec4fAverage_setup()
{
    ofeliaGetVec4fAverage_class = class_new(gensym("ofGetVec4fAverage"),
                                            reinterpret_cast<t_newmethod>(ofeliaGetVec4fAverage_new),
                                            0, sizeof(t_ofeliaGetVec4fAverage),
                                            CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetVec4fAverage_class, reinterpret_cast<t_method>(ofeliaGetVec4fAverage_bang));
    class_addmethod(ofeliaGetVec4fAverage_class, reinterpret_cast<t_method>(ofeliaGetVec4fAverage_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec4fAverage_class, reinterpret_cast<t_method>(ofeliaGetVec4fAverage_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec4fAverage_class, reinterpret_cast<t_method>(ofeliaGetVec4fAverage_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetVec4fDist object methods
 */
void *ofeliaGetVec4fDist_new(t_symbol *s)
{
    t_ofeliaGetVec4fDist *x = reinterpret_cast<t_ofeliaGetVec4fDist*>(pd_new(ofeliaGetVec4fDist_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetVec4fDist_float(t_ofeliaGetVec4fDist *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetVec4fDist_list(t_ofeliaGetVec4fDist *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec4fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec4f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec4f::vecData[pos].vecFloats.size())-1);
                const ofVec4f &vecFloat = t_ofeliaLoadVec4f::vecData[pos].vecFloats[index];
                
                if (argc >= 4) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT &&
                        argv[3].a_type == A_FLOAT) {
                        
                        outlet_float(x->x_obj.ob_outlet, vecFloat.distance(ofVec4f(argv[0].a_w.w_float,
                                                                                   argv[1].a_w.w_float,
                                                                                   argv[2].a_w.w_float,
                                                                                   argv[3].a_w.w_float)));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaGetVec4fDist::objName);
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaGetVec4fDist::objName);
                }
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetVec4fDist::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetVec4fDist::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetVec4fDist::objName);
    }
}

void ofeliaGetVec4fDist_set(t_ofeliaGetVec4fDist *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetVec4fDist_print(t_ofeliaGetVec4fDist *x)
{
    post("\n[%s]", t_ofeliaGetVec4fDist::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetVec4fDist_setup()
{
    ofeliaGetVec4fDist_class = class_new(gensym("ofGetVec4fDist"),
                                         reinterpret_cast<t_newmethod>(ofeliaGetVec4fDist_new),
                                         0, sizeof(t_ofeliaGetVec4fDist),
                                         CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaGetVec4fDist_class, reinterpret_cast<t_method>(ofeliaGetVec4fDist_float));
    class_addlist(ofeliaGetVec4fDist_class, reinterpret_cast<t_method>(ofeliaGetVec4fDist_list));
    class_addmethod(ofeliaGetVec4fDist_class, reinterpret_cast<t_method>(ofeliaGetVec4fDist_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec4fDist_class, reinterpret_cast<t_method>(ofeliaGetVec4fDist_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec4fDist_class, reinterpret_cast<t_method>(ofeliaGetVec4fDist_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetVec4fDistSquared object methods
 */
void *ofeliaGetVec4fDistSquared_new(t_symbol *s)
{
    t_ofeliaGetVec4fDistSquared *x = reinterpret_cast<t_ofeliaGetVec4fDistSquared*>(pd_new(ofeliaGetVec4fDistSquared_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetVec4fDistSquared_float(t_ofeliaGetVec4fDistSquared *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetVec4fDistSquared_list(t_ofeliaGetVec4fDistSquared *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec4fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec4f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec4f::vecData[pos].vecFloats.size())-1);
                const ofVec4f &vecFloat = t_ofeliaLoadVec4f::vecData[pos].vecFloats[index];
                
                if (argc >= 4) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT &&
                        argv[3].a_type == A_FLOAT) {
                        
                        outlet_float(x->x_obj.ob_outlet, vecFloat.squareDistance(ofVec4f(argv[0].a_w.w_float,
                                                                                         argv[1].a_w.w_float,
                                                                                         argv[2].a_w.w_float,
                                                                                         argv[3].a_w.w_float)));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaGetVec4fDistSquared::objName);
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaGetVec4fDistSquared::objName);
                }
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetVec4fDistSquared::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetVec4fDistSquared::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetVec4fDistSquared::objName);
    }
}

void ofeliaGetVec4fDistSquared_set(t_ofeliaGetVec4fDistSquared *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetVec4fDistSquared_print(t_ofeliaGetVec4fDistSquared *x)
{
    post("\n[%s]", t_ofeliaGetVec4fDistSquared::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetVec4fDistSquared_setup()
{
    ofeliaGetVec4fDistSquared_class = class_new(gensym("ofGetVec4fDistSquared"),
                                                reinterpret_cast<t_newmethod>(ofeliaGetVec4fDistSquared_new),
                                                0, sizeof(t_ofeliaGetVec4fDistSquared),
                                                CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaGetVec4fDistSquared_class, reinterpret_cast<t_method>(ofeliaGetVec4fDistSquared_float));
    class_addlist(ofeliaGetVec4fDistSquared_class, reinterpret_cast<t_method>(ofeliaGetVec4fDistSquared_list));
    class_addmethod(ofeliaGetVec4fDistSquared_class, reinterpret_cast<t_method>(ofeliaGetVec4fDistSquared_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec4fDistSquared_class, reinterpret_cast<t_method>(ofeliaGetVec4fDistSquared_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec4fDistSquared_class, reinterpret_cast<t_method>(ofeliaGetVec4fDistSquared_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetVec4fDot object methods
 */
void *ofeliaGetVec4fDot_new(t_symbol *s)
{
    t_ofeliaGetVec4fDot *x = reinterpret_cast<t_ofeliaGetVec4fDot*>(pd_new(ofeliaGetVec4fDot_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetVec4fDot_float(t_ofeliaGetVec4fDot *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetVec4fDot_list(t_ofeliaGetVec4fDot *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec4fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec4f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec4f::vecData[pos].vecFloats.size())-1);
                const ofVec4f &vecFloat = t_ofeliaLoadVec4f::vecData[pos].vecFloats[index];
                
                if (argc >= 4) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT &&
                        argv[3].a_type == A_FLOAT) {
                        
                        outlet_float(x->x_obj.ob_outlet, vecFloat.dot(ofVec4f(argv[0].a_w.w_float,
                                                                              argv[1].a_w.w_float,
                                                                              argv[2].a_w.w_float,
                                                                              argv[3].a_w.w_float)));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaGetVec4fDot::objName);
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaGetVec4fDot::objName);
                }
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetVec4fDot::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetVec4fDot::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetVec4fDot::objName);
    }
}

void ofeliaGetVec4fDot_set(t_ofeliaGetVec4fDot *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetVec4fDot_print(t_ofeliaGetVec4fDot *x)
{
    post("\n[%s]", t_ofeliaGetVec4fDot::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetVec4fDot_setup()
{
    ofeliaGetVec4fDot_class = class_new(gensym("ofGetVec4fDot"),
                                        reinterpret_cast<t_newmethod>(ofeliaGetVec4fDot_new),
                                        0, sizeof(t_ofeliaGetVec4fDot),
                                        CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaGetVec4fDot_class, reinterpret_cast<t_method>(ofeliaGetVec4fDot_float));
    class_addlist(ofeliaGetVec4fDot_class, reinterpret_cast<t_method>(ofeliaGetVec4fDot_list));
    class_addmethod(ofeliaGetVec4fDot_class, reinterpret_cast<t_method>(ofeliaGetVec4fDot_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec4fDot_class, reinterpret_cast<t_method>(ofeliaGetVec4fDot_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec4fDot_class, reinterpret_cast<t_method>(ofeliaGetVec4fDot_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetVec4fLength object methods
 */
void *ofeliaGetVec4fLength_new(t_symbol *s)
{
    t_ofeliaGetVec4fLength *x = reinterpret_cast<t_ofeliaGetVec4fLength*>(pd_new(ofeliaGetVec4fLength_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetVec4fLength_bang(t_ofeliaGetVec4fLength *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec4fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec4f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec4f::vecData[pos].vecFloats.size())-1);
                const ofVec4f &vecFloat = t_ofeliaLoadVec4f::vecData[pos].vecFloats[index];
                outlet_float(x->x_obj.ob_outlet, vecFloat.length());
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetVec4fLength::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetVec4fLength::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetVec4fLength::objName);
    }
}

void ofeliaGetVec4fLength_float(t_ofeliaGetVec4fLength *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetVec4fLength_set(t_ofeliaGetVec4fLength *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetVec4fLength_print(t_ofeliaGetVec4fLength *x)
{
    post("\n[%s]", t_ofeliaGetVec4fLength::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetVec4fLength_setup()
{
    ofeliaGetVec4fLength_class = class_new(gensym("ofGetVec4fLength"),
                                           reinterpret_cast<t_newmethod>(ofeliaGetVec4fLength_new),
                                           0, sizeof(t_ofeliaGetVec4fLength),
                                           CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetVec4fLength_class, reinterpret_cast<t_method>(ofeliaGetVec4fLength_bang));
    class_addfloat(ofeliaGetVec4fLength_class, reinterpret_cast<t_method>(ofeliaGetVec4fLength_float));
    class_addmethod(ofeliaGetVec4fLength_class, reinterpret_cast<t_method>(ofeliaGetVec4fLength_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec4fLength_class, reinterpret_cast<t_method>(ofeliaGetVec4fLength_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec4fLength_class, reinterpret_cast<t_method>(ofeliaGetVec4fLength_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetVec4fLengthSquared object methods
 */
void *ofeliaGetVec4fLengthSquared_new(t_symbol *s)
{
    t_ofeliaGetVec4fLengthSquared *x = reinterpret_cast<t_ofeliaGetVec4fLengthSquared*>(pd_new(ofeliaGetVec4fLengthSquared_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetVec4fLengthSquared_bang(t_ofeliaGetVec4fLengthSquared *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByVec4fName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadVec4f::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadVec4f::vecData[pos].vecFloats.size())-1);
                const ofVec4f &vecFloat = t_ofeliaLoadVec4f::vecData[pos].vecFloats[index];
                outlet_float(x->x_obj.ob_outlet, vecFloat.lengthSquared());
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetVec4fLengthSquared::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetVec4fLengthSquared::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetVec4fLengthSquared::objName);
    }
}

void ofeliaGetVec4fLengthSquared_float(t_ofeliaGetVec4fLengthSquared *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetVec4fLengthSquared_set(t_ofeliaGetVec4fLengthSquared *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetVec4fLengthSquared_print(t_ofeliaGetVec4fLengthSquared *x)
{
    post("\n[%s]", t_ofeliaGetVec4fLengthSquared::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetVec4fLengthSquared_setup()
{
    ofeliaGetVec4fLengthSquared_class = class_new(gensym("ofGetVec4fLengthSquared"),
                                                  reinterpret_cast<t_newmethod>(ofeliaGetVec4fLengthSquared_new),
                                                  0, sizeof(t_ofeliaGetVec4fLengthSquared),
                                                  CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetVec4fLengthSquared_class, reinterpret_cast<t_method>(ofeliaGetVec4fLengthSquared_bang));
    class_addfloat(ofeliaGetVec4fLengthSquared_class, reinterpret_cast<t_method>(ofeliaGetVec4fLengthSquared_float));
    class_addmethod(ofeliaGetVec4fLengthSquared_class, reinterpret_cast<t_method>(ofeliaGetVec4fLengthSquared_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec4fLengthSquared_class, reinterpret_cast<t_method>(ofeliaGetVec4fLengthSquared_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetVec4fLengthSquared_class, reinterpret_cast<t_method>(ofeliaGetVec4fLengthSquared_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * setup methods
 */
void ofeliaVec4f_setup()
{
    ofeliaLoadVec4f_setup();
    ofeliaEditVec4f_setup();
    ofeliaDoesVec4fNameExist_setup();
    ofeliaGetVec4f_setup();
    ofeliaGetVec4fs_setup();
    ofeliaGetVec4fAverage_setup();
    ofeliaGetVec4fDist_setup();
    ofeliaGetVec4fDistSquared_setup();
    ofeliaGetVec4fDot_setup();
    ofeliaGetVec4fLength_setup();
    ofeliaGetVec4fLengthSquared_setup();
}

