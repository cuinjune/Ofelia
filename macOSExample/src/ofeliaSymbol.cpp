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

#include "ofeliaSymbol.h"

/* ________________________________________________________________________________
 * static variables
 */
const char *t_ofeliaLoadSymbol::objName = "ofLoadSymbol";
unsigned int t_ofeliaLoadSymbol::counter;
vector<t_ofeliaLoadSymbolData> t_ofeliaLoadSymbol::vecData;
const char *t_ofeliaEditSymbol::objName = "ofEditSymbol";
const char *t_ofeliaDoesSymbolNameExist::objName = "ofDoesSymbolNameExist";
const char *t_ofeliaGetSymbol::objName = "ofGetSymbol";
const char *t_ofeliaGetSymbols::objName = "ofGetSymbols";

/* ________________________________________________________________________________
 * ofLoadSymbol object methods
 */
void ofeliaLoadSymbol_vecSizeOut(t_ofeliaLoadSymbol *x);

int getPositionBySymbolObjID(const unsigned int objID)
{
    const auto it = find_if(begin(t_ofeliaLoadSymbol::vecData), end(t_ofeliaLoadSymbol::vecData),
                            [&objID](const t_ofeliaLoadSymbolData &vecData)
                            {
                                return vecData.objID == objID;
                            });
    return static_cast<int>(it - t_ofeliaLoadSymbol::vecData.begin());
}

bool getLoadSymbolFromArgs(const int argc, const t_atom *argv, t_string &Symbol)
{
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            Symbol = argv[0].a_w.w_symbol->s_name;
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadSymbol::objName);
            return 0;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadSymbol::objName);
        return 0;
    }
    return 1;
}

bool getLoadSymbolsFromArgs(const int argc, const t_atom *argv, vector<t_string> &Symbols)
{
    for (int i=0; i<argc; ++i) {
        
        if (argv[i].a_type == A_SYMBOL) {
            
            Symbols.push_back(argv[i].a_w.w_symbol->s_name);
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadSymbol::objName);
            return 0;
        }
    }
    return 1;
}

bool getLoadSymbolCmdRangeFromArgs(const int argc, const t_atom *argv, t_ofeliaLoadSymbolCmdData &cmd)
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
                
                error("%s: wrong argument type", t_ofeliaLoadSymbol::objName);
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
                
                error("%s: wrong argument type", t_ofeliaLoadSymbol::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaLoadSymbol::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaLoadSymbol_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadSymbol *x = reinterpret_cast<t_ofeliaLoadSymbol*>(pd_new(ofeliaLoadSymbol_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    t_ofeliaLoadSymbolData vecData;
    getVarNameLocalPrefixes(vecData.varName);
    vector<t_string> Symbols;
    
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            vecData.varName.name = argv[0].a_w.w_symbol->s_name;
            getVarNameLocalized(vecData.varName);
            
            if (--argc) {
                
                if (!getLoadSymbolsFromArgs(argc, argv+1, Symbols))
                    return 0;
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadSymbol::objName);
            return 0;
        }
    }
    x->objID = vecData.objID = t_ofeliaLoadSymbol::counter++;
    t_ofeliaLoadSymbol::vecData.push_back(vecData);
    x->vecSize = 0;
    x->vecSizeOutClock = clock_new(x, reinterpret_cast<t_method>(ofeliaLoadSymbol_vecSizeOut));
    outlet_new(&x->x_obj, &s_float);
    
    if (argc) {
        
        const int pos = getPositionBySymbolObjID(x->objID);
        t_ofeliaLoadSymbol::vecData[pos].vecSymbols = Symbols;
        x->vecSize = static_cast<int>(Symbols.size());
        clock_delay(x->vecSizeOutClock, 0.0);
    }
    return (x);
}

void ofeliaLoadSymbol_vecSizeOut(t_ofeliaLoadSymbol *x)
{
    outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(x->vecSize));
}

void ofeliaLoadSymbol_name(t_ofeliaLoadSymbol *x, t_symbol *s)
{
    const int pos = getPositionBySymbolObjID(x->objID);
    t_ofeliaLoadSymbol::vecData[pos].varName.name = s->s_name;
    getVarNameLocalized(t_ofeliaLoadSymbol::vecData[pos].varName);
}

