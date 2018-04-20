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

#include "ofeliaSeparators.h"
#include "ofeliaWindow.h"
#include "ofeliaStyle.h"

/* ________________________________________________________________________________
 * static variables
 */
const char *t_ofeliaSepStyle::objName = "ofSepStyle";
const char *t_ofeliaSepMatrix::objName = "ofSepMatrix";
const char *t_ofeliaSeparator::objName = "ofSeparator";

/* ________________________________________________________________________________
 * common methods
 */
bool getNumOutletsFromArgs(const int argc, const t_atom *argv, int &numOutlets, const char *objName)
{
    switch (argc) {
            
        case 0:
        {
            numOutlets = 1;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                numOutlets = static_cast<int>(argv[0].a_w.w_float);
                
                if (numOutlets < 1 || numOutlets > 64) {
                    
                    error("%s: invalid number of outlets", objName);
                    return 0;
                }
            }
            else {
                
                error("%s: wrong argument type", objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", objName);
            return 0;
        }
    }
    return 1;
}

/* ________________________________________________________________________________
 * ofSepStyle object methods
 */
void *ofeliaSepStyle_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSepStyle *x = reinterpret_cast<t_ofeliaSepStyle*>(pd_new(ofeliaSepStyle_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    int numOutlets;
    
    if (!getNumOutletsFromArgs(argc, argv, numOutlets, t_ofeliaSepStyle::objName))
        return 0;
    x->sep_outs.resize(numOutlets);
    
    for (int i=0; i<numOutlets; ++i)
        x->sep_outs[i] = outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSepStyle_bang(t_ofeliaSepStyle *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        for (size_t i=x->sep_outs.size(); i-- > 0;) {
            
            t_ofeliaSetTextMode::textModeVec.push_back(t_ofeliaSetTextMode::textModeVec.back());
            ofGetCurrentRenderer()->pushStyle();
            outlet_bang(x->sep_outs[i]);
            ofGetCurrentRenderer()->popStyle();
            t_ofeliaSetTextMode::textModeVec.pop_back();
        }
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSepStyle::objName);
    }
}

void ofeliaSepStyle_free(t_ofeliaSepStyle *x)
{
    for (size_t i=0; i<x->sep_outs.size(); ++i)
        outlet_free(x->sep_outs[i]);
}

void ofeliaSepStyle_setup()
{
    ofeliaSepStyle_class = class_new(gensym("ofSepStyle"),
                                     reinterpret_cast<t_newmethod>(ofeliaSepStyle_new),
                                     reinterpret_cast<t_method>(ofeliaSepStyle_free),
                                     sizeof(t_ofeliaSepStyle),
                                     CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSepStyle_class, reinterpret_cast<t_method>(ofeliaSepStyle_bang));
}

/* ________________________________________________________________________________
 * ofSepMatrix object methods
 */
void *ofeliaSepMatrix_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSepMatrix *x = reinterpret_cast<t_ofeliaSepMatrix*>(pd_new(ofeliaSepMatrix_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    int numOutlets;
    
    if (!getNumOutletsFromArgs(argc, argv, numOutlets, t_ofeliaSepMatrix::objName))
        return 0;
    x->sep_outs.resize(numOutlets);
    
    for (int i=0; i<numOutlets; ++i)
        x->sep_outs[i] = outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSepMatrix_bang(t_ofeliaSepMatrix *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        for (size_t i=x->sep_outs.size(); i-- > 0;) {
            
            ofGetCurrentRenderer()->pushMatrix();
            outlet_bang(x->sep_outs[i]);
            ofGetCurrentRenderer()->popMatrix();
        }
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSepMatrix::objName);
    }
}

void ofeliaSepMatrix_free(t_ofeliaSepMatrix *x)
{
    for (size_t i=0; i<x->sep_outs.size(); ++i)
        outlet_free(x->sep_outs[i]);
}

void ofeliaSepMatrix_setup()
{
    ofeliaSepMatrix_class = class_new(gensym("ofSepMatrix"),
                                      reinterpret_cast<t_newmethod>(ofeliaSepMatrix_new),
                                      reinterpret_cast<t_method>(ofeliaSepMatrix_free),
                                      sizeof(t_ofeliaSepMatrix),
                                      CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaSepMatrix_class, reinterpret_cast<t_method>(ofeliaSepMatrix_bang));
}

/* ________________________________________________________________________________
 * ofSeparator object methods
 */
void *ofeliaSeparator_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaSeparator *x = reinterpret_cast<t_ofeliaSeparator*>(pd_new(ofeliaSeparator_class));
 
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    int numOutlets;
    
    if (!getNumOutletsFromArgs(argc, argv, numOutlets, t_ofeliaSeparator::objName))
        return 0;
    x->sep_outs.resize(numOutlets);
    
    for (int i=0; i<numOutlets; ++i)
        x->sep_outs[i] = outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSeparator_bang(t_ofeliaSeparator *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        for (size_t i=x->sep_outs.size(); i-- > 0;) {
            
            t_ofeliaSetTextMode::textModeVec.push_back(t_ofeliaSetTextMode::textModeVec.back());
            ofGetCurrentRenderer()->pushStyle();
            ofGetCurrentRenderer()->pushMatrix();
            outlet_bang(x->sep_outs[i]);
            ofGetCurrentRenderer()->popMatrix();
            ofGetCurrentRenderer()->popStyle();
            t_ofeliaSetTextMode::textModeVec.pop_back();
        }
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaSeparator::objName);
    }
}

void ofeliaSeparator_free(t_ofeliaSeparator *x)
{
    for (size_t i=0; i<x->sep_outs.size(); ++i)
        outlet_free(x->sep_outs[i]);
}

void ofeliaSeparator_setup()
{
    ofeliaSeparator_class = class_new(gensym("ofSeparator"),
                                      reinterpret_cast<t_newmethod>(ofeliaSeparator_new),
                                      reinterpret_cast<t_method>(ofeliaSeparator_free),
                                      sizeof(t_ofeliaSeparator),
                                      CLASS_DEFAULT, A_GIMME, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaSeparator_new),
                     gensym("ofSep"), A_GIMME, 0);
    class_addbang(ofeliaSeparator_class, reinterpret_cast<t_method>(ofeliaSeparator_bang));
}

/* ________________________________________________________________________________
 * setup methods
 */
void ofeliaSeparators_setup()
{
    ofeliaSepStyle_setup();
    ofeliaSepMatrix_setup();
    ofeliaSeparator_setup();
}

