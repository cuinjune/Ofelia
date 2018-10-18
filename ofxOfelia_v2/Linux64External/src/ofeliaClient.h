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

#ifndef ofeliaClient_h
#define ofeliaClient_h

#pragma once

#include "m_pd.h"
#include "ofeliaData.h"

extern "C"
{
    class ofeliaClient
    {
    public:
        ofeliaClient(){};
        ~ofeliaClient(){};
        void bangMethod();
        void floatMethod(t_floatarg f);
        void symbolMethod(t_symbol *s);
        void pointerMethod(t_gpointer *p);
        void listMethod(t_symbol *s, int argc, t_atom *argv);
        void getVariableByArgs(t_symbol *s, int argc, t_atom *argv);
        void setVariableByArgs(t_symbol *s, int argc, t_atom *argv);
        void dspMethod(t_signal **sp);
        void freeMethod();
        ofeliaData data;
    private:
        bool isDataValid();
    };
}

#endif /* ofeliaClient_h */
