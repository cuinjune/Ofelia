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

#include "ofeliaListeners.h"
#include "ofeliaWindow.h"

/* ________________________________________________________________________________
 * static variables
 */
t_symbol *t_ofeliaTouchListener::touchListenerSym;
t_symbol *t_ofeliaMouseListener::mouseListenerSym;
t_symbol *t_ofeliaScrollListener::scrollListenerSym;
t_symbol *t_ofeliaKeyListener::keyListenerSym;
t_symbol *t_ofeliaKeyCodeListener::keyCodeListenerSym;
t_symbol *t_ofeliaAccelListener::accelListenerSym;
t_symbol *t_ofeliaWindowScaleListener::windowScaleListenerSym;
t_symbol *t_ofeliaOrienListener::orienListenerSym;
t_symbol *t_ofeliaFullscreenListener::fullscreenListenerSym;
t_symbol *t_ofeliaFocusListener::focusListenerSym;
t_symbol *t_ofeliaWindowPosListener::windowPosListenerSym;
t_symbol *t_ofeliaWindowListener::windowListenerSym;
t_symbol *t_ofeliaBackListener::backListenerSym;

/* ________________________________________________________________________________
 * ofTouchListener object methods
 */
void *ofeliaTouchListener_new()
{
    t_ofeliaTouchListener *x = reinterpret_cast<t_ofeliaTouchListener*>(pd_new(ofeliaTouchListener_class));
    pd_bind(&x->x_obj.ob_pd, t_ofeliaTouchListener::touchListenerSym);
    x->touchDown_out = outlet_new(&x->x_obj, &s_list);
    x->touchMoved_out = outlet_new(&x->x_obj, &s_list);
    x->touchUp_out = outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaTouchListener_list(t_ofeliaTouchListener *x, t_symbol *s, int argc, t_atom *argv)
{
    const int touchState = static_cast<int>(argv[0].a_w.w_float);
    
    switch (touchState) {
            
        case LISTENERS_TOUCH_DOWN:
            outlet_list(x->touchDown_out, &s_list, 3, argv+1);
            break;
            
        case LISTENERS_TOUCH_MOVED:
            outlet_list(x->touchMoved_out, &s_list, 3, argv+1);
            break;
            
        case LISTENERS_TOUCH_UP:
            outlet_list(x->touchUp_out, &s_list, 3, argv+1);
            break;
    }
}

void ofeliaTouchListener_free(t_ofeliaTouchListener *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaTouchListener::touchListenerSym);
    outlet_free(x->touchDown_out);
    outlet_free(x->touchMoved_out);
    outlet_free(x->touchUp_out);
}

void ofeliaTouchListener_setup()
{
    ofeliaTouchListener_class = class_new(gensym("ofTouchListener"),
                                          reinterpret_cast<t_newmethod>(ofeliaTouchListener_new),
                                          reinterpret_cast<t_method>(ofeliaTouchListener_free),
                                          sizeof(t_ofeliaTouchListener),
                                          CLASS_NOINLET, A_NULL, 0);
    class_addlist(ofeliaTouchListener_class, reinterpret_cast<t_method>(ofeliaTouchListener_list));
}

/* ________________________________________________________________________________
 * ofMouseListener object methods
 */
void *ofeliaMouseListener_new()
{
    t_ofeliaMouseListener *x = reinterpret_cast<t_ofeliaMouseListener*>(pd_new(ofeliaMouseListener_class));
    pd_bind(&x->x_obj.ob_pd, t_ofeliaMouseListener::mouseListenerSym);
    x->mousePressed_out = outlet_new(&x->x_obj, &s_list);
    x->mouseMoved_out = outlet_new(&x->x_obj, &s_list);
    x->mouseReleased_out = outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaMouseListener_list(t_ofeliaMouseListener *x, t_symbol *s, int argc, t_atom *argv)
{
    const int mouseState = static_cast<int>(argv[0].a_w.w_float);
    
    switch (mouseState) {
            
        case LISTENERS_MOUSE_PRESSED:
            outlet_list(x->mousePressed_out, &s_list, 3, argv+1);
            break;
            
        case LISTENERS_MOUSE_MOVED:
            outlet_list(x->mouseMoved_out, &s_list, 3, argv+1);
            break;
            
        case LISTENERS_MOUSE_RELEASED:
            outlet_list(x->mouseReleased_out, &s_list, 3, argv+1);
            break;
    }
}

void ofeliaMouseListener_free(t_ofeliaMouseListener *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaMouseListener::mouseListenerSym);
    outlet_free(x->mousePressed_out);
    outlet_free(x->mouseMoved_out);
    outlet_free(x->mouseReleased_out);
}

