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

#include "ofeliaColor.h"

/* ________________________________________________________________________________
 * static variables
 */
const char *t_ofeliaLoadColor::objName = "ofLoadColor";
unsigned int t_ofeliaLoadColor::counter;
vector<t_ofeliaLoadColorData> t_ofeliaLoadColor::colorData;
const char *t_ofeliaEditColor::objName = "ofEditColor";
const char *t_ofeliaDoesColorNameExist::objName = "ofDoesColorNameExist";
const char *t_ofeliaGetColor::objName = "ofGetColor";
const char *t_ofeliaGetColors::objName = "ofGetColors";

/* ________________________________________________________________________________
 * ofLoadColor object methods
 */
void ofeliaLoadColor_vecSizeOut(t_ofeliaLoadColor *x);

int getPositionByColorObjID(const unsigned int objID)
{
    const auto it = find_if(begin(t_ofeliaLoadColor::colorData), end(t_ofeliaLoadColor::colorData),
                            [&objID](const t_ofeliaLoadColorData &colorData)
                            {
                                return colorData.objID == objID;
                            });
    return static_cast<int>(it - t_ofeliaLoadColor::colorData.begin());
}

bool getLoadColorFromArgs(const int argc, const t_atom *argv, ofColor &color)
{
    if (argc) {
        
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
            
            error("%s: wrong argument type", t_ofeliaLoadColor::objName);
            return 0;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadColor::objName);
        return 0;
    }
    return 1;
}

bool getLoadColorsFromArgs(const int argc, const t_atom *argv, vector<ofColor> &colors)
{
    if (argc && argc%4 == 0) {
        
        for (int i=0; i<argc; i+=4) {
            
            if (argv[i].a_type == A_FLOAT &&
                argv[i+1].a_type == A_FLOAT &&
                argv[i+2].a_type == A_FLOAT &&
                argv[i+3].a_type == A_FLOAT) {
                
                colors.push_back(ofColor(static_cast<unsigned char>(argv[i].a_w.w_float),
                                         static_cast<unsigned char>(argv[i+1].a_w.w_float),
                                         static_cast<unsigned char>(argv[i+2].a_w.w_float),
                                         static_cast<unsigned char>(argv[i+3].a_w.w_float)));
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaLoadColor::objName);
                return 0;
            }
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadColor::objName);
        return 0;
    }
    return 1;
}

bool getLoadColorCmdRangeFromArgs(const int argc, const t_atom *argv, t_ofeliaLoadColorCmdData &cmd)
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
                
                error("%s: wrong argument type", t_ofeliaLoadColor::objName);
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
                
                error("%s: wrong argument type", t_ofeliaLoadColor::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaLoadColor::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaLoadColor_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadColor *x = reinterpret_cast<t_ofeliaLoadColor*>(pd_new(ofeliaLoadColor_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    t_ofeliaLoadColorData colorData;
    getVarNameLocalPrefixes(colorData.varName);
    vector<ofColor> colors;
    
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            colorData.varName.name = argv[0].a_w.w_symbol->s_name;
            getVarNameLocalized(colorData.varName);
            
            if (--argc) {
                
                if (!getLoadColorsFromArgs(argc, argv+1, colors))
                    return 0;
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadColor::objName);
            return 0;
        }
    }
    x->objID = colorData.objID = t_ofeliaLoadColor::counter++;
    t_ofeliaLoadColor::colorData.push_back(colorData);
    x->vecSize = 0;
    x->vecSizeOutClock = clock_new(x, reinterpret_cast<t_method>(ofeliaLoadColor_vecSizeOut));
    outlet_new(&x->x_obj, &s_float);
    
    if (argc) {
        
        const int pos = getPositionByColorObjID(x->objID);
        t_ofeliaLoadColor::colorData[pos].colors = colors;
        x->vecSize = static_cast<int>(colors.size());
        clock_delay(x->vecSizeOutClock, 0.0);
    }
    return (x);
}

