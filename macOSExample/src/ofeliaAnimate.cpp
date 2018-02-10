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

#include "ofeliaAnimate.h"
#include "ofeliaWindow.h"

/* ________________________________________________________________________________
 * static variables
 */
const char *t_ofeliaAnimate::objName = "ofAnimate";

/* ________________________________________________________________________________
 * ofAnimate object methods
 */
void *ofeliaAnimate_new(t_floatarg f)
{
    t_ofeliaAnimate *x = reinterpret_cast<t_ofeliaAnimate*>(pd_new(ofeliaAnimate_class));
    x->easingType = EASINGTYPE_LINEAR;
    x->currentValue = f;
    x->beginTime = 0.0f;
    x->isAnimating = false;
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::updateSym);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaAnimate_animate(t_ofeliaAnimate *x)
{
    if (x->duration > 0.0f) {
        
        x->beginValue = x->currentValue;
        x->changeInValue = x->endValue - x->currentValue;
        x->currentTime = 0.0f;
    }
    else {
        
        x->beginValue = x->currentValue = x->endValue;
        x->changeInValue = 0.0f;
        x->currentTime = x->duration = 0.0001f;
    }
    x->beginTime = static_cast<float>(ofGetElapsedTimeMillis());
    x->isAnimating = true;
}

float getBounceEaseOut(float t, float b, float c, float d)
{
    if ((t/=d) < (1/2.75f)) {
        
        return c*(7.5625f*t*t) + b;
    }
    else if (t < (2/2.75f)) {
        
        float postFix = t-=(1.5f/2.75f);
        return c*(7.5625f*(postFix)*t + .75f) + b;
    }
    else if (t < (2.5/2.75)) {
        
        float postFix = t-=(2.25f/2.75f);
        return c*(7.5625f*(postFix)*t + .9375f) + b;
    }
    else {
        
        float postFix = t-=(2.625f/2.75f);
        return c*(7.5625f*(postFix)*t + .984375f) + b;
    }
}

float getBounceEaseIn(float t, float b, float c, float d)
{
    return c - getBounceEaseOut(d-t, 0, c, d) + b;
}

