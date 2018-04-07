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

#ifndef ofeliaShader_h
#define ofeliaShader_h

#pragma once

#include "ofeliaBase.h"

extern "C" {
    
    /* ofLoadShader object class */
    static t_class *ofeliaLoadShader_class;
    
    enum LoadShaderCmdState {
        
        SHADER_LOAD_CMD_INSERT,
        SHADER_LOAD_CMD_ERASE,
        SHADER_LOAD_CMD_FILL
    };
    typedef struct _ofeliaLoadShaderData {
        
        unsigned int objID;
        t_ofeliaVarName varName;
        vector<t_string> paths;
        
    } t_ofeliaLoadShaderData;
    
    typedef struct _ofeliaLoadShaderCmdData {
        
        LoadShaderCmdState state;
        t_string path;
        t_float fromIndex, toIndex;
        
    } t_ofeliaLoadShaderCmdData;
    
    typedef struct _ofeliaLoadShader
    {
        t_object x_obj;
        static const char *objName;
        static unsigned int counter;
        static vector<t_ofeliaLoadShaderData> shaderData;
        static vector<vector<shared_ptr<ofShader>>> shaders;
        vector<t_ofeliaLoadShaderCmdData> cmdVec;
        unique_ptr<vector<char>> shaderLoaded;
        unique_ptr<vector<unsigned int>> shaderIDs;
        unique_ptr<vector<unsigned int>> tempShaderIDs;
        vector<t_string> failPaths;
        bool failPathsShouldClear;
        vector<int> vecSizes;
        bool vecSizesShouldClear;
        static bool bInited;
        bool bInitGate;
        unsigned int objID;
        unsigned int shdrID;
        bool shouldOutlet;
        t_canvas *canvas;
        t_outlet *jobDone_out, *vecSize_out;
        
    } t_ofeliaLoadShader;
    
    /* ofApplyShader object class */
    static t_class *ofeliaApplyShader_class;
    
    typedef struct _ofeliaApplyShader
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        static ofShader *currentShader;
        
    } t_ofeliaApplyShader;
    
    /* ofDoesShaderNameExist object class */
    static t_class *ofeliaDoesShaderNameExist_class;
    
    typedef struct _ofeliaDoesShaderNameExist
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaDoesShaderNameExist;
    
    /* ofGetShaderPath object class */
    static t_class *ofeliaGetShaderPath_class;
    
    typedef struct _ofeliaGetShaderPath
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetShaderPath;
    
    /* ofIsShaderLoaded object class */
    static t_class *ofeliaIsShaderLoaded_class;
    
    typedef struct _ofeliaIsShaderLoaded
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaIsShaderLoaded;
    
    /* ofSetShaderUniform1i object class */
    static t_class *ofeliaSetShaderUniform1i_class;
    
    typedef struct _ofeliaSetShaderUniform1iElem
    {
        t_string uniformName;
        int value;
        
    } t_ofeliaSetShaderUniform1iElem;
    
    typedef struct _ofeliaSetShaderUniform1i
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaSetShaderUniform1iElem elem;
        
    } t_ofeliaSetShaderUniform1i;
    
    /* ofSetShaderUniform2i object class */
    static t_class *ofeliaSetShaderUniform2i_class;
    
    typedef struct _ofeliaSetShaderUniform2iElem
    {
        t_string uniformName;
        int value[2];
        
    } t_ofeliaSetShaderUniform2iElem;
    
    typedef struct _ofeliaSetShaderUniform2i
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaSetShaderUniform2iElem elem;
        
    } t_ofeliaSetShaderUniform2i;
    
    /* ofSetShaderUniform3i object class */
    static t_class *ofeliaSetShaderUniform3i_class;
    
    typedef struct _ofeliaSetShaderUniform3iElem
    {
        t_string uniformName;
        int value[3];
        
    } t_ofeliaSetShaderUniform3iElem;
    
    typedef struct _ofeliaSetShaderUniform3i
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaSetShaderUniform3iElem elem;
        
    } t_ofeliaSetShaderUniform3i;
    
    /* ofSetShaderUniform4i object class */
    static t_class *ofeliaSetShaderUniform4i_class;
    
    typedef struct _ofeliaSetShaderUniform4iElem
    {
        t_string uniformName;
        int value[4];
        
    } t_ofeliaSetShaderUniform4iElem;
    
    typedef struct _ofeliaSetShaderUniform4i
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaSetShaderUniform4iElem elem;
        
    } t_ofeliaSetShaderUniform4i;
    
    /* ofSetShaderUniform1f object class */
    static t_class *ofeliaSetShaderUniform1f_class;
    
    typedef struct _ofeliaSetShaderUniform1fElem
    {
        t_string uniformName;
        float value;
        
    } t_ofeliaSetShaderUniform1fElem;
    
    typedef struct _ofeliaSetShaderUniform1f
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaSetShaderUniform1fElem elem;
        
    } t_ofeliaSetShaderUniform1f;
    
    /* ofSetShaderUniform2f object class */
    static t_class *ofeliaSetShaderUniform2f_class;
    
    typedef struct _ofeliaSetShaderUniform2fElem
    {
        t_string uniformName;
        float value[2];
        
    } t_ofeliaSetShaderUniform2fElem;
    
    typedef struct _ofeliaSetShaderUniform2f
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaSetShaderUniform2fElem elem;
        
    } t_ofeliaSetShaderUniform2f;
    
    /* ofSetShaderUniform3f object class */
    static t_class *ofeliaSetShaderUniform3f_class;
    
    typedef struct _ofeliaSetShaderUniform3fElem
    {
        t_string uniformName;
        float value[3];
        
    } t_ofeliaSetShaderUniform3fElem;
    
    typedef struct _ofeliaSetShaderUniform3f
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaSetShaderUniform3fElem elem;
        
    } t_ofeliaSetShaderUniform3f;
    
    /* ofSetShaderUniform4f object class */
    static t_class *ofeliaSetShaderUniform4f_class;
    
    typedef struct _ofeliaSetShaderUniform4fElem
    {
        t_string uniformName;
        float value[4];
        
    } t_ofeliaSetShaderUniform4fElem;
    
    typedef struct _ofeliaSetShaderUniform4f
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaSetShaderUniform4fElem elem;
        
    } t_ofeliaSetShaderUniform4f;
    
    /* common structs */
    typedef struct _ofeliaSetShaderUniformivElem
    {
        t_string uniformName;
        vector<int> values;
        
    } t_ofeliaSetShaderUniformivElem;
    
    typedef struct _ofeliaSetShaderUniformfvElem
    {
        t_string uniformName;
        vector<float> values;
        
    } t_ofeliaSetShaderUniformfvElem;
    
    typedef struct _ofeliaSetShaderAttributefvElem
    {
        t_string attributeName;
        vector<float> values;
        
    } t_ofeliaSetShaderAttributefvElem;
    
    /* ofSetShaderUniform1iv object class */
    static t_class *ofeliaSetShaderUniform1iv_class;
    
    typedef struct _ofeliaSetShaderUniform1iv
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaSetShaderUniformivElem elem;
        
    } t_ofeliaSetShaderUniform1iv;
    
    /* ofSetShaderUniform2iv object class */
    static t_class *ofeliaSetShaderUniform2iv_class;
    
    typedef struct _ofeliaSetShaderUniform2iv
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaSetShaderUniformivElem elem;
        
    } t_ofeliaSetShaderUniform2iv;
    
    /* ofSetShaderUniform3iv object class */
    static t_class *ofeliaSetShaderUniform3iv_class;
    
    typedef struct _ofeliaSetShaderUniform3iv
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaSetShaderUniformivElem elem;
        
    } t_ofeliaSetShaderUniform3iv;
    
    /* ofSetShaderUniform4iv object class */
    static t_class *ofeliaSetShaderUniform4iv_class;
    
    typedef struct _ofeliaSetShaderUniform4iv
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaSetShaderUniformivElem elem;
        
    } t_ofeliaSetShaderUniform4iv;
    
    /* ofSetShaderUniform1fv object class */
    static t_class *ofeliaSetShaderUniform1fv_class;
    
    typedef struct _ofeliaSetShaderUniform1fv
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaSetShaderUniformfvElem elem;
        
    } t_ofeliaSetShaderUniform1fv;
    
    /* ofSetShaderUniform2fv object class */
    static t_class *ofeliaSetShaderUniform2fv_class;
    
    typedef struct _ofeliaSetShaderUniform2fv
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaSetShaderUniformfvElem elem;
        
    } t_ofeliaSetShaderUniform2fv;
    
    /* ofSetShaderUniform3fv object class */
    static t_class *ofeliaSetShaderUniform3fv_class;
    
    typedef struct _ofeliaSetShaderUniform3fv
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaSetShaderUniformfvElem elem;
        
    } t_ofeliaSetShaderUniform3fv;
    
    /* ofSetShaderUniform4fv object class */
    static t_class *ofeliaSetShaderUniform4fv_class;
    
    typedef struct _ofeliaSetShaderUniform4fv
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaSetShaderUniformfvElem elem;
        
    } t_ofeliaSetShaderUniform4fv;
    
    /* ofSetShaderUniformTex object class */
    static t_class *ofeliaSetShaderUniformTex_class;
    
    typedef struct _ofeliaSetShaderUniformTexElem
    {
        t_string uniformName;
        t_ofeliaVarName imageName;
        int textureLocation;
        
    } t_ofeliaSetShaderUniformTexElem;
    
    typedef struct _ofeliaSetShaderUniformTex
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaSetShaderUniformTexElem elem;
        
    } t_ofeliaSetShaderUniformTex;
    
    /* ofSetShaderAttribute1f object class */
    static t_class *ofeliaSetShaderAttribute1f_class;
    
    typedef struct _ofeliaSetShaderAttribute1fElem
    {
        t_string attributeName;
        float value;
        
    } t_ofeliaSetShaderAttribute1fElem;
    
    typedef struct _ofeliaSetShaderAttribute1f
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaSetShaderAttribute1fElem elem;
        
    } t_ofeliaSetShaderAttribute1f;
    
    /* ofSetShaderAttribute2f object class */
    static t_class *ofeliaSetShaderAttribute2f_class;
    
    typedef struct _ofeliaSetShaderAttribute2fElem
    {
        t_string attributeName;
        float value[2];
        
    } t_ofeliaSetShaderAttribute2fElem;
    
    typedef struct _ofeliaSetShaderAttribute2f
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaSetShaderAttribute2fElem elem;
        
    } t_ofeliaSetShaderAttribute2f;
    
    /* ofSetShaderAttribute3f object class */
    static t_class *ofeliaSetShaderAttribute3f_class;
    
    typedef struct _ofeliaSetShaderAttribute3fElem
    {
        t_string attributeName;
        float value[3];
        
    } t_ofeliaSetShaderAttribute3fElem;
    
    typedef struct _ofeliaSetShaderAttribute3f
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaSetShaderAttribute3fElem elem;
        
    } t_ofeliaSetShaderAttribute3f;
    
    /* ofSetShaderAttribute4f object class */
    static t_class *ofeliaSetShaderAttribute4f_class;
    
    typedef struct _ofeliaSetShaderAttribute4fElem
    {
        t_string attributeName;
        float value[4];
        
    } t_ofeliaSetShaderAttribute4fElem;
    
    typedef struct _ofeliaSetShaderAttribute4f
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaSetShaderAttribute4fElem elem;
        
    } t_ofeliaSetShaderAttribute4f;
    
    /* ofSetShaderAttribute1fv object class */
    static t_class *ofeliaSetShaderAttribute1fv_class;
    
    typedef struct _ofeliaSetShaderAttribute1fv
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaSetShaderAttributefvElem elem;
        
    } t_ofeliaSetShaderAttribute1fv;
    
    /* ofSetShaderAttribute2fv object class */
    static t_class *ofeliaSetShaderAttribute2fv_class;
    
    typedef struct _ofeliaSetShaderAttribute2fv
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaSetShaderAttributefvElem elem;
        
    } t_ofeliaSetShaderAttribute2fv;
    
    /* ofSetShaderAttribute3fv object class */
    static t_class *ofeliaSetShaderAttribute3fv_class;
    
    typedef struct _ofeliaSetShaderAttribute3fv
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaSetShaderAttributefvElem elem;
        
    } t_ofeliaSetShaderAttribute3fv;
    
    /* ofSetShaderAttribute4fv object class */
    static t_class *ofeliaSetShaderAttribute4fv_class;
    
    typedef struct _ofeliaSetShaderAttribute4fv
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaSetShaderAttributefvElem elem;
        
    } t_ofeliaSetShaderAttribute4fv;
    
    /* setup method */
    void ofeliaShader_setup();
}

#endif /* ofeliaShader_h */