void ofeliaLoadColor_vecSizeOut(t_ofeliaLoadColor *x)
{
    outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(x->vecSize));
}

void ofeliaLoadColor_name(t_ofeliaLoadColor *x, t_symbol *s)
{
    const int pos = getPositionByColorObjID(x->objID);
    t_ofeliaLoadColor::colorData[pos].varName.name = s->s_name;
    getVarNameLocalized(t_ofeliaLoadColor::colorData[pos].varName);
}

void ofeliaLoadColor_load(t_ofeliaLoadColor *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<ofColor> colors;
    
    if (getLoadColorsFromArgs(argc, argv, colors)) {
        
        const int pos = getPositionByColorObjID(x->objID);
        t_ofeliaLoadColor::colorData[pos].colors = colors;
        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(colors.size()));
    }
}

void ofeliaLoadColor_add(t_ofeliaLoadColor *x, t_symbol *s, int argc, t_atom *argv)
{
    ofColor color;
    
    if (getLoadColorFromArgs(argc, argv, color)) {
        
        const int pos = getPositionByColorObjID(x->objID);
        t_ofeliaLoadColor::colorData[pos].colors.push_back(color);
        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadColor::colorData[pos].colors.size()));
    }
}

void ofeliaLoadColor_append(t_ofeliaLoadColor *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<ofColor> colors;
    
    if (getLoadColorsFromArgs(argc, argv, colors)) {
        
        const int pos = getPositionByColorObjID(x->objID);
        t_ofeliaLoadColor::colorData[pos].colors.insert(t_ofeliaLoadColor::colorData[pos].colors.end(),
                                                         colors.begin(), colors.end());
        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadColor::colorData[pos].colors.size()));
    }
}

void ofeliaLoadColor_prepend(t_ofeliaLoadColor *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<ofColor> colors;
    
    if (getLoadColorsFromArgs(argc, argv, colors)) {
        
        const int pos = getPositionByColorObjID(x->objID);
        t_ofeliaLoadColor::colorData[pos].colors.insert(t_ofeliaLoadColor::colorData[pos].colors.begin(),
                                                         colors.begin(), colors.end());
        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadColor::colorData[pos].colors.size()));
    }
}

void ofeliaLoadColor_insert(t_ofeliaLoadColor *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadColorCmdData cmd;
    
    if (getLoadColorFromArgs(argc, argv, cmd.color)) {
        
        if (getLoadColorCmdRangeFromArgs(argc-4, argv+4, cmd)) {
            
            const int pos = getPositionByColorObjID(x->objID);
            const int size = static_cast<int>(t_ofeliaLoadColor::colorData[pos].colors.size());
            const auto colorsBegin = t_ofeliaLoadColor::colorData[pos].colors.begin();
            const int fromIndex = static_cast<int>(ofClamp(cmd.fromIndex, 0.0f, size));
            const int insertSize = static_cast<int>(max(0.0f, max(0.0f, cmd.toIndex) - max(0.0f, cmd.fromIndex))) + 1;
            t_ofeliaLoadColor::colorData[pos].colors.insert(colorsBegin + fromIndex,
                                                             insertSize, cmd.color);
            outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadColor::colorData[pos].colors.size()));
        }
    }
}

void ofeliaLoadColor_fill(t_ofeliaLoadColor *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadColorCmdData cmd;
    
    if (getLoadColorFromArgs(argc, argv, cmd.color)) {
        
        if (getLoadColorCmdRangeFromArgs(argc-4, argv+4, cmd)) {
            
            const int pos = getPositionByColorObjID(x->objID);
            const int size = static_cast<int>(t_ofeliaLoadColor::colorData[pos].colors.size());
            const auto colorsBegin = t_ofeliaLoadColor::colorData[pos].colors.begin();
            
            if (size) {
                
                const int maxIndex = size - 1;
                const int fromIndex = static_cast<int>(ofClamp(cmd.fromIndex, 0.0f, maxIndex));
                const int toIndex = static_cast<int>(ofClamp(cmd.toIndex, fromIndex, maxIndex)) + 1;
                fill(colorsBegin + fromIndex, colorsBegin + toIndex, cmd.color);
                outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadColor::colorData[pos].colors.size()));
            }
            else {
                
                outlet_float(x->x_obj.ob_outlet, 0.0f);
            }
        }
    }
}

