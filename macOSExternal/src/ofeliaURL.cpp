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

#include "ofeliaURL.h"

/* ________________________________________________________________________________
 * static variables
 */
const char *t_ofeliaSaveURL::objName = "ofSaveURL";

/* ________________________________________________________________________________
 * ofSaveURL object methods
 */
void ofeliaSaveURL_jobDoneOut(t_ofeliaSaveURL *x);

void *ofeliaSaveURL_new()
{
    t_ofeliaSaveURL *x = reinterpret_cast<t_ofeliaSaveURL*>(pd_new(ofeliaSaveURL_class));
    x->jobDoneOutClock = clock_new(x, reinterpret_cast<t_method>(ofeliaSaveURL_jobDoneOut));
    x->saver = make_unique<ofeliaURLSaver>();
    x->saver->setJobDoneOutClock(x->jobDoneOutClock);
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSaveURL_save(t_ofeliaSaveURL *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc == 2) {
        
        if (argv[0].a_type == A_SYMBOL &&
            argv[1].a_type == A_SYMBOL) {
            
            x->saver->saveURL(argv[0].a_w.w_symbol->s_name, argv[1].a_w.w_symbol->s_name);
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaSaveURL::objName);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaSaveURL::objName);
    }
}

void ofeliaSaveURL_clear(t_ofeliaSaveURL *x)
{
    x->saver->clear();
}

void ofeliaSaveURL_jobDoneOut(t_ofeliaSaveURL *x)
{
    switch (x->saver->getErrorType()) {
            
        case URLSAVER_ERROR_NOERROR:
            outlet_bang(x->x_obj.ob_outlet);
            break;
        case URLSAVER_ERROR_FAILTOSAVE:
            error("%s: failed to save the url '%s'", t_ofeliaSaveURL::objName, x->saver->getFailedURL().c_str());
            break;
        default:
            break;
    }
}

void ofeliaSaveURL_free(t_ofeliaSaveURL *x)
{
    clock_free(x->jobDoneOutClock);
    x->saver.reset();
}

void ofeliaSaveURL_setup()
{
    ofeliaSaveURL_class = class_new(gensym("ofSaveURL"),
                                 reinterpret_cast<t_newmethod>(ofeliaSaveURL_new),
                                 reinterpret_cast<t_method>(ofeliaSaveURL_free),
                                 sizeof(t_ofeliaSaveURL),
                                 CLASS_DEFAULT, A_NULL, 0);
    class_addmethod(ofeliaSaveURL_class, reinterpret_cast<t_method>(ofeliaSaveURL_save),
                    gensym("save"), A_GIMME, 0);
    class_addmethod(ofeliaSaveURL_class, reinterpret_cast<t_method>(ofeliaSaveURL_clear),
                    gensym("clear"), A_NULL, 0);
}

void ofeliaURL_setup()
{
    ofeliaSaveURL_setup();
}

