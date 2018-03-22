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

#include "ofeliaGetters.h"
#include "ofeliaWindow.h"
#include "ofeliaHead.h"

/* ________________________________________________________________________________
 * static variables
 */
t_symbol *t_ofeliaGetWidth::getWidthSym;
t_symbol *t_ofeliaGetHeight::getHeightSym;
t_symbol *t_ofeliaGetDimen::getDimenSym;
t_symbol *t_ofeliaGetWindowScale::getWindowScaleSym;
t_symbol *t_ofeliaGetFrameNum::getFrameNumSym;
t_symbol *t_ofeliaGetFrameRate::getFrameRateSym;
t_symbol *t_ofeliaGetTargetFrameRate::getTargetFrameRateSym;
t_symbol *t_ofeliaGetElapsedTime::getElapsedTimeSym;
t_symbol *t_ofeliaGetElapsedTimeMillis::getElapsedTimeMillisSym;
t_symbol *t_ofeliaGetLastFrameTime::getLastFrameTimeSym;
t_symbol *t_ofeliaGetLastFrameTimeMillis::getLastFrameTimeMillisSym;
t_symbol *t_ofeliaGetOrienLock::getOrienLockSym;
t_symbol *t_ofeliaGetOrien::getOrienSym;
t_symbol *t_ofeliaGetFullscreen::getFullscreenSym;
t_symbol *t_ofeliaGetFocus::getFocusSym;
t_symbol *t_ofeliaGetWindowPosX::getWindowPosXSym;
t_symbol *t_ofeliaGetWindowPosY::getWindowPosYSym;
t_symbol *t_ofeliaGetWindowPos::getWindowPosSym;
t_symbol *t_ofeliaGetScreenWidth::getScreenWidthSym;
t_symbol *t_ofeliaGetScreenHeight::getScreenHeightSym;
t_symbol *t_ofeliaGetScreenDimen::getScreenDimenSym;
t_symbol *t_ofeliaGetRetina::getRetinaSym;
t_symbol *t_ofeliaGetBgColorR::getBgColorRSym;
t_symbol *t_ofeliaGetBgColorG::getBgColorGSym;
t_symbol *t_ofeliaGetBgColorB::getBgColorBSym;
t_symbol *t_ofeliaGetBgColor::getBgColorSym;
t_symbol *t_ofeliaGetWindow::getWindowSym;

/* ________________________________________________________________________________
 * ofGetWidth object methods
 */
