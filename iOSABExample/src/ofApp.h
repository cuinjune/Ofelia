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

#pragma once

#include "ofMain.h"
#include "ofxiOS.h"
#include "ofxiOSExtras.h"
#include "ofxPd.h"
#include "ofxMidi.h"
#include "ABiOSSoundStream.h"

// a namespace for the Pd types
using namespace pd;

// derive from Pd receiver classes to receieve message and midi events
class ofApp : public ofxiOSApp, public PdReceiver, public PdMidiReceiver, public ofxMidiListener, public ofxMidiConnectionListener {
	
    public:
    
        void initAudio();
        void setup();
        void update();
        void draw();
        void exit();
	
        void touchDown(ofTouchEventArgs & touch);
        void touchMoved(ofTouchEventArgs & touch);
        void touchUp(ofTouchEventArgs & touch);
        void touchDoubleTap(ofTouchEventArgs & touch);
        void touchCancelled(ofTouchEventArgs & touch);

        void lostFocus();
        void gotFocus();
        void gotMemoryWarning();
        void deviceOrientationChanged(int newOrientation);
    
        // audio callbacks
        void audioReceived(float *input, int bufferSize, int nChannels);
        void audioRequested(float *output, int bufferSize, int nChannels);
    
        // pd message receiver callbacks
        void print(const std::string& message);
    
        void receiveBang(const std::string& dest);
        void receiveFloat(const std::string& dest, float value);
        void receiveSymbol(const std::string& dest, const std::string& symbol);
        void receiveList(const std::string& dest, const List& list);
        void receiveMessage(const std::string& dest, const std::string& msg, const List& list);
    
        // pd midi receiver callbacks
        void receiveNoteOn(const int channel, const int pitch, const int velocity);
        void receiveControlChange(const int channel, const int controller, const int value);
        void receiveProgramChange(const int channel, const int value);
        void receivePitchBend(const int channel, const int value);
        void receiveAftertouch(const int channel, const int value);
        void receivePolyAftertouch(const int channel, const int pitch, const int value);
        void receiveMidiByte(const int port, const int byte);
    
        //MIDI stuffs--------------
    
        // add a message to the display queue
        void addMessage(string msg);
    
        // midi message callback
        void newMidiMessage(ofxMidiMessage& msg);
    
        // midi device (dis)connection event callbacks
        void midiInputAdded(string name, bool isNetwork);
        void midiInputRemoved(string name, bool isNetwork);
    
        void midiOutputAdded(string nam, bool isNetwork);
        void midiOutputRemoved(string name, bool isNetwork);
    
        //setup audio and midi
        void setupAudioStream();
        void setupAudio(bool isReinit);
        void setupMidi();
        vector<ofSoundDevice> getDeviceList();
        void getInputOutputAudioDeviceList(vector<ofSoundDevice> &inputDeviceList,
                                           vector<ofSoundDevice> &outputDeviceList);
        ABiOSSoundStream *stream;
        ABiOSSoundStream *getSoundStream();
        int inputDeviceID, outputDeviceID;
        int numInputChannels, numOutputChannels;
        int sampleRate, ticksPerBuffer;
        bool shouldReinitAudio;
        ofxMidiIn midiIns[9];
        ofxMidiOut midiOuts[9];
        ofxPd pd;
    
    private:
    
        // sets the preferred sample rate, returns the *actual* samplerate
        // which may be different ie. iPhone 6S only wants 48k
        float setAVSessionSampleRate(float preferredSampleRate);
        ofxMidiMessage midiMessage;
        int midiChan = 0;
        deque<string> messages;
        int maxMessages;
        ofMutex messageMutex; // make sure we don't read from queue while writing
    
        enum Orientation {
        
            ORIENTATION_DEFAULT,
            ORIENTATION_RIGHT,
            ORIENTATION_INVERT,
            ORIENTATION_LEFT
        };
        int orientation = -1;
        int realSampleRate;
};

