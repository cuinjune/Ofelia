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

#include "ofeliaDevices.h"

/* ________________________________________________________________________________
 * ofGetAudioDevices object methods
 */
void *ofeliaGetAudioDevices_new()
{
    t_ofeliaGetAudioDevices *x = reinterpret_cast<t_ofeliaGetAudioDevices *>(pd_new(ofeliaGetAudioDevices_class));
    x->app = static_cast<ofApp *>(ofGetAppPtr());
    x->inDevices_out = outlet_new(&x->x_obj, &s_list);
    x->outDevices_out = outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetAudioDevices_bang(t_ofeliaGetAudioDevices *x)
{
    /* separate input/output device list */
    vector<ofSoundDevice> inputDeviceList, outputDeviceList;
    x->app->getInputOutputAudioDeviceList(inputDeviceList, outputDeviceList);
    
    /* outlet audioOut devices */
    const int acout = static_cast<int>(outputDeviceList.size());
    t_atom *avout = (t_atom *)getbytes(acout * sizeof(t_atom));
    
    for (int i=0; i<acout; ++i) {
        
        avout[i].a_type = A_SYMBOL;
        t_string name = outputDeviceList[i].name.c_str();
        const size_t pos = name.rfind(':');
        
        if (pos != t_string::npos)
            name.erase(0, pos+2); //erase company name
        avout[i].a_w.w_symbol = gensym(name.c_str());
    }
    outlet_list(x->outDevices_out, &s_list, acout, avout);
    freebytes(avout, acout * sizeof(t_atom));
    
    /* outlet audioIn devices */
    const int acin = static_cast<int>(inputDeviceList.size());
    t_atom *avin = (t_atom *)getbytes(acin * sizeof(t_atom));
    
    for (int i=0; i<acin; ++i) {
        
        avin[i].a_type = A_SYMBOL;
        t_string name = inputDeviceList[i].name.c_str();
        const size_t pos = name.rfind(':');
        
        if (pos != t_string::npos)
            name.erase(0, pos+2); //erase company name
        avin[i].a_w.w_symbol = gensym(name.c_str());
    }
    outlet_list(x->inDevices_out, &s_list, acin, avin);
    freebytes(avin, acin * sizeof(t_atom));
}

void ofeliaGetAudioDevices_free(t_ofeliaGetAudioDevices *x)
{
    outlet_free(x->inDevices_out);
    outlet_free(x->outDevices_out);
}

void ofeliaGetAudioDevices_setup()
{
    ofeliaGetAudioDevices_class = class_new(gensym("ofGetAudioDevices"),
                                            reinterpret_cast<t_newmethod>(ofeliaGetAudioDevices_new),
                                            reinterpret_cast<t_method>(ofeliaGetAudioDevices_free),
                                            sizeof(t_ofeliaGetAudioDevices),
                                            CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetAudioDevices_class, reinterpret_cast<t_method>(ofeliaGetAudioDevices_bang));
}

/* ________________________________________________________________________________
 * ofSetAudioDevices object methods
 */
void *ofeliaSetAudioDevices_new()
{
    t_ofeliaSetAudioDevices *x = (t_ofeliaSetAudioDevices *)pd_new(ofeliaSetAudioDevices_class);
    x->app = static_cast<ofApp *>(ofGetAppPtr());
    x->inputDeviceID = x->app->inputDeviceID;
    x->numInputChannels = x->app->numInputChannels;
    x->outputDeviceID = x->app->outputDeviceID;
    x->numOutputChannels = x->app->numOutputChannels;
    x->sampleRate = x->app->sampleRate;
    x->blockSize = ofxPd::blockSize() * x->app->ticksPerBuffer;
    return (x);
}

void ofeliaSetAudioDevices_bang(t_ofeliaSetAudioDevices *x)
{
    x->app->inputDeviceID = x->inputDeviceID;
    x->app->numInputChannels = x->numInputChannels;
    x->app->outputDeviceID = x->outputDeviceID;
    x->app->numOutputChannels = x->numOutputChannels;
    x->app->sampleRate = x->sampleRate;
    x->app->ticksPerBuffer = static_cast<int>(x->blockSize / ofxPd::blockSize());
    x->app->shouldReinitAudio = true;
}

void ofeliaSetAudioDevices_input(t_ofeliaSetAudioDevices *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc == 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            const vector<ofSoundDevice> &deviceList = x->app->getDeviceList();
            vector<ofSoundDevice> inputDeviceList;
            
            for (size_t i=0; i<deviceList.size(); ++i) {
                
                if (deviceList[i].inputChannels)
                    inputDeviceList.push_back(deviceList[i]);
            }
            x->inputDeviceID = getClampIntValue(static_cast<int>(argv[0].a_w.w_float),
                                                0, static_cast<int>(inputDeviceList.size()-1));
            x->numInputChannels = getClampIntValue(static_cast<int>(argv[1].a_w.w_float),
                                                   0, 64);
        }
        else if (argv[0].a_type == A_SYMBOL &&
                 argv[1].a_type == A_FLOAT) {
            
            const t_string &name = argv[0].a_w.w_symbol->s_name;
            const vector<ofSoundDevice> &deviceList = x->app->getDeviceList();
            vector<ofSoundDevice> inputDeviceList;
            int inputDeviceID = -1;
            
            for (size_t i=0; i<deviceList.size(); ++i) {
                
                if (deviceList[i].inputChannels)
                    inputDeviceList.push_back(deviceList[i]);
            }
            for (size_t i=0; i<inputDeviceList.size(); ++i) {
                
                if (endsWith(inputDeviceList[i].name.c_str(), name.c_str())) {
                    
                    inputDeviceID = static_cast<int>(i);
                    break;
                }
            }
            if (inputDeviceID < 0) {
                
                error("ofSetAudioDevices: couldn't find audio input device: %s", name.c_str());
                return;
            }
            x->inputDeviceID = inputDeviceID;
            x->numInputChannels = getClampIntValue(static_cast<int>(argv[1].a_w.w_float),
                                                   0, 64);
        }
        else {
            
            error("ofSetAudioDevices: wrong argument type");
        }
    }
    else {
        
        error("ofSetAudioDevices: invalid number of arguments");
    }
}