float getAnimatedValue(t_ofeliaAnimate *x)
{
    /* all of easing equations below are originally produced by Robert Penner */
    switch (x->easingType) {
            
        case EASINGTYPE_LINEAR:
        {
            return x->changeInValue*x->currentTime/x->duration+x->beginValue;
        }
        case EASINGTYPE_SINE_IN:
        {
            return -x->changeInValue*cosf(x->currentTime/x->duration*(PI/2))+x->changeInValue+x->beginValue;
        }
        case EASINGTYPE_SINE_OUT:
        {
            return x->changeInValue*sinf(x->currentTime/x->duration*(PI/2))+x->beginValue;
        }
        case EASINGTYPE_SINE_INOUT:
        {
            return -x->changeInValue/2*(cosf(PI*x->currentTime/x->duration)-1)+x->beginValue;
        }
        case EASINGTYPE_QUAD_IN:
        {
            x->currentTime/=x->duration;
            return x->changeInValue*x->currentTime*x->currentTime+x->beginValue;
        }
        case EASINGTYPE_QUAD_OUT:
        {
            x->currentTime/=x->duration;
            return -x->changeInValue*x->currentTime*(x->currentTime-2)+x->beginValue;
        }
        case EASINGTYPE_QUAD_INOUT:
        {
            x->currentTime/=x->duration/2;
            if (x->currentTime<1) return x->changeInValue/2*x->currentTime*x->currentTime+x->beginValue;
            --x->currentTime;
            return -x->changeInValue/2*(x->currentTime*(x->currentTime-2)-1)+x->beginValue;
        }
        case EASINGTYPE_CUBIC_IN:
        {
            x->currentTime/=x->duration;
            return x->changeInValue*x->currentTime*x->currentTime*x->currentTime+x->beginValue;
        }
        case EASINGTYPE_CUBIC_OUT:
        {
            x->currentTime/=x->duration;
            --x->currentTime;
            return x->changeInValue*(x->currentTime*x->currentTime*x->currentTime+1)+x->beginValue;
        }
        case EASINGTYPE_CUBIC_INOUT:
        {
            x->currentTime/=x->duration/2;
            if (x->currentTime<1) return x->changeInValue/2*x->currentTime*x->currentTime*x->currentTime+x->beginValue;
            x->currentTime-=2;
            return x->changeInValue/2*(x->currentTime*x->currentTime*x->currentTime+2)+x->beginValue;
        }
        case EASINGTYPE_QUART_IN:
        {
            x->currentTime/=x->duration;
            return x->changeInValue*x->currentTime*x->currentTime*x->currentTime*x->currentTime+x->beginValue;
        }
        case EASINGTYPE_QUART_OUT:
        {
            x->currentTime/=x->duration;
            --x->currentTime;
            return -x->changeInValue*(x->currentTime*x->currentTime*x->currentTime*x->currentTime-1)+x->beginValue;
        }
        case EASINGTYPE_QUART_INOUT:
        {
            x->currentTime/=x->duration/2;
            if (x->currentTime<1) return x->changeInValue/2*x->currentTime*x->currentTime*x->currentTime*x->currentTime+x->beginValue;
            x->currentTime-=2;
            return -x->changeInValue/2*(x->currentTime*x->currentTime*x->currentTime*x->currentTime-2)+x->beginValue;
        }
        case EASINGTYPE_QUINT_IN:
        {
            x->currentTime/=x->duration;
            return x->changeInValue*x->currentTime*x->currentTime*x->currentTime*x->currentTime*x->currentTime+x->beginValue;
        }
        case EASINGTYPE_QUINT_OUT:
        {
            x->currentTime/=x->duration;
            --x->currentTime;
            return x->changeInValue*(x->currentTime*x->currentTime*x->currentTime*x->currentTime*x->currentTime+1)+x->beginValue;
        }
        case EASINGTYPE_QUINT_INOUT:
        {
            x->currentTime/=x->duration/2;
            if (x->currentTime<1) return x->changeInValue/2*x->currentTime*x->currentTime*x->currentTime*x->currentTime*x->currentTime+x->beginValue;
            x->currentTime-=2;
            return x->changeInValue/2*(x->currentTime*x->currentTime*x->currentTime*x->currentTime*x->currentTime+2)+x->beginValue;
        }
        case EASINGTYPE_EXPO_IN:
        {
            return (x->currentTime==0)?x->beginValue:x->changeInValue*powf(2, 10*(x->currentTime/x->duration-1))+x->beginValue;
        }
        case EASINGTYPE_EXPO_OUT:
        {
            return (x->currentTime==x->duration)?x->beginValue+x->changeInValue:x->changeInValue*(-powf(2, -10*x->currentTime/x->duration)+1)+x->beginValue;
        }
        case EASINGTYPE_EXPO_INOUT:
        {
            if (x->currentTime==0) return x->beginValue;
            if (x->currentTime==x->duration) return x->beginValue+x->changeInValue;
            if ((x->currentTime/=x->duration/2)<1) return x->changeInValue/2*powf(2, 10*(x->currentTime-1))+x->beginValue;
            return x->changeInValue/2*(-powf(2, -10*--x->currentTime)+2)+x->beginValue;
        }
        case EASINGTYPE_CIRC_IN:
        {
            x->currentTime/=x->duration;
            return -x->changeInValue*(sqrtf(1-x->currentTime*x->currentTime)-1)+x->beginValue;
        }
        case EASINGTYPE_CIRC_OUT:
        {
            x->currentTime/=x->duration;
            --x->currentTime;
            return x->changeInValue*sqrtf(1-x->currentTime*x->currentTime)+x->beginValue;
        }
        case EASINGTYPE_CIRC_INOUT:
        {
            x->currentTime/=x->duration/2;
            if (x->currentTime<1) return -x->changeInValue/2*(sqrtf(1-x->currentTime*x->currentTime)-1)+x->beginValue;
            x->currentTime-=2;
            return x->changeInValue/2*(sqrtf(1-x->currentTime*x->currentTime)+1)+x->beginValue;
        }
        case EASINGTYPE_BACK_IN:
        {
            const float s=1.70158f;
            const float postFix=x->currentTime/=x->duration;
            return x->changeInValue*(postFix)*x->currentTime*((s+1)*x->currentTime-s)+x->beginValue;
        }
        case EASINGTYPE_BACK_OUT:
        {
            const float s=1.70158f;
            x->currentTime=x->currentTime/x->duration-1;
            return x->changeInValue*(x->currentTime*x->currentTime*((s+1)*x->currentTime+s)+1)+x->beginValue;
        }
        case EASINGTYPE_BACK_INOUT:
        {
            float s=1.70158f;
            s*=1.525f;
            x->currentTime/=x->duration/2;
            if (x->currentTime<1) return x->changeInValue/2*(x->currentTime*x->currentTime*((s+1)*x->currentTime-s))+x->beginValue;
            const float postFix = x->currentTime-=2;
            s*=1.525f;
            return x->changeInValue/2*((postFix)*x->currentTime*((s+1)*x->currentTime+s)+2)+x->beginValue;
        }
        case EASINGTYPE_ELASTIC_IN:
        {
            if (x->currentTime==0) return x->beginValue;
            if ((x->currentTime/=x->duration)==1) return x->beginValue+x->changeInValue;
            const float p=x->duration*.3f;
            const float a=x->changeInValue;
            const float s=p/4;
            const float postFix=a*powf(2,10*(x->currentTime-=1));
            return -(postFix*sinf((x->currentTime*x->duration-s)*(2*PI)/p))+x->beginValue;
        }
        case EASINGTYPE_ELASTIC_OUT:
        {
            if (x->currentTime==0) return x->beginValue;
            if ((x->currentTime/=x->duration)==1) return x->beginValue+x->changeInValue;
            const float p=x->duration*.3f;
            const float a=x->changeInValue;
            const float s=p/4;
            return (a*powf(2,-10*x->currentTime)*sinf((x->currentTime*x->duration-s)*(2*PI)/p)+x->changeInValue+x->beginValue);
        }
        case EASINGTYPE_ELASTIC_INOUT:
        {
            if (x->currentTime==0) return x->beginValue;
            if ((x->currentTime/=x->duration/2)==2) return x->beginValue+x->changeInValue;
            const float p=x->duration*(.3f*1.5f);
            const float a=x->changeInValue;
            const float s=p/4;
            
            if (x->currentTime<1) {
                
                const float postFix=a*powf(2,10*(x->currentTime-=1));
                return -.5f*(postFix*sinf((x->currentTime*x->duration-s)*(2*PI)/p))+x->beginValue;
            }
            const float postFix=a*powf(2,-10*(x->currentTime-=1));
            return postFix*sinf((x->currentTime*x->duration-s)*(2*PI)/p)*.5f+x->changeInValue+x->beginValue;
        }
        case EASINGTYPE_BOUNCE_IN:
        {
            return getBounceEaseIn(x->currentTime, x->beginValue, x->changeInValue, x->duration);
        }
        case EASINGTYPE_BOUNCE_OUT:
        {
            return getBounceEaseOut(x->currentTime, x->beginValue, x->changeInValue, x->duration);
        }
        case EASINGTYPE_BOUNCE_INOUT:
        {
            if (x->currentTime<x->duration/2) return getBounceEaseIn(x->currentTime*2, 0.0f, x->changeInValue, x->duration)*.5f+x->beginValue;
            else return getBounceEaseOut(x->currentTime*2-x->duration, 0, x->changeInValue, x->duration)*.5f+x->changeInValue*.5f+x->beginValue;
        }
        default:
        {
            return x->changeInValue*x->currentTime/x->duration+x->beginValue;
        }
    }
}

