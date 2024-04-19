/*
 * Copyright (c) 2013-2023 Dan Wilcox <danomatika@gmail.com>
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
	#include "ios/ofxPGMidiIn.h"
	#define OFX_MIDI_IN_TYPE ofxPGMidiIn
#else // OSX, Win, Linux
	#include "desktop/ofxRtMidiIn.h"
	#define OFX_MIDI_IN_TYPE ofxRtMidiIn
#endif

///
/// a MIDI input port
///
/// create multiple instances to connect to multiple ports
///
/// message passing is either direct on the MIDI thread or queued via thread channel:
/// * direct: add an ofxMidiListener instance, messages sent to newMidiMessage()
/// * queued: manually handle messages using hasWaitingMessages()/getNextMessage()
///
/// queued message passing is enabled by default and is disabled when a listener
/// is added
///
/// warning: *do not* create static instances as this will lead to a crash on
/// Linux, instead create a static std::shared_pttr and initialize it later:
///
/// in .h:
///    class MyClass {
///
///        ...
///
///        static std::shared_ptr<ofxMidiIn> s_midiIn;
///    }
///
/// in .cpp:
///    std::shared_ptr<ofxMidiIn> MyClass::s_midiIn;
///
///    ...
///
///    // initialize somewhere else
///    void MyClass::setup() {
///        if(s_midiIn == nullptr) {
///            s_midiIn = std::shared_ptr<ofxMidiIn>(new ofxMidiIn("ofxMidi Client"));
///        }
///    }
///
class ofxMidiIn {

public:

	/// set the input client name (optional) and api (optional)
	ofxMidiIn(const std::string name="ofxMidiIn Client", ofxMidiApi api=MIDI_API_DEFAULT);
	virtual ~ofxMidiIn();
	
/// \section Global Port Info
	
	/// print the connected input ports
	void listInPorts();
	
	/// get a list of input port names
	/// 
	/// the vector index corresponds with the name's port number
	///
	/// note: this order may change when new devices are added/removed
	///       from the system
	///
	std::vector<std::string> getInPortList();
	
	/// get the number of input ports
	int getNumInPorts();
	
	/// get the name of an input port by it's number
	///
	/// returns "" if number is invalid
	///
	std::string getInPortName(unsigned int portNumber);

/// \section Connection
	
	/// connect to an input port
	///
	/// setting port = 0 will open the first available
	///
	bool openPort(unsigned int portNumber=0);

	/// connect to an input port by device name
	bool openPort(std::string deviceName);
	
	/// create and connect to a virtual output port (macOS and Linux ALSA only)
	///
	/// allows for connections between software
	///
	/// note: a connected virtual port has a portNum = -1
	/// note: an open virtual port ofxMidiIn object cannot see it's virtual
	///       own virtual port when listing ports
	///
	bool openVirtualPort(std::string portName="ofxMidi Virtual Input");
	
	/// close the port connection
	void closePort();
	
	/// get the port number if connected
	///
	/// returns -1 if not connected or this is a virtual port
	///
	int getPort();
	
	/// get the connected input port name
	///
	/// returns "" if not connected
	///
	std::string getName();
	
	/// returns true if connected
	bool isOpen();
	
	/// returns true if this is a virtual port
	bool isVirtual();

	/// returns true if queued message passing is enabled
	bool isQueued();
	
/// \section Receiving

	/// specify if certain message types should be ignored
	///
	/// sysex messages are ignored by default
	///
	/// timing and active sensing messages have high data rates
	/// and are ignored by default 
	///
	void ignoreTypes(bool midiSysex=true, bool midiTiming=true, bool midiSense=true);

	/// add listener for incoming MIDI events
	///
	/// listeners receive from *all* incoming MIDI channels,
	/// listeners receive on the midi thread
	///
	/// note: adding a listener disables queued message passing
	///
	void addListener(ofxMidiListener *listener);

	/// remove listener for incoming MIDI events
	///
	/// listeners receive from *all* incoming MIDI channels
	///
	/// note: queued message passing re-enabled on last listener removal
	///
	void removeListener(ofxMidiListener *listener);

	/// returns true if there are any messages waiting in the queue
	///
	/// note: queued message passing disabled if any listeners added
	///
	/// returns true if there are any messages waiting
	bool hasWaitingMessages() const;

	/// remove a message from the queue and copy it's data into message
	///
	/// note: queued message passing disabled if any listeners added
	///
	/// returns false if there are no waiting messages, otherwise true
	bool getNextMessage(ofxMidiMessage &message);

	/// set to verbose = true to print received byte info
	///
	/// warning: this will impact performance with large numbers
	///          of MIDI messages
	///
	void setVerbose(bool verbose);

private:

	std::shared_ptr<ofxBaseMidiIn> midiIn;
};
