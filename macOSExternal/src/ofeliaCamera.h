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

#ifndef ofeliaCamera_h
#define ofeliaCamera_h

#pragma once

#include "ofeliaBase.h"

extern "C" {
    
    /* enumerations */
    enum CameraOrienType {
        
        CAMERA_ORIEN_TYPE_ROTATE,
        CAMERA_ORIEN_TYPE_EULERANGLES,
        CAMERA_ORIEN_TYPE_QUATERNION,
        CAMERA_ORIEN_TYPE_LOOKAT
    };
    
    /* ofCamera object class */
    static t_class *ofeliaCamera_class;
    
    typedef struct _ofeliaCamera
    {
        t_object x_obj;
        static const char *objName;
        unique_ptr<ofCamera> camera;
        ofVec3f pos;
        t_ofeliaRotateElem rotate;
        ofVec3f eulerAngles;
        ofVec4f quaternion;
        ofVec3f lookAt;
        CameraOrienType orienType;
        float fov;
        float nearClip;
        float farClip;
        float aspectRatio;
        ofVec2f lensOffset;
        bool VFlip;
        bool debug;
        bool bInitGate;
        bool shouldUpdateCamera;
        
    } t_ofeliaCamera;
    
    /* setup method */
    void ofeliaCamera_setup();
}

#endif /* ofeliaCamera_h */