void ofeliaLoadSymbol_load(t_ofeliaLoadSymbol *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_string> Symbols;
    
    if (getLoadSymbolsFromArgs(argc, argv, Symbols)) {
        
        const int pos = getPositionBySymbolObjID(x->objID);
        t_ofeliaLoadSymbol::vecData[pos].vecSymbols = Symbols;
        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(Symbols.size()));
    }
}

void ofeliaLoadSymbol_add(t_ofeliaLoadSymbol *x, t_symbol *s, int argc, t_atom *argv)
{
    t_string Symbol;
    
    if (getLoadSymbolFromArgs(argc, argv, Symbol)) {
        
        const int pos = getPositionBySymbolObjID(x->objID);
        t_ofeliaLoadSymbol::vecData[pos].vecSymbols.push_back(Symbol);
        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadSymbol::vecData[pos].vecSymbols.size()));
    }
}

void ofeliaLoadSymbol_append(t_ofeliaLoadSymbol *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_string> Symbols;
    
    if (getLoadSymbolsFromArgs(argc, argv, Symbols)) {
        
        const int pos = getPositionBySymbolObjID(x->objID);
        t_ofeliaLoadSymbol::vecData[pos].vecSymbols.insert(t_ofeliaLoadSymbol::vecData[pos].vecSymbols.end(),
                                                        Symbols.begin(), Symbols.end());
        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadSymbol::vecData[pos].vecSymbols.size()));
    }
}

void ofeliaLoadSymbol_prepend(t_ofeliaLoadSymbol *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_string> Symbols;
    
    if (getLoadSymbolsFromArgs(argc, argv, Symbols)) {
        
        const int pos = getPositionBySymbolObjID(x->objID);
        t_ofeliaLoadSymbol::vecData[pos].vecSymbols.insert(t_ofeliaLoadSymbol::vecData[pos].vecSymbols.begin(),
                                                        Symbols.begin(), Symbols.end());
        outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadSymbol::vecData[pos].vecSymbols.size()));
    }
}

void ofeliaLoadSymbol_insert(t_ofeliaLoadSymbol *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadSymbolCmdData cmd;
    
    if (getLoadSymbolFromArgs(argc, argv, cmd.vecSymbol)) {
        
        if (getLoadSymbolCmdRangeFromArgs(argc-1, argv+1, cmd)) {
            
            const int pos = getPositionBySymbolObjID(x->objID);
            const int size = static_cast<int>(t_ofeliaLoadSymbol::vecData[pos].vecSymbols.size());
            const auto SymbolsBegin = t_ofeliaLoadSymbol::vecData[pos].vecSymbols.begin();
            const int fromIndex = static_cast<int>(ofClamp(cmd.fromIndex, 0.0f, size));
            const int insertSize = static_cast<int>(max(0.0f, max(0.0f, cmd.toIndex) - max(0.0f, cmd.fromIndex))) + 1;
            t_ofeliaLoadSymbol::vecData[pos].vecSymbols.insert(SymbolsBegin + fromIndex,
                                                            insertSize, cmd.vecSymbol);
            outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadSymbol::vecData[pos].vecSymbols.size()));
        }
    }
}

void ofeliaLoadSymbol_fill(t_ofeliaLoadSymbol *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadSymbolCmdData cmd;
    
    if (getLoadSymbolFromArgs(argc, argv, cmd.vecSymbol)) {
        
        if (getLoadSymbolCmdRangeFromArgs(argc-1, argv+1, cmd)) {
            
            const int pos = getPositionBySymbolObjID(x->objID);
            const int size = static_cast<int>(t_ofeliaLoadSymbol::vecData[pos].vecSymbols.size());
            const auto SymbolsBegin = t_ofeliaLoadSymbol::vecData[pos].vecSymbols.begin();
            
            if (size) {
                
                const int maxIndex = size - 1;
                const int fromIndex = static_cast<int>(ofClamp(cmd.fromIndex, 0.0f, maxIndex));
                const int toIndex = static_cast<int>(ofClamp(cmd.toIndex, fromIndex, maxIndex)) + 1;
                fill(SymbolsBegin + fromIndex, SymbolsBegin + toIndex, cmd.vecSymbol);
                outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadSymbol::vecData[pos].vecSymbols.size()));
            }
            else {
                
                outlet_float(x->x_obj.ob_outlet, 0.0f);
            }
        }
    }
}

