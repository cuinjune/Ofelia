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

#include "ofeliaFbo.h"
#include "ofeliaWindow.h"
#include "ofeliaStyle.h"

/* ________________________________________________________________________________
 * static variables
 */
const char *t_ofeliaCreateFbo::objName = "ofCreateFbo";
unsigned int t_ofeliaCreateFbo::counter;
vector<t_ofeliaCreateFboData> t_ofeliaCreateFbo::fboData;
vector<shared_ptr<ofFbo>> t_ofeliaCreateFbo::fbos;
bool t_ofeliaCreateFbo::bInited;
const char *t_ofeliaBindFboTex::objName = "ofBindFboTex";
const char *t_ofeliaDrawFbo::objName = "ofDrawFbo";
const char *t_ofeliaDoesFboNameExist::objName = "ofDoesFboNameExist";
const char *t_ofeliaIsFboAllocated::objName = "ofIsFboAllocated";
const char *t_ofeliaGetFboDimen::objName = "ofGetFboDimen";
const char *t_ofeliaGetFboType::objName = "ofGetFboType";
const char *t_ofeliaGetFboMaxSamples::objName = "ofGetFboMaxSamples";
const char *t_ofeliaGetFboTexID::objName = "ofGetFboTexID";

/* ________________________________________________________________________________
 * ofCreateFbo object methods
 */
int getPositionByFboObjID(const unsigned int objID)
{
    const auto it = find_if(begin(t_ofeliaCreateFbo::fboData), end(t_ofeliaCreateFbo::fboData),
                            [&objID](const t_ofeliaCreateFboData &fboData)
                            {
                                return fboData.objID == objID;
                            });
    return static_cast<int>(it - t_ofeliaCreateFbo::fboData.begin());
}

void *ofeliaCreateFbo_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaCreateFbo *x = reinterpret_cast<t_ofeliaCreateFbo*>(pd_new(ofeliaCreateFbo_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    t_ofeliaCreateFboData fboData;
    getVarNameLocalPrefixes(fboData.varName);
    
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            fboData.varName.name = argv[0].a_w.w_symbol->s_name;
            getVarNameLocalized(fboData.varName);
            
            if (--argc) {
                
                if (!getImageDataFromArgs(argc, argv+1, fboData.data, t_ofeliaCreateFbo::objName))
                    return 0;
            }
            else {
                
                fboData.data.width = 0;
                fboData.data.height = 0;
                fboData.data.type = OF_IMAGE_GRAYSCALE;
                fboData.data.color.set(ofColor::white);
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaCreateFbo::objName);
            return 0;
        }
    }
    else {
        
        fboData.data.width = 0;
        fboData.data.height = 0;
        fboData.data.type = OF_IMAGE_GRAYSCALE;
        fboData.data.color.set(ofColor::white);
    }
    x->shouldAlloc = true;
    x->bInitGate = false;
    x->objID = fboData.objID = t_ofeliaCreateFbo::counter++;
    t_ofeliaCreateFbo::fboData.push_back(fboData);
    t_ofeliaCreateFbo::fbos.push_back(make_shared<ofFbo>());
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::updateSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaCreateFbo_init(t_ofeliaCreateFbo *x)
{
    if (x->bInitGate) {
        
        if (!t_ofeliaCreateFbo::bInited) {
            
            t_ofeliaCreateFbo::fbos.clear();
            
            for (size_t i=0; i<t_ofeliaCreateFbo::fboData.size(); ++i)
                t_ofeliaCreateFbo::fbos.push_back(make_shared<ofFbo>());
            t_ofeliaCreateFbo::bInited = true;
        }
        x->shouldAlloc = true;
    }
}

