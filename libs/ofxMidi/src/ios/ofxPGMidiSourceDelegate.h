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

#import "PGMidi.h"
#include <vector>

class ofxPGMidiIn;

/// interface to pgmidi input
@interface ofxPGMidiSourceDelegate : NSObject <PGMidiSourceDelegate> {
	
	ofxPGMidiIn *inputPtr; ///< object to send receieved midi messages to
	
	bool bIgnoreSysex, bIgnoreTiming, bIgnoreSense;	///< ignore midi types?
	
	unsigned long long lastTime; ///< timestamp from last packet
	bool bFirstPacket;           ///< is this the first received packet?
	bool bContinueSysex;         ///< is this packet part of a sysex message?
	unsigned int maxMessageLen;  ///< max size of the byte buffer
	
	std::vector<unsigned char> message; ///< raw byte buffer
}

/// pgmidi callback
- (void) midiSource:(PGMidiSource *)input midiReceived:(const MIDIPacketList *)packetList;

/// set the pointer to the ofxPGMidiIn object to send messages to
- (void) setInputPtr:(void *)p;

@property bool bIgnoreSysex;
@property bool bIgnoreTiming;
@property bool bIgnoreSense;

@end