void ofeliaLoadSymbol_erase(t_ofeliaLoadSymbol *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadSymbolCmdData cmd;
    
    if (getLoadSymbolCmdRangeFromArgs(argc, argv, cmd)) {
        
        const int pos = getPositionBySymbolObjID(x->objID);
        const int size = static_cast<int>(t_ofeliaLoadSymbol::vecData[pos].vecSymbols.size());
        const auto SymbolsBegin = t_ofeliaLoadSymbol::vecData[pos].vecSymbols.begin();
        
        if (size) {
            
            const int maxIndex = size - 1;
            const int fromIndex = static_cast<int>(ofClamp(cmd.fromIndex, 0.0f, maxIndex));
            const int toIndex = static_cast<int>(ofClamp(cmd.toIndex, fromIndex, maxIndex)) + 1;
            t_ofeliaLoadSymbol::vecData[pos].vecSymbols.erase(SymbolsBegin + fromIndex,
                                                           SymbolsBegin + toIndex);
            outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaLoadSymbol::vecData[pos].vecSymbols.size()));
        }
        else {
            
            outlet_float(x->x_obj.ob_outlet, 0.0f);
        }
    }
}

void ofeliaLoadSymbol_clear(t_ofeliaLoadSymbol *x)
{
    const int pos = getPositionBySymbolObjID(x->objID);
    t_ofeliaLoadSymbol::vecData[pos].vecSymbols.clear();
    outlet_float(x->x_obj.ob_outlet, 0.0f);
}

void ofeliaLoadSymbol_set(t_ofeliaLoadSymbol *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            ofeliaLoadSymbol_name(x, argv[0].a_w.w_symbol);
            
            if (--argc)
                ofeliaLoadSymbol_load(x, 0, argc, argv+1);
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadSymbol::objName);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadSymbol::objName);
    }
}

void ofeliaLoadSymbol_print(t_ofeliaLoadSymbol *x)
{
    post("\n[%s]", t_ofeliaLoadSymbol::objName);
    const int pos = getPositionBySymbolObjID(x->objID);
    post("name : %s", t_ofeliaLoadSymbol::vecData[pos].varName.name.c_str());
    post("load : ");
    
    for (size_t i=0; i<t_ofeliaLoadSymbol::vecData[pos].vecSymbols.size(); ++i) {
        
        post(" %s", t_ofeliaLoadSymbol::vecData[pos].vecSymbols[i].c_str());
    }
}

void ofeliaLoadSymbol_free(t_ofeliaLoadSymbol *x)
{
    clock_free(x->vecSizeOutClock);
    const int pos = getPositionBySymbolObjID(x->objID);
    t_ofeliaLoadSymbol::vecData.erase(t_ofeliaLoadSymbol::vecData.begin() + pos);
}

