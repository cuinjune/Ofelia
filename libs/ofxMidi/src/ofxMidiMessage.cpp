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
#include "ofxMidiMessage.h"

#include "ofxMidi.h"

// -----------------------------------------------------------------------------
ofxMidiMessage::ofxMidiMessage() {
	clear();
}

// -----------------------------------------------------------------------------
ofxMidiMessage::ofxMidiMessage(std::vector<unsigned char>* rawBytes) {
	fromBytes(rawBytes);
}

// -----------------------------------------------------------------------------
ofxMidiMessage::ofxMidiMessage(const ofxMidiMessage& from) {
	copy(from);
}

// -----------------------------------------------------------------------------
ofxMidiMessage& ofxMidiMessage::operator=(const ofxMidiMessage& from) {
	copy(from);
	return *this;
}

// -----------------------------------------------------------------------------
void ofxMidiMessage::copy(const ofxMidiMessage& from) {
	bytes.clear();
	for(unsigned int i = 0; i < from.bytes.size(); ++i) {
		bytes.push_back(from.bytes[i]);
	}
	status = from.status;
	channel = from.channel;
	pitch = from.pitch;
	velocity = from.velocity;
	control = from.control;
	value = from.value;
	bytes = from.bytes;
	deltatime = from.deltatime;
	portNum = from.portNum;
	portName = from.portName;
}

// -----------------------------------------------------------------------------
void ofxMidiMessage::fromBytes(std::vector<unsigned char> *rawBytes) {

	// copy bytes
	clear();
	for(unsigned int i = 0; i < rawBytes->size(); ++i) {
		bytes.push_back(rawBytes->at(i));
	}

	// parse
	if(bytes[0] >= MIDI_SYSEX) {
		status = (MidiStatus) (bytes[0] & 0xFF);
		channel = 0;
	} else {
		status = (MidiStatus) (bytes[0] & 0xF0);
		channel = (int) (bytes[0] & 0x0F)+1;
	}
	switch(status) {
		case MIDI_NOTE_ON :
		case MIDI_NOTE_OFF:
			pitch = (int) bytes[1];
			velocity = (int) bytes[2];
			break;
		case MIDI_CONTROL_CHANGE:
			control = (int) bytes[1];
			value = (int) bytes[2];
			break;
		case MIDI_PROGRAM_CHANGE:
		case MIDI_AFTERTOUCH:
			value = (int) bytes[1];
			break;
		case MIDI_PITCH_BEND:
			value = (int) (bytes[2] << 7) + (int) bytes[1]; // msb + lsb
			break;
		case MIDI_POLY_AFTERTOUCH:
			pitch = (int) bytes[1];
			value = (int) bytes[2];
			break;
		case MIDI_SONG_POS_POINTER:
			value = (int) (bytes[2] << 7) + (int) bytes[1]; // msb + lsb
			break;
		default:
			break;
	}
}

// -----------------------------------------------------------------------------
void ofxMidiMessage::clear() {
	status = MIDI_UNKNOWN;
	channel = 0;
	pitch = 0;
	velocity = 0;
	control = 0;
	value = 0;
	deltatime = 0;
	portNum = -1;
	portName = "";
	bytes.clear();
}

// -----------------------------------------------------------------------------
std::string ofxMidiMessage::toString() {
	std::stringstream stream;
	stream << portName << ": " << getStatusString(status) << " "
		   << channel << " [ ";
	stream << ofxMidi::bytesToString(bytes);
	stream << "] " << deltatime;
	return stream.str();
}

// -----------------------------------------------------------------------------
std::string ofxMidiMessage::getStatusString(MidiStatus status) {
	switch(status) {
		case MIDI_NOTE_OFF:
			return "Note Off";
		case MIDI_NOTE_ON:
			return "Note On";
		case MIDI_CONTROL_CHANGE:
			return "Control Change";
		case MIDI_PROGRAM_CHANGE:
			return "Program Change";
		case MIDI_PITCH_BEND:
			return "Pitch Bend";
		case MIDI_AFTERTOUCH:
			return "Aftertouch";
		case MIDI_POLY_AFTERTOUCH:
			return "Poly Aftertouch";
		case MIDI_SYSEX:
			return "Sysex";
		case MIDI_TIME_CODE:
			return "Time Code";
		case MIDI_SONG_POS_POINTER:
			return "Song Pos";
		case MIDI_SONG_SELECT:
			return "Song Select";
		case MIDI_TUNE_REQUEST:
			return "Tune Request";
		case MIDI_SYSEX_END:
			return "Sysex End";
		case MIDI_TIME_CLOCK:
			return "Time Clock";
		case MIDI_START:
			return "Start";
		case MIDI_CONTINUE:
			return "Continue";
		case MIDI_STOP:
			return "Stop";
		case MIDI_ACTIVE_SENSING:
			return "Active Sensing";
		case MIDI_SYSTEM_RESET:
			return "System Reset";
		default:
			return "Unknown";
	}
}
