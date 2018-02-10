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

#include "ofApp.h"
#include "ofeliaSetup.h"

#define AUTO_NUM_CHANNELS -1

//--------------------------------------------------------------
void ofApp::initAudio()
{
    // you can change the settings below
    sampleRate = 44100;
    numInputChannels = AUTO_NUM_CHANNELS;
    numOutputChannels = AUTO_NUM_CHANNELS;
    
    // the number of libpd ticks per buffer,
    // used to compute the audio buffer len: tpb * blocksize (always 64)
    ticksPerBuffer = 8; // 8 * 64 = buffer len of 512
    shouldReinitAudio = false;
}

//--------------------------------------------------------------
void ofApp::setup()
{
#ifdef TARGET_OSX
    ofSetDataPathRoot("../Resources/data/");
#endif
    initAudio();
    setupAudio(false);
    setupMidi();
    pd.openPatch("pd/main.pd");
}

//--------------------------------------------------------------
void ofApp::update()
{
    if (shouldReinitAudio) {
        
        setupAudio(true); //reinit audio
        shouldReinitAudio = false;
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
}

//--------------------------------------------------------------
void ofApp::exit()
{
    // clean up midi inputs and outputs
    for (int i=0; i<9; ++i) {
        
        midiIns[i].closePort();
        midiIns[i].removeListener(this);
    }
    for (int i=0; i<9; ++i)
        midiOuts[i].closePort();
    
    // clean up audio inputs and outputs
    inputStream.close();
    outputStream.close();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}

//--------------------------------------------------------------
void ofApp::audioReceived(float *input, int bufferSize, int nChannels)
{
    pd.audioIn(input, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofApp::audioRequested(float *output, int bufferSize, int nChannels)
{
    pd.audioOut(output, bufferSize, nChannels);
}

//--------------------------------------------------------------
void ofApp::print(const std::string& message)
{
    ofLogNotice() << message;
}

//--------------------------------------------------------------
void ofApp::receiveBang(const std::string& dest)
{
}

void ofApp::receiveFloat(const std::string& dest, float value)
{
}

void ofApp::receiveSymbol(const std::string& dest, const std::string& symbol)
{
}

void ofApp::receiveList(const std::string& dest, const List& list)
{
}

void ofApp::receiveMessage(const std::string& dest, const std::string& msg, const List& list)
{
}

//--------------------------------------------------------------
void ofApp::receiveNoteOn(const int channel, const int pitch, const int velocity)
{
    for (int i=0; i<9; ++i) {
        
        if (midiOuts[i].isOpen())
            midiOuts[i].sendNoteOn(channel, pitch, velocity);
    }
}

void ofApp::receiveControlChange(const int channel, const int controller, const int value)
{
    for (int i=0; i<9; ++i) {
        
        if (midiOuts[i].isOpen())
            midiOuts[i].sendControlChange(channel, controller, value);
    }
}

// note: pgm nums are 1-128 to match pd
void ofApp::receiveProgramChange(const int channel, const int value)
{
    for (int i=0; i<9; ++i) {
        
        if (midiOuts[i].isOpen())
            midiOuts[i].sendProgramChange(channel, value);
    }
}

void ofApp::receivePitchBend(const int channel, const int value)
{
    for (int i=0; i<9; ++i) {
        
        if (midiOuts[i].isOpen())
            midiOuts[i].sendPitchBend(channel, value);
    }
}

void ofApp::receiveAftertouch(const int channel, const int value)
{
    for (int i=0; i<9; ++i) {
        
        if (midiOuts[i].isOpen())
            midiOuts[i].sendAftertouch(channel, value);
    }
}

void ofApp::receivePolyAftertouch(const int channel, const int pitch, const int value)
{
    for (int i=0; i<9; ++i) {
        
        if (midiOuts[i].isOpen())
            midiOuts[i].sendPolyAftertouch(channel, pitch, value);
    }
}

// note: pd adds +2 to the port num, so sending to port 3 in pd to [midiout],
//       shows up at port 1 in ofxPd
void ofApp::receiveMidiByte(const int port, const int byte)
{
    for (int i=0; i<9; ++i) {
        
        if (midiOuts[i].isOpen())
            midiOuts[i].sendMidiByte(byte);
    }
}

//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage& msg)
{
    midiMessage = msg;
    midiChan = midiMessage.channel;
    
    if (midiMessage.getStatusString(midiMessage.status) == "Note On") {
        
        pd.sendNoteOn(midiChan, midiMessage.pitch, midiMessage.velocity);
    }
    else if (midiMessage.getStatusString(midiMessage.status) == "Control Change") {
        
        pd.sendControlChange(midiChan, midiMessage.control, midiMessage.value);
    }
    else if (midiMessage.getStatusString(midiMessage.status) == "Program Change") {
        
        pd.sendProgramChange(midiChan, midiMessage.value); // note: pgm num range is 1 - 128
    }
    else if (midiMessage.getStatusString(midiMessage.status) == "Pitch Bend") {
        
        pd.sendPitchBend(midiChan, midiMessage.value - 8192); //note: ofxPd uses -8192 - 8192 while
        // [bendin] returns 0 - 16383,
        // so sending a val of 2000 gives 10192 in pd
    }
    else if (midiMessage.getStatusString(midiMessage.status) == "Aftertouch") {
        
        pd.sendAftertouch(midiChan, midiMessage.value);
    }
    else if (midiMessage.getStatusString(midiMessage.status) == "Poly Aftertouch") {
        
        pd.sendPolyAftertouch(midiChan, midiMessage.pitch, midiMessage.value);
    }
    else if (midiMessage.getStatusString(midiMessage.status) == "Sysex") {
        
        pd.sendSysex(midiMessage.portNum, midiMessage.value);       // note: pd adds +2 to the port number from
        pd.sendSysRealTime(midiMessage.portNum, midiMessage.value); // [midiin], [sysexin], & [realtimein].
        pd.sendMidiByte(midiMessage.portNum, midiMessage.value);    // so sending to port 0 gives port 2 in pd
    }
}

void ofApp::setupAudio(bool isReinit)
{
    // setup OF sound stream
    if (isReinit) {
        
        inputStream.close();
        outputStream.close();
    }
    
    // sort device list into input & output
    vector<ofSoundDevice> inputDeviceList, outputDeviceList;
    getInputOutputAudioDeviceList(inputDeviceList, outputDeviceList);
    bool inputDeviceNotFound = false;
    
    // check if a device exists
    if (inputDeviceList.empty()) {
        
        inputDeviceNotFound = true;
    }
    if (outputDeviceList.empty()) { // if no audio output device found, exit the app
        
        ofLogError() << "error : audio device not found";
        OF_EXIT_APP(1);
    }
    if (!isReinit) {
        
        inputDeviceID = outputDeviceID = -1;
        
        // search default input and output then assign sorted deviceID (not real)
        for (size_t i=0; i<inputDeviceList.size(); ++i) {
            
            if (inputDeviceList[i].isDefaultInput) {
                
                inputDeviceID = static_cast<int>(i);
                break;
            }
        }
        for (size_t i=0; i<outputDeviceList.size(); ++i) {
            
            if (outputDeviceList[i].isDefaultOutput) {
                
                outputDeviceID = static_cast<int>(i);
                break;
            }
        }

        // if no default input or output are found, assign the first deviceID
        if (inputDeviceID < 0)
            inputDeviceID = 0;
        
        if (outputDeviceID < 0)
            outputDeviceID = 0;
    }
    const vector<ofSoundDevice> &deviceList = inputStream.getDeviceList();
    inputDeviceID = getClampIntValue(inputDeviceID, 0, static_cast<int>(inputDeviceList.size())-1);
    outputDeviceID = getClampIntValue(outputDeviceID, 0, static_cast<int>(outputDeviceList.size())-1);
    
    // convert sorted deviceID into real deviceID
    const int realInputDeviceID = inputDeviceList[static_cast<size_t>(inputDeviceID)].deviceID;
    const int realOutputDeviceID = outputDeviceList[static_cast<size_t>(outputDeviceID)].deviceID;
    
    if (!isReinit) {
        
        // use all available channels
        if (numInputChannels < 0)
            numInputChannels = deviceList[static_cast<size_t>(realInputDeviceID)].inputChannels;
        
        if (numOutputChannels < 0)
            numOutputChannels = deviceList[static_cast<size_t>(realOutputDeviceID)].outputChannels;
    }
    numInputChannels = getClampIntValue(numInputChannels, 0,
                                        deviceList[static_cast<size_t>(realInputDeviceID)].inputChannels);
    numOutputChannels = getClampIntValue(numOutputChannels, 0,
                                         deviceList[static_cast<size_t>(realOutputDeviceID)].outputChannels);
    
    if (realInputDeviceID != realOutputDeviceID) { // if input and output use different device, setup separately
        
        if (!inputDeviceNotFound) { // do not setup input stream if no device was found
            
            inputStream.setDevice(inputStream.getDeviceList()[static_cast<size_t>(realInputDeviceID)]);
            inputStream.setup(this, 0, numInputChannels, sampleRate,
                              ofxPd::blockSize()*ticksPerBuffer, 3);
            inputStream.setInput(this);
        }
        outputStream.setDevice(outputStream.getDeviceList()[static_cast<size_t>(realOutputDeviceID)]);
        outputStream.setup(this, numOutputChannels, 0, sampleRate,
                           ofxPd::blockSize()*ticksPerBuffer, 3);
        outputStream.setOutput(this);
    }
    else { // if input and output use same device, setup once
        
        outputStream.setDevice(outputStream.getDeviceList()[static_cast<size_t>(realOutputDeviceID)]);
        outputStream.setup(this, numOutputChannels, numInputChannels, sampleRate,
                           ofxPd::blockSize()*ticksPerBuffer, 3);
    }
    
    // setup Pd
    if(!pd.init(numOutputChannels, numInputChannels, sampleRate, ticksPerBuffer, false)) {
        
        OF_EXIT_APP(1);
    }
    if (!isReinit) {
        
        // load libs
        ofelia_setup();
        
        // add message receiver, required if you want to recieve messages
        pd.addReceiver(*this); // automatically receives from all subscribed sources
        
        // add midi receiver, required if you want to recieve midi messages
        pd.addMidiReceiver(*this); // automatically receives from all channels
    }
    else {
        
        ofSleepMillis(100);
    }
    pd.start();
}

void ofApp::setupMidi()
{
    // setup midiIns and midiOuts
    for (int i=0; i<9; ++i) {

        midiIns[i].ignoreTypes(false, false, false);
        midiIns[i].addListener(this);
    }
}

void ofApp::getInputOutputAudioDeviceList(vector<ofSoundDevice> &inputDeviceList,
                                          vector<ofSoundDevice> &outputDeviceList)
{
    const vector<ofSoundDevice> &deviceList = inputStream.getDeviceList();
    
    for (size_t i=0; i<deviceList.size(); ++i) {
        
        if (deviceList[i].inputChannels)
            inputDeviceList.push_back(deviceList[i]);
        
        if (deviceList[i].outputChannels)
            outputDeviceList.push_back(deviceList[i]);
    }
}