void ofeliaLoadSymbol_setup()
{
    ofeliaLoadSymbol_class = class_new(gensym("ofLoadSymbol"),
                                       reinterpret_cast<t_newmethod>(ofeliaLoadSymbol_new),
                                       reinterpret_cast<t_method>(ofeliaLoadSymbol_free),
                                       sizeof(t_ofeliaLoadSymbol),
                                       CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaLoadSymbol_class, reinterpret_cast<t_method>(ofeliaLoadSymbol_name),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaLoadSymbol_class, reinterpret_cast<t_method>(ofeliaLoadSymbol_load),
                    gensym("load"), A_GIMME, 0);
    class_addmethod(ofeliaLoadSymbol_class, reinterpret_cast<t_method>(ofeliaLoadSymbol_add),
                    gensym("add"), A_GIMME, 0);
    class_addmethod(ofeliaLoadSymbol_class, reinterpret_cast<t_method>(ofeliaLoadSymbol_append),
                    gensym("append"), A_GIMME, 0);
    class_addmethod(ofeliaLoadSymbol_class, reinterpret_cast<t_method>(ofeliaLoadSymbol_prepend),
                    gensym("prepend"), A_GIMME, 0);
    class_addmethod(ofeliaLoadSymbol_class, reinterpret_cast<t_method>(ofeliaLoadSymbol_insert),
                    gensym("insert"), A_GIMME, 0);
    class_addmethod(ofeliaLoadSymbol_class, reinterpret_cast<t_method>(ofeliaLoadSymbol_fill),
                    gensym("fill"), A_GIMME, 0);
    class_addmethod(ofeliaLoadSymbol_class, reinterpret_cast<t_method>(ofeliaLoadSymbol_erase),
                    gensym("erase"), A_GIMME, 0);
    class_addmethod(ofeliaLoadSymbol_class, reinterpret_cast<t_method>(ofeliaLoadSymbol_clear),
                    gensym("clear"), A_NULL, 0);
    class_addmethod(ofeliaLoadSymbol_class, reinterpret_cast<t_method>(ofeliaLoadSymbol_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaLoadSymbol_class, reinterpret_cast<t_method>(ofeliaLoadSymbol_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * common methods
 */
int getPositionBySymbolName(const t_string &name)
{
    auto it = find_if(begin(t_ofeliaLoadSymbol::vecData), end(t_ofeliaLoadSymbol::vecData),
                      [&name] (const t_ofeliaLoadSymbolData &vecData) {
                          return (vecData.varName.name == name);
                      });
    if (it == end(t_ofeliaLoadSymbol::vecData))
        return -1;
    return static_cast<int>(it - begin(t_ofeliaLoadSymbol::vecData));
}

/* ________________________________________________________________________________
 * ofEditSymbol object methods
 */
void *ofeliaEditSymbol_new(t_symbol *s)
{
    t_ofeliaEditSymbol *x = reinterpret_cast<t_ofeliaEditSymbol*>(pd_new(ofeliaEditSymbol_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaEditSymbol_float(t_ofeliaEditSymbol *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaEditSymbol_assign(t_ofeliaEditSymbol *x, t_symbol *s)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionBySymbolName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadSymbol::vecData[pos].vecSymbols.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadSymbol::vecData[pos].vecSymbols.size())-1);
                t_string &vecSymbol = t_ofeliaLoadSymbol::vecData[pos].vecSymbols[index];
                vecSymbol = s->s_name;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditSymbol::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditSymbol::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditSymbol::objName);
    }
}

void ofeliaEditSymbol_append(t_ofeliaEditSymbol *x, t_symbol *s)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionBySymbolName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadSymbol::vecData[pos].vecSymbols.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadSymbol::vecData[pos].vecSymbols.size())-1);
                t_string &vecSymbol = t_ofeliaLoadSymbol::vecData[pos].vecSymbols[index];
                vecSymbol += s->s_name;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditSymbol::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditSymbol::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditSymbol::objName);
    }
}

void ofeliaEditSymbol_prepend(t_ofeliaEditSymbol *x, t_symbol *s)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionBySymbolName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadSymbol::vecData[pos].vecSymbols.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadSymbol::vecData[pos].vecSymbols.size())-1);
                t_string &vecSymbol = t_ofeliaLoadSymbol::vecData[pos].vecSymbols[index];
                vecSymbol = s->s_name + vecSymbol;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditSymbol::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditSymbol::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditSymbol::objName);
    }
}

void ofeliaEditSymbol_insert(t_ofeliaEditSymbol *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionBySymbolName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadSymbol::vecData[pos].vecSymbols.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadSymbol::vecData[pos].vecSymbols.size())-1);
                t_string &vecSymbol = t_ofeliaLoadSymbol::vecData[pos].vecSymbols[index];
                
                switch (argc) {
                        
                    case 1:
                    {
                        if (argv[0].a_type == A_SYMBOL) {
                            
                            vecSymbol += argv[0].a_w.w_symbol->s_name;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditSymbol::objName);
                            return;
                        }
                        break;
                    }
                    case 2:
                    {
                        if (argv[0].a_type == A_SYMBOL &&
                            argv[1].a_type == A_FLOAT) {
 
                            const int pos = getClampIntValue(static_cast<int>(argv[1].a_w.w_float), 0, static_cast<int>(vecSymbol.size()));
                            vecSymbol.insert(pos, argv[0].a_w.w_symbol->s_name);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditSymbol::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditSymbol::objName);
                        return;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditSymbol::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditSymbol::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditSymbol::objName);
    }
}

