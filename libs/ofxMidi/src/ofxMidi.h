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

#include "ofxMidiIn.h"
#include "ofxMidiOut.h"
#include "ofxMidiClock.h"
#include "ofxMidiTimecode.h"

/// receives iOS MIDI device (dis)connection events
class ofxMidiConnectionListener {

public:

	ofxMidiConnectionListener() {}
	virtual ~ofxMidiConnectionListener() {}
	
	virtual void midiInputAdded(std::string name, bool isNetwork=false);
	virtual void midiInputRemoved(std::string name, bool isNetwork=false);
	
	virtual void midiOutputAdded(std::string nam, bool isNetwork=false);
	virtual void midiOutputRemoved(std::string name, bool isNetwork=false);
};

// global access
namespace ofxMidi {

/// \section Util

	/// convert MIDI note to frequency in Hz
	/// ala the [mtof] object in Max & Pure Data
	float mtof(float note);

	/// convert a frequency in Hz to a MIDI note
	/// ala the [ftom] object in Max & Pure Data
	float ftom(float frequency);

	/// convert raw MIDI bytes to a printable string, ex. "F0 0C 33"
	std::string bytesToString(std::vector<unsigned char> &bytes);

/// \section iOS Specific
	
	/// set a listener to receieve iOS device (dis)connection events
	///
	/// don't forget to clear before the listener is deallocated!
	///
	/// note: these are noops on Mac, Win, & Linux
	///
	void setConnectionListener(ofxMidiConnectionListener *listener);

	/// clear iOS device event receiver
	void clearConnectionListener();
	
	/// enables the network MIDI session between iOS and macOS on a
	/// local wifi network
	///
	/// in ofxMidi: open the input/outport network ports named "Session 1"
	///
	/// on OSX: use the Audio MIDI Setup Utility to connect to the iOS device
	///
	/// note: this is a noop on Mac, Win, & Linux
	///
	void enableNetworking();
};
