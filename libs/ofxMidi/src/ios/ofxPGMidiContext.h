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
#import "ofxPGMidiDelegate.h"

/// global static wrapper around PGMidi Obj-C instance
class ofxPGMidiContext {

	public:
	
		/// creates the PGMidi instance if not already existing
		static void setup();
		
		/// get the PGMidi instance
		static PGMidi* getMidi();
		
		/// set the listener for device (dis)connection events
		static void setConnectionListener(ofxMidiConnectionListener *listener);
		static void clearConnectionListener();
		
		/// enable the iOS CoreMidi network interface? 
		static void enableNetwork(bool enable=true);
	
	private:

		static NSAutoreleasePool *pool; // for happy garbage collection ...
		static PGMidi *midi; ///< global Obj-C PGMidi instance
		static ofxPGMidiDelegate *delegate; ///< device (dis)connection interface
};
