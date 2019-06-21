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

#include "ofxMidiConstants.h"
#include "ofxMidiMessage.h"
#include "ofxMidiTypes.h"

/// a base MIDI input port
///
/// see ofxMidiIn for functional documentation
///
class ofxBaseMidiIn {

public:

	ofxBaseMidiIn(const std::string name, ofxMidiApi api);
	virtual ~ofxBaseMidiIn() {}
	
	virtual bool openPort(unsigned int portNumber) = 0;
	virtual bool openPort(std::string deviceName) = 0;
	virtual bool openVirtualPort(std::string portName) = 0;
	virtual void closePort() = 0;

	virtual void listInPorts() = 0;
	virtual std::vector<std::string> getInPortList() = 0;
	virtual int getNumInPorts() = 0;
	virtual std::string getInPortName(unsigned int portNumber) = 0;

	int getPort();
	std::string getName();
	bool isOpen();
	bool isVirtual();
	ofxMidiApi getApi();

	virtual void ignoreTypes(bool midiSysex=true, bool midiTiming=true,
	                         bool midiSense=true) = 0;

	void addListener(ofxMidiListener *listener);
	void removeListener(ofxMidiListener *listener);

	void setVerbose(bool verbose);

protected:

	/// parses and sends received raw messages to listeners
	void manageNewMessage(double deltatime, std::vector<unsigned char> *message);
	
	int portNum;     //< current port num, -1 if not connected
	std::string portName; //< current port name, "" if not connected

	ofEvent<ofxMidiMessage> newMessageEvent; //< current message event
	
	bool bOpen;     //< is the port currently open?
	bool bVerbose;  //< print incoming bytes?
	bool bVirtual;  //< are we connected to a virtual port?
	ofxMidiApi api; //< backend api
};

/// a MIDI output port
///
/// see ofxMidiOut for functional documentation
///
class ofxBaseMidiOut {

public:

	ofxBaseMidiOut(const std::string name, ofxMidiApi api);
	virtual ~ofxBaseMidiOut() {}
	
	virtual bool openPort(unsigned int portNumber=0) = 0;
	virtual bool openPort(std::string deviceName) = 0;
	virtual bool openVirtualPort(std::string portName) = 0;
	virtual void closePort() = 0;

	virtual void listOutPorts() = 0;
	virtual std::vector<std::string> getOutPortList() = 0;
	virtual int getNumOutPorts() = 0;
	virtual std::string getOutPortName(unsigned int portNumber) = 0;
	
	int getPort();
	std::string getName();
	bool isOpen();
	bool isVirtual();
	ofxMidiApi getApi();
	
	void sendNoteOn(int channel, int pitch, int velocity);
	void sendNoteOff(int channel, int pitch, int velocity);
	void sendControlChange(int channel, int control, int value);
	void sendProgramChange(int channel, int value);
	void sendPitchBend(int channel, int value);
	void sendPitchBend(int channel, unsigned char lsb, unsigned char msb);
	void sendAftertouch(int channel, int value);
	void sendPolyAftertouch(int channel, int pitch, int value);
	
	void sendMidiByte(unsigned char byte);
	void sendMidiBytes(std::vector<unsigned char>& bytes);
	
	void startMidiStream();
	void finishMidiStream();
	
protected:

	/// send a raw byte message
	virtual void sendMessage(std::vector<unsigned char> &message) = 0;
	
	int portNum;          //< current port num, -1 if not connected
	std::string portName; //< current port name, "" if not connected

	std::vector<unsigned char> stream; //< byte stream message byte buffer
	
	bool bOpen;             //< is the port currently open?
	bool bStreamInProgress; //< used with byte stream
	bool bVirtual;          //< are we connected to a virtual port?
	ofxMidiApi api;         //< backend api
};