void ofeliaMouseListener_setup()
{
    ofeliaMouseListener_class = class_new(gensym("ofMouseListener"),
                                          reinterpret_cast<t_newmethod>(ofeliaMouseListener_new),
                                          reinterpret_cast<t_method>(ofeliaMouseListener_free),
                                          sizeof(t_ofeliaMouseListener),
                                          CLASS_NOINLET, A_NULL, 0);
    class_addlist(ofeliaMouseListener_class, reinterpret_cast<t_method>(ofeliaMouseListener_list));
}

/* ________________________________________________________________________________
 * ofScrollListener object methods
 */
void *ofeliaScrollListener_new()
{
    t_ofeliaScrollListener *x = reinterpret_cast<t_ofeliaScrollListener*>(pd_new(ofeliaScrollListener_class));
    pd_bind(&x->x_obj.ob_pd, t_ofeliaScrollListener::scrollListenerSym);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaScrollListener_list(t_ofeliaScrollListener *x, t_symbol *s, int argc, t_atom *argv)
{
    outlet_list(x->x_obj.ob_outlet, s, argc, argv);
}

void ofeliaScrollListener_free(t_ofeliaScrollListener *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaScrollListener::scrollListenerSym);
}

void ofeliaScrollListener_setup()
{
    ofeliaScrollListener_class = class_new(gensym("ofScrollListener"),
                                           reinterpret_cast<t_newmethod>(ofeliaScrollListener_new),
                                           reinterpret_cast<t_method>(ofeliaScrollListener_free),
                                           sizeof(t_ofeliaScrollListener),
                                           CLASS_NOINLET, A_NULL, 0);
    class_addlist(ofeliaScrollListener_class, reinterpret_cast<t_method>(ofeliaScrollListener_list));
}

/* ________________________________________________________________________________
 * ofKeyListener object methods
 */
void *ofeliaKeyListener_new()
{
    t_ofeliaKeyListener *x = reinterpret_cast<t_ofeliaKeyListener*>(pd_new(ofeliaKeyListener_class));
    pd_bind(&x->x_obj.ob_pd, t_ofeliaKeyListener::keyListenerSym);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaKeyListener_list(t_ofeliaKeyListener *x, t_symbol *s, int argc, t_atom *argv)
{
    outlet_list(x->x_obj.ob_outlet, s, argc, argv);
}

void ofeliaKeyListener_free(t_ofeliaKeyListener *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaKeyListener::keyListenerSym);
}

void ofeliaKeyListener_setup()
{
    ofeliaKeyListener_class = class_new(gensym("ofKeyListener"),
                                        reinterpret_cast<t_newmethod>(ofeliaKeyListener_new),
                                        reinterpret_cast<t_method>(ofeliaKeyListener_free),
                                        sizeof(t_ofeliaKeyListener),
                                        CLASS_NOINLET, A_NULL, 0);
    class_addlist(ofeliaKeyListener_class, reinterpret_cast<t_method>(ofeliaKeyListener_list));
}

/* ________________________________________________________________________________
 * ofKeyCodeListener object methods
 */
void *ofeliaKeyCodeListener_new()
{
    t_ofeliaKeyCodeListener *x = reinterpret_cast<t_ofeliaKeyCodeListener*>(pd_new(ofeliaKeyCodeListener_class));
    pd_bind(&x->x_obj.ob_pd, t_ofeliaKeyCodeListener::keyCodeListenerSym);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaKeyCodeListener_list(t_ofeliaKeyCodeListener *x, t_symbol *s, int argc, t_atom *argv)
{
    outlet_list(x->x_obj.ob_outlet, s, argc, argv);
}

void ofeliaKeyCodeListener_free(t_ofeliaKeyCodeListener *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaKeyCodeListener::keyCodeListenerSym);
}

