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

#include "ofeliaFloat.h"

/* ________________________________________________________________________________
 * static variables
 */
const char *t_ofeliaLoadFloat::objName = "ofLoadFloat";
unsigned int t_ofeliaLoadFloat::counter;
vector<t_ofeliaLoadFloatData> t_ofeliaLoadFloat::vecData;
const char *t_ofeliaEditFloat::objName = "ofEditFloat";
const char *t_ofeliaDoesFloatNameExist::objName = "ofDoesFloatNameExist";
const char *t_ofeliaGetFloat::objName = "ofGetFloat";
const char *t_ofeliaGetFloats::objName = "ofGetFloats";
const char *t_ofeliaGetFloatAverage::objName = "ofGetFloatAverage";

/* ________________________________________________________________________________
 * ofLoadFloat object methods
 */
void ofeliaLoadFloat_vecSizeOut(t_ofeliaLoadFloat *x);

int getPositionByFloatObjID(const unsigned int objID)
{
    const auto it = find_if(begin(t_ofeliaLoadFloat::vecData), end(t_ofeliaLoadFloat::vecData),
                            [&objID](const t_ofeliaLoadFloatData &vecData)
                            {
                                return vecData.objID == objID;
                            });
    return static_cast<int>(it - t_ofeliaLoadFloat::vecData.begin());
}

bool getLoadFloatFloatFromArgs(const int argc, const t_atom *argv, float &vecFloat)
{
    if (argc) {
        
        if (argv[0].a_type == A_FLOAT) {
            
            vecFloat = argv[0].a_w.w_float;
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadFloat::objName);
            return 0;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadFloat::objName);
        return 0;
    }
    return 1;
}

bool getLoadFloatFloatsFromArgs(const int argc, const t_atom *argv, vector<float> &vecFloats)
{
    for (int i=0; i<argc; ++i) {
        
        if (argv[i].a_type == A_FLOAT) {
            
            vecFloats.push_back(argv[i].a_w.w_float);
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadFloat::objName);
            return 0;
        }
    }
    return 1;
}

bool getLoadFloatCmdRangeFromArgs(const int argc, const t_atom *argv, t_ofeliaLoadFloatCmdData &cmd)
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
                
                error("%s: wrong argument type", t_ofeliaLoadFloat::objName);
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
                
                error("%s: wrong argument type", t_ofeliaLoadFloat::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaLoadFloat::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaLoadFloat_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadFloat *x = reinterpret_cast<t_ofeliaLoadFloat*>(pd_new(ofeliaLoadFloat_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    t_ofeliaLoadFloatData vecData;
    getVarNameLocalPrefixes(vecData.varName);
    vector<float> vecFloats;
    
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            vecData.varName.name = argv[0].a_w.w_symbol->s_name;
            getVarNameLocalized(vecData.varName);
            
            if (--argc) {
                
                if (!getLoadFloatFloatsFromArgs(argc, argv+1, vecFloats))
                    return 0;
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadFloat::objName);
            return 0;
        }
    }
    x->objID = vecData.objID = t_ofeliaLoadFloat::counter++;
    t_ofeliaLoadFloat::vecData.push_back(vecData);
    x->vecSize = 0;
    x->vecSizeOutClock = clock_new(x, reinterpret_cast<t_method>(ofeliaLoadFloat_vecSizeOut));
    outlet_new(&x->x_obj, &s_float);
    
    if (argc) {
        
        const int pos = getPositionByFloatObjID(x->objID);
        t_ofeliaLoadFloat::vecData[pos].vecFloats = vecFloats;
        x->vecSize = static_cast<int>(vecFloats.size());
        clock_delay(x->vecSizeOutClock, 0.0);
    }
    return (x);
}

void ofeliaLoadFloat_vecSizeOut(t_ofeliaLoadFloat *x)
{
    outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(x->vecSize));
}

void ofeliaLoadFloat_name(t_ofeliaLoadFloat *x, t_symbol *s)
{
    const int pos = getPositionByFloatObjID(x->objID);
    t_ofeliaLoadFloat::vecData[pos].varName.name = s->s_name;
    getVarNameLocalized(t_ofeliaLoadFloat::vecData[pos].varName);
}

void ofeliaLoadFloat_load(t_ofeliaLoadFloat *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<float> vecFloats;
    
    if (getLoadFloatFloatsFromArgs(argc, argv, vecFloats)) {

        const int pos = getPositionByFloatObjID(x->objID);
        t_ofeliaLoadFloat::vecData[pos].vecFloats = vecFloats;
        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(vecFloats.size()));
    }
}

