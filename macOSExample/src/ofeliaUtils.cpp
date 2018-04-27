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

#include "ofeliaUtils.h"
#include "ofeliaWindow.h"
#include <random>

/* ________________________________________________________________________________
 * static variables
 */
const char *t_ofeliaAppend::objName = "ofAppend";
const char *t_ofeliaPrepend::objName = "ofPrepend";
const char *t_ofeliaPack::objName = "ofPack";
const char *t_ofeliaListFind::objName = "ofListFind";
const char *t_ofeliaFindList::objName = "ofFindList";
const char *t_ofeliaListInsert::objName = "ofListInsert";
const char *t_ofeliaInsertList::objName = "ofInsertList";
const char *t_ofeliaListFill::objName = "ofListFill";
const char *t_ofeliaFillList::objName = "ofFillList";
const char *t_ofeliaListReplace::objName = "ofListReplace";
const char *t_ofeliaReplaceList::objName = "ofReplaceList";
const char *t_ofeliaListRemove::objName = "ofListRemove";
const char *t_ofeliaRemoveList::objName = "ofRemoveList";
const char *t_ofeliaListErase::objName = "ofListErase";
const char *t_ofeliaEraseList::objName = "ofEraseList";
const char *t_ofeliaListSort::objName = "ofListSort";
const char *t_ofeliaListUnique::objName = "ofListUnique";
const char *t_ofeliaListReverse::objName = "ofListReverse";
const char *t_ofeliaListShuffle::objName = "ofListShuffle";
const char *t_ofeliaListToSymbol::objName = "ofListToSymbol";
const char *t_ofeliaSymbolToList::objName = "ofSymbolToList";
const char *t_ofeliaCount::objName = "ofCount";
const char *t_ofeliaCountUntil::objName = "ofCountUntil";
const char *t_ofeliaStep::objName = "ofStep";
const char *t_ofeliaSwitch::objName = "ofSwitch";
const char *t_ofeliaGate::objName = "ofGate";
const char *t_ofeliaValue::objName = "ofValue";
const char *t_ofeliaSend::objName = "ofSend";
const char *t_ofeliaReceive::objName = "ofReceive";
const char *t_ofeliaGetPatchDirectory::objName = "ofGetPatchDirectory";
const char *t_ofeliaGetHomeDirectory::objName = "ofGetHomeDirectory";
const char *t_ofeliaGetDocumentsDirectory::objName = "ofGetDocumentsDirectory";
const char *t_ofeliaGetLibraryDirectory::objName = "ofGetLibraryDirectory";
const char *t_ofeliaGetTemporaryDirectory::objName = "ofGetTemporaryDirectory";
const char *t_ofeliaDoesDirectoryExist::objName = "ofDoesDirectoryExist";
const char *t_ofeliaDoesFileExist::objName = "ofDoesFileExist";
const char *t_ofeliaGetDirectoryFileNames::objName = "ofGetDirectoryFileNames";
const char *t_ofeliaGetDirectoryFilePaths::objName = "ofGetDirectoryFilePaths";
const char *t_ofeliaGetDollarZero::objName = "ofGetDollarZero";
const char *t_ofeliaGetCanvasName::objName = "ofGetCanvasName";
const char *t_ofeliaGetCanvasIndex::objName = "ofGetCanvasIndex";
const char *t_ofeliaGetCanvasArgs::objName = "ofGetCanvasArgs";
const char *t_ofeliaSetCanvasArgs::objName = "ofSetCanvasArgs";
const char *t_ofeliaRemoveCanvas::objName = "ofRemoveCanvas";
const char *t_ofeliaHexToHsb::objName = "ofHexToHsb";
const char *t_ofeliaHexToRgb::objName = "ofHexToRgb";
const char *t_ofeliaHsbToHex::objName = "ofHsbToHex";
const char *t_ofeliaHsbToRgb::objName = "ofHsbToRgb";
const char *t_ofeliaRgbToHex::objName = "ofRgbToHex";
const char *t_ofeliaRgbToHsb::objName = "ofRgbToHsb";
const char *t_ofeliaError::objName = "ofError";
const char *t_ofeliaSystemLoadDialog::objName = "ofSystemLoadDialog";
const char *t_ofeliaSystemSaveDialog::objName = "ofSystemSaveDialog";
const char *t_ofeliaSystemAlertDialog::objName = "ofSystemAlertDialog";
const char *t_ofeliaSystemTextBoxDialog::objName = "ofSystemTextBoxDialog";

/* ________________________________________________________________________________
 * ofAppend object methods
 */
void *ofeliaAppend_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaAppend *x = reinterpret_cast<t_ofeliaAppend*>(pd_new(ofeliaAppend_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    switch (argc) {
            
        case 0:
        {
            x->append = gensym("");
            x->isAppendEmpty = true;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_SYMBOL) {
                
                x->append = argv[0].a_w.w_symbol;
                x->isAppendEmpty = false;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaAppend::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaAppend::objName);
            return 0;
        }
    }
    outlet_new(&x->x_obj, &s_anything);
    return (x);
}

void ofeliaAppend_float(t_ofeliaAppend *x, t_floatarg f)
{
    if (!x->isAppendEmpty) {
        
        t_atom av[2];
        av[0].a_type = A_FLOAT;
        av[0].a_w.w_float = f;
        av[1].a_type = A_SYMBOL;
        av[1].a_w.w_symbol = x->append;
        outlet_list(x->x_obj.ob_outlet, &s_list, 2, av);
    }
    else
        outlet_float(x->x_obj.ob_outlet, f);
}

void ofeliaAppend_symbol(t_ofeliaAppend *x, t_symbol *s)
{
    if (!x->isAppendEmpty) {
        
        t_atom av[1];
        av[0].a_type = A_SYMBOL;
        av[0].a_w.w_symbol = x->append;
        outlet_anything(x->x_obj.ob_outlet, s, 1, av);
    }
    else
        outlet_symbol(x->x_obj.ob_outlet, s);
}

void ofeliaAppend_list(t_ofeliaAppend *x, t_symbol *s, int argc, t_atom *argv)
{
    if (!x->isAppendEmpty) {
        
        t_atom *av = (t_atom *)copybytes(argv, (argc+1) * sizeof(t_atom));
        av[argc].a_type = A_SYMBOL;
        av[argc].a_w.w_symbol = x->append;
        outlet_list(x->x_obj.ob_outlet, s, argc+1, av);
        freebytes(av, (argc+1) * sizeof(t_atom));
    }
    else
        outlet_list(x->x_obj.ob_outlet, 0, argc, argv);
}

void ofeliaAppend_anything(t_ofeliaAppend *x, t_symbol *s, int argc, t_atom *argv)
{
    if (!x->isAppendEmpty) {
        
        t_atom *av = (t_atom *)copybytes(argv, (argc+1) * sizeof(t_atom));
        av[argc].a_type = A_SYMBOL;
        av[argc].a_w.w_symbol = x->append;
        outlet_anything(x->x_obj.ob_outlet, s, argc+1, av);
        freebytes(av, (argc+1) * sizeof(t_atom));
    }
    else {
        
        outlet_anything(x->x_obj.ob_outlet, s, argc, argv);
    }
}

void ofeliaAppend_set(t_ofeliaAppend *x, t_symbol *s)
{
    x->append = s;
    x->isAppendEmpty = false;
}

void ofeliaAppend_print(t_ofeliaAppend *x)
{
    post("\n[%s]", t_ofeliaAppend::objName);
    post("append : %s", x->append->s_name);
}

