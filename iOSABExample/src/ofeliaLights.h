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

#ifndef ofeliaLights_h
#define ofeliaLights_h

#pragma once

#include "ofeliaBase.h"

extern "C" {
    
    /* enumerations */
    enum LightOrienType {
        
        LIGHT_ORIEN_TYPE_ROTATE,
        LIGHT_ORIEN_TYPE_EULERANGLES,
        LIGHT_ORIEN_TYPE_QUATERNION,
        LIGHT_ORIEN_TYPE_LOOKAT
    };
    
    /* common structs */
    typedef struct _ofeliaLightColor
    {
        ofColor specular;
        ofColor diffuse;
        ofColor ambient;
        
    } t_ofeliaLightColor;
    
    /* ofPointLight object class */
    static t_class *ofeliaPointLight_class;
    
    typedef struct _ofeliaPointLight
    {
        t_object x_obj;
        static const char *objName;
        unique_ptr<ofLight> light;
        ofVec3f pos;
        t_ofeliaLightColor color;
        ofVec3f attenuation;
        bool debug;
        bool bInitGate;
        bool shouldUpdateLight;
        
    } t_ofeliaPointLight;
    
    /* ofSpotlight object class */
    static t_class *ofeliaSpotlight_class;

    typedef struct _ofeliaSpotlight
    {
        t_object x_obj;
        static const char *objName;
        unique_ptr<ofLight> light;
        ofVec3f pos;
        t_ofeliaRotateElem rotate;
        ofVec3f eulerAngles;
        ofVec4f quaternion;
        ofVec3f lookAt;
        LightOrienType orienType;
        t_ofeliaLightColor color;
        float cutoff;
        float concentration;
        ofVec3f attenuation;
        bool debug;
        bool bInitGate;
        bool shouldUpdateLight;
        
    } t_ofeliaSpotlight;
    
    /* ofDirectionalLight object class */
    static t_class *ofeliaDirectionalLight_class;
    
    typedef struct _ofeliaDirectionalLight
    {
        t_object x_obj;
        static const char *objName;
        unique_ptr<ofLight> light;
        t_ofeliaRotateElem rotate;
        ofVec3f eulerAngles;
        ofVec4f quaternion;
        LightOrienType orienType;
        t_ofeliaLightColor color;
        bool bInitGate;
        bool shouldUpdateLight;
        
    } t_ofeliaDirectionalLight;
    
    /* setup method */
    void ofeliaLights_setup();
}

#endif /* ofeliaLights_h */
