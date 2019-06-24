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

#include "RtMidi.h"
#include "../ofxBaseMidi.h"

class ofxRtMidiIn : public ofxBaseMidiIn {

public:

	ofxRtMidiIn(const std::string name, ofxMidiApi api=MIDI_API_DEFAULT);
	virtual ~ofxRtMidiIn();

	void listInPorts();
	std::vector<std::string> getInPortList();
	int getNumInPorts();
	std::string getInPortName(unsigned int portNumber);

	bool openPort(unsigned int portNumber);
	bool openPort(std::string deviceName);
	bool openVirtualPort(std::string portName);
	void closePort();

	void ignoreTypes(bool midiSysex, bool midiTiming, bool midiSense);

private:
	
	RtMidiIn midiIn;
	
	/// static callback for RtMidi
	static void _midiMessageCallback(double deltatime, std::vector<unsigned char> *message, void *userData);
};