void ofeliaKeyCodeListener_setup()
{
    ofeliaKeyCodeListener_class = class_new(gensym("ofKeyCodeListener"),
                                            reinterpret_cast<t_newmethod>(ofeliaKeyCodeListener_new),
                                            reinterpret_cast<t_method>(ofeliaKeyCodeListener_free),
                                            sizeof(t_ofeliaKeyCodeListener),
                                            CLASS_NOINLET, A_NULL, 0);
    class_addlist(ofeliaKeyCodeListener_class, reinterpret_cast<t_method>(ofeliaKeyCodeListener_list));
}

/* ________________________________________________________________________________
 * ofAccelListener object methods
 */
void *ofeliaAccelListener_new()
{
    t_ofeliaAccelListener *x = reinterpret_cast<t_ofeliaAccelListener*>(pd_new(ofeliaAccelListener_class));
    pd_bind(&x->x_obj.ob_pd, t_ofeliaAccelListener::accelListenerSym);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaAccelListener_list(t_ofeliaAccelListener *x, t_symbol *s, int argc, t_atom *argv)
{
    outlet_list(x->x_obj.ob_outlet, s, argc, argv);
}

void ofeliaAccelListener_free(t_ofeliaAccelListener *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaAccelListener::accelListenerSym);
}

void ofeliaAccelListener_setup()
{
    ofeliaAccelListener_class = class_new(gensym("ofAccelListener"),
                                          reinterpret_cast<t_newmethod>(ofeliaAccelListener_new),
                                          reinterpret_cast<t_method>(ofeliaAccelListener_free),
                                          sizeof(t_ofeliaAccelListener),
                                          CLASS_NOINLET, A_NULL, 0);
    class_addlist(ofeliaAccelListener_class, reinterpret_cast<t_method>(ofeliaAccelListener_list));
}

/* ________________________________________________________________________________
 * ofWindowScaleListener object methods
 */
void *ofeliaWindowScaleListener_new()
{
    t_ofeliaWindowScaleListener *x = reinterpret_cast<t_ofeliaWindowScaleListener*>(pd_new(ofeliaWindowScaleListener_class));
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindowScaleListener::windowScaleListenerSym);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaWindowScaleListener_float(t_ofeliaWindowScaleListener *x, t_floatarg f)
{
    outlet_float(x->x_obj.ob_outlet, f);
}

void ofeliaWindowScaleListener_free(t_ofeliaWindowScaleListener *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindowScaleListener::windowScaleListenerSym);
}

void ofeliaWindowScaleListener_setup()
{
    ofeliaWindowScaleListener_class = class_new(gensym("ofWindowScaleListener"),
                                          reinterpret_cast<t_newmethod>(ofeliaWindowScaleListener_new),
                                          reinterpret_cast<t_method>(ofeliaWindowScaleListener_free),
                                          sizeof(t_ofeliaWindowScaleListener),
                                          CLASS_NOINLET, A_NULL, 0);
    class_addfloat(ofeliaWindowScaleListener_class, reinterpret_cast<t_method>(ofeliaWindowScaleListener_float));
}

/* ________________________________________________________________________________
 * ofOrienListener object methods
 */
void *ofeliaOrienListener_new()
{
    t_ofeliaOrienListener *x = reinterpret_cast<t_ofeliaOrienListener*>(pd_new(ofeliaOrienListener_class));
    pd_bind(&x->x_obj.ob_pd, t_ofeliaOrienListener::orienListenerSym);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaOrienListener_float(t_ofeliaOrienListener *x, t_floatarg f)
{
    outlet_float(x->x_obj.ob_outlet, f);
}

void ofeliaOrienListener_free(t_ofeliaOrienListener *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaOrienListener::orienListenerSym);
}

void ofeliaOrienListener_setup()
{
    ofeliaOrienListener_class = class_new(gensym("ofOrienListener"),
                                          reinterpret_cast<t_newmethod>(ofeliaOrienListener_new),
                                          reinterpret_cast<t_method>(ofeliaOrienListener_free),
                                          sizeof(t_ofeliaOrienListener),
                                          CLASS_NOINLET, A_NULL, 0);
    class_addfloat(ofeliaOrienListener_class, reinterpret_cast<t_method>(ofeliaOrienListener_float));
}

