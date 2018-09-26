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

#ifndef ofeliaCreator_h
#define ofeliaCreator_h

#pragma once

#include "m_pd.h"

extern "C"
{
    class ofeliaCreator
    {
    public:
        ofeliaCreator(){};
        ~ofeliaCreator(){};
        static void *newWrapper(t_symbol *s, int argc, t_atom *argv);
        static void setup();
    private:
    };
}

#endif /* ofeliaCreator_h */
