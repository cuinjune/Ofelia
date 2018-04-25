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

#ifndef ofeliaListeners_h
#define ofeliaListeners_h

#pragma once

#include "ofeliaBase.h"

extern "C" {
    
    /* ofTouchListener object class */
    static t_class *ofeliaTouchListener_class;
    
    enum TouchState {
        
        LISTENERS_TOUCH_DOWN,
        LISTENERS_TOUCH_MOVED,
        LISTENERS_TOUCH_UP
    };
    enum MouseState {
        
        LISTENERS_MOUSE_PRESSED,
        LISTENERS_MOUSE_MOVED,
        LISTENERS_MOUSE_RELEASED
    };
    typedef struct _ofeliaTouchListener
    {
        t_object x_obj;
        static t_symbol *touchListenerSym;
        t_outlet *touchDown_out, *touchMoved_out, *touchUp_out;
        
    } t_ofeliaTouchListener;
    
    /* ofMouseListener object class */
    static t_class *ofeliaMouseListener_class;
    
    typedef struct _ofeliaMouseListener
    {
        t_object x_obj;
        static t_symbol *mouseListenerSym;
        t_outlet *mousePressed_out, *mouseMoved_out, *mouseReleased_out;
        
    } t_ofeliaMouseListener;
    
    /* ofScrollListener object class */
    static t_class *ofeliaScrollListener_class;
    
    typedef struct _ofeliaScrollListener
    {
        t_object x_obj;
        static t_symbol *scrollListenerSym;
        
    } t_ofeliaScrollListener;
    
    /* ofKeyListener object class */
    static t_class *ofeliaKeyListener_class;
    
    typedef struct _ofeliaKeyListener
    {
        t_object x_obj;
        static t_symbol *keyListenerSym;
        
    } t_ofeliaKeyListener;
    
    /* ofKeyCodeListener object class */
    static t_class *ofeliaKeyCodeListener_class;
    
    typedef struct _ofeliaKeyCodeListener
    {
        t_object x_obj;
        static t_symbol *keyCodeListenerSym;
        
    } t_ofeliaKeyCodeListener;
    
    /* ofAccelListener object class */
    static t_class *ofeliaAccelListener_class;
    
    typedef struct _ofeliaAccelListener
    {
        t_object x_obj;
        static t_symbol *accelListenerSym;
        
    } t_ofeliaAccelListener;
    
    /* ofWindowScaleListener object class */
    static t_class *ofeliaWindowScaleListener_class;
    
    typedef struct _ofeliaWindowScaleListener
    {
        t_object x_obj;
        static t_symbol *windowScaleListenerSym;
        
    } t_ofeliaWindowScaleListener;
    
    /* ofOrienListener object class */
    static t_class *ofeliaOrienListener_class;
    
    typedef struct _ofeliaOrienListener
    {
        t_object x_obj;
        static t_symbol *orienListenerSym;
        
    } t_ofeliaOrienListener;
    
    /* ofFullscreenListener object class */
    static t_class *ofeliaFullscreenListener_class;
    
    typedef struct _ofeliaFullscreenListener
    {
        t_object x_obj;
        static t_symbol *fullscreenListenerSym;
        
    } t_ofeliaFullscreenListener;
    
    /* ofFocusListener object class */
    static t_class *ofeliaFocusListener_class;
    
    typedef struct _ofeliaFocusListener
    {
        t_object x_obj;
        static t_symbol *focusListenerSym;
        
    } t_ofeliaFocusListener;
    
    /* ofWindowPosListener object class */
    static t_class *ofeliaWindowPosListener_class;
    
    typedef struct _ofeliaWindowPosListener
    {
        t_object x_obj;
        static t_symbol *windowPosListenerSym;
        
    } t_ofeliaWindowPosListener;
    
    /* ofWindowListener object class */
    static t_class *ofeliaWindowListener_class;
    
    typedef struct _ofeliaWindowListener
    {
        t_object x_obj;
        static t_symbol *windowListenerSym;
        t_clock *windowExistsClock;
        
    } t_ofeliaWindowListener;
    
    /* ofWindowLoadBang object class */
    static t_class *ofeliaWindowLoadBang_class;
    
    typedef struct _ofeliaWindowLoadBang
    {
        t_object x_obj;
        t_clock *windowExistsClock;
        
    } t_ofeliaWindowLoadBang;
    
    /* ofWindowCloseBang object class */
    static t_class *ofeliaWindowCloseBang_class;
    
    typedef struct _ofeliaWindowCloseBang
    {
        t_object x_obj;
        
    } t_ofeliaWindowCloseBang;
    
    /* ofBackListener object class */
    static t_class *ofeliaBackListener_class;
    
    typedef struct _ofeliaBackListener
    {
        t_object x_obj;
        static t_symbol *backListenerSym;
        
    } t_ofeliaBackListener;
    
    /* setup methods */
    void ofeliaListeners_setup();
}

#endif /* ofeliaListeners_h */