void ofeliaCreateFbo_update(t_ofeliaCreateFbo *x)
{
    if (!x->shouldAlloc && !x->shouldColor)
        return;
    const int pos = getPositionByFboObjID(x->objID);
    
    if (x->shouldAlloc) {
        
        /* OFELIANOTE : reallocating format doesn't work properly */
        t_ofeliaCreateFbo::fbos[pos] = make_shared<ofFbo>();
        
        if (t_ofeliaCreateFbo::fboData[pos].data.width && t_ofeliaCreateFbo::fboData[pos].data.height) {
            
            int type;
            
#if defined(TARGET_OF_IOS)
            /* OFELIANOTE : can't copy ofFbo to ofImage when allocate with GL_RGB on iOS */
            type = GL_RGBA;
#else
            switch (t_ofeliaCreateFbo::fboData[pos].data.type) {
                    
                case OF_IMAGE_GRAYSCALE:
                    type = GL_RGB;
                    break;
                case OF_IMAGE_COLOR:
                    type = GL_RGB;
                    break;
                case OF_IMAGE_COLOR_ALPHA:
                    type = GL_RGBA;
                    break;
                default:
                    break;
            }
#endif
            t_ofeliaCreateFbo::fbos[pos]->allocate(t_ofeliaCreateFbo::fboData[pos].data.width * ofeliaWindow::retinaScale, t_ofeliaCreateFbo::fboData[pos].data.height * ofeliaWindow::retinaScale, type);
            x->shouldColor = true;
        }
        else {
            
            t_ofeliaCreateFbo::fbos[pos]->clear();
        }
        x->shouldAlloc = false;
    }
    if (x->shouldColor) {

        if (t_ofeliaCreateFbo::fbos[pos]->isAllocated()) {
            
            t_ofeliaCreateFbo::fbos[pos]->begin();
            const ofColor &initColor = t_ofeliaCreateFbo::fboData[pos].data.color;
            ofGetCurrentRenderer()->clear(initColor.r, initColor.g, initColor.b, initColor.a);
            t_ofeliaCreateFbo::fbos[pos]->end();
        }
        x->shouldColor = false;
    }
}

void ofeliaCreateFbo_exit(t_ofeliaCreateFbo *x)
{
    if (t_ofeliaCreateFbo::bInited)
        t_ofeliaCreateFbo::bInited = false;
    x->shouldAlloc = false;
    x->bInitGate = true;
}

void ofeliaCreateFbo_bang(t_ofeliaCreateFbo *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const int pos = getPositionByFboObjID(x->objID);
        
        if (t_ofeliaCreateFbo::fbos[pos]->isAllocated()) {
            
            t_ofeliaCreateFbo::fbos[pos]->begin();
            ofGetCurrentRenderer()->pushMatrix();
            ofGetCurrentRenderer()->scale(ofeliaWindow::retinaScale, ofeliaWindow::retinaScale, 1.0f);
            outlet_bang(x->x_obj.ob_outlet);
            ofGetCurrentRenderer()->popMatrix();
            t_ofeliaCreateFbo::fbos[pos]->end();
        }
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaCreateFbo::objName);
    }
}

void ofeliaCreateFbo_name(t_ofeliaCreateFbo *x, t_symbol *s)
{
    const int pos = getPositionByFboObjID(x->objID);
    t_ofeliaCreateFbo::fboData[pos].varName.name = s->s_name;
    getVarNameLocalized(t_ofeliaCreateFbo::fboData[pos].varName);
}

void ofeliaCreateFbo_allocate(t_ofeliaCreateFbo *x, t_symbol *s, int argc, t_atom *argv)
{
    const int pos = getPositionByFboObjID(x->objID);
    
    if (getImageDataFromArgs(argc, argv, t_ofeliaCreateFbo::fboData[pos].data, t_ofeliaCreateFbo::objName))
        x->shouldAlloc = true;
}

void ofeliaCreateFbo_color(t_ofeliaCreateFbo *x, t_symbol *s, int argc, t_atom *argv)
{
    const int pos = getPositionByFboObjID(x->objID);
    
    if (getColorAlphaFromArgs(argc, argv, t_ofeliaCreateFbo::fboData[pos].data.color, t_ofeliaCreateFbo::objName))
        x->shouldColor = true;
}

void ofeliaCreateFbo_clear(t_ofeliaCreateFbo *x)
{
    const int pos = getPositionByFboObjID(x->objID);
    t_ofeliaCreateFbo::fboData[pos].data.width = 0;
    t_ofeliaCreateFbo::fboData[pos].data.height = 0;
    t_ofeliaCreateFbo::fboData[pos].data.type = OF_IMAGE_GRAYSCALE;
    t_ofeliaCreateFbo::fboData[pos].data.color.set(ofColor::white);
    x->shouldAlloc = true;
}

