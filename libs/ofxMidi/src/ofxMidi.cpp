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
#include "ofxMidi.h"

// choose the MIDI backend
#ifdef TARGET_OF_IPHONE
	#include "ios/ofxPGMidiIn.h"
#endif
#include "ofLog.h"
#include <cmath>
#include <sstream>
#include <iomanip>

// -----------------------------------------------------------------------------
void ofxMidiConnectionListener::midiInputAdded(std::string name, bool isNetwork) {
	ofLogNotice("ofxMidi") << "input added: " << name << " network: " << isNetwork;
}

void ofxMidiConnectionListener::midiInputRemoved(std::string name, bool isNetwork) {
	ofLogNotice("ofxMidi") << "input removed: " << name << " network: " << isNetwork;
}

void ofxMidiConnectionListener::midiOutputAdded(std::string name, bool isNetwork) {
	ofLogNotice("ofxMidi") << "output added: " << name << " network: " << isNetwork;
}

void ofxMidiConnectionListener::midiOutputRemoved(std::string name, bool isNetwork) {
	ofLogNotice("ofxMidi") << "output removed: " << name << " network: " << isNetwork;
}

namespace ofxMidi {

// -----------------------------------------------------------------------------
// from Pure Data x_acoustics.c
float mtof(float note) {
	if(note <= -1500) return (0);
	else if(note > 1499) return (mtof(1499));
	else return (8.17579891564 * exp(.0577622650 * note));
}

// from Pure Data x_acoustics.c
float ftom(float frequency) {
	return (frequency > 0 ? 17.3123405046 * log(.12231220585 * frequency) : -1500);
}

// -----------------------------------------------------------------------------
std::string bytesToString(std::vector<unsigned char> &bytes) {
	std::stringstream stream;
	stream << std::hex << std::uppercase << std::setw(2) << std::setfill('0');
	for(unsigned int i = 0; i < bytes.size(); ++i) {
		stream << (int)bytes[i];
		if(i < bytes.size()-1) {
			stream << " ";
		}
	}
	return stream.str();
}

// -----------------------------------------------------------------------------
void setConnectionListener(ofxMidiConnectionListener *listener) {
	#ifdef TARGET_OF_IPHONE
		ofxPGMidiIn::setConnectionListener(listener);
	#endif
}

// -----------------------------------------------------------------------------
void clearConnectionListener() {
	#ifdef TARGET_OF_IPHONE
		ofxPGMidiIn::clearConnectionListener();
	#endif
}

// -----------------------------------------------------------------------------
void enableNetworking() {
	#ifdef TARGET_OF_IPHONE
		ofxPGMidiIn::enableNetworking();
	#endif
}

}; // namespace
