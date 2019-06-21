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
#include "ofxPGMidiContext.h"

#import "pgmidi/iOSVersionDetection.h"
#include "ofLog.h"

NSAutoreleasePool *ofxPGMidiContext::pool = nil;
PGMidi *ofxPGMidiContext::midi = nil;
ofxPGMidiDelegate *ofxPGMidiContext::delegate = nil;

// -----------------------------------------------------------------------------
void ofxPGMidiContext::setup() {
	if(midi != nil)
		return;
	IF_IOS_HAS_COREMIDI (
		pool = [[NSAutoreleasePool alloc] init];
		midi = [[PGMidi alloc] init];
		delegate = [[ofxPGMidiDelegate alloc] init];
		midi.delegate = delegate;
	)
}

// -----------------------------------------------------------------------------
PGMidi* ofxPGMidiContext::getMidi() {
	setup();
	return midi;
}

// -----------------------------------------------------------------------------
void ofxPGMidiContext::setConnectionListener(ofxMidiConnectionListener *listener) {
	[delegate setListenerPtr:(void*) listener];
}

// -----------------------------------------------------------------------------
void ofxPGMidiContext::clearConnectionListener() {
	[delegate setListenerPtr:NULL];
}

// -----------------------------------------------------------------------------
void ofxPGMidiContext::enableNetwork(bool enable) {
	if(enable) {
		midi.networkEnabled = YES;
		ofLogVerbose("ofxMidi") << "iOS Midi Networking enabled";
	}
	else {
		midi.networkEnabled = NO;
		ofLogVerbose("ofxMidi") << "iOS Midi Networking disabled";
	}
}
