/*
 * Copyright (c) 2013 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxMidi for documentation
 *
 * references:
 *	http://www.srm.com/qtma/davidsmidispec.html
 */
#pragma once

#include "ofConstants.h"

// for RtMidi
#if defined(TARGET_LINUX)
    #ifndef __LINUX_ALSA__
        #define __LINUX_ALSA__
    #endif
#elif defined(TARGET_WIN32)
    #ifndef __WINDOWS_MM__
        #define __WINDOWS_MM__
    #endif
#elif defined(TARGET_MACOSX)
    #ifndef __MACOSX_CORE__
        #define __MACOSX_CORE__
    #endif
#endif

// api types, most of these match RtMidi::Api enums
enum ofxMidiApi {
	MIDI_API_DEFAULT,      // choose platform default
	MIDI_API_COREMIDI,     // CoreMidi macOS or iOS
	MIDI_API_ALSA,         // ALSA Linux
	MIDI_API_JACK,         // JACK
	MIDI_API_WINDOWS_MM    // Windows Multimedia MIDI
};

// MIDI status bytes
enum MidiStatus {

    MIDI_UNKNOWN            = 0x00,

    // channel voice messages
    MIDI_NOTE_OFF           = 0x80,
    MIDI_NOTE_ON            = 0x90,
    MIDI_CONTROL_CHANGE     = 0xB0,
    MIDI_PROGRAM_CHANGE     = 0xC0,
    MIDI_PITCH_BEND         = 0xE0,
    MIDI_AFTERTOUCH         = 0xD0, // aka channel pressure
    MIDI_POLY_AFTERTOUCH    = 0xA0, // aka key pressure

    // system messages
    MIDI_SYSEX              = 0xF0,
    MIDI_TIME_CODE          = 0xF1,
    MIDI_SONG_POS_POINTER   = 0xF2,
    MIDI_SONG_SELECT        = 0xF3,
    MIDI_TUNE_REQUEST       = 0xF6,
    MIDI_SYSEX_END          = 0xF7,
    MIDI_TIME_CLOCK         = 0xF8,
    MIDI_START              = 0xFA,
    MIDI_CONTINUE           = 0xFB,
    MIDI_STOP               = 0xFC,
    MIDI_ACTIVE_SENSING     = 0xFE,
    MIDI_SYSTEM_RESET       = 0xFF
};

// number range defines
// because it's sometimes hard to remember these  ...
#define MIDI_MIN_BEND       0
#define MIDI_MAX_BEND       16383
