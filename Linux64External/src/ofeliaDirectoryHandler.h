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

#ifndef ofeliaDirectoryHandler_h
#define ofeliaDirectoryHandler_h

#pragma once

#include "ofeliaBase.h"

/* enumerations */
enum DirectoryCmdType {
    
    DIR_CREATE,
    DIR_REMOVE,
    DIR_RENAME,
    DIR_COPY,
    DIR_MOVE
};
enum DirectoryErrorType {
    
    DIR_ERROR_NOERROR,
    DIR_ERROR_FAILTOCREATE,
    DIR_ERROR_FAILTOREMOVE,
    DIR_ERROR_FAILTORENAME,
    DIR_ERROR_FAILTOCOPY,
    DIR_ERROR_FAILTOMOVE
};

class ofeliaDirectoryHandler: public ofThread {
    
public:
    
    ofeliaDirectoryHandler();
    ~ofeliaDirectoryHandler();
    
    void setJobDoneOutClock(t_clock *clock);
    void createDirectory(const t_string &dstPath, const bool recursive);
    void removeDirectory(const t_string &dstPath, const bool recursive);
    void renameDirectory(const t_string &srcPath, const t_string &dstPath, bool overwrite);
    void copyDirectory(const t_string &srcPath, const t_string &dstPath, bool overwrite);
    void moveDirectory(const t_string &srcPath, const t_string &dstPath, bool overwrite);
    DirectoryErrorType getErrorType();
    
    struct ofDirectoryHandlerEntry {
        
        t_string srcPath;
        t_string dstPath;
        bool recursive;
        bool overwrite;
        DirectoryCmdType cmdType;
    };
    
private:
    
    t_clock *jobDoneOutClock;
    ofThreadChannel<ofDirectoryHandlerEntry> commands;
    virtual void threadedFunction();
    DirectoryErrorType errorType;
};

#endif /* ofeliaDirectoryHandler_h */
