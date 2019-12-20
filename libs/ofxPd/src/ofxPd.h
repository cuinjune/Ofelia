/*
 * Copyright (c) 2011 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxPd for documentation
 *
 * This project uses libpd, copyrighted by Miller Puckette and others using the
 * "Standard Improved BSD License". See the file "LICENSE.txt" in src/pd.
 *
 * See http://gitorious.org/pdlib/pages/Libpd for documentation
 *
 */
#pragma once

#include <map>
#include <set>

#include "PdBase.hpp"

///
/// a Pure Data instance
///
/// references: https://github.com/libpd/libpd/wiki
///
/// note: libpd currently does not support multiple states and it is
///       suggested that you use only one ofxPd object at a time
///
/// also: see PdBase.h in src/pd/cpp for some functions which are not wrapped by
///       ofxPd and PdTypes.h for small Pd C++ Objects
///
/// differences from libpd C api and/or C++ wrapper:
///     - the ofxPd object is thread safe
///     - midi channels are 1-16 to match pd ranges
///     - pgm values are 1-128 to match [pgmin]/[pgmout] in pd
///     - init() takes numOutChannels first to match ofSoundStream
///
class ofxPd : public pd::PdBase, protected pd::PdReceiver, protected pd::PdMidiReceiver {

	public :

		ofxPd();
		virtual ~ofxPd();

	/// \section Initializing Pd

		/// initialize audio resources
		///
		/// set the audio latency by setting the libpd ticks per buffer:
		/// ticks per buffer * lib pd block size (always 64)
		///
		/// ie 4 ticks per buffer * 64 = buffer len of 512
		///
		/// you can call this again after loading patches & setting receivers
		/// in order to update the audio settings
		///
		/// the lower the number of ticks, the faster the audio processing
		/// if you experience audio dropouts (audible clicks), increase the
		/// ticks per buffer
		///
		/// set queued = true to use the built in ringbuffers for message and
		/// midi event passing, you will then need to call receiveMessages() and
		/// receiveMidi() in order to pass messages from the ringbuffers to your
		/// PdReceiver and PdMidiReceiver implementations
		///
		/// the queued ringbuffers are useful when you need to receieve events
		/// on a gui thread and don't want to use locking (aka the mutex)
		///
		bool init(const int numOutChannels, const int numInChannels,
		          const int sampleRate, const int ticksPerBuffer=32,
				  bool queued=false);

		/// clear resources, here for future proofing, currently does nothing
		void clear();

	/// \section Adding Search Paths

		/// add to the pd search path
		/// takes an absolute or relative path (in data folder)
		///
		/// note: fails silently if path not found
		///
		void addToSearchPath(const std::string& path);

		/// clear the current pd search path
		void clearSearchPath();

	/// \section Opening Patches

		/// the pd::Patch class is a wrapper around a unique pointer to an open
		/// instance, giving you access to the $0 value for that instance
		///
		/// if you are going to use multiple instances of a patch, you will need
		/// need to keep a Patch object in order to differentiate between them
		///
		/// see src/pd/cpp/PdTypes.hpp for more info

		/// open a patch file, takes an absolute or relative path (in data folder)
		/// returns a Patch object
		pd::Patch openPatch(const std::string& patch);

		/// open a patch file using the filename and path of an existing patch
		///
		/// // open a patch, don't need pd::Patch instance object
		/// pd.openPatch("apatch.pd", "/some/path");
		///
		/// set the filename within the patch object or use a previously opened
		/// object and create a new instance
		///
		/// // open an instance of "somefile.pd", save the instance in a pd::Patch
		/// Patch p2("somefile.pd", "/some/path");    // set file and path
		/// pd.openPatch(p2);
		///
		/// // open a new instance of "somefile.pd"
		/// Patch p3 = pd.openPatch(p2);
		///
		/// p2 and p3 now refer to 2 different instances of "somefile.pd" and 
		/// p2.dollarZero() & p3.dollarZero() should now return different ids
		///
		pd::Patch openPatch(pd::Patch& patch);