/* ________________________________________________________________________________
 * ofFullscreenListener object methods
 */
void *ofeliaFullscreenListener_new()
{
    t_ofeliaFullscreenListener *x = reinterpret_cast<t_ofeliaFullscreenListener*>(pd_new(ofeliaFullscreenListener_class));
    pd_bind(&x->x_obj.ob_pd, t_ofeliaFullscreenListener::fullscreenListenerSym);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaFullscreenListener_float(t_ofeliaFullscreenListener *x, t_floatarg f)
{
    outlet_float(x->x_obj.ob_outlet, f);
}

void ofeliaFullscreenListener_free(t_ofeliaFullscreenListener *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaFullscreenListener::fullscreenListenerSym);
}

void ofeliaFullscreenListener_setup()
{
    ofeliaFullscreenListener_class = class_new(gensym("ofFullscreenListener"),
                                               reinterpret_cast<t_newmethod>(ofeliaFullscreenListener_new),
                                               reinterpret_cast<t_method>(ofeliaFullscreenListener_free),
                                               sizeof(t_ofeliaFullscreenListener),
                                               CLASS_NOINLET, A_NULL, 0);
    class_addfloat(ofeliaFullscreenListener_class, reinterpret_cast<t_method>(ofeliaFullscreenListener_float));
}

/* ________________________________________________________________________________
 * ofFocusListener object methods
 */
void *ofeliaFocusListener_new()
{
    t_ofeliaFocusListener *x = reinterpret_cast<t_ofeliaFocusListener*>(pd_new(ofeliaFocusListener_class));
    pd_bind(&x->x_obj.ob_pd, t_ofeliaFocusListener::focusListenerSym);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaFocusListener_float(t_ofeliaFocusListener *x, t_floatarg f)
{
    outlet_float(x->x_obj.ob_outlet, f);
}

void ofeliaFocusListener_free(t_ofeliaFocusListener *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaFocusListener::focusListenerSym);
}

void ofeliaFocusListener_setup()
{
    ofeliaFocusListener_class = class_new(gensym("ofFocusListener"),
                                          reinterpret_cast<t_newmethod>(ofeliaFocusListener_new),
                                          reinterpret_cast<t_method>(ofeliaFocusListener_free),
                                          sizeof(t_ofeliaFocusListener),
                                          CLASS_NOINLET, A_NULL, 0);
    class_addfloat(ofeliaFocusListener_class, reinterpret_cast<t_method>(ofeliaFocusListener_float));
}

/* ________________________________________________________________________________
 * ofWindowPosListener object methods
 */
void *ofeliaWindowPosListener_new()
{
    t_ofeliaWindowPosListener *x = reinterpret_cast<t_ofeliaWindowPosListener*>(pd_new(ofeliaWindowPosListener_class));
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindowPosListener::windowPosListenerSym);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaWindowPosListener_list(t_ofeliaWindowPosListener *x, t_symbol *s, int argc, t_atom *argv)
{
    outlet_list(x->x_obj.ob_outlet, s, argc, argv);
}

void ofeliaWindowPosListener_free(t_ofeliaWindowPosListener *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindowPosListener::windowPosListenerSym);
}

void ofeliaWindowPosListener_setup()
{
    ofeliaWindowPosListener_class = class_new(gensym("ofWindowPosListener"),
                                        reinterpret_cast<t_newmethod>(ofeliaWindowPosListener_new),
                                        reinterpret_cast<t_method>(ofeliaWindowPosListener_free),
                                        sizeof(t_ofeliaWindowPosListener),
                                        CLASS_NOINLET, A_NULL, 0);
    class_addlist(ofeliaWindowPosListener_class, reinterpret_cast<t_method>(ofeliaWindowPosListener_list));
}


/* ________________________________________________________________________________
 * ofWindowListener object methods
 */
void ofeliaWindowListener_windowExistsOut(t_ofeliaWindowListener *x);

