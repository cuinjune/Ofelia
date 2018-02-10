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

#ifndef ofeliaDevices_h
#define ofeliaDevices_h

#pragma once

#include "ofeliaBase.h"
#include "ofApp.h"

extern "C" {
    
    /* ofGetAudioDevices object class */
    static t_class *ofeliaGetAudioDevices_class;
    
    typedef struct _ofeliaGetAudioDevices
    {
        t_object x_obj;
        ofApp *app;
        t_outlet *inDevices_out, *outDevices_out;
        
    } t_ofeliaGetAudioDevices;
    
    /* ofSetAudioDevices object class */
    static t_class *ofeliaSetAudioDevices_class;
    
    typedef struct _ofeliaSetAudioDevices
    {
        t_object x_obj;
        ofApp *app;
        int inputDeviceID;
        int numInputChannels;
        int outputDeviceID;
        int numOutputChannels;
        int sampleRate;
        int blockSize;
        
    } t_ofeliaSetAudioDevices;
    
    /* ofGetMidiDevices object class */
    static t_class *ofeliaGetMidiDevices_class;
    
    typedef struct _ofeliaGetMidiDevices
    {
        t_object x_obj;
        ofApp *app;
        t_outlet *inDevices_out, *outDevices_out;
        
    } t_ofeliaGetMidiDevices;
    
    /* ofSetMidiDevices object class */
    static t_class *ofeliaSetMidiDevices_class;
    
    typedef struct _ofeliaSetMidiDevices
    {
        t_object x_obj;
        ofApp *app;
        int midiInDevices[9];
        int midiOutDevices[9];
        
    } t_ofeliaSetMidiDevices;
    
    /* setup method */
    void ofeliaDevices_setup();
}

#endif /* ofeliaDevices_h */