void ofeliaLoadFloat_add(t_ofeliaLoadFloat *x, t_symbol *s, int argc, t_atom *argv)
{
    float vecFloat;
    
    if (getLoadFloatFloatFromArgs(argc, argv, vecFloat)) {
        
        const int pos = getPositionByFloatObjID(x->objID);
        t_ofeliaLoadFloat::vecData[pos].vecFloats.push_back(vecFloat);
        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadFloat::vecData[pos].vecFloats.size()));
    }
}

void ofeliaLoadFloat_append(t_ofeliaLoadFloat *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<float> vecFloats;
    
    if (getLoadFloatFloatsFromArgs(argc, argv, vecFloats)) {
        
        const int pos = getPositionByFloatObjID(x->objID);
        t_ofeliaLoadFloat::vecData[pos].vecFloats.insert(t_ofeliaLoadFloat::vecData[pos].vecFloats.end(),
                                                         vecFloats.begin(), vecFloats.end());
        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadFloat::vecData[pos].vecFloats.size()));
    }
}

void ofeliaLoadFloat_prepend(t_ofeliaLoadFloat *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<float> vecFloats;
    
    if (getLoadFloatFloatsFromArgs(argc, argv, vecFloats)) {
        
        const int pos = getPositionByFloatObjID(x->objID);
        t_ofeliaLoadFloat::vecData[pos].vecFloats.insert(t_ofeliaLoadFloat::vecData[pos].vecFloats.begin(),
                                                         vecFloats.begin(), vecFloats.end());
        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadFloat::vecData[pos].vecFloats.size()));
    }
}

void ofeliaLoadFloat_insert(t_ofeliaLoadFloat *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadFloatCmdData cmd;
    
    if (getLoadFloatFloatFromArgs(argc, argv, cmd.vecFloat)) {
        
        if (getLoadFloatCmdRangeFromArgs(argc-1, argv+1, cmd)) {
            
            const int pos = getPositionByFloatObjID(x->objID);
            const int size = static_cast<int>(t_ofeliaLoadFloat::vecData[pos].vecFloats.size());
            const auto floatsBegin = t_ofeliaLoadFloat::vecData[pos].vecFloats.begin();
            const int fromIndex = static_cast<int>(ofClamp(cmd.fromIndex, 0.0f, size));
            const int insertSize = static_cast<int>(max(0.0f, max(0.0f, cmd.toIndex) - max(0.0f, cmd.fromIndex))) + 1;
            t_ofeliaLoadFloat::vecData[pos].vecFloats.insert(floatsBegin + fromIndex,
                                                          insertSize, cmd.vecFloat);
            outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadFloat::vecData[pos].vecFloats.size()));
        }
    }
}

void ofeliaLoadFloat_fill(t_ofeliaLoadFloat *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadFloatCmdData cmd;
    
    if (getLoadFloatFloatFromArgs(argc, argv, cmd.vecFloat)) {
        
        if (getLoadFloatCmdRangeFromArgs(argc-1, argv+1, cmd)) {
            
            const int pos = getPositionByFloatObjID(x->objID);
            const int size = static_cast<int>(t_ofeliaLoadFloat::vecData[pos].vecFloats.size());
            const auto floatsBegin = t_ofeliaLoadFloat::vecData[pos].vecFloats.begin();
            
            if (size) {
                
                const int maxIndex = size - 1;
                const int fromIndex = static_cast<int>(ofClamp(cmd.fromIndex, 0.0f, maxIndex));
                const int toIndex = static_cast<int>(ofClamp(cmd.toIndex, fromIndex, maxIndex)) + 1;
                fill(floatsBegin + fromIndex, floatsBegin + toIndex, cmd.vecFloat);
                outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadFloat::vecData[pos].vecFloats.size()));
            }
            else {
                
                outlet_float(x->x_obj.ob_outlet, 0.0f);
            }
        }
    }
}