		/// close a patch file, takes the patch's basename (filename without extension),
		/// use this function if you've only opened 1 instance of the given patch
		void closePatch(const std::string& patch);

		/// close a patch file using a Patch object, clears the given Patch object
		/// does not affect other open instances of the same patch
		void closePatch(pd::Patch& patch);

	/// \section Audio Processing Control

		/// start/stop audio processing
		///
		/// note: in general, once started, you won't need to turn off audio processing
		///
		/// shortcuts for [; pd dsp 1( & [; pd dsp 0(
		///
		void computeAudio(bool state);
		void start();
		void stop();

	//// \section Message Receiving

		/// subscribe/unsubscribe to source names from libpd
		///
		/// aka the pd receive name
		///
		/// [r source]
		/// |
		///
		/// note: the global source (aka "") exists by default
		///
		void subscribe(const std::string& source);
		void unsubscribe(const std::string& source);
		bool exists(const std::string& source);
		void unsubscribeAll(); //< receivers will be unsubscribed from *all* sources

		/// process the interal message queue if using the ringbuffer:
		///
		/// internally, libpd will use a ringbuffer to pass messages & midi without
		/// needing to require locking (mutexes) if you call init() with queued = true
		///
		/// call these in your update() loop in order to receive waiting messages
		/// or midi data which are then sent to your PdReceiver & PdMidiReceiver
		///
		/// warning: if you call init() with queued = true and *do not* call these
		///          functions, you will *not* receive any messages!
		///
		/// void receiveMessages(); -> calls PdReceiver
		/// void receiveMidi();     -> calls PdMidiReceiver

		/// add/remove incoming event receiver
		///
		/// receivers automatically receive from *all* subscribed sources
		/// as well as print events
		///
		/// see receive/ignore for specific source receiving control
		///
		void addReceiver(pd::PdReceiver& receiver);
		void removeReceiver(pd::PdReceiver& receiver);
		bool receiverExists(pd::PdReceiver& receiver);
		void clearReceivers();	//< also unsubscribes all receivers

		/// set a receiver to receive/ignore a subscribed source from libpd
		///
		/// receive/ignore using a source name or "" for all sources,
		/// make sure to add the receiver and source first
		///
		/// note: the global source (aka "") is added by default
		/// note: ignoring the global source ignores *all* sources,
		///       so the receiver will not receive any message events,
		///		  but still get print events
		///
		/// also: use negation if you want to plug into all sources but one:
		///
		/// pd.receive(receiver);          // receive from *all*
		/// pd.ignore(receiver, "source"); // ignore "source"
		///
		void receiveSource(pd::PdReceiver& receiver, const std::string& source="");
		void ignoreSource(pd::PdReceiver& receiver, const std::string& source="");
		bool isReceivingSource(pd::PdReceiver& receiver, const std::string& source="");

	/// \section Midi Receiving

		/// add/remove incoming midi event receiver
		///
		/// receivers automatically receive from *all* incoming midi channels
		///
		/// see receive/ignore for specific source receiving control
		///
		void addMidiReceiver(pd::PdMidiReceiver& receiver);
		void removeMidiReceiver(pd::PdMidiReceiver& receiver);
		bool midiReceiverExists(pd::PdMidiReceiver& receiver);
		void clearMidiReceivers();

		/// set a receiver to receive/ignore an incoming midi channel
		///
		/// receive/ignore a specific midi channel or 0 for all channels,
		/// make sure to add the receiver first
		///
		/// note: midi bytes are sent to all receivers
		/// note: the global channel (aka 0) is added by default
		/// note: ignoring the global channel ignores *all* channels,
		///       so the receiver will not receive any midi events except for
		///       midi bytes
		///
		/// also: use negation if you want to plug into all channels but one:
		///
		/// pd.receiveMidi(midiReceiver);   // receive from *all* channels
		/// pd.ignoreMidi(midiReceiver, 2); // ignore channel 2
		///
		void receiveMidiChannel(pd::PdMidiReceiver& receiver, int channel=0);
		void ignoreMidiChannel(pd::PdMidiReceiver& receiver, int channel=0);
		bool isReceivingMidiChannel(pd::PdMidiReceiver& receiver, int channel=0);

