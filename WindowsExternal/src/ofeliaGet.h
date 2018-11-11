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

#ifndef ofeliaGet_h
#define ofeliaGet_h

#pragma once

#include "m_pd.h"
#include "ofeliaClient.h"

extern "C"
{
    class ofeliaGet
    {
    public:
        ofeliaGet(){};
        ~ofeliaGet(){};
        void *newMethod(t_symbol *s, int argc, t_atom *argv);
        void bangMethod();
        void floatMethod(t_floatarg f);
        void symbolMethod(t_symbol *s);
        void pointerMethod(t_gpointer *p);
        void listMethod(t_symbol *s, int argc, t_atom *argv);
        void anythingMethod(t_symbol *s, int argc, t_atom *argv);
        void dspMethod(t_signal **sp);
        void freeMethod();
        static void *newWrapper(t_symbol *s, int argc, t_atom *argv);
        static void bangWrapper(ofeliaGet *x);
        static void floatWrapper(ofeliaGet *x, t_floatarg f);
        static void symbolWrapper(ofeliaGet *x, t_symbol *s);
        static void pointerWrapper(ofeliaGet *x, t_gpointer *p);
        static void listWrapper(ofeliaGet *x, t_symbol *s, int argc, t_atom *argv);
        static void anythingWrapper(ofeliaGet *x, t_symbol *s, int argc, t_atom *argv);
        static void dspWrapper(ofeliaGet *x, t_signal **sp);
        static void freeWrapper(ofeliaGet *x);
        static void setup();
        static t_class *pdClass;
        ofeliaClient client;
    private:
    };
}

#endif /* ofeliaGet_h */