void ofeliaEditSymbol_replace(t_ofeliaEditSymbol *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionBySymbolName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadSymbol::vecData[pos].vecSymbols.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadSymbol::vecData[pos].vecSymbols.size())-1);
                t_string &vecSymbol = t_ofeliaLoadSymbol::vecData[pos].vecSymbols[index];
                
                if (argc == 2) {
                    
                    if (argv[0].a_type == A_SYMBOL &&
                        argv[1].a_type == A_SYMBOL) {

                        t_string searchStr(argv[0].a_w.w_symbol->s_name);
                        t_string replaceStr(argv[1].a_w.w_symbol->s_name);
                        
                        if (!searchStr.empty()) {
                            
                            auto pos = vecSymbol.find(searchStr);
                            
                            while(pos != t_string::npos) {
                                
                                vecSymbol.replace(pos, searchStr.size(), replaceStr);
                                pos += replaceStr.size();
                                t_string nextfind(vecSymbol.begin() + pos);
                                auto nextpos = nextfind.find(searchStr);
                                
                                if (nextpos == t_string::npos)
                                    break;
                                pos += nextpos;
                            }
                        }
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditSymbol::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditSymbol::objName);
                    return;
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditSymbol::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditSymbol::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditSymbol::objName);
    }
}

void ofeliaEditSymbol_remove(t_ofeliaEditSymbol *x, t_symbol *s)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionBySymbolName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadSymbol::vecData[pos].vecSymbols.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadSymbol::vecData[pos].vecSymbols.size())-1);
                t_string &vecSymbol = t_ofeliaLoadSymbol::vecData[pos].vecSymbols[index];
                t_string searchStr(s->s_name);
                
                if (!searchStr.empty()) {
                    
                    const size_t n = searchStr.size();
                    
                    for (size_t p = vecSymbol.find(searchStr); p != t_string::npos; p = vecSymbol.find(searchStr))
                        vecSymbol.erase(p, n);
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditSymbol::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditSymbol::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditSymbol::objName);
    }
}

void ofeliaEditSymbol_erase(t_ofeliaEditSymbol *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionBySymbolName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadSymbol::vecData[pos].vecSymbols.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadSymbol::vecData[pos].vecSymbols.size())-1);
                t_string &vecSymbol = t_ofeliaLoadSymbol::vecData[pos].vecSymbols[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        if (!vecSymbol.empty())
                            vecSymbol.pop_back();
                        break;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            if (!vecSymbol.empty()) {
                                
                                const int pos = getClampIntValue(static_cast<int>(argv[0].a_w.w_float), 0, static_cast<int>(vecSymbol.size()-1));
                                vecSymbol.erase(pos, 1);
                            }
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditSymbol::objName);
                            return;
                        }
                        break;
                    }
                    case 2:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            if (!vecSymbol.empty()) {
                                
                                const int maxIndex = static_cast<int>(vecSymbol.size()) - 1;
                                const int fromIndex = static_cast<int>(ofClamp(argv[0].a_w.w_float, 0.0f, maxIndex));
                                const int toIndex = static_cast<int>(ofClamp(argv[1].a_w.w_float, fromIndex, maxIndex)) + 1;
                                vecSymbol.erase(fromIndex, toIndex-fromIndex);
                            }
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditSymbol::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditSymbol::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditSymbol::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditSymbol::objName);
    }
}

void ofeliaEditSymbol_clear(t_ofeliaEditSymbol *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionBySymbolName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadSymbol::vecData[pos].vecSymbols.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadSymbol::vecData[pos].vecSymbols.size())-1);
                t_string &vecSymbol = t_ofeliaLoadSymbol::vecData[pos].vecSymbols[index];
                vecSymbol.clear();
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditSymbol::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditSymbol::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditSymbol::objName);
    }
}