void ofeliaCreateFbo_set(t_ofeliaCreateFbo *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            ofeliaCreateFbo_name(x, argv[0].a_w.w_symbol);
            
            if (--argc)
                ofeliaCreateFbo_allocate(x, 0, argc, argv+1);
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaCreateFbo::objName);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaCreateFbo::objName);
    }
}

void ofeliaCreateFbo_print(t_ofeliaCreateFbo *x)
{
    post("\n[%s]", t_ofeliaCreateFbo::objName);
    const int pos = getPositionByFboObjID(x->objID);
    post("name : %s", t_ofeliaCreateFbo::fboData[pos].varName.name.c_str());
    t_string type;
    
    if (t_ofeliaCreateFbo::fboData[pos].data.type == OF_IMAGE_GRAYSCALE)
        type = "RGB";
    else if (t_ofeliaCreateFbo::fboData[pos].data.type == OF_IMAGE_COLOR)
        type = "RGB";
    else if (t_ofeliaCreateFbo::fboData[pos].data.type == OF_IMAGE_COLOR_ALPHA)
        type = "RGBA";
    post("allocate : %d %d %s", t_ofeliaCreateFbo::fboData[pos].data.width, t_ofeliaCreateFbo::fboData[pos].data.height, type.c_str());
    const ofColor &color = t_ofeliaCreateFbo::fboData[pos].data.color;
    post("color : %d %d %d %d", color.r, color.g, color.b, color.a);
}

void ofeliaCreateFbo_free(t_ofeliaCreateFbo *x)
{
    const int pos = getPositionByFboObjID(x->objID);
    t_ofeliaCreateFbo::fboData.erase(t_ofeliaCreateFbo::fboData.begin() + pos);
    t_ofeliaCreateFbo::fbos.erase(t_ofeliaCreateFbo::fbos.begin() + pos);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::updateSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
}

