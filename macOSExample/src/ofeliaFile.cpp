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

#include "ofeliaFile.h"

/* ________________________________________________________________________________
 * static variables
 */
const char *t_ofeliaFile::objName = "ofFile";

/* ________________________________________________________________________________
 * ofFile object methods
 */
void ofeliaFile_jobDoneOut(t_ofeliaFile *x);

void *ofeliaFile_new()
{
    t_ofeliaFile *x = reinterpret_cast<t_ofeliaFile*>(pd_new(ofeliaFile_class));
    x->jobDoneOutClock = clock_new(x, reinterpret_cast<t_method>(ofeliaFile_jobDoneOut));
    x->handler = make_unique<ofeliaFileHandler>();
    x->handler->setJobDoneOutClock(x->jobDoneOutClock);
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaFile_create(t_ofeliaFile *x, t_symbol *s)
{
    x->handler->createFile(s->s_name);
}

void ofeliaFile_remove(t_ofeliaFile *x, t_symbol *s, int argc, t_atom *argv)
{
    switch (argc) {
            
        case 1:
        {
            if (argv[0].a_type == A_SYMBOL) {
                
                x->handler->removeFile(argv[0].a_w.w_symbol->s_name, true);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaFile::objName);
            }
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_SYMBOL &&
                argv[1].a_type == A_FLOAT) {
                
                const bool recursive = argv[1].a_w.w_float != 0.0f;
                x->handler->removeFile(argv[0].a_w.w_symbol->s_name, recursive);
            }
            else if (argv[0].a_type == A_SYMBOL &&
                     argv[1].a_type == A_SYMBOL) {
                
                bool recursive;
                
                if (getToggleFromSym(argv[1].a_w.w_symbol, recursive, t_ofeliaFile::objName))
                    x->handler->removeFile(argv[0].a_w.w_symbol->s_name, recursive);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaFile::objName);
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaFile::objName);
            break;
        }
    }
}

void ofeliaFile_rename(t_ofeliaFile *x, t_symbol *s, int argc, t_atom *argv)
{
    switch (argc) {
            
        case 2:
        {
            if (argv[0].a_type == A_SYMBOL &&
                argv[1].a_type == A_SYMBOL) {
                
                x->handler->renameFile(argv[0].a_w.w_symbol->s_name, argv[1].a_w.w_symbol->s_name, true);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaFile::objName);
            }
            break;
        }
        case 3:
        {
            if (argv[0].a_type == A_SYMBOL &&
                argv[1].a_type == A_SYMBOL &&
                argv[2].a_type == A_FLOAT) {
                
                const bool overwrite = argv[2].a_w.w_float != 0.0f;
                x->handler->renameFile(argv[0].a_w.w_symbol->s_name, argv[1].a_w.w_symbol->s_name, overwrite);
            }
            else if (argv[0].a_type == A_SYMBOL &&
                     argv[1].a_type == A_SYMBOL &&
                     argv[2].a_type == A_SYMBOL) {
                
                bool overwrite;
                
                if (getToggleFromSym(argv[2].a_w.w_symbol, overwrite, t_ofeliaFile::objName))
                    x->handler->renameFile(argv[0].a_w.w_symbol->s_name, argv[1].a_w.w_symbol->s_name, overwrite);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaFile::objName);
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaFile::objName);
            break;
        }
    }
}

void ofeliaFile_copy(t_ofeliaFile *x, t_symbol *s, int argc, t_atom *argv)
{
    switch (argc) {
            
        case 2:
        {
            if (argv[0].a_type == A_SYMBOL &&
                argv[1].a_type == A_SYMBOL) {
                
                x->handler->copyFile(argv[0].a_w.w_symbol->s_name, argv[1].a_w.w_symbol->s_name, true);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaFile::objName);
            }
            break;
        }
        case 3:
        {
            if (argv[0].a_type == A_SYMBOL &&
                argv[1].a_type == A_SYMBOL &&
                argv[2].a_type == A_FLOAT) {
                
                const bool overwrite = argv[2].a_w.w_float != 0.0f;
                x->handler->copyFile(argv[0].a_w.w_symbol->s_name, argv[1].a_w.w_symbol->s_name, overwrite);
            }
            else if (argv[0].a_type == A_SYMBOL &&
                     argv[1].a_type == A_SYMBOL &&
                     argv[2].a_type == A_SYMBOL) {
                
                bool overwrite;
                
                if (getToggleFromSym(argv[2].a_w.w_symbol, overwrite, t_ofeliaFile::objName))
                    x->handler->copyFile(argv[0].a_w.w_symbol->s_name, argv[1].a_w.w_symbol->s_name, overwrite);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaFile::objName);
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaFile::objName);
            break;
        }
    }
}

