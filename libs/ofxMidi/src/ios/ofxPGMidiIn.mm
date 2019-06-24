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
#include "ofxPGMidiIn.h"

#include "ofLog.h"

#import "ofxPGMidiContext.h"
#import "ofxPGMidiSourceDelegate.h"

// PIMPL wrapper from http://stackoverflow.com/questions/7132755/wrapping-objective-c-in-objective-c-c
struct ofxPGMidiIn::InputDelegate {
	ofxPGMidiSourceDelegate *d; ///< Obj-C input delegate
};

// -----------------------------------------------------------------------------
ofxPGMidiIn::ofxPGMidiIn(const std::string name, ofxMidiApi api) : ofxBaseMidiIn(name, MIDI_API_COREMIDI) {

	// setup global midi instance
	ofxPGMidiContext::setup();

	// setup Obj-C interface to PGMidi
	inputDelegate = new InputDelegate;
	inputDelegate->d = [[ofxPGMidiSourceDelegate alloc] init];
	[inputDelegate->d setInputPtr:(void*) this];
}

// -----------------------------------------------------------------------------
ofxPGMidiIn::~ofxPGMidiIn() {
	[inputDelegate->d release];
	delete inputDelegate;
}

// -----------------------------------------------------------------------------
void ofxPGMidiIn::listInPorts() {
	PGMidi *midi = ofxPGMidiContext::getMidi();
	int count = [midi.sources count];
	ofLogNotice("ofxMidiIn") << count << " ports available";
	for(NSUInteger i = 0; i < count; ++i) {
		PGMidiSource *source = [midi.sources objectAtIndex:i];
		ofLogNotice("ofxMidiIn") << i << ": " << [source.name UTF8String];
	}
}

// -----------------------------------------------------------------------------
std::vector<std::string> ofxPGMidiIn::getInPortList() {
	PGMidi *midi = ofxPGMidiContext::getMidi();
	std::vector<std::string> portList;
	for(PGMidiSource *source in midi.sources) {
		portList.push_back([source.name UTF8String]);
	}
	return portList;
}

// -----------------------------------------------------------------------------
int ofxPGMidiIn::getNumInPorts() {
	return [ofxPGMidiContext::getMidi().sources count];
}

// -----------------------------------------------------------------------------
std::string ofxPGMidiIn::getInPortName(unsigned int portNumber) {

	PGMidi *midi = ofxPGMidiContext::getMidi();

	// handle OBJ-C exceptions
	@try {
		PGMidiSource *source = [midi.sources objectAtIndex:portNumber]; 
		return [source.name UTF8String];
	}
	@catch(NSException *ex) {
		ofLogError("ofxMidiIn") << "couldn't get name for port " << portNumber
		    << " " << [ex.name UTF8String] << ": " << [ex.reason UTF8String];
	}
	return "";
}

// -----------------------------------------------------------------------------
bool ofxPGMidiIn::openPort(unsigned int portNumber) {

	PGMidi *midi = ofxPGMidiContext::getMidi();
	PGMidiSource *source = nil;

	// handle OBJ-C exceptions
	@try {
		source = [midi.sources objectAtIndex:portNumber]; 
	}
	@catch(NSException *ex) {
		ofLogError("ofxMidiIn") << "couldn't open port " << portNumber
			<< " " << [ex.name UTF8String] << ": " << [ex.reason UTF8String];
		return false;
	}
	[source addDelegate:inputDelegate->d];
	portNum = portNumber;
	portName = [source.name UTF8String];
	bOpen = true;
	ofLogVerbose("ofxMidiIn") << "opened port " << portNum << " " << portName;
	return true;
}

// -----------------------------------------------------------------------------
bool ofxPGMidiIn::openPort(std::string deviceName) {

	PGMidi *midi = ofxPGMidiContext::getMidi();

	// iterate through MIDI ports, find requested device
	int port = -1;
	for(NSUInteger i = 0; i < [midi.sources count]; ++i) {
		PGMidiSource *source = [midi.sources objectAtIndex:i];
		if([source.name UTF8String] == deviceName) {
			port = i;
			break;
		}
	}

	// bail if not found
	if(port == -1) {
		ofLogError("ofxMidiIn") << "port \"" << deviceName << "\" is not available";;
		return false;
	} 
	
	return openPort(port);
}

// -----------------------------------------------------------------------------
bool ofxPGMidiIn::openVirtualPort(std::string portName) {
	ofLogWarning("ofxMidiIn") << "couldn't open virtual port \"" << portName << "\"";
	ofLogWarning("ofxMidiIn") << "virtual ports are currently not supported on iOS";
	return false;
}

// -----------------------------------------------------------------------------
void ofxPGMidiIn::closePort() {

	if(bOpen) {
		ofLogVerbose("ofxMidiIn") << "closing port " << portNum << " " << portName;
		
		// sometimes the source may already have been removed in PGMidi, so make
		// sure we have a valid index otherwise the app will crash
		PGMidi *midi = ofxPGMidiContext::getMidi();
		if(portNum < midi.sources.count) {
			PGMidiSource *source = [midi.sources objectAtIndex:portNum];
			[source removeDelegate:inputDelegate->d];
		}
	}
	
	portNum = -1;
	portName = "";
	bOpen = false;
	bVirtual = false;
}

// -----------------------------------------------------------------------------
void ofxPGMidiIn::ignoreTypes(bool midiSysex, bool midiTiming, bool midiSense) {
	
	inputDelegate->d.bIgnoreSysex = midiSysex;
	inputDelegate->d.bIgnoreTiming = midiTiming;
	inputDelegate->d.bIgnoreSense = midiSense;
	
	ofLogVerbose("ofxMidiIn") << "ignore types on " << portName << ": sysex: " << midiSysex
	    << " timing: " << midiTiming << " sense: " << midiSense;
}

// -----------------------------------------------------------------------------
void ofxPGMidiIn::messageReceived(double deltatime, std::vector<unsigned char> *message) {
	manageNewMessage(deltatime, message);
}

// -----------------------------------------------------------------------------
void ofxPGMidiIn::setConnectionListener(ofxMidiConnectionListener * listener) {
	ofxPGMidiContext::setConnectionListener(listener);
}

// -----------------------------------------------------------------------------
void ofxPGMidiIn::clearConnectionListener() {
	ofxPGMidiContext::clearConnectionListener();
}

// -----------------------------------------------------------------------------
void ofxPGMidiIn::enableNetworking() {
	ofxPGMidiContext::enableNetwork();
}