void ofeliaSetAudioDevices_output(t_ofeliaSetAudioDevices *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc == 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            const vector<ofSoundDevice> &deviceList = x->app->getDeviceList();
            vector<ofSoundDevice> outputDeviceList;
            
            for (size_t i=0; i<deviceList.size(); ++i) {
                
                if (deviceList[i].outputChannels)
                    outputDeviceList.push_back(deviceList[i]);
            }
            x->outputDeviceID = getClampIntValue(static_cast<int>(argv[0].a_w.w_float),
                                                 0, static_cast<int>(outputDeviceList.size()-1));
            x->numOutputChannels = getClampIntValue(static_cast<int>(argv[1].a_w.w_float),
                                                    0, 64);
        }
        else if (argv[0].a_type == A_SYMBOL &&
                 argv[1].a_type == A_FLOAT) {
            
            const t_string &name = argv[0].a_w.w_symbol->s_name;
            const vector<ofSoundDevice> &deviceList = x->app->getDeviceList();
            vector<ofSoundDevice> outputDeviceList;
            int outputDeviceID = -1;
            
            for (size_t i=0; i<deviceList.size(); ++i) {
                
                if (deviceList[i].outputChannels)
                    outputDeviceList.push_back(deviceList[i]);
            }
            for (size_t i=0; i<outputDeviceList.size(); ++i) {
                
                if (endsWith(outputDeviceList[i].name.c_str(), name.c_str())) {
                    
                    outputDeviceID = static_cast<int>(i);
                    break;
                }
            }
            if (outputDeviceID < 0) {
                
                error("ofSetAudioDevices: couldn't find audio output device: %s", name.c_str());
                return;
            }
            x->outputDeviceID = outputDeviceID;
            x->numOutputChannels = getClampIntValue(static_cast<int>(argv[1].a_w.w_float),
                                                    0, 64);
        }
        else {
            
            error("ofSetAudioDevices: wrong argument type");
        }
    }
    else {
        
        error("ofSetAudioDevices: invalid number of arguments");
    }
}

