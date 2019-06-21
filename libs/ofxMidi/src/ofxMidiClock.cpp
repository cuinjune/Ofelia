/*
 * Copyright (c) 2018 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxMidi for documentation
 *
 * Adapted from code written in Swift for ZKM | Karlsruhe
 *
 */
#include "ofxMidiClock.h"

#include "ofLog.h"

using namespace std::chrono;

// -----------------------------------------------------------------------------
ofxMidiClock::ofxMidiClock() {
	reset();
}

// -----------------------------------------------------------------------------
bool ofxMidiClock::update(std::vector<unsigned char> &message) {
	switch(message[0]) {
		case MIDI_TIME_CLOCK:
			tick();
			return true;
		case MIDI_SONG_POS_POINTER:
			if(message.size() < 3) {return false;}
			unsigned int beats = (message[2] << 7) + message[1];
			ticks = beats * 6;
			ofLogVerbose("ofxMidiClock") << "Song Pos " << beats;
			return true;
	}
	return false;
}

// -----------------------------------------------------------------------------
void ofxMidiClock::tick() {
	double us = duration_cast<microseconds>(steady_clock::now()-timestamp).count();
	if(us < 200000) { // filter obviously bad values
		length += ((us/1000.0) - length) / 5.0; // average last 5 ticks
		ticks++;
	}
	timestamp = steady_clock::now();
}

// -----------------------------------------------------------------------------
void ofxMidiClock::reset() {
	timestamp = steady_clock::now();
	ticks = 0;
}

/// Status

// -----------------------------------------------------------------------------
unsigned int ofxMidiClock::getBeats() {
	return ticks / 6;
}

// -----------------------------------------------------------------------------
void ofxMidiClock::setBeats(unsigned int beats) {
	ticks = beats * 6;
}

// -----------------------------------------------------------------------------
double ofxMidiClock::getSeconds() {
	return beatsToSeconds(ticks / 6);
}

// -----------------------------------------------------------------------------
void ofxMidiClock::setSeconds(double s) {
	ticks = secondsToBeats(s) * 6;
}

// -----------------------------------------------------------------------------
double ofxMidiClock::getBpm() {
	return msToBpm(length);
}

// -----------------------------------------------------------------------------
void ofxMidiClock::setBpm(double bpm) {
	length = bpmToMs(bpm);
}

// Util

// -----------------------------------------------------------------------------
double ofxMidiClock::beatsToSeconds(unsigned int beats) {
	return (double)beats * length * 0.006; // 6 / 1000.0;
}

// -----------------------------------------------------------------------------
unsigned int ofxMidiClock::secondsToBeats(double seconds) {
	return (seconds * 1000) / (6 * length);
}

// -----------------------------------------------------------------------------
double ofxMidiClock::bpmToMs(double bpm) {
	if(bpm == 0) {return 0;}
	return 2500.0 / bpm; // 1000.0 / ((bpm * 24.0) / 60.0);
}

// -----------------------------------------------------------------------------
double ofxMidiClock::msToBpm(double ms) {
	if(ms == 0) {return 0;}
	return 2500.0 / ms; // (1000.0 / ms / 24.0) * 60.0;
}