	/// \section Sending Functions

		/// messages
		///
		/// pd.sendBang("test");
		/// pd.sendFloat("test", 1.23);
		/// pd.sendSymbol("test", "hello");
		///
		/// see PdBase.h for function declarations

		/// compound messages
		///
		/// pd.startMessage();
		/// pd.addSymbol("hello");
		/// pd.addFloat(1.23);
		/// pd.finishList("test");  // "test" is the reciever name in pd
		///
		/// sends [list hello 1.23( -> [r test],
		/// you will need to use the [list trim] object on the reciving end
		///
		/// finishMsg sends a typed message -> [; test msg1 hello 1.23(
		///
		/// pd.startMessage();
		/// pd.addSymbol("hello");
		/// pd.addFloat(1.23);
		/// pd.finishMessage("test", "msg1");
		///
		/// see PdBase.h for function declarations

		/// compound messages using the Pd List type
		///
		/// List list;
		/// list.addSymbol("hello");
		/// list.addFloat(1.23);
		/// pd.sendList("test", list);
		///
		/// sends [list hello 1.23( -> [r test]
		///
		/// clear the list:
		///
		/// list.clear();
		///
		/// stream operators work as well:
		///
		/// list << "hello" << 1.23;
		/// pd.sendMessage("test", "msg1", list);
		///
		/// sends a typed message -> [; test msg1 hello 1.23(
		///
		/// see PdBase.h for function declarations

		/// midi
		///
		/// send midi messages, any out of range messages will be silently ignored
		///
		/// number ranges:
		/// channel     1 - 16 * dev# (dev #0: 1-16, dev #1: 17-32, etc)
		/// pitch       0 - 127
		/// velocity    0 - 127
		/// control value   0 - 127
		/// program value   1 - 128
		/// bend value     -8192 - 8191
		/// touch value     0 - 127
		///
		/// note, in pd:
		/// [bendin] takes 0 - 16383 while [bendout] returns -8192 - 8192
		/// [pgmin] and [pgmout] are 1 - 128
		///
		void sendNoteOn(const int channel, const int pitch, const int velocity=64);
		void sendControlChange(const int channel, const int controller, const int value);
		void sendProgramChange(const int channel, const int value);
		void sendPitchBend(const int channel, const int value);
		void sendAftertouch(const int channel, const int value);
		void sendPolyAftertouch(const int channel, const int pitch, const int value);

	/// \section Sending Stream Interface

		/// single messages
		///
		/// pd << Bang("test"); /// "test" is the reciever name in pd
		/// pd << Float("test", 100);
		/// pd << Symbol("test", "a symbol");
		///
		/// compound messages
		///
		/// pd << StartMessage() << 100 << 1.2 << "a symbol" << FinishList("test");
		///
		/// midi
		///
		/// pd << NoteOn(64) << NoteOn(64, 60) << NoteOn(64, 60, 1);
		/// pd << ControlChange(100, 64) << ProgramChange(100, 1) << PitchBend(2000, 1);
		/// pd << Aftertouch(127, 1) << PolyAftertouch(64, 127, 1);
		///
		/// compound raw midi byte stream
		///
		/// pd << StartMidi() << 0xEF << 0x45 << Finish();
		/// pd << StartSysex() << 0xE7 << 0x45 << 0x56 << 0x17 << Finish();
		///
		/// see PdBase.h for function declarations

	/// \section Array Access

		/// get array size
		///
		/// int s = pd.arraySize("array1");
		///
		/// read an array into a float vector
		///
		/// vector<float> array1;
		/// readArray("array1", array1);
		///
		/// write a float vector to an array
		///
		/// writeArray("array1", array1);
		///
		/// see PdBase.h for function declarations

	/// \section Utils

