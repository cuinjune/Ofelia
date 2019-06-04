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

#ifndef ofeliaAliases_h
#define ofeliaAliases_h

#pragma once

#include "lua.hpp"

extern "C"
{
    class ofeliaAliases
    {
    public:
        ofeliaAliases();
        ~ofeliaAliases(){};
        
        /* make aliases of classes and functions in modules */
        static void makeAliases(lua_State *L);
    private:
    };
}

#endif /* ofeliaAliases_h */
