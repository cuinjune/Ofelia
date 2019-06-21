/*
 * Copyright (c) 2018 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxMidi for documentation
 *
 * Adapted from code written in Swift for the Hertz-Lab @ ZKM | Karlsruhe
 *
 */
#pragma once

#include <vector>
#include <thread>
#include "ofEvents.h"
#include "ofxMidiConstants.h"

/// MTC frame
struct ofxMidiTimecodeFrame {

	int hours = 0;   //< hours 0-23
	int minutes = 0; //< minutes 0-59
	int seconds = 0; //< seconds 0-59
	int frames = 0;  //< frames 0-29 (depending on framerate)
	unsigned char rate = 0x0; //< 0x0: 24, 0x1: 25, 0x2: 29.97, 0x3: 30

	/// get the framerate value in fps
	double getFps() const;

/// \section Util

	/// convert to a string: hh:mm:ss:ff
	std::string toString() const;

	/// convert to time in seconds
	double toSeconds() const;

	/// convert from time in seconds, uses default 24 fps
	void fromSeconds(double s);

	/// convert from time in seconds & framerate value (not fps!)
    void fromSeconds(double s, unsigned char r);
};

///
/// MIDI timecode message parser
///
/// currently input only, does not send timecode
///
class ofxMidiTimecode {

public:

	ofxMidiTimecode() {}
	virtual ~ofxMidiTimecode() {}

	/// update the timecode frame from a raw MIDI message,
	/// returns true if the frame is new
	bool update(std::vector<unsigned char> &message);

	/// reset current frame data
	void reset();

/// \section Status

	/// get the last complete (current) timecode frame
	ofxMidiTimecodeFrame getFrame() {return frame;}

/// \section Util

	/// framerate values
	enum Framerate : unsigned char {
		FRAMERATE_24      = 0x0,
		FRAMERATE_25      = 0x1,
		FRAMERATE_30_DROP = 0x2, // 29.997 drop frame
		FRAMERATE_30      = 0x3
	};

	/// calculate ms from a frame count & framerate value
    static int framesToMs(int frames, unsigned char rate);

	/// calculate frame count from ms & framerate value
    static int msToFrames(int ms, unsigned char rate);

	/// returns the fps for a framerate value, ie. 0x0 -> 24 fps
    static double rateToFps(unsigned char rate);

	/// convert fps to the closest framerate value, ie. 24 fps -> 0x0
    static unsigned char fpsToRate(double fps);

	/// returns conversion multiplier from framerate value, ie. 1.0/fps
    static double rateToMultiplier(unsigned char rate);

protected:

	/// current frame, ie. last complete Quarter or Full Frame message
	ofxMidiTimecodeFrame frame;

	/// number of bytes in a FF message
	static const int FULLFRAME_LEN = 10;

	/// number of QF messages to make up a full MTC frame
	static const int QUARTERFRAME_LEN = 8;

	/// MTC quarter frame info
    struct QuarterFrame {

        /// detected time direction
        enum Direction {
            BACKWARDS = -1, //< time is moving backwards ie. rewinding
            UNKNOWN   =  0, //< unknown so far
            FORWARDS  =  1  //< time is advancing
        };

        // data
        ofxMidiTimecodeFrame frame;

        // protocol handling
        unsigned int count = 0; //< current received QF message count
        bool receivedFirst = false; //< did we receive the first message? (0x0* frames)
        bool receivedLast = false; //< did we receive the last message? (0x7* hours)
        unsigned int lastDataByte = 0x00; //< last received data byte for direction detection
        Direction direction = UNKNOWN; //< forwards or backwards?
    };

	/// current quarter frame info
	QuarterFrame quarterFrame;

	/// decode a Quarter Frame message, update when we have a full set of 8 messages
    ///
    /// if we receive too many, just overwrite until we have both the first and last
    /// messages (aka 0xF10* & 0xF17*) since a DAW could stop in the middle and start
    /// a new set of Quarter Frame messages based on user input aka start/stop
    ///
    /// also try to detect direction based on last receeved byte,
    /// this should hopefully handle both forwards and backwards playback:
    /// * forwards:  0xF10*, 0xF11*, 0xF12*, 0xF13*, 0xF14*, 0xF15*, 0xF16*, 0xF17*
    /// * backwards: 0xF17*, 0xF16*, 0xF15*, 0xF14*, 0xF13*, 0xF12*, 0xF11*, 0xF10*
    ///
    /// note: Quarter Frame state is cleared when a Full Frame message is received
    ///
    /// returns true if a complete quarter frame was decoded
    bool decodeQuarterFrame(std::vector<unsigned char> &message);

	/// decode a FF message: F0 7F 7F 01 01 hh mm ss ff F7
	/// returns true if a full frame was decoded
    bool decodeFullFrame(std::vector<unsigned char> &message);

	/// check if all bytes are correct
    bool isFullFrame(std::vector<unsigned char> &message);
};
