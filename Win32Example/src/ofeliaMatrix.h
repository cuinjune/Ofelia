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

#ifndef ofeliaMatrix_h
#define ofeliaMatrix_h

#pragma once

#include "ofeliaBase.h"

extern "C" {
    
    /* ofPushMatrix object class */
    static t_class *ofeliaPushMatrix_class;
    
    typedef struct _ofeliaPushMatrix
    {
        t_object x_obj;
        static const char *objName;

    } t_ofeliaPushMatrix;
    
    /* ofPopMatrix object class */
    static t_class *ofeliaPopMatrix_class;
    
    typedef struct _ofeliaPopMatrix
    {
        t_object x_obj;
        static const char *objName;
        
    } t_ofeliaPopMatrix;
    
    /* ofTranslate object class */
    static t_class *ofeliaTranslate_class;
    
    typedef struct _ofeliaTranslate
    {
        t_object x_obj;
        static const char *objName;
        ofVec3f pos;
        
    } t_ofeliaTranslate;
    
    /* ofRotateX object class */
    static t_class *ofeliaRotateX_class;
    
    typedef struct _ofeliaRotateX
    {
        t_object x_obj;
        static const char *objName;
        float degree;
        
    } t_ofeliaRotateX;
    
    /* ofRotateY object class */
    static t_class *ofeliaRotateY_class;
    
    typedef struct _ofeliaRotateY
    {
        t_object x_obj;
        static const char *objName;
        float degree;
        
    } t_ofeliaRotateY;
    
    /* ofRotateZ object class */
    static t_class *ofeliaRotateZ_class;
    
    typedef struct _ofeliaRotateZ
    {
        t_object x_obj;
        static const char *objName;
        float degree;
        
    } t_ofeliaRotateZ;
    
    /* ofRotateXYZ object class */
    static t_class *ofeliaRotateXYZ_class;
    
    typedef struct _ofeliaRotateXYZ
    {
        t_object x_obj;
        static const char *objName;
        ofVec3f degrees;
        
    } t_ofeliaRotateXYZ;
    
    /* ofRotate object class */
    static t_class *ofeliaRotate_class;
    
    typedef struct _ofeliaRotate
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaRotateElem elem;
        
    } t_ofeliaRotate;
    
    /* ofScale object class */
    static t_class *ofeliaScale_class;
    
    typedef struct _ofeliaScale
    {
        t_object x_obj;
        static const char *objName;
        ofVec3f scales;
        
    } t_ofeliaScale;
    
    /* ofGetTranslate object class */
    static t_class *ofeliaGetTranslate_class;
    
    typedef struct _ofeliaGetTranslate
    {
        t_object x_obj;
        static const char *objName;
        
    } t_ofeliaGetTranslate;
    
    /* ofGetRotate object class */
    static t_class *ofeliaGetRotate_class;
    
    typedef struct _ofeliaGetRotate
    {
        t_object x_obj;
        static const char *objName;
        
    } t_ofeliaGetRotate;
    
    /* ofGetScale object class */
    static t_class *ofeliaGetScale_class;
    
    typedef struct _ofeliaGetScale
    {
        t_object x_obj;
        static const char *objName;
        
    } t_ofeliaGetScale;
    
    /* setup methods */
    void ofeliaMatrix_setup();
}

#endif /* ofeliaMatrix_h */
