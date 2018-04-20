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

#include "ofeliaDirectory.h"

/* ________________________________________________________________________________
 * static variables
 */
const char *t_ofeliaDirectory::objName = "ofDirectory";

/* ________________________________________________________________________________
 * ofDirectory object methods
 */
void ofeliaDirectory_jobDoneOut(t_ofeliaDirectory *x);

void *ofeliaDirectory_new()
{
    t_ofeliaDirectory *x = reinterpret_cast<t_ofeliaDirectory*>(pd_new(ofeliaDirectory_class));
    x->jobDoneOutClock = clock_new(x, reinterpret_cast<t_method>(ofeliaDirectory_jobDoneOut));
    x->handler = make_unique<ofeliaDirectoryHandler>();
    x->handler->setJobDoneOutClock(x->jobDoneOutClock);
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaDirectory_create(t_ofeliaDirectory *x, t_symbol *s, int argc, t_atom *argv)
{
    switch (argc) {
            
        case 1:
        {
            if (argv[0].a_type == A_SYMBOL) {
                
                x->handler->createDirectory(argv[0].a_w.w_symbol->s_name, true);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaDirectory::objName);
            }
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_SYMBOL &&
                argv[1].a_type == A_FLOAT) {
                
                const bool recursive = argv[1].a_w.w_float != 0.0f;
                x->handler->createDirectory(argv[0].a_w.w_symbol->s_name, recursive);
            }
            else if (argv[0].a_type == A_SYMBOL &&
                     argv[1].a_type == A_SYMBOL) {
                
                bool recursive;
                
                if (getToggleFromSym(argv[1].a_w.w_symbol, recursive, t_ofeliaDirectory::objName))
                    x->handler->createDirectory(argv[0].a_w.w_symbol->s_name, recursive);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaDirectory::objName);
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaDirectory::objName);
            break;
        }
    }
}

void ofeliaDirectory_remove(t_ofeliaDirectory *x, t_symbol *s, int argc, t_atom *argv)
{
    switch (argc) {
            
        case 1:
        {
            if (argv[0].a_type == A_SYMBOL) {
                
                x->handler->removeDirectory(argv[0].a_w.w_symbol->s_name, true);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaDirectory::objName);
            }
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_SYMBOL &&
                argv[1].a_type == A_FLOAT) {
                
                const bool recursive = argv[1].a_w.w_float != 0.0f;
                x->handler->removeDirectory(argv[0].a_w.w_symbol->s_name, recursive);
            }
            else if (argv[0].a_type == A_SYMBOL &&
                     argv[1].a_type == A_SYMBOL) {
                
                bool recursive;
                
                if (getToggleFromSym(argv[1].a_w.w_symbol, recursive, t_ofeliaDirectory::objName))
                    x->handler->removeDirectory(argv[0].a_w.w_symbol->s_name, recursive);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaDirectory::objName);
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaDirectory::objName);
            break;
        }
    }
}

void ofeliaDirectory_rename(t_ofeliaDirectory *x, t_symbol *s, int argc, t_atom *argv)
{
    switch (argc) {
            
        case 2:
        {
            if (argv[0].a_type == A_SYMBOL &&
                argv[1].a_type == A_SYMBOL) {
                
                x->handler->renameDirectory(argv[0].a_w.w_symbol->s_name, argv[1].a_w.w_symbol->s_name, true);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaDirectory::objName);
            }
            break;
        }
        case 3:
        {
            if (argv[0].a_type == A_SYMBOL &&
                argv[1].a_type == A_SYMBOL &&
                argv[2].a_type == A_FLOAT) {
                
                const bool overwrite = argv[2].a_w.w_float != 0.0f;
                x->handler->renameDirectory(argv[0].a_w.w_symbol->s_name, argv[1].a_w.w_symbol->s_name, overwrite);
            }
            else if (argv[0].a_type == A_SYMBOL &&
                     argv[1].a_type == A_SYMBOL &&
                     argv[2].a_type == A_SYMBOL) {
                
                bool overwrite;
                
                if (getToggleFromSym(argv[2].a_w.w_symbol, overwrite, t_ofeliaDirectory::objName))
                    x->handler->renameDirectory(argv[0].a_w.w_symbol->s_name, argv[1].a_w.w_symbol->s_name, overwrite);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaDirectory::objName);
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaDirectory::objName);
            break;
        }
    }
}

