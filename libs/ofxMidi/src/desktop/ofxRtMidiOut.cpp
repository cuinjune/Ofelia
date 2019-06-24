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
#include "ofxRtMidiOut.h"

#include "ofUtils.h"
#include "ofLog.h"

// -----------------------------------------------------------------------------
ofxRtMidiOut::ofxRtMidiOut(const std::string name, ofxMidiApi api) :
	ofxBaseMidiOut(name, api), midiOut((RtMidi::Api)api, name) {
}

// -----------------------------------------------------------------------------
ofxRtMidiOut::~ofxRtMidiOut() {
	closePort();
}

// -----------------------------------------------------------------------------
void ofxRtMidiOut::listOutPorts() {
	ofLogNotice("ofxMidiOut") << midiOut.getPortCount() << " ports available";
	for(unsigned int i = 0; i < midiOut.getPortCount(); ++i){
		ofLogNotice("ofxMidiOut") <<  i << ": " << midiOut.getPortName(i);
	}
}

// -----------------------------------------------------------------------------
std::vector<std::string> ofxRtMidiOut::getOutPortList() {
	std::vector<std::string> portList;
	for(unsigned int i = 0; i < midiOut.getPortCount(); ++i) {
		portList.push_back(midiOut.getPortName(i));
	}
	return portList;
}

// -----------------------------------------------------------------------------
int ofxRtMidiOut::getNumOutPorts() {
	return midiOut.getPortCount();
}

// -----------------------------------------------------------------------------
std::string ofxRtMidiOut::getOutPortName(unsigned int portNumber) {
	// handle RtMidi exceptions
	try {
		return midiOut.getPortName(portNumber);
	}
	catch(RtMidiError& err) {
		ofLogError("ofxMidiOut") << "couldn't get name for port " << portNumber << ": " << err.what();
	}
	return "";
}

// -----------------------------------------------------------------------------
bool ofxRtMidiOut::openPort(unsigned int portNumber) {	
	// handle RtMidi exceptions
	try {
		closePort();
		midiOut.openPort(portNumber, "ofxMidi Output "+ofToString(portNumber));
	}
	catch(RtMidiError& err) {
		ofLogError("ofxMidiOut") << "couldn't open port " << portNumber << ": " << err.what();
		return false;
	}
	portNum = portNumber;
	portName = midiOut.getPortName(portNumber);
	bOpen = true;
	ofLogVerbose("ofxMidiOut") << "opened port " << portNum << " " <<  portName;
	return true;
}

// -----------------------------------------------------------------------------
bool ofxRtMidiOut::openPort(std::string deviceName) {
	
	// iterate through MIDI ports, find requested device
	int port = -1;
	for(unsigned int i = 0; i < midiOut.getPortCount(); ++i) {
		std::string name = midiOut.getPortName(i);
		if(name == deviceName) {
			port = i;
			break;
		}
	}
	
	// bail if not found
	if(port == -1) {
		ofLogError("ofxMidiOut") << "port \"" << deviceName << "\" is not available";
		return false;
	} 
	
	return openPort(port);
}

// -----------------------------------------------------------------------------
bool ofxRtMidiOut::openVirtualPort(std::string portName) {
	// handle RtMidi exceptions
	try {
		closePort();
		midiOut.openVirtualPort(portName);
	}
	catch(RtMidiError& err) {
		ofLogError("ofxMidiOut") << "couldn't open virtual port \"" << portName << "\": " << err.what();
		return false;
	}
	
	this->portName = portName;
	bOpen = true;
	bVirtual = true;
	ofLogVerbose("ofxMidiOut") << "opened virtual port " << portName;
	return true;
}

// -----------------------------------------------------------------------------
void ofxRtMidiOut::closePort() {
	if(bVirtual && bOpen) {
		ofLogVerbose("ofxMidiOut") << "closing virtual port " << portName;
	}
	else if(bOpen && portNum > -1) {
		ofLogVerbose("ofxMidiOut") << "closing port " << portNum << " " << portName;
	}
	midiOut.closePort();
	portNum = -1;
	portName = "";
	bOpen = false;
	bStreamInProgress = false;
	bVirtual = false;
}

// PRIVATE
// -----------------------------------------------------------------------------
void ofxRtMidiOut::sendMessage(std::vector<unsigned char> &message) {
	// handle RtMidi exceptions
	try {
		midiOut.sendMessage(&message);
	}
	catch(RtMidiError &err) {
		ofLogError("ofxMidiOut") << "couldn't send message: " << err.what();
	}
}
