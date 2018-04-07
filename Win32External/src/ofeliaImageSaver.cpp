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

#include "ofeliaImageSaver.h"

ofeliaImageSaver::ofeliaImageSaver()
{
    startThread();
}

ofeliaImageSaver::~ofeliaImageSaver()
{
    commands.close();
    waitForThread(true);
}

void ofeliaImageSaver::setJobDoneOutClock(t_clock *clock)
{
    jobDoneOutClock = clock;
}

void ofeliaImageSaver::saveImage(ofImage &image, const t_string &filename, const ofImageQualityType quality)
{
    ofImageSaverEntry entry(image);
    entry.filename = filename;
    entry.quality = quality;
    commands.send(entry);
}

void ofeliaImageSaver::threadedFunction()
{    
    ofImageSaverEntry entry;
    
    while (commands.receive(entry)) {
        
        entry.image->save(entry.filename.c_str(), entry.quality);
        clock_delay(jobDoneOutClock, 0.0);
    }
}

