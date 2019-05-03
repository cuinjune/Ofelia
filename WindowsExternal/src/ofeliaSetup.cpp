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

#include "ofeliaSetup.h"
#include "ofeliaLua.h"
#include "ofeliaDefine.h"
#include "ofeliaGet.h"
#include "ofeliaSet.h"
#include "ofeliaExists.h"
#include "ofeliaCreator.h"

void ofelia_setup()
{
    if (!ofeliaLua::init()) return;
    ofeliaDefine::setup();
    ofeliaGet::setup();
    ofeliaSet::setup();
    ofeliaExists::setup();
    ofeliaCreator::setup();
    
    /* print version info if loaded successfully */
    post("-------------------------------------------------------------------");
    post("ofelia v%d.%d : compiled on %s",
         OFELIA_MAJOR_VERSION, OFELIA_MINOR_VERSION, __DATE__);
    post("(c) 2018 Zack Lee <cuinjune@gmail.com>");
    post("-------------------------------------------------------------------");
}