void ofeliaWindowListener_init(t_ofeliaWindowListener *x)
{
    x->windowExistsClock = clock_new(x, reinterpret_cast<t_method>(ofeliaWindowListener_windowExistsOut));
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindowListener::windowListenerSym);
    outlet_new(&x->x_obj, &s_float);
    
    if (ofeliaWindow::bWindowExists && !ofeliaWindow::bFirstLoop)
        clock_delay(x->windowExistsClock, 0.0);
}

void *ofeliaWindowListener_new()
{
    t_ofeliaWindowListener *x = reinterpret_cast<t_ofeliaWindowListener*>(pd_new(ofeliaWindowListener_class));
    ofeliaWindowListener_init(x);
    return (x);
}

void ofeliaWindowListener_windowExistsOut(t_ofeliaWindowListener *x)
{
    outlet_float(x->x_obj.ob_outlet, 1.0f);
}

void ofeliaWindowListener_float(t_ofeliaWindowListener *x, t_floatarg f)
{
    outlet_float(x->x_obj.ob_outlet, f);
}

void ofeliaWindowListener_free(t_ofeliaWindowListener *x)
{
    clock_free(x->windowExistsClock);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindowListener::windowListenerSym);
}

void ofeliaWindowListener_setup()
{
    ofeliaWindowListener_class = class_new(gensym("ofWindowListener"),
                                           reinterpret_cast<t_newmethod>(ofeliaWindowListener_new),
                                           reinterpret_cast<t_method>(ofeliaWindowListener_free),
                                           sizeof(t_ofeliaWindowListener),
                                           CLASS_NOINLET, A_NULL, 0);
    class_addfloat(ofeliaWindowListener_class, reinterpret_cast<t_method>(ofeliaWindowListener_float));
}

/* ________________________________________________________________________________
 * ofWindowLoadBang object methods
 */
void ofeliaWindowLoadBang_windowExistsOut(t_ofeliaWindowLoadBang *x);

void ofeliaWindowLoadBang_init(t_ofeliaWindowLoadBang *x)
{
    x->windowExistsClock = clock_new(x, reinterpret_cast<t_method>(ofeliaWindowLoadBang_windowExistsOut));
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindowListener::windowListenerSym);
    outlet_new(&x->x_obj, &s_bang);
    
    if (ofeliaWindow::bWindowExists && !ofeliaWindow::bFirstLoop)
        clock_delay(x->windowExistsClock, 0.0);
}

void *ofeliaWindowLoadBang_new()
{
    t_ofeliaWindowLoadBang *x = reinterpret_cast<t_ofeliaWindowLoadBang*>(pd_new(ofeliaWindowLoadBang_class));
    ofeliaWindowLoadBang_init(x);
    return (x);
}

void ofeliaWindowLoadBang_windowExistsOut(t_ofeliaWindowLoadBang *x)
{
    outlet_bang(x->x_obj.ob_outlet);
}

void ofeliaWindowLoadBang_float(t_ofeliaWindowLoadBang *x, t_floatarg f)
{
    if (f != 0.0f)
        outlet_bang(x->x_obj.ob_outlet);
}

void ofeliaWindowLoadBang_free(t_ofeliaWindowLoadBang *x)
{
    clock_free(x->windowExistsClock);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindowListener::windowListenerSym);
}

void ofeliaWindowLoadBang_setup()
{
    ofeliaWindowLoadBang_class = class_new(gensym("ofWindowLoadBang"),
                                           reinterpret_cast<t_newmethod>(ofeliaWindowLoadBang_new),
                                           reinterpret_cast<t_method>(ofeliaWindowLoadBang_free),
                                           sizeof(t_ofeliaWindowLoadBang),
                                           CLASS_NOINLET, A_NULL, 0);
    class_addfloat(ofeliaWindowLoadBang_class, reinterpret_cast<t_method>(ofeliaWindowLoadBang_float));
}

/* ________________________________________________________________________________
 * ofWindowCloseBang object methods
 */
void ofeliaWindowCloseBang_init(t_ofeliaWindowCloseBang *x)
{
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindowListener::windowListenerSym);
    outlet_new(&x->x_obj, &s_bang);
}

void *ofeliaWindowCloseBang_new()
{
    t_ofeliaWindowCloseBang *x = reinterpret_cast<t_ofeliaWindowCloseBang*>(pd_new(ofeliaWindowCloseBang_class));
    ofeliaWindowCloseBang_init(x);
    return (x);
}