void *ofeliaGetWidth_new()
{
    t_ofeliaGetWidth *x = reinterpret_cast<t_ofeliaGetWidth*>(pd_new(ofeliaGetWidth_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetWidth_bang(t_ofeliaGetWidth *x)
{
    t_float f;
    
    if (!value_getfloat(t_ofeliaGetWidth::getWidthSym, &f))
        outlet_float(x->x_obj.ob_outlet, f);
}

void ofeliaGetWidth_setup()
{
    ofeliaGetWidth_class = class_new(gensym("ofGetWidth"),
                                       reinterpret_cast<t_newmethod>(ofeliaGetWidth_new),
                                       0, sizeof(t_ofeliaGetWidth),
                                       CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetWidth_class, reinterpret_cast<t_method>(ofeliaGetWidth_bang));
}

/* ________________________________________________________________________________
 * ofGetHeight object methods
 */
void *ofeliaGetHeight_new()
{
    t_ofeliaGetHeight *x = reinterpret_cast<t_ofeliaGetHeight*>(pd_new(ofeliaGetHeight_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetHeight_bang(t_ofeliaGetHeight *x)
{
    t_float f;
    
    if (!value_getfloat(t_ofeliaGetHeight::getHeightSym, &f))
        outlet_float(x->x_obj.ob_outlet, f);
}

void ofeliaGetHeight_setup()
{
    ofeliaGetHeight_class = class_new(gensym("ofGetHeight"),
                                       reinterpret_cast<t_newmethod>(ofeliaGetHeight_new),
                                       0, sizeof(t_ofeliaGetHeight),
                                       CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetHeight_class, reinterpret_cast<t_method>(ofeliaGetHeight_bang));
}

/* ________________________________________________________________________________
 * ofGetDimen object methods
 */
void *ofeliaGetDimen_new()
{
    t_ofeliaGetDimen *x = reinterpret_cast<t_ofeliaGetDimen*>(pd_new(ofeliaGetDimen_class));
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetDimen_bang(t_ofeliaGetDimen *x)
{
    t_float f_w, f_h;
    
    if (!value_getfloat(t_ofeliaGetWidth::getWidthSym, &f_w) &&
        !value_getfloat(t_ofeliaGetHeight::getHeightSym, &f_h)) {
        
        t_atom av[2];
        av[0].a_type = A_FLOAT;
        av[0].a_w.w_float = f_w;
        av[1].a_type = A_FLOAT;
        av[1].a_w.w_float = f_h;
        outlet_list(x->x_obj.ob_outlet, &s_list, 2, av);
    }
}

void ofeliaGetDimen_setup()
{
    ofeliaGetDimen_class = class_new(gensym("ofGetDimen"),
                                     reinterpret_cast<t_newmethod>(ofeliaGetDimen_new),
                                     0, sizeof(t_ofeliaGetDimen),
                                     CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetDimen_class, reinterpret_cast<t_method>(ofeliaGetDimen_bang));
}

/* ________________________________________________________________________________
 * ofGetWindowScale object methods
 */
void *ofeliaGetWindowScale_new()
{
    t_ofeliaGetWindowScale *x = reinterpret_cast<t_ofeliaGetWindowScale*>(pd_new(ofeliaGetWindowScale_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetWindowScale_bang(t_ofeliaGetWindowScale *x)
{
    t_float f;
    
    if (!value_getfloat(t_ofeliaGetWindowScale::getWindowScaleSym, &f))
        outlet_float(x->x_obj.ob_outlet, f);
}

void ofeliaGetWindowScale_setup()
{
    ofeliaGetWindowScale_class = class_new(gensym("ofGetWindowScale"),
                                     reinterpret_cast<t_newmethod>(ofeliaGetWindowScale_new),
                                     0, sizeof(t_ofeliaGetWindowScale),
                                     CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetWindowScale_class, reinterpret_cast<t_method>(ofeliaGetWindowScale_bang));
}

/* ________________________________________________________________________________
 * ofGetFrameNum object methods
 */
void *ofeliaGetFrameNum_new()
{
    t_ofeliaGetFrameNum *x = reinterpret_cast<t_ofeliaGetFrameNum*>(pd_new(ofeliaGetFrameNum_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetFrameNum_bang(t_ofeliaGetFrameNum *x)
{
    t_float f;
    
    if (!value_getfloat(t_ofeliaGetFrameNum::getFrameNumSym, &f))
        outlet_float(x->x_obj.ob_outlet, f);
}

void ofeliaGetFrameNum_setup()
{
    ofeliaGetFrameNum_class = class_new(gensym("ofGetFrameNum"),
                                        reinterpret_cast<t_newmethod>(ofeliaGetFrameNum_new),
                                        0, sizeof(t_ofeliaGetFrameNum),
                                        CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetFrameNum_class, reinterpret_cast<t_method>(ofeliaGetFrameNum_bang));
}

/* ________________________________________________________________________________
 * ofGetFrameRate object methods
 */
void *ofeliaGetFrameRate_new()
{
    t_ofeliaGetFrameRate *x = reinterpret_cast<t_ofeliaGetFrameRate*>(pd_new(ofeliaGetFrameRate_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetFrameRate_bang(t_ofeliaGetFrameRate *x)
{
    t_float f;
    
    if (!value_getfloat(t_ofeliaGetFrameRate::getFrameRateSym, &f))
        outlet_float(x->x_obj.ob_outlet, f);
}

void ofeliaGetFrameRate_setup()
{
    ofeliaGetFrameRate_class = class_new(gensym("ofGetFrameRate"),
                                         reinterpret_cast<t_newmethod>(ofeliaGetFrameRate_new),
                                         0, sizeof(t_ofeliaGetFrameRate),
                                         CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetFrameRate_class, reinterpret_cast<t_method>(ofeliaGetFrameRate_bang));
}

/* ________________________________________________________________________________
 * ofGetTargetFrameRate object methods
 */
void *ofeliaGetTargetFrameRate_new()
{
    t_ofeliaGetTargetFrameRate *x = reinterpret_cast<t_ofeliaGetTargetFrameRate*>(pd_new(ofeliaGetTargetFrameRate_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetTargetFrameRate_bang(t_ofeliaGetTargetFrameRate *x)
{
    t_float f;
    
    if (!value_getfloat(t_ofeliaGetTargetFrameRate::getTargetFrameRateSym, &f))
        outlet_float(x->x_obj.ob_outlet, f);
}

void ofeliaGetTargetFrameRate_setup()
{
    ofeliaGetTargetFrameRate_class = class_new(gensym("ofGetTargetFrameRate"),
                                               reinterpret_cast<t_newmethod>(ofeliaGetTargetFrameRate_new),
                                               0, sizeof(t_ofeliaGetTargetFrameRate),
                                               CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetTargetFrameRate_class, reinterpret_cast<t_method>(ofeliaGetTargetFrameRate_bang));
}

/* ________________________________________________________________________________
 * ofGetElapsedTime object methods
 */
void *ofeliaGetElapsedTime_new()
{
    t_ofeliaGetElapsedTime *x = reinterpret_cast<t_ofeliaGetElapsedTime*>(pd_new(ofeliaGetElapsedTime_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetElapsedTime_bang(t_ofeliaGetElapsedTime *x)
{
    t_float f;
    
    if (!value_getfloat(t_ofeliaGetElapsedTime::getElapsedTimeSym, &f))
        outlet_float(x->x_obj.ob_outlet, f);
}

void ofeliaGetElapsedTime_setup()
{
    ofeliaGetElapsedTime_class = class_new(gensym("ofGetElapsedTime"),
                                           reinterpret_cast<t_newmethod>(ofeliaGetElapsedTime_new),
                                           0, sizeof(t_ofeliaGetElapsedTime),
                                           CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetElapsedTime_class, reinterpret_cast<t_method>(ofeliaGetElapsedTime_bang));
}

/* ________________________________________________________________________________
 * ofGetLastFrameTime object methods
 */
void *ofeliaGetLastFrameTime_new()
{
    t_ofeliaGetLastFrameTime *x = reinterpret_cast<t_ofeliaGetLastFrameTime*>(pd_new(ofeliaGetLastFrameTime_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetLastFrameTime_bang(t_ofeliaGetLastFrameTime *x)
{
    t_float f;
    
    if (!value_getfloat(t_ofeliaGetLastFrameTime::getLastFrameTimeSym, &f))
        outlet_float(x->x_obj.ob_outlet, f);
}

void ofeliaGetLastFrameTime_setup()
{
    ofeliaGetLastFrameTime_class = class_new(gensym("ofGetLastFrameTime"),
                                             reinterpret_cast<t_newmethod>(ofeliaGetLastFrameTime_new),
                                             0, sizeof(t_ofeliaGetLastFrameTime),
                                             CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetLastFrameTime_class, reinterpret_cast<t_method>(ofeliaGetLastFrameTime_bang));
}

/* ________________________________________________________________________________
 * ofGetElapsedTimeMillis object methods
 */
void *ofeliaGetElapsedTimeMillis_new()
{
    t_ofeliaGetElapsedTimeMillis *x = reinterpret_cast<t_ofeliaGetElapsedTimeMillis*>(pd_new(ofeliaGetElapsedTimeMillis_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetElapsedTimeMillis_bang(t_ofeliaGetElapsedTimeMillis *x)
{
    t_float f;
    
    if (!value_getfloat(t_ofeliaGetElapsedTimeMillis::getElapsedTimeMillisSym, &f))
        outlet_float(x->x_obj.ob_outlet, f);
}

void ofeliaGetElapsedTimeMillis_setup()
{
    ofeliaGetElapsedTimeMillis_class = class_new(gensym("ofGetElapsedTimeMillis"),
                                                 reinterpret_cast<t_newmethod>(ofeliaGetElapsedTimeMillis_new),
                                                 0, sizeof(t_ofeliaGetElapsedTimeMillis),
                                                 CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetElapsedTimeMillis_class, reinterpret_cast<t_method>(ofeliaGetElapsedTimeMillis_bang));
}

/* ________________________________________________________________________________
 * ofGetLastFrameTimeMillis object methods
 */
void *ofeliaGetLastFrameTimeMillis_new()
{
    t_ofeliaGetLastFrameTimeMillis *x = reinterpret_cast<t_ofeliaGetLastFrameTimeMillis*>(pd_new(ofeliaGetLastFrameTimeMillis_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetLastFrameTimeMillis_bang(t_ofeliaGetLastFrameTimeMillis *x)
{
    t_float f;
    
    if (!value_getfloat(t_ofeliaGetLastFrameTimeMillis::getLastFrameTimeMillisSym, &f))
        outlet_float(x->x_obj.ob_outlet, f);
}

void ofeliaGetLastFrameTimeMillis_setup()
{
    ofeliaGetLastFrameTimeMillis_class = class_new(gensym("ofGetLastFrameTimeMillis"),
                                                   reinterpret_cast<t_newmethod>(ofeliaGetLastFrameTimeMillis_new),
                                                   0, sizeof(t_ofeliaGetLastFrameTimeMillis),
                                                   CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetLastFrameTimeMillis_class, reinterpret_cast<t_method>(ofeliaGetLastFrameTimeMillis_bang));
}

/* ________________________________________________________________________________
 * ofGetOrienLock object methods
 */
void *ofeliaGetOrienLock_new()
{
    t_ofeliaGetOrienLock *x = reinterpret_cast<t_ofeliaGetOrienLock*>(pd_new(ofeliaGetOrienLock_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetOrienLock_bang(t_ofeliaGetOrienLock *x)
{
    t_float f;
    
    if (!value_getfloat(t_ofeliaGetOrienLock::getOrienLockSym, &f))
        outlet_float(x->x_obj.ob_outlet, f);
}

void ofeliaGetOrienLock_setup()
{
    ofeliaGetOrienLock_class = class_new(gensym("ofGetOrienLock"),
                                         reinterpret_cast<t_newmethod>(ofeliaGetOrienLock_new),
                                         0, sizeof(t_ofeliaGetOrienLock),
                                         CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetOrienLock_class, reinterpret_cast<t_method>(ofeliaGetOrienLock_bang));
}

/* ________________________________________________________________________________
 * ofGetOrien object methods
 */
void *ofeliaGetOrien_new()
{
    t_ofeliaGetOrien *x = reinterpret_cast<t_ofeliaGetOrien*>(pd_new(ofeliaGetOrien_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetOrien_bang(t_ofeliaGetOrien *x)
{
    t_float f;
    
    if (!value_getfloat(t_ofeliaGetOrien::getOrienSym, &f))
        outlet_float(x->x_obj.ob_outlet, f);
}

void ofeliaGetOrien_setup()
{
    ofeliaGetOrien_class = class_new(gensym("ofGetOrien"),
                                     reinterpret_cast<t_newmethod>(ofeliaGetOrien_new),
                                     0, sizeof(t_ofeliaGetOrien),
                                     CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetOrien_class, reinterpret_cast<t_method>(ofeliaGetOrien_bang));
}

/* ________________________________________________________________________________
 * ofGetFullscreen object methods
 */
void *ofeliaGetFullscreen_new()
{
    t_ofeliaGetFullscreen *x = reinterpret_cast<t_ofeliaGetFullscreen*>(pd_new(ofeliaGetFullscreen_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetFullscreen_bang(t_ofeliaGetFullscreen *x)
{
    t_float f;
    
    if (!value_getfloat(t_ofeliaGetFullscreen::getFullscreenSym, &f))
        outlet_float(x->x_obj.ob_outlet, f);
}

void ofeliaGetFullscreen_setup()
{
    ofeliaGetFullscreen_class = class_new(gensym("ofGetFullscreen"),
                                          reinterpret_cast<t_newmethod>(ofeliaGetFullscreen_new),
                                          0, sizeof(t_ofeliaGetFullscreen),
                                          CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetFullscreen_class, reinterpret_cast<t_method>(ofeliaGetFullscreen_bang));
}

/* ________________________________________________________________________________
 * ofGetFocus object methods
 */
void *ofeliaGetFocus_new()
{
    t_ofeliaGetFocus *x = reinterpret_cast<t_ofeliaGetFocus*>(pd_new(ofeliaGetFocus_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetFocus_bang(t_ofeliaGetFocus *x)
{
    t_float f;
    
    if (!value_getfloat(t_ofeliaGetFocus::getFocusSym, &f))
        outlet_float(x->x_obj.ob_outlet, f);
}

void ofeliaGetFocus_setup()
{
    ofeliaGetFocus_class = class_new(gensym("ofGetFocus"),
                                     reinterpret_cast<t_newmethod>(ofeliaGetFocus_new),
                                     0, sizeof(t_ofeliaGetFocus),
                                     CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetFocus_class, reinterpret_cast<t_method>(ofeliaGetFocus_bang));
}

/* ________________________________________________________________________________
 * ofGetWindowPosX object methods
 */
void *ofeliaGetWindowPosX_new()
{
    t_ofeliaGetWindowPosX *x = reinterpret_cast<t_ofeliaGetWindowPosX*>(pd_new(ofeliaGetWindowPosX_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetWindowPosX_bang(t_ofeliaGetWindowPosX *x)
{
    t_float f;
    
    if (!value_getfloat(t_ofeliaGetWindowPosX::getWindowPosXSym, &f))
        outlet_float(x->x_obj.ob_outlet, f);
}

void ofeliaGetWindowPosX_setup()
{
    ofeliaGetWindowPosX_class = class_new(gensym("ofGetWindowPosX"),
                                     reinterpret_cast<t_newmethod>(ofeliaGetWindowPosX_new),
                                     0, sizeof(t_ofeliaGetWindowPosX),
                                     CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetWindowPosX_class, reinterpret_cast<t_method>(ofeliaGetWindowPosX_bang));
}

/* ________________________________________________________________________________
 * ofGetWindowPosY object methods
 */
void *ofeliaGetWindowPosY_new()
{
    t_ofeliaGetWindowPosY *x = reinterpret_cast<t_ofeliaGetWindowPosY*>(pd_new(ofeliaGetWindowPosY_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetWindowPosY_bang(t_ofeliaGetWindowPosY *x)
{
    t_float f;
    
    if (!value_getfloat(t_ofeliaGetWindowPosY::getWindowPosYSym, &f))
        outlet_float(x->x_obj.ob_outlet, f);
}

void ofeliaGetWindowPosY_setup()
{
    ofeliaGetWindowPosY_class = class_new(gensym("ofGetWindowPosY"),
                                     reinterpret_cast<t_newmethod>(ofeliaGetWindowPosY_new),
                                     0, sizeof(t_ofeliaGetWindowPosY),
                                     CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetWindowPosY_class, reinterpret_cast<t_method>(ofeliaGetWindowPosY_bang));
}

/* ________________________________________________________________________________
 * ofGetWindowPos object methods
 */
void *ofeliaGetWindowPos_new()
{
    t_ofeliaGetWindowPos *x = reinterpret_cast<t_ofeliaGetWindowPos*>(pd_new(ofeliaGetWindowPos_class));
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetWindowPos_bang(t_ofeliaGetWindowPos *x)
{
    t_float f_x, f_y;
    
    if (!value_getfloat(t_ofeliaGetWindowPosX::getWindowPosXSym, &f_x) &&
        !value_getfloat(t_ofeliaGetWindowPosY::getWindowPosYSym, &f_y)) {
        
        t_atom av[2];
        av[0].a_type = A_FLOAT;
        av[0].a_w.w_float = f_x;
        av[1].a_type = A_FLOAT;
        av[1].a_w.w_float = f_y;
        outlet_list(x->x_obj.ob_outlet, &s_list, 2, av);
    }
}

void ofeliaGetWindowPos_setup()
{
    ofeliaGetWindowPos_class = class_new(gensym("ofGetWindowPos"),
                                   reinterpret_cast<t_newmethod>(ofeliaGetWindowPos_new),
                                   0, sizeof(t_ofeliaGetWindowPos),
                                   CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetWindowPos_class, reinterpret_cast<t_method>(ofeliaGetWindowPos_bang));
}

/* ________________________________________________________________________________
 * ofGetScreenWidth object methods
 */
void *ofeliaGetScreenWidth_new()
{
    t_ofeliaGetScreenWidth *x = reinterpret_cast<t_ofeliaGetScreenWidth*>(pd_new(ofeliaGetScreenWidth_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetScreenWidth_bang(t_ofeliaGetScreenWidth *x)
{
    t_float f;
    
    if (!value_getfloat(t_ofeliaGetScreenWidth::getScreenWidthSym, &f))
        outlet_float(x->x_obj.ob_outlet, f);
}

void ofeliaGetScreenWidth_setup()
{
    ofeliaGetScreenWidth_class = class_new(gensym("ofGetScreenWidth"),
                                             reinterpret_cast<t_newmethod>(ofeliaGetScreenWidth_new),
                                             0, sizeof(t_ofeliaGetScreenWidth),
                                             CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetScreenWidth_class, reinterpret_cast<t_method>(ofeliaGetScreenWidth_bang));
}

/* ________________________________________________________________________________
 * ofGetScreenHeight object methods
 */
void *ofeliaGetScreenHeight_new()
{
    t_ofeliaGetScreenHeight *x = reinterpret_cast<t_ofeliaGetScreenHeight*>(pd_new(ofeliaGetScreenHeight_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetScreenHeight_bang(t_ofeliaGetScreenHeight *x)
{
    t_float f;
    
    if (!value_getfloat(t_ofeliaGetScreenHeight::getScreenHeightSym, &f))
        outlet_float(x->x_obj.ob_outlet, f);
}

void ofeliaGetScreenHeight_setup()
{
    ofeliaGetScreenHeight_class = class_new(gensym("ofGetScreenHeight"),
                                             reinterpret_cast<t_newmethod>(ofeliaGetScreenHeight_new),
                                             0, sizeof(t_ofeliaGetScreenHeight),
                                             CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetScreenHeight_class, reinterpret_cast<t_method>(ofeliaGetScreenHeight_bang));
}

/* ________________________________________________________________________________
 * ofGetScreenDimen object methods
 */
void *ofeliaGetScreenDimen_new()
{
    t_ofeliaGetScreenDimen *x = reinterpret_cast<t_ofeliaGetScreenDimen*>(pd_new(ofeliaGetScreenDimen_class));
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetScreenDimen_bang(t_ofeliaGetScreenDimen *x)
{
    t_float f_w, f_h;
    
    if (!value_getfloat(t_ofeliaGetScreenWidth::getScreenWidthSym, &f_w) &&
        !value_getfloat(t_ofeliaGetScreenHeight::getScreenHeightSym, &f_h)) {
        
        t_atom av[2];
        av[0].a_type = A_FLOAT;
        av[0].a_w.w_float = f_w;
        av[1].a_type = A_FLOAT;
        av[1].a_w.w_float = f_h;
        outlet_list(x->x_obj.ob_outlet, &s_list, 2, av);
    }
}

void ofeliaGetScreenDimen_setup()
{
    ofeliaGetScreenDimen_class = class_new(gensym("ofGetScreenDimen"),
                                           reinterpret_cast<t_newmethod>(ofeliaGetScreenDimen_new),
                                           0, sizeof(t_ofeliaGetScreenDimen),
                                           CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetScreenDimen_class, reinterpret_cast<t_method>(ofeliaGetScreenDimen_bang));
}

/* ________________________________________________________________________________
 * ofGetRetina object methods
 */
void *ofeliaGetRetina_new()
{
    t_ofeliaGetRetina *x = reinterpret_cast<t_ofeliaGetRetina*>(pd_new(ofeliaGetRetina_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetRetina_bang(t_ofeliaGetRetina *x)
{
    t_float f;
    
    if (!value_getfloat(t_ofeliaGetRetina::getRetinaSym, &f))
        outlet_float(x->x_obj.ob_outlet, f);
}

void ofeliaGetRetina_setup()
{
    ofeliaGetRetina_class = class_new(gensym("ofGetRetina"),
                                      reinterpret_cast<t_newmethod>(ofeliaGetRetina_new),
                                      0, sizeof(t_ofeliaGetRetina),
                                      CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetRetina_class, reinterpret_cast<t_method>(ofeliaGetRetina_bang));
}

/* ________________________________________________________________________________
 * ofGetBgColorR object methods
 */
void *ofeliaGetBgColorR_new()
{
    t_ofeliaGetBgColorR *x = reinterpret_cast<t_ofeliaGetBgColorR*>(pd_new(ofeliaGetBgColorR_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetBgColorR_bang(t_ofeliaGetBgColorR *x)
{
    t_float f;
    
    if (!value_getfloat(t_ofeliaGetBgColorR::getBgColorRSym, &f))
        outlet_float(x->x_obj.ob_outlet, f);
}

void ofeliaGetBgColorR_setup()
{
    ofeliaGetBgColorR_class = class_new(gensym("ofGetBgColorR"),
                                         reinterpret_cast<t_newmethod>(ofeliaGetBgColorR_new),
                                         0, sizeof(t_ofeliaGetBgColorR),
                                         CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetBgColorR_class, reinterpret_cast<t_method>(ofeliaGetBgColorR_bang));
}

/* ________________________________________________________________________________
 * ofGetBgColorG object methods
 */
void *ofeliaGetBgColorG_new()
{
    t_ofeliaGetBgColorG *x = reinterpret_cast<t_ofeliaGetBgColorG*>(pd_new(ofeliaGetBgColorG_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetBgColorG_bang(t_ofeliaGetBgColorG *x)
{
    t_float f;
    
    if (!value_getfloat(t_ofeliaGetBgColorG::getBgColorGSym, &f))
        outlet_float(x->x_obj.ob_outlet, f);
}

void ofeliaGetBgColorG_setup()
{
    ofeliaGetBgColorG_class = class_new(gensym("ofGetBgColorG"),
                                         reinterpret_cast<t_newmethod>(ofeliaGetBgColorG_new),
                                         0, sizeof(t_ofeliaGetBgColorG),
                                         CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetBgColorG_class, reinterpret_cast<t_method>(ofeliaGetBgColorG_bang));
}

/* ________________________________________________________________________________
 * ofGetBgColorB object methods
 */
void *ofeliaGetBgColorB_new()
{
    t_ofeliaGetBgColorB *x = reinterpret_cast<t_ofeliaGetBgColorB*>(pd_new(ofeliaGetBgColorB_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetBgColorB_bang(t_ofeliaGetBgColorB *x)
{
    t_float f;
    
    if (!value_getfloat(t_ofeliaGetBgColorB::getBgColorBSym, &f))
        outlet_float(x->x_obj.ob_outlet, f);
}

void ofeliaGetBgColorB_setup()
{
    ofeliaGetBgColorB_class = class_new(gensym("ofGetBgColorB"),
                                         reinterpret_cast<t_newmethod>(ofeliaGetBgColorB_new),
                                         0, sizeof(t_ofeliaGetBgColorB),
                                         CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetBgColorB_class, reinterpret_cast<t_method>(ofeliaGetBgColorB_bang));
}

/* ________________________________________________________________________________
 * ofGetBgColor object methods
 */
void *ofeliaGetBgColor_new()
{
    t_ofeliaGetBgColor *x = reinterpret_cast<t_ofeliaGetBgColor*>(pd_new(ofeliaGetBgColor_class));
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetBgColorBang(t_ofeliaGetBgColor *x)
{
    t_float f_r, f_g, f_b;
    
    if (!value_getfloat(t_ofeliaGetBgColorR::getBgColorRSym, &f_r) &&
        !value_getfloat(t_ofeliaGetBgColorG::getBgColorGSym, &f_g) &&
        !value_getfloat(t_ofeliaGetBgColorB::getBgColorBSym, &f_b)) {
        
        t_atom av[3];
        av[0].a_type = A_FLOAT;
        av[0].a_w.w_float = f_r;
        av[1].a_type = A_FLOAT;
        av[1].a_w.w_float = f_g;
        av[2].a_type = A_FLOAT;
        av[2].a_w.w_float = f_b;
        outlet_list(x->x_obj.ob_outlet, &s_list, 3, av);
    }
}

void ofeliaGetBgColor_setup()
{
    ofeliaGetBgColor_class = class_new(gensym("ofGetBgColor"),
                                       reinterpret_cast<t_newmethod>(ofeliaGetBgColor_new),
                                       0, sizeof(t_ofeliaGetBgColor),
                                       CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetBgColor_class, reinterpret_cast<t_method>(ofeliaGetBgColorBang));
}

/* ________________________________________________________________________________
 * ofGetWindow object methods
 */
void *ofeliaGetWindow_new()
{
    t_ofeliaGetWindow *x = reinterpret_cast<t_ofeliaGetWindow*>(pd_new(ofeliaGetWindow_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetWindow_bang(t_ofeliaGetWindow *x)
{
    t_float f;
    
    if (!value_getfloat(t_ofeliaGetWindow::getWindowSym, &f))
        outlet_float(x->x_obj.ob_outlet, f);
}

void ofeliaGetWindow_setup()
{
    ofeliaGetWindow_class = class_new(gensym("ofGetWindow"),
                                      reinterpret_cast<t_newmethod>(ofeliaGetWindow_new),
                                      0, sizeof(t_ofeliaGetWindow),
                                      CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetWindow_class, reinterpret_cast<t_method>(ofeliaGetWindow_bang));
}

/* ________________________________________________________________________________
 * ofGetFirstRenderOrder object methods
 */
void *ofeliaGetFirstRenderOrder_new(t_floatarg f)
{
    t_ofeliaGetFirstRenderOrder *x = reinterpret_cast<t_ofeliaGetFirstRenderOrder*>(pd_new(ofeliaGetFirstRenderOrder_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetFirstRenderOrder_bang(t_ofeliaGetFirstRenderOrder *x)
{
    sort(t_ofeliaHead::vec.begin(),t_ofeliaHead::vec.end());
    
    if (!t_ofeliaHead::vec.empty())
        outlet_float(x->x_obj.ob_outlet, t_ofeliaHead::vec.front().first);
    else
        outlet_float(x->x_obj.ob_outlet, OFELIA_DEFAULT_RENDER_ORDER);
}

void ofeliaGetFirstRenderOrder_setup()
{
    ofeliaGetFirstRenderOrder_class = class_new(gensym("ofGetFirstRenderOrder"),
                                                reinterpret_cast<t_newmethod>(ofeliaGetFirstRenderOrder_new),
                                                0, sizeof(t_ofeliaGetFirstRenderOrder),
                                                CLASS_DEFAULT, A_DEFFLOAT, 0);
    class_addbang(ofeliaGetFirstRenderOrder_class, reinterpret_cast<t_method>(ofeliaGetFirstRenderOrder_bang));
}

/* ________________________________________________________________________________
 * ofGetLastRenderOrder object methods
 */
void *ofeliaGetLastRenderOrder_new(t_floatarg f)
{
    t_ofeliaGetLastRenderOrder *x = reinterpret_cast<t_ofeliaGetLastRenderOrder*>(pd_new(ofeliaGetLastRenderOrder_class));
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetLastRenderOrder_bang(t_ofeliaGetLastRenderOrder *x)
{
    sort(t_ofeliaHead::vec.begin(),t_ofeliaHead::vec.end());
    
    if (!t_ofeliaHead::vec.empty())
        outlet_float(x->x_obj.ob_outlet, t_ofeliaHead::vec.back().first);
    else
        outlet_float(x->x_obj.ob_outlet, OFELIA_DEFAULT_RENDER_ORDER);
}

void ofeliaGetLastRenderOrder_setup()
{
    ofeliaGetLastRenderOrder_class = class_new(gensym("ofGetLastRenderOrder"),
                                               reinterpret_cast<t_newmethod>(ofeliaGetLastRenderOrder_new),
                                               0, sizeof(t_ofeliaGetLastRenderOrder),
                                               CLASS_DEFAULT, A_DEFFLOAT, 0);
    class_addbang(ofeliaGetLastRenderOrder_class, reinterpret_cast<t_method>(ofeliaGetLastRenderOrder_bang));
}

/* ________________________________________________________________________________
 * setup methods
 */
void ofeliaGetters_setup()
{
    t_ofeliaGetWidth::getWidthSym = gensym("ofGetWidth");
    t_ofeliaGetHeight::getHeightSym = gensym("ofGetHeight");
    t_ofeliaGetDimen::getDimenSym = gensym("{ofGetDimen}");
    t_ofeliaGetWindowScale::getWindowScaleSym = gensym("ofGetWindowScale");
    t_ofeliaGetFrameNum::getFrameNumSym = gensym("ofGetFrameNum");
    t_ofeliaGetFrameRate::getFrameRateSym = gensym("ofGetFrameRate");
    t_ofeliaGetTargetFrameRate::getTargetFrameRateSym = gensym("ofGetTargetFrameRate");
    t_ofeliaGetElapsedTime::getElapsedTimeSym = gensym("ofGetElapsedTime");
    t_ofeliaGetElapsedTimeMillis::getElapsedTimeMillisSym = gensym("ofGetElapsedTimeMillis");
    t_ofeliaGetLastFrameTime::getLastFrameTimeSym = gensym("ofGetLastFrameTime");
    t_ofeliaGetLastFrameTimeMillis::getLastFrameTimeMillisSym = gensym("ofGetLastFrameTimeMillis");
    t_ofeliaGetOrienLock::getOrienLockSym = gensym("ofGetOrienLock");
    t_ofeliaGetOrien::getOrienSym = gensym("ofGetOrien");
    t_ofeliaGetFullscreen::getFullscreenSym = gensym("ofGetFullscreen");
    t_ofeliaGetFocus::getFocusSym = gensym("ofGetFocus");
    t_ofeliaGetWindowPosX::getWindowPosXSym = gensym("ofGetWindowPosX");
    t_ofeliaGetWindowPosY::getWindowPosYSym = gensym("ofGetWindowPosY");
    t_ofeliaGetWindowPos::getWindowPosSym = gensym("{ofGetWindowPos}");
    t_ofeliaGetScreenWidth::getScreenWidthSym = gensym("ofGetScreenWidth");
    t_ofeliaGetScreenHeight::getScreenHeightSym = gensym("ofGetScreenHeight");
    t_ofeliaGetScreenDimen::getScreenDimenSym = gensym("{ofGetScreenDimen}");
    t_ofeliaGetRetina::getRetinaSym = gensym("ofGetRetina");
    t_ofeliaGetBgColorR::getBgColorRSym = gensym("ofGetBgColorR");
    t_ofeliaGetBgColorG::getBgColorGSym = gensym("ofGetBgColorG");
    t_ofeliaGetBgColorB::getBgColorBSym = gensym("ofGetBgColorB");
    t_ofeliaGetBgColor::getBgColorSym = gensym("{ofGetBgColor}");
    t_ofeliaGetWindow::getWindowSym = gensym("ofGetWindow");
    ofeliaGetWidth_setup();
    ofeliaGetHeight_setup();
    ofeliaGetDimen_setup();
    ofeliaGetWindowScale_setup();
    ofeliaGetFrameNum_setup();
    ofeliaGetFrameRate_setup();
    ofeliaGetTargetFrameRate_setup();
    ofeliaGetElapsedTime_setup();
    ofeliaGetElapsedTimeMillis_setup();
    ofeliaGetLastFrameTime_setup();
    ofeliaGetLastFrameTimeMillis_setup();
    ofeliaGetOrienLock_setup();
    ofeliaGetOrien_setup();
    ofeliaGetFullscreen_setup();
    ofeliaGetFocus_setup();
    ofeliaGetWindowPosX_setup();
    ofeliaGetWindowPosY_setup();
    ofeliaGetWindowPos_setup();
    ofeliaGetScreenWidth_setup();
    ofeliaGetScreenHeight_setup();
    ofeliaGetScreenDimen_setup();
    ofeliaGetRetina_setup();
    ofeliaGetBgColorR_setup();
    ofeliaGetBgColorG_setup();
    ofeliaGetBgColorB_setup();
    ofeliaGetBgColor_setup();
    ofeliaGetWindow_setup();
    ofeliaGetFirstRenderOrder_setup();
    ofeliaGetLastRenderOrder_setup();
}