void ofeliaSetAudioDevices_sampleRate(t_ofeliaSetAudioDevices *x, t_floatarg f)
{
    const int sampleRate = static_cast<int>(f);
    
    /* available sampleRates may differ depending on devices & platforms */
    if (sampleRate == 22050 ||
        sampleRate == 44100 ||
        sampleRate == 48000 ||
        sampleRate == 88200 ||
        sampleRate == 96000) {
        
        x->sampleRate = sampleRate;
    }
    else {
        
        error("ofSetAudioDevices: invalid sampleRate");
    }
}

void ofeliaSetAudioDevices_blockSize(t_ofeliaSetAudioDevices *x, t_floatarg f)
{
    const int blockSize = static_cast<int>(f);
    
    if (blockSize == 64 ||
        blockSize == 128 ||
        blockSize == 256 ||
        blockSize == 512 ||
        blockSize == 1024 ||
        blockSize == 2048) {
        
        x->blockSize = blockSize;
    }
    else {
        
        error("ofSetAudioDevices: invalid blockSize");
    }
}

void ofeliaSetAudioDevices_print(t_ofeliaSetAudioDevices *x)
{
    post("\n[ofSetAudioDevices]");
    vector<ofSoundDevice> inputDeviceList, outputDeviceList;
    x->app->getInputOutputAudioDeviceList(inputDeviceList, outputDeviceList);
    t_string inputDeviceName = inputDeviceList[static_cast<size_t>(x->inputDeviceID)].name.c_str();
    const size_t inputDevicePos = inputDeviceName.rfind(':');
    t_string outputDeviceName = outputDeviceList[static_cast<size_t>(x->outputDeviceID)].name.c_str();
    const size_t outputDevicePos = outputDeviceName.rfind(':');
    
    /* remove company names if found */
    if (inputDevicePos != t_string::npos)
        inputDeviceName.erase(0, inputDevicePos+2);
    if (outputDevicePos != t_string::npos)
        outputDeviceName.erase(0, outputDevicePos+2);
    post("input : %s %d", inputDeviceName.c_str(), x->numInputChannels);
    post("output : %s %d", outputDeviceName.c_str(), x->numOutputChannels);
    post("sampleRate : %d", x->sampleRate);
    post("blockSize : %d", x->blockSize);
}

