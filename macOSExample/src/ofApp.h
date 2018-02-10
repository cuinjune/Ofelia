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
#include "ofxPd.h"
#include "ofxMidi.h"

// a namespace for the Pd types
using namespace pd;

class ofApp : public ofBaseApp, public PdReceiver, public PdMidiReceiver, public ofxMidiListener {

	public:

        void initAudio();
		void setup();
		void update();
		void draw();
        void exit();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
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
    
        //ofxMidi
        void newMidiMessage(ofxMidiMessage& eventArgs);
    
        //setup audio and midi
        void setupAudio(bool isReinit);
        void setupMidi();
        void getInputOutputAudioDeviceList(vector<ofSoundDevice> &inputDeviceList,
                                           vector<ofSoundDevice> &outputDeviceList);
        ofSoundStream inputStream, outputStream;
        int inputDeviceID, outputDeviceID;
        int numInputChannels, numOutputChannels;
        int sampleRate, ticksPerBuffer;
        bool shouldReinitAudio;
        ofxMidiIn midiIns[9];
        ofxMidiOut midiOuts[9];
        ofxPd pd;
    
    private:
    
        ofxMidiMessage midiMessage;
        int midiChan = 0;
};