void ofeliaAppend_setup()
{
    ofeliaAppend_class = class_new(gensym("ofAppend"),
                                   reinterpret_cast<t_newmethod>(ofeliaAppend_new),
                                   0, sizeof(t_ofeliaAppend),
                                   CLASS_DEFAULT, A_GIMME, 0);
    class_addfloat(ofeliaAppend_class, reinterpret_cast<t_method>(ofeliaAppend_float));
    class_addsymbol(ofeliaAppend_class, reinterpret_cast<t_method>(ofeliaAppend_symbol));
    class_addlist(ofeliaAppend_class, reinterpret_cast<t_method>(ofeliaAppend_list));
    class_addanything(ofeliaAppend_class, reinterpret_cast<t_method>(ofeliaAppend_anything));
    class_addmethod(ofeliaAppend_class, reinterpret_cast<t_method>(ofeliaAppend_set),
                    gensym("append"), A_SYMBOL, 0);
    class_addmethod(ofeliaAppend_class, reinterpret_cast<t_method>(ofeliaAppend_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaAppend_class, reinterpret_cast<t_method>(ofeliaAppend_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofPrepend object methods
 */
void *ofeliaPrepend_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaPrepend *x = reinterpret_cast<t_ofeliaPrepend*>(pd_new(ofeliaPrepend_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    switch (argc) {
            
        case 0:
        {
            x->prepend = gensym("");
            x->isPrependEmpty = true;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_SYMBOL) {
                
                x->prepend = argv[0].a_w.w_symbol;
                x->isPrependEmpty = false;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaPrepend::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaPrepend::objName);
            return 0;
        }
    }
    outlet_new(&x->x_obj, &s_anything);
    return (x);
}

void ofeliaPrepend_float(t_ofeliaPrepend *x, t_floatarg f)
{
    if (!x->isPrependEmpty) {
        
        t_atom av[1];
        av[0].a_type = A_FLOAT;
        av[0].a_w.w_float = f;
        outlet_anything(x->x_obj.ob_outlet, x->prepend, 1, av);
    }
    else
        outlet_float(x->x_obj.ob_outlet, f);
}

void ofeliaPrepend_symbol(t_ofeliaPrepend *x, t_symbol *s)
{
    if (!x->isPrependEmpty) {
        
        t_atom av[1];
        av[0].a_type = A_SYMBOL;
        av[0].a_w.w_symbol = s;
        outlet_anything(x->x_obj.ob_outlet, x->prepend, 1, av);
    }
    else
        outlet_symbol(x->x_obj.ob_outlet, s);
}

void ofeliaPrepend_list(t_ofeliaPrepend *x, t_symbol *s, int argc, t_atom *argv)
{
    if (!x->isPrependEmpty)
        outlet_anything(x->x_obj.ob_outlet, x->prepend, argc, argv);
    else
        outlet_list(x->x_obj.ob_outlet, 0, argc, argv);
}

void ofeliaPrepend_anything(t_ofeliaPrepend *x, t_symbol *s, int argc, t_atom *argv)
{
    if (!x->isPrependEmpty) {
        
        t_atom *av = (t_atom *)copybytes(argv, (argc+1) * sizeof(t_atom));
        memmove(av+1, av, argc * sizeof(t_atom));
        av[0].a_type = A_SYMBOL;
        av[0].a_w.w_symbol = s;
        outlet_anything(x->x_obj.ob_outlet, x->prepend, argc+1, av);
        freebytes(av, (argc+1) * sizeof(t_atom));
    }
    else {
        
        outlet_anything(x->x_obj.ob_outlet, s, argc, argv);
    }
}

void ofeliaPrepend_set(t_ofeliaPrepend *x, t_symbol *s)
{
    x->prepend = s;
    x->isPrependEmpty = false;
}

void ofeliaPrepend_print(t_ofeliaPrepend *x)
{
    post("\n[%s]", t_ofeliaPrepend::objName);
    post("prepend : %s", x->prepend->s_name);
}

void ofeliaPrepend_setup()
{
    ofeliaPrepend_class = class_new(gensym("ofPrepend"),
                                    reinterpret_cast<t_newmethod>(ofeliaPrepend_new),
                                    0, sizeof(t_ofeliaPrepend),
                                    CLASS_DEFAULT, A_GIMME, 0);
    class_addfloat(ofeliaPrepend_class, reinterpret_cast<t_method>(ofeliaPrepend_float));
    class_addsymbol(ofeliaPrepend_class, reinterpret_cast<t_method>(ofeliaPrepend_symbol));
    class_addlist(ofeliaPrepend_class, reinterpret_cast<t_method>(ofeliaPrepend_list));
    class_addanything(ofeliaPrepend_class, reinterpret_cast<t_method>(ofeliaPrepend_anything));
    class_addmethod(ofeliaPrepend_class, reinterpret_cast<t_method>(ofeliaPrepend_set),
                    gensym("prepend"), A_SYMBOL, 0);
    class_addmethod(ofeliaPrepend_class, reinterpret_cast<t_method>(ofeliaPrepend_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaPrepend_class, reinterpret_cast<t_method>(ofeliaPrepend_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofPack object methods
 */
void outletPackedAtoms(t_ofeliaPack *x)
{
    if (x->av[0].a_type == A_FLOAT)
        outlet_list(x->x_obj.ob_outlet, &s_list, x->ac, x->av);
    else if (x->av[0].a_type == A_SYMBOL)
        outlet_anything(x->x_obj.ob_outlet, x->av[0].a_w.w_symbol, x->ac-1, x->av+1);
}

void *ofeliaPack_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaPack *x = reinterpret_cast<t_ofeliaPack*>(pd_new(ofeliaPack_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (argc) {
        
        if (argc <= 64) {
            
            x->ac = argc;
            x->av = (t_atom *)copybytes(argv, x->ac * sizeof(t_atom));
        }
        else {
            
            error("%s: invalid number of arguments", t_ofeliaPack::objName);
            return 0;
        }
    }
    else {
        
        x->ac = 1;
        t_atom av[1];
        av[0].a_type = A_FLOAT;
        av[0].a_w.w_float = 0.0f;
        x->av = (t_atom *)copybytes(av, x->ac * sizeof(t_atom));
    }
    for (int i=1; i<x->ac; ++i) {
        
        if (argv[i].a_type == A_FLOAT) {
            
            stringstream ss;
            ss << "{f}" << i;
            inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym(ss.str().c_str()));
        }
        else if (argv[i].a_type == A_SYMBOL) {
            
            stringstream ss;
            ss << "{s}" << i;
            inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_symbol, gensym(ss.str().c_str()));
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaPack::objName);
            return 0;
        }
    }
    outlet_new(&x->x_obj, &s_anything);
    return (x);
}

void ofeliaPack_bang(t_ofeliaPack *x)
{
    outletPackedAtoms(x);
}

void ofeliaPack_float(t_ofeliaPack *x, t_floatarg f)
{
    if (x->av[0].a_type == A_FLOAT) {
        
        x->av[0].a_w.w_float = f;
        outlet_list(x->x_obj.ob_outlet, &s_list, x->ac, x->av);
    }
    else {
        
        error("%s: wrong type", t_ofeliaPack::objName);
    }
}

void ofeliaPack_symbol(t_ofeliaPack *x, t_symbol *s)
{
    if (x->av[0].a_type == A_SYMBOL) {
        
        x->av[0].a_w.w_symbol = s;
        outlet_anything(x->x_obj.ob_outlet, x->av[0].a_w.w_symbol, x->ac-1, x->av+1);
    }
    else {
        
        error("%s: wrong type", t_ofeliaPack::objName);
    }
}

void ofeliaPack_list(t_ofeliaPack *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc <= x->ac) {
        
        for (int i=0; i<argc; ++i) {
            
            if (argv[i].a_type != x->av[i].a_type) {
                
                error("%s: wrong type", t_ofeliaPack::objName);
                return;
            }
        }
        for (int i=0; i<argc; ++i)
            x->av[i] = argv[i];
        outletPackedAtoms(x);
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaPack::objName);
    }
}

void ofeliaPack_anything(t_ofeliaPack *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc+1 <= x->ac) {
        
        if (x->av[0].a_type != A_SYMBOL) {
            
            error("%s: wrong type", t_ofeliaPack::objName);
            return;
        }
        for (int i=0; i<argc; ++i) {
            
            if (argv[i].a_type != x->av[i+1].a_type) {
                
                error("%s: wrong type", t_ofeliaPack::objName);
                return;
            }
        }
        x->av[0].a_w.w_symbol = s;
        
        for (int i=0; i<argc; ++i)
            x->av[i+1] = argv[i];
        outlet_anything(x->x_obj.ob_outlet, x->av[0].a_w.w_symbol, x->ac-1, x->av+1);
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaPack::objName);
    }
}

void ofeliaPack_f1(t_ofeliaPack *x, t_floatarg f)
{
    x->av[1].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f2(t_ofeliaPack *x, t_floatarg f)
{
    x->av[2].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f3(t_ofeliaPack *x, t_floatarg f)
{
    x->av[3].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f4(t_ofeliaPack *x, t_floatarg f)
{
    x->av[4].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f5(t_ofeliaPack *x, t_floatarg f)
{
    x->av[5].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f6(t_ofeliaPack *x, t_floatarg f)
{
    x->av[6].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f7(t_ofeliaPack *x, t_floatarg f)
{
    x->av[7].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f8(t_ofeliaPack *x, t_floatarg f)
{
    x->av[8].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f9(t_ofeliaPack *x, t_floatarg f)
{
    x->av[9].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f10(t_ofeliaPack *x, t_floatarg f)
{
    x->av[10].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f11(t_ofeliaPack *x, t_floatarg f)
{
    x->av[11].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f12(t_ofeliaPack *x, t_floatarg f)
{
    x->av[12].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f13(t_ofeliaPack *x, t_floatarg f)
{
    x->av[13].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f14(t_ofeliaPack *x, t_floatarg f)
{
    x->av[14].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f15(t_ofeliaPack *x, t_floatarg f)
{
    x->av[15].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f16(t_ofeliaPack *x, t_floatarg f)
{
    x->av[16].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f17(t_ofeliaPack *x, t_floatarg f)
{
    x->av[17].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f18(t_ofeliaPack *x, t_floatarg f)
{
    x->av[18].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f19(t_ofeliaPack *x, t_floatarg f)
{
    x->av[19].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f20(t_ofeliaPack *x, t_floatarg f)
{
    x->av[20].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f21(t_ofeliaPack *x, t_floatarg f)
{
    x->av[21].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f22(t_ofeliaPack *x, t_floatarg f)
{
    x->av[22].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f23(t_ofeliaPack *x, t_floatarg f)
{
    x->av[23].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f24(t_ofeliaPack *x, t_floatarg f)
{
    x->av[24].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f25(t_ofeliaPack *x, t_floatarg f)
{
    x->av[25].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f26(t_ofeliaPack *x, t_floatarg f)
{
    x->av[26].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f27(t_ofeliaPack *x, t_floatarg f)
{
    x->av[27].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f28(t_ofeliaPack *x, t_floatarg f)
{
    x->av[28].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f29(t_ofeliaPack *x, t_floatarg f)
{
    x->av[29].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f30(t_ofeliaPack *x, t_floatarg f)
{
    x->av[30].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f31(t_ofeliaPack *x, t_floatarg f)
{
    x->av[31].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f32(t_ofeliaPack *x, t_floatarg f)
{
    x->av[32].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f33(t_ofeliaPack *x, t_floatarg f)
{
    x->av[33].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f34(t_ofeliaPack *x, t_floatarg f)
{
    x->av[34].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f35(t_ofeliaPack *x, t_floatarg f)
{
    x->av[35].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f36(t_ofeliaPack *x, t_floatarg f)
{
    x->av[36].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f37(t_ofeliaPack *x, t_floatarg f)
{
    x->av[37].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f38(t_ofeliaPack *x, t_floatarg f)
{
    x->av[38].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f39(t_ofeliaPack *x, t_floatarg f)
{
    x->av[39].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f40(t_ofeliaPack *x, t_floatarg f)
{
    x->av[40].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f41(t_ofeliaPack *x, t_floatarg f)
{
    x->av[41].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f42(t_ofeliaPack *x, t_floatarg f)
{
    x->av[42].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f43(t_ofeliaPack *x, t_floatarg f)
{
    x->av[43].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f44(t_ofeliaPack *x, t_floatarg f)
{
    x->av[44].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f45(t_ofeliaPack *x, t_floatarg f)
{
    x->av[45].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f46(t_ofeliaPack *x, t_floatarg f)
{
    x->av[46].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f47(t_ofeliaPack *x, t_floatarg f)
{
    x->av[47].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f48(t_ofeliaPack *x, t_floatarg f)
{
    x->av[48].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f49(t_ofeliaPack *x, t_floatarg f)
{
    x->av[49].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f50(t_ofeliaPack *x, t_floatarg f)
{
    x->av[50].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f51(t_ofeliaPack *x, t_floatarg f)
{
    x->av[51].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f52(t_ofeliaPack *x, t_floatarg f)
{
    x->av[52].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f53(t_ofeliaPack *x, t_floatarg f)
{
    x->av[53].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f54(t_ofeliaPack *x, t_floatarg f)
{
    x->av[54].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f55(t_ofeliaPack *x, t_floatarg f)
{
    x->av[55].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f56(t_ofeliaPack *x, t_floatarg f)
{
    x->av[56].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f57(t_ofeliaPack *x, t_floatarg f)
{
    x->av[57].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f58(t_ofeliaPack *x, t_floatarg f)
{
    x->av[58].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f59(t_ofeliaPack *x, t_floatarg f)
{
    x->av[59].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f60(t_ofeliaPack *x, t_floatarg f)
{
    x->av[60].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f61(t_ofeliaPack *x, t_floatarg f)
{
    x->av[61].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f62(t_ofeliaPack *x, t_floatarg f)
{
    x->av[62].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f63(t_ofeliaPack *x, t_floatarg f)
{
    x->av[63].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_f64(t_ofeliaPack *x, t_floatarg f)
{
    x->av[64].a_w.w_float = f;
    outletPackedAtoms(x);
}

void ofeliaPack_s1(t_ofeliaPack *x, t_symbol *s)
{
    x->av[1].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s2(t_ofeliaPack *x, t_symbol *s)
{
    x->av[2].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s3(t_ofeliaPack *x, t_symbol *s)
{
    x->av[3].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s4(t_ofeliaPack *x, t_symbol *s)
{
    x->av[4].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s5(t_ofeliaPack *x, t_symbol *s)
{
    x->av[5].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s6(t_ofeliaPack *x, t_symbol *s)
{
    x->av[6].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s7(t_ofeliaPack *x, t_symbol *s)
{
    x->av[7].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s8(t_ofeliaPack *x, t_symbol *s)
{
    x->av[8].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s9(t_ofeliaPack *x, t_symbol *s)
{
    x->av[9].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s10(t_ofeliaPack *x, t_symbol *s)
{
    x->av[10].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s11(t_ofeliaPack *x, t_symbol *s)
{
    x->av[11].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s12(t_ofeliaPack *x, t_symbol *s)
{
    x->av[12].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s13(t_ofeliaPack *x, t_symbol *s)
{
    x->av[13].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s14(t_ofeliaPack *x, t_symbol *s)
{
    x->av[14].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s15(t_ofeliaPack *x, t_symbol *s)
{
    x->av[15].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s16(t_ofeliaPack *x, t_symbol *s)
{
    x->av[16].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s17(t_ofeliaPack *x, t_symbol *s)
{
    x->av[17].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s18(t_ofeliaPack *x, t_symbol *s)
{
    x->av[18].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s19(t_ofeliaPack *x, t_symbol *s)
{
    x->av[19].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s20(t_ofeliaPack *x, t_symbol *s)
{
    x->av[20].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s21(t_ofeliaPack *x, t_symbol *s)
{
    x->av[21].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s22(t_ofeliaPack *x, t_symbol *s)
{
    x->av[22].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s23(t_ofeliaPack *x, t_symbol *s)
{
    x->av[23].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s24(t_ofeliaPack *x, t_symbol *s)
{
    x->av[24].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s25(t_ofeliaPack *x, t_symbol *s)
{
    x->av[25].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s26(t_ofeliaPack *x, t_symbol *s)
{
    x->av[26].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s27(t_ofeliaPack *x, t_symbol *s)
{
    x->av[27].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s28(t_ofeliaPack *x, t_symbol *s)
{
    x->av[28].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s29(t_ofeliaPack *x, t_symbol *s)
{
    x->av[29].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s30(t_ofeliaPack *x, t_symbol *s)
{
    x->av[30].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s31(t_ofeliaPack *x, t_symbol *s)
{
    x->av[31].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s32(t_ofeliaPack *x, t_symbol *s)
{
    x->av[32].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s33(t_ofeliaPack *x, t_symbol *s)
{
    x->av[33].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s34(t_ofeliaPack *x, t_symbol *s)
{
    x->av[34].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s35(t_ofeliaPack *x, t_symbol *s)
{
    x->av[35].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s36(t_ofeliaPack *x, t_symbol *s)
{
    x->av[36].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s37(t_ofeliaPack *x, t_symbol *s)
{
    x->av[37].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s38(t_ofeliaPack *x, t_symbol *s)
{
    x->av[38].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s39(t_ofeliaPack *x, t_symbol *s)
{
    x->av[39].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s40(t_ofeliaPack *x, t_symbol *s)
{
    x->av[40].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s41(t_ofeliaPack *x, t_symbol *s)
{
    x->av[41].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s42(t_ofeliaPack *x, t_symbol *s)
{
    x->av[42].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s43(t_ofeliaPack *x, t_symbol *s)
{
    x->av[43].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s44(t_ofeliaPack *x, t_symbol *s)
{
    x->av[44].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s45(t_ofeliaPack *x, t_symbol *s)
{
    x->av[45].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s46(t_ofeliaPack *x, t_symbol *s)
{
    x->av[46].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s47(t_ofeliaPack *x, t_symbol *s)
{
    x->av[47].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s48(t_ofeliaPack *x, t_symbol *s)
{
    x->av[48].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s49(t_ofeliaPack *x, t_symbol *s)
{
    x->av[49].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s50(t_ofeliaPack *x, t_symbol *s)
{
    x->av[50].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s51(t_ofeliaPack *x, t_symbol *s)
{
    x->av[51].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s52(t_ofeliaPack *x, t_symbol *s)
{
    x->av[52].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s53(t_ofeliaPack *x, t_symbol *s)
{
    x->av[53].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s54(t_ofeliaPack *x, t_symbol *s)
{
    x->av[54].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s55(t_ofeliaPack *x, t_symbol *s)
{
    x->av[55].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s56(t_ofeliaPack *x, t_symbol *s)
{
    x->av[56].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s57(t_ofeliaPack *x, t_symbol *s)
{
    x->av[57].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s58(t_ofeliaPack *x, t_symbol *s)
{
    x->av[58].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s59(t_ofeliaPack *x, t_symbol *s)
{
    x->av[59].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s60(t_ofeliaPack *x, t_symbol *s)
{
    x->av[60].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s61(t_ofeliaPack *x, t_symbol *s)
{
    x->av[61].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s62(t_ofeliaPack *x, t_symbol *s)
{
    x->av[62].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s63(t_ofeliaPack *x, t_symbol *s)
{
    x->av[63].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_s64(t_ofeliaPack *x, t_symbol *s)
{
    x->av[64].a_w.w_symbol = s;
    outletPackedAtoms(x);
}

void ofeliaPack_free(t_ofeliaPack *x)
{
    freebytes(x->av, x->ac * sizeof(t_atom));
}

void ofeliaPack_setup()
{
    ofeliaPack_class = class_new(gensym("ofPack"),
                                 reinterpret_cast<t_newmethod>(ofeliaPack_new),
                                 reinterpret_cast<t_method>(ofeliaPack_free),
                                 sizeof(t_ofeliaPack),
                                 CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_bang));
    class_addfloat(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_float));
    class_addsymbol(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_symbol));
    class_addlist(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_list));
    class_addanything(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_anything));
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f1),
                    gensym("{f}1"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f2),
                    gensym("{f}2"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f3),
                    gensym("{f}3"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f4),
                    gensym("{f}4"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f5),
                    gensym("{f}5"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f6),
                    gensym("{f}6"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f7),
                    gensym("{f}7"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f8),
                    gensym("{f}8"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f9),
                    gensym("{f}9"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f10),
                    gensym("{f}10"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f11),
                    gensym("{f}11"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f12),
                    gensym("{f}12"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f13),
                    gensym("{f}13"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f14),
                    gensym("{f}14"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f15),
                    gensym("{f}15"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f16),
                    gensym("{f}16"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f17),
                    gensym("{f}17"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f18),
                    gensym("{f}18"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f19),
                    gensym("{f}19"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f20),
                    gensym("{f}20"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f21),
                    gensym("{f}21"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f22),
                    gensym("{f}22"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f23),
                    gensym("{f}23"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f24),
                    gensym("{f}24"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f25),
                    gensym("{f}25"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f26),
                    gensym("{f}26"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f27),
                    gensym("{f}27"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f28),
                    gensym("{f}28"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f29),
                    gensym("{f}29"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f30),
                    gensym("{f}30"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f31),
                    gensym("{f}31"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f32),
                    gensym("{f}32"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f33),
                    gensym("{f}33"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f34),
                    gensym("{f}34"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f35),
                    gensym("{f}35"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f36),
                    gensym("{f}36"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f37),
                    gensym("{f}37"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f38),
                    gensym("{f}38"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f39),
                    gensym("{f}39"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f40),
                    gensym("{f}40"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f41),
                    gensym("{f}41"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f42),
                    gensym("{f}42"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f43),
                    gensym("{f}43"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f44),
                    gensym("{f}44"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f45),
                    gensym("{f}45"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f46),
                    gensym("{f}46"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f47),
                    gensym("{f}47"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f48),
                    gensym("{f}48"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f49),
                    gensym("{f}49"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f50),
                    gensym("{f}50"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f51),
                    gensym("{f}51"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f52),
                    gensym("{f}52"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f53),
                    gensym("{f}53"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f54),
                    gensym("{f}54"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f55),
                    gensym("{f}55"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f56),
                    gensym("{f}56"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f57),
                    gensym("{f}57"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f58),
                    gensym("{f}58"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f59),
                    gensym("{f}59"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f60),
                    gensym("{f}60"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f61),
                    gensym("{f}61"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f62),
                    gensym("{f}62"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f63),
                    gensym("{f}63"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_f64),
                    gensym("{f}64"), A_FLOAT, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s1),
                    gensym("{s}1"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s2),
                    gensym("{s}2"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s3),
                    gensym("{s}3"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s4),
                    gensym("{s}4"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s5),
                    gensym("{s}5"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s6),
                    gensym("{s}6"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s7),
                    gensym("{s}7"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s8),
                    gensym("{s}8"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s9),
                    gensym("{s}9"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s10),
                    gensym("{s}10"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s11),
                    gensym("{s}11"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s12),
                    gensym("{s}12"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s13),
                    gensym("{s}13"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s14),
                    gensym("{s}14"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s15),
                    gensym("{s}15"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s16),
                    gensym("{s}16"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s17),
                    gensym("{s}17"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s18),
                    gensym("{s}18"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s19),
                    gensym("{s}19"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s20),
                    gensym("{s}20"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s21),
                    gensym("{s}21"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s22),
                    gensym("{s}22"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s23),
                    gensym("{s}23"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s24),
                    gensym("{s}24"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s25),
                    gensym("{s}25"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s26),
                    gensym("{s}26"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s27),
                    gensym("{s}27"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s28),
                    gensym("{s}28"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s29),
                    gensym("{s}29"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s30),
                    gensym("{s}30"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s31),
                    gensym("{s}31"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s32),
                    gensym("{s}32"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s33),
                    gensym("{s}33"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s34),
                    gensym("{s}34"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s35),
                    gensym("{s}35"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s36),
                    gensym("{s}36"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s37),
                    gensym("{s}37"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s38),
                    gensym("{s}38"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s39),
                    gensym("{s}39"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s40),
                    gensym("{s}40"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s41),
                    gensym("{s}41"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s42),
                    gensym("{s}42"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s43),
                    gensym("{s}43"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s44),
                    gensym("{s}44"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s45),
                    gensym("{s}45"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s46),
                    gensym("{s}46"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s47),
                    gensym("{s}47"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s48),
                    gensym("{s}48"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s49),
                    gensym("{s}49"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s50),
                    gensym("{s}50"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s51),
                    gensym("{s}51"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s52),
                    gensym("{s}52"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s53),
                    gensym("{s}53"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s54),
                    gensym("{s}54"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s55),
                    gensym("{s}55"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s56),
                    gensym("{s}56"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s57),
                    gensym("{s}57"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s58),
                    gensym("{s}58"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s59),
                    gensym("{s}59"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s60),
                    gensym("{s}60"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s61),
                    gensym("{s}61"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s62),
                    gensym("{s}62"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s63),
                    gensym("{s}63"), A_SYMBOL, 0);
    class_addmethod(ofeliaPack_class, reinterpret_cast<t_method>(ofeliaPack_s64),
                    gensym("{s}64"), A_SYMBOL, 0);
}

/* ________________________________________________________________________________
 * common methods
 */
bool getAtomElemFromArgs(const int argc, const t_atom *argv, t_ofeliaAtomElem &elem, const char *objName)
{
    if (argc) {
        
        if (argv[0].a_type == A_FLOAT) {
            
            elem.type = ATOM_ELEM_TYPE_FLOAT;
            elem.value = argv[0].a_w.w_float;
            elem.name = "";
        }
        else if (argv[0].a_type == A_SYMBOL) {
            
            elem.type = ATOM_ELEM_TYPE_SYMBOL;
            elem.value = 0.0f;
            elem.name = argv[0].a_w.w_symbol->s_name;
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

bool getAtomElemsFromArgs(const int argc, const t_atom *argv, vector<t_ofeliaAtomElem> &elems, const char *objName)
{
    vector<t_ofeliaAtomElem> atoms(argc);
    
    for (int i=0; i<argc; ++i) {
        
        if (argv[i].a_type == A_FLOAT) {
            
            atoms[i].type = ATOM_ELEM_TYPE_FLOAT;
            atoms[i].value = argv[i].a_w.w_float;
            atoms[i].name = "";
        }
        else if (argv[i].a_type == A_SYMBOL) {
            
            atoms[i].type = ATOM_ELEM_TYPE_SYMBOL;
            atoms[i].value = 0.0f;
            atoms[i].name = argv[i].a_w.w_symbol->s_name;
        }
        else {
            
            error("%s: wrong argument type", objName);
            return 0;
        }
    }
    elems = atoms;
    return 1;
}

bool operator==(t_ofeliaAtomElem const& a, t_ofeliaAtomElem const& b)
{
    return a.type == b.type && a.value == b.value && a.name == b.name;
}

void getAtomElemsFoundPos(const vector<t_ofeliaAtomElem> &haystack, const vector<t_ofeliaAtomElem> &needle, vector<size_t> &foundPos)
{
    auto it = haystack.begin();
    while ((it = search(it, haystack.end(), needle.begin(), needle.end())) != haystack.end())
        foundPos.push_back(distance(haystack.begin(), it++));
}

bool getListPosRangeFromArgs(const int argc, const t_atom *argv, t_float &firstPos, t_float &lastPos, const char *objName)
{
    switch (argc) {
            
        case 0:
        {
            firstPos = lastPos = numeric_limits<t_float>::max();
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                const t_float pos = argv[0].a_w.w_float;
                
                if (pos >= 0.0f) {
                    
                    firstPos = lastPos = truncf(pos);
                }
                else {
                    
                    error("%s: pos out of range", objName);
                    return 0;
                }
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
                
                const t_float first = argv[0].a_w.w_float;
                const t_float last = max(argv[1].a_w.w_float, first);
                
                if (first >= 0.0f) {
                    
                    firstPos = truncf(first);
                    lastPos = truncf(last);
                }
                else {
                    
                    error("%s: pos out of range", objName);
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

/* ________________________________________________________________________________
 * ofListFind object methods
 */
void *ofeliaListFind_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaListFind *x = reinterpret_cast<t_ofeliaListFind*>(pd_new(ofeliaListFind_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getAtomElemsFromArgs(argc, argv, x->elems, t_ofeliaListFind::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("needle"));
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaListFind_list(t_ofeliaListFind *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaAtomElem> elems;
    
    if (getAtomElemsFromArgs(argc, argv, elems, t_ofeliaListFind::objName)) {
        
        vector<size_t> foundPos;
        getAtomElemsFoundPos(elems, x->elems, foundPos);
        const int ac = static_cast<int>(foundPos.size());
        t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
        
        for (int i=0; i<ac; ++i) {
            
            av[i].a_type = A_FLOAT;
            av[i].a_w.w_float = static_cast<t_float>(foundPos[i]);
        }
        outlet_list(x->x_obj.ob_outlet, &s_list, ac, av);
        freebytes(av, ac * sizeof(t_atom));
    }
}

void ofeliaListFind_set(t_ofeliaListFind *x, t_symbol *s, int argc, t_atom *argv)
{
    getAtomElemsFromArgs(argc, argv, x->elems, t_ofeliaListFind::objName);
}

void ofeliaListFind_print(t_ofeliaListFind *x)
{
    post("\n[%s]", t_ofeliaListFind::objName);
    stringstream ss;
    const char space = ' ';
    
    for (size_t i=0; i<x->elems.size(); ++i) {
        
        if (x->elems[i].type == ATOM_ELEM_TYPE_FLOAT)
            ss << x->elems[i].value;
        else if (x->elems[i].type == ATOM_ELEM_TYPE_SYMBOL)
            ss << x->elems[i].name;
        ss << space;
    }
    t_string str = ss.str().c_str();
    str.pop_back();
    post("needle : %s", str.c_str());
}

void ofeliaListFind_setup()
{
    ofeliaListFind_class = class_new(gensym("ofListFind"),
                                     reinterpret_cast<t_newmethod>(ofeliaListFind_new),
                                     0, sizeof(t_ofeliaListFind),
                                     CLASS_DEFAULT, A_GIMME, 0);
    class_addlist(ofeliaListFind_class, reinterpret_cast<t_method>(ofeliaListFind_list));
    class_addmethod(ofeliaListFind_class, reinterpret_cast<t_method>(ofeliaListFind_set),
                    gensym("needle"), A_GIMME, 0);
    class_addmethod(ofeliaListFind_class, reinterpret_cast<t_method>(ofeliaListFind_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaListFind_class, reinterpret_cast<t_method>(ofeliaListFind_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofFindList object methods
 */
void *ofeliaFindList_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaFindList *x = reinterpret_cast<t_ofeliaFindList*>(pd_new(ofeliaFindList_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getAtomElemsFromArgs(argc, argv, x->elems, t_ofeliaFindList::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("haystack"));
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaFindList_list(t_ofeliaFindList *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaAtomElem> elems;
    
    if (getAtomElemsFromArgs(argc, argv, elems, t_ofeliaFindList::objName)) {
        
        vector<size_t> foundPos;
        getAtomElemsFoundPos(x->elems, elems, foundPos);
        const int ac = static_cast<int>(foundPos.size());
        t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
        
        for (int i=0; i<ac; ++i) {
            
            av[i].a_type = A_FLOAT;
            av[i].a_w.w_float = static_cast<t_float>(foundPos[i]);
        }
        outlet_list(x->x_obj.ob_outlet, &s_list, ac, av);
        freebytes(av, ac * sizeof(t_atom));
    }
}

void ofeliaFindList_set(t_ofeliaFindList *x, t_symbol *s, int argc, t_atom *argv)
{
    getAtomElemsFromArgs(argc, argv, x->elems, t_ofeliaFindList::objName);
}

void ofeliaFindList_print(t_ofeliaFindList *x)
{
    post("\n[%s]", t_ofeliaFindList::objName);
    stringstream ss;
    const char space = ' ';
    
    for (size_t i=0; i<x->elems.size(); ++i) {
        
        if (x->elems[i].type == ATOM_ELEM_TYPE_FLOAT)
            ss << x->elems[i].value;
        else if (x->elems[i].type == ATOM_ELEM_TYPE_SYMBOL)
            ss << x->elems[i].name;
        ss << space;
    }
    t_string str = ss.str().c_str();
    str.pop_back();
    post("haystack : %s", str.c_str());
}

void ofeliaFindList_setup()
{
    ofeliaFindList_class = class_new(gensym("ofFindList"),
                                     reinterpret_cast<t_newmethod>(ofeliaFindList_new),
                                     0, sizeof(t_ofeliaFindList),
                                     CLASS_DEFAULT, A_GIMME, 0);
    class_addlist(ofeliaFindList_class, reinterpret_cast<t_method>(ofeliaFindList_list));
    class_addmethod(ofeliaFindList_class, reinterpret_cast<t_method>(ofeliaFindList_set),
                    gensym("haystack"), A_GIMME, 0);
    class_addmethod(ofeliaFindList_class, reinterpret_cast<t_method>(ofeliaFindList_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaFindList_class, reinterpret_cast<t_method>(ofeliaFindList_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofListInsert object methods
 */
void *ofeliaListInsert_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaListInsert *x = reinterpret_cast<t_ofeliaListInsert*>(pd_new(ofeliaListInsert_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getAtomElemsFromArgs(argc, argv, x->elems, t_ofeliaListInsert::objName))
        return 0;
    x->pos = numeric_limits<t_float>::max();
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("needle"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("pos"));
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaListInsert_list(t_ofeliaListInsert *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaAtomElem> elems;
    
    if (getAtomElemsFromArgs(argc, argv, elems, t_ofeliaListInsert::objName)) {
        
        const int pos = static_cast<int>(ofClamp(x->pos, 0.0f, elems.size()));
        elems.insert(elems.begin()+pos, x->elems.begin(), x->elems.end());
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
        outlet_list(x->x_obj.ob_outlet, &s_list, ac, av);
        freebytes(av, ac * sizeof(t_atom));
    }
}

void ofeliaListInsert_pos(t_ofeliaListInsert *x, t_floatarg f)
{
    if (f >= 0.0f)
        x->pos = truncf(f);
    else
        error("%s: pos out of range", t_ofeliaListInsert::objName);
}

void ofeliaListInsert_set(t_ofeliaListInsert *x, t_symbol *s, int argc, t_atom *argv)
{
    getAtomElemsFromArgs(argc, argv, x->elems, t_ofeliaListInsert::objName);
}

void ofeliaListInsert_print(t_ofeliaListInsert *x)
{
    post("\n[%s]", t_ofeliaListInsert::objName);
    stringstream ss;
    const char space = ' ';
    
    for (size_t i=0; i<x->elems.size(); ++i) {
        
        if (x->elems[i].type == ATOM_ELEM_TYPE_FLOAT)
            ss << x->elems[i].value;
        else if (x->elems[i].type == ATOM_ELEM_TYPE_SYMBOL)
            ss << x->elems[i].name;
        ss << space;
    }
    t_string str = ss.str().c_str();
    str.pop_back();
    post("needle : %s", str.c_str());
    post("pos : %g", x->pos);
}

void ofeliaListInsert_setup()
{
    ofeliaListInsert_class = class_new(gensym("ofListInsert"),
                                       reinterpret_cast<t_newmethod>(ofeliaListInsert_new),
                                       0, sizeof(t_ofeliaListInsert),
                                       CLASS_DEFAULT, A_GIMME, 0);
    class_addlist(ofeliaListInsert_class, reinterpret_cast<t_method>(ofeliaListInsert_list));
    class_addmethod(ofeliaListInsert_class, reinterpret_cast<t_method>(ofeliaListInsert_set),
                    gensym("needle"), A_GIMME, 0);
    class_addmethod(ofeliaListInsert_class, reinterpret_cast<t_method>(ofeliaListInsert_pos),
                    gensym("pos"), A_FLOAT, 0);
    class_addmethod(ofeliaListInsert_class, reinterpret_cast<t_method>(ofeliaListInsert_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaListInsert_class, reinterpret_cast<t_method>(ofeliaListInsert_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofInsertList object methods
 */
void *ofeliaInsertList_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaInsertList *x = reinterpret_cast<t_ofeliaInsertList*>(pd_new(ofeliaInsertList_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getAtomElemsFromArgs(argc, argv, x->elems, t_ofeliaInsertList::objName))
        return 0;
    x->pos = numeric_limits<t_float>::max();
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("pos"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("haystack"));
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaInsertList_list(t_ofeliaInsertList *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaAtomElem> elems;
    
    if (getAtomElemsFromArgs(argc, argv, elems, t_ofeliaInsertList::objName)) {
        
        vector<t_ofeliaAtomElem> result = x->elems;
        const int pos = static_cast<int>(ofClamp(x->pos, 0.0f, result.size()));
        result.insert(result.begin()+pos, elems.begin(), elems.end());
        const int ac = static_cast<int>(result.size());
        t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
        
        for (int i=0; i<ac; ++i) {
            
            if (result[i].type == ATOM_ELEM_TYPE_FLOAT) {
                
                av[i].a_type = A_FLOAT;
                av[i].a_w.w_float = result[i].value;
            }
            else if (result[i].type == ATOM_ELEM_TYPE_SYMBOL) {
                
                av[i].a_type = A_SYMBOL;
                av[i].a_w.w_symbol = gensym(result[i].name.c_str());
            }
        }
        outlet_list(x->x_obj.ob_outlet, &s_list, ac, av);
        freebytes(av, ac * sizeof(t_atom));
    }
}

void ofeliaInsertList_pos(t_ofeliaInsertList *x, t_floatarg f)
{
    if (f >= 0.0f)
        x->pos = truncf(f);
    else
        error("%s: pos out of range", t_ofeliaInsertList::objName);
}

void ofeliaInsertList_set(t_ofeliaInsertList *x, t_symbol *s, int argc, t_atom *argv)
{
    getAtomElemsFromArgs(argc, argv, x->elems, t_ofeliaInsertList::objName);
}

void ofeliaInsertList_print(t_ofeliaInsertList *x)
{
    post("\n[%s]", t_ofeliaInsertList::objName);
    post("pos : %g", x->pos);
    stringstream ss;
    const char space = ' ';
    
    for (size_t i=0; i<x->elems.size(); ++i) {
        
        if (x->elems[i].type == ATOM_ELEM_TYPE_FLOAT)
            ss << x->elems[i].value;
        else if (x->elems[i].type == ATOM_ELEM_TYPE_SYMBOL)
            ss << x->elems[i].name;
        ss << space;
    }
    t_string str = ss.str().c_str();
    str.pop_back();
    post("haystack : %s", str.c_str());
}

void ofeliaInsertList_setup()
{
    ofeliaInsertList_class = class_new(gensym("ofInsertList"),
                                       reinterpret_cast<t_newmethod>(ofeliaInsertList_new),
                                       0, sizeof(t_ofeliaInsertList),
                                       CLASS_DEFAULT, A_GIMME, 0);
    class_addlist(ofeliaInsertList_class, reinterpret_cast<t_method>(ofeliaInsertList_list));
    class_addmethod(ofeliaInsertList_class, reinterpret_cast<t_method>(ofeliaInsertList_pos),
                    gensym("pos"), A_FLOAT, 0);
    class_addmethod(ofeliaInsertList_class, reinterpret_cast<t_method>(ofeliaInsertList_set),
                    gensym("haystack"), A_GIMME, 0);
    class_addmethod(ofeliaInsertList_class, reinterpret_cast<t_method>(ofeliaInsertList_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaInsertList_class, reinterpret_cast<t_method>(ofeliaInsertList_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofListFill object methods
 */
void *ofeliaListFill_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaListFill *x = reinterpret_cast<t_ofeliaListFill*>(pd_new(ofeliaListFill_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (argc) {
        
        if (getAtomElemFromArgs(argc, argv, x->elem, t_ofeliaListFill::objName)) {
            
            if (!getListPosRangeFromArgs(argc-1, argv+1, x->firstPos, x->lastPos, t_ofeliaListFill::objName))
                return 0;
        }
    }
    else {
        
        x->elem.type = ATOM_ELEM_TYPE_FLOAT;
        x->elem.value = 0.0f;
        x->elem.name = "";
        getListPosRangeFromArgs(argc, argv, x->firstPos, x->lastPos, t_ofeliaListFill::objName);
    }
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("needle"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("pos"));
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaListFill_list(t_ofeliaListFill *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaAtomElem> elems;
    
    if (getAtomElemsFromArgs(argc, argv, elems, t_ofeliaListFill::objName)) {
        
        const int size = static_cast<int>(elems.size());
        
        if (size) {

            const int maxIndex = size - 1;
            const int fromIndex = static_cast<int>(ofClamp(x->firstPos, 0.0f, maxIndex));
            const int toIndex = static_cast<int>(ofClamp(x->lastPos, fromIndex, maxIndex)) + 1;
            fill(elems.begin() + fromIndex, elems.begin() + toIndex, x->elem);
        }
        for (int i=0; i<argc; ++i) {
            
            if (elems[i].type == ATOM_ELEM_TYPE_FLOAT) {
                
                argv[i].a_type = A_FLOAT;
                argv[i].a_w.w_float = elems[i].value;
            }
            else if (elems[i].type == ATOM_ELEM_TYPE_SYMBOL) {
                
                argv[i].a_type = A_SYMBOL;
                argv[i].a_w.w_symbol = gensym(elems[i].name.c_str());
            }
        }
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
    }
}

void ofeliaListFill_needle(t_ofeliaListFill *x, t_symbol *s, int argc, t_atom *argv)
{
    getAtomElemFromArgs(argc, argv, x->elem, t_ofeliaListFill::objName);
}

void ofeliaListFill_pos(t_ofeliaListFill *x, t_symbol *s, int argc, t_atom *argv)
{
    getListPosRangeFromArgs(argc, argv, x->firstPos, x->lastPos, t_ofeliaListFill::objName);
}

void ofeliaListFill_set(t_ofeliaListFill *x, t_symbol *s, int argc, t_atom *argv)
{
    if (getAtomElemFromArgs(argc, argv, x->elem, t_ofeliaListFill::objName))
        getListPosRangeFromArgs(argc-1, argv+1, x->firstPos, x->lastPos, t_ofeliaListFill::objName);
}

void ofeliaListFill_print(t_ofeliaListFill *x)
{
    post("\n[%s]", t_ofeliaListFill::objName);
    
    if (x->elem.type == ATOM_ELEM_TYPE_FLOAT)
        post("needle : %g", x->elem.value);
    else if (x->elem.type == ATOM_ELEM_TYPE_SYMBOL)
        post("needle : %s", x->elem.name.c_str());
    post("pos : %g %g", x->firstPos, x->lastPos);
}

void ofeliaListFill_setup()
{
    ofeliaListFill_class = class_new(gensym("ofListFill"),
                                     reinterpret_cast<t_newmethod>(ofeliaListFill_new),
                                     0, sizeof(t_ofeliaListFill),
                                     CLASS_DEFAULT, A_GIMME, 0);
    class_addlist(ofeliaListFill_class, reinterpret_cast<t_method>(ofeliaListFill_list));
    class_addmethod(ofeliaListFill_class, reinterpret_cast<t_method>(ofeliaListFill_needle),
                    gensym("needle"), A_GIMME, 0);
    class_addmethod(ofeliaListFill_class, reinterpret_cast<t_method>(ofeliaListFill_pos),
                    gensym("pos"), A_GIMME, 0);
    class_addmethod(ofeliaListFill_class, reinterpret_cast<t_method>(ofeliaListFill_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaListFill_class, reinterpret_cast<t_method>(ofeliaListFill_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofFillList object methods
 */
void *ofeliaFillList_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaFillList *x = reinterpret_cast<t_ofeliaFillList*>(pd_new(ofeliaFillList_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getAtomElemsFromArgs(argc, argv, x->elems, t_ofeliaFillList::objName))
        return 0;
    x->firstPos = x->lastPos = numeric_limits<t_float>::max();
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("pos"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("haystack"));
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaFillList_list(t_ofeliaFillList *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaAtomElem value;
    
    if (getAtomElemFromArgs(argc, argv, value, t_ofeliaFillList::objName)) {
     
        vector<t_ofeliaAtomElem> result = x->elems;
        const int size = static_cast<int>(result.size());
        
        if (size) {
            
            const int maxIndex = size - 1;
            const int fromIndex = static_cast<int>(ofClamp(x->firstPos, 0.0f, maxIndex));
            const int toIndex = static_cast<int>(ofClamp(x->lastPos, fromIndex, maxIndex)) + 1;
            fill(result.begin() + fromIndex, result.begin() + toIndex, value);
        }
        const int ac = static_cast<int>(result.size());
        t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
        
        for (int i=0; i<ac; ++i) {
            
            if (result[i].type == ATOM_ELEM_TYPE_FLOAT) {
                
                av[i].a_type = A_FLOAT;
                av[i].a_w.w_float = result[i].value;
            }
            else if (result[i].type == ATOM_ELEM_TYPE_SYMBOL) {
                
                av[i].a_type = A_SYMBOL;
                av[i].a_w.w_symbol = gensym(result[i].name.c_str());
            }
        }
        outlet_list(x->x_obj.ob_outlet, &s_list, ac, av);
        freebytes(av, ac * sizeof(t_atom));
    }
}

void ofeliaFillList_pos(t_ofeliaFillList *x, t_symbol *s, int argc, t_atom *argv)
{
    getListPosRangeFromArgs(argc, argv, x->firstPos, x->lastPos, t_ofeliaFillList::objName);
}

void ofeliaFillList_set(t_ofeliaFillList *x, t_symbol *s, int argc, t_atom *argv)
{
    getAtomElemsFromArgs(argc, argv, x->elems, t_ofeliaFillList::objName);
}

void ofeliaFillList_print(t_ofeliaFillList *x)
{
    post("\n[%s]", t_ofeliaFillList::objName);
    post("pos : %g %g", x->firstPos, x->lastPos);
    stringstream ss;
    const char space = ' ';
    
    for (size_t i=0; i<x->elems.size(); ++i) {
        
        if (x->elems[i].type == ATOM_ELEM_TYPE_FLOAT)
            ss << x->elems[i].value;
        else if (x->elems[i].type == ATOM_ELEM_TYPE_SYMBOL)
            ss << x->elems[i].name;
        ss << space;
    }
    t_string str = ss.str().c_str();
    str.pop_back();
    post("haystack : %s", str.c_str());
}

void ofeliaFillList_setup()
{
    ofeliaFillList_class = class_new(gensym("ofFillList"),
                                     reinterpret_cast<t_newmethod>(ofeliaFillList_new),
                                     0, sizeof(t_ofeliaFillList),
                                     CLASS_DEFAULT, A_GIMME, 0);
    class_addlist(ofeliaFillList_class, reinterpret_cast<t_method>(ofeliaFillList_list));
    class_addmethod(ofeliaFillList_class, reinterpret_cast<t_method>(ofeliaFillList_pos),
                    gensym("pos"), A_GIMME, 0);
    class_addmethod(ofeliaFillList_class, reinterpret_cast<t_method>(ofeliaFillList_set),
                    gensym("haystack"), A_GIMME, 0);
    class_addmethod(ofeliaFillList_class, reinterpret_cast<t_method>(ofeliaFillList_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaFillList_class, reinterpret_cast<t_method>(ofeliaFillList_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofListReplace object methods
 */
void *ofeliaListReplace_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaListReplace *x = reinterpret_cast<t_ofeliaListReplace*>(pd_new(ofeliaListReplace_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getAtomElemsFromArgs(argc, argv, x->from, t_ofeliaListReplace::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("from"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("to"));
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaListReplace_list(t_ofeliaListReplace *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaAtomElem> elems;
    
    if (getAtomElemsFromArgs(argc, argv, elems, t_ofeliaListReplace::objName)) {
        
        vector<size_t> foundPos;
        getAtomElemsFoundPos(elems, x->from, foundPos);
        const size_t eraseLength = x->from.size();
        
        for (size_t i=foundPos.size(); i-- > 0;) {
            
            const auto erasePos = elems.begin() + foundPos[i];
            elems.erase(erasePos, erasePos + eraseLength);
            elems.insert(erasePos, x->to.begin(), x->to.end());
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
        outlet_list(x->x_obj.ob_outlet, &s_list, ac, av);
        freebytes(av, ac * sizeof(t_atom));
    }
}

void ofeliaListReplace_to(t_ofeliaListReplace *x, t_symbol *s, int argc, t_atom *argv)
{
    getAtomElemsFromArgs(argc, argv, x->to, t_ofeliaListReplace::objName);
}

void ofeliaListReplace_set(t_ofeliaListReplace *x, t_symbol *s, int argc, t_atom *argv)
{
    getAtomElemsFromArgs(argc, argv, x->from, t_ofeliaListReplace::objName);
}

void ofeliaListReplace_print(t_ofeliaListReplace *x)
{
    post("\n[%s]", t_ofeliaListReplace::objName);
    stringstream ss1, ss2;
    const char space = ' ';
    
    for (size_t i=0; i<x->from.size(); ++i) {
        
        if (x->from[i].type == ATOM_ELEM_TYPE_FLOAT)
            ss1 << x->from[i].value;
        else if (x->from[i].type == ATOM_ELEM_TYPE_SYMBOL)
            ss1 << x->from[i].name;
        ss1 << space;
    }
    t_string str1 = ss1.str().c_str();
    str1.pop_back();
    post("from : %s", str1.c_str());
    
    for (size_t i=0; i<x->to.size(); ++i) {
        
        if (x->to[i].type == ATOM_ELEM_TYPE_FLOAT)
            ss2 << x->to[i].value;
        else if (x->to[i].type == ATOM_ELEM_TYPE_SYMBOL)
            ss2 << x->to[i].name;
        ss2 << space;
    }
    t_string str2 = ss2.str().c_str();
    str2.pop_back();
    post("to : %s", str2.c_str());
}

void ofeliaListReplace_setup()
{
    ofeliaListReplace_class = class_new(gensym("ofListReplace"),
                                        reinterpret_cast<t_newmethod>(ofeliaListReplace_new),
                                        0, sizeof(t_ofeliaListReplace),
                                        CLASS_DEFAULT, A_GIMME, 0);
    class_addlist(ofeliaListReplace_class, reinterpret_cast<t_method>(ofeliaListReplace_list));
    class_addmethod(ofeliaListReplace_class, reinterpret_cast<t_method>(ofeliaListReplace_set),
                    gensym("from"), A_GIMME, 0);
    class_addmethod(ofeliaListReplace_class, reinterpret_cast<t_method>(ofeliaListReplace_to),
                    gensym("to"), A_GIMME, 0);
    class_addmethod(ofeliaListReplace_class, reinterpret_cast<t_method>(ofeliaListReplace_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaListReplace_class, reinterpret_cast<t_method>(ofeliaListReplace_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofReplaceList object methods
 */
void *ofeliaReplaceList_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaReplaceList *x = reinterpret_cast<t_ofeliaReplaceList*>(pd_new(ofeliaReplaceList_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getAtomElemsFromArgs(argc, argv, x->src, t_ofeliaReplaceList::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("to"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("src"));
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaReplaceList_list(t_ofeliaReplaceList *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaAtomElem> elems;
    
    if (getAtomElemsFromArgs(argc, argv, elems, t_ofeliaReplaceList::objName)) {
        
        vector<size_t> foundPos;
        vector<t_ofeliaAtomElem> result = x->src;
        getAtomElemsFoundPos(result, elems, foundPos);
        const size_t eraseLength = elems.size();
        
        for (size_t i=foundPos.size(); i-- > 0;) {
            
            const auto erasePos = result.begin() + foundPos[i];
            result.erase(erasePos, erasePos + eraseLength);
            result.insert(erasePos, x->to.begin(), x->to.end());
        }
        const int ac = static_cast<int>(result.size());
        t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
        
        for (int i=0; i<ac; ++i) {
            
            if (result[i].type == ATOM_ELEM_TYPE_FLOAT) {
                
                av[i].a_type = A_FLOAT;
                av[i].a_w.w_float = result[i].value;
            }
            else if (result[i].type == ATOM_ELEM_TYPE_SYMBOL) {
                
                av[i].a_type = A_SYMBOL;
                av[i].a_w.w_symbol = gensym(result[i].name.c_str());
            }
        }
        outlet_list(x->x_obj.ob_outlet, &s_list, ac, av);
        freebytes(av, ac * sizeof(t_atom));
    }
}

void ofeliaReplaceList_to(t_ofeliaReplaceList *x, t_symbol *s, int argc, t_atom *argv)
{
    getAtomElemsFromArgs(argc, argv, x->to, t_ofeliaReplaceList::objName);
}

void ofeliaReplaceList_set(t_ofeliaReplaceList *x, t_symbol *s, int argc, t_atom *argv)
{
    getAtomElemsFromArgs(argc, argv, x->src, t_ofeliaReplaceList::objName);
}

void ofeliaReplaceList_print(t_ofeliaReplaceList *x)
{
    post("\n[%s]", t_ofeliaReplaceList::objName);
    stringstream ss1, ss2;
    const char space = ' ';
    
    for (size_t i=0; i<x->to.size(); ++i) {
        
        if (x->to[i].type == ATOM_ELEM_TYPE_FLOAT)
            ss1 << x->to[i].value;
        else if (x->to[i].type == ATOM_ELEM_TYPE_SYMBOL)
            ss1 << x->to[i].name;
        ss1 << space;
    }
    t_string str1 = ss1.str().c_str();
    str1.pop_back();
    post("to : %s", str1.c_str());
    
    for (size_t i=0; i<x->src.size(); ++i) {
        
        if (x->src[i].type == ATOM_ELEM_TYPE_FLOAT)
            ss2 << x->src[i].value;
        else if (x->src[i].type == ATOM_ELEM_TYPE_SYMBOL)
            ss2 << x->src[i].name;
        ss2 << space;
    }
    t_string str2 = ss2.str().c_str();
    str2.pop_back();
    post("src : %s", str2.c_str());
}

void ofeliaReplaceList_setup()
{
    ofeliaReplaceList_class = class_new(gensym("ofReplaceList"),
                                        reinterpret_cast<t_newmethod>(ofeliaReplaceList_new),
                                        0, sizeof(t_ofeliaReplaceList),
                                        CLASS_DEFAULT, A_GIMME, 0);
    class_addlist(ofeliaReplaceList_class, reinterpret_cast<t_method>(ofeliaReplaceList_list));
    class_addmethod(ofeliaReplaceList_class, reinterpret_cast<t_method>(ofeliaReplaceList_to),
                    gensym("to"), A_GIMME, 0);
    class_addmethod(ofeliaReplaceList_class, reinterpret_cast<t_method>(ofeliaReplaceList_set),
                    gensym("src"), A_GIMME, 0);
    class_addmethod(ofeliaReplaceList_class, reinterpret_cast<t_method>(ofeliaReplaceList_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaReplaceList_class, reinterpret_cast<t_method>(ofeliaReplaceList_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofListRemove object methods
 */
void *ofeliaListRemove_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaListRemove *x = reinterpret_cast<t_ofeliaListRemove*>(pd_new(ofeliaListRemove_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getAtomElemsFromArgs(argc, argv, x->elems, t_ofeliaListRemove::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("needle"));
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaListRemove_list(t_ofeliaListRemove *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaAtomElem> elems;
    
    if (getAtomElemsFromArgs(argc, argv, elems, t_ofeliaListRemove::objName)) {
        
        vector<size_t> foundPos;
        getAtomElemsFoundPos(elems, x->elems, foundPos);
        const size_t eraseLength = x->elems.size();
        
        for (size_t i=foundPos.size(); i-- > 0;) {
            
            const auto erasePos = elems.begin() + foundPos[i];
            elems.erase(erasePos, erasePos + eraseLength);
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
        outlet_list(x->x_obj.ob_outlet, &s_list, ac, av);
        freebytes(av, ac * sizeof(t_atom));
    }
}

void ofeliaListRemove_set(t_ofeliaListRemove *x, t_symbol *s, int argc, t_atom *argv)
{
    getAtomElemsFromArgs(argc, argv, x->elems, t_ofeliaListRemove::objName);
}

void ofeliaListRemove_print(t_ofeliaListRemove *x)
{
    post("\n[%s]", t_ofeliaListRemove::objName);
    stringstream ss;
    const char space = ' ';
    
    for (size_t i=0; i<x->elems.size(); ++i) {
        
        if (x->elems[i].type == ATOM_ELEM_TYPE_FLOAT)
            ss << x->elems[i].value;
        else if (x->elems[i].type == ATOM_ELEM_TYPE_SYMBOL)
            ss << x->elems[i].name;
        ss << space;
    }
    t_string str = ss.str().c_str();
    str.pop_back();
    post("needle : %s", str.c_str());
}

void ofeliaListRemove_setup()
{
    ofeliaListRemove_class = class_new(gensym("ofListRemove"),
                                       reinterpret_cast<t_newmethod>(ofeliaListRemove_new),
                                       0, sizeof(t_ofeliaListRemove),
                                       CLASS_DEFAULT, A_GIMME, 0);
    class_addlist(ofeliaListRemove_class, reinterpret_cast<t_method>(ofeliaListRemove_list));
    class_addmethod(ofeliaListRemove_class, reinterpret_cast<t_method>(ofeliaListRemove_set),
                    gensym("needle"), A_GIMME, 0);
    class_addmethod(ofeliaListRemove_class, reinterpret_cast<t_method>(ofeliaListRemove_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaListRemove_class, reinterpret_cast<t_method>(ofeliaListRemove_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofRemoveList object methods
 */
void *ofeliaRemoveList_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaRemoveList *x = reinterpret_cast<t_ofeliaRemoveList*>(pd_new(ofeliaRemoveList_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getAtomElemsFromArgs(argc, argv, x->elems, t_ofeliaRemoveList::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("haystack"));
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaRemoveList_list(t_ofeliaRemoveList *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaAtomElem> elems;
    
    if (getAtomElemsFromArgs(argc, argv, elems, t_ofeliaRemoveList::objName)) {
        
        vector<size_t> foundPos;
        vector<t_ofeliaAtomElem> result = x->elems;
        getAtomElemsFoundPos(result, elems, foundPos);
        const size_t eraseLength = elems.size();
        
        for (size_t i=foundPos.size(); i-- > 0;) {
            
            const auto erasePos = result.begin() + foundPos[i];
            result.erase(erasePos, erasePos + eraseLength);
        }
        const int ac = static_cast<int>(result.size());
        t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
        
        for (int i=0; i<ac; ++i) {
            
            if (result[i].type == ATOM_ELEM_TYPE_FLOAT) {
                
                av[i].a_type = A_FLOAT;
                av[i].a_w.w_float = result[i].value;
            }
            else if (result[i].type == ATOM_ELEM_TYPE_SYMBOL) {
                
                av[i].a_type = A_SYMBOL;
                av[i].a_w.w_symbol = gensym(result[i].name.c_str());
            }
        }
        outlet_list(x->x_obj.ob_outlet, &s_list, ac, av);
        freebytes(av, ac * sizeof(t_atom));
    }
}

void ofeliaRemoveList_set(t_ofeliaRemoveList *x, t_symbol *s, int argc, t_atom *argv)
{
    getAtomElemsFromArgs(argc, argv, x->elems, t_ofeliaRemoveList::objName);
}

void ofeliaRemoveList_print(t_ofeliaRemoveList *x)
{
    post("\n[%s]", t_ofeliaRemoveList::objName);
    stringstream ss;
    const char space = ' ';
    
    for (size_t i=0; i<x->elems.size(); ++i) {
        
        if (x->elems[i].type == ATOM_ELEM_TYPE_FLOAT)
            ss << x->elems[i].value;
        else if (x->elems[i].type == ATOM_ELEM_TYPE_SYMBOL)
            ss << x->elems[i].name;
        ss << space;
    }
    t_string str = ss.str().c_str();
    str.pop_back();
    post("haystack : %s", str.c_str());
}

void ofeliaRemoveList_setup()
{
    ofeliaRemoveList_class = class_new(gensym("ofRemoveList"),
                                       reinterpret_cast<t_newmethod>(ofeliaRemoveList_new),
                                       0, sizeof(t_ofeliaRemoveList),
                                       CLASS_DEFAULT, A_GIMME, 0);
    class_addlist(ofeliaRemoveList_class, reinterpret_cast<t_method>(ofeliaRemoveList_list));
    class_addmethod(ofeliaRemoveList_class, reinterpret_cast<t_method>(ofeliaRemoveList_set),
                    gensym("haystack"), A_GIMME, 0);
    class_addmethod(ofeliaRemoveList_class, reinterpret_cast<t_method>(ofeliaRemoveList_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaRemoveList_class, reinterpret_cast<t_method>(ofeliaRemoveList_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofListErase object methods
 */
void *ofeliaListErase_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaListErase *x = reinterpret_cast<t_ofeliaListErase*>(pd_new(ofeliaListErase_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getListPosRangeFromArgs(argc, argv, x->firstPos, x->lastPos, t_ofeliaListErase::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("pos"));
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaListErase_list(t_ofeliaListErase *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaAtomElem> elems;
    
    if (getAtomElemsFromArgs(argc, argv, elems, t_ofeliaListErase::objName)) {
        
        const int size = static_cast<int>(elems.size());
        
        if (size) {
            
            const int maxIndex = size - 1;
            const int fromIndex = static_cast<int>(ofClamp(x->firstPos, 0.0f, maxIndex));
            const int toIndex = static_cast<int>(ofClamp(x->lastPos, fromIndex, maxIndex)) + 1;
            elems.erase(elems.begin() + fromIndex, elems.begin() + toIndex);
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
        outlet_list(x->x_obj.ob_outlet, &s_list, ac, av);
        freebytes(av, ac * sizeof(t_atom));
    }
}

void ofeliaListErase_set(t_ofeliaListErase *x, t_symbol *s, int argc, t_atom *argv)
{
    getListPosRangeFromArgs(argc, argv, x->firstPos, x->lastPos, t_ofeliaListErase::objName);
}

void ofeliaListErase_print(t_ofeliaListErase *x)
{
    post("\n[%s]", t_ofeliaListErase::objName);
    post("pos : %g %g", x->firstPos, x->lastPos);
}

void ofeliaListErase_setup()
{
    ofeliaListErase_class = class_new(gensym("ofListErase"),
                                      reinterpret_cast<t_newmethod>(ofeliaListErase_new),
                                      0, sizeof(t_ofeliaListErase),
                                      CLASS_DEFAULT, A_GIMME, 0);
    class_addlist(ofeliaListErase_class, reinterpret_cast<t_method>(ofeliaListErase_list));
    class_addmethod(ofeliaListErase_class, reinterpret_cast<t_method>(ofeliaListErase_set),
                    gensym("pos"), A_GIMME, 0);
    class_addmethod(ofeliaListErase_class, reinterpret_cast<t_method>(ofeliaListErase_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaListErase_class, reinterpret_cast<t_method>(ofeliaListErase_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofEraseList object methods
 */
void *ofeliaEraseList_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaEraseList *x = reinterpret_cast<t_ofeliaEraseList*>(pd_new(ofeliaEraseList_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getAtomElemsFromArgs(argc, argv, x->elems, t_ofeliaRemoveList::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("src"));
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaEraseList_list(t_ofeliaEraseList *x, t_symbol *s, int argc, t_atom *argv)
{
    t_float firstPos, lastPos;
    
    if (getListPosRangeFromArgs(argc, argv, firstPos, lastPos, t_ofeliaEraseList::objName)) {
        
        vector<t_ofeliaAtomElem> result = x->elems;
        const int size = static_cast<int>(result.size());
        
        if (size) {
            
            const int maxIndex = size - 1;
            const int fromIndex = static_cast<int>(ofClamp(firstPos, 0.0f, maxIndex));
            const int toIndex = static_cast<int>(ofClamp(lastPos, fromIndex, maxIndex)) + 1;
            result.erase(result.begin() + fromIndex, result.begin() + toIndex);
        }
        const int ac = static_cast<int>(result.size());
        t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
        
        for (int i=0; i<ac; ++i) {
            
            if (result[i].type == ATOM_ELEM_TYPE_FLOAT) {
                
                av[i].a_type = A_FLOAT;
                av[i].a_w.w_float = result[i].value;
            }
            else if (result[i].type == ATOM_ELEM_TYPE_SYMBOL) {
                
                av[i].a_type = A_SYMBOL;
                av[i].a_w.w_symbol = gensym(result[i].name.c_str());
            }
        }
        outlet_list(x->x_obj.ob_outlet, &s_list, ac, av);
        freebytes(av, ac * sizeof(t_atom));
    }
}

void ofeliaEraseList_set(t_ofeliaEraseList *x, t_symbol *s, int argc, t_atom *argv)
{
    getAtomElemsFromArgs(argc, argv, x->elems, t_ofeliaEraseList::objName);
}

void ofeliaEraseList_print(t_ofeliaEraseList *x)
{
    post("\n[%s]", t_ofeliaEraseList::objName);
    stringstream ss;
    const char space = ' ';
    
    for (size_t i=0; i<x->elems.size(); ++i) {
        
        if (x->elems[i].type == ATOM_ELEM_TYPE_FLOAT)
            ss << x->elems[i].value;
        else if (x->elems[i].type == ATOM_ELEM_TYPE_SYMBOL)
            ss << x->elems[i].name;
        ss << space;
    }
    t_string str = ss.str().c_str();
    str.pop_back();
    post("src : %s", str.c_str());
}

void ofeliaEraseList_setup()
{
    ofeliaEraseList_class = class_new(gensym("ofEraseList"),
                                      reinterpret_cast<t_newmethod>(ofeliaEraseList_new),
                                      0, sizeof(t_ofeliaEraseList),
                                      CLASS_DEFAULT, A_GIMME, 0);
    class_addlist(ofeliaEraseList_class, reinterpret_cast<t_method>(ofeliaEraseList_list));
    class_addmethod(ofeliaEraseList_class, reinterpret_cast<t_method>(ofeliaEraseList_set),
                    gensym("src"), A_GIMME, 0);
    class_addmethod(ofeliaEraseList_class, reinterpret_cast<t_method>(ofeliaEraseList_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaEraseList_class, reinterpret_cast<t_method>(ofeliaEraseList_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofListSort object methods
 */
bool getListOrderFromArgs(const int argc, const t_atom *argv, ListOrder &order)
{
    switch (argc) {
            
        case 0:
        {
            order = LIST_ORDER_ASC;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                order = static_cast<ListOrder>(argv[0].a_w.w_float != 0.0f);
            }
            else if (argv[0].a_type == A_SYMBOL) {
                
                const char *mode = argv[0].a_w.w_symbol->s_name;
                
                if (!strcmp(mode, "ASC"))
                    order = LIST_ORDER_ASC;
                else if (!strcmp(mode, "DESC"))
                    order = LIST_ORDER_DESC;
                else {
                    
                    error("%s: no method for '%s'", t_ofeliaListSort::objName, mode);
                    return 0;
                }
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaListSort::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaListSort::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaListSort_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaListSort *x = reinterpret_cast<t_ofeliaListSort*>(pd_new(ofeliaListSort_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getListOrderFromArgs(argc, argv, x->order))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("order"));
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

bool sortListByAscOrder(const t_ofeliaAtomElem &a, const t_ofeliaAtomElem &b)
{
    if (a.type == ATOM_ELEM_TYPE_FLOAT && b.type == ATOM_ELEM_TYPE_FLOAT)
        return a.value < b.value;
    else if (a.type == ATOM_ELEM_TYPE_SYMBOL && b.type == ATOM_ELEM_TYPE_SYMBOL)
        return strcmp(a.name.c_str(), b.name.c_str()) < 0;
    else
        return a.type < b.type;
}

bool sortListByDescOrder(const t_ofeliaAtomElem &a, const t_ofeliaAtomElem &b)
{
    if (a.type == ATOM_ELEM_TYPE_FLOAT && b.type == ATOM_ELEM_TYPE_FLOAT)
        return a.value > b.value;
    else if (a.type == ATOM_ELEM_TYPE_SYMBOL && b.type == ATOM_ELEM_TYPE_SYMBOL)
        return strcmp(a.name.c_str(), b.name.c_str()) > 0;
    else
        return a.type > b.type;
}

void ofeliaListSort_list(t_ofeliaListSort *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaAtomElem> elems;
    
    if (getAtomElemsFromArgs(argc, argv, elems, t_ofeliaListSort::objName)) {
        
        if (x->order == LIST_ORDER_ASC)
            sort(elems.begin(), elems.end(), sortListByAscOrder);
        else if (x->order == LIST_ORDER_DESC)
            sort(elems.begin(), elems.end(), sortListByDescOrder);
        
        for (int i=0; i<argc; ++i) {
            
            if (elems[i].type == ATOM_ELEM_TYPE_FLOAT) {
                
                argv[i].a_type = A_FLOAT;
                argv[i].a_w.w_float = elems[i].value;
            }
            else if (elems[i].type == ATOM_ELEM_TYPE_SYMBOL) {
                
                argv[i].a_type = A_SYMBOL;
                argv[i].a_w.w_symbol = gensym(elems[i].name.c_str());
            }
        }
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
    }
}

void ofeliaListSort_set(t_ofeliaListSort *x, t_symbol *s, int argc, t_atom *argv)
{
    getListOrderFromArgs(argc, argv, x->order);
}

void ofeliaListSort_print(t_ofeliaListSort *x)
{
    post("\n[%s]", t_ofeliaListSort::objName);
    t_string mode;
    
    if (x->order == LIST_ORDER_ASC)
        mode = "ASC";
    else if (x->order == LIST_ORDER_DESC)
        mode = "DESC";
    post("order : %s", mode.c_str());
}

void ofeliaListSort_setup()
{
    ofeliaListSort_class = class_new(gensym("ofListSort"),
                                     reinterpret_cast<t_newmethod>(ofeliaListSort_new),
                                     0, sizeof(t_ofeliaListSort),
                                     CLASS_DEFAULT, A_GIMME, 0);
    class_addlist(ofeliaListSort_class, reinterpret_cast<t_method>(ofeliaListSort_list));
    class_addmethod(ofeliaListSort_class, reinterpret_cast<t_method>(ofeliaListSort_set),
                    gensym("order"), A_GIMME, 0);
    class_addmethod(ofeliaListSort_class, reinterpret_cast<t_method>(ofeliaListSort_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaListSort_class, reinterpret_cast<t_method>(ofeliaListSort_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofListUnique object methods
 */
void *ofeliaListUnique_new()
{
    t_ofeliaListUnique *x = reinterpret_cast<t_ofeliaListUnique*>(pd_new(ofeliaListUnique_class));
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaListUnique_list(t_ofeliaListUnique *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaAtomElem> elems;
    
    if (getAtomElemsFromArgs(argc, argv, elems, t_ofeliaListUnique::objName)) {
        
        vector<t_ofeliaAtomElem> result;
        result.reserve(elems.size());
        
        for (auto it=elems.begin(); it!=elems.end(); ++it) {
            
            if (find(result.begin(), result.end(), *it) == result.end())
                result.push_back(*it);
        }
        const int ac = static_cast<int>(result.size());
        t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
        
        for (int i=0; i<ac; ++i) {
            
            if (result[i].type == ATOM_ELEM_TYPE_FLOAT) {
                
                av[i].a_type = A_FLOAT;
                av[i].a_w.w_float = result[i].value;
            }
            else if (result[i].type == ATOM_ELEM_TYPE_SYMBOL) {
                
                av[i].a_type = A_SYMBOL;
                av[i].a_w.w_symbol = gensym(result[i].name.c_str());
            }
        }
        outlet_list(x->x_obj.ob_outlet, &s_list, ac, av);
        freebytes(av, ac * sizeof(t_atom));
    }
}

void ofeliaListUnique_setup()
{
    ofeliaListUnique_class = class_new(gensym("ofListUnique"),
                                       reinterpret_cast<t_newmethod>(ofeliaListUnique_new),
                                       0, sizeof(t_ofeliaListUnique),
                                       CLASS_DEFAULT, A_NULL, 0);
    class_addlist(ofeliaListUnique_class, reinterpret_cast<t_method>(ofeliaListUnique_list));
}

/* ________________________________________________________________________________
 * ofListReverse object methods
 */
void *ofeliaListReverse_new()
{
    t_ofeliaListReverse *x = reinterpret_cast<t_ofeliaListReverse*>(pd_new(ofeliaListReverse_class));
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaListReverse_list(t_ofeliaListReverse *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaAtomElem> elems;
    
    if (getAtomElemsFromArgs(argc, argv, elems, t_ofeliaListReverse::objName)) {
        
        reverse(elems.begin(), elems.end());
        
        for (int i=0; i<argc; ++i) {
            
            if (elems[i].type == ATOM_ELEM_TYPE_FLOAT) {
                
                argv[i].a_type = A_FLOAT;
                argv[i].a_w.w_float = elems[i].value;
            }
            else if (elems[i].type == ATOM_ELEM_TYPE_SYMBOL) {
                
                argv[i].a_type = A_SYMBOL;
                argv[i].a_w.w_symbol = gensym(elems[i].name.c_str());
            }
        }
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
    }
}


void ofeliaListReverse_setup()
{
    ofeliaListReverse_class = class_new(gensym("ofListReverse"),
                                        reinterpret_cast<t_newmethod>(ofeliaListReverse_new),
                                        0, sizeof(t_ofeliaListReverse),
                                        CLASS_DEFAULT, A_NULL, 0);
    class_addlist(ofeliaListReverse_class, reinterpret_cast<t_method>(ofeliaListReverse_list));
}

/* ________________________________________________________________________________
 * ofListShuffle object methods
 */
void *ofeliaListShuffle_new()
{
    t_ofeliaListShuffle *x = reinterpret_cast<t_ofeliaListShuffle*>(pd_new(ofeliaListShuffle_class));
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaListShuffle_list(t_ofeliaListShuffle *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaAtomElem> elems;
    
    if (getAtomElemsFromArgs(argc, argv, elems, t_ofeliaListShuffle::objName)) {

        shuffle(elems.begin(), elems.end(), default_random_engine(ofRandom(1, RAND_MAX)));
        
        for (int i=0; i<argc; ++i) {
            
            if (elems[i].type == ATOM_ELEM_TYPE_FLOAT) {
                
                argv[i].a_type = A_FLOAT;
                argv[i].a_w.w_float = elems[i].value;
            }
            else if (elems[i].type == ATOM_ELEM_TYPE_SYMBOL) {
                
                argv[i].a_type = A_SYMBOL;
                argv[i].a_w.w_symbol = gensym(elems[i].name.c_str());
            }
        }
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
    }
}

void ofeliaListShuffle_setup()
{
    ofeliaListShuffle_class = class_new(gensym("ofListShuffle"),
                                        reinterpret_cast<t_newmethod>(ofeliaListShuffle_new),
                                        0, sizeof(t_ofeliaListShuffle),
                                        CLASS_DEFAULT, A_NULL, 0);
    class_addlist(ofeliaListShuffle_class, reinterpret_cast<t_method>(ofeliaListShuffle_list));
}

/* ________________________________________________________________________________
 * common methods
 */
bool getDelimiterFromArgs(const int argc, const t_atom *argv, t_string &delimiter, const char *objName)
{
    switch (argc) {
            
        case 0:
        {
            delimiter = " ";
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                delimiter = ofToString(argv[0].a_w.w_float).c_str();
            }
            else if (argv[0].a_type == A_SYMBOL) {
                
                delimiter = argv[0].a_w.w_symbol->s_name;
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

/* ________________________________________________________________________________
 * ofListToSymbol object methods
 */
void *ofeliaListToSymbol_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaListToSymbol *x = reinterpret_cast<t_ofeliaListToSymbol*>(pd_new(ofeliaListToSymbol_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getDelimiterFromArgs(argc, argv, x->delimiter, t_ofeliaListToSymbol::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("delimiter"));
    outlet_new(&x->x_obj, &s_symbol);
    return (x);
}

void ofeliaListToSymbol_float(t_ofeliaListToSymbol *x, t_floatarg f)
{
    outlet_symbol(x->x_obj.ob_outlet, gensym(ofToString(f).c_str()));
}

void ofeliaListToSymbol_symbol(t_ofeliaListToSymbol *x, t_symbol *s)
{
    outlet_symbol(x->x_obj.ob_outlet, s);
}

void ofeliaListToSymbol_list(t_ofeliaListToSymbol *x, t_symbol *s, int argc, t_atom *argv)
{
    stringstream ss;
    
    for (int i=0; i<argc; ++i) {
        
        if (argv[i].a_type == A_FLOAT) {
            
            ss << argv[i].a_w.w_float;
        }
        else if (argv[i].a_type == A_SYMBOL) {
            
            ss << argv[i].a_w.w_symbol->s_name;
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaListToSymbol::objName);
            return;
        }
        if (i<argc-1)
            ss << x->delimiter;
    }
    t_string str(ss.str().c_str());
    outlet_symbol(x->x_obj.ob_outlet, gensym(str.c_str()));
}

void ofeliaListToSymbol_anything(t_ofeliaListToSymbol *x, t_symbol *s, int argc, t_atom *argv)
{
    stringstream ss;
    ss << s->s_name;
    
    if (argc)
        ss << x->delimiter;
    
    for (int i=0; i<argc; ++i) {
        
        if (argv[i].a_type == A_FLOAT) {
            
            ss << argv[i].a_w.w_float;
        }
        else if (argv[i].a_type == A_SYMBOL) {
            
            ss << argv[i].a_w.w_symbol->s_name;
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaListToSymbol::objName);
            return;
        }
        if (i<argc-1)
            ss << x->delimiter;
    }
    t_string str(ss.str().c_str());
    outlet_symbol(x->x_obj.ob_outlet, gensym(str.c_str()));
}

void ofeliaListToSymbol_set(t_ofeliaListToSymbol *x, t_symbol *s, int argc, t_atom *argv)
{
    getDelimiterFromArgs(argc, argv, x->delimiter, t_ofeliaListToSymbol::objName);
}

void ofeliaListToSymbol_print(t_ofeliaListToSymbol *x)
{
    post("\n[%s]", t_ofeliaListToSymbol::objName);
    post("delimiter : %s", x->delimiter.c_str());
}

void ofeliaListToSymbol_setup()
{
    ofeliaListToSymbol_class = class_new(gensym("ofListToSymbol"),
                                         reinterpret_cast<t_newmethod>(ofeliaListToSymbol_new),
                                         0, sizeof(t_ofeliaListToSymbol),
                                         CLASS_DEFAULT, A_GIMME, 0);
    class_addfloat(ofeliaListToSymbol_class, reinterpret_cast<t_method>(ofeliaListToSymbol_float));
    class_addsymbol(ofeliaListToSymbol_class, reinterpret_cast<t_method>(ofeliaListToSymbol_symbol));
    class_addlist(ofeliaListToSymbol_class, reinterpret_cast<t_method>(ofeliaListToSymbol_list));
    class_addanything(ofeliaListToSymbol_class, reinterpret_cast<t_method>(ofeliaListToSymbol_anything));
    class_addmethod(ofeliaListToSymbol_class, reinterpret_cast<t_method>(ofeliaListToSymbol_set),
                    gensym("delimiter"), A_GIMME, 0);
    class_addmethod(ofeliaListToSymbol_class, reinterpret_cast<t_method>(ofeliaListToSymbol_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaListToSymbol_class, reinterpret_cast<t_method>(ofeliaListToSymbol_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSymbolToList object methods
 */
void *ofeliaSymbolToList_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSymbolToList *x = reinterpret_cast<t_ofeliaSymbolToList*>(pd_new(ofeliaSymbolToList_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getDelimiterFromArgs(argc, argv, x->delimiter, t_ofeliaSymbolToList::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("delimiter"));
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaSymbolToList_symbol(t_ofeliaSymbolToList *x, t_symbol *s)
{
    int ac;
    t_atom *av;
    t_string str = s->s_name;
    const t_string delimiter = x->delimiter;
    
    if (!delimiter.empty()) {
        
        vector<t_string> elems;
        size_t pos = 0;
        t_string elem;
        
        while ((pos = str.find(delimiter)) != t_string::npos) {
            
            elem = str.substr(0, pos);
            
            if (!elem.empty())
                elems.push_back(elem);
            str.erase(0, pos + delimiter.length());
        }
        if (!str.empty())
            elems.push_back(str);
        ac = static_cast<int>(elems.size());
        av = (t_atom *)getbytes(ac * sizeof(t_atom));
        
        for (int i=0; i<ac; ++i) {
            
            if (isFloat(elems[i].c_str())) {
                
                av[i].a_type = A_FLOAT;
                av[i].a_w.w_float = ofToFloat(elems[i].c_str());
            }
            else {
                
                av[i].a_type = A_SYMBOL;
                av[i].a_w.w_symbol = gensym(elems[i].c_str());
            }
        }
    }
    else {
        
        vector<char> elems(str.begin(), str.end());
        ac = static_cast<int>(elems.size());
        av = (t_atom *)getbytes(ac * sizeof(t_atom));
        
        for (int i=0; i<ac; ++i) {
            
            if (isdigit(elems[i])) {
                
                av[i].a_type = A_FLOAT;
                av[i].a_w.w_float = static_cast<t_float>(elems[i]-'0');
            }
            else {
                
                char elem[2];
                elem[0] = static_cast<char>(elems[i]);
                elem[1] = '\0';
                av[i].a_type = A_SYMBOL;
                av[i].a_w.w_symbol = gensym(elem);
            }
        }
    }
    outlet_list(x->x_obj.ob_outlet, &s_list, ac, av);
    freebytes(av, ac * sizeof(t_atom));
}

void ofeliaSymbolToList_set(t_ofeliaSymbolToList *x, t_symbol *s, int argc, t_atom *argv)
{
    getDelimiterFromArgs(argc, argv, x->delimiter, t_ofeliaSymbolToList::objName);
}

void ofeliaSymbolToList_print(t_ofeliaSymbolToList *x)
{
    post("\n[%s]", t_ofeliaSymbolToList::objName);
    post("delimiter : %s", x->delimiter.c_str());
}

void ofeliaSymbolToList_setup()
{
    ofeliaSymbolToList_class = class_new(gensym("ofSymbolToList"),
                                         reinterpret_cast<t_newmethod>(ofeliaSymbolToList_new),
                                         0, sizeof(t_ofeliaSymbolToList),
                                         CLASS_DEFAULT, A_GIMME, 0);
    class_addsymbol(ofeliaSymbolToList_class, reinterpret_cast<t_method>(ofeliaSymbolToList_symbol));
    class_addmethod(ofeliaSymbolToList_class, reinterpret_cast<t_method>(ofeliaSymbolToList_set),
                    gensym("delimiter"), A_GIMME, 0);
    class_addmethod(ofeliaSymbolToList_class, reinterpret_cast<t_method>(ofeliaSymbolToList_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaSymbolToList_class, reinterpret_cast<t_method>(ofeliaSymbolToList_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofCount object methods
 */
bool getCountElemFromArgs(const int argc, const t_atom *argv, t_ofeliaCountElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.from = 0.0f;
            elem.to = numeric_limits<t_float>::max();
            elem.value = elem.from;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                elem.from = 0.0f;
                elem.to = truncf(argv[0].a_w.w_float);
                elem.value = elem.from;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaCount::objName);
                return 0;
            }
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT) {
                
                elem.from = truncf(argv[0].a_w.w_float);
                elem.to = truncf(argv[1].a_w.w_float);
                elem.value = elem.from;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaCount::objName);
                return 0;
            }
            break;
        }
        case 3:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT) {
                
                elem.from = truncf(argv[0].a_w.w_float);
                elem.to = truncf(argv[1].a_w.w_float);
                elem.value = truncf(argv[2].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaCount::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaCount::objName);
            return 0;
        }
    }
    return 1;
}

inline float getModulo(const float a, const float b)
{
    return a >= 0 ? fmod(a, b) : fmod((b - abs(fmod(a, b))), b);
}

void *ofeliaCount_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaCount *x = reinterpret_cast<t_ofeliaCount*>(pd_new(ofeliaCount_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getCountElemFromArgs(argc, argv, x->elem))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("from"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("to"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("value"));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaCount_bang(t_ofeliaCount *x)
{
    const float minValue = min(x->elem.from, x->elem.to);
    const float numCount = abs(x->elem.from - x->elem.to) + 1.0f;
    x->elem.value = getModulo(x->elem.value - minValue, numCount) + minValue;
    outlet_float(x->x_obj.ob_outlet, x->elem.value);
    
    if (x->elem.from < x->elem.to)
        ++x->elem.value;
    else
        --x->elem.value;
}

void ofeliaCount_float(t_ofeliaCount *x, t_floatarg f)
{
    x->elem.value = truncf(f);
    ofeliaCount_bang(x);
}

void ofeliaCount_from(t_ofeliaCount *x, t_floatarg f)
{
    x->elem.from = truncf(f);
}

void ofeliaCount_to(t_ofeliaCount *x, t_floatarg f)
{
    x->elem.to = truncf(f);
}

void ofeliaCount_value(t_ofeliaCount *x, t_floatarg f)
{
    x->elem.value = truncf(f);
}

void ofeliaCount_set(t_ofeliaCount *x, t_symbol *s, int argc, t_atom *argv)
{
    getCountElemFromArgs(argc, argv, x->elem);
}

void ofeliaCount_print(t_ofeliaCount *x)
{
    post("\n[%s]", t_ofeliaCount::objName);
    post("from : %g", x->elem.from);
    post("to : %g", x->elem.to);
    const float minValue = min(x->elem.from, x->elem.to);
    const float numCount = abs(x->elem.from - x->elem.to) + 1;
    post("value : %g", getModulo(x->elem.value - minValue, numCount) + minValue);
}

void ofeliaCount_setup()
{
    ofeliaCount_class = class_new(gensym("ofCount"),
                                  reinterpret_cast<t_newmethod>(ofeliaCount_new),
                                  0, sizeof(t_ofeliaCount),
                                  CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaCount_class, reinterpret_cast<t_method>(ofeliaCount_bang));
    class_addfloat(ofeliaCount_class, reinterpret_cast<t_method>(ofeliaCount_float));
    class_addmethod(ofeliaCount_class, reinterpret_cast<t_method>(ofeliaCount_from),
                    gensym("from"), A_FLOAT, 0);
    class_addmethod(ofeliaCount_class, reinterpret_cast<t_method>(ofeliaCount_to),
                    gensym("to"), A_FLOAT, 0);
    class_addmethod(ofeliaCount_class, reinterpret_cast<t_method>(ofeliaCount_value),
                    gensym("value"), A_FLOAT, 0);
    class_addmethod(ofeliaCount_class, reinterpret_cast<t_method>(ofeliaCount_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaCount_class, reinterpret_cast<t_method>(ofeliaCount_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofCountUntil object methods
 */
bool getCountUntilElemFromArgs(const int argc, const t_atom *argv, t_ofeliaCountUntilElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.from = 0.0f;
            elem.to = numeric_limits<t_float>::max();
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                elem.from = 0.0f;
                elem.to = truncf(argv[0].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaCountUntil::objName);
                return 0;
            }
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT) {
                
                elem.from = truncf(argv[0].a_w.w_float);
                elem.to = truncf(argv[1].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaCountUntil::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaCountUntil::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaCountUntil_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaCountUntil *x = reinterpret_cast<t_ofeliaCountUntil*>(pd_new(ofeliaCountUntil_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getCountUntilElemFromArgs(argc, argv, x->elem))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("from"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("to"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_bang, gensym("break"));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaCountUntil_bang(t_ofeliaCountUntil *x)
{
    x->shouldBreak = false;
    
    if (x->elem.from <= x->elem.to) {
        
        for (t_float c=x->elem.from; c<=x->elem.to && !x->shouldBreak; ++c)
            outlet_float(x->x_obj.ob_outlet, c);
    }
    else {
        
        for (t_float c=x->elem.from; c>=x->elem.to && !x->shouldBreak; --c)
            outlet_float(x->x_obj.ob_outlet, c);
    }
}

void ofeliaCountUntil_float(t_ofeliaCountUntil *x, t_floatarg f)
{
    const float numCounts = truncf(f);
    
    if (numCounts) {
        
        x->shouldBreak = false;
        
        if (numCounts > 0.0f) {
            
            for (t_float c=0.0f; c<numCounts && !x->shouldBreak; ++c)
                outlet_float(x->x_obj.ob_outlet, c);
        }
        else {
            
            for (t_float c=abs(numCounts)-1; c>=0.0f && !x->shouldBreak; --c)
                outlet_float(x->x_obj.ob_outlet, c);
        }
    }
}

void ofeliaCountUntil_from(t_ofeliaCountUntil *x, t_floatarg f)
{
    x->elem.from = truncf(f);
}

void ofeliaCountUntil_to(t_ofeliaCountUntil *x, t_floatarg f)
{
    x->elem.to = truncf(f);
}

void ofeliaCountUntil_break(t_ofeliaCountUntil *x)
{
    x->shouldBreak = true;
}

void ofeliaCountUntil_set(t_ofeliaCountUntil *x, t_symbol *s, int argc, t_atom *argv)
{
    getCountUntilElemFromArgs(argc, argv, x->elem);
}

void ofeliaCountUntil_print(t_ofeliaCountUntil *x)
{
    post("\n[%s]", t_ofeliaCountUntil::objName);
    post("from : %g", x->elem.from);
    post("to : %g", x->elem.to);
}

void ofeliaCountUntil_setup()
{
    ofeliaCountUntil_class = class_new(gensym("ofCountUntil"),
                                       reinterpret_cast<t_newmethod>(ofeliaCountUntil_new),
                                       0, sizeof(t_ofeliaCountUntil),
                                       CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaCountUntil_class, reinterpret_cast<t_method>(ofeliaCountUntil_bang));
    class_addfloat(ofeliaCountUntil_class, reinterpret_cast<t_method>(ofeliaCountUntil_float));
    class_addmethod(ofeliaCountUntil_class, reinterpret_cast<t_method>(ofeliaCountUntil_from),
                    gensym("from"), A_FLOAT, 0);
    class_addmethod(ofeliaCountUntil_class, reinterpret_cast<t_method>(ofeliaCountUntil_to),
                    gensym("to"), A_FLOAT, 0);
    class_addmethod(ofeliaCountUntil_class, reinterpret_cast<t_method>(ofeliaCountUntil_break),
                    gensym("break"), A_NULL, 0);
    class_addmethod(ofeliaCountUntil_class, reinterpret_cast<t_method>(ofeliaCountUntil_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaCountUntil_class, reinterpret_cast<t_method>(ofeliaCountUntil_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofStep object methods
 */
bool getStepElemFromArgs(const int argc, const t_atom *argv, t_ofeliaStepElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.from = truncf(numeric_limits<t_float>::lowest());
            elem.to = numeric_limits<t_float>::max();
            elem.value = 0.0f;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                elem.from = truncf(numeric_limits<t_float>::lowest());
                elem.to = numeric_limits<t_float>::max();
                elem.value = truncf(argv[0].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaStep::objName);
                return 0;
            }
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT) {
                
                elem.from = truncf(argv[0].a_w.w_float);
                elem.to = truncf(argv[1].a_w.w_float);
                elem.value = elem.from;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaStep::objName);
                return 0;
            }
            break;
        }
        case 3:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT) {
                
                elem.from = truncf(argv[0].a_w.w_float);
                elem.to = truncf(argv[1].a_w.w_float);
                elem.value = ofClamp(truncf(argv[2].a_w.w_float),
                                     min(elem.from, elem.to),
                                     max(elem.from, elem.to));
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaStep::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaStep::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaStep_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaStep *x = reinterpret_cast<t_ofeliaStep*>(pd_new(ofeliaStep_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getStepElemFromArgs(argc, argv, x->elem))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_bang, gensym("inc"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("value"));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaStep_bang(t_ofeliaStep *x)
{
    if (x->elem.from < x->elem.to)
        --x->elem.value;
    else
        ++x->elem.value;
    x->elem.value = ofClamp(x->elem.value,
                            min(x->elem.from, x->elem.to),
                            max(x->elem.from, x->elem.to));
    outlet_float(x->x_obj.ob_outlet, x->elem.value);
}

void ofeliaStep_inc(t_ofeliaStep *x)
{
    if (x->elem.from < x->elem.to)
        ++x->elem.value;
    else
        --x->elem.value;
    x->elem.value = ofClamp(x->elem.value,
                            min(x->elem.from, x->elem.to),
                            max(x->elem.from, x->elem.to));
    outlet_float(x->x_obj.ob_outlet, x->elem.value);
}

void ofeliaStep_from(t_ofeliaStep *x, t_floatarg f)
{
    x->elem.from = truncf(f);
}

void ofeliaStep_to(t_ofeliaStep *x, t_floatarg f)
{
    x->elem.to = truncf(f);
}

void ofeliaStep_value(t_ofeliaStep *x, t_floatarg f)
{
    x->elem.value = ofClamp(f, min(x->elem.from, x->elem.to), max(x->elem.from, x->elem.to));
}

void ofeliaStep_set(t_ofeliaStep *x, t_symbol *s, int argc, t_atom *argv)
{
    getStepElemFromArgs(argc, argv, x->elem);
}

void ofeliaStep_print(t_ofeliaStep *x)
{
    post("\n[%s]", t_ofeliaStep::objName);
    post("from : %g", x->elem.from);
    post("to : %g", x->elem.to);
    post("value : %g", x->elem.value);
}

void ofeliaStep_setup()
{
    ofeliaStep_class = class_new(gensym("ofStep"),
                                 reinterpret_cast<t_newmethod>(ofeliaStep_new),
                                 0, sizeof(t_ofeliaStep),
                                 CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaStep_class, reinterpret_cast<t_method>(ofeliaStep_bang));
    class_addmethod(ofeliaStep_class, reinterpret_cast<t_method>(ofeliaStep_inc),
                    gensym("inc"), A_NULL, 0);
    class_addmethod(ofeliaStep_class, reinterpret_cast<t_method>(ofeliaStep_from),
                    gensym("from"), A_FLOAT, 0);
    class_addmethod(ofeliaStep_class, reinterpret_cast<t_method>(ofeliaStep_to),
                    gensym("to"), A_FLOAT, 0);
    class_addmethod(ofeliaStep_class, reinterpret_cast<t_method>(ofeliaStep_value),
                    gensym("value"), A_FLOAT, 0);
    class_addmethod(ofeliaStep_class, reinterpret_cast<t_method>(ofeliaStep_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaStep_class, reinterpret_cast<t_method>(ofeliaStep_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSwitch object methods
 */
void *ofeliaSwitch_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSwitch *x = reinterpret_cast<t_ofeliaSwitch*>(pd_new(ofeliaSwitch_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    int numPorts;
    
    switch (argc) {
            
        case 0:
        {
            numPorts = 1;
            x->selectedPort = 0;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                numPorts = static_cast<int>(argv[0].a_w.w_float);
                x->selectedPort = 0;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSwitch::objName);
                return 0;
            }
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT) {
                
                numPorts = static_cast<int>(argv[0].a_w.w_float);
                x->selectedPort = static_cast<int>(argv[1].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaSwitch::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaSwitch::objName);
            return 0;
        }
    }
    if (numPorts < 1 || numPorts > 64) {
        
        error("%s: invalid number of ports", t_ofeliaSwitch::objName);
        return 0;
    }
    for (int i=0; i<numPorts; ++i) {
        
        stringstream ss;
        ss << "{l}" << i+1;
        inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym(ss.str().c_str()));
    }
    outlet_new(&x->x_obj, &s_anything);
    return (x);
}

void ofeliaSwitch_float(t_ofeliaSwitch *x, t_floatarg f)
{
    x->selectedPort = static_cast<int>(f);
}

void ofeliaSwitch_l1(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 1)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l2(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 2)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l3(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 3)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l4(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 4)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l5(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 5)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l6(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 6)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l7(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 7)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l8(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 8)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l9(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 9)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l10(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 10)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l11(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 11)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l12(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 12)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l13(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 13)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l14(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 14)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l15(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 15)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l16(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 16)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l17(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 17)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l18(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 18)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l19(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 19)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l20(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 20)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l21(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 21)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l22(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 22)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l23(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 23)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l24(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 24)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l25(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 25)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l26(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 26)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l27(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 27)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l28(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 28)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l29(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 29)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l30(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 30)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l31(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 31)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l32(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 32)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l33(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 33)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l34(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 34)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l35(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 35)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l36(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 36)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l37(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 37)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l38(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 38)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l39(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 39)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l40(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 40)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l41(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 41)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l42(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 42)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l43(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 43)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l44(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 44)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l45(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 45)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l46(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 46)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l47(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 47)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l48(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 48)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l49(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 49)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l50(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 50)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l51(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 51)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l52(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 52)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l53(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 53)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l54(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 54)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l55(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 55)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l56(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 56)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l57(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 57)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l58(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 58)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l59(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 59)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l60(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 60)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l61(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 61)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l62(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 62)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l63(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 63)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_l64(t_ofeliaSwitch *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort == 64)
        outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
}

void ofeliaSwitch_setup()
{
    ofeliaSwitch_class = class_new(gensym("ofSwitch"),
                                   reinterpret_cast<t_newmethod>(ofeliaSwitch_new),
                                   0, sizeof(t_ofeliaSwitch),
                                   CLASS_DEFAULT, A_GIMME, 0);
    class_addfloat(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_float));
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l1),
                    gensym("{l}1"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l2),
                    gensym("{l}2"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l3),
                    gensym("{l}3"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l4),
                    gensym("{l}4"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l5),
                    gensym("{l}5"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l6),
                    gensym("{l}6"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l7),
                    gensym("{l}7"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l8),
                    gensym("{l}8"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l9),
                    gensym("{l}9"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l10),
                    gensym("{l}10"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l11),
                    gensym("{l}11"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l12),
                    gensym("{l}12"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l13),
                    gensym("{l}13"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l14),
                    gensym("{l}14"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l15),
                    gensym("{l}15"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l16),
                    gensym("{l}16"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l17),
                    gensym("{l}17"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l18),
                    gensym("{l}18"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l19),
                    gensym("{l}19"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l20),
                    gensym("{l}20"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l21),
                    gensym("{l}21"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l22),
                    gensym("{l}22"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l23),
                    gensym("{l}23"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l24),
                    gensym("{l}24"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l25),
                    gensym("{l}25"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l26),
                    gensym("{l}26"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l27),
                    gensym("{l}27"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l28),
                    gensym("{l}28"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l29),
                    gensym("{l}29"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l30),
                    gensym("{l}30"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l31),
                    gensym("{l}31"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l32),
                    gensym("{l}32"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l33),
                    gensym("{l}33"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l34),
                    gensym("{l}34"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l35),
                    gensym("{l}35"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l36),
                    gensym("{l}36"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l37),
                    gensym("{l}37"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l38),
                    gensym("{l}38"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l39),
                    gensym("{l}39"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l40),
                    gensym("{l}40"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l41),
                    gensym("{l}41"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l42),
                    gensym("{l}42"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l43),
                    gensym("{l}43"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l44),
                    gensym("{l}44"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l45),
                    gensym("{l}45"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l46),
                    gensym("{l}46"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l47),
                    gensym("{l}47"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l48),
                    gensym("{l}48"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l49),
                    gensym("{l}49"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l50),
                    gensym("{l}50"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l51),
                    gensym("{l}51"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l52),
                    gensym("{l}52"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l53),
                    gensym("{l}53"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l54),
                    gensym("{l}54"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l55),
                    gensym("{l}55"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l56),
                    gensym("{l}56"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l57),
                    gensym("{l}57"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l58),
                    gensym("{l}58"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l59),
                    gensym("{l}59"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l60),
                    gensym("{l}60"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l61),
                    gensym("{l}61"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l62),
                    gensym("{l}62"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l63),
                    gensym("{l}63"), A_GIMME, 0);
    class_addmethod(ofeliaSwitch_class, reinterpret_cast<t_method>(ofeliaSwitch_l64),
                    gensym("{l}64"), A_GIMME, 0);
}

/* ________________________________________________________________________________
 * ofGate object methods
 */
void *ofeliaGate_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaGate *x = reinterpret_cast<t_ofeliaGate*>(pd_new(ofeliaGate_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    int numPorts;
    
    switch (argc) {
            
        case 0:
        {
            numPorts = 1;
            x->selectedPort = 0;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                numPorts = static_cast<int>(argv[0].a_w.w_float);
                x->selectedPort = 0;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaGate::objName);
                return 0;
            }
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT) {
                
                numPorts = static_cast<int>(argv[0].a_w.w_float);
                x->selectedPort = static_cast<int>(argv[1].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaGate::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaGate::objName);
            return 0;
        }
    }
    if (numPorts < 1 || numPorts > 64) {
        
        error("%s: invalid number of ports", t_ofeliaGate::objName);
        return 0;
    }
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("value"));
    x->gate_outs.resize(numPorts);
    
    for (int i=0; i<numPorts; ++i)
        x->gate_outs[i] = outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGate_float(t_ofeliaGate *x, t_floatarg f)
{
    x->selectedPort = static_cast<int>(f);
}

void ofeliaGate_value(t_ofeliaGate *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->selectedPort > 0 && x->selectedPort <= x->gate_outs.size())
        outlet_list(x->gate_outs[x->selectedPort-1], &s_list, argc, argv);
}

void ofeliaGate_free(t_ofeliaGate *x)
{
    for (size_t i=0; i<x->gate_outs.size(); ++i)
        outlet_free(x->gate_outs[i]);
}

void ofeliaGate_setup()
{
    ofeliaGate_class = class_new(gensym("ofGate"),
                                 reinterpret_cast<t_newmethod>(ofeliaGate_new),
                                 reinterpret_cast<t_method>(ofeliaGate_free),
                                 sizeof(t_ofeliaGate),
                                 CLASS_DEFAULT, A_GIMME, 0);
    class_addfloat(ofeliaGate_class, reinterpret_cast<t_method>(ofeliaGate_float));
    class_addmethod(ofeliaGate_class, reinterpret_cast<t_method>(ofeliaGate_value),
                    gensym("value"), A_GIMME, 0);
}

/* ________________________________________________________________________________
 * ofValue object methods
 */
void *ofeliaValue_new(t_symbol *s)
{
    t_ofeliaValue *x = reinterpret_cast<t_ofeliaValue*>(pd_new(ofeliaValue_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    x->value = value_get(gensym(x->varName.name.c_str()));
    
    if (!*s->s_name)
        inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_symbol, gensym("{name}"));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaValue_bang(t_ofeliaValue *x)
{
    outlet_float(x->x_obj.ob_outlet, *x->value);
}

void ofeliaValue_float(t_ofeliaValue *x, t_floatarg f)
{
    *x->value = f;
}

void ofeliaValue_set(t_ofeliaValue *x, t_symbol *s)
{
    value_release(gensym(x->varName.name.c_str()));
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    x->value = value_get(gensym(x->varName.name.c_str()));
}

void ofeliaValue_free(t_ofeliaValue *x)
{
    value_release(gensym(x->varName.name.c_str()));
}

void ofeliaValue_setup()
{
    ofeliaValue_class = class_new(gensym("ofValue"),
                                  reinterpret_cast<t_newmethod>(ofeliaValue_new),
                                  reinterpret_cast<t_method>(ofeliaValue_free),
                                  sizeof(t_ofeliaValue),
                                  CLASS_DEFAULT, A_DEFSYM, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaValue_new),
                     gensym("ofVal"), A_DEFSYM, 0);
    class_addbang(ofeliaValue_class, reinterpret_cast<t_method>(ofeliaValue_bang));
    class_addfloat(ofeliaValue_class, reinterpret_cast<t_method>(ofeliaValue_float));
    class_addmethod(ofeliaValue_class, reinterpret_cast<t_method>(ofeliaValue_set),
                    gensym("{name}"), A_SYMBOL, 0);
}

/* ________________________________________________________________________________
 * ofSend object methods
 */
void *ofeliaSend_new(t_symbol *s)
{
    t_ofeliaSend *x = reinterpret_cast<t_ofeliaSend*>(pd_new(ofeliaSend_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    x->sym = gensym(x->varName.name.c_str());

    if (!*s->s_name)
        inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_symbol, gensym("{name}"));
    return (x);
}

void ofeliaSend_bang(t_ofeliaSend *x)
{
    if (x->sym->s_thing) pd_bang(x->sym->s_thing);
}

void ofeliaSend_float(t_ofeliaSend *x, t_float f)
{
    if (x->sym->s_thing) pd_float(x->sym->s_thing, f);
}

void ofeliaSend_symbol(t_ofeliaSend *x, t_symbol *s)
{
    if (x->sym->s_thing) pd_symbol(x->sym->s_thing, s);
}

void ofeliaSend_pointer(t_ofeliaSend *x, t_gpointer *gp)
{
    if (x->sym->s_thing) pd_pointer(x->sym->s_thing, gp);
}

void ofeliaSend_list(t_ofeliaSend *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->sym->s_thing) pd_list(x->sym->s_thing, s, argc, argv);
}

void ofeliaSend_anything(t_ofeliaSend *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->sym->s_thing) typedmess(x->sym->s_thing, s, argc, argv);
}

void ofeliaSend_set(t_ofeliaSend *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    x->sym = gensym(x->varName.name.c_str());
}

void ofeliaSend_setup()
{
    ofeliaSend_class = class_new(gensym("ofSend"),
                                 reinterpret_cast<t_newmethod>(ofeliaSend_new),
                                 0, sizeof(t_ofeliaSend),
                                 CLASS_DEFAULT, A_DEFSYM, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaSend_new),
                     gensym("ofSnd"), A_DEFSYM, 0);
    class_addbang(ofeliaSend_class, reinterpret_cast<t_method>(ofeliaSend_bang));
    class_addfloat(ofeliaSend_class, reinterpret_cast<t_method>(ofeliaSend_float));
    class_addsymbol(ofeliaSend_class, reinterpret_cast<t_method>(ofeliaSend_symbol));
    class_addpointer(ofeliaSend_class, reinterpret_cast<t_method>(ofeliaSend_pointer));
    class_addlist(ofeliaSend_class, reinterpret_cast<t_method>(ofeliaSend_list));
    class_addanything(ofeliaSend_class, reinterpret_cast<t_method>(ofeliaSend_anything));
    class_addmethod(ofeliaSend_class, reinterpret_cast<t_method>(ofeliaSend_set),
                    gensym("{name}"), A_SYMBOL, 0);
}

/* ________________________________________________________________________________
 * ofReceive object methods
 */
void *ofeliaReceive_new(t_symbol *s)
{
    t_ofeliaReceive *x = reinterpret_cast<t_ofeliaReceive*>(pd_new(ofeliaReceive_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    pd_bind(&x->x_obj.ob_pd, gensym(x->varName.name.c_str()));
    
    if (!*s->s_name)
        inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_symbol, gensym("{name}"));
    outlet_new(&x->x_obj, 0);
    return (x);
}

void ofeliaReceive_name(t_ofeliaReceive *x, t_symbol *s)
{
    pd_unbind(&x->x_obj.ob_pd, gensym(x->varName.name.c_str()));
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    pd_bind(&x->x_obj.ob_pd, gensym(x->varName.name.c_str()));
}

void ofeliaReceive_bang(t_ofeliaReceive *x)
{
    outlet_bang(x->x_obj.ob_outlet);
}

void ofeliaReceive_float(t_ofeliaReceive *x, t_float f)
{
    outlet_float(x->x_obj.ob_outlet, f);
}

void ofeliaReceive_symbol(t_ofeliaReceive *x, t_symbol *s)
{
    outlet_symbol(x->x_obj.ob_outlet, s);
}

void ofeliaReceive_pointer(t_ofeliaReceive *x, t_gpointer *gp)
{
    outlet_pointer(x->x_obj.ob_outlet, gp);
}

void ofeliaReceive_list(t_ofeliaReceive *x, t_symbol *s, int argc, t_atom *argv)
{
    outlet_list(x->x_obj.ob_outlet, s, argc, argv);
}

void ofeliaReceive_anything(t_ofeliaReceive *x, t_symbol *s, int argc, t_atom *argv)
{
    outlet_anything(x->x_obj.ob_outlet, s, argc, argv);
}

void ofeliaReceive_free(t_ofeliaReceive *x)
{
    pd_unbind(&x->x_obj.ob_pd, gensym(x->varName.name.c_str()));
}

void ofeliaReceive_setup()
{
    ofeliaReceive_class = class_new(gensym("ofReceive"),
                                    reinterpret_cast<t_newmethod>(ofeliaReceive_new),
                                    reinterpret_cast<t_method>(ofeliaReceive_free),
                                    sizeof(t_ofeliaReceive),
                                    CLASS_NOINLET, A_DEFSYM, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaReceive_new),
                     gensym("ofRcv"), A_DEFSYM, 0);
    class_addbang(ofeliaReceive_class, reinterpret_cast<t_method>(ofeliaReceive_bang));
    class_addfloat(ofeliaReceive_class, reinterpret_cast<t_method>(ofeliaReceive_float));
    class_addsymbol(ofeliaReceive_class, reinterpret_cast<t_method>(ofeliaReceive_symbol));
    class_addpointer(ofeliaReceive_class, reinterpret_cast<t_method>(ofeliaReceive_pointer));
    class_addlist(ofeliaReceive_class, reinterpret_cast<t_method>(ofeliaReceive_list));
    class_addanything(ofeliaReceive_class, reinterpret_cast<t_method>(ofeliaReceive_anything));
    class_addmethod(ofeliaReceive_class, reinterpret_cast<t_method>(ofeliaReceive_name),
                    gensym("{name}"), A_SYMBOL, 0);
}

/* ________________________________________________________________________________
 * ofGetPlatform object methods
 */
void *ofeliaGetPlatform_new()
{
    t_ofeliaGetPlatform *x = reinterpret_cast<t_ofeliaGetPlatform*>(pd_new(ofeliaGetPlatform_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetPlatform_bang(t_ofeliaGetPlatform *x)
{
    switch (ofGetTargetPlatform()) {
            
        case OF_TARGET_OSX:
            outlet_float(x->x_obj.ob_outlet, 0.0f);
            break;
        case OF_TARGET_MINGW:
            outlet_float(x->x_obj.ob_outlet, 1.0f);
            break;
        case OF_TARGET_WINVS:
            outlet_float(x->x_obj.ob_outlet, 2.0f);
            break;
        case OF_TARGET_IOS:
            outlet_float(x->x_obj.ob_outlet, 3.0f);
            break;
        case OF_TARGET_ANDROID:
            outlet_float(x->x_obj.ob_outlet, 4.0f);
            break;
        case OF_TARGET_LINUX:
            outlet_float(x->x_obj.ob_outlet, 5.0f);
            break;
        case OF_TARGET_LINUX64:
            outlet_float(x->x_obj.ob_outlet, 6.0f);
            break;
        case OF_TARGET_LINUXARMV6L:
            outlet_float(x->x_obj.ob_outlet, 7.0f);
            break;
        case OF_TARGET_LINUXARMV7L:
            outlet_float(x->x_obj.ob_outlet, 8.0f);
            break;
        case OF_TARGET_EMSCRIPTEN:
            outlet_float(x->x_obj.ob_outlet, 9.0f);
            break;
        default:
            break;
    }
}

void ofeliaGetPlatform_setup()
{
    ofeliaGetPlatform_class = class_new(gensym("ofGetPlatform"),
                                        reinterpret_cast<t_newmethod>(ofeliaGetPlatform_new),
                                        0, sizeof(t_ofeliaGetPlatform),
                                        CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetPlatform_class, reinterpret_cast<t_method>(ofeliaGetPlatform_bang));
}

/* ________________________________________________________________________________
 * ofGetDate object methods
 */
void *ofeliaGetDate_new()
{
    t_ofeliaGetDate *x = reinterpret_cast<t_ofeliaGetDate*>(pd_new(ofeliaGetDate_class));
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetDate_bang(t_ofeliaGetDate *x)
{
    t_atom av[3];
    av[0].a_type = A_FLOAT;
    av[0].a_w.w_float = static_cast<t_float>(ofGetDay());
    av[1].a_type = A_FLOAT;
    av[1].a_w.w_float = static_cast<t_float>(ofGetMonth());
    av[2].a_type = A_FLOAT;
    av[2].a_w.w_float = static_cast<t_float>(ofGetYear());
    outlet_list(x->x_obj.ob_outlet, &s_list, 3, av);
}

void ofeliaGetDate_setup()
{
    ofeliaGetDate_class = class_new(gensym("ofGetDate"),
                                    reinterpret_cast<t_newmethod>(ofeliaGetDate_new),
                                    0, sizeof(t_ofeliaGetDate),
                                    CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetDate_class, reinterpret_cast<t_method>(ofeliaGetDate_bang));
}

/* ________________________________________________________________________________
 * ofGetTime object methods
 */
void *ofeliaGetTime_new()
{
    t_ofeliaGetTime *x = reinterpret_cast<t_ofeliaGetTime*>(pd_new(ofeliaGetTime_class));
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetTime_bang(t_ofeliaGetTime *x)
{
    t_atom av[3];
    av[0].a_type = A_FLOAT;
    av[0].a_w.w_float = static_cast<t_float>(ofGetSeconds());
    av[1].a_type = A_FLOAT;
    av[1].a_w.w_float = static_cast<t_float>(ofGetMinutes());
    av[2].a_type = A_FLOAT;
    av[2].a_w.w_float = static_cast<t_float>(ofGetHours());
    outlet_list(x->x_obj.ob_outlet, &s_list, 3, av);
}

void ofeliaGetTime_setup()
{
    ofeliaGetTime_class = class_new(gensym("ofGetTime"),
                                    reinterpret_cast<t_newmethod>(ofeliaGetTime_new),
                                    0, sizeof(t_ofeliaGetTime),
                                    CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetTime_class, reinterpret_cast<t_method>(ofeliaGetTime_bang));
}

/* ________________________________________________________________________________
 * ofGetPatchDirectory object methods
 */
void *ofeliaGetPatchDirectory_new(t_floatarg f)
{
    t_ofeliaGetPatchDirectory *x = reinterpret_cast<t_ofeliaGetPatchDirectory*>(pd_new(ofeliaGetPatchDirectory_class));
    t_canvas *canvas = canvas_getcurrent();
    const int level = truncf(f);
    
    for (int i=0; i<level; ++i) {
        
        if (canvas->gl_owner)
            canvas = canvas->gl_owner;
        else
            break;
    }
    x->patchDir = canvas_getdir(canvas);
    outlet_new(&x->x_obj, &s_symbol);
    return (x);
}

void ofeliaGetPatchDirectory_bang(t_ofeliaGetPatchDirectory *x)
{
    outlet_symbol(x->x_obj.ob_outlet, x->patchDir);
}

void ofeliaGetPatchDirectory_setup()
{
    ofeliaGetPatchDirectory_class = class_new(gensym("ofGetPatchDirectory"),
                                              reinterpret_cast<t_newmethod>(ofeliaGetPatchDirectory_new),
                                              0, sizeof(t_ofeliaGetPatchDirectory),
                                              CLASS_DEFAULT, A_DEFFLOAT, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaGetPatchDirectory_new),
                     gensym("ofGetPatchDir"), A_DEFFLOAT, 0);
    class_addbang(ofeliaGetPatchDirectory_class, reinterpret_cast<t_method>(ofeliaGetPatchDirectory_bang));
}

/* ________________________________________________________________________________
 * ofGetHomeDirectory object methods
 */
void *ofeliaGetHomeDirectory_new()
{
    t_ofeliaGetHomeDirectory *x = reinterpret_cast<t_ofeliaGetHomeDirectory*>(pd_new(ofeliaGetHomeDirectory_class));
    outlet_new(&x->x_obj, &s_symbol);
    return (x);
}

void ofeliaGetHomeDirectory_bang(t_ofeliaGetHomeDirectory *x)
{
#if defined(TARGET_LINUX) || defined(TARGET_OSX) || defined(TARGET_WIN32)
    outlet_symbol(x->x_obj.ob_outlet, gensym(ofFilePath::getUserHomeDir().c_str()));
#else
    error("%s: not supported on this platform", t_ofeliaGetHomeDirectory::objName);
#endif
}

void ofeliaGetHomeDirectory_setup()
{
    ofeliaGetHomeDirectory_class = class_new(gensym("ofGetHomeDirectory"),
                                             reinterpret_cast<t_newmethod>(ofeliaGetHomeDirectory_new),
                                             0, sizeof(t_ofeliaGetHomeDirectory),
                                             CLASS_DEFAULT, A_NULL, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaGetHomeDirectory_new),
                     gensym("ofGetHomeDir"), A_NULL, 0);
    class_addbang(ofeliaGetHomeDirectory_class, reinterpret_cast<t_method>(ofeliaGetHomeDirectory_bang));
}

/* ________________________________________________________________________________
 * ofGetDocumentsDirectory object methods
 */
void *ofeliaGetDocumentsDirectory_new()
{
    t_ofeliaGetDocumentsDirectory *x = reinterpret_cast<t_ofeliaGetDocumentsDirectory*>(pd_new(ofeliaGetDocumentsDirectory_class));
    outlet_new(&x->x_obj, &s_symbol);
    return (x);
}

void ofeliaGetDocumentsDirectory_bang(t_ofeliaGetDocumentsDirectory *x)
{
#if defined(TARGET_OF_IOS)
    outlet_symbol(x->x_obj.ob_outlet, gensym(ofxiOSGetDocumentsDirectory().c_str()));
#else
    error("%s: not supported on this platform", t_ofeliaGetDocumentsDirectory::objName);
#endif
}

void ofeliaGetDocumentsDirectory_setup()
{
    ofeliaGetDocumentsDirectory_class = class_new(gensym("ofGetDocumentsDirectory"),
                                                  reinterpret_cast<t_newmethod>(ofeliaGetDocumentsDirectory_new),
                                                  0, sizeof(t_ofeliaGetDocumentsDirectory),
                                                  CLASS_DEFAULT, A_NULL, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaGetDocumentsDirectory_new),
                     gensym("ofGetDocumentsDir"), A_NULL, 0);
    class_addbang(ofeliaGetDocumentsDirectory_class, reinterpret_cast<t_method>(ofeliaGetDocumentsDirectory_bang));
}

/* ________________________________________________________________________________
 * ofGetLibraryDirectory object methods
 */
void *ofeliaGetLibraryDirectory_new()
{
    t_ofeliaGetLibraryDirectory *x = reinterpret_cast<t_ofeliaGetLibraryDirectory*>(pd_new(ofeliaGetLibraryDirectory_class));
    outlet_new(&x->x_obj, &s_symbol);
    return (x);
}

void ofeliaGetLibraryDirectory_bang(t_ofeliaGetLibraryDirectory *x)
{
#if defined(TARGET_OF_IOS)
    NSString* libPath = [NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES) objectAtIndex:0];
    t_string path([libPath UTF8String]);
    outlet_symbol(x->x_obj.ob_outlet, gensym(path.c_str()));
#else
    error("%s: not supported on this platform", t_ofeliaGetLibraryDirectory::objName);
#endif
}

void ofeliaGetLibraryDirectory_setup()
{
    ofeliaGetLibraryDirectory_class = class_new(gensym("ofGetLibraryDirectory"),
                                                reinterpret_cast<t_newmethod>(ofeliaGetLibraryDirectory_new),
                                                0, sizeof(t_ofeliaGetLibraryDirectory),
                                                CLASS_DEFAULT, A_NULL, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaGetLibraryDirectory_new),
                     gensym("ofGetLibraryDir"), A_NULL, 0);
    class_addbang(ofeliaGetLibraryDirectory_class, reinterpret_cast<t_method>(ofeliaGetLibraryDirectory_bang));
}

/* ________________________________________________________________________________
 * ofGetTemporaryDirectory object methods
 */
void *ofeliaGetTemporaryDirectory_new()
{
    t_ofeliaGetTemporaryDirectory *x = reinterpret_cast<t_ofeliaGetTemporaryDirectory*>(pd_new(ofeliaGetTemporaryDirectory_class));
    outlet_new(&x->x_obj, &s_symbol);
    return (x);
}

void ofeliaGetTemporaryDirectory_bang(t_ofeliaGetTemporaryDirectory *x)
{
#if defined(TARGET_OF_IOS)
    NSString *tempPath = NSTemporaryDirectory();
    t_string path([tempPath UTF8String]);
    outlet_symbol(x->x_obj.ob_outlet, gensym(path.c_str()));
#else
    error("%s: not supported on this platform", t_ofeliaGetTemporaryDirectory::objName);
#endif
}

void ofeliaGetTemporaryDirectory_setup()
{
    ofeliaGetTemporaryDirectory_class = class_new(gensym("ofGetTemporaryDirectory"),
                                                  reinterpret_cast<t_newmethod>(ofeliaGetTemporaryDirectory_new),
                                                  0, sizeof(t_ofeliaGetTemporaryDirectory),
                                                  CLASS_DEFAULT, A_NULL, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaGetTemporaryDirectory_new),
                     gensym("ofGetTemporaryDir"), A_NULL, 0);
    class_addbang(ofeliaGetTemporaryDirectory_class, reinterpret_cast<t_method>(ofeliaGetTemporaryDirectory_bang));
}

/* ________________________________________________________________________________
 * ofDoesDirectoryExist object methods
 */
void *ofeliaDoesDirectoryExist_new()
{
    t_ofeliaDoesDirectoryExist *x = reinterpret_cast<t_ofeliaDoesDirectoryExist*>(pd_new(ofeliaDoesDirectoryExist_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaDoesDirectoryExist_symbol(t_ofeliaDoesDirectoryExist *x, t_symbol *s)
{
    if (ofDirectory::doesDirectoryExist(s->s_name, false))
        outlet_float(x->x_obj.ob_outlet, 1.0f);
    else
        outlet_float(x->x_obj.ob_outlet, 0.0f);
}

void ofeliaDoesDirectoryExist_setup()
{
    ofeliaDoesDirectoryExist_class = class_new(gensym("ofDoesDirectoryExist"),
                                              reinterpret_cast<t_newmethod>(ofeliaDoesDirectoryExist_new),
                                              0, sizeof(t_ofeliaDoesDirectoryExist),
                                              CLASS_DEFAULT, A_NULL, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaDoesDirectoryExist_new),
                     gensym("ofDoesDirExist"), A_NULL, 0);
    class_addsymbol(ofeliaDoesDirectoryExist_class, reinterpret_cast<t_method>(ofeliaDoesDirectoryExist_symbol));
}

/* ________________________________________________________________________________
 * ofDoesFileExist object methods
 */
void *ofeliaDoesFileExist_new()
{
    t_ofeliaDoesFileExist *x = reinterpret_cast<t_ofeliaDoesFileExist*>(pd_new(ofeliaDoesFileExist_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaDoesFileExist_symbol(t_ofeliaDoesFileExist *x, t_symbol *s)
{
    if (ofFile::doesFileExist(s->s_name, false))
        outlet_float(x->x_obj.ob_outlet, 1.0f);
    else
        outlet_float(x->x_obj.ob_outlet, 0.0f);
}

void ofeliaDoesFileExist_setup()
{
    ofeliaDoesFileExist_class = class_new(gensym("ofDoesFileExist"),
                                         reinterpret_cast<t_newmethod>(ofeliaDoesFileExist_new),
                                         0, sizeof(t_ofeliaDoesFileExist),
                                         CLASS_DEFAULT, A_NULL, 0);
    class_addsymbol(ofeliaDoesFileExist_class, reinterpret_cast<t_method>(ofeliaDoesFileExist_symbol));
}

/* ________________________________________________________________________________
 * common methods
 */
bool getAllowedExtsFromArgs(const int argc, const t_atom *argv,
                            vector<t_string> &allowedExts, const char *objName)
{
    for (int i=0; i<argc; ++i) {
        
        if (argv[i].a_type == A_SYMBOL) {
            
            allowedExts.push_back(argv[i].a_w.w_symbol->s_name);
        }
        else {
            
            error("%s: wrong argument type", objName);
            return 0;
        }
    }
    return 1;
}

/* ________________________________________________________________________________
 * ofGetDirectoryFileNames object methods
 */
void *ofeliaGetDirectoryFileNames_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaGetDirectoryFileNames *x = reinterpret_cast<t_ofeliaGetDirectoryFileNames*>(pd_new(ofeliaGetDirectoryFileNames_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getAllowedExtsFromArgs(argc, argv, x->allowedExts, t_ofeliaGetDirectoryFileNames::objName))
        return 0;
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetDirectoryFileNames_symbol(t_ofeliaGetDirectoryFileNames *x, t_symbol *s)
{
    t_string path = s->s_name;
    ofDirectory dir(path.c_str());
    
    if (dir.exists()) {
        
        for (size_t i=0; i<x->allowedExts.size(); ++i)
            dir.allowExt(x->allowedExts[i].c_str());
        dir.listDir();
        const int ac = static_cast<int>(dir.size());
        t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
        
        for (int i=0; i<ac; ++i) {
            
            av[i].a_type = A_SYMBOL;
            av[i].a_w.w_symbol = gensym(dir.getName(i).c_str());
        }
        outlet_list(x->x_obj.ob_outlet, &s_list, ac, av);
        freebytes(av, ac * sizeof(t_atom));
    }
    else {
        
        error("%s: the directory '%s' does not exist", t_ofeliaGetDirectoryFileNames::objName, path.c_str());
    }
}

void ofeliaGetDirectoryFileNames_set(t_ofeliaGetDirectoryFileNames *x, t_symbol *s, int argc, t_atom *argv)
{
    getAllowedExtsFromArgs(argc, argv, x->allowedExts, t_ofeliaGetDirectoryFileNames::objName);
}

void ofeliaGetDirectoryFileNames_print(t_ofeliaGetDirectoryFileNames *x)
{
    post("\n[%s]", t_ofeliaGetDirectoryFileNames::objName);
    post("allowedExts : ");
    
    for (size_t i=0; i<x->allowedExts.size(); ++i)
        post("%s", x->allowedExts[i].c_str());
}

void ofeliaGetDirectoryFileNames_setup()
{
    ofeliaGetDirectoryFileNames_class = class_new(gensym("ofGetDirectoryFileNames"),
                                              reinterpret_cast<t_newmethod>(ofeliaGetDirectoryFileNames_new),
                                              0, sizeof(t_ofeliaGetDirectoryFileNames),
                                              CLASS_DEFAULT, A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaGetDirectoryFileNames_new),
                     gensym("ofGetDirFileNames"), A_GIMME, 0);
    class_addsymbol(ofeliaGetDirectoryFileNames_class, reinterpret_cast<t_method>(ofeliaGetDirectoryFileNames_symbol));
    class_addmethod(ofeliaGetDirectoryFileNames_class, reinterpret_cast<t_method>(ofeliaGetDirectoryFileNames_set),
                    gensym("allowedExts"), A_GIMME, 0);
    class_addmethod(ofeliaGetDirectoryFileNames_class, reinterpret_cast<t_method>(ofeliaGetDirectoryFileNames_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaGetDirectoryFileNames_class, reinterpret_cast<t_method>(ofeliaGetDirectoryFileNames_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetDirectoryFilePaths object methods
 */
void *ofeliaGetDirectoryFilePaths_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaGetDirectoryFilePaths *x = reinterpret_cast<t_ofeliaGetDirectoryFilePaths*>(pd_new(ofeliaGetDirectoryFilePaths_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!getAllowedExtsFromArgs(argc, argv, x->allowedExts, t_ofeliaGetDirectoryFilePaths::objName))
        return 0;
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetDirectoryFilePaths_symbol(t_ofeliaGetDirectoryFilePaths *x, t_symbol *s)
{
    t_string path = s->s_name;
    ofDirectory dir(path.c_str());
    
    if (dir.exists()) {
        
        for (size_t i=0; i<x->allowedExts.size(); ++i)
            dir.allowExt(x->allowedExts[i].c_str());
        dir.listDir();
        const int ac = static_cast<int>(dir.size());
        t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
        
        for (int i=0; i<ac; ++i) {
            
            av[i].a_type = A_SYMBOL;
            av[i].a_w.w_symbol = gensym(dir.getPath(i).c_str());
        }
        outlet_list(x->x_obj.ob_outlet, &s_list, ac, av);
        freebytes(av, ac * sizeof(t_atom));
    }
    else {
        
        error("%s: the directory '%s' does not exist", t_ofeliaGetDirectoryFilePaths::objName, path.c_str());
    }
}

void ofeliaGetDirectoryFilePaths_set(t_ofeliaGetDirectoryFilePaths *x, t_symbol *s, int argc, t_atom *argv)
{
    getAllowedExtsFromArgs(argc, argv, x->allowedExts, t_ofeliaGetDirectoryFilePaths::objName);
}

void ofeliaGetDirectoryFilePaths_print(t_ofeliaGetDirectoryFilePaths *x)
{
    post("\n[%s]", t_ofeliaGetDirectoryFilePaths::objName);
    post("allowedExts : ");
    
    for (size_t i=0; i<x->allowedExts.size(); ++i)
        post("%s", x->allowedExts[i].c_str());
}

void ofeliaGetDirectoryFilePaths_setup()
{
    ofeliaGetDirectoryFilePaths_class = class_new(gensym("ofGetDirectoryFilePaths"),
                                                  reinterpret_cast<t_newmethod>(ofeliaGetDirectoryFilePaths_new),
                                                  0, sizeof(t_ofeliaGetDirectoryFilePaths),
                                                  CLASS_DEFAULT, A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaGetDirectoryFilePaths_new),
                     gensym("ofGetDirFilePaths"), A_GIMME, 0);
    class_addsymbol(ofeliaGetDirectoryFilePaths_class, reinterpret_cast<t_method>(ofeliaGetDirectoryFilePaths_symbol));
    class_addmethod(ofeliaGetDirectoryFilePaths_class, reinterpret_cast<t_method>(ofeliaGetDirectoryFilePaths_set),
                    gensym("allowedExts"), A_GIMME, 0);
    class_addmethod(ofeliaGetDirectoryFilePaths_class, reinterpret_cast<t_method>(ofeliaGetDirectoryFilePaths_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaGetDirectoryFilePaths_class, reinterpret_cast<t_method>(ofeliaGetDirectoryFilePaths_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetDollarZero object methods
 */
void *ofeliaGetDollarZero_new(t_floatarg f)
{
    t_ofeliaGetDollarZero *x = reinterpret_cast<t_ofeliaGetDollarZero*>(pd_new(ofeliaGetDollarZero_class));
    t_canvas *canvas = canvas_getcurrent();
    const int level = truncf(f);
    
    for (int i=0; i<level; ++i) {
        
        if (canvas->gl_owner)
            canvas = canvas->gl_owner;
        else
            break;
    }
    x->dollarZeroSym = canvas_realizedollar(canvas, gensym("$0"));
    outlet_new(&x->x_obj, &s_symbol);
    return (x);
}

void ofeliaGetDollarZero_bang(t_ofeliaGetDollarZero *x)
{
    outlet_symbol(x->x_obj.ob_outlet, x->dollarZeroSym);
}

void ofeliaGetDollarZero_setup()
{
    ofeliaGetDollarZero_class = class_new(gensym("ofGetDollarZero"),
                                          reinterpret_cast<t_newmethod>(ofeliaGetDollarZero_new),
                                          0, sizeof(t_ofeliaGetDollarZero),
                                          CLASS_DEFAULT, A_DEFFLOAT, 0);
    class_addbang(ofeliaGetDollarZero_class, reinterpret_cast<t_method>(ofeliaGetDollarZero_bang));
}

/* ________________________________________________________________________________
 * ofGetCanvasName object methods
 */
void *ofeliaGetCanvasName_new(t_floatarg f)
{
    t_ofeliaGetCanvasName *x = reinterpret_cast<t_ofeliaGetCanvasName*>(pd_new(ofeliaGetCanvasName_class));
    t_canvas *canvas = canvas_getcurrent();
    const int level = truncf(f);
    
    for (int i=0; i<level; ++i) {
        
        if (canvas->gl_owner)
            canvas = canvas->gl_owner;
        else
            break;
    }
    char canvasVarPrefix[MAXPDSTRING];
    snprintf(canvasVarPrefix, MAXPDSTRING, "%lx", reinterpret_cast<long unsigned int>(canvas));
    x->canvasName = gensym(canvasVarPrefix);
    outlet_new(&x->x_obj, &s_symbol);
    return (x);
}

void ofeliaGetCanvasName_bang(t_ofeliaGetCanvasName *x)
{
    outlet_symbol(x->x_obj.ob_outlet, x->canvasName);
}

void ofeliaGetCanvasName_setup()
{
    ofeliaGetCanvasName_class = class_new(gensym("ofGetCanvasName"),
                                           reinterpret_cast<t_newmethod>(ofeliaGetCanvasName_new),
                                           0, sizeof(t_ofeliaGetCanvasName),
                                           CLASS_DEFAULT, A_DEFFLOAT, 0);
    class_addbang(ofeliaGetCanvasName_class, reinterpret_cast<t_method>(ofeliaGetCanvasName_bang));
}

/* ________________________________________________________________________________
 * ofGetCanvasIndex object methods
 */
void *ofeliaGetCanvasIndex_new(t_floatarg f)
{
    t_ofeliaGetCanvasIndex *x = reinterpret_cast<t_ofeliaGetCanvasIndex*>(pd_new(ofeliaGetCanvasIndex_class));
    t_canvas *canvas = canvas_getcurrent();
    t_gobj *obj = reinterpret_cast<t_gobj*>(x);
    const int level = truncf(f);
    
    for (int i=0; i<level; ++i) {
        
        if (canvas->gl_owner) {
            
            obj = reinterpret_cast<t_gobj*>(canvas);
            canvas = canvas->gl_owner;
        }
        else
            break;
    }
    x->canvas = canvas;
    x->gobj = obj;
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetCanvasIndex_bang(t_ofeliaGetCanvasIndex *x)
{
    t_glist *glist = static_cast<t_glist*>(x->canvas);
    t_gobj *obj;
    int index = 0;
    
    for (obj = glist->gl_list; obj && obj != x->gobj; obj = obj->g_next)
        index++;
    outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(index));
}

void ofeliaGetCanvasIndex_setup()
{
    ofeliaGetCanvasIndex_class = class_new(gensym("ofGetCanvasIndex"),
                                           reinterpret_cast<t_newmethod>(ofeliaGetCanvasIndex_new),
                                           0, sizeof(t_ofeliaGetCanvasIndex),
                                           CLASS_DEFAULT, A_DEFFLOAT, 0);
    class_addbang(ofeliaGetCanvasIndex_class, reinterpret_cast<t_method>(ofeliaGetCanvasIndex_bang));
}

/* ________________________________________________________________________________
 * ofGetCanvasArgs object methods
 */
void *ofeliaGetCanvasArgs_new(t_floatarg f)
{
    t_ofeliaGetCanvasArgs *x = reinterpret_cast<t_ofeliaGetCanvasArgs*>(pd_new(ofeliaGetCanvasArgs_class));
    t_canvas *canvas = canvas_getcurrent();
    const int level = truncf(f);
    
    for (int i=0; i<level; ++i) {
        
        if (canvas->gl_owner)
            canvas = canvas->gl_owner;
        else
            break;
    }
    x->canvas = canvas;
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetCanvasArgs_bang(t_ofeliaGetCanvasArgs *x)
{
    t_binbuf *binbuf = x->canvas->gl_obj.te_binbuf;
    
    if (binbuf)
        outlet_list(x->x_obj.ob_outlet, &s_list, binbuf_getnatom(binbuf)-1, binbuf_getvec(binbuf)+1);
    else
        outlet_bang(x->x_obj.ob_outlet);
}

void ofeliaGetCanvasArgs_setup()
{
    ofeliaGetCanvasArgs_class = class_new(gensym("ofGetCanvasArgs"),
                                          reinterpret_cast<t_newmethod>(ofeliaGetCanvasArgs_new),
                                          0, sizeof(t_ofeliaGetCanvasArgs),
                                          CLASS_DEFAULT, A_DEFFLOAT, 0);
    class_addbang(ofeliaGetCanvasArgs_class, reinterpret_cast<t_method>(ofeliaGetCanvasArgs_bang));
}

/* ________________________________________________________________________________
 * ofSetCanvasArgs object methods
 */
void *ofeliaSetCanvasArgs_new(t_floatarg f)
{
    t_ofeliaSetCanvasArgs *x = reinterpret_cast<t_ofeliaSetCanvasArgs*>(pd_new(ofeliaSetCanvasArgs_class));
    t_canvas *canvas = canvas_getcurrent();
    const int level = truncf(f);
    
    for (int i=0; i<level; ++i) {
        
        if (canvas->gl_owner)
            canvas = canvas->gl_owner;
        else
            break;
    }
    x->canvas = canvas;
    return (x);
}

void ofeliaSetCanvasArgs_list(t_ofeliaSetCanvasArgs *x, t_symbol *s, int argc, t_atom *argv)
{
    t_binbuf *binbuf = x->canvas->gl_obj.te_binbuf;
    
    if (!binbuf)
        return;
    t_atom av[1];
    av[0].a_type = A_SYMBOL;
    av[0].a_w.w_symbol = binbuf_getvec(binbuf)[0].a_w.w_symbol;
    binbuf_clear(binbuf);
    binbuf_add(binbuf, 1, av);
    binbuf_add(binbuf, argc, argv);
}

void ofeliaSetCanvasArgs_setup()
{
    ofeliaSetCanvasArgs_class = class_new(gensym("ofSetCanvasArgs"),
                                          reinterpret_cast<t_newmethod>(ofeliaSetCanvasArgs_new),
                                          0, sizeof(t_ofeliaSetCanvasArgs),
                                          CLASS_DEFAULT, A_DEFFLOAT, 0);
    class_addlist(ofeliaSetCanvasArgs_class, reinterpret_cast<t_method>(ofeliaSetCanvasArgs_list));
}

/* ________________________________________________________________________________
 * ofRemoveCanvas object methods
 */
void ofeliaRemoveCanvas_removeCanvas(t_ofeliaRemoveCanvas *x);

void *ofeliaRemoveCanvas_new(t_floatarg f)
{
    t_ofeliaRemoveCanvas *x = reinterpret_cast<t_ofeliaRemoveCanvas*>(pd_new(ofeliaRemoveCanvas_class));
    t_canvas *canvas = canvas_getcurrent();
    t_gobj *obj = reinterpret_cast<t_gobj*>(x);
    const int level = truncf(f);
    
    for (int i=0; i<level; ++i) {
        
        if (canvas->gl_owner) {
            
            obj = reinterpret_cast<t_gobj*>(canvas);
            canvas = canvas->gl_owner;
        }
        else
            break;
    }
    x->canvas = canvas;
    x->gobj = obj;
    x->clock = clock_new(x, reinterpret_cast<t_method>(ofeliaRemoveCanvas_removeCanvas));
    return (x);
}

void ofeliaRemoveCanvas_bang(t_ofeliaRemoveCanvas *x)
{
    clock_delay(x->clock, 0.0);
}

void ofeliaRemoveCanvas_removeCanvas(t_ofeliaRemoveCanvas *x)
{
    clock_free(x->clock);
    const int dspstate = canvas_suspend_dsp();
    glist_delete(static_cast<t_glist*>(x->canvas), x->gobj);
    canvas_resume_dsp(dspstate);
}

void ofeliaRemoveCanvas_setup()
{
    ofeliaRemoveCanvas_class = class_new(gensym("ofRemoveCanvas"),
                                         reinterpret_cast<t_newmethod>(ofeliaRemoveCanvas_new),
                                         0, sizeof(t_ofeliaRemoveCanvas),
                                         CLASS_DEFAULT, A_DEFFLOAT, 0);
    class_addbang(ofeliaRemoveCanvas_class, reinterpret_cast<t_method>(ofeliaRemoveCanvas_bang));
}

/* ________________________________________________________________________________
 * ofGetMinFloat object methods
 */
void *ofeliaGetMinFloat_new()
{
    t_ofeliaGetMinFloat *x = reinterpret_cast<t_ofeliaGetMinFloat*>(pd_new(ofeliaGetMinFloat_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetMinFloat_bang(t_ofeliaGetMinFloat *x)
{
    outlet_float(x->x_obj.ob_outlet, numeric_limits<t_float>::lowest());
}

void ofeliaGetMinFloat_setup()
{
    ofeliaGetMinFloat_class = class_new(gensym("ofGetMinFloat"),
                                        reinterpret_cast<t_newmethod>(ofeliaGetMinFloat_new),
                                        0, sizeof(t_ofeliaGetMinFloat),
                                        CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetMinFloat_class, reinterpret_cast<t_method>(ofeliaGetMinFloat_bang));
}

/* ________________________________________________________________________________
 * ofGetMaxFloat object methods
 */
void *ofeliaGetMaxFloat_new()
{
    t_ofeliaGetMaxFloat *x = reinterpret_cast<t_ofeliaGetMaxFloat*>(pd_new(ofeliaGetMaxFloat_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetMaxFloat_bang(t_ofeliaGetMaxFloat *x)
{
    outlet_float(x->x_obj.ob_outlet, numeric_limits<t_float>::max());
}

void ofeliaGetMaxFloat_setup()
{
    ofeliaGetMaxFloat_class = class_new(gensym("ofGetMaxFloat"),
                                        reinterpret_cast<t_newmethod>(ofeliaGetMaxFloat_new),
                                        0, sizeof(t_ofeliaGetMaxFloat),
                                        CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetMaxFloat_class, reinterpret_cast<t_method>(ofeliaGetMaxFloat_bang));
}

/* ________________________________________________________________________________
 * ofHexToHsb object methods
 */
void *ofeliaHexToHsb_new()
{
    t_ofeliaHexToHsb *x = reinterpret_cast<t_ofeliaHexToHsb*>(pd_new(ofeliaHexToHsb_class));
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaHexToHsb_symbol(t_ofeliaHexToHsb *x, t_symbol *s)
{
    const char *hex = s->s_name;
    
    if (strlen(hex) == 8 && !strncmp(hex, "0x", 2)) {

        const ofColor &color = ofColor::fromHex(ofHexToInt(hex));
        t_atom av[3];
        av[0].a_type = A_FLOAT;
        av[0].a_w.w_float = color.getHue();
        av[1].a_type = A_FLOAT;
        av[1].a_w.w_float = color.getSaturation();
        av[2].a_type = A_FLOAT;
        av[2].a_w.w_float = color.getBrightness();
        outlet_list(x->x_obj.ob_outlet, &s_list, 3, av);
    }
    else {
        
        error("%s: invalid hex color '%s'", t_ofeliaHexToHsb::objName, hex);
    }
}

void ofeliaHexToHsb_setup()
{
    ofeliaHexToHsb_class = class_new(gensym("ofHexToHsb"),
                                     reinterpret_cast<t_newmethod>(ofeliaHexToHsb_new),
                                     0, sizeof(t_ofeliaHexToHsb),
                                     CLASS_DEFAULT, A_NULL, 0);
    class_addsymbol(ofeliaHexToHsb_class, reinterpret_cast<t_method>(ofeliaHexToHsb_symbol));
}

/* ________________________________________________________________________________
 * ofHexToRgb object methods
 */
void *ofeliaHexToRgb_new()
{
    t_ofeliaHexToRgb *x = reinterpret_cast<t_ofeliaHexToRgb*>(pd_new(ofeliaHexToRgb_class));
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaHexToRgb_symbol(t_ofeliaHexToRgb *x, t_symbol *s)
{
    const char *hex = s->s_name;
    
    if (strlen(hex) == 8 && !strncmp(hex, "0x", 2)) {
        
        const ofColor &color = ofColor::fromHex(ofHexToInt(hex));
        t_atom av[3];
        av[0].a_type = A_FLOAT;
        av[0].a_w.w_float = color.r;
        av[1].a_type = A_FLOAT;
        av[1].a_w.w_float = color.g;
        av[2].a_type = A_FLOAT;
        av[2].a_w.w_float = color.b;
        outlet_list(x->x_obj.ob_outlet, &s_list, 3, av);
    }
    else {
        
        error("%s: invalid hex color '%s'", t_ofeliaHexToRgb::objName, hex);
    }
}

void ofeliaHexToRgb_setup()
{
    ofeliaHexToRgb_class = class_new(gensym("ofHexToRgb"),
                                     reinterpret_cast<t_newmethod>(ofeliaHexToRgb_new),
                                     0, sizeof(t_ofeliaHexToRgb),
                                     CLASS_DEFAULT, A_NULL, 0);
    class_addsymbol(ofeliaHexToRgb_class, reinterpret_cast<t_method>(ofeliaHexToRgb_symbol));
}

/* ________________________________________________________________________________
 * ofHsbToHex object methods
 */
void *ofeliaHsbToHex_new()
{
    t_ofeliaHsbToHex *x = reinterpret_cast<t_ofeliaHsbToHex*>(pd_new(ofeliaHsbToHex_class));
    outlet_new(&x->x_obj, &s_symbol);
    return (x);
}

void ofeliaHsbToHex_list(t_ofeliaHsbToHex *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            const ofColor &color = ofColor::fromHsb(argv[0].a_w.w_float,
                                                    argv[1].a_w.w_float,
                                                    argv[2].a_w.w_float);
            t_string hex = ofToHex(color.getHex()).c_str();
            hex[1] = 'x';
            outlet_symbol(x->x_obj.ob_outlet, gensym(hex.c_str()));
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaHsbToHex::objName);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaHsbToHex::objName);
    }
}

void ofeliaHsbToHex_setup()
{
    ofeliaHsbToHex_class = class_new(gensym("ofHsbToHex"),
                                     reinterpret_cast<t_newmethod>(ofeliaHsbToHex_new),
                                     0, sizeof(t_ofeliaHsbToHex),
                                     CLASS_DEFAULT, A_NULL, 0);
    class_addlist(ofeliaHsbToHex_class, reinterpret_cast<t_method>(ofeliaHsbToHex_list));
}

/* ________________________________________________________________________________
 * ofHsbToRgb object methods
 */
void *ofeliaHsbToRgb_new()
{
    t_ofeliaHsbToRgb *x = reinterpret_cast<t_ofeliaHsbToRgb*>(pd_new(ofeliaHsbToRgb_class));
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaHsbToRgb_list(t_ofeliaHsbToRgb *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            const ofColor &color = ofColor::fromHsb(argv[0].a_w.w_float,
                                                    argv[1].a_w.w_float,
                                                    argv[2].a_w.w_float);
            t_atom av[3];
            av[0].a_type = A_FLOAT;
            av[0].a_w.w_float = color.r;
            av[1].a_type = A_FLOAT;
            av[1].a_w.w_float = color.g;
            av[2].a_type = A_FLOAT;
            av[2].a_w.w_float = color.b;
            outlet_list(x->x_obj.ob_outlet, &s_list, 3, av);
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaHsbToRgb::objName);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaHsbToRgb::objName);
    }
}

void ofeliaHsbToRgb_setup()
{
    ofeliaHsbToRgb_class = class_new(gensym("ofHsbToRgb"),
                                     reinterpret_cast<t_newmethod>(ofeliaHsbToRgb_new),
                                     0, sizeof(t_ofeliaHsbToRgb),
                                     CLASS_DEFAULT, A_NULL, 0);
    class_addlist(ofeliaHsbToRgb_class, reinterpret_cast<t_method>(ofeliaHsbToRgb_list));
}

/* ________________________________________________________________________________
 * ofRgbToHex object methods
 */
void *ofeliaRgbToHex_new()
{
    t_ofeliaRgbToHex *x = reinterpret_cast<t_ofeliaRgbToHex*>(pd_new(ofeliaRgbToHex_class));
    outlet_new(&x->x_obj, &s_symbol);
    return (x);
}

void ofeliaRgbToHex_list(t_ofeliaRgbToHex *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            const ofColor &color = ofColor(argv[0].a_w.w_float,
                                           argv[1].a_w.w_float,
                                           argv[2].a_w.w_float);
            t_string hex = ofToHex(color.getHex()).c_str();
            hex[1] = 'x';
            outlet_symbol(x->x_obj.ob_outlet, gensym(hex.c_str()));
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaRgbToHex::objName);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaRgbToHex::objName);
    }
}

void ofeliaRgbToHex_setup()
{
    ofeliaRgbToHex_class = class_new(gensym("ofRgbToHex"),
                                     reinterpret_cast<t_newmethod>(ofeliaRgbToHex_new),
                                     0, sizeof(t_ofeliaRgbToHex),
                                     CLASS_DEFAULT, A_NULL, 0);
    class_addlist(ofeliaRgbToHex_class, reinterpret_cast<t_method>(ofeliaRgbToHex_list));
}

/* ________________________________________________________________________________
 * ofRgbToHsb object methods
 */
void *ofeliaRgbToHsb_new()
{
    t_ofeliaRgbToHsb *x = reinterpret_cast<t_ofeliaRgbToHsb*>(pd_new(ofeliaRgbToHsb_class));
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaRgbToHsb_list(t_ofeliaRgbToHsb *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc >= 3) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT &&
            argv[2].a_type == A_FLOAT) {
            
            const ofColor &color = ofColor(argv[0].a_w.w_float,
                                           argv[1].a_w.w_float,
                                           argv[2].a_w.w_float);
            t_atom av[3];
            av[0].a_type = A_FLOAT;
            av[0].a_w.w_float = color.getHue();
            av[1].a_type = A_FLOAT;
            av[1].a_w.w_float = color.getSaturation();
            av[2].a_type = A_FLOAT;
            av[2].a_w.w_float = color.getBrightness();
            outlet_list(x->x_obj.ob_outlet, &s_list, 3, av);
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaRgbToHsb::objName);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaRgbToHsb::objName);
    }
}

void ofeliaRgbToHsb_setup()
{
    ofeliaRgbToHsb_class = class_new(gensym("ofRgbToHsb"),
                                     reinterpret_cast<t_newmethod>(ofeliaRgbToHsb_new),
                                     0, sizeof(t_ofeliaRgbToHsb),
                                     CLASS_DEFAULT, A_NULL, 0);
    class_addlist(ofeliaRgbToHsb_class, reinterpret_cast<t_method>(ofeliaRgbToHsb_list));
}

/* ________________________________________________________________________________
 * ofError object methods
 */
void *ofeliaError_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaError *x = reinterpret_cast<t_ofeliaError*>(pd_new(ofeliaError_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    
    if (!argc) {
        
        x->symbol = gensym("error");
    }
    else {
        
        stringstream ss;
        const char space = ' ';
        
        for (int i=0; i<argc; ++i) {
            
            if (argv[i].a_type == A_FLOAT) {
                
                ss << argv[i].a_w.w_float;
            }
            else if (argv[i].a_type == A_SYMBOL) {
                
                ss << argv[i].a_w.w_symbol->s_name;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaError::objName);
                return 0;
            }
            ss << space;
        }
        t_string str = ss.str().c_str();
        str.pop_back();
        x->symbol = gensym(str.c_str());
    }
    return (x);
}

void ofeliaError_bang(t_ofeliaError *x)
{
    error("%s%sbang", x->symbol->s_name, (*x->symbol->s_name ? ": " : ""));
}

void ofeliaError_pointer(t_ofeliaError *x, t_gpointer *gp)
{
    error("%s%s(gpointer)", x->symbol->s_name, (*x->symbol->s_name ? ": " : ""));
}

void ofeliaError_float(t_ofeliaError *x, t_floatarg f)
{
    error("%s%s%g", x->symbol->s_name, (*x->symbol->s_name ? ": " : ""), f);
}

void ofeliaError_symbol(t_ofeliaError *x, t_symbol *s)
{
    error("%s%ssymbol %s", x->symbol->s_name, (*x->symbol->s_name ? ": " : ""), s->s_name);
}

void ofeliaError_list(t_ofeliaError *x, t_symbol *s, int argc, t_atom *argv)
{
    if (!argc) {
        
        error("%s%sbang", x->symbol->s_name, (*x->symbol->s_name ? ": " : ""));
    }
    else {
        
        stringstream ss;
        const char space = ' ';
        
        if (argv[0].a_type == A_SYMBOL)
            ss << s->s_name << space;
        
        for (int i=0; i<argc; ++i) {
            
            if (argv[i].a_type == A_FLOAT) {
                
                ss << argv[i].a_w.w_float;
            }
            else if (argv[i].a_type == A_SYMBOL) {
                
                ss << argv[i].a_w.w_symbol->s_name;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaError::objName);
                return;
            }
            ss << space;
        }
        t_string str = ss.str().c_str();
        str.pop_back();
        error("%s%s%s", x->symbol->s_name, (*x->symbol->s_name ? ": " : ""), str.c_str());
    }
}

void ofeliaError_anything(t_ofeliaError *x, t_symbol *s, int argc, t_atom *argv)
{
    if (!argc) {
        
        error("%s%s%s", x->symbol->s_name, (*x->symbol->s_name ? ": " : ""), s->s_name);
    }
    else {
        
        stringstream ss;
        const char space = ' ';
        ss << s->s_name << space;
        
        for (int i=0; i<argc; ++i) {
            
            if (argv[i].a_type == A_FLOAT) {
                
                ss << argv[i].a_w.w_float;
            }
            else if (argv[i].a_type == A_SYMBOL) {
                
                ss << argv[i].a_w.w_symbol->s_name;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaError::objName);
                return;
            }
            ss << space;
        }
        t_string str = ss.str().c_str();
        str.pop_back();
        error("%s%s%s", x->symbol->s_name, (*x->symbol->s_name ? ": " : ""), str.c_str());
    }
}

void ofeliaError_setup()
{
    ofeliaError_class = class_new(gensym("ofError"),
                                  reinterpret_cast<t_newmethod>(ofeliaError_new),
                                  0, sizeof(t_ofeliaError),
                                  CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaError_class, reinterpret_cast<t_method>(ofeliaError_bang));
    class_addpointer(ofeliaError_class, reinterpret_cast<t_method>(ofeliaError_pointer));
    class_addfloat(ofeliaError_class, reinterpret_cast<t_method>(ofeliaError_float));
    class_addsymbol(ofeliaError_class, reinterpret_cast<t_method>(ofeliaError_symbol));
    class_addlist(ofeliaError_class, reinterpret_cast<t_method>(ofeliaError_list));
    class_addanything(ofeliaError_class, reinterpret_cast<t_method>(ofeliaError_anything));
}

/* ________________________________________________________________________________
 * ofSystemLoadDialog object methods
 */
void *ofeliaSystemLoadDialog_new()
{
    t_ofeliaSystemLoadDialog *x = reinterpret_cast<t_ofeliaSystemLoadDialog*>(pd_new(ofeliaSystemLoadDialog_class));
    x->windowTitle = "";
    x->folderSelection = false;
    x->defaultPath = "";
    outlet_new(&x->x_obj, &s_symbol);
    return (x);
}

void ofeliaSystemLoadDialog_bang(t_ofeliaSystemLoadDialog *x)
{
    if (!ofeliaWindow::bWindowExists) {
        
        error("%s: window does not exist", t_ofeliaSystemLoadDialog::objName);
        return;
    }
    ofFileDialogResult result = ofSystemLoadDialog(x->windowTitle.c_str(), x->folderSelection, x->defaultPath.c_str());
    
    if (result.bSuccess)
        outlet_symbol(x->x_obj.ob_outlet, gensym(result.getPath().c_str()));
}

void ofeliaSystemLoadDialog_windowTitle(t_ofeliaSystemLoadDialog *x, t_symbol *s, int argc, t_atom *argv)
{
    stringstream ss;
    const char space = ' ';
    
    for (int i=0; i<argc; ++i) {
        
        if (argv[i].a_type == A_FLOAT) {
            
            ss << argv[i].a_w.w_float;
        }
        else if (argv[i].a_type == A_SYMBOL) {
            
            ss << argv[i].a_w.w_symbol->s_name;
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaSystemLoadDialog::objName);
            return;
        }
        ss << space;
    }
    x->windowTitle = ss.str().c_str();
    x->windowTitle.pop_back();
}

void ofeliaSystemLoadDialog_folderSelection(t_ofeliaSystemLoadDialog *x, t_symbol *s, int argc, t_atom *argv)
{
    getToggleFromArgs(argc, argv, x->folderSelection, t_ofeliaSystemLoadDialog::objName);
}

void ofeliaSystemLoadDialog_defaultPath(t_ofeliaSystemLoadDialog *x, t_symbol *s)
{
    x->defaultPath = s->s_name;
}

void ofeliaSystemLoadDialog_print(t_ofeliaSystemLoadDialog *x)
{
    post("\n[%s]", t_ofeliaSystemLoadDialog::objName);
    post("windowTitle : %s", x->windowTitle.c_str());
    printToggle("folderSelection", x->folderSelection);
    post("defaultPath : %s", x->defaultPath.c_str());
}

void ofeliaSystemLoadDialog_setup()
{
    ofeliaSystemLoadDialog_class = class_new(gensym("ofSystemLoadDialog"),
                                             reinterpret_cast<t_newmethod>(ofeliaSystemLoadDialog_new),
                                             0, sizeof(t_ofeliaSystemLoadDialog),
                                             CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaSystemLoadDialog_class, reinterpret_cast<t_method>(ofeliaSystemLoadDialog_bang));
    class_addmethod(ofeliaSystemLoadDialog_class, reinterpret_cast<t_method>(ofeliaSystemLoadDialog_windowTitle),
                    gensym("windowTitle"), A_GIMME, 0);
    class_addmethod(ofeliaSystemLoadDialog_class, reinterpret_cast<t_method>(ofeliaSystemLoadDialog_folderSelection),
                    gensym("folderSelection"), A_GIMME, 0);
    class_addmethod(ofeliaSystemLoadDialog_class, reinterpret_cast<t_method>(ofeliaSystemLoadDialog_defaultPath),
                    gensym("defaultPath"), A_SYMBOL, 0);
    class_addmethod(ofeliaSystemLoadDialog_class, reinterpret_cast<t_method>(ofeliaSystemLoadDialog_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSystemSaveDialog object methods
 */
void *ofeliaSystemSaveDialog_new()
{
    t_ofeliaSystemSaveDialog *x = reinterpret_cast<t_ofeliaSystemSaveDialog*>(pd_new(ofeliaSystemSaveDialog_class));
    x->defaultName = "";
    x->messageName = "";
    outlet_new(&x->x_obj, &s_symbol);
    return (x);
}

void ofeliaSystemSaveDialog_bang(t_ofeliaSystemSaveDialog *x)
{
    if (!ofeliaWindow::bWindowExists) {
        
        error("%s: window does not exist", t_ofeliaSystemSaveDialog::objName);
        return;
    }
    ofFileDialogResult result = ofSystemSaveDialog(x->defaultName.c_str(), x->messageName.c_str());
    
    if (result.bSuccess)
        outlet_symbol(x->x_obj.ob_outlet, gensym(result.getPath().c_str()));
}

void ofeliaSystemSaveDialog_defaultName(t_ofeliaSystemSaveDialog *x, t_symbol *s)
{
    x->defaultName = s->s_name;
}

void ofeliaSystemSaveDialog_messageName(t_ofeliaSystemSaveDialog *x, t_symbol *s, int argc, t_atom *argv)
{
    stringstream ss;
    const char space = ' ';
    
    for (int i=0; i<argc; ++i) {
        
        if (argv[i].a_type == A_FLOAT) {
            
            ss << argv[i].a_w.w_float;
        }
        else if (argv[i].a_type == A_SYMBOL) {
            
            ss << argv[i].a_w.w_symbol->s_name;
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaSystemSaveDialog::objName);
            return;
        }
        ss << space;
    }
    x->messageName = ss.str().c_str();
    x->messageName.pop_back();
}

void ofeliaSystemSaveDialog_print(t_ofeliaSystemSaveDialog *x)
{
    post("\n[%s]", t_ofeliaSystemSaveDialog::objName);
    post("defaultName : %s", x->defaultName.c_str());
    post("messageName : %s", x->messageName.c_str());
}

void ofeliaSystemSaveDialog_setup()
{
    ofeliaSystemSaveDialog_class = class_new(gensym("ofSystemSaveDialog"),
                                             reinterpret_cast<t_newmethod>(ofeliaSystemSaveDialog_new),
                                             0, sizeof(t_ofeliaSystemSaveDialog),
                                             CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaSystemSaveDialog_class, reinterpret_cast<t_method>(ofeliaSystemSaveDialog_bang));
    class_addmethod(ofeliaSystemSaveDialog_class, reinterpret_cast<t_method>(ofeliaSystemSaveDialog_defaultName),
                    gensym("defaultName"), A_SYMBOL, 0);
    class_addmethod(ofeliaSystemSaveDialog_class, reinterpret_cast<t_method>(ofeliaSystemSaveDialog_messageName),
                    gensym("messageName"), A_GIMME, 0);
    class_addmethod(ofeliaSystemSaveDialog_class, reinterpret_cast<t_method>(ofeliaSystemSaveDialog_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSystemAlertDialog object methods
 */
void *ofeliaSystemAlertDialog_new()
{
    t_ofeliaSystemAlertDialog *x = reinterpret_cast<t_ofeliaSystemAlertDialog*>(pd_new(ofeliaSystemAlertDialog_class));
    x->errorMessage = "";
    return (x);
}

void ofeliaSystemAlertDialog_bang(t_ofeliaSystemAlertDialog *x)
{
    if (!ofeliaWindow::bWindowExists) {
        
        error("%s: window does not exist", t_ofeliaSystemAlertDialog::objName);
        return;
    }
    ofSystemAlertDialog(x->errorMessage.c_str());
}

void ofeliaSystemAlertDialog_errorMessage(t_ofeliaSystemAlertDialog *x, t_symbol *s, int argc, t_atom *argv)
{
    stringstream ss;
    const char space = ' ';
    
    for (int i=0; i<argc; ++i) {
        
        if (argv[i].a_type == A_FLOAT) {
            
            ss << argv[i].a_w.w_float;
        }
        else if (argv[i].a_type == A_SYMBOL) {
            
            ss << argv[i].a_w.w_symbol->s_name;
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaSystemAlertDialog::objName);
            return;
        }
        ss << space;
    }
    x->errorMessage = ss.str().c_str();
    x->errorMessage.pop_back();
}

void ofeliaSystemAlertDialog_print(t_ofeliaSystemAlertDialog *x)
{
    post("\n[%s]", t_ofeliaSystemAlertDialog::objName);
    post("errorMessage : %s", x->errorMessage.c_str());
}

void ofeliaSystemAlertDialog_setup()
{
    ofeliaSystemAlertDialog_class = class_new(gensym("ofSystemAlertDialog"),
                                              reinterpret_cast<t_newmethod>(ofeliaSystemAlertDialog_new),
                                              0, sizeof(t_ofeliaSystemAlertDialog),
                                              CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaSystemAlertDialog_class, reinterpret_cast<t_method>(ofeliaSystemAlertDialog_bang));
    class_addmethod(ofeliaSystemAlertDialog_class, reinterpret_cast<t_method>(ofeliaSystemAlertDialog_errorMessage),
                    gensym("errorMessage"), A_GIMME, 0);
    class_addmethod(ofeliaSystemAlertDialog_class, reinterpret_cast<t_method>(ofeliaSystemAlertDialog_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSystemTextBoxDialog object methods
 */
void *ofeliaSystemTextBoxDialog_new()
{
    t_ofeliaSystemTextBoxDialog *x = reinterpret_cast<t_ofeliaSystemTextBoxDialog*>(pd_new(ofeliaSystemTextBoxDialog_class));
    x->question = "";
    x->text = "";
    outlet_new(&x->x_obj, &s_symbol);
    return (x);
}

void ofeliaSystemTextBoxDialog_bang(t_ofeliaSystemTextBoxDialog *x)
{
    if (!ofeliaWindow::bWindowExists) {
        
        error("%s: window does not exist", t_ofeliaSystemTextBoxDialog::objName);
        return;
    }
    outlet_symbol(x->x_obj.ob_outlet, gensym(ofSystemTextBoxDialog(x->question.c_str(), x->text.c_str()).c_str()));
}

void ofeliaSystemTextBoxDialog_question(t_ofeliaSystemTextBoxDialog *x, t_symbol *s, int argc, t_atom *argv)
{
    stringstream ss;
    const char space = ' ';
    
    for (int i=0; i<argc; ++i) {
        
        if (argv[i].a_type == A_FLOAT) {
            
            ss << argv[i].a_w.w_float;
        }
        else if (argv[i].a_type == A_SYMBOL) {
            
            ss << argv[i].a_w.w_symbol->s_name;
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaSystemTextBoxDialog::objName);
            return;
        }
        ss << space;
    }
    x->question = ss.str().c_str();
    x->question.pop_back();
}

void ofeliaSystemTextBoxDialog_text(t_ofeliaSystemTextBoxDialog *x, t_symbol *s, int argc, t_atom *argv)
{
    stringstream ss;
    const char space = ' ';
    
    for (int i=0; i<argc; ++i) {
        
        if (argv[i].a_type == A_FLOAT) {
            
            ss << argv[i].a_w.w_float;
        }
        else if (argv[i].a_type == A_SYMBOL) {
            
            ss << argv[i].a_w.w_symbol->s_name;
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaSystemTextBoxDialog::objName);
            return;
        }
        ss << space;
    }
    x->text = ss.str().c_str();
    x->text.pop_back();
}

void ofeliaSystemTextBoxDialog_print(t_ofeliaSystemTextBoxDialog *x)
{
    post("\n[%s]", t_ofeliaSystemTextBoxDialog::objName);
    post("question : %s", x->question.c_str());
    post("text : %s", x->text.c_str());
}

void ofeliaSystemTextBoxDialog_setup()
{
    ofeliaSystemTextBoxDialog_class = class_new(gensym("ofSystemTextBoxDialog"),
                                                reinterpret_cast<t_newmethod>(ofeliaSystemTextBoxDialog_new),
                                                0, sizeof(t_ofeliaSystemTextBoxDialog),
                                                CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaSystemTextBoxDialog_class, reinterpret_cast<t_method>(ofeliaSystemTextBoxDialog_bang));
    class_addmethod(ofeliaSystemTextBoxDialog_class, reinterpret_cast<t_method>(ofeliaSystemTextBoxDialog_question),
                    gensym("question"), A_GIMME, 0);
    class_addmethod(ofeliaSystemTextBoxDialog_class, reinterpret_cast<t_method>(ofeliaSystemTextBoxDialog_text),
                    gensym("text"), A_GIMME, 0);
    class_addmethod(ofeliaSystemTextBoxDialog_class, reinterpret_cast<t_method>(ofeliaSystemTextBoxDialog_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * setup methods
 */
void ofeliaUtils_setup()
{
    ofeliaAppend_setup();
    ofeliaPrepend_setup();
    ofeliaPack_setup();
    ofeliaListFind_setup();
    ofeliaFindList_setup();
    ofeliaListInsert_setup();
    ofeliaInsertList_setup();
    ofeliaListFill_setup();
    ofeliaFillList_setup();
    ofeliaListReplace_setup();
    ofeliaReplaceList_setup();
    ofeliaListRemove_setup();
    ofeliaRemoveList_setup();
    ofeliaListErase_setup();
    ofeliaEraseList_setup();
    ofeliaListSort_setup();
    ofeliaListUnique_setup();
    ofeliaListReverse_setup();
    ofeliaListShuffle_setup();
    ofeliaListToSymbol_setup();
    ofeliaSymbolToList_setup();
    ofeliaCount_setup();
    ofeliaCountUntil_setup();
    ofeliaStep_setup();
    ofeliaSwitch_setup();
    ofeliaGate_setup();
    ofeliaValue_setup();
    ofeliaSend_setup();
    ofeliaReceive_setup();
    ofeliaGetPlatform_setup();
    ofeliaGetDate_setup();
    ofeliaGetTime_setup();
    ofeliaGetPatchDirectory_setup();
    ofeliaGetHomeDirectory_setup();
    ofeliaGetDocumentsDirectory_setup();
    ofeliaGetLibraryDirectory_setup();
    ofeliaGetTemporaryDirectory_setup();
    ofeliaDoesDirectoryExist_setup();
    ofeliaDoesFileExist_setup();
    ofeliaGetDirectoryFileNames_setup();
    ofeliaGetDirectoryFilePaths_setup();
    ofeliaGetDollarZero_setup();
    ofeliaGetCanvasName_setup();
    ofeliaGetCanvasIndex_setup();
    ofeliaGetCanvasArgs_setup();
    ofeliaSetCanvasArgs_setup();
    ofeliaRemoveCanvas_setup();
    ofeliaGetMinFloat_setup();
    ofeliaGetMaxFloat_setup();
    ofeliaHexToHsb_setup();
    ofeliaHexToRgb_setup();
    ofeliaHsbToHex_setup();
    ofeliaHsbToRgb_setup();
    ofeliaRgbToHex_setup();
    ofeliaRgbToHsb_setup();
    ofeliaError_setup();
    ofeliaSystemLoadDialog_setup();
    ofeliaSystemSaveDialog_setup();
    ofeliaSystemAlertDialog_setup();
    ofeliaSystemTextBoxDialog_setup();
}