void ofeliaLoadFloat_erase(t_ofeliaLoadFloat *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadFloatCmdData cmd;
    
    if (getLoadFloatCmdRangeFromArgs(argc, argv, cmd)) {
        
        const int pos = getPositionByFloatObjID(x->objID);
        const int size = static_cast<int>(t_ofeliaLoadFloat::vecData[pos].vecFloats.size());
        const auto floatsBegin = t_ofeliaLoadFloat::vecData[pos].vecFloats.begin();
        
        if (size) {
            
            const int maxIndex = size - 1;
            const int fromIndex = static_cast<int>(ofClamp(cmd.fromIndex, 0.0f, maxIndex));
            const int toIndex = static_cast<int>(ofClamp(cmd.toIndex, fromIndex, maxIndex)) + 1;
            t_ofeliaLoadFloat::vecData[pos].vecFloats.erase(floatsBegin + fromIndex,
                                                            floatsBegin + toIndex);
            outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadFloat::vecData[pos].vecFloats.size()));
        }
        else {
            
            outlet_float(x->x_obj.ob_outlet, 0.0f);
        }
    }
}

void ofeliaLoadFloat_clear(t_ofeliaLoadFloat *x)
{
    const int pos = getPositionByFloatObjID(x->objID);
    t_ofeliaLoadFloat::vecData[pos].vecFloats.clear();
    outlet_float(x->x_obj.ob_outlet, 0.0f);
}

void ofeliaLoadFloat_set(t_ofeliaLoadFloat *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            ofeliaLoadFloat_name(x, argv[0].a_w.w_symbol);
            
            if (--argc)
                ofeliaLoadFloat_load(x, 0, argc, argv+1);
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadFloat::objName);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadFloat::objName);
    }
}

void ofeliaLoadFloat_print(t_ofeliaLoadFloat *x)
{
    post("\n[%s]", t_ofeliaLoadFloat::objName);
    const int pos = getPositionByFloatObjID(x->objID);
    post("name : %s", t_ofeliaLoadFloat::vecData[pos].varName.name.c_str());
    post("load : ");
    
    for (size_t i=0; i<t_ofeliaLoadFloat::vecData[pos].vecFloats.size(); ++i) {
        
        post(" %g", t_ofeliaLoadFloat::vecData[pos].vecFloats[i]);
    }
}

void ofeliaLoadFloat_free(t_ofeliaLoadFloat *x)
{
    clock_free(x->vecSizeOutClock);
    const int pos = getPositionByFloatObjID(x->objID);
    t_ofeliaLoadFloat::vecData.erase(t_ofeliaLoadFloat::vecData.begin() + pos);
}

void ofeliaLoadFloat_setup()
{
    ofeliaLoadFloat_class = class_new(gensym("ofLoadFloat"),
                                      reinterpret_cast<t_newmethod>(ofeliaLoadFloat_new),
                                      reinterpret_cast<t_method>(ofeliaLoadFloat_free),
                                      sizeof(t_ofeliaLoadFloat),
                                      CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaLoadFloat_class, reinterpret_cast<t_method>(ofeliaLoadFloat_name),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaLoadFloat_class, reinterpret_cast<t_method>(ofeliaLoadFloat_load),
                    gensym("load"), A_GIMME, 0);
    class_addmethod(ofeliaLoadFloat_class, reinterpret_cast<t_method>(ofeliaLoadFloat_add),
                    gensym("add"), A_GIMME, 0);
    class_addmethod(ofeliaLoadFloat_class, reinterpret_cast<t_method>(ofeliaLoadFloat_append),
                    gensym("append"), A_GIMME, 0);
    class_addmethod(ofeliaLoadFloat_class, reinterpret_cast<t_method>(ofeliaLoadFloat_prepend),
                    gensym("prepend"), A_GIMME, 0);
    class_addmethod(ofeliaLoadFloat_class, reinterpret_cast<t_method>(ofeliaLoadFloat_insert),
                    gensym("insert"), A_GIMME, 0);
    class_addmethod(ofeliaLoadFloat_class, reinterpret_cast<t_method>(ofeliaLoadFloat_fill),
                    gensym("fill"), A_GIMME, 0);
    class_addmethod(ofeliaLoadFloat_class, reinterpret_cast<t_method>(ofeliaLoadFloat_erase),
                    gensym("erase"), A_GIMME, 0);
    class_addmethod(ofeliaLoadFloat_class, reinterpret_cast<t_method>(ofeliaLoadFloat_clear),
                    gensym("clear"), A_NULL, 0);
    class_addmethod(ofeliaLoadFloat_class, reinterpret_cast<t_method>(ofeliaLoadFloat_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaLoadFloat_class, reinterpret_cast<t_method>(ofeliaLoadFloat_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * common methods
 */
int getPositionByFloatName(const t_string &name)
{
    auto it = find_if(begin(t_ofeliaLoadFloat::vecData), end(t_ofeliaLoadFloat::vecData),
                      [&name] (const t_ofeliaLoadFloatData &vecData) {
                          return (vecData.varName.name == name);
                      });
    if (it == end(t_ofeliaLoadFloat::vecData))
        return -1;
    return static_cast<int>(it - begin(t_ofeliaLoadFloat::vecData));
}

/* ________________________________________________________________________________
 * ofEditFloat object methods
 */
void *ofeliaEditFloat_new(t_symbol *s)
{
    t_ofeliaEditFloat *x = reinterpret_cast<t_ofeliaEditFloat*>(pd_new(ofeliaEditFloat_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaEditFloat_float(t_ofeliaEditFloat *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaEditFloat_assign(t_ofeliaEditFloat *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByFloatName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadFloat::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadFloat::vecData[pos].vecFloats.size())-1);
                t_ofeliaLoadFloat::vecData[pos].vecFloats[index] = f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditFloat::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditFloat::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditFloat::objName);
    }
}

void ofeliaEditFloat_add(t_ofeliaEditFloat *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByFloatName(name);

        if (pos != -1) {
            
            if (!t_ofeliaLoadFloat::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadFloat::vecData[pos].vecFloats.size())-1);
                t_ofeliaLoadFloat::vecData[pos].vecFloats[index] += f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditFloat::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditFloat::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditFloat::objName);
    }
}

void ofeliaEditFloat_sub(t_ofeliaEditFloat *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {

        const int pos = getPositionByFloatName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadFloat::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadFloat::vecData[pos].vecFloats.size())-1);
                t_ofeliaLoadFloat::vecData[pos].vecFloats[index] -= f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditFloat::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditFloat::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditFloat::objName);
    }
}

