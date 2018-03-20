/*==============================================================================
	ofelia: OpenFrameworks as an External Library for Interactive Applications
 
	Copyright (c) 2018 Zack Lee <cuinjune@gmail.com>
 
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.
 
	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.
	
	See https://github.com/cuinjune/ofxOfelia for documentation
 ==============================================================================*/

/*==============================================================================
	The implementation of biquad filters is based on the work of
 
	Simple implementation of Biquad filters
	by Tom St Denis
	http://www.musicdsp.org/files/biquad.c
 
	Cookbook formulae for audio EQ biquad filter coefficients
	by Robert Bristow-Johnson  <rbj@audioimagination.com>
	http://www.musicdsp.org/files/Audio-EQ-Cookbook.txt
 ==============================================================================*/

#ifndef ofeliaAudio_h
#define ofeliaAudio_h

#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus)
extern "C" {
#endif

    /* setup methods */
    void ofeliaAudio_setup();
    
#if defined(_LANGUAGE_C_PLUS_PLUS) || defined(__cplusplus)
}
#endif

#endif /* ofeliaAudio_h */