void ofeliaEditSymbol_set(t_ofeliaEditSymbol *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaEditSymbol_print(t_ofeliaEditSymbol *x)
{
    post("\n[%s]", t_ofeliaEditSymbol::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaEditSymbol_setup()
{
    ofeliaEditSymbol_class = class_new(gensym("ofEditSymbol"),
                                       reinterpret_cast<t_newmethod>(ofeliaEditSymbol_new),
                                       0, sizeof(t_ofeliaEditSymbol),
                                       CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaEditSymbol_class, reinterpret_cast<t_method>(ofeliaEditSymbol_float));
    class_addmethod(ofeliaEditSymbol_class, reinterpret_cast<t_method>(ofeliaEditSymbol_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditSymbol_class, reinterpret_cast<t_method>(ofeliaEditSymbol_assign),
                    gensym("assign"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditSymbol_class, reinterpret_cast<t_method>(ofeliaEditSymbol_append),
                    gensym("append"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditSymbol_class, reinterpret_cast<t_method>(ofeliaEditSymbol_prepend),
                    gensym("prepend"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditSymbol_class, reinterpret_cast<t_method>(ofeliaEditSymbol_insert),
                    gensym("insert"), A_GIMME, 0);
    class_addmethod(ofeliaEditSymbol_class, reinterpret_cast<t_method>(ofeliaEditSymbol_replace),
                    gensym("replace"), A_GIMME, 0);
    class_addmethod(ofeliaEditSymbol_class, reinterpret_cast<t_method>(ofeliaEditSymbol_remove),
                    gensym("remove"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditSymbol_class, reinterpret_cast<t_method>(ofeliaEditSymbol_erase),
                    gensym("erase"), A_GIMME, 0);
    class_addmethod(ofeliaEditSymbol_class, reinterpret_cast<t_method>(ofeliaEditSymbol_clear),
                    gensym("clear"), A_NULL, 0);
    class_addmethod(ofeliaEditSymbol_class, reinterpret_cast<t_method>(ofeliaEditSymbol_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditSymbol_class, reinterpret_cast<t_method>(ofeliaEditSymbol_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofDoesSymbolNameExist object methods
 */
void *ofeliaDoesSymbolNameExist_new(t_symbol *s)
{
    t_ofeliaDoesSymbolNameExist *x = reinterpret_cast<t_ofeliaDoesSymbolNameExist*>(pd_new(ofeliaDoesSymbolNameExist_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaDoesSymbolNameExist_bang(t_ofeliaDoesSymbolNameExist *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        if (getPositionBySymbolName(name) != -1)
            outlet_float(x->x_obj.ob_outlet, 1.0f);
        else
            outlet_float(x->x_obj.ob_outlet, 0.0f);
    }
    else {
        
        error("%s: name not assigned", t_ofeliaDoesSymbolNameExist::objName);
    }
}

void ofeliaDoesSymbolNameExist_set(t_ofeliaDoesSymbolNameExist *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaDoesSymbolNameExist_print(t_ofeliaDoesSymbolNameExist *x)
{
    post("\n[%s]", t_ofeliaDoesSymbolNameExist::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaDoesSymbolNameExist_setup()
{
    ofeliaDoesSymbolNameExist_class = class_new(gensym("ofDoesSymbolNameExist"),
                                               reinterpret_cast<t_newmethod>(ofeliaDoesSymbolNameExist_new),
                                               0, sizeof(t_ofeliaDoesSymbolNameExist),
                                               CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaDoesSymbolNameExist_class, reinterpret_cast<t_method>(ofeliaDoesSymbolNameExist_bang));
    class_addmethod(ofeliaDoesSymbolNameExist_class, reinterpret_cast<t_method>(ofeliaDoesSymbolNameExist_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesSymbolNameExist_class, reinterpret_cast<t_method>(ofeliaDoesSymbolNameExist_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesSymbolNameExist_class, reinterpret_cast<t_method>(ofeliaDoesSymbolNameExist_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetSymbol object methods
 */
void *ofeliaGetSymbol_new(t_symbol *s)
{
    t_ofeliaGetSymbol *x = reinterpret_cast<t_ofeliaGetSymbol*>(pd_new(ofeliaGetSymbol_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_symbol);
    return (x);
}

void ofeliaGetSymbol_bang(t_ofeliaGetSymbol *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionBySymbolName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadSymbol::vecData[pos].vecSymbols.empty()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadSymbol::vecData[pos].vecSymbols.size())-1);
                outlet_symbol(x->x_obj.ob_outlet, gensym(t_ofeliaLoadSymbol::vecData[pos].vecSymbols[index].c_str()));
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetSymbol::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetSymbol::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetSymbol::objName);
    }
}

void ofeliaGetSymbol_float(t_ofeliaGetSymbol *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetSymbol_set(t_ofeliaGetSymbol *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetSymbol_print(t_ofeliaGetSymbol *x)
{
    post("\n[%s]", t_ofeliaGetSymbol::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetSymbol_setup()
{
    ofeliaGetSymbol_class = class_new(gensym("ofGetSymbol"),
                                      reinterpret_cast<t_newmethod>(ofeliaGetSymbol_new),
                                      0, sizeof(t_ofeliaGetSymbol),
                                      CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetSymbol_class, reinterpret_cast<t_method>(ofeliaGetSymbol_bang));
    class_addfloat(ofeliaGetSymbol_class, reinterpret_cast<t_method>(ofeliaGetSymbol_float));
    class_addmethod(ofeliaGetSymbol_class, reinterpret_cast<t_method>(ofeliaGetSymbol_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetSymbol_class, reinterpret_cast<t_method>(ofeliaGetSymbol_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetSymbol_class, reinterpret_cast<t_method>(ofeliaGetSymbol_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetSymbols object methods
 */
void *ofeliaGetSymbols_new(t_symbol *s)
{
    t_ofeliaGetSymbols *x = reinterpret_cast<t_ofeliaGetSymbols*>(pd_new(ofeliaGetSymbols_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_anything);
    return (x);
}

void ofeliaGetSymbols_bang(t_ofeliaGetSymbols *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionBySymbolName(name);
        
        if (pos != -1) {
            
            if (!t_ofeliaLoadSymbol::vecData[pos].vecSymbols.empty()) {
                
                const int ac = static_cast<int>(t_ofeliaLoadSymbol::vecData[pos].vecSymbols.size());
                t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
                
                for (int i=0; i<ac; ++i) {
                    
                    av[i].a_type = A_SYMBOL;
                    av[i].a_w.w_symbol = gensym(t_ofeliaLoadSymbol::vecData[pos].vecSymbols[i].c_str());
                }
                outlet_anything(x->x_obj.ob_outlet, av[0].a_w.w_symbol, ac-1, av+1);
                freebytes(av, ac * sizeof(t_atom));
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetSymbols::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetSymbols::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetSymbols::objName);
    }
}

void ofeliaGetSymbols_set(t_ofeliaGetSymbols *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetSymbols_print(t_ofeliaGetSymbols *x)
{
    post("\n[%s]", t_ofeliaGetSymbols::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetSymbols_setup()
{
    ofeliaGetSymbols_class = class_new(gensym("ofGetSymbols"),
                                       reinterpret_cast<t_newmethod>(ofeliaGetSymbols_new),
                                       0, sizeof(t_ofeliaGetSymbols),
                                       CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetSymbols_class, reinterpret_cast<t_method>(ofeliaGetSymbols_bang));
    class_addmethod(ofeliaGetSymbols_class, reinterpret_cast<t_method>(ofeliaGetSymbols_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetSymbols_class, reinterpret_cast<t_method>(ofeliaGetSymbols_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetSymbols_class, reinterpret_cast<t_method>(ofeliaGetSymbols_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * setup methods
 */
void ofeliaSymbol_setup()
{
    ofeliaLoadSymbol_setup();
    ofeliaEditSymbol_setup();
    ofeliaDoesSymbolNameExist_setup();
    ofeliaGetSymbol_setup();
    ofeliaGetSymbols_setup();
}