void ofeliaLoadColor_erase(t_ofeliaLoadColor *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadColorCmdData cmd;
    
    if (getLoadColorCmdRangeFromArgs(argc, argv, cmd)) {
        
        const int pos = getPositionByColorObjID(x->objID);
        const int size = static_cast<int>(t_ofeliaLoadColor::colorData[pos].colors.size());
        const auto colorsBegin = t_ofeliaLoadColor::colorData[pos].colors.begin();
        
        if (size) {
            
            const int maxIndex = size - 1;
            const int fromIndex = static_cast<int>(ofClamp(cmd.fromIndex, 0.0f, maxIndex));
            const int toIndex = static_cast<int>(ofClamp(cmd.toIndex, fromIndex, maxIndex)) + 1;
            t_ofeliaLoadColor::colorData[pos].colors.erase(colorsBegin + fromIndex,
                                                            colorsBegin + toIndex);
            outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadColor::colorData[pos].colors.size()));
        }
        else {
            
            outlet_float(x->x_obj.ob_outlet, 0.0f);
        }
    }
}

void ofeliaLoadColor_clear(t_ofeliaLoadColor *x)
{
    const int pos = getPositionByColorObjID(x->objID);
    t_ofeliaLoadColor::colorData[pos].colors.clear();
    outlet_float(x->x_obj.ob_outlet, 0.0f);
}

void ofeliaLoadColor_set(t_ofeliaLoadColor *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            ofeliaLoadColor_name(x, argv[0].a_w.w_symbol);
            
            if (--argc)
                ofeliaLoadColor_load(x, 0, argc, argv+1);
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadColor::objName);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadColor::objName);
    }
}

void ofeliaLoadColor_print(t_ofeliaLoadColor *x)
{
    post("\n[%s]", t_ofeliaLoadColor::objName);
    const int pos = getPositionByColorObjID(x->objID);
    post("name : %s", t_ofeliaLoadColor::colorData[pos].varName.name.c_str());
    post("load : ");
    
    for (size_t i=0; i<t_ofeliaLoadColor::colorData[pos].colors.size(); ++i) {
        
        post(" %d %d %d %d", t_ofeliaLoadColor::colorData[pos].colors[i].r, t_ofeliaLoadColor::colorData[pos].colors[i].g, t_ofeliaLoadColor::colorData[pos].colors[i].b, t_ofeliaLoadColor::colorData[pos].colors[i].a);
    }
}

void ofeliaLoadColor_free(t_ofeliaLoadColor *x)
{
    clock_free(x->vecSizeOutClock);
    const int pos = getPositionByColorObjID(x->objID);
    t_ofeliaLoadColor::colorData.erase(t_ofeliaLoadColor::colorData.begin() + pos);
}

