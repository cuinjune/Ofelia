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

/*==============================================================================
	[ofAnimate] object makes use of Robert Penner's easing equations
	which is free open source software, licensed under the BSD License.
 
	Copyright © 2001 Robert Penner All rights reserved.
 
	Redistribution and use in source and binary forms, with or without modification,
	are permitted provided that the following conditions are met:
 
	Redistributions of source code must retain the above copyright notice, this list
	of conditions and the following disclaimer.
	Redistributions in binary form must reproduce the above copyright notice, this list
	of conditions and the following disclaimer in the documentation and/or other materials
	provided with the distribution.
	Neither the name of the author nor the names of contributors may be used to endorse
	or promote products derived from this software without specific prior written permission.
 
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS
	OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
	AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
	CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
	DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
	IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
	OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
	See http://robertpenner.com/easing for documentation
 ==============================================================================*/

#ifndef ofeliaAnimate_h
#define ofeliaAnimate_h

#pragma once

#include "ofeliaBase.h"

extern "C" {
    
    /* ofAnimate object class */
    static t_class *ofeliaAnimate_class;
    
    enum EasingType {
        
        EASINGTYPE_LINEAR,
        EASINGTYPE_SINE_IN,
        EASINGTYPE_SINE_OUT,
        EASINGTYPE_SINE_INOUT,
        EASINGTYPE_QUAD_IN,
        EASINGTYPE_QUAD_OUT,
        EASINGTYPE_QUAD_INOUT,
        EASINGTYPE_CUBIC_IN,
        EASINGTYPE_CUBIC_OUT,
        EASINGTYPE_CUBIC_INOUT,
        EASINGTYPE_QUART_IN,
        EASINGTYPE_QUART_OUT,
        EASINGTYPE_QUART_INOUT,
        EASINGTYPE_QUINT_IN,
        EASINGTYPE_QUINT_OUT,
        EASINGTYPE_QUINT_INOUT,
        EASINGTYPE_EXPO_IN,
        EASINGTYPE_EXPO_OUT,
        EASINGTYPE_EXPO_INOUT,
        EASINGTYPE_CIRC_IN,
        EASINGTYPE_CIRC_OUT,
        EASINGTYPE_CIRC_INOUT,
        EASINGTYPE_BACK_IN,
        EASINGTYPE_BACK_OUT,
        EASINGTYPE_BACK_INOUT,
        EASINGTYPE_ELASTIC_IN,
        EASINGTYPE_ELASTIC_OUT,
        EASINGTYPE_ELASTIC_INOUT,
        EASINGTYPE_BOUNCE_IN,
        EASINGTYPE_BOUNCE_OUT,
        EASINGTYPE_BOUNCE_INOUT,
    };
    
    typedef struct _ofeliaAnimate
    {
        t_object x_obj;
        static const char *objName;
        EasingType easingType;
        float beginTime, currentTime, duration, beginValue, endValue, currentValue, changeInValue;
        bool isAnimating;
        
    } t_ofeliaAnimate;
    
    /* setup methods */
    void ofeliaAnimate_setup();
}

#endif /* ofeliaAnimate_h */
