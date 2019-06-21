/*
 * Copyright (c) 2013 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxMidi for documentation
 *
 */
#include "ofxRtMidiIn.h"

#include "ofUtils.h"
#include "ofLog.h"

// -----------------------------------------------------------------------------
ofxRtMidiIn::ofxRtMidiIn(const std::string name, ofxMidiApi api) :
	ofxBaseMidiIn(name, api), midiIn((RtMidi::Api)api, name) {
}

// -----------------------------------------------------------------------------
ofxRtMidiIn::~ofxRtMidiIn() {
	closePort();
}

// -----------------------------------------------------------------------------
void ofxRtMidiIn::listInPorts() {
	ofLogNotice("ofxMidiIn") << midiIn.getPortCount() << " ports available";
	for(unsigned int i = 0; i < midiIn.getPortCount(); ++i){
		ofLogNotice("ofxMidiIn") <<  i << ": " << midiIn.getPortName(i);
	}
}

// -----------------------------------------------------------------------------
std::vector<std::string> ofxRtMidiIn::getInPortList() {
	std::vector<std::string> portList;
	for(unsigned int i = 0; i < midiIn.getPortCount(); ++i) {
		portList.push_back(midiIn.getPortName(i));
	}
	return portList;
}

// -----------------------------------------------------------------------------
int ofxRtMidiIn::getNumInPorts() {
	return midiIn.getPortCount();
}

// -----------------------------------------------------------------------------
std::string ofxRtMidiIn::getInPortName(unsigned int portNumber) {
	// handle RtMidi exceptions
	try {
		return midiIn.getPortName(portNumber);
	}
	catch(RtMidiError& err) {
		ofLogError("ofxMidiIn") << "couldn't get name for port " << portNumber << ": " << err.what();
	}
	return "";
}

// -----------------------------------------------------------------------------
bool ofxRtMidiIn::openPort(unsigned int portNumber) {	
	// handle RtMidi exceptions
	try {
		closePort();
		midiIn.setCallback(&_midiMessageCallback, this);
		midiIn.openPort(portNumber, "ofxMidi Input "+ofToString(portNumber));
	}
	catch(RtMidiError& err) {
		ofLogError("ofxMidiIn") << "couldn't get open port " << portNumber << ": " << err.what();
		return false;
	}
	portNum = portNumber;
	portName = midiIn.getPortName(portNumber);
	bOpen = true;
	ofLogVerbose("ofxMidiIn") << "opened port " << portNumber << " " << portName;
	return true;
}

// -----------------------------------------------------------------------------
bool ofxRtMidiIn::openPort(std::string deviceName) {
	
	// iterate through MIDI ports, find requested device
	int port = -1;
	for(unsigned int i = 0; i < midiIn.getPortCount(); ++i) {
		std::string name = midiIn.getPortName(i);
		if(name == deviceName) {
			port = i;
			break;
		}
	}
	
	// bail if not found
	if(port == -1) {
		ofLogError("ofxMidiIn") << "port \"" << deviceName << "\" is not available";
		return false;
	} 
	
	return openPort(port);
}

// -----------------------------------------------------------------------------
bool ofxRtMidiIn::openVirtualPort(std::string portName) {
	// handle RtMidi exceptions
	try {
		closePort();
		midiIn.setCallback(&_midiMessageCallback, this);
		midiIn.openVirtualPort(portName);
	}
	catch(RtMidiError& err) {
		ofLogError("ofxMidiIn") << "couldn't open virtual port \"" << portName << "\": " << err.what();
		return false;
	}
	this->portName = portName;
	bOpen = true;
	bVirtual = true;
	ofLogVerbose("ofxMidiIn") << "opened virtual port " << portName;
	return true;
}

// -----------------------------------------------------------------------------
void ofxRtMidiIn::closePort() {
	if(bVirtual && bOpen) {
		ofLogVerbose("ofxMidiIn") << "closing virtual port " << portName;
	}
	else if(bOpen && portNum > -1) {
		ofLogVerbose("ofxMidiIn") << "closing port " << portNum << " " << portName;
	}
	midiIn.closePort();
	if(bOpen) {
		midiIn.cancelCallback();
	}
	portNum = -1;
	portName = "";
	bOpen = false;
	bVirtual = false;
}

// -----------------------------------------------------------------------------
void ofxRtMidiIn::ignoreTypes(bool midiSysex, bool midiTiming, bool midiSense) {
	midiIn.ignoreTypes(midiSysex, midiTiming, midiSense);
	ofLogVerbose("ofxMidiIn") <<"ignore types on " << portName << ": sysex: " << midiSysex
	    << " timing: " << midiTiming << " sense: " << midiSense;
}
// -----------------------------------------------------------------------------
void ofxRtMidiIn::_midiMessageCallback(double deltatime, std::vector<unsigned char> *message, void *userData) {
	((ofxRtMidiIn*) userData)->manageNewMessage(deltatime * 1000, message); // convert s to ms
}