void ofeliaCreateFbo_setup()
{
    ofeliaCreateFbo_class = class_new(gensym("ofCreateFbo"),
                                    reinterpret_cast<t_newmethod>(ofeliaCreateFbo_new),
                                    reinterpret_cast<t_method>(ofeliaCreateFbo_free),
                                    sizeof(t_ofeliaCreateFbo),
                                    CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaCreateFbo_class, reinterpret_cast<t_method>(ofeliaCreateFbo_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaCreateFbo_class, reinterpret_cast<t_method>(ofeliaCreateFbo_update),
                    t_ofeliaWindow::updateMess, A_NULL, 0);
    class_addmethod(ofeliaCreateFbo_class, reinterpret_cast<t_method>(ofeliaCreateFbo_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addbang(ofeliaCreateFbo_class, reinterpret_cast<t_method>(ofeliaCreateFbo_bang));
    class_addmethod(ofeliaCreateFbo_class, reinterpret_cast<t_method>(ofeliaCreateFbo_name),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaCreateFbo_class, reinterpret_cast<t_method>(ofeliaCreateFbo_allocate),
                    gensym("allocate"), A_GIMME, 0);
    class_addmethod(ofeliaCreateFbo_class, reinterpret_cast<t_method>(ofeliaCreateFbo_color),
                    gensym("color"), A_GIMME, 0);
    class_addmethod(ofeliaCreateFbo_class, reinterpret_cast<t_method>(ofeliaCreateFbo_clear),
                    gensym("clear"), A_NULL, 0);
    class_addmethod(ofeliaCreateFbo_class, reinterpret_cast<t_method>(ofeliaCreateFbo_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaCreateFbo_class, reinterpret_cast<t_method>(ofeliaCreateFbo_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * common methods
 */
int getPositionByFboName(const t_string &name)
{
    auto it = find_if(begin(t_ofeliaCreateFbo::fboData), end(t_ofeliaCreateFbo::fboData),
                      [&name] (const t_ofeliaCreateFboData &fboData) {
                          return (fboData.varName.name == name);
                      });
    if (it == end(t_ofeliaCreateFbo::fboData))
        return -1;
    return static_cast<int>(it - begin(t_ofeliaCreateFbo::fboData));
}

/* ________________________________________________________________________________
 * ofBindFboTex object methods
 */
void *ofeliaBindFboTex_new(t_symbol *s)
{
    t_ofeliaBindFboTex *x = reinterpret_cast<t_ofeliaBindFboTex*>(pd_new(ofeliaBindFboTex_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaBindFboTex_bang(t_ofeliaBindFboTex *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const t_string &name = x->varName.name;
        
        if (!name.empty()) {
            
            const int pos = getPositionByFboName(name);
            
            if (pos != -1) {
                
                if (t_ofeliaCreateFbo::fbos[pos]->isAllocated()) {
                    
                    t_ofeliaCreateFbo::fbos[pos]->getTexture().bind();
                    outlet_bang(x->x_obj.ob_outlet);
                    t_ofeliaCreateFbo::fbos[pos]->getTexture().unbind();
                    return;
                }
            }
        }
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaBindFboTex::objName);
    }
}

void ofeliaBindFboTex_set(t_ofeliaBindFboTex *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaBindFboTex_print(t_ofeliaBindFboTex *x)
{
    post("\n[%s]", t_ofeliaBindFboTex::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaBindFboTex_setup()
{
    ofeliaBindFboTex_class = class_new(gensym("ofBindFboTex"),
                                       reinterpret_cast<t_newmethod>(ofeliaBindFboTex_new),
                                       0, sizeof(t_ofeliaBindFboTex),
                                       CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaBindFboTex_class, reinterpret_cast<t_method>(ofeliaBindFboTex_bang));
    class_addmethod(ofeliaBindFboTex_class, reinterpret_cast<t_method>(ofeliaBindFboTex_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaBindFboTex_class, reinterpret_cast<t_method>(ofeliaBindFboTex_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaBindFboTex_class, reinterpret_cast<t_method>(ofeliaBindFboTex_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofDrawFbo object methods
 */
void *ofeliaDrawFbo_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaDrawFbo *x = reinterpret_cast<t_ofeliaDrawFbo*>(pd_new(ofeliaDrawFbo_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    getVarNameLocalPrefixes(x->elem.varName);
    
    if (!getNameDimen2dElemFromArgs(argc, argv, x->elem, t_ofeliaDrawFbo::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("width"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("height"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaDrawFbo_bang(t_ofeliaDrawFbo *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const t_string &name = x->elem.varName.name;
        
        if (!name.empty()) {
            
            const int pos = getPositionByFboName(name);
            
            if (pos != -1) {
                
                if (t_ofeliaCreateFbo::fbos[pos]->isAllocated()) {
                    
                    const float scaleAmt = 1.0f / ofeliaWindow::retinaScale;
                    
                    if (!x->elem.width && !x->elem.height)
                        t_ofeliaCreateFbo::fbos[pos]->draw(0.0f, 0.0f, t_ofeliaCreateFbo::fbos[pos]->getWidth() * scaleAmt, t_ofeliaCreateFbo::fbos[pos]->getHeight() * scaleAmt);
                    else
                        t_ofeliaCreateFbo::fbos[pos]->draw(0.0f, 0.0f, x->elem.width, x->elem.height);
                }
            }
        }
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaDrawFbo::objName);
    }
}

void ofeliaDrawFbo_name(t_ofeliaDrawFbo *x, t_symbol *s)
{
    x->elem.varName.name = s->s_name;
    getVarNameLocalized(x->elem.varName);
}

void ofeliaDrawFbo_width(t_ofeliaDrawFbo *x, t_floatarg f)
{
    x->elem.width = f;
}

void ofeliaDrawFbo_height(t_ofeliaDrawFbo *x, t_floatarg f)
{
    x->elem.height = f;
}

void ofeliaDrawFbo_set(t_ofeliaDrawFbo *x, t_symbol *s, int argc, t_atom *argv)
{
    getNameDimen2dElemFromArgs(argc, argv, x->elem, t_ofeliaDrawFbo::objName);
}

void ofeliaDrawFbo_print(t_ofeliaDrawFbo *x)
{
    post("\n[%s]", t_ofeliaDrawFbo::objName);
    const t_string &name = x->elem.varName.name;
    float width, height;
    
    if (!x->elem.width && !x->elem.height) {
        
        if (!name.empty()) {
            
            const int pos = getPositionByFboName(name);
            
            if (pos != -1) {
                
                if (t_ofeliaCreateFbo::fbos[pos]->isAllocated()) {
                    
                    const float scaleAmt = 1.0f / ofeliaWindow::retinaScale;
                    width = t_ofeliaCreateFbo::fbos[pos]->getWidth() * scaleAmt;
                    height = t_ofeliaCreateFbo::fbos[pos]->getHeight() * scaleAmt;
                }
                else {
                    
                    width = 0.0f;
                    height = 0.0f;
                }
            }
            else {
                
                width = 0.0f;
                height = 0.0f;
            }
        }
        else {
            
            width = 0.0f;
            height = 0.0f;
        }
    }
    else {
        
        width = x->elem.width;
        height = x->elem.height;
    }
    post("name : %s", name.c_str());
    post("width : %g", width);
    post("height : %g", height);
}

void ofeliaDrawFbo_setup()
{
    ofeliaDrawFbo_class = class_new(gensym("ofDrawFbo"),
                                    reinterpret_cast<t_newmethod>(ofeliaDrawFbo_new),
                                    0, sizeof(t_ofeliaDrawFbo),
                                    CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaDrawFbo_class, reinterpret_cast<t_method>(ofeliaDrawFbo_bang));
    class_addmethod(ofeliaDrawFbo_class, reinterpret_cast<t_method>(ofeliaDrawFbo_name),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaDrawFbo_class, reinterpret_cast<t_method>(ofeliaDrawFbo_width),
                    gensym("width"), A_FLOAT, 0);
    class_addmethod(ofeliaDrawFbo_class, reinterpret_cast<t_method>(ofeliaDrawFbo_height),
                    gensym("height"), A_FLOAT, 0);
    class_addmethod(ofeliaDrawFbo_class, reinterpret_cast<t_method>(ofeliaDrawFbo_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaDrawFbo_class, reinterpret_cast<t_method>(ofeliaDrawFbo_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofDoesFboNameExist object methods
 */
void *ofeliaDoesFboNameExist_new(t_symbol *s)
{
    t_ofeliaDoesFboNameExist *x = reinterpret_cast<t_ofeliaDoesFboNameExist*>(pd_new(ofeliaDoesFboNameExist_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaDoesFboNameExist_bang(t_ofeliaDoesFboNameExist *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        if (getPositionByFboName(name) != -1)
            outlet_float(x->x_obj.ob_outlet, 1.0f);
        else
            outlet_float(x->x_obj.ob_outlet, 0.0f);
    }
    else {
        
        error("%s: name not assigned", t_ofeliaDoesFboNameExist::objName);
    }
}

void ofeliaDoesFboNameExist_set(t_ofeliaDoesFboNameExist *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaDoesFboNameExist_print(t_ofeliaDoesFboNameExist *x)
{
    post("\n[%s]", t_ofeliaDoesFboNameExist::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaDoesFboNameExist_setup()
{
    ofeliaDoesFboNameExist_class = class_new(gensym("ofDoesFboNameExist"),
                                            reinterpret_cast<t_newmethod>(ofeliaDoesFboNameExist_new),
                                            0, sizeof(t_ofeliaDoesFboNameExist),
                                            CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaDoesFboNameExist_class, reinterpret_cast<t_method>(ofeliaDoesFboNameExist_bang));
    class_addmethod(ofeliaDoesFboNameExist_class, reinterpret_cast<t_method>(ofeliaDoesFboNameExist_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesFboNameExist_class, reinterpret_cast<t_method>(ofeliaDoesFboNameExist_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesFboNameExist_class, reinterpret_cast<t_method>(ofeliaDoesFboNameExist_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofIsFboAllocated object methods
 */
void *ofeliaIsFboAllocated_new(t_symbol *s)
{
    t_ofeliaIsFboAllocated *x = reinterpret_cast<t_ofeliaIsFboAllocated*>(pd_new(ofeliaIsFboAllocated_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaIsFboAllocated_bang(t_ofeliaIsFboAllocated *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByFboName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaCreateFbo::fbos[pos]->isAllocated())
                outlet_float(x->x_obj.ob_outlet, 1.0f);
            else
                outlet_float(x->x_obj.ob_outlet, 0.0f);
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaIsFboAllocated::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaIsFboAllocated::objName);
    }
}

void ofeliaIsFboAllocated_set(t_ofeliaIsFboAllocated *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaIsFboAllocated_print(t_ofeliaIsFboAllocated *x)
{
    post("\n[%s]", t_ofeliaIsFboAllocated::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaIsFboAllocated_setup()
{
    ofeliaIsFboAllocated_class = class_new(gensym("ofIsFboAllocated"),
                                           reinterpret_cast<t_newmethod>(ofeliaIsFboAllocated_new),
                                           0, sizeof(t_ofeliaIsFboAllocated),
                                           CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaIsFboAllocated_class, reinterpret_cast<t_method>(ofeliaIsFboAllocated_bang));
    class_addmethod(ofeliaIsFboAllocated_class, reinterpret_cast<t_method>(ofeliaIsFboAllocated_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaIsFboAllocated_class, reinterpret_cast<t_method>(ofeliaIsFboAllocated_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaIsFboAllocated_class, reinterpret_cast<t_method>(ofeliaIsFboAllocated_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetFboDimen object methods
 */
void *ofeliaGetFboDimen_new(t_symbol *s)
{
    t_ofeliaGetFboDimen *x = reinterpret_cast<t_ofeliaGetFboDimen*>(pd_new(ofeliaGetFboDimen_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetFboDimen_bang(t_ofeliaGetFboDimen *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByFboName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaCreateFbo::fbos[pos]->isAllocated()) {
                
                const float scaleAmt = 1.0f / ofeliaWindow::retinaScale;
                t_atom av[2];
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = t_ofeliaCreateFbo::fbos[pos]->getWidth() * scaleAmt;
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = t_ofeliaCreateFbo::fbos[pos]->getHeight() * scaleAmt;
                outlet_list(x->x_obj.ob_outlet, &s_list, 2, av);
            }
            else {
                
                error("%s: '%s' is not allocated", t_ofeliaGetFboDimen::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetFboDimen::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetFboDimen::objName);
    }
}

void ofeliaGetFboDimen_set(t_ofeliaGetFboDimen *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetFboDimen_print(t_ofeliaGetFboDimen *x)
{
    post("\n[%s]", t_ofeliaGetFboDimen::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetFboDimen_setup()
{
    ofeliaGetFboDimen_class = class_new(gensym("ofGetFboDimen"),
                                        reinterpret_cast<t_newmethod>(ofeliaGetFboDimen_new),
                                        0, sizeof(t_ofeliaGetFboDimen),
                                        CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetFboDimen_class, reinterpret_cast<t_method>(ofeliaGetFboDimen_bang));
    class_addmethod(ofeliaGetFboDimen_class, reinterpret_cast<t_method>(ofeliaGetFboDimen_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetFboDimen_class, reinterpret_cast<t_method>(ofeliaGetFboDimen_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetFboDimen_class, reinterpret_cast<t_method>(ofeliaGetFboDimen_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetFboType object methods
 */
void *ofeliaGetFboType_new(t_symbol *s)
{
    t_ofeliaGetFboType *x = reinterpret_cast<t_ofeliaGetFboType*>(pd_new(ofeliaGetFboType_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetFboType_bang(t_ofeliaGetFboType *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByFboName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaCreateFbo::fbos[pos]->isAllocated()) {
                
                switch (t_ofeliaCreateFbo::fboData[pos].data.type) {
                        
                    case OF_IMAGE_GRAYSCALE:
                        outlet_float(x->x_obj.ob_outlet, 0.0f);
                        break;
                    case OF_IMAGE_COLOR:
                        outlet_float(x->x_obj.ob_outlet, 1.0f);
                        break;
                    case OF_IMAGE_COLOR_ALPHA:
                        outlet_float(x->x_obj.ob_outlet, 2.0f);
                        break;
                    case OF_IMAGE_UNDEFINED:
                        outlet_float(x->x_obj.ob_outlet, 3.0f);
                        break;
                    default:
                        break;
                }
            }
            else {
                
                error("%s: '%s' is not allocated", t_ofeliaGetFboType::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetFboType::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetFboType::objName);
    }
}

void ofeliaGetFboType_set(t_ofeliaGetFboType *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetFboType_print(t_ofeliaGetFboType *x)
{
    post("\n[%s]", t_ofeliaGetFboType::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetFboType_setup()
{
    ofeliaGetFboType_class = class_new(gensym("ofGetFboType"),
                                       reinterpret_cast<t_newmethod>(ofeliaGetFboType_new),
                                       0, sizeof(t_ofeliaGetFboType),
                                       CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetFboType_class, reinterpret_cast<t_method>(ofeliaGetFboType_bang));
    class_addmethod(ofeliaGetFboType_class, reinterpret_cast<t_method>(ofeliaGetFboType_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetFboType_class, reinterpret_cast<t_method>(ofeliaGetFboType_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetFboType_class, reinterpret_cast<t_method>(ofeliaGetFboType_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetFboMaxSamples object methods
 */
void *ofeliaGetFboMaxSamples_new(t_symbol *s)
{
    t_ofeliaGetFboMaxSamples *x = reinterpret_cast<t_ofeliaGetFboMaxSamples*>(pd_new(ofeliaGetFboMaxSamples_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetFboMaxSamples_bang(t_ofeliaGetFboMaxSamples *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByFboName(name);
        
        if (pos != -1) {
            
            const int maxSamples = t_ofeliaCreateFbo::fbos[pos]->maxSamples();
            outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(maxSamples));
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetFboMaxSamples::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetFboMaxSamples::objName);
    }
}

void ofeliaGetFboMaxSamples_set(t_ofeliaGetFboMaxSamples *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetFboMaxSamples_print(t_ofeliaGetFboMaxSamples *x)
{
    post("\n[%s]", t_ofeliaGetFboMaxSamples::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetFboMaxSamples_setup()
{
    ofeliaGetFboMaxSamples_class = class_new(gensym("ofGetFboMaxSamples"),
                                             reinterpret_cast<t_newmethod>(ofeliaGetFboMaxSamples_new),
                                             0, sizeof(t_ofeliaGetFboMaxSamples),
                                             CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetFboMaxSamples_class, reinterpret_cast<t_method>(ofeliaGetFboMaxSamples_bang));
    class_addmethod(ofeliaGetFboMaxSamples_class, reinterpret_cast<t_method>(ofeliaGetFboMaxSamples_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetFboMaxSamples_class, reinterpret_cast<t_method>(ofeliaGetFboMaxSamples_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetFboMaxSamples_class, reinterpret_cast<t_method>(ofeliaGetFboMaxSamples_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetFboTexID object methods
 */
void *ofeliaGetFboTexID_new(t_symbol *s)
{
    t_ofeliaGetFboTexID *x = reinterpret_cast<t_ofeliaGetFboTexID*>(pd_new(ofeliaGetFboTexID_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetFboTexID_bang(t_ofeliaGetFboTexID *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByFboName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaCreateFbo::fbos[pos]->isAllocated())
                outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(t_ofeliaCreateFbo::fbos[pos]->getTexture().getTextureData().textureID));
            else
                error("%s: '%s' is not allocated", t_ofeliaGetFboTexID::objName, name.c_str());
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetFboTexID::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetFboTexID::objName);
    }
}

void ofeliaGetFboTexID_set(t_ofeliaGetFboTexID *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetFboTexID_print(t_ofeliaGetFboTexID *x)
{
    post("\n[%s]", t_ofeliaGetFboTexID::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetFboTexID_setup()
{
    ofeliaGetFboTexID_class = class_new(gensym("ofGetFboTexID"),
                                        reinterpret_cast<t_newmethod>(ofeliaGetFboTexID_new),
                                        0, sizeof(t_ofeliaGetFboTexID),
                                        CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetFboTexID_class, reinterpret_cast<t_method>(ofeliaGetFboTexID_bang));
    class_addmethod(ofeliaGetFboTexID_class, reinterpret_cast<t_method>(ofeliaGetFboTexID_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetFboTexID_class, reinterpret_cast<t_method>(ofeliaGetFboTexID_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetFboTexID_class, reinterpret_cast<t_method>(ofeliaGetFboTexID_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * setup methods
 */
void ofeliaFbo_setup()
{
    ofeliaCreateFbo_setup();
    ofeliaBindFboTex_setup();
    ofeliaDrawFbo_setup();
    ofeliaDoesFboNameExist_setup();
    ofeliaIsFboAllocated_setup();
    ofeliaGetFboDimen_setup();
    ofeliaGetFboType_setup();
    ofeliaGetFboMaxSamples_setup();
    ofeliaGetFboTexID_setup();
}

