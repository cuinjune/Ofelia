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
#include "ofxPGMidiOut.h"

#include "ofLog.h"

#import "ofxPGMidiContext.h"

// PIMPL wrapper from http://stackoverflow.com/questions/7132755/wrapping-objective-c-in-objective-c-c
struct ofxPGMidiOut::Destination {
	PGMidiDestination *d; ///< output destination
};

// -----------------------------------------------------------------------------
ofxPGMidiOut::ofxPGMidiOut(const std::string name, ofxMidiApi api) : ofxBaseMidiOut(name, MIDI_API_COREMIDI) {
	
	// setup global midi instance
	ofxPGMidiContext::setup();
	
	// setup destination pointer
	destination = new Destination;
	destination->d = nil;
}

// -----------------------------------------------------------------------------
ofxPGMidiOut::~ofxPGMidiOut() {
	closePort();
	delete destination;
}

// -----------------------------------------------------------------------------
void ofxPGMidiOut::listOutPorts() {
	PGMidi *midi = ofxPGMidiContext::getMidi();
	int count = [midi.destinations count]; 
	ofLogNotice("ofxMidiOut") << count << " ports available";
	for(NSUInteger i = 0; i < count; ++i) {
		PGMidiDestination *dest = [midi.destinations objectAtIndex:i];
		ofLogNotice("ofxMidiOut") << i << ": " << [dest.name UTF8String];
	}
}

// -----------------------------------------------------------------------------
std::vector<std::string> ofxPGMidiOut::getOutPortList() {
	PGMidi *midi = ofxPGMidiContext::getMidi();
	std::vector<std::string> portList;
	for(PGMidiDestination *dest in midi.destinations) {
		portList.push_back([dest.name UTF8String]);
	}
	return portList;
}

// -----------------------------------------------------------------------------
int ofxPGMidiOut::getNumOutPorts() {
	return [ofxPGMidiContext::getMidi().destinations count];
}

// -----------------------------------------------------------------------------
std::string ofxPGMidiOut::getOutPortName(unsigned int portNumber) {
	
	PGMidi *midi = ofxPGMidiContext::getMidi();
	
	// handle OBJ-C exceptions
	@try {
		PGMidiDestination *dest = [midi.destinations objectAtIndex:portNumber]; 
		return [dest.name UTF8String];
	}
	@catch(NSException *ex) {
		ofLogError("ofxMidiOut") << "couldn't get name for port " << portNumber
			<< " " << [ex.name UTF8String] << ": " << [ex.reason UTF8String];
	}
	return "";
}

// -----------------------------------------------------------------------------
bool ofxPGMidiOut::openPort(unsigned int portNumber) {	
	
	PGMidi *midi = ofxPGMidiContext::getMidi();
	PGMidiDestination *dest = nil;
	
	// handle OBJ-C exceptions
	@try {
		dest = [midi.destinations objectAtIndex:portNumber]; 
	}
	@catch(NSException *ex) {
		ofLogError("ofxMidiOut") << "couldn't open port " << portNumber
			<< " " << [ex.name UTF8String] << ": " << [ex.reason UTF8String];
		return false;
	}
	destination->d = dest;
	portNum = portNumber;
	portName = [dest.name UTF8String];
	bOpen = true;
	ofLogVerbose("ofxMidiOut") << "opened port " << portNum << " " << portName;
	return true;
}

// -----------------------------------------------------------------------------
bool ofxPGMidiOut::openPort(std::string deviceName) {
	
	PGMidi *midi = ofxPGMidiContext::getMidi();
	
	// iterate through MIDI ports, find requested device
	int port = -1;
	for(NSUInteger i = 0; i < [midi.destinations count]; ++i) {
		PGMidiSource *dest = [midi.destinations objectAtIndex:i];
		if([dest.name UTF8String] == deviceName) {
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
bool ofxPGMidiOut::openVirtualPort(std::string portName) {
	ofLogWarning("ofxMidiOut") << "couldn't open virtual port \"" << portName << "\"";
	ofLogWarning("ofxMidiOut") << "virtual ports are currently not supported on iOS";
	return false;
}

// -----------------------------------------------------------------------------
void ofxPGMidiOut::closePort() {

	if(destination->d != nil) {
		ofLogVerbose("ofxMidiOut") << "closing port " <<  portNum << " " << portName;
	}
	destination->d = nil;
	
	portNum = -1;
	portName = "";
	bOpen = false;
	bVirtual = false;
}

// PRIVATE
// -----------------------------------------------------------------------------
// adapted from PGMidi sendBytes
void ofxPGMidiOut::sendMessage(std::vector<unsigned char> &message) {

	Byte packetBuffer[message.size()+100];
	MIDIPacketList *packetList = (MIDIPacketList*)packetBuffer;
	MIDIPacket *packet = MIDIPacketListInit(packetList);

	packet = MIDIPacketListAdd(packetList, sizeof(packetBuffer), packet, 0, message.size(), &message[0]);

	[destination->d sendPacketList:packetList];
}
