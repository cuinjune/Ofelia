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

#include "ofeliaURLSaver.h"

ofeliaURLSaver::ofeliaURLSaver()
{
    startThread();
}

ofeliaURLSaver::~ofeliaURLSaver()
{
    commands.close();
    waitForThread(true);
}

void ofeliaURLSaver::setJobDoneOutClock(t_clock *clock)
{
    jobDoneOutClock = clock;
}

void ofeliaURLSaver::saveURL(const t_string &url, const t_string &path)
{
    ofURLSaverEntry entry;
    entry.url = url;
    entry.path = path;
    shouldClear = false;
    commands.send(entry);
}

void ofeliaURLSaver::clear()
{
    shouldClear = true;
}

URLSaverErrorType ofeliaURLSaver::getErrorType()
{
    return errorType;
}

t_string ofeliaURLSaver::getFailedURL()
{
    return failedURL;
}

void ofeliaURLSaver::threadedFunction()
{
    ofURLSaverEntry entry;
    
    while (commands.receive(entry)) {
        
        if (!shouldClear) {
            
            const ofHttpResponse &response = ofSaveURLTo(entry.url.c_str(), entry.path.c_str());
            
            if (response.error == "OK") {
                
                errorType = URLSAVER_ERROR_NOERROR;
            }
            else {
                
                errorType = URLSAVER_ERROR_FAILTOSAVE;
                failedURL = response.request.url.c_str();
            }
            clock_delay(jobDoneOutClock, 0.0);
        }
    }
}

