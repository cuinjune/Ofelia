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

#ifndef ofeliaIO_h
#define ofeliaIO_h

#pragma once

#include "m_pd.h"

extern "C"
{
    class ofeliaData;
    class ofeliaIO
    {
    public:
        ofeliaIO(ofeliaData *dataPtr)
        :dataPtr(dataPtr)
        ,hasMultiControlInlets(false)
        ,hasMultiControlOutlets(false)
        ,hasControlOutlet(false){};
        ~ofeliaIO(){};
        void newControlIO(int numInlets, int numOutlets);
        void newSignalIO(int numInlets, int numOutlets);
        void freeControlIO();
        void freeSignalIO();
        void doList(int ac, t_atom *av);
        t_inlet **inlets;
        t_outlet **outlets;
        t_float *fv; /* variables for float inlets */
        int numInlets;
        int numOutlets;
        bool hasMultiControlInlets; /* whether an object has multiple control inlets */
        bool hasMultiControlOutlets; /* whether an object has multiple control outlets */
        bool hasControlOutlet; /* whether an object has any control outlet */
    private:
        void newIO(int ninlets, int noutlets);
        void freeIO(int ninlets, int noutlets);
        ofeliaData *dataPtr; /* pointer to data */
    };
}

#endif /* ofeliaIO_h */