void ofeliaAnimate_update(t_ofeliaAnimate *x)
{
    if (x->isAnimating) {
        
        x->currentTime = static_cast<float>(ofGetElapsedTimeMillis()) - x->beginTime;
        
        if (x->currentTime < x->duration) {
            
            x->currentValue = getAnimatedValue(x);
        }
        else {
            
            x->currentValue = x->endValue;
            x->isAnimating = false;
        }
        outlet_float(x->x_obj.ob_outlet, x->currentValue);
    }
}

bool setEasingTypeFromFloat(t_ofeliaAnimate *x, t_float f)
{
    const int type = static_cast<int>(f);
    
    if (type < 0 || type > 30) {
        
        error("%s: easingType out of range", t_ofeliaAnimate::objName);
        return 0;
    }
    x->easingType = static_cast<EasingType>(type);
    return 1;
}

bool setEasingTypeFromSymbol(t_ofeliaAnimate *x, t_symbol *s)
{
    const char *type = s->s_name;
    
    if (!strcmp(type, "LINEAR"))
        x->easingType = EASINGTYPE_LINEAR;
    else if (!strcmp(type, "SINE_IN"))
        x->easingType = EASINGTYPE_SINE_IN;
    else if (!strcmp(type, "SINE_OUT"))
        x->easingType = EASINGTYPE_SINE_OUT;
    else if (!strcmp(type, "SINE_INOUT"))
        x->easingType = EASINGTYPE_SINE_INOUT;
    else if (!strcmp(type, "QUAD_IN"))
        x->easingType = EASINGTYPE_QUAD_IN;
    else if (!strcmp(type, "QUAD_OUT"))
        x->easingType = EASINGTYPE_QUAD_OUT;
    else if (!strcmp(type, "QUAD_INOUT"))
        x->easingType = EASINGTYPE_QUAD_INOUT;
    else if (!strcmp(type, "CUBIC_IN"))
        x->easingType = EASINGTYPE_CUBIC_IN;
    else if (!strcmp(type, "CUBIC_OUT"))
        x->easingType = EASINGTYPE_CUBIC_OUT;
    else if (!strcmp(type, "CUBIC_INOUT"))
        x->easingType = EASINGTYPE_CUBIC_INOUT;
    else if (!strcmp(type, "QUART_IN"))
        x->easingType = EASINGTYPE_QUART_IN;
    else if (!strcmp(type, "QUART_OUT"))
        x->easingType = EASINGTYPE_QUART_OUT;
    else if (!strcmp(type, "QUART_INOUT"))
        x->easingType = EASINGTYPE_QUART_INOUT;
    else if (!strcmp(type, "QUINT_IN"))
        x->easingType = EASINGTYPE_QUINT_IN;
    else if (!strcmp(type, "QUINT_OUT"))
        x->easingType = EASINGTYPE_QUINT_OUT;
    else if (!strcmp(type, "QUINT_INOUT"))
        x->easingType = EASINGTYPE_QUINT_INOUT;
    else if (!strcmp(type, "EXPO_IN"))
        x->easingType = EASINGTYPE_EXPO_IN;
    else if (!strcmp(type, "EXPO_OUT"))
        x->easingType = EASINGTYPE_EXPO_OUT;
    else if (!strcmp(type, "EXPO_INOUT"))
        x->easingType = EASINGTYPE_EXPO_INOUT;
    else if (!strcmp(type, "CIRC_IN"))
        x->easingType = EASINGTYPE_CIRC_IN;
    else if (!strcmp(type, "CIRC_OUT"))
        x->easingType = EASINGTYPE_CIRC_OUT;
    else if (!strcmp(type, "CIRC_INOUT"))
        x->easingType = EASINGTYPE_CIRC_INOUT;
    else if (!strcmp(type, "BACK_IN"))
        x->easingType = EASINGTYPE_BACK_IN;
    else if (!strcmp(type, "BACK_OUT"))
        x->easingType = EASINGTYPE_BACK_OUT;
    else if (!strcmp(type, "BACK_INOUT"))
        x->easingType = EASINGTYPE_BACK_INOUT;
    else if (!strcmp(type, "ELASTIC_IN"))
        x->easingType = EASINGTYPE_ELASTIC_IN;
    else if (!strcmp(type, "ELASTIC_OUT"))
        x->easingType = EASINGTYPE_ELASTIC_OUT;
    else if (!strcmp(type, "ELASTIC_INOUT"))
        x->easingType = EASINGTYPE_ELASTIC_INOUT;
    else if (!strcmp(type, "BOUNCE_IN"))
        x->easingType = EASINGTYPE_BOUNCE_IN;
    else if (!strcmp(type, "BOUNCE_OUT"))
        x->easingType = EASINGTYPE_BOUNCE_OUT;
    else if (!strcmp(type, "BOUNCE_INOUT"))
        x->easingType = EASINGTYPE_BOUNCE_INOUT;
    else {
        
        error("%s: no method for '%s'", t_ofeliaAnimate::objName, type);
        return 0;
    }
    return 1;
}