void ofeliaFile_move(t_ofeliaFile *x, t_symbol *s, int argc, t_atom *argv)
{
    switch (argc) {
            
        case 2:
        {
            if (argv[0].a_type == A_SYMBOL &&
                argv[1].a_type == A_SYMBOL) {
                
                x->handler->moveFile(argv[0].a_w.w_symbol->s_name, argv[1].a_w.w_symbol->s_name, true);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaFile::objName);
            }
            break;
        }
        case 3:
        {
            if (argv[0].a_type == A_SYMBOL &&
                argv[1].a_type == A_SYMBOL &&
                argv[2].a_type == A_FLOAT) {
                
                const bool overwrite = argv[2].a_w.w_float != 0.0f;
                x->handler->moveFile(argv[0].a_w.w_symbol->s_name, argv[1].a_w.w_symbol->s_name, overwrite);
            }
            else if (argv[0].a_type == A_SYMBOL &&
                     argv[1].a_type == A_SYMBOL &&
                     argv[2].a_type == A_SYMBOL) {
                
                bool overwrite;
                
                if (getToggleFromSym(argv[2].a_w.w_symbol, overwrite, t_ofeliaFile::objName))
                    x->handler->moveFile(argv[0].a_w.w_symbol->s_name, argv[1].a_w.w_symbol->s_name, overwrite);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaFile::objName);
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaFile::objName);
            break;
        }
    }
}

void ofeliaFile_jobDoneOut(t_ofeliaFile *x)
{
    switch (x->handler->getErrorType()) {
            
        case FILE_ERROR_NOERROR:
            outlet_bang(x->x_obj.ob_outlet);
            break;
        case FILE_ERROR_FAILTOCREATE:
            error("%s: failed to create file", t_ofeliaFile::objName);
            break;
        case FILE_ERROR_FAILTOREMOVE:
            error("%s: failed to remove file", t_ofeliaFile::objName);
            break;
        case FILE_ERROR_FAILTORENAME:
            error("%s: failed to rename file", t_ofeliaFile::objName);
            break;
        case FILE_ERROR_FAILTOCOPY:
            error("%s: failed to copy file", t_ofeliaFile::objName);
            break;
        case FILE_ERROR_FAILTOMOVE:
            error("%s: failed to move file", t_ofeliaFile::objName);
            break;
        default:
            break;
    }
}

void ofeliaFile_free(t_ofeliaFile *x)
{
    clock_free(x->jobDoneOutClock);
    x->handler.reset();
}

void ofeliaFile_setup()
{
    ofeliaFile_class = class_new(gensym("ofFile"),
                                      reinterpret_cast<t_newmethod>(ofeliaFile_new),
                                      reinterpret_cast<t_method>(ofeliaFile_free),
                                      sizeof(t_ofeliaFile),
                                      CLASS_DEFAULT, A_NULL, 0);
    class_addmethod(ofeliaFile_class, reinterpret_cast<t_method>(ofeliaFile_create),
                    gensym("create"), A_SYMBOL, 0);
    class_addmethod(ofeliaFile_class, reinterpret_cast<t_method>(ofeliaFile_remove),
                    gensym("remove"), A_GIMME, 0);
    class_addmethod(ofeliaFile_class, reinterpret_cast<t_method>(ofeliaFile_rename),
                    gensym("rename"), A_GIMME, 0);
    class_addmethod(ofeliaFile_class, reinterpret_cast<t_method>(ofeliaFile_copy),
                    gensym("copy"), A_GIMME, 0);
    class_addmethod(ofeliaFile_class, reinterpret_cast<t_method>(ofeliaFile_move),
                    gensym("move"), A_GIMME, 0);
}

