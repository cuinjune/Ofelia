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

#ifndef ofeliaExists_h
#define ofeliaExists_h

#pragma once

#include "m_pd.h"
#include "ofeliaClient.h"

extern "C"
{
    class ofeliaExists
    {
    public:
        ofeliaExists(){};
        ~ofeliaExists(){};
        void *newMethod(t_symbol *s, int argc, t_atom *argv);
        void bangMethod();
        static void *newWrapper(t_symbol *s, int argc, t_atom *argv);
        static void bangWrapper(ofeliaExists *x);
        static void freeWrapper(ofeliaExists *x);
        static void setup();
        static t_class *pdClass;
        ofeliaClient client;
    private:
    };
}

#endif /* ofeliaExists_h */
