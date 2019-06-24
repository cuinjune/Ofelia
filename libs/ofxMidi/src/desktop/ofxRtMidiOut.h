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

class ofxRtMidiOut : public ofxBaseMidiOut {

public:

	/// set the output client name (optional)
	ofxRtMidiOut(const std::string name, ofxMidiApi api=MIDI_API_DEFAULT);
	virtual ~ofxRtMidiOut();

	void listOutPorts();
	std::vector<std::string> getOutPortList();
	int getNumOutPorts();
	std::string getOutPortName(unsigned int portNumber);

	bool openPort(unsigned int portNumber);
	bool openPort(std::string deviceName);
	bool openVirtualPort(std::string portName);
	void closePort();
	
private:
	
	void sendMessage(std::vector<unsigned char> &message);

	RtMidiOut midiOut;
};