void ofeliaSetAudioDevices_setup()
{
    ofeliaSetAudioDevices_class = class_new(gensym("ofSetAudioDevices"),
                                            reinterpret_cast<t_newmethod>(ofeliaSetAudioDevices_new),
                                            0, sizeof(t_ofeliaSetAudioDevices),
                                            CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaSetAudioDevices_class, reinterpret_cast<t_method>(ofeliaSetAudioDevices_bang));
    class_addmethod(ofeliaSetAudioDevices_class, reinterpret_cast<t_method>(ofeliaSetAudioDevices_input), gensym("input"), A_GIMME, 0);
    class_addmethod(ofeliaSetAudioDevices_class, reinterpret_cast<t_method>(ofeliaSetAudioDevices_output), gensym("output"), A_GIMME, 0);
    class_addmethod(ofeliaSetAudioDevices_class, reinterpret_cast<t_method>(ofeliaSetAudioDevices_sampleRate), gensym("sampleRate"), A_FLOAT, 0);
    class_addmethod(ofeliaSetAudioDevices_class, reinterpret_cast<t_method>(ofeliaSetAudioDevices_blockSize), gensym("blockSize"), A_FLOAT, 0);
    class_addmethod(ofeliaSetAudioDevices_class, reinterpret_cast<t_method>(ofeliaSetAudioDevices_print), gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetMidiDevices object methods
 */
void *ofeliaGetMidiDevices_new()
{
    t_ofeliaGetMidiDevices *x = reinterpret_cast<t_ofeliaGetMidiDevices *>(pd_new(ofeliaGetMidiDevices_class));
    x->app = static_cast<ofApp *>(ofGetAppPtr());
    x->inDevices_out = outlet_new(&x->x_obj, &s_list);
    x->outDevices_out = outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetMidiDevices_bang(t_ofeliaGetMidiDevices *x)
{
    /* outlet midiOut devices */
    const int acout = ofxMidiOut::getNumPorts();
    t_atom *avout = (t_atom *)getbytes(acout * sizeof(t_atom));
    
    for (int i=0; i<acout; ++i) {
        
        avout[i].a_type = A_SYMBOL;
        avout[i].a_w.w_symbol = gensym(ofxMidiOut::getPortName(i).c_str());
    }
    outlet_list(x->outDevices_out, &s_list, acout, avout);
    freebytes(avout, acout * sizeof(t_atom));
    
    /* outlet midiIn devices */
    const int acin = ofxMidiIn::getNumPorts();
    t_atom *avin = (t_atom *)getbytes(acin * sizeof(t_atom));
    
    for (int i=0; i<acin; ++i) {
        
        avin[i].a_type = A_SYMBOL;
        avin[i].a_w.w_symbol = gensym(ofxMidiIn::getPortName(i).c_str());
    }
    outlet_list(x->inDevices_out, &s_list, acin, avin);
    freebytes(avin, acin * sizeof(t_atom));
}

void ofeliaGetMidiDevices_free(t_ofeliaGetMidiDevices *x)
{
    outlet_free(x->inDevices_out);
    outlet_free(x->outDevices_out);
}

void ofeliaGetMidiDevices_setup()
{
    ofeliaGetMidiDevices_class = class_new(gensym("ofGetMidiDevices"),
                                           reinterpret_cast<t_newmethod>(ofeliaGetMidiDevices_new),
                                           reinterpret_cast<t_method>(ofeliaGetMidiDevices_free),
                                           sizeof(t_ofeliaGetMidiDevices),
                                           CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaGetMidiDevices_class, reinterpret_cast<t_method>(ofeliaGetMidiDevices_bang));
}

/* ________________________________________________________________________________
 * ofSetMidiDevices object methods
 */
void *ofeliaSetMidiDevices_new()
{
    t_ofeliaSetMidiDevices *x = reinterpret_cast<t_ofeliaSetMidiDevices *>(pd_new(ofeliaSetMidiDevices_class));
    x->app = static_cast<ofApp *>(ofGetAppPtr());
    fill(x->midiInDevices, x->midiInDevices+9, 0);
    fill(x->midiOutDevices, x->midiOutDevices+9, 0);
    return (x);
}

void ofeliaSetMidiDevices_bang(t_ofeliaSetMidiDevices *x)
{
    for (int i=0; i<9; ++i) {
        
        x->app->midiIns[i].closePort();
        x->app->midiOuts[i].closePort();
    }
    for (int i=0; i<9; ++i) {
        
        const int indevno = x->midiInDevices[i];
        const int outdevno = x->midiOutDevices[i];
        
        if (indevno > 0)
            x->app->midiIns[i].openPort(indevno-1);
        
        if (outdevno > 0)
            x->app->midiOuts[i].openPort(outdevno-1);
    }
}

void ofeliaSetMidiDevices_input(t_ofeliaSetMidiDevices *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc > 0 && argc <= 9) {
        
        int inDevices[9] = {0,0,0,0,0,0,0,0,0};
        
        for (int i=0; i<argc; ++i) {
            
            if (argv[i].a_type == A_FLOAT) {
                
                inDevices[i] = getClampIntValue(static_cast<int>(argv[i].a_w.w_float), 0,
                                                ofxMidiIn::getNumPorts());
            }
            else if (argv[i].a_type == A_SYMBOL) {
                
                const t_string &name = argv[i].a_w.w_symbol->s_name;
                const size_t len = ofxMidiIn::getPortList().size();
                vector<t_string> portList(len);
                
                for (size_t i=0; i<len; ++i)
                    portList[i] = ofxMidiIn::getPortList()[i].c_str();
                const ptrdiff_t pos = find(portList.begin(), portList.end(), name) - portList.begin();
                
                if (pos < portList.size()) {
                    
                    inDevices[i] = static_cast<int>(pos) + 1;
                }
                else {
                    
                    error("ofSetMidiDevices: couldn't find midi input device: %s", name.c_str());
                    return;
                }
            }
            else {
                
                error("ofSetMidiDevices: wrong argument type");
                return;
            }
        }
        for (int i=0; i<9; ++i)
            x->midiInDevices[i] = inDevices[i];
    }
    else {
        
        error("ofSetMidiDevices: invalid number of arguments");
    }
}

void ofeliaSetMidiDevices_output(t_ofeliaSetMidiDevices *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc > 0 && argc <= 9) {
        
        int outDevices[9] = {0,0,0,0,0,0,0,0,0};
        
        for (int i=0; i<argc; ++i) {
            
            if (argv[i].a_type == A_FLOAT) {
                
                outDevices[i] = getClampIntValue(static_cast<int>(argv[i].a_w.w_float), 0,
                                                 ofxMidiOut::getNumPorts());
            }
            else if (argv[i].a_type == A_SYMBOL) {
                
                const t_string &name = argv[i].a_w.w_symbol->s_name;
                const size_t len = ofxMidiOut::getPortList().size();
                vector<t_string> portList(len);
                
                for (size_t i=0; i<len; ++i)
                    portList[i] = ofxMidiOut::getPortList()[i].c_str();
                const ptrdiff_t pos = find(portList.begin(), portList.end(), name) - portList.begin();
                
                if (pos < portList.size()) {
                    
                    outDevices[i] = static_cast<int>(pos) + 1;
                }
                else {
                    
                    error("ofSetMidiDevices: couldn't find midi output device: %s", name.c_str());
                    return;
                }
            }
            else {
                
                error("ofSetMidiDevices: wrong argument type");
                return;
            }
        }
        for (int i=0; i<9; ++i)
            x->midiOutDevices[i] = outDevices[i];
    }
    else {
        
        error("ofSetMidiDevices: invalid number of arguments");
    }
}

void ofeliaSetMidiDevices_print(t_ofeliaSetMidiDevices *x)
{
    post("\n[ofSetMidiDevices]");
    post("input : ");
    
    for (int i=0; i<9; ++i) {
        
        const int indevno = x->midiInDevices[i];
        
        if (indevno > 0)
            post(" %s", ofxMidiIn::getPortName(indevno-1).c_str());
    }
    post("output : ");
    
    for (int i=0; i<9; ++i) {
        
        const int outdevno = x->midiOutDevices[i];
        
        if (outdevno > 0)
            post(" %s", ofxMidiOut::getPortName(outdevno-1).c_str());
    }
}

void ofeliaSetMidiDevices_setup()
{
    ofeliaSetMidiDevices_class = class_new(gensym("ofSetMidiDevices"),
                                           reinterpret_cast<t_newmethod>(ofeliaSetMidiDevices_new),
                                           0, sizeof(t_ofeliaSetMidiDevices),
                                           CLASS_DEFAULT, A_NULL, 0);
    class_addbang(ofeliaSetMidiDevices_class, reinterpret_cast<t_method>(ofeliaSetMidiDevices_bang));
    class_addmethod(ofeliaSetMidiDevices_class, reinterpret_cast<t_method>(ofeliaSetMidiDevices_input), gensym("input"), A_GIMME, 0);
    class_addmethod(ofeliaSetMidiDevices_class, reinterpret_cast<t_method>(ofeliaSetMidiDevices_output), gensym("output"), A_GIMME, 0);
    class_addmethod(ofeliaSetMidiDevices_class, reinterpret_cast<t_method>(ofeliaSetMidiDevices_print), gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * setup methods
 */
void ofeliaDevices_setup()
{
    ofeliaGetAudioDevices_setup();
    ofeliaSetAudioDevices_setup();
    ofeliaGetMidiDevices_setup();
    ofeliaSetMidiDevices_setup();
}

