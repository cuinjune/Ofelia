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

#include "ofeliaPatch.h"

/* ________________________________________________________________________________
 * static variables
 */
const char *t_ofeliaPatch::objName = "ofPatch";

/* ________________________________________________________________________________
 * ofPatch object methods
 */
void *ofeliaPatch_new()
{
    t_ofeliaPatch *x = reinterpret_cast<t_ofeliaPatch*>(pd_new(ofeliaPatch_class));
    x->app = static_cast<ofApp *>(ofGetAppPtr());
    x->canvas = canvas_getcurrent();
    return (x);
}

/* can receive both full and relative path */
void ofeliaPatch_open(t_ofeliaPatch *x, t_symbol *s)
{
    t_string path = s->s_name;
    
    if (!doesFilenameHaveExtension(path)) {
        
        error("%s: wrong file format '%s'", t_ofeliaPatch::objName, s->s_name);
        return;
    }
    if (!makePathAbsolute(path, x->canvas)) {
        
        error("%s: failed to find '%s'", t_ofeliaPatch::objName, s->s_name);
        return;
    }
    x->app->pd.openPatch(path.c_str());
}

/* can receive filename only */
void ofeliaPatch_close(t_ofeliaPatch *x, t_symbol *s)
{
    t_string filename = s->s_name;
    
    if (!doesFilenameHaveExtension(filename)) {
        
        error("%s: wrong file format '%s'", t_ofeliaPatch::objName, s->s_name);
        return;
    }
    /* detect if path is received */
    const size_t found = filename.rfind(OFELIA_PATH_SEPARATOR);
    
    if (found != t_string::npos) {
        
        error("%s: need patch filename for 'close'", t_ofeliaPatch::objName);
        return;
    }
    x->app->pd.closePatch(filename.c_str());
}

void ofeliaPatch_setup()
{
    ofeliaPatch_class = class_new(gensym("ofPatch"),
                                  reinterpret_cast<t_newmethod>(ofeliaPatch_new),
                                  0, sizeof(t_ofeliaPatch),
                                  CLASS_DEFAULT, A_NULL, 0);
    class_addmethod(ofeliaPatch_class, reinterpret_cast<t_method>(ofeliaPatch_open),
                    gensym("open"), A_SYMBOL, 0);
    class_addmethod(ofeliaPatch_class, reinterpret_cast<t_method>(ofeliaPatch_close),
                    gensym("close"), A_SYMBOL, 0);
}

