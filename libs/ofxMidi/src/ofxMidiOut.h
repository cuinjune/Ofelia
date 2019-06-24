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

#include "ofxBaseMidi.h"

// choose the MIDI backend
#ifdef TARGET_OF_IPHONE
	#include "ios/ofxPGMidiOut.h"
	#define OFX_MIDI_OUT_TYPE ofxPGMidiOut
#else // OSX, Win, Linux
	#include "desktop/ofxRtMidiOut.h"
	#define OFX_MIDI_OUT_TYPE ofxRtMidiOut
#endif

///
/// a MIDI output port
///
/// create multiple instances to connect to multiple ports
///
/// *do not* create static instances as this will lead to a crash on Linux,
/// instead create a static std::shared_ptr and initialize it later:
///
/// in .h:
///    class MyClass {
///
///        ...
///
///        static std::shared_ptr<ofxMidiOut> s_midiOut;
///    }
///
/// in .cpp:
///    std::shared_ptr<ofxMidiOut> MyClass::s_midiOut;
///
///    ...
///
///    // initialize somewhere else
///    void MyClass::setup() {
///        if(s_midiOut == NULL) {
///            s_midiOut = std::shared_ptr<ofxMidiOut>(new ofxMidiOut("ofxMidi Client"));
///        }
///    }
///
class ofxMidiOut {

public:

	/// set the output client name (optional)
	ofxMidiOut(const std::string name="ofxMidiOut Client", ofxMidiApi api=MIDI_API_DEFAULT);
	virtual ~ofxMidiOut();
	
/// \section Global Port Info
	
	/// print the connected output ports
	void listOutPorts();
	
	/// get a list of output port names
	/// 
	/// the vector index corresponds with the name's port number
	///
	/// note: this order may change when new devices are added/removed
	///       from the system
	///
	std::vector<std::string> getOutPortList();
	
	/// get the number of output ports
	int getNumOutPorts();
	
	/// get the name of an output port by it's number
	///
	/// returns "" if number is invalid
	///
	std::string getOutPortName(unsigned int portNumber);
	
/// \section Connection
	
	/// connect to an output port
	///
	/// setting port = 0 will open the first available
	///
	bool openPort(unsigned int portNumber=0);
	bool openPort(std::string deviceName);
	
	/// create and connect to a virtual output port (macOS and Linux ALSA only)
	///
	/// allows for connections between software
	///
	/// note: a connected virtual port has a portNum = -1
	/// note: an open virtual port ofxMidiOut object cannot see it's virtual
	///       own virtual port when listing ports
	///
	bool openVirtualPort(std::string portName="ofxMidi Virtual Output");
	
	/// close the port connection
	void closePort();
	
	/// get the port number if connected
	///
	/// returns -1 if not connected or this is a virtual port
	///
	int getPort();
	
	/// get the connected output port name
	///
	/// returns "" if not connected
	///
	std::string getName();
	
	/// returns true if connected
	bool isOpen();
	
	/// returns true if this is a virtual port
	bool isVirtual();
	
/// \section Sending
	
	/// MIDI events
	///
	/// number ranges:
	///     channel         1 - 16
	///     pitch           0 - 127
	///     velocity        0 - 127
	///     control value   0 - 127
	///     program value   0 - 127
	///     bend value      0 - 16383
	///     touch value     0 - 127
	///
	/// note:
	///     - a noteon with vel = 0 is equivalent to a noteoff
	///     - send velocity = 64 if not using velocity values
	///     - most synths don't use the velocity value in a noteoff
	///     - the lsb & msb for raw pitch bend bytes are 7 bit
	///
	/// references:
	///     http://www.srm.com/qtma/davidsmidispec.html
	///
	void sendNoteOn(int channel, int pitch, int velocity=64);
	void sendNoteOff(int channel, int pitch, int velocity=64);
	void sendControlChange(int channel, int control, int value);
	void sendProgramChange(int channel, int value);
	void sendPitchBend(int channel, int value);
	void sendPitchBend(int channel, unsigned char lsb, unsigned char msb);
	void sendAftertouch(int channel, int value);
	void sendPolyAftertouch(int channel, int pitch, int value);
	
	/// raw MIDI bytes
	///
	void sendMidiByte(unsigned char byte);
	void sendMidiBytes(std::vector<unsigned char>& bytes);
	
/// \section Sending Stream Interface
	
	/// MIDI events
	///
	/// midiOut << NoteOn(1, 64, 64) << NoteOff(1, 64);
	/// midiOut << ControlChange(1, 100, 64) << ProgramChange(1, 100);
	/// midiOut << << PitchBend(1, 2000);
	/// midiOut << Aftertouch(1, 127) << PolyAftertouch(1, 64, 127);
	///
	ofxMidiOut& operator<<(const NoteOn& var);
	ofxMidiOut& operator<<(const NoteOff& var);
	ofxMidiOut& operator<<(const ControlChange& var);
	ofxMidiOut& operator<<(const ProgramChange& var);
	ofxMidiOut& operator<<(const PitchBend& var);
	ofxMidiOut& operator<<(const Aftertouch& var);
	ofxMidiOut& operator<<(const PolyAftertouch& var);
	
	/// compound raw MIDI byte stream
	///
	/// midiOut << StartMidi() << 0x90 << 0x3C << 0x40 << FinishMidi();
	///
	/// build a raw MIDI byte message and send it with FinishMidi()
	///
	/// note: other MIDI messages (except raw MIDI bytes) cannot be sent while
	///       the stream is in progress
	///
	/// warning: this is not thread safe, use sendMidiBytes() in a shared context
	//
	ofxMidiOut& operator<<(const StartMidi& var);
	ofxMidiOut& operator<<(const FinishMidi& var);
	ofxMidiOut& operator<<(const unsigned char var);
	
private:
	
	std::shared_ptr<ofxBaseMidiOut> midiOut;
};
