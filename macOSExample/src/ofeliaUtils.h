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

#ifndef ofeliaUtils_h
#define ofeliaUtils_h

#pragma once

#include "ofeliaBase.h"

extern "C" {
    
    /* ofAppend object class */
    static t_class *ofeliaAppend_class;
    
    typedef struct _ofeliaAppend
    {
        t_object x_obj;
        static const char *objName;
        t_symbol *append;
        bool isAppendEmpty;
        
    } t_ofeliaAppend;
    
    /* ofPrepend object class */
    static t_class *ofeliaPrepend_class;
    
    typedef struct _ofeliaPrepend
    {
        t_object x_obj;
        static const char *objName;
        t_symbol *prepend;
        bool isPrependEmpty;
        
    } t_ofeliaPrepend;
    
    /* ofPack object class */
    static t_class *ofeliaPack_class;
    
    typedef struct _ofeliaPack
    {
        t_object x_obj;
        static const char *objName;
        int ac;
        t_atom *av;
        
    } t_ofeliaPack;
    
    /* ofListFind object class */
    static t_class *ofeliaListFind_class;
    
    typedef struct _ofeliaListFind
    {
        t_object x_obj;
        static const char *objName;
        vector<t_ofeliaAtomElem> elems;
        
    } t_ofeliaListFind;
    
    /* ofFindList object class */
    static t_class *ofeliaFindList_class;
    
    typedef struct _ofeliaFindList
    {
        t_object x_obj;
        static const char *objName;
        vector<t_ofeliaAtomElem> elems;
        
    } t_ofeliaFindList;
    
    /* ofListInsert object class */
    static t_class *ofeliaListInsert_class;
    
    typedef struct _ofeliaListInsert
    {
        t_object x_obj;
        static const char *objName;
        vector<t_ofeliaAtomElem> elems;
        t_float pos;
        
    } t_ofeliaListInsert;
    
    /* ofInsertList object class */
    static t_class *ofeliaInsertList_class;
    
    typedef struct _ofeliaInsertList
    {
        t_object x_obj;
        static const char *objName;
        t_float pos;
        vector<t_ofeliaAtomElem> elems;
        
    } t_ofeliaInsertList;
    
    /* ofListFill object class */
    static t_class *ofeliaListFill_class;
    
    typedef struct _ofeliaListFill
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaAtomElem elem;
        t_float firstPos, lastPos;
        
    } t_ofeliaListFill;
    
    /* ofFillList object class */
    static t_class *ofeliaFillList_class;
    
    typedef struct _ofeliaFillList
    {
        t_object x_obj;
        static const char *objName;
        t_float firstPos, lastPos;
        vector<t_ofeliaAtomElem> elems;
        
    } t_ofeliaFillList;
    
    /* ofListReplace object class */
    static t_class *ofeliaListReplace_class;
    
    typedef struct _ofeliaListReplace
    {
        t_object x_obj;
        static const char *objName;
        vector<t_ofeliaAtomElem> from, to;
        
    } t_ofeliaListReplace;
    
    /* ofReplaceList object class */
    static t_class *ofeliaReplaceList_class;
    
    typedef struct _ofeliaReplaceList
    {
        t_object x_obj;
        static const char *objName;
        vector<t_ofeliaAtomElem> to, src;
        
    } t_ofeliaReplaceList;
    
    /* ofListRemove object class */
    static t_class *ofeliaListRemove_class;
    
    typedef struct _ofeliaListRemove
    {
        t_object x_obj;
        static const char *objName;
        vector<t_ofeliaAtomElem> elems;
        
    } t_ofeliaListRemove;
    
    /* ofRemoveList object class */
    static t_class *ofeliaRemoveList_class;
    
    typedef struct _ofeliaRemoveList
    {
        t_object x_obj;
        static const char *objName;
        vector<t_ofeliaAtomElem> elems;
        
    } t_ofeliaRemoveList;
    
    /* ofListErase object class */
    static t_class *ofeliaListErase_class;
    
    typedef struct _ofeliaListErase
    {
        t_object x_obj;
        static const char *objName;
        t_float firstPos, lastPos;
        
    } t_ofeliaListErase;
    
    /* ofEraseList object class */
    static t_class *ofeliaEraseList_class;
    
    typedef struct _ofeliaEraseList
    {
        t_object x_obj;
        static const char *objName;
        vector<t_ofeliaAtomElem> elems;
        
    } t_ofeliaEraseList;
    
    /* ofListSort object class */
    static t_class *ofeliaListSort_class;
    
    enum ListOrder {
        
        LIST_ORDER_ASC,
        LIST_ORDER_DESC
    };
    typedef struct _ofeliaListSort
    {
        t_object x_obj;
        static const char *objName;
        ListOrder order;
        
    } t_ofeliaListSort;
    
    /* ofListUnique object class */
    static t_class *ofeliaListUnique_class;

    typedef struct _ofeliaListUnique
    {
        t_object x_obj;
        static const char *objName;
        
    } t_ofeliaListUnique;
    
    /* ofListReverse object class */
    static t_class *ofeliaListReverse_class;
    
    typedef struct _ofeliaListReverse
    {
        t_object x_obj;
        static const char *objName;
        
    } t_ofeliaListReverse;
    
    /* ofListShuffle object class */
    static t_class *ofeliaListShuffle_class;
    
    typedef struct _ofeliaListShuffle
    {
        t_object x_obj;
        static const char *objName;
        unsigned int seed;
        
    } t_ofeliaListShuffle;
    
    /* ofListToSymbol object class */
    static t_class *ofeliaListToSymbol_class;
    
    typedef struct _ofeliaListToSymbol
    {
        t_object x_obj;
        static const char *objName;
        t_string delimiter;
        
    } t_ofeliaListToSymbol;
    
    /* ofSymbolToList object class */
    static t_class *ofeliaSymbolToList_class;
    
    typedef struct _ofeliaSymbolToList
    {
        t_object x_obj;
        static const char *objName;
        t_string delimiter;
        
    } t_ofeliaSymbolToList;
    
    /* ofCount object class */
    static t_class *ofeliaCount_class;
    
    typedef struct _ofeliaCountElem
    {
        t_float from;
        t_float to;
        t_float value;
        
    } t_ofeliaCountElem;

    typedef struct _ofeliaCount
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaCountElem elem;
        
    } t_ofeliaCount;
    
    /* ofCountUntil object class */
    static t_class *ofeliaCountUntil_class;
    
    typedef struct _ofeliaCountUntilElem
    {
        t_float from;
        t_float to;
        
    } t_ofeliaCountUntilElem;
    
    typedef struct _ofeliaCountUntil
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaCountUntilElem elem;
        bool shouldBreak;
        
    } t_ofeliaCountUntil;
    
    /* ofStep object class */
    static t_class *ofeliaStep_class;
    
    typedef struct _ofeliaStepElem
    {
        t_float from;
        t_float to;
        t_float value;
        
    } t_ofeliaStepElem;
    
    typedef struct _ofeliaStep
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaStepElem elem;
        
    } t_ofeliaStep;
    
    /* ofSwitch object class */
    static t_class *ofeliaSwitch_class;
    
    typedef struct _ofeliaSwitch
    {
        t_object x_obj;
        static const char *objName;
        int selectedPort;
        
    } t_ofeliaSwitch;
    
    /* ofGate object class */
    static t_class *ofeliaGate_class;
    
    typedef struct _ofeliaGate
    {
        t_object x_obj;
        static const char *objName;
        int selectedPort;
        vector<t_outlet*> gate_outs;
        
    } t_ofeliaGate;
    
    /* ofValue object class */
    static t_class *ofeliaValue_class;
    
    typedef struct _ofeliaValue
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        t_float *value;
        
    } t_ofeliaValue;
    
    /* ofSend object class */
    static t_class *ofeliaSend_class;
    
    typedef struct _ofeliaSend
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        t_symbol *sym;
        t_float *value;
        
    } t_ofeliaSend;
    
    /* ofReceive object class */
    static t_class *ofeliaReceive_class;
    
    typedef struct _ofeliaReceive
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaReceive;
    
    /* ofGetPlatform object class */
    static t_class *ofeliaGetPlatform_class;
    
    typedef struct _ofeliaGetPlatform
    {
        t_object x_obj;
        
    } t_ofeliaGetPlatform;
    
    /* ofGetDate object class */
    static t_class *ofeliaGetDate_class;
    
    typedef struct _ofeliaGetDate
    {
        t_object x_obj;
        
    } t_ofeliaGetDate;
    
    /* ofGetTime object class */
    static t_class *ofeliaGetTime_class;
    
    typedef struct _ofeliaGetTime
    {
        t_object x_obj;
        
    } t_ofeliaGetTime;
    
    /* ofGetPatchDirectory object class */
    static t_class *ofeliaGetPatchDirectory_class;
    
    typedef struct _ofeliaGetPatchDirectory
    {
        t_object x_obj;
        static const char *objName;
        t_symbol *patchDir;
        
    } t_ofeliaGetPatchDirectory;
    
    /* ofGetHomeDirectory object class */
    static t_class *ofeliaGetHomeDirectory_class;
    
    typedef struct _ofeliaGetHomeDirectory
    {
        t_object x_obj;
        static const char *objName;
        
    } t_ofeliaGetHomeDirectory;
    
    /* ofGetDocumentsDirectory object class */
    static t_class *ofeliaGetDocumentsDirectory_class;
    
    typedef struct _ofeliaGetDocumentsDirectory
    {
        t_object x_obj;
        static const char *objName;
        
    } t_ofeliaGetDocumentsDirectory;
    
    /* ofGetLibraryDirectory object class */
    static t_class *ofeliaGetLibraryDirectory_class;
    
    typedef struct _ofeliaGetLibraryDirectory
    {
        t_object x_obj;
        static const char *objName;
        
    } t_ofeliaGetLibraryDirectory;
    
    /* ofGetTemporaryDirectory object class */
    static t_class *ofeliaGetTemporaryDirectory_class;
    
    typedef struct _ofeliaGetTemporaryDirectory
    {
        t_object x_obj;
        static const char *objName;
        
    } t_ofeliaGetTemporaryDirectory;
    
    /* ofDoesDirectoryExist object class */
    static t_class *ofeliaDoesDirectoryExist_class;
    
    typedef struct _ofeliaDoesDirectoryExist
    {
        t_object x_obj;
        static const char *objName;
        
    } t_ofeliaDoesDirectoryExist;
    
    /* ofDoesFileExist object class */
    static t_class *ofeliaDoesFileExist_class;
    
    typedef struct _ofeliaDoesFileExist
    {
        t_object x_obj;
        static const char *objName;
        
    } t_ofeliaDoesFileExist;
    
    /* ofGetDirectoryFileNames object class */
    static t_class *ofeliaGetDirectoryFileNames_class;
    
    typedef struct _ofeliaGetDirectoryFileNames
    {
        t_object x_obj;
        static const char *objName;
        vector<t_string> allowedExts;
        
    } t_ofeliaGetDirectoryFileNames;
    
    /* ofGetDirectoryFilePaths object class */
    static t_class *ofeliaGetDirectoryFilePaths_class;
    
    typedef struct _ofeliaGetDirectoryFilePaths
    {
        t_object x_obj;
        static const char *objName;
        vector<t_string> allowedExts;
        
    } t_ofeliaGetDirectoryFilePaths;
    
    /* ofGetDollarZero object class */
    static t_class *ofeliaGetDollarZero_class;
    
    typedef struct _ofeliaGetDollarZero
    {
        t_object x_obj;
        static const char *objName;
        t_symbol *dollarZeroSym;
        
    } t_ofeliaGetDollarZero;
    
    /* ofGetCanvasName object class */
    static t_class *ofeliaGetCanvasName_class;
    
    typedef struct _ofeliaGetCanvasName
    {
        t_object x_obj;
        static const char *objName;
        t_symbol *canvasName;
        
    } t_ofeliaGetCanvasName;
    
    /* ofGetCanvasIndex object class */
    static t_class *ofeliaGetCanvasIndex_class;
    
    typedef struct _ofeliaGetCanvasIndex
    {
        t_object x_obj;
        static const char *objName;
        t_canvas *canvas;
        t_gobj *gobj;
        
    } t_ofeliaGetCanvasIndex;
    
    /* ofGetCanvasArgs object class */
    static t_class *ofeliaGetCanvasArgs_class;
    
    typedef struct _ofeliaGetCanvasArgs
    {
        t_object x_obj;
        static const char *objName;
        t_canvas *canvas;
        
    } t_ofeliaGetCanvasArgs;
    
    /* ofSetCanvasArgs object class */
    static t_class *ofeliaSetCanvasArgs_class;
    
    typedef struct _ofeliaSetCanvasArgs
    {
        t_object x_obj;
        static const char *objName;
        t_canvas *canvas;
        
    } t_ofeliaSetCanvasArgs;
    
    /* ofRemoveCanvas object class */
    static t_class *ofeliaRemoveCanvas_class;
    
    typedef struct _ofeliaRemoveCanvas
    {
        t_object x_obj;
        static const char *objName;
        t_canvas *canvas;
        t_gobj *gobj;
        t_clock *clock;
        
    } t_ofeliaRemoveCanvas;
    
    /* ofGetMinFloat object class */
    static t_class *ofeliaGetMinFloat_class;
    
    typedef struct _ofeliaGetMinFloat
    {
        t_object x_obj;
        
    } t_ofeliaGetMinFloat;
    
    /* ofGetMaxFloat object class */
    static t_class *ofeliaGetMaxFloat_class;
    
    typedef struct _ofeliaGetMaxFloat
    {
        t_object x_obj;
        
    } t_ofeliaGetMaxFloat;
    
    /* ofHexToHsb object class */
    static t_class *ofeliaHexToHsb_class;
    
    typedef struct _ofeliaHexToHsb
    {
        t_object x_obj;
        static const char *objName;
        
    } t_ofeliaHexToHsb;
    
    /* ofHexToRgb object class */
    static t_class *ofeliaHexToRgb_class;
    
    typedef struct _ofeliaHexToRgb
    {
        t_object x_obj;
        static const char *objName;
        
    } t_ofeliaHexToRgb;
    
    /* ofHsbToHex object class */
    static t_class *ofeliaHsbToHex_class;
    
    typedef struct _ofeliaHsbToHex
    {
        t_object x_obj;
        static const char *objName;
        
    } t_ofeliaHsbToHex;
    
    /* ofHsbToRgb object class */
    static t_class *ofeliaHsbToRgb_class;
    
    typedef struct _ofeliaHsbToRgb
    {
        t_object x_obj;
        static const char *objName;
        
    } t_ofeliaHsbToRgb;
    
    /* ofRgbToHex object class */
    static t_class *ofeliaRgbToHex_class;
    
    typedef struct _ofeliaRgbToHex
    {
        t_object x_obj;
        static const char *objName;
        
    } t_ofeliaRgbToHex;
    
    /* ofRgbToHsb object class */
    static t_class *ofeliaRgbToHsb_class;
    
    typedef struct _ofeliaRgbToHsb
    {
        t_object x_obj;
        static const char *objName;
        
    } t_ofeliaRgbToHsb;
    
    /* ofError object class */
    static t_class *ofeliaError_class;
    
    typedef struct _ofeliaError
    {
        t_object x_obj;
        static const char *objName;
        t_symbol *symbol;
        
    } t_ofeliaError;
    
    /* ofSystemLoadDialog object class */
    static t_class *ofeliaSystemLoadDialog_class;
    
    typedef struct _ofeliaSystemLoadDialog
    {
        t_object x_obj;
        static const char *objName;
        t_string windowTitle;
        bool folderSelection;
        t_string defaultPath;
        
    } t_ofeliaSystemLoadDialog;
    
    /* ofSystemSaveDialog object class */
    static t_class *ofeliaSystemSaveDialog_class;
    
    typedef struct _ofeliaSystemSaveDialog
    {
        t_object x_obj;
        static const char *objName;
        t_string defaultName;
        t_string messageName;
        
    } t_ofeliaSystemSaveDialog;
    
    /* ofSystemAlertDialog object class */
    static t_class *ofeliaSystemAlertDialog_class;
    
    typedef struct _ofeliaSystemAlertDialog
    {
        t_object x_obj;
        static const char *objName;
        t_string errorMessage;
        
    } t_ofeliaSystemAlertDialog;
    
    /* ofSystemTextBoxDialog object class */
    static t_class *ofeliaSystemTextBoxDialog_class;
    
    typedef struct _ofeliaSystemTextBoxDialog
    {
        t_object x_obj;
        static const char *objName;
        t_string question;
        t_string text;
        
    } t_ofeliaSystemTextBoxDialog;
    
    /* setup methods */
    void ofeliaUtils_setup();
}

#endif /* ofeliaUtils_h */