void ofeliaEditFloat_mult(t_ofeliaEditFloat *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByFloatName(name);

        if (pos != -1) {
            
            if (!t_ofeliaLoadFloat::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadFloat::vecData[pos].vecFloats.size())-1);
                t_ofeliaLoadFloat::vecData[pos].vecFloats[index] *= f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditFloat::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditFloat::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditFloat::objName);
    }
}

void ofeliaEditFloat_div(t_ofeliaEditFloat *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByFloatName(name);

        if (pos != -1) {
            
            if (!t_ofeliaLoadFloat::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadFloat::vecData[pos].vecFloats.size())-1);
                t_ofeliaLoadFloat::vecData[pos].vecFloats[index] /= f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditFloat::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditFloat::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditFloat::objName);
    }
}

void ofeliaEditFloat_lerp(t_ofeliaEditFloat *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {

        const int pos = getPositionByFloatName(name);

        if (pos != -1) {
            
            if (!t_ofeliaLoadFloat::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadFloat::vecData[pos].vecFloats.size())-1);
                float &vecFloat = t_ofeliaLoadFloat::vecData[pos].vecFloats[index];
                
                if (argc == 2) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT) {

                        const float p = argv[1].a_w.w_float;
                        vecFloat = vecFloat * (1-p) + argv[0].a_w.w_float * p;
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditFloat::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditFloat::objName);
                    return;
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditFloat::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditFloat::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditFloat::objName);
    }
}

void ofeliaEditFloat_middle(t_ofeliaEditFloat *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {

        const int pos = getPositionByFloatName(name);

        if (pos != -1) {
            
            if (!t_ofeliaLoadFloat::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadFloat::vecData[pos].vecFloats.size())-1);
                float &vecFloat = t_ofeliaLoadFloat::vecData[pos].vecFloats[index];
                vecFloat = (vecFloat + f) * 0.5f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditFloat::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditFloat::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditFloat::objName);
    }
}