void ofeliaLoadColor_setup()
{
    ofeliaLoadColor_class = class_new(gensym("ofLoadColor"),
                                      reinterpret_cast<t_newmethod>(ofeliaLoadColor_new),
                                      reinterpret_cast<t_method>(ofeliaLoadColor_free),
                                      sizeof(t_ofeliaLoadColor),
                                      CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaLoadColor_class, reinterpret_cast<t_method>(ofeliaLoadColor_name),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaLoadColor_class, reinterpret_cast<t_method>(ofeliaLoadColor_load),
                    gensym("load"), A_GIMME, 0);
    class_addmethod(ofeliaLoadColor_class, reinterpret_cast<t_method>(ofeliaLoadColor_add),
                    gensym("add"), A_GIMME, 0);
    class_addmethod(ofeliaLoadColor_class, reinterpret_cast<t_method>(ofeliaLoadColor_append),
                    gensym("append"), A_GIMME, 0);
    class_addmethod(ofeliaLoadColor_class, reinterpret_cast<t_method>(ofeliaLoadColor_prepend),
                    gensym("prepend"), A_GIMME, 0);
    class_addmethod(ofeliaLoadColor_class, reinterpret_cast<t_method>(ofeliaLoadColor_insert),
                    gensym("insert"), A_GIMME, 0);
    class_addmethod(ofeliaLoadColor_class, reinterpret_cast<t_method>(ofeliaLoadColor_fill),
                    gensym("fill"), A_GIMME, 0);
    class_addmethod(ofeliaLoadColor_class, reinterpret_cast<t_method>(ofeliaLoadColor_erase),
                    gensym("erase"), A_GIMME, 0);
    class_addmethod(ofeliaLoadColor_class, reinterpret_cast<t_method>(ofeliaLoadColor_clear),
                    gensym("clear"), A_NULL, 0);
    class_addmethod(ofeliaLoadColor_class, reinterpret_cast<t_method>(ofeliaLoadColor_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaLoadColor_class, reinterpret_cast<t_method>(ofeliaLoadColor_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * common methods
 */
int getPositionByColorName(const t_string &name)
{
    auto it = find_if(begin(t_ofeliaLoadColor::colorData), end(t_ofeliaLoadColor::colorData),
                      [&name] (const t_ofeliaLoadColorData &colorData) {
                          return (colorData.varName.name == name);
                      });
    if (it == end(t_ofeliaLoadColor::colorData))
        return -1;
    return static_cast<int>(it - begin(t_ofeliaLoadColor::colorData));
}

/* ________________________________________________________________________________
 * ofEditColor object methods
 */
void *ofeliaEditColor_new(t_symbol *s)
{
    t_ofeliaEditColor *x = reinterpret_cast<t_ofeliaEditColor*>(pd_new(ofeliaEditColor_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaEditColor_float(t_ofeliaEditColor *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaEditColor_assign(t_ofeliaEditColor *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByColorName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadColor::colorData[pos].colors.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadColor::colorData[pos].colors.size())-1);
                ofColor &color = t_ofeliaLoadColor::colorData[pos].colors[index];
                
                if (!getColorAlphaFromArgs(argc, argv, color, t_ofeliaEditColor::objName))
                    return;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditColor::objName);
    }
}

void ofeliaEditColor_assignR(t_ofeliaEditColor *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByColorName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadColor::colorData[pos].colors.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadColor::colorData[pos].colors.size())-1);
                ofColor &color = t_ofeliaLoadColor::colorData[pos].colors[index];
                color.r = static_cast<unsigned char>(f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditColor::objName);
    }
}

void ofeliaEditColor_assignG(t_ofeliaEditColor *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByColorName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadColor::colorData[pos].colors.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadColor::colorData[pos].colors.size())-1);
                ofColor &color = t_ofeliaLoadColor::colorData[pos].colors[index];
                color.g = static_cast<unsigned char>(f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditColor::objName);
    }
}

void ofeliaEditColor_assignB(t_ofeliaEditColor *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByColorName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadColor::colorData[pos].colors.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadColor::colorData[pos].colors.size())-1);
                ofColor &color = t_ofeliaLoadColor::colorData[pos].colors[index];
                color.b = static_cast<unsigned char>(f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditColor::objName);
    }
}

void ofeliaEditColor_assignA(t_ofeliaEditColor *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByColorName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadColor::colorData[pos].colors.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadColor::colorData[pos].colors.size())-1);
                ofColor &color = t_ofeliaLoadColor::colorData[pos].colors[index];
                color.a = static_cast<unsigned char>(f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditColor::objName);
    }
}