void ofeliaWindowCloseBang_float(t_ofeliaWindowCloseBang *x, t_floatarg f)
{
    if (f == 0.0f)
        outlet_bang(x->x_obj.ob_outlet);
}

void ofeliaWindowCloseBang_free(t_ofeliaWindowCloseBang *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindowListener::windowListenerSym);
}

void ofeliaWindowCloseBang_setup()
{
    ofeliaWindowCloseBang_class = class_new(gensym("ofWindowCloseBang"),
                                            reinterpret_cast<t_newmethod>(ofeliaWindowCloseBang_new),
                                            reinterpret_cast<t_method>(ofeliaWindowCloseBang_free),
                                            sizeof(t_ofeliaWindowCloseBang),
                                            CLASS_NOINLET, A_NULL, 0);
    class_addfloat(ofeliaWindowCloseBang_class, reinterpret_cast<t_method>(ofeliaWindowCloseBang_float));
}

/* ________________________________________________________________________________
 * ofBackListener object methods
 */
void *ofeliaBackListener_new()
{
    t_ofeliaBackListener *x = reinterpret_cast<t_ofeliaBackListener*>(pd_new(ofeliaBackListener_class));
    pd_bind(&x->x_obj.ob_pd, t_ofeliaBackListener::backListenerSym);
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaBackListener_bang(t_ofeliaBackListener *x)
{
    outlet_bang(x->x_obj.ob_outlet);
}

void ofeliaBackListener_free(t_ofeliaBackListener *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaBackListener::backListenerSym);
}

void ofeliaBackListener_setup()
{
    ofeliaBackListener_class = class_new(gensym("ofBackListener"),
                                         reinterpret_cast<t_newmethod>(ofeliaBackListener_new),
                                         reinterpret_cast<t_method>(ofeliaBackListener_free),
                                         sizeof(t_ofeliaBackListener),
                                         CLASS_NOINLET, A_NULL, 0);
    class_addbang(ofeliaBackListener_class, reinterpret_cast<t_method>(ofeliaBackListener_bang));
}

/* ________________________________________________________________________________
 * setup methods
 */
void ofeliaListeners_setup()
{
    t_ofeliaTouchListener::touchListenerSym = gensym("{ofTouchListener}");
    t_ofeliaMouseListener::mouseListenerSym = gensym("{ofMouseListener}");
    t_ofeliaScrollListener::scrollListenerSym = gensym("{ofScrollListener}");
    t_ofeliaKeyListener::keyListenerSym = gensym("{ofKeyListener}");
    t_ofeliaKeyCodeListener::keyCodeListenerSym = gensym("{ofKeyCodeListener}");
    t_ofeliaAccelListener::accelListenerSym = gensym("{ofAccelListener}");
    t_ofeliaWindowScaleListener::windowScaleListenerSym = gensym("{ofWindowScaleListener}");
    t_ofeliaOrienListener::orienListenerSym = gensym("{ofOrienListener}");
    t_ofeliaFullscreenListener::fullscreenListenerSym = gensym("{ofFullscreenListener}");
    t_ofeliaFocusListener::focusListenerSym = gensym("{ofFocusListener}");
    t_ofeliaWindowPosListener::windowPosListenerSym = gensym("{ofWindowPosListener}");
    t_ofeliaWindowListener::windowListenerSym = gensym("{ofWindowListener}");
    t_ofeliaBackListener::backListenerSym = gensym("{ofBackListener}");
    ofeliaTouchListener_setup();
    ofeliaMouseListener_setup();
    ofeliaScrollListener_setup();
    ofeliaKeyListener_setup();
    ofeliaKeyCodeListener_setup();
    ofeliaAccelListener_setup();
    ofeliaWindowScaleListener_setup();
    ofeliaOrienListener_setup();
    ofeliaFullscreenListener_setup();
    ofeliaFocusListener_setup();
    ofeliaWindowPosListener_setup();
    ofeliaWindowListener_setup();
    ofeliaWindowLoadBang_setup();
    ofeliaWindowCloseBang_setup();
    ofeliaBackListener_setup();
}