void ofeliaEditFloat_set(t_ofeliaEditFloat *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaEditFloat_print(t_ofeliaEditFloat *x)
{
    post("\n[%s]", t_ofeliaEditFloat::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaEditFloat_setup()
{
    ofeliaEditFloat_class = class_new(gensym("ofEditFloat"),
                                      reinterpret_cast<t_newmethod>(ofeliaEditFloat_new),
                                      0, sizeof(t_ofeliaEditFloat),
                                      CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaEditFloat_class, reinterpret_cast<t_method>(ofeliaEditFloat_float));
    class_addmethod(ofeliaEditFloat_class, reinterpret_cast<t_method>(ofeliaEditFloat_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditFloat_class, reinterpret_cast<t_method>(ofeliaEditFloat_assign),
                    gensym("assign"), A_FLOAT, 0);
    class_addmethod(ofeliaEditFloat_class, reinterpret_cast<t_method>(ofeliaEditFloat_add),
                    gensym("add"), A_FLOAT, 0);
    class_addmethod(ofeliaEditFloat_class, reinterpret_cast<t_method>(ofeliaEditFloat_sub),
                    gensym("sub"), A_FLOAT, 0);
    class_addmethod(ofeliaEditFloat_class, reinterpret_cast<t_method>(ofeliaEditFloat_mult),
                    gensym("mult"), A_FLOAT, 0);
    class_addmethod(ofeliaEditFloat_class, reinterpret_cast<t_method>(ofeliaEditFloat_div),
                    gensym("div"), A_FLOAT, 0);
    class_addmethod(ofeliaEditFloat_class, reinterpret_cast<t_method>(ofeliaEditFloat_lerp),
                    gensym("lerp"), A_GIMME, 0);
    class_addmethod(ofeliaEditFloat_class, reinterpret_cast<t_method>(ofeliaEditFloat_middle),
                    gensym("middle"), A_FLOAT, 0);
    class_addmethod(ofeliaEditFloat_class, reinterpret_cast<t_method>(ofeliaEditFloat_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditFloat_class, reinterpret_cast<t_method>(ofeliaEditFloat_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofDoesFloatNameExist object methods
 */
void *ofeliaDoesFloatNameExist_new(t_symbol *s)
{
    t_ofeliaDoesFloatNameExist *x = reinterpret_cast<t_ofeliaDoesFloatNameExist*>(pd_new(ofeliaDoesFloatNameExist_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaDoesFloatNameExist_bang(t_ofeliaDoesFloatNameExist *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        if (getPositionByFloatName(name) != -1)
            outlet_float(x->x_obj.ob_outlet, 1.0f);
        else
            outlet_float(x->x_obj.ob_outlet, 0.0f);
    }
    else {
        
        error("%s: name not assigned", t_ofeliaDoesFloatNameExist::objName);
    }
}

void ofeliaDoesFloatNameExist_set(t_ofeliaDoesFloatNameExist *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaDoesFloatNameExist_print(t_ofeliaDoesFloatNameExist *x)
{
    post("\n[%s]", t_ofeliaDoesFloatNameExist::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaDoesFloatNameExist_setup()
{
    ofeliaDoesFloatNameExist_class = class_new(gensym("ofDoesFloatNameExist"),
                                              reinterpret_cast<t_newmethod>(ofeliaDoesFloatNameExist_new),
                                              0, sizeof(t_ofeliaDoesFloatNameExist),
                                              CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaDoesFloatNameExist_class, reinterpret_cast<t_method>(ofeliaDoesFloatNameExist_bang));
    class_addmethod(ofeliaDoesFloatNameExist_class, reinterpret_cast<t_method>(ofeliaDoesFloatNameExist_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesFloatNameExist_class, reinterpret_cast<t_method>(ofeliaDoesFloatNameExist_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesFloatNameExist_class, reinterpret_cast<t_method>(ofeliaDoesFloatNameExist_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetFloat object methods
 */
void *ofeliaGetFloat_new(t_symbol *s)
{
    t_ofeliaGetFloat *x = reinterpret_cast<t_ofeliaGetFloat*>(pd_new(ofeliaGetFloat_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetFloat_bang(t_ofeliaGetFloat *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByFloatName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadFloat::vecData[pos].vecFloats.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadFloat::vecData[pos].vecFloats.size())-1);
                outlet_float(x->x_obj.ob_outlet, t_ofeliaLoadFloat::vecData[pos].vecFloats[index]);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetFloat::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetFloat::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetFloat::objName);
    }
}

void ofeliaGetFloat_float(t_ofeliaGetFloat *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetFloat_set(t_ofeliaGetFloat *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetFloat_print(t_ofeliaGetFloat *x)
{
    post("\n[%s]", t_ofeliaGetFloat::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetFloat_setup()
{
    ofeliaGetFloat_class = class_new(gensym("ofGetFloat"),
                                     reinterpret_cast<t_newmethod>(ofeliaGetFloat_new),
                                     0, sizeof(t_ofeliaGetFloat),
                                     CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetFloat_class, reinterpret_cast<t_method>(ofeliaGetFloat_bang));
    class_addfloat(ofeliaGetFloat_class, reinterpret_cast<t_method>(ofeliaGetFloat_float));
    class_addmethod(ofeliaGetFloat_class, reinterpret_cast<t_method>(ofeliaGetFloat_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetFloat_class, reinterpret_cast<t_method>(ofeliaGetFloat_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetFloat_class, reinterpret_cast<t_method>(ofeliaGetFloat_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetFloats object methods
 */
void *ofeliaGetFloats_new(t_symbol *s)
{
    t_ofeliaGetFloats *x = reinterpret_cast<t_ofeliaGetFloats*>(pd_new(ofeliaGetFloats_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetFloats_bang(t_ofeliaGetFloats *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByFloatName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadFloat::vecData[pos].vecFloats.empty()) {
                
                const int ac = static_cast<int>(t_ofeliaLoadFloat::vecData[pos].vecFloats.size());
                t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
                
                for (int i=0; i<ac; ++i) {
                    
                    av[i].a_type = A_FLOAT;
                    av[i].a_w.w_float = t_ofeliaLoadFloat::vecData[pos].vecFloats[i];
                }
                outlet_list(x->x_obj.ob_outlet, &s_list, ac, av);
                freebytes(av, ac * sizeof(t_atom));
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetFloats::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetFloats::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetFloats::objName);
    }
}

void ofeliaGetFloats_set(t_ofeliaGetFloats *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetFloats_print(t_ofeliaGetFloats *x)
{
    post("\n[%s]", t_ofeliaGetFloats::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetFloats_setup()
{
    ofeliaGetFloats_class = class_new(gensym("ofGetFloats"),
                                      reinterpret_cast<t_newmethod>(ofeliaGetFloats_new),
                                      0, sizeof(t_ofeliaGetFloats),
                                      CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetFloats_class, reinterpret_cast<t_method>(ofeliaGetFloats_bang));
    class_addmethod(ofeliaGetFloats_class, reinterpret_cast<t_method>(ofeliaGetFloats_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetFloats_class, reinterpret_cast<t_method>(ofeliaGetFloats_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetFloats_class, reinterpret_cast<t_method>(ofeliaGetFloats_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetFloatAverage object methods
 */
void *ofeliaGetFloatAverage_new(t_symbol *s)
{
    t_ofeliaGetFloatAverage *x = reinterpret_cast<t_ofeliaGetFloatAverage*>(pd_new(ofeliaGetFloatAverage_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetFloatAverage_bang(t_ofeliaGetFloatAverage *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByFloatName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadFloat::vecData[pos].vecFloats.empty()) {
                
                const size_t vecSize = t_ofeliaLoadFloat::vecData[pos].vecFloats.size();
                float sum = 0.0f;

                for( int i=0; i<vecSize; i++)
                    sum += t_ofeliaLoadFloat::vecData[pos].vecFloats[i];
                sum /= vecSize;
                outlet_float(x->x_obj.ob_outlet, sum);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetFloatAverage::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetFloatAverage::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetFloatAverage::objName);
    }
}

void ofeliaGetFloatAverage_set(t_ofeliaGetFloatAverage *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetFloatAverage_print(t_ofeliaGetFloatAverage *x)
{
    post("\n[%s]", t_ofeliaGetFloatAverage::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetFloatAverage_setup()
{
    ofeliaGetFloatAverage_class = class_new(gensym("ofGetFloatAverage"),
                                            reinterpret_cast<t_newmethod>(ofeliaGetFloatAverage_new),
                                            0, sizeof(t_ofeliaGetFloatAverage),
                                            CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetFloatAverage_class, reinterpret_cast<t_method>(ofeliaGetFloatAverage_bang));
    class_addmethod(ofeliaGetFloatAverage_class, reinterpret_cast<t_method>(ofeliaGetFloatAverage_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetFloatAverage_class, reinterpret_cast<t_method>(ofeliaGetFloatAverage_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetFloatAverage_class, reinterpret_cast<t_method>(ofeliaGetFloatAverage_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * setup methods
 */
void ofeliaFloat_setup()
{
    ofeliaLoadFloat_setup();
    ofeliaEditFloat_setup();
    ofeliaDoesFloatNameExist_setup();
    ofeliaGetFloat_setup();
    ofeliaGetFloats_setup();
    ofeliaGetFloatAverage_setup();
}