void ofeliaEditColor_add(t_ofeliaEditColor *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByColorName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadColor::colorData[pos].colors.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadColor::colorData[pos].colors.size())-1);
                ofColor &color = t_ofeliaLoadColor::colorData[pos].colors[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditColor::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            color += ofColor(static_cast<unsigned char>(argv[0].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditColor::objName);
                            return;
                        }
                        break;
                    }
                    case 2:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            color += ofColor(static_cast<unsigned char>(argv[0].a_w.w_float),
                                             static_cast<unsigned char>(argv[1].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditColor::objName);
                            return;
                        }
                        break;
                    }
                    case 3:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            color += ofColor(static_cast<unsigned char>(argv[0].a_w.w_float),
                                             static_cast<unsigned char>(argv[1].a_w.w_float),
                                             static_cast<unsigned char>(argv[2].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditColor::objName);
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
                            
                            color += ofColor(static_cast<unsigned char>(argv[0].a_w.w_float),
                                             static_cast<unsigned char>(argv[1].a_w.w_float),
                                             static_cast<unsigned char>(argv[2].a_w.w_float),
                                             static_cast<unsigned char>(argv[3].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditColor::objName);
                            return;
                        }
                        break;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditColor::objName);
    }
}

void ofeliaEditColor_sub(t_ofeliaEditColor *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByColorName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadColor::colorData[pos].colors.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadColor::colorData[pos].colors.size())-1);
                ofColor &color = t_ofeliaLoadColor::colorData[pos].colors[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditColor::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            color -= ofColor(static_cast<unsigned char>(argv[0].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditColor::objName);
                            return;
                        }
                        break;
                    }
                    case 2:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            color -= ofColor(static_cast<unsigned char>(argv[0].a_w.w_float),
                                             static_cast<unsigned char>(argv[1].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditColor::objName);
                            return;
                        }
                        break;
                    }
                    case 3:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            color -= ofColor(static_cast<unsigned char>(argv[0].a_w.w_float),
                                             static_cast<unsigned char>(argv[1].a_w.w_float),
                                             static_cast<unsigned char>(argv[2].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditColor::objName);
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
                            
                            color -= ofColor(static_cast<unsigned char>(argv[0].a_w.w_float),
                                             static_cast<unsigned char>(argv[1].a_w.w_float),
                                             static_cast<unsigned char>(argv[2].a_w.w_float),
                                             static_cast<unsigned char>(argv[3].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditColor::objName);
                            return;
                        }
                        break;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditColor::objName);
    }
}

void ofeliaEditColor_mult(t_ofeliaEditColor *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByColorName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadColor::colorData[pos].colors.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadColor::colorData[pos].colors.size())-1);
                ofColor &color = t_ofeliaLoadColor::colorData[pos].colors[index];
                color *= f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditColor::objName);
    }
}

void ofeliaEditColor_div(t_ofeliaEditColor *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByColorName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadColor::colorData[pos].colors.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadColor::colorData[pos].colors.size())-1);
                ofColor &color = t_ofeliaLoadColor::colorData[pos].colors[index];
                color /= f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditColor::objName);
    }
}

void ofeliaEditColor_invert(t_ofeliaEditColor *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByColorName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadColor::colorData[pos].colors.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadColor::colorData[pos].colors.size())-1);
                ofColor &color = t_ofeliaLoadColor::colorData[pos].colors[index];
                color.invert();
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditColor::objName);
    }
}

void ofeliaEditColor_lerp(t_ofeliaEditColor *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByColorName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadColor::colorData[pos].colors.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadColor::colorData[pos].colors.size())-1);
                ofColor &color = t_ofeliaLoadColor::colorData[pos].colors[index];
                
                if (argc == 5) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT &&
                        argv[3].a_type == A_FLOAT &&
                        argv[4].a_type == A_FLOAT) {
                        
                        color.lerp(ofColor(static_cast<unsigned char>(argv[0].a_w.w_float),
                                           static_cast<unsigned char>(argv[1].a_w.w_float),
                                           static_cast<unsigned char>(argv[2].a_w.w_float),
                                           static_cast<unsigned char>(argv[3].a_w.w_float)),
                                   argv[4].a_w.w_float);
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditColor::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditColor::objName);
                    return;
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditColor::objName);
    }
}

