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

#ifndef ofeliaDefine_h
#define ofeliaDefine_h

#pragma once

#include "m_pd.h"
#include "ofeliaData.h"

extern "C"
{
    class ofeliaDefine
    {
    public:
        ofeliaDefine(){};
        ~ofeliaDefine(){};
        void *newMethod(t_symbol *s, int argc, t_atom *argv);
        void bangMethod();
        void floatMethod(t_floatarg f);
        void symbolMethod(t_symbol *s);
        void pointerMethod(t_gpointer *p);
        void listMethod(t_symbol *s, int argc, t_atom *argv);
        void anythingMethod(t_symbol *s, int argc, t_atom *argv);
        void clearMethod();
        void setMethod(t_symbol *s, int argc, t_atom *argv);
        void saveMethod(t_object *ob, t_binbuf *bb);
        void dspMethod(t_signal **sp);
        void notifyMethod();
        void freeMethod();
        static void *newWrapper(t_symbol *s, int argc, t_atom *argv);
        static void bangWrapper(ofeliaDefine *x);
        static void floatWrapper(ofeliaDefine *x, t_floatarg f);
        static void symbolWrapper(ofeliaDefine *x, t_symbol *s);
        static void pointerWrapper(ofeliaDefine *x, t_gpointer *p);
        static void listWrapper(ofeliaDefine *x, t_symbol *s, int argc, t_atom *argv);
        static void anythingWrapper(ofeliaDefine *x, t_symbol *s, int argc, t_atom *argv);
        static void clearWrapper(ofeliaDefine *x);
        static void setWrapper(ofeliaDefine *x, t_symbol *s, int argc, t_atom *argv);
        static void saveWrapper(t_gobj *z, t_binbuf *bb);
        static void dspWrapper(ofeliaDefine *x, t_signal **sp);
        static void notifyWrapper(ofeliaDefine *x);
        static void openWrapper(ofeliaData *x);
        static void closeWrapper(ofeliaData *x);
        static void addLineWrapper(ofeliaData *x, t_symbol *s, int argc, t_atom *argv);
        static void readWrapper(ofeliaData *x, t_symbol *s, int argc, t_atom *argv);
        static void writeWrapper(ofeliaData *x, t_symbol *s, int argc, t_atom *argv);
        static void freeWrapper(ofeliaDefine *x);
        static void setup();
        static t_class *pdClass;
        ofeliaData data;
    private:
    };
}

#endif /* ofeliaDefine_h */
