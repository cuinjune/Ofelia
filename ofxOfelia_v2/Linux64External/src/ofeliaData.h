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

#ifndef ofeliaData_h
#define ofeliaData_h

#pragma once

#include "m_pd.h"
#include "g_canvas.h"
#include "ofeliaLua.h"
#include "ofeliaSignal.h"
#include "ofeliaIO.h"
#include "ofeliaTextBuf.h"

extern "C"
{
    class ofeliaData
    {
    public:
        ofeliaData()
        :binbuf(nullptr)
        ,canvas(nullptr)
        ,guiconnect(nullptr)
        ,sym(&s_)
        ,shouldKeep(false)
        ,isSignalObject(false)
        ,isDirectMode(false)
        ,lua(this)
        ,signal(this)
        ,io(this)
        ,textBuf(this){};
        ~ofeliaData(){};

        void initCanvas();
        void initTextBuf();
        void initSym();
        void argParse(int argc, t_atom *argv, t_symbol *s, bool define);
        static ofeliaData *getPtr(t_symbol *s);
        
        t_object ob;
        t_binbuf *binbuf;
        t_canvas *canvas;
        t_guiconnect *guiconnect;
        t_symbol *sym;
        t_symbol *getUniqueSym();
        
        /* states */
        bool shouldKeep; /* whether to embed contents in patch on save */
        bool isSignalObject; /* whether an object is a signal object or not */
        bool isDirectMode; /* whether to write script directly on object */
        
        ofeliaLua lua;
        ofeliaSignal signal;
        ofeliaIO io;
        ofeliaTextBuf textBuf;
    private:
    };
}

#endif /* ofeliaData_h */
