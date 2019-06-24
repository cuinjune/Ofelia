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
#include "ofxBaseMidi.h"

#include "ofLog.h"

// MIDI IN

// -----------------------------------------------------------------------------
ofxBaseMidiIn::ofxBaseMidiIn(const std::string name, ofxMidiApi api) {
	portNum = -1;
	portName = "";
	bOpen = false;
	bVerbose = false;
	bVirtual = false;
	this->api = api;
}
// -----------------------------------------------------------------------------
int ofxBaseMidiIn::getPort() {
	return portNum;
}

// -----------------------------------------------------------------------------
std::string ofxBaseMidiIn::getName() {
	return portName;
}

// -----------------------------------------------------------------------------
bool ofxBaseMidiIn::isOpen() {
	return bOpen;
}

// -----------------------------------------------------------------------------
bool ofxBaseMidiIn::isVirtual() {
	return bVirtual;
}

// -----------------------------------------------------------------------------
ofxMidiApi ofxBaseMidiIn::getApi() {
	return api;
}

// -----------------------------------------------------------------------------
void ofxBaseMidiIn::addListener(ofxMidiListener *listener) {
	ofAddListener(newMessageEvent, listener, &ofxMidiListener::newMidiMessage);
}

// -----------------------------------------------------------------------------
void ofxBaseMidiIn::removeListener(ofxMidiListener *listener) {
	ofRemoveListener(newMessageEvent, listener, &ofxMidiListener::newMidiMessage);
}

// -----------------------------------------------------------------------------
void ofxBaseMidiIn::setVerbose(bool verbose) {
	bVerbose = verbose;
}

// PRIVATE
// -----------------------------------------------------------------------------
void ofxBaseMidiIn::manageNewMessage(double deltatime, std::vector<unsigned char> *message) {
			
	// parse message and fill event
	ofxMidiMessage midiMessage(message);
	midiMessage.deltatime = deltatime;
	midiMessage.portNum = portNum;
	midiMessage.portName = portName;
	if(bVerbose) {
		ofLogVerbose("ofxMidiIn") << midiMessage.toString();
	}
	
	// send event to listeners
	ofNotifyEvent(newMessageEvent, midiMessage, this);
}

// MIDI OUT

// -----------------------------------------------------------------------------
ofxBaseMidiOut::ofxBaseMidiOut(const std::string name, ofxMidiApi api) {
	portNum = -1;
	portName = "";
	bOpen = false;
	bStreamInProgress = false;
	bVirtual = false;
	this->api = api;
}

// -----------------------------------------------------------------------------
int ofxBaseMidiOut::getPort() {
	return portNum;
}

// -----------------------------------------------------------------------------
std::string ofxBaseMidiOut::getName() {
	return portName;
}

// -----------------------------------------------------------------------------
bool ofxBaseMidiOut::isOpen() {
	return bOpen;
}

// -----------------------------------------------------------------------------
bool ofxBaseMidiOut::isVirtual() {
	return bVirtual;
}

// -----------------------------------------------------------------------------
ofxMidiApi ofxBaseMidiOut::getApi() {
	return api;
}

// -----------------------------------------------------------------------------
void ofxBaseMidiOut::sendNoteOn(int channel, int pitch, int velocity) {
	std::vector<unsigned char> message;
	message.push_back(MIDI_NOTE_ON+(channel-1));
	message.push_back(pitch);
	message.push_back(velocity);
	sendMessage(message);
}

// -----------------------------------------------------------------------------
void ofxBaseMidiOut::sendNoteOff(int channel, int pitch, int velocity) {
	std::vector<unsigned char> message;
	message.push_back(MIDI_NOTE_OFF+(channel-1));
	message.push_back(pitch);
	message.push_back(velocity);
	sendMessage(message);
}

// -----------------------------------------------------------------------------
void ofxBaseMidiOut::sendControlChange(int channel, int control, int value) {
	std::vector<unsigned char> message;
	message.push_back(MIDI_CONTROL_CHANGE+(channel-1));
	message.push_back(control);
	message.push_back(value);
	sendMessage(message);
}

// -----------------------------------------------------------------------------
void ofxBaseMidiOut::sendProgramChange(int channel, int value) {
	std::vector<unsigned char> message;
	message.push_back(MIDI_PROGRAM_CHANGE+(channel-1));
	message.push_back(value);
	sendMessage(message);
}

// -----------------------------------------------------------------------------
void ofxBaseMidiOut::sendPitchBend(int channel, int value) {
	std::vector<unsigned char> message;
	message.push_back(MIDI_PITCH_BEND+(channel-1));
	message.push_back(value & 0x7F);        // lsb 7bit
	message.push_back((value >> 7) & 0x7F); // msb 7bit
	sendMessage(message);
}

void ofxBaseMidiOut::sendPitchBend(int channel, unsigned char lsb, unsigned char msb) {
	std::vector<unsigned char> message;
	message.push_back(MIDI_PITCH_BEND+(channel-1));
	message.push_back(lsb);
	message.push_back(msb);
	sendMessage(message);
}

// -----------------------------------------------------------------------------
void ofxBaseMidiOut::sendAftertouch(int channel, int value) {
	std::vector<unsigned char> message;
	message.push_back(MIDI_AFTERTOUCH+(channel-1));
	message.push_back(value);
	sendMessage(message);
}

// -----------------------------------------------------------------------------
void ofxBaseMidiOut::sendPolyAftertouch(int channel, int pitch, int value) {
	std::vector<unsigned char> message;
	message.push_back(MIDI_POLY_AFTERTOUCH+(channel-1));
	message.push_back(pitch);
	message.push_back(value);
	sendMessage(message);
}

// -----------------------------------------------------------------------------
void ofxBaseMidiOut::sendMidiByte(unsigned char byte) {
	if(bStreamInProgress) {
		stream.push_back(byte);
	}
	else {
		std::vector<unsigned char> message;
		message.push_back(byte);
		sendMessage(message);
	}
}

//----------------------------------------------------------
void ofxBaseMidiOut::sendMidiBytes(std::vector<unsigned char>& bytes) {
	if(bStreamInProgress) {
		stream.insert(stream.end(), bytes.begin(), bytes.end());
	}
	else {
		sendMessage(bytes);
	}
}

//----------------------------------------------------------
void ofxBaseMidiOut::startMidiStream() {
	if(bStreamInProgress) {
		ofLogWarning("ofxMidiOut") << "calling StartMidi when byte stream in progress";
		return;
	}
	stream.clear();
	bStreamInProgress = true;
}

// -----------------------------------------------------------------------------
void ofxBaseMidiOut::finishMidiStream() {
	if(!bStreamInProgress) {
		ofLogWarning("ofxMidiOut") << "can not finish midi byte stream, stream not in progress";
		return;
	}
	sendMessage(stream);
}