void ofeliaEditColor_normalize(t_ofeliaEditColor *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByColorName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadColor::colorData[pos].colors.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadColor::colorData[pos].colors.size())-1);
                ofColor &color = t_ofeliaLoadColor::colorData[pos].colors[index];
                color.normalize();
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditColor::objName);
    }
}

void ofeliaEditColor_hue(t_ofeliaEditColor *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByColorName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadColor::colorData[pos].colors.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadColor::colorData[pos].colors.size())-1);
                ofColor &color = t_ofeliaLoadColor::colorData[pos].colors[index];
                color.setHue(f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditColor::objName);
    }
}

void ofeliaEditColor_hueAngle(t_ofeliaEditColor *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByColorName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadColor::colorData[pos].colors.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadColor::colorData[pos].colors.size())-1);
                ofColor &color = t_ofeliaLoadColor::colorData[pos].colors[index];
                color.setHueAngle(f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditColor::objName);
    }
}

void ofeliaEditColor_brightness(t_ofeliaEditColor *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByColorName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadColor::colorData[pos].colors.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadColor::colorData[pos].colors.size())-1);
                ofColor &color = t_ofeliaLoadColor::colorData[pos].colors[index];
                color.setBrightness(f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditColor::objName);
    }
}

void ofeliaEditColor_saturation(t_ofeliaEditColor *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByColorName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadColor::colorData[pos].colors.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadColor::colorData[pos].colors.size())-1);
                ofColor &color = t_ofeliaLoadColor::colorData[pos].colors[index];
                color.setSaturation(f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditColor::objName);
    }
}

