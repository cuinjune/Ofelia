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

#ifndef ofeliaFileHandler_h
#define ofeliaFileHandler_h

#pragma once

#include "ofeliaBase.h"

/* enumerations */
enum FileCmdType {
    
    FILE_CREATE,
    FILE_REMOVE,
    FILE_RENAME,
    FILE_COPY,
    FILE_MOVE
};
enum FileErrorType {
    
    FILE_ERROR_NOERROR,
    FILE_ERROR_FAILTOCREATE,
    FILE_ERROR_FAILTOREMOVE,
    FILE_ERROR_FAILTORENAME,
    FILE_ERROR_FAILTOCOPY,
    FILE_ERROR_FAILTOMOVE
};

class ofeliaFileHandler: public ofThread {
    
public:
    
    ofeliaFileHandler();
    ~ofeliaFileHandler();
    
    void setJobDoneOutClock(t_clock *clock);
    void createFile(const t_string &dstPath);
    void removeFile(const t_string &dstPath, const bool recursive);
    void renameFile(const t_string &srcPath, const t_string &dstPath, bool overwrite);
    void copyFile(const t_string &srcPath, const t_string &dstPath, bool overwrite);
    void moveFile(const t_string &srcPath, const t_string &dstPath, bool overwrite);
    FileErrorType getErrorType();
    
    struct ofFileHandlerEntry {
        
        t_string srcPath;
        t_string dstPath;
        bool recursive;
        bool overwrite;
        FileCmdType cmdType;
    };
    
private:
    
    t_clock *jobDoneOutClock;
    ofThreadChannel<ofFileHandlerEntry> commands;
    virtual void threadedFunction();
    FileErrorType errorType;
};

#endif /* ofeliaFileHandler_h */