		/// has this pd instance been initialized?
		/// bool isInited();
		///
		/// get the blocksize of pd (sample length per channel)
		/// static int blockSize();
		///
		/// get/set the max length of messages and lists, default: 32
		/// void setMaxMsgLength(unsigned int len);
		/// unsigned int maxMsgLength();
		///
		/// see PdBase.h for function declarations
	
		/// get the current ticks per buffer,
		/// updated if the buffer size changes in audioIn or audioOut
		int ticksPerBuffer();

		/// get the current buffer size: ticks per buffer * blockSize(),
		/// updated if the buffer size changes in audioIn or audioOut
		int bufferSize();
	
		/// get the current sample rate
		int sampleRate();
	
		/// get the number of input channels,
		/// updated if the number changes in audioIn()
		int numInChannels();
	
		/// get the number of output channels,
		/// updated if the number changes in audioOut()
		int numOutChannels();
	
		/// check if ofxPd is currently computing audio
		bool isComputingAudio();

	/// \section Audio Processing Callbacks

		/// audio settings will be reinited if the buffersize or number of
		/// channels changes, will produce a verbose print for debugging as well
		///
		/// note: the libpd processing is done in the audioOut callback
		virtual void audioIn(float * input, int bufferSize, int nChannels);
		virtual void audioOut(float * output, int bufferSize, int nChannels);

	protected:

		/// message callbacks
		void print(const std::string& message);
		void receiveBang(const std::string& dest);
		void receiveFloat(const std::string& dest, float value);
		void receiveSymbol(const std::string& dest, const std::string& symbol);
		void receiveList(const std::string& dest, const pd::List& list);
		void receiveMessage(const std::string& dest, const std::string& msg, const pd::List& list);

		/// midi callbacks
		void receiveNoteOn(const int channel, const int pitch, const int velocity);
		void receiveControlChange(const int channel, const int controller, const int value);
		void receiveProgramChange(const int channel, const int value);
		void receivePitchBend(const int channel, const int value);
		void receiveAftertouch(const int channel, const int value);
		void receivePolyAftertouch(const int channel, const int pitch, const int value);
		void receiveMidiByte(const int port, const int byte);

	private:

		int ticks; //< number of ticks per buffer
		int bsize; //< current buffer size aka tbp*blocksize
		int srate; //< current sample rate
		int inChannels, outChannels; //< current num of input & output channels
		bool computing; //< is compute audio on?
	
		float * inBuffer; //< interleaved input audio buffer

		/// a receiving source's pointer and receivers
		struct Source {

			std::set<pd::PdReceiver*> receivers; //< receivers

			// helper functions
			void addReceiver(pd::PdReceiver* receiver) {
				receivers.insert(receiver);
			}

			void removeReceiver(pd::PdReceiver* receiver) {
				std::set<pd::PdReceiver*>::iterator iter;
				iter = receivers.find(receiver);
				if(iter != receivers.end())
					receivers.erase(iter);
			}

			bool receiverExists(pd::PdReceiver* receiver) {
				if(receivers.find(receiver) != receivers.end())
					return true;
				return false;
			}
		};

		std::set<pd::PdReceiver*> receivers;    //< the receivers
		std::map<std::string,Source> sources;   //< subscribed sources
		                                        //< first object always global

		/// a receiving midi channel's receivers
		struct Channel {

			std::set<pd::PdMidiReceiver*> receivers; //< receivers

			// helper functions
			void addMidiReceiver(pd::PdMidiReceiver* receiver) {
				receivers.insert(receiver);
			}

			void removeMidiReceiver(pd::PdMidiReceiver* receiver) {
				std::set<pd::PdMidiReceiver*>::iterator iter;
				iter = receivers.find(receiver);
				if(iter != receivers.end())
					receivers.erase(iter);
			}

			bool midiReceiverExists(pd::PdMidiReceiver* receiver) {
				if(receivers.find(receiver) != receivers.end())
					return true;
				return false;
			}
		};

		std::set<pd::PdMidiReceiver*> midiReceivers;  //< the midi receivers
		std::map<int,Channel> channels;               //< subscribed channels
		                                              //< first object always global
};