void ofeliaEditColor_set(t_ofeliaEditColor *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaEditColor_print(t_ofeliaEditColor *x)
{
    post("\n[%s]", t_ofeliaEditColor::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaEditColor_setup()
{
    ofeliaEditColor_class = class_new(gensym("ofEditColor"),
                                      reinterpret_cast<t_newmethod>(ofeliaEditColor_new),
                                      0, sizeof(t_ofeliaEditColor),
                                      CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaEditColor_class, reinterpret_cast<t_method>(ofeliaEditColor_float));
    class_addmethod(ofeliaEditColor_class, reinterpret_cast<t_method>(ofeliaEditColor_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditColor_class, reinterpret_cast<t_method>(ofeliaEditColor_assign),
                    gensym("assign"), A_GIMME, 0);
    class_addmethod(ofeliaEditColor_class, reinterpret_cast<t_method>(ofeliaEditColor_assignR),
                    gensym("assignR"), A_FLOAT, 0);
    class_addmethod(ofeliaEditColor_class, reinterpret_cast<t_method>(ofeliaEditColor_assignG),
                    gensym("assignG"), A_FLOAT, 0);
    class_addmethod(ofeliaEditColor_class, reinterpret_cast<t_method>(ofeliaEditColor_assignB),
                    gensym("assignB"), A_FLOAT, 0);
    class_addmethod(ofeliaEditColor_class, reinterpret_cast<t_method>(ofeliaEditColor_assignA),
                    gensym("assignA"), A_FLOAT, 0);
    class_addmethod(ofeliaEditColor_class, reinterpret_cast<t_method>(ofeliaEditColor_add),
                    gensym("add"), A_GIMME, 0);
    class_addmethod(ofeliaEditColor_class, reinterpret_cast<t_method>(ofeliaEditColor_sub),
                    gensym("sub"), A_GIMME, 0);
    class_addmethod(ofeliaEditColor_class, reinterpret_cast<t_method>(ofeliaEditColor_mult),
                    gensym("mult"), A_FLOAT, 0);
    class_addmethod(ofeliaEditColor_class, reinterpret_cast<t_method>(ofeliaEditColor_div),
                    gensym("div"), A_FLOAT, 0);
    class_addmethod(ofeliaEditColor_class, reinterpret_cast<t_method>(ofeliaEditColor_invert),
                    gensym("invert"), A_NULL, 0);
    class_addmethod(ofeliaEditColor_class, reinterpret_cast<t_method>(ofeliaEditColor_lerp),
                    gensym("lerp"), A_GIMME, 0);
    class_addmethod(ofeliaEditColor_class, reinterpret_cast<t_method>(ofeliaEditColor_normalize),
                    gensym("normalize"), A_NULL, 0);
    class_addmethod(ofeliaEditColor_class, reinterpret_cast<t_method>(ofeliaEditColor_hue),
                    gensym("hue"), A_FLOAT, 0);
    class_addmethod(ofeliaEditColor_class, reinterpret_cast<t_method>(ofeliaEditColor_hueAngle),
                    gensym("hueAngle"), A_FLOAT, 0);
    class_addmethod(ofeliaEditColor_class, reinterpret_cast<t_method>(ofeliaEditColor_brightness),
                    gensym("brightness"), A_FLOAT, 0);
    class_addmethod(ofeliaEditColor_class, reinterpret_cast<t_method>(ofeliaEditColor_saturation),
                    gensym("saturation"), A_FLOAT, 0);
    class_addmethod(ofeliaEditColor_class, reinterpret_cast<t_method>(ofeliaEditColor_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditColor_class, reinterpret_cast<t_method>(ofeliaEditColor_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofDoesColorNameExist object methods
 */
void *ofeliaDoesColorNameExist_new(t_symbol *s)
{
    t_ofeliaDoesColorNameExist *x = reinterpret_cast<t_ofeliaDoesColorNameExist*>(pd_new(ofeliaDoesColorNameExist_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaDoesColorNameExist_bang(t_ofeliaDoesColorNameExist *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        if (getPositionByColorName(name) != -1)
            outlet_float(x->x_obj.ob_outlet, 1.0f);
        else
            outlet_float(x->x_obj.ob_outlet, 0.0f);
    }
    else {
        
        error("%s: name not assigned", t_ofeliaDoesColorNameExist::objName);
    }
}

void ofeliaDoesColorNameExist_set(t_ofeliaDoesColorNameExist *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaDoesColorNameExist_print(t_ofeliaDoesColorNameExist *x)
{
    post("\n[%s]", t_ofeliaDoesColorNameExist::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaDoesColorNameExist_setup()
{
    ofeliaDoesColorNameExist_class = class_new(gensym("ofDoesColorNameExist"),
                                               reinterpret_cast<t_newmethod>(ofeliaDoesColorNameExist_new),
                                               0, sizeof(t_ofeliaDoesColorNameExist),
                                               CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaDoesColorNameExist_class, reinterpret_cast<t_method>(ofeliaDoesColorNameExist_bang));
    class_addmethod(ofeliaDoesColorNameExist_class, reinterpret_cast<t_method>(ofeliaDoesColorNameExist_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesColorNameExist_class, reinterpret_cast<t_method>(ofeliaDoesColorNameExist_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesColorNameExist_class, reinterpret_cast<t_method>(ofeliaDoesColorNameExist_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetColor object methods
 */
void *ofeliaGetColor_new(t_symbol *s)
{
    t_ofeliaGetColor *x = reinterpret_cast<t_ofeliaGetColor*>(pd_new(ofeliaGetColor_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetColor_bang(t_ofeliaGetColor *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByColorName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadColor::colorData[pos].colors.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadColor::colorData[pos].colors.size())-1);
                const ofColor &color = t_ofeliaLoadColor::colorData[pos].colors[index];
                t_atom av[4];
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = static_cast<t_float>(color.r);
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = static_cast<t_float>(color.g);
                av[2].a_type = A_FLOAT;
                av[2].a_w.w_float = static_cast<t_float>(color.b);
                av[3].a_type = A_FLOAT;
                av[3].a_w.w_float = static_cast<t_float>(color.a);
                outlet_list(x->x_obj.ob_outlet, &s_list, 4, av);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetColor::objName);
    }
}

void ofeliaGetColor_float(t_ofeliaGetColor *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetColor_set(t_ofeliaGetColor *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetColor_print(t_ofeliaGetColor *x)
{
    post("\n[%s]", t_ofeliaGetColor::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetColor_setup()
{
    ofeliaGetColor_class = class_new(gensym("ofGetColor"),
                                     reinterpret_cast<t_newmethod>(ofeliaGetColor_new),
                                     0, sizeof(t_ofeliaGetColor),
                                     CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetColor_class, reinterpret_cast<t_method>(ofeliaGetColor_bang));
    class_addfloat(ofeliaGetColor_class, reinterpret_cast<t_method>(ofeliaGetColor_float));
    class_addmethod(ofeliaGetColor_class, reinterpret_cast<t_method>(ofeliaGetColor_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetColor_class, reinterpret_cast<t_method>(ofeliaGetColor_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetColor_class, reinterpret_cast<t_method>(ofeliaGetColor_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetColors object methods
 */
void *ofeliaGetColors_new(t_symbol *s)
{
    t_ofeliaGetColors *x = reinterpret_cast<t_ofeliaGetColors*>(pd_new(ofeliaGetColors_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetColors_bang(t_ofeliaGetColors *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByColorName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadColor::colorData[pos].colors.empty()) {
                
                const vector<ofColor> &colors = t_ofeliaLoadColor::colorData[pos].colors;
                const int ac = static_cast<int>(colors.size()*4);
                t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
                
                for (int i=0; i<ac; i+=4) {
                    
                    const int idx = i/4;
                    av[i].a_type = A_FLOAT;
                    av[i].a_w.w_float = static_cast<t_float>(colors[idx].r);
                    av[i+1].a_type = A_FLOAT;
                    av[i+1].a_w.w_float = static_cast<t_float>(colors[idx].g);
                    av[i+2].a_type = A_FLOAT;
                    av[i+2].a_w.w_float = static_cast<t_float>(colors[idx].b);
                    av[i+3].a_type = A_FLOAT;
                    av[i+3].a_w.w_float = static_cast<t_float>(colors[idx].a);
                }
                outlet_list(x->x_obj.ob_outlet, &s_list, ac, av);
                freebytes(av, ac * sizeof(t_atom));
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetColors::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetColors::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetColors::objName);
    }
}

void ofeliaGetColors_set(t_ofeliaGetColors *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetColors_print(t_ofeliaGetColors *x)
{
    post("\n[%s]", t_ofeliaGetColors::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetColors_setup()
{
    ofeliaGetColors_class = class_new(gensym("ofGetColors"),
                                      reinterpret_cast<t_newmethod>(ofeliaGetColors_new),
                                      0, sizeof(t_ofeliaGetColors),
                                      CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetColors_class, reinterpret_cast<t_method>(ofeliaGetColors_bang));
    class_addmethod(ofeliaGetColors_class, reinterpret_cast<t_method>(ofeliaGetColors_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetColors_class, reinterpret_cast<t_method>(ofeliaGetColors_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetColors_class, reinterpret_cast<t_method>(ofeliaGetColors_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * setup methods
 */
void ofeliaColor_setup()
{
    ofeliaLoadColor_setup();
    ofeliaEditColor_setup();
    ofeliaDoesColorNameExist_setup();
    ofeliaGetColor_setup();
    ofeliaGetColors_setup();
}

