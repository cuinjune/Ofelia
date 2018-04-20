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

#include "ofeliaFileHandler.h"

ofeliaFileHandler::ofeliaFileHandler()
{
    startThread();
}

ofeliaFileHandler::~ofeliaFileHandler()
{
    commands.close();
    waitForThread(true);
}

void ofeliaFileHandler::setJobDoneOutClock(t_clock *clock)
{
    jobDoneOutClock = clock;
}

void ofeliaFileHandler::createFile(const t_string &dstPath)
{
    ofFileHandlerEntry entry;
    entry.dstPath = dstPath;
    entry.cmdType = FILE_CREATE;
    commands.send(entry);
}

void ofeliaFileHandler::removeFile(const t_string &dstPath, const bool recursive)
{
    ofFileHandlerEntry entry;
    entry.dstPath = dstPath;
    entry.recursive = recursive;
    entry.cmdType = FILE_REMOVE;
    commands.send(entry);
}

void ofeliaFileHandler::renameFile(const t_string &srcPath, const t_string &dstPath, const bool overwrite)
{
    ofFileHandlerEntry entry;
    entry.srcPath = srcPath;
    entry.dstPath = dstPath;
    entry.overwrite = overwrite;
    entry.cmdType = FILE_RENAME;
    commands.send(entry);
}

void ofeliaFileHandler::copyFile(const t_string &srcPath, const t_string &dstPath, const bool overwrite)
{
    ofFileHandlerEntry entry;
    entry.srcPath = srcPath;
    entry.dstPath = dstPath;
    entry.overwrite = overwrite;
    entry.cmdType = FILE_COPY;
    commands.send(entry);
}

void ofeliaFileHandler::moveFile(const t_string &srcPath, const t_string &dstPath, const bool overwrite)
{
    ofFileHandlerEntry entry;
    entry.srcPath = srcPath;
    entry.dstPath = dstPath;
    entry.overwrite = overwrite;
    entry.cmdType = FILE_MOVE;
    commands.send(entry);
}

FileErrorType ofeliaFileHandler::getErrorType()
{
    return errorType;
}

void ofeliaFileHandler::threadedFunction()
{    
    ofFileHandlerEntry entry;
    
    while (commands.receive(entry)) {
        
        errorType = FILE_ERROR_NOERROR;
        
        switch (entry.cmdType) {
                
            case FILE_CREATE:
            {
                ofFile file(entry.dstPath.c_str());
                
                if (!file.create())
                    errorType = FILE_ERROR_FAILTOCREATE;
                break;
            }
            case FILE_REMOVE:
            {
                ofFile file(entry.dstPath.c_str());
                
                if (!file.remove(entry.recursive))
                    errorType = FILE_ERROR_FAILTOREMOVE;
                break;
            }
            case FILE_RENAME:
            {
                ofFile file(entry.srcPath.c_str());
                
                if (!file.renameTo(entry.dstPath.c_str(), false, entry.overwrite))
                    errorType = FILE_ERROR_FAILTORENAME;
                break;
            }
            case FILE_COPY:
            {
                ofFile file(entry.srcPath.c_str());
                
                if (!file.copyTo(entry.dstPath.c_str(), false, entry.overwrite))
                    errorType = FILE_ERROR_FAILTOCOPY;
                break;
            }
            case FILE_MOVE:
            {
                ofFile file(entry.srcPath.c_str());
                
                if (!file.moveTo(entry.dstPath.c_str(), false, entry.overwrite))
                    errorType = FILE_ERROR_FAILTOMOVE;
                break;
            }
            default:
            {
                break;
            }
        }
        clock_delay(jobDoneOutClock, 0.0);
    }
}

