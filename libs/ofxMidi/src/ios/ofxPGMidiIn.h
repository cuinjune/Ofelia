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

#include "../ofxBaseMidi.h"

class ofxMidiConnectionListener;

// reference http://syntheticbits.com/blog/?p=878
class ofxPGMidiIn : public ofxBaseMidiIn {

public:

	ofxPGMidiIn(const std::string name, ofxMidiApi api=MIDI_API_DEFAULT);
	virtual ~ofxPGMidiIn();

	void listInPorts();
	std::vector<std::string> getInPortList();
	int getNumInPorts();
	std::string getInPortName(unsigned int portNumber);

	bool openPort(unsigned int portNumber);
	bool openPort(std::string deviceName);
	bool openVirtualPort(std::string portName); ///< currently noop on iOS
	void closePort();
	
	void ignoreTypes(bool midiSysex, bool midiTiming, bool midiSense);

	/// wrapper around manageNewMessage
	void messageReceived(double deltatime, std::vector<unsigned char> *message);

	// iOS specific global stuff,
	// easier to route through here thanks to Obj-C/C++ mix
	static void setConnectionListener(ofxMidiConnectionListener * listener);
	static void clearConnectionListener();
	static void enableNetworking();

private:

	struct InputDelegate; // forward declaration for Obj-C wrapper
	InputDelegate *inputDelegate; ///< Obj-C midi input interface
};