void ofeliaAnimate_list(t_ofeliaAnimate *x, t_symbol *s, int argc, t_atom *argv)
{
    switch (argc) {
            
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                x->endValue = argv[0].a_w.w_float;
                x->duration = 0.0f;
                x->easingType = EASINGTYPE_LINEAR;
                ofeliaAnimate_animate(x);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaAnimate::objName);
                return;
            }
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT) {
                
                x->endValue = argv[0].a_w.w_float;
                x->duration = argv[1].a_w.w_float;
                x->easingType = EASINGTYPE_LINEAR;
                ofeliaAnimate_animate(x);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaAnimate::objName);
                return;
            }
            break;
        }
        case 3:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT) {
                
                if (setEasingTypeFromFloat(x, argv[2].a_w.w_float)) {
                    
                    x->endValue = argv[0].a_w.w_float;
                    x->duration = argv[1].a_w.w_float;
                    ofeliaAnimate_animate(x);
                }
            }
            else if (argv[0].a_type == A_FLOAT &&
                     argv[1].a_type == A_FLOAT &&
                     argv[2].a_type == A_SYMBOL) {
                
                if (setEasingTypeFromSymbol(x, argv[2].a_w.w_symbol)) {
                    
                    x->endValue = argv[0].a_w.w_float;
                    x->duration = argv[1].a_w.w_float;
                    ofeliaAnimate_animate(x);
                }
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaAnimate::objName);
                return;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaAnimate::objName);
            return;
        }
    }
}

