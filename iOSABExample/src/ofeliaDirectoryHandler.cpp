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

#include "ofeliaDirectoryHandler.h"

ofeliaDirectoryHandler::ofeliaDirectoryHandler()
{
    startThread();
}

ofeliaDirectoryHandler::~ofeliaDirectoryHandler()
{
    commands.close();
    waitForThread(true);
}

void ofeliaDirectoryHandler::setJobDoneOutClock(t_clock *clock)
{
    jobDoneOutClock = clock;
}

void ofeliaDirectoryHandler::createDirectory(const t_string &dstPath, const bool recursive)
{
    ofDirectoryHandlerEntry entry;
    entry.dstPath = dstPath;
    entry.recursive = recursive;
    entry.cmdType = DIR_CREATE;
    commands.send(entry);
}

void ofeliaDirectoryHandler::removeDirectory(const t_string &dstPath, const bool recursive)
{
    ofDirectoryHandlerEntry entry;
    entry.dstPath = dstPath;
    entry.recursive = recursive;
    entry.cmdType = DIR_REMOVE;
    commands.send(entry);
}

void ofeliaDirectoryHandler::renameDirectory(const t_string &srcPath, const t_string &dstPath, const bool overwrite)
{
    ofDirectoryHandlerEntry entry;
    entry.srcPath = srcPath;
    entry.dstPath = dstPath;
    entry.overwrite = overwrite;
    entry.cmdType = DIR_RENAME;
    commands.send(entry);
}

void ofeliaDirectoryHandler::copyDirectory(const t_string &srcPath, const t_string &dstPath, const bool overwrite)
{
    ofDirectoryHandlerEntry entry;
    entry.srcPath = srcPath;
    entry.dstPath = dstPath;
    entry.overwrite = overwrite;
    entry.cmdType = DIR_COPY;
    commands.send(entry);
}

void ofeliaDirectoryHandler::moveDirectory(const t_string &srcPath, const t_string &dstPath, const bool overwrite)
{
    ofDirectoryHandlerEntry entry;
    entry.srcPath = srcPath;
    entry.dstPath = dstPath;
    entry.overwrite = overwrite;
    entry.cmdType = DIR_MOVE;
    commands.send(entry);
}

DirectoryErrorType ofeliaDirectoryHandler::getErrorType()
{
    return errorType;
}

void ofeliaDirectoryHandler::threadedFunction()
{    
    ofDirectoryHandlerEntry entry;
    
    while (commands.receive(entry)) {
        
        errorType = DIR_ERROR_NOERROR;
        
        switch (entry.cmdType) {
                
            case DIR_CREATE:
            {
                ofDirectory dir(entry.dstPath.c_str());
                
                if (!dir.create(entry.recursive))
                    errorType = DIR_ERROR_FAILTOCREATE;
                break;
            }
            case DIR_REMOVE:
            {
                ofDirectory dir(entry.dstPath.c_str());
                
                if (!dir.remove(entry.recursive))
                    errorType = DIR_ERROR_FAILTOREMOVE;
                break;
            }
            case DIR_RENAME:
            {
                ofDirectory dir(entry.srcPath.c_str());
                
                if (!dir.renameTo(entry.dstPath.c_str(), false, entry.overwrite))
                    errorType = DIR_ERROR_FAILTORENAME;
                break;
            }
            case DIR_COPY:
            {
                ofDirectory dir(entry.srcPath.c_str());
                
                if (!dir.copyTo(entry.dstPath.c_str(), false, entry.overwrite))
                    errorType = DIR_ERROR_FAILTOCOPY;
                break;
            }
            case DIR_MOVE:
            {
                ofDirectory dir(entry.srcPath.c_str());
                
                if (!dir.moveTo(entry.dstPath.c_str(), false, entry.overwrite))
                    errorType = DIR_ERROR_FAILTOMOVE;
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

