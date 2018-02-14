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

#ifndef ofeliaURLSaver_h
#define ofeliaURLSaver_h

#pragma once

#include "ofeliaBase.h"

/* enumerations */
enum URLSaverErrorType {
    
    URLSAVER_ERROR_NOERROR,
    URLSAVER_ERROR_FAILTOSAVE
};

class ofeliaURLSaver: public ofThread {
    
public:
    
    ofeliaURLSaver();
    ~ofeliaURLSaver();
    
    void setJobDoneOutClock(t_clock *clock);
    void saveURL(const t_string &url, const t_string &path);
    void clear();
    URLSaverErrorType getErrorType();
    t_string getFailedURL();
    
    struct ofURLSaverEntry {
        
        t_string url;
        t_string path;
    };
    
private:
    
    t_clock *jobDoneOutClock;
    ofThreadChannel<ofURLSaverEntry> commands;
    virtual void threadedFunction();
    URLSaverErrorType errorType;
    t_string failedURL;
    bool shouldClear;
};

#endif /* ofeliaURLSaver_h */