void ofeliaAnimate_stop(t_ofeliaAnimate *x)
{
    x->isAnimating = false;
}

void ofeliaAnimate_set(t_ofeliaAnimate *x, t_floatarg f)
{
    x->currentValue = f;
}

void ofeliaAnimate_print(t_ofeliaAnimate *x)
{
    post("\n[%s]", t_ofeliaAnimate::objName);
    post("value : %g", x->currentValue);
}

void ofeliaAnimate_free(t_ofeliaAnimate *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::updateSym);
}

void ofeliaAnimate_setup()
{
    ofeliaAnimate_class = class_new(gensym("ofAnimate"),
                                    reinterpret_cast<t_newmethod>(ofeliaAnimate_new),
                                    reinterpret_cast<t_method>(ofeliaAnimate_free),
                                    sizeof(t_ofeliaAnimate),
                                    CLASS_DEFAULT, A_DEFFLOAT, 0);
    class_addmethod(ofeliaAnimate_class, reinterpret_cast<t_method>(ofeliaAnimate_update), t_ofeliaWindow::updateMess, A_NULL, 0);
    class_addlist(ofeliaAnimate_class, reinterpret_cast<t_method>(ofeliaAnimate_list));
    class_addmethod(ofeliaAnimate_class, reinterpret_cast<t_method>(ofeliaAnimate_set),
                    gensym("value"), A_FLOAT, 0);
    class_addmethod(ofeliaAnimate_class, reinterpret_cast<t_method>(ofeliaAnimate_stop),
                    gensym("stop"), A_NULL, 0);
    class_addmethod(ofeliaAnimate_class, reinterpret_cast<t_method>(ofeliaAnimate_set),
                    gensym("set"), A_FLOAT, 0);
    class_addmethod(ofeliaAnimate_class, reinterpret_cast<t_method>(ofeliaAnimate_print),
                    gensym("print"), A_NULL, 0);
}

