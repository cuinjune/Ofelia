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

class ofxMidiConnectionListener;

/// interface to global pgmidi events
@interface ofxPGMidiDelegate : NSObject <PGMidiDelegate> {
	ofxMidiConnectionListener *listenerPtr; ///< object to send receieved events to
}

- (void) midi:(PGMidi*)midi sourceAdded:(PGMidiSource *)source;
- (void) midi:(PGMidi*)midi sourceRemoved:(PGMidiSource *)source;
- (void) midi:(PGMidi*)midi destinationAdded:(PGMidiDestination *)destination;
- (void) midi:(PGMidi*)midi destinationRemoved:(PGMidiDestination *)destination;

/// set the pointer to the ofxMidiConnectionListener object to send messages to
- (void) setListenerPtr:(void *)p;

@end
