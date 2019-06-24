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
#pragma once

#include "ofEvents.h"
#include "ofxMidiConstants.h"

class ofxMidiMessage;

/// receives MIDI messages
class ofxMidiListener {

public:

	ofxMidiListener() {}
	virtual ~ofxMidiListener() {}

	virtual void newMidiMessage(ofxMidiMessage& message) = 0;
};

/// a single multi byte MIDI message
///
/// check status type and grab data:
/// 
///     if(message.status == MIDI_NOTE_ON) {
///         ofLog() << "note on " << message.channel
///                 << message.note << " " << message.velocity;
///     }
///
/// the message-specific types are only set for the appropriate
/// message types ie pitch is only set for noteon, noteoff, and
/// polyaftertouch messages
///
class ofxMidiMessage: public ofEventArgs {

public:

/// \section Variables

	MidiStatus status;
	int channel;        //< 1 - 16

	/// message-specific values,
	/// converted from raw bytes
	int pitch;          //< 0 - 127
	int velocity;       //< 0 - 127
	int control;        //< 0 - 127
	int value;          //< depends on message status type
	
	/// raw bytes
	std::vector<unsigned char> bytes;
	
	/// delta time since last message in ms
	double deltatime;

	/// the input port we received this message from
	///
	/// note: portNum will be -1 from a virtual port
	///
	int portNum;
	std::string portName;

/// \section Main

	ofxMidiMessage();
	ofxMidiMessage(std::vector<unsigned char>* rawBytes); //< parses
	ofxMidiMessage(const ofxMidiMessage& from);
	ofxMidiMessage& operator=(const ofxMidiMessage& from);
	void copy(const ofxMidiMessage& from);
	
	/// parse message from raw MIDI bytes
	void fromBytes(std::vector<unsigned char> *rawBytes);
	
	/// clear the message contents, also resets status
	void clear();
	
/// \section Util
	
	/// get the raw message as a string in the format:
	///
	/// PortName: status channel [ raw bytes in hex ] deltatime
	///
	std::string toString();

	/// get a MIDI status byte as a string
	/// ie "Note On", "Note Off", "Control Change", etc
	static std::string getStatusString(MidiStatus status);
};

typedef ofEvent<ofxMidiMessage> ofxMidiEvent;