void ofeliaDirectory_copy(t_ofeliaDirectory *x, t_symbol *s, int argc, t_atom *argv)
{
    switch (argc) {
            
        case 2:
        {
            if (argv[0].a_type == A_SYMBOL &&
                argv[1].a_type == A_SYMBOL) {
                
                x->handler->copyDirectory(argv[0].a_w.w_symbol->s_name, argv[1].a_w.w_symbol->s_name, true);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaDirectory::objName);
            }
            break;
        }
        case 3:
        {
            if (argv[0].a_type == A_SYMBOL &&
                argv[1].a_type == A_SYMBOL &&
                argv[2].a_type == A_FLOAT) {
                
                const bool overwrite = argv[2].a_w.w_float != 0.0f;
                x->handler->copyDirectory(argv[0].a_w.w_symbol->s_name, argv[1].a_w.w_symbol->s_name, overwrite);
            }
            else if (argv[0].a_type == A_SYMBOL &&
                     argv[1].a_type == A_SYMBOL &&
                     argv[2].a_type == A_SYMBOL) {
                
                bool overwrite;
                
                if (getToggleFromSym(argv[2].a_w.w_symbol, overwrite, t_ofeliaDirectory::objName))
                    x->handler->copyDirectory(argv[0].a_w.w_symbol->s_name, argv[1].a_w.w_symbol->s_name, overwrite);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaDirectory::objName);
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaDirectory::objName);
            break;
        }
    }
}

void ofeliaDirectory_move(t_ofeliaDirectory *x, t_symbol *s, int argc, t_atom *argv)
{
    switch (argc) {
            
        case 2:
        {
            if (argv[0].a_type == A_SYMBOL &&
                argv[1].a_type == A_SYMBOL) {
                
                x->handler->moveDirectory(argv[0].a_w.w_symbol->s_name, argv[1].a_w.w_symbol->s_name, true);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaDirectory::objName);
            }
            break;
        }
        case 3:
        {
            if (argv[0].a_type == A_SYMBOL &&
                argv[1].a_type == A_SYMBOL &&
                argv[2].a_type == A_FLOAT) {
                
                const bool overwrite = argv[2].a_w.w_float != 0.0f;
                x->handler->moveDirectory(argv[0].a_w.w_symbol->s_name, argv[1].a_w.w_symbol->s_name, overwrite);
            }
            else if (argv[0].a_type == A_SYMBOL &&
                     argv[1].a_type == A_SYMBOL &&
                     argv[2].a_type == A_SYMBOL) {
                
                bool overwrite;
                
                if (getToggleFromSym(argv[2].a_w.w_symbol, overwrite, t_ofeliaDirectory::objName))
                    x->handler->moveDirectory(argv[0].a_w.w_symbol->s_name, argv[1].a_w.w_symbol->s_name, overwrite);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaDirectory::objName);
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaDirectory::objName);
            break;
        }
    }
}

void ofeliaDirectory_jobDoneOut(t_ofeliaDirectory *x)
{
    switch (x->handler->getErrorType()) {
            
        case DIR_ERROR_NOERROR:
            outlet_bang(x->x_obj.ob_outlet);
            break;
        case DIR_ERROR_FAILTOCREATE:
            error("%s: failed to create directory", t_ofeliaDirectory::objName);
            break;
        case DIR_ERROR_FAILTOREMOVE:
            error("%s: failed to remove directory", t_ofeliaDirectory::objName);
            break;
        case DIR_ERROR_FAILTORENAME:
            error("%s: failed to rename directory", t_ofeliaDirectory::objName);
            break;
        case DIR_ERROR_FAILTOCOPY:
            error("%s: failed to copy directory", t_ofeliaDirectory::objName);
            break;
        case DIR_ERROR_FAILTOMOVE:
            error("%s: failed to move directory", t_ofeliaDirectory::objName);
            break;
        default:
            break;
    }
}

void ofeliaDirectory_free(t_ofeliaDirectory *x)
{
    clock_free(x->jobDoneOutClock);
    x->handler.reset();
}

void ofeliaDirectory_setup()
{
    ofeliaDirectory_class = class_new(gensym("ofDirectory"),
                                  reinterpret_cast<t_newmethod>(ofeliaDirectory_new),
                                  reinterpret_cast<t_method>(ofeliaDirectory_free),
                                      sizeof(t_ofeliaDirectory),
                                  CLASS_DEFAULT, A_NULL, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaDirectory_new),
                     gensym("ofDir"), A_NULL, 0);
    class_addmethod(ofeliaDirectory_class, reinterpret_cast<t_method>(ofeliaDirectory_create),
                    gensym("create"), A_GIMME, 0);
    class_addmethod(ofeliaDirectory_class, reinterpret_cast<t_method>(ofeliaDirectory_remove),
                    gensym("remove"), A_GIMME, 0);
    class_addmethod(ofeliaDirectory_class, reinterpret_cast<t_method>(ofeliaDirectory_rename),
                    gensym("rename"), A_GIMME, 0);
    class_addmethod(ofeliaDirectory_class, reinterpret_cast<t_method>(ofeliaDirectory_copy),
                    gensym("copy"), A_GIMME, 0);
    class_addmethod(ofeliaDirectory_class, reinterpret_cast<t_method>(ofeliaDirectory_move),
                    gensym("move"), A_GIMME, 0);
}

