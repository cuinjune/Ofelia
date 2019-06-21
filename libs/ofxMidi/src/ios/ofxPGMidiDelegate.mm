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
#include "ofxPGMidiDelegate.h"

#include "ofMain.h"

#include "../ofxMidi.h"

// -----------------------------------------------------------------------------
@implementation ofxPGMidiDelegate

// -----------------------------------------------------------------------------
- (id) init {
	self = [super init];
	listenerPtr = NULL;
	return self;
}

// -----------------------------------------------------------------------------
- (void) midi:(PGMidi*)midi sourceAdded:(PGMidiSource *)source {
	if(listenerPtr) {
		listenerPtr->midiInputAdded([source.name UTF8String], source.isNetworkSession);
	}
}

// -----------------------------------------------------------------------------
- (void) midi:(PGMidi*)midi sourceRemoved:(PGMidiSource *)source {
	if(listenerPtr) {
		listenerPtr->midiInputRemoved([source.name UTF8String], source.isNetworkSession);
	}
}

// -----------------------------------------------------------------------------
- (void) midi:(PGMidi*)midi destinationAdded:(PGMidiDestination *)destination {
	if(listenerPtr) {
		listenerPtr->midiOutputAdded([destination.name UTF8String], destination.isNetworkSession);
	}
}

// -----------------------------------------------------------------------------
- (void) midi:(PGMidi*)midi destinationRemoved:(PGMidiDestination *)destination {
	if(listenerPtr) {
		listenerPtr->midiOutputRemoved([destination.name UTF8String], destination.isNetworkSession);
	}
}

// -----------------------------------------------------------------------------
- (void) setListenerPtr:(void *)p {
	listenerPtr = (ofxMidiConnectionListener*) p;
}

@end
