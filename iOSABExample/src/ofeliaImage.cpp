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

#include "ofeliaImage.h"
#include "ofeliaWindow.h"
#include "ofeliaFbo.h"

/* ________________________________________________________________________________
 * static variables
 */
const char *t_ofeliaCreateImage::objName = "ofCreateImage";
unsigned int t_ofeliaCreateImage::counter;
vector<t_ofeliaCommonImageData> t_ofeliaCreateImage::imageData;
vector<shared_ptr<ofImage>> t_ofeliaCreateImage::images;
bool t_ofeliaCreateImage::bInited;
const char *t_ofeliaLoadImage::objName = "ofLoadImage";
unsigned int t_ofeliaLoadImage::counter;
vector<t_ofeliaCommonImageData> t_ofeliaLoadImage::imageData;
vector<vector<shared_ptr<ofImage>>> t_ofeliaLoadImage::images;
vector<vector<shared_ptr<ofImage>>> t_ofeliaLoadImage::tempImages;
vector<unique_ptr<ofeliaImageLoader>> t_ofeliaLoadImage::loaders;
bool t_ofeliaLoadImage::bInited;
const char *t_ofeliaEditImage::objName = "ofEditImage";
const char *t_ofeliaSaveImage::objName = "ofSaveImage";
const char *t_ofeliaBindImageTex::objName = "ofBindImageTex";
const char *t_ofeliaDrawImage::objName = "ofDrawImage";
const char *t_ofeliaDrawSubImage::objName = "ofDrawSubImage";
const char *t_ofeliaDoesImageNameExist::objName = "ofDoesImageNameExist";
const char *t_ofeliaGetImagePath::objName = "ofGetImagePath";
const char *t_ofeliaIsImageAllocated::objName = "ofIsImageAllocated";
const char *t_ofeliaGetImageDimen::objName = "ofGetImageDimen";
const char *t_ofeliaGetImageType::objName = "ofGetImageType";
const char *t_ofeliaGetImageColorAt::objName = "ofGetImageColorAt";
const char *t_ofeliaGetImageTexCoord::objName = "ofGetImageTexCoord";
const char *t_ofeliaGetImageTexCoords::objName = "ofGetImageTexCoords";
const char *t_ofeliaGetImageTexID::objName = "ofGetImageTexID";

/* ________________________________________________________________________________
 * ofCreateImage object methods
 */
void ofeliaCreateImage_jobDoneOut(t_ofeliaCreateImage *x);

int getPositionByCreatedImageObjID(const unsigned int objID)
{
    const auto it = find_if(begin(t_ofeliaCreateImage::imageData), end(t_ofeliaCreateImage::imageData),
                            [&objID](const t_ofeliaCommonImageData &imageData)
                            {
                                return imageData.objID == objID;
                            });
    return static_cast<int>(it - t_ofeliaCreateImage::imageData.begin());
}

void *ofeliaCreateImage_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaCreateImage *x = reinterpret_cast<t_ofeliaCreateImage*>(pd_new(ofeliaCreateImage_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    t_ofeliaCommonImageData imageData;
    getVarNameLocalPrefixes(imageData.varName);
    
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            imageData.varName.name = argv[0].a_w.w_symbol->s_name;
            getVarNameLocalized(imageData.varName);
            
            if (--argc) {
                
                if (!getImageDataFromArgs(argc, argv+1, imageData.data, t_ofeliaCreateImage::objName))
                    return 0;
            }
            else {
                
                imageData.data.width = 0;
                imageData.data.height = 0;
                imageData.data.type = OF_IMAGE_GRAYSCALE;
                imageData.data.color.set(ofColor::white);
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaCreateImage::objName);
            return 0;
        }
    }
    else {
        
        imageData.data.width = 0;
        imageData.data.height = 0;
        imageData.data.type = OF_IMAGE_GRAYSCALE;
        imageData.data.color.set(ofColor::white);
    }
    x->shouldAlloc = true;
    x->bInitGate = false;
    x->objID = imageData.objID = t_ofeliaCreateImage::counter++;
    t_ofeliaCreateImage::imageData.push_back(imageData);
    t_ofeliaCreateImage::images.push_back(make_shared<ofImage>());
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::updateSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    x->jobDone_out = outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaCreateImage_init(t_ofeliaCreateImage *x)
{
    if (x->bInitGate) {
        
        if (!t_ofeliaCreateImage::bInited) {
            
            t_ofeliaCreateImage::images.clear();
            
            for (size_t i=0; i<t_ofeliaCreateImage::imageData.size(); ++i)
                t_ofeliaCreateImage::images.push_back(make_shared<ofImage>());
            t_ofeliaCreateImage::bInited = true;
        }
        x->shouldAlloc = true;
    }
}

void ofeliaCreateImage_update(t_ofeliaCreateImage *x)
{
    if (!x->shouldAlloc && !x->shouldColor)
        return;
    const int pos = getPositionByCreatedImageObjID(x->objID);
    
    if (x->shouldAlloc) {
        
        if (t_ofeliaCreateImage::imageData[pos].data.width && t_ofeliaCreateImage::imageData[pos].data.height) {
            
            t_ofeliaCreateImage::images[pos]->allocate(t_ofeliaCreateImage::imageData[pos].data.width, t_ofeliaCreateImage::imageData[pos].data.height, t_ofeliaCreateImage::imageData[pos].data.type);
            x->shouldColor = true;
        }
        else {
            
            t_ofeliaCreateImage::images[pos]->clear();
        }
        x->shouldAlloc = false;
    }
    if (x->shouldColor) {
        
        if (t_ofeliaCreateImage::images[pos]->isAllocated()) {
            
            t_ofeliaCreateImage::images[pos]->setColor(t_ofeliaCreateImage::imageData[pos].data.color);
            t_ofeliaCreateImage::images[pos]->update();
        }
        ofeliaCreateImage_jobDoneOut(x);
        x->shouldColor = false;
    }
}

void ofeliaCreateImage_exit(t_ofeliaCreateImage *x)
{
    if (t_ofeliaCreateImage::bInited)
        t_ofeliaCreateImage::bInited = false;
    x->shouldAlloc = false;
    x->bInitGate = true;
}

void ofeliaCreateImage_jobDoneOut(t_ofeliaCreateImage *x)
{
    outlet_bang(x->jobDone_out);
}

void ofeliaCreateImage_name(t_ofeliaCreateImage *x, t_symbol *s)
{
    const int pos = getPositionByCreatedImageObjID(x->objID);
    t_ofeliaCreateImage::imageData[pos].varName.name = s->s_name;
    getVarNameLocalized(t_ofeliaCreateImage::imageData[pos].varName);
}

void ofeliaCreateImage_allocate(t_ofeliaCreateImage *x, t_symbol *s, int argc, t_atom *argv)
{
    const int pos = getPositionByCreatedImageObjID(x->objID);
    
    if (getImageDataFromArgs(argc, argv, t_ofeliaCreateImage::imageData[pos].data, t_ofeliaCreateImage::objName))
        x->shouldAlloc = true;
}

void ofeliaCreateImage_color(t_ofeliaCreateImage *x, t_symbol *s, int argc, t_atom *argv)
{
    const int pos = getPositionByCreatedImageObjID(x->objID);
    
    if (getColorAlphaFromArgs(argc, argv, t_ofeliaCreateImage::imageData[pos].data.color, t_ofeliaCreateImage::objName))
        x->shouldColor = true;
}

void ofeliaCreateImage_clear(t_ofeliaCreateImage *x)
{
    const int pos = getPositionByCreatedImageObjID(x->objID);
    t_ofeliaCreateImage::imageData[pos].data.width = 0;
    t_ofeliaCreateImage::imageData[pos].data.height = 0;
    t_ofeliaCreateImage::imageData[pos].data.type = OF_IMAGE_GRAYSCALE;
    t_ofeliaCreateImage::imageData[pos].data.color.set(ofColor::white);
    x->shouldAlloc = true;
}

void ofeliaCreateImage_set(t_ofeliaCreateImage *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            ofeliaCreateImage_name(x, argv[0].a_w.w_symbol);
            
            if (--argc)
                ofeliaCreateImage_allocate(x, 0, argc, argv+1);
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaCreateImage::objName);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaCreateImage::objName);
    }
}

void ofeliaCreateImage_print(t_ofeliaCreateImage *x)
{
    post("\n[%s]", t_ofeliaCreateImage::objName);
    const int pos = getPositionByCreatedImageObjID(x->objID);
    post("name : %s", t_ofeliaCreateImage::imageData[pos].varName.name.c_str());
    t_string type;
    
    if (t_ofeliaCreateImage::imageData[pos].data.type == OF_IMAGE_GRAYSCALE)
        type = "GRAY";
    else if (t_ofeliaCreateImage::imageData[pos].data.type == OF_IMAGE_COLOR)
        type = "RGB";
    else if (t_ofeliaCreateImage::imageData[pos].data.type == OF_IMAGE_COLOR_ALPHA)
        type = "RGBA";
    post("allocate : %d %d %s", t_ofeliaCreateImage::imageData[pos].data.width, t_ofeliaCreateImage::imageData[pos].data.height, type.c_str());
    const ofColor &color = t_ofeliaCreateImage::imageData[pos].data.color;
    post("color : %d %d %d %d", color.r, color.g, color.b, color.a);
}

void ofeliaCreateImage_free(t_ofeliaCreateImage *x)
{
    const int pos = getPositionByCreatedImageObjID(x->objID);
    t_ofeliaCreateImage::imageData.erase(t_ofeliaCreateImage::imageData.begin() + pos);
    t_ofeliaCreateImage::images.erase(t_ofeliaCreateImage::images.begin() + pos);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::updateSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    outlet_free(x->jobDone_out);
}

void ofeliaCreateImage_setup()
{
    ofeliaCreateImage_class = class_new(gensym("ofCreateImage"),
                                        reinterpret_cast<t_newmethod>(ofeliaCreateImage_new),
                                        reinterpret_cast<t_method>(ofeliaCreateImage_free),
                                        sizeof(t_ofeliaCreateImage),
                                        CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaCreateImage_class, reinterpret_cast<t_method>(ofeliaCreateImage_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaCreateImage_class, reinterpret_cast<t_method>(ofeliaCreateImage_update),
                    t_ofeliaWindow::updateMess, A_NULL, 0);
    class_addmethod(ofeliaCreateImage_class, reinterpret_cast<t_method>(ofeliaCreateImage_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addmethod(ofeliaCreateImage_class, reinterpret_cast<t_method>(ofeliaCreateImage_name),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaCreateImage_class, reinterpret_cast<t_method>(ofeliaCreateImage_allocate),
                    gensym("allocate"), A_GIMME, 0);
    class_addmethod(ofeliaCreateImage_class, reinterpret_cast<t_method>(ofeliaCreateImage_color),
                    gensym("color"), A_GIMME, 0);
    class_addmethod(ofeliaCreateImage_class, reinterpret_cast<t_method>(ofeliaCreateImage_clear),
                    gensym("clear"), A_NULL, 0);
    class_addmethod(ofeliaCreateImage_class, reinterpret_cast<t_method>(ofeliaCreateImage_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaCreateImage_class, reinterpret_cast<t_method>(ofeliaCreateImage_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofLoadImage object methods
 */
void ofeliaLoadImage_clear(t_ofeliaLoadImage *x);
void ofeliaLoadImage_vecSizeOut(t_ofeliaLoadImage *x);

int getPositionByLoadedImageObjID(const unsigned int objID)
{
    const auto it = find_if(begin(t_ofeliaLoadImage::imageData), end(t_ofeliaLoadImage::imageData),
                            [&objID](const t_ofeliaCommonImageData &imageData)
                            {
                                return imageData.objID == objID;
                            });
    return static_cast<int>(it - t_ofeliaLoadImage::imageData.begin());
}

bool getLoadImagePathFromArgs(t_ofeliaLoadImage *x, const int argc, const t_atom *argv, t_string &path)
{
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            path = argv[0].a_w.w_symbol->s_name;
            
            if (!doesFilenameHaveExtension(path)) {
                
                error("%s: wrong file format '%s'", t_ofeliaLoadImage::objName, path.c_str());
                return 0;
            }
            if (!makePathAbsolute(path, x->canvas)) {
                
                error("%s: failed to find '%s'", t_ofeliaLoadImage::objName, path.c_str());
                return 0;
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadImage::objName);
            return 0;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadImage::objName);
        return 0;
    }
    return 1;
}

bool getLoadImagePathsFromArgs(t_ofeliaLoadImage *x, const int argc, const t_atom *argv, vector<t_string> &paths)
{
    if (argc) {
        
        for (int i=0; i<argc; ++i) {
            
            if (argv[i].a_type == A_SYMBOL) {
                
                t_string path = argv[i].a_w.w_symbol->s_name;
                
                if (!doesFilenameHaveExtension(path)) {
                    
                    error("%s: wrong file format '%s'", t_ofeliaLoadImage::objName, path.c_str());
                    return 0;
                }
                if (!makePathAbsolute(path, x->canvas)) {
                    
                    error("%s: failed to find '%s'", t_ofeliaLoadImage::objName, path.c_str());
                    return 0;
                }
                paths.push_back(path);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaLoadImage::objName);
                return 0;
            }
        }
        return 1;
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadImage::objName);
        return 0;
    }
    return 1;
}

bool getCmdRangeFromArgs(t_ofeliaLoadImage *x, const int argc, const t_atom *argv, t_ofeliaLoadImageCmdData &cmd)
{
    switch (argc) {
            
        case 0:
        {
            cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                cmd.fromIndex = cmd.toIndex = truncf(argv[0].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaLoadImage::objName);
                return 0;
            }
            break;
        }
        case 2:
        {
            if (argv[0].a_type == A_FLOAT &&
                argv[1].a_type == A_FLOAT) {
                
                cmd.fromIndex = truncf(argv[0].a_w.w_float);
                cmd.toIndex = truncf(argv[1].a_w.w_float);
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaLoadImage::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaLoadImage::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaLoadImage_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadImage *x = reinterpret_cast<t_ofeliaLoadImage*>(pd_new(ofeliaLoadImage_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    t_ofeliaCommonImageData imageData;
    getVarNameLocalPrefixes(imageData.varName);
    vector<t_string> paths;
    x->canvas = canvas_getcurrent();
    
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            imageData.varName.name = argv[0].a_w.w_symbol->s_name;
            getVarNameLocalized(imageData.varName);
            
            if (--argc) {
                
                if (!getLoadImagePathsFromArgs(x, argc, argv+1, paths))
                    return 0;
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadImage::objName);
            return 0;
        }
    }
    x->failPathsShouldClear = false;
    x->vecSizesShouldClear = false;
    x->shouldOutlet = false;
    x->bInitGate = false;
    x->imgID = 0;
    x->objID = imageData.objID = t_ofeliaLoadImage::counter++;
    t_ofeliaLoadImage::imageData.push_back(imageData);
    t_ofeliaLoadImage::images.push_back(vector<shared_ptr<ofImage>>());
    t_ofeliaLoadImage::tempImages.push_back(vector<shared_ptr<ofImage>>());
    t_ofeliaLoadImage::loaders.push_back(make_unique<ofeliaImageLoader>());
    x->imageLoaded = make_unique<vector<char>>();
    x->imageIDs = make_unique<vector<unsigned int>>();
    x->tempImageIDs = make_unique<vector<unsigned int>>();
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::updateSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    x->jobDone_out = outlet_new(&x->x_obj, &s_bang);
    x->vecSize_out = outlet_new(&x->x_obj, &s_float);
    
    if (argc) {
        
        ofeliaLoadImage_clear(x);
        
        for (size_t i=0; i<paths.size(); ++i) {
            
            t_ofeliaLoadImageCmdData cmd;
            cmd.path = paths[i];
            cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
            cmd.state = IMAGE_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
    return (x);
}

void ofeliaLoadImage_init(t_ofeliaLoadImage *x)
{
    if (!t_ofeliaLoadImage::bInited && x->bInitGate) {
        
        t_ofeliaLoadImage::images.clear();
        t_ofeliaLoadImage::tempImages.clear();
        t_ofeliaLoadImage::loaders.clear();
        
        for (size_t i=0; i<t_ofeliaLoadImage::imageData.size(); ++i) {
            
            t_ofeliaLoadImage::images.push_back(vector<shared_ptr<ofImage>>());
            t_ofeliaLoadImage::tempImages.push_back(vector<shared_ptr<ofImage>>());
            t_ofeliaLoadImage::loaders.push_back(make_unique<ofeliaImageLoader>());
            
            for (size_t j=0; j<t_ofeliaLoadImage::imageData[i].paths.size(); ++j) {
                
                t_ofeliaLoadImage::images[i].push_back(make_shared<ofImage>());
                t_ofeliaLoadImage::tempImages[i].push_back(make_shared<ofImage>());
                t_ofeliaLoadImage::loaders[i]->loadFromDisk(*t_ofeliaLoadImage::tempImages[i].back(),
                                                            t_ofeliaLoadImage::imageData[i].paths[j]);
            }
        }
        t_ofeliaLoadImage::bInited = true;
    }
}

void ofeliaLoadImage_update(t_ofeliaLoadImage *x)
{
    const int pos = getPositionByLoadedImageObjID(x->objID);
    
    if (!x->cmdVec.empty()) {
        
        const size_t cmdVecSize = x->cmdVec.size();
        
        /* read all recorded commands */
        for (size_t i=0; i<cmdVecSize; ++i) {
            
            const int size = static_cast<int>(t_ofeliaLoadImage::imageData[pos].paths.size());
            const auto pathsBegin = t_ofeliaLoadImage::imageData[pos].paths.begin();
            const auto imagesBegin = t_ofeliaLoadImage::images[pos].begin();
            const auto imageLoadedBegin = x->imageLoaded->begin();
            const auto imageIDsBegin = x->imageIDs->begin();
            const t_string &path = x->cmdVec[i].path;
            
            switch (x->cmdVec[i].state) {
                    
                case IMAGE_LOAD_CMD_INSERT:
                {
                    const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, size));
                    const int insertSize = static_cast<int>(max(0.0f, max(0.0f, x->cmdVec[i].toIndex) - max(0.0f, x->cmdVec[i].fromIndex))) + 1;
                    t_ofeliaLoadImage::imageData[pos].paths.insert(pathsBegin + fromIndex,
                                                                   insertSize, path);
                    t_ofeliaLoadImage::images[pos].insert(imagesBegin + fromIndex,
                                                          insertSize, make_shared<ofImage>());
                    x->imageLoaded->insert(imageLoadedBegin + fromIndex, insertSize, 0);
                    x->imageIDs->insert(imageIDsBegin + fromIndex, insertSize, 0);
                    break;
                }
                case IMAGE_LOAD_CMD_ERASE:
                {
                    if (size) {
                        
                        const int maxIndex = size - 1;
                        const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, maxIndex));
                        const int toIndex = static_cast<int>(ofClamp(x->cmdVec[i].toIndex, fromIndex, maxIndex)) + 1;
                        t_ofeliaLoadImage::imageData[pos].paths.erase(pathsBegin + fromIndex,
                                                                      pathsBegin + toIndex);
                        t_ofeliaLoadImage::images[pos].erase(imagesBegin + fromIndex,
                                                             imagesBegin + toIndex);
                        x->imageLoaded->erase(imageLoadedBegin + fromIndex, imageLoadedBegin + toIndex);
                        x->imageIDs->erase(imageIDsBegin + fromIndex, imageIDsBegin + toIndex);
                        x->shouldOutlet = true;
                    }
                    break;
                }
                case IMAGE_LOAD_CMD_FILL:
                {
                    if (size) {
                        
                        const int maxIndex = size - 1;
                        const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, maxIndex));
                        const int toIndex = static_cast<int>(ofClamp(x->cmdVec[i].toIndex, fromIndex, maxIndex)) + 1;
                        fill(pathsBegin + fromIndex, pathsBegin + toIndex, path);
                        fill(imageIDsBegin + fromIndex, imageIDsBegin + toIndex, 0);
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
        /* load images in other thread. store IDs to pass the image when loading gets done */
        for (size_t i=0; i<x->imageIDs->size(); ++i) {
            
            if (x->imageIDs->at(i) == 0) {
                
                x->imgID++;
                x->imageIDs->at(i) = x->imgID;
                x->tempImageIDs->push_back(x->imgID);
                t_ofeliaLoadImage::tempImages[pos].push_back(make_shared<ofImage>());
                t_ofeliaLoadImage::loaders[pos]->loadFromDisk(*t_ofeliaLoadImage::tempImages[pos].back(),
                                                              t_ofeliaLoadImage::imageData[pos].paths[i]);
            }
        }
        if (x->cmdVec.size() > cmdVecSize) {
            
            x->cmdVec.erase(x->cmdVec.begin(), x->cmdVec.begin() + cmdVecSize);
            return;
        }
        x->cmdVec.clear();
    }
    /* update threaded loader to catch loaded images and to enable texture */
    t_ofeliaLoadImage::loaders[pos]->update();
    
    if (x->failPathsShouldClear) {
        
        x->failPaths.clear();
        x->failPathsShouldClear = false;
    }
    if (!x->tempImageIDs->empty()) {
        
        if (t_ofeliaLoadImage::tempImages[pos][0]->isUsingTexture()) {
            
            /* find where to pass the image based on ID */
            for (size_t i=0; i<x->imageIDs->size(); ++i) {
                
                if (x->imageIDs->at(i) == x->tempImageIDs->at(0)) {
                    
                    if (t_ofeliaLoadImage::tempImages[pos][0]->isAllocated())
                        t_ofeliaLoadImage::images[pos][i] = t_ofeliaLoadImage::tempImages[pos][0];
                    else
                        x->failPaths.push_back(t_ofeliaLoadImage::imageData[pos].paths[i]);
                    x->imageLoaded->at(i) = 1;
                    x->shouldOutlet = true;
                    break;
                }
            }
            x->tempImageIDs->erase(x->tempImageIDs->begin());
            t_ofeliaLoadImage::tempImages[pos].erase(t_ofeliaLoadImage::tempImages[pos].begin());
        }
    }
    /* output size when there's change. report error if loading fails */
    if (x->shouldOutlet) {
        
        if (x->vecSizesShouldClear) {
            
            x->vecSizes.clear();
            x->vecSizesShouldClear = false;
        }
        const int numLoadedImages = count_if(x->imageLoaded->begin(),
                                             x->imageLoaded->end(),
                                             [] (char c)
                                             {
                                                 return c != 0;
                                             });
        x->vecSizes.push_back(numLoadedImages);
        ofeliaLoadImage_vecSizeOut(x);
        x->shouldOutlet = false;
    }
}

void ofeliaLoadImage_exit(t_ofeliaLoadImage *x)
{
    const int pos = getPositionByLoadedImageObjID(x->objID);
    t_ofeliaLoadImage::loaders[pos]->images_to_load_from_disk.close();
    t_ofeliaLoadImage::loaders[pos]->images_to_update.close();
    t_ofeliaLoadImage::loaders[pos]->waitForThread();
    
    if (t_ofeliaLoadImage::bInited)
        t_ofeliaLoadImage::bInited = false;
    fill(x->imageLoaded->begin(), x->imageLoaded->end(), 0); /* mark unloaded */
    *x->tempImageIDs = *x->imageIDs; /* copy for reloading */
    x->vecSizes.clear();
    x->failPathsShouldClear = false;
    x->vecSizesShouldClear = false;
    x->shouldOutlet = false;
    x->bInitGate = true;
}

void ofeliaLoadImage_vecSizeOut(t_ofeliaLoadImage *x)
{
    if (!x->failPaths.empty()) {
        
        for (size_t i=0; i<x->failPaths.size(); ++i)
            error("%s: failed to load '%s'", t_ofeliaLoadImage::objName, x->failPaths[i].c_str());
        x->failPathsShouldClear = true;
    }
    if (!x->vecSizes.empty()) {
        
        for (size_t i=0; i<x->vecSizes.size(); ++i)
            outlet_float(x->vecSize_out, static_cast<t_float>(x->vecSizes[i]));
        x->vecSizesShouldClear = true;
    }
    if (x->tempImageIDs->empty())
        outlet_bang(x->jobDone_out);
}

void ofeliaLoadImage_name(t_ofeliaLoadImage *x, t_symbol *s)
{
    const int pos = getPositionByLoadedImageObjID(x->objID);
    t_ofeliaLoadImage::imageData[pos].varName.name = s->s_name;
    getVarNameLocalized(t_ofeliaLoadImage::imageData[pos].varName);
}

void ofeliaLoadImage_load(t_ofeliaLoadImage *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_string> paths;
    
    if (getLoadImagePathsFromArgs(x, argc, argv, paths)) {
        
        ofeliaLoadImage_clear(x);
        
        for (size_t i=0; i<paths.size(); ++i) {
            
            t_ofeliaLoadImageCmdData cmd;
            cmd.path = paths[i];
            cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
            cmd.state = IMAGE_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadImage_add(t_ofeliaLoadImage *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadImageCmdData cmd;
    
    if (getLoadImagePathFromArgs(x, argc, argv, cmd.path)) {
        
        cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
        cmd.state = IMAGE_LOAD_CMD_INSERT;
        x->cmdVec.push_back(cmd);
    }
}

void ofeliaLoadImage_append(t_ofeliaLoadImage *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_string> paths;
    
    if (getLoadImagePathsFromArgs(x, argc, argv, paths)) {
        
        for (size_t i=0; i<paths.size(); ++i) {
            
            t_ofeliaLoadImageCmdData cmd;
            cmd.path = paths[i];
            cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
            cmd.state = IMAGE_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadImage_prepend(t_ofeliaLoadImage *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_string> paths;
    
    if (getLoadImagePathsFromArgs(x, argc, argv, paths)) {
        
        for (size_t i=0; i<paths.size(); ++i) {
            
            t_ofeliaLoadImageCmdData cmd;
            cmd.path = paths[i];
            cmd.fromIndex = cmd.toIndex = static_cast<t_float>(i);
            cmd.state = IMAGE_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadImage_insert(t_ofeliaLoadImage *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadImageCmdData cmd;
    
    if (getLoadImagePathFromArgs(x, argc, argv, cmd.path)) {
        
        if (getCmdRangeFromArgs(x, argc-1, argv+1, cmd)) {
            
            cmd.state = IMAGE_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadImage_fill(t_ofeliaLoadImage *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadImageCmdData cmd;
    
    if (getLoadImagePathFromArgs(x, argc, argv, cmd.path)) {
        
        if (getCmdRangeFromArgs(x, argc-1, argv+1, cmd)) {
            
            cmd.state = IMAGE_LOAD_CMD_FILL;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadImage_erase(t_ofeliaLoadImage *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadImageCmdData cmd;
    
    if (getCmdRangeFromArgs(x, argc, argv, cmd)) {
        
        cmd.state = IMAGE_LOAD_CMD_ERASE;
        x->cmdVec.push_back(cmd);
    }
}

void ofeliaLoadImage_clear(t_ofeliaLoadImage *x)
{
    t_ofeliaLoadImageCmdData cmd;
    cmd.fromIndex = 0.0f;
    cmd.toIndex = numeric_limits<t_float>::max();
    cmd.state = IMAGE_LOAD_CMD_ERASE;
    x->cmdVec.push_back(cmd);
}

void ofeliaLoadImage_set(t_ofeliaLoadImage *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            ofeliaLoadImage_name(x, argv[0].a_w.w_symbol);
            
            if (--argc)
                ofeliaLoadImage_load(x, 0, argc, argv+1);
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadImage::objName);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadImage::objName);
    }
}

void ofeliaLoadImage_print(t_ofeliaLoadImage *x)
{
    post("\n[%s]", t_ofeliaLoadImage::objName);
    const int pos = getPositionByLoadedImageObjID(x->objID);
    post("name : %s", t_ofeliaLoadImage::imageData[pos].varName.name.c_str());
    post("load : ");
    
    for (size_t i=0; i<t_ofeliaLoadImage::imageData[pos].paths.size(); ++i)
        post(" %s", t_ofeliaLoadImage::imageData[pos].paths[i].c_str());
}

void ofeliaLoadImage_free(t_ofeliaLoadImage *x)
{
    const int pos = getPositionByLoadedImageObjID(x->objID);
    t_ofeliaLoadImage::loaders[pos]->images_to_load_from_disk.close();
    t_ofeliaLoadImage::loaders[pos]->images_to_update.close();
    t_ofeliaLoadImage::loaders[pos]->waitForThread();
    t_ofeliaLoadImage::imageData.erase(t_ofeliaLoadImage::imageData.begin() + pos);
    t_ofeliaLoadImage::images.erase(t_ofeliaLoadImage::images.begin() + pos);
    t_ofeliaLoadImage::tempImages.erase(t_ofeliaLoadImage::tempImages.begin() + pos);
    t_ofeliaLoadImage::loaders.erase(t_ofeliaLoadImage::loaders.begin() + pos);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::updateSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    outlet_free(x->jobDone_out);
    outlet_free(x->vecSize_out);
}

void ofeliaLoadImage_setup()
{
    ofeliaLoadImage_class = class_new(gensym("ofLoadImage"),
                                      reinterpret_cast<t_newmethod>(ofeliaLoadImage_new),
                                      reinterpret_cast<t_method>(ofeliaLoadImage_free),
                                      sizeof(t_ofeliaLoadImage),
                                      CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaLoadImage_class, reinterpret_cast<t_method>(ofeliaLoadImage_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaLoadImage_class, reinterpret_cast<t_method>(ofeliaLoadImage_update),
                    t_ofeliaWindow::updateMess, A_NULL, 0);
    class_addmethod(ofeliaLoadImage_class, reinterpret_cast<t_method>(ofeliaLoadImage_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addmethod(ofeliaLoadImage_class, reinterpret_cast<t_method>(ofeliaLoadImage_name),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaLoadImage_class, reinterpret_cast<t_method>(ofeliaLoadImage_load),
                    gensym("load"), A_GIMME, 0);
    class_addmethod(ofeliaLoadImage_class, reinterpret_cast<t_method>(ofeliaLoadImage_add),
                    gensym("add"), A_GIMME, 0);
    class_addmethod(ofeliaLoadImage_class, reinterpret_cast<t_method>(ofeliaLoadImage_append),
                    gensym("append"), A_GIMME, 0);
    class_addmethod(ofeliaLoadImage_class, reinterpret_cast<t_method>(ofeliaLoadImage_prepend),
                    gensym("prepend"), A_GIMME, 0);
    class_addmethod(ofeliaLoadImage_class, reinterpret_cast<t_method>(ofeliaLoadImage_insert),
                    gensym("insert"), A_GIMME, 0);
    class_addmethod(ofeliaLoadImage_class, reinterpret_cast<t_method>(ofeliaLoadImage_fill),
                    gensym("fill"), A_GIMME, 0);
    class_addmethod(ofeliaLoadImage_class, reinterpret_cast<t_method>(ofeliaLoadImage_erase),
                    gensym("erase"), A_GIMME, 0);
    class_addmethod(ofeliaLoadImage_class, reinterpret_cast<t_method>(ofeliaLoadImage_clear),
                    gensym("clear"), A_NULL, 0);
    class_addmethod(ofeliaLoadImage_class, reinterpret_cast<t_method>(ofeliaLoadImage_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaLoadImage_class, reinterpret_cast<t_method>(ofeliaLoadImage_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * common methods
 */
int getPositionByCreatedImageName(const t_string &name)
{
    auto it = find_if(begin(t_ofeliaCreateImage::imageData), end(t_ofeliaCreateImage::imageData),
                      [&name] (const t_ofeliaCommonImageData &imageData) {
                          return (imageData.varName.name == name);
                      });
    if (it == end(t_ofeliaCreateImage::imageData))
        return -1;
    return static_cast<int>(it - begin(t_ofeliaCreateImage::imageData));
}

int getPositionByLoadedImageName(const t_string &name)
{
    auto it = find_if(begin(t_ofeliaLoadImage::imageData), end(t_ofeliaLoadImage::imageData),
                      [&name] (const t_ofeliaCommonImageData &imageData) {
                          return (imageData.varName.name == name);
                      });
    if (it == end(t_ofeliaLoadImage::imageData))
        return -1;
    return static_cast<int>(it - begin(t_ofeliaLoadImage::imageData));
}

int getPositionByImageName(const t_string &name, bool &isLoadedImage)
{
    const int pos = getPositionByCreatedImageName(name);
    
    if (pos != -1) {
        
        isLoadedImage = false;
        return pos;
    }
    isLoadedImage = true;
    return getPositionByLoadedImageName(name);
}

/* ________________________________________________________________________________
 * ofEditImage object methods
 */
void ofeliaEditImage_jobDoneOut(t_ofeliaEditImage *x);

void *ofeliaEditImage_new(t_symbol *s)
{
    t_ofeliaEditImage *x = reinterpret_cast<t_ofeliaEditImage*>(pd_new(ofeliaEditImage_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::updateSym);
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaEditImage_float(t_ofeliaEditImage *x, t_floatarg f)
{
    if (x->cmdVec.empty())
        x->varName.index = max(0, static_cast<int>(f));
    else
        error("%s: cannot change index while editing", t_ofeliaEditImage::objName);
}

void ofeliaEditImage_update(t_ofeliaEditImage *x)
{
    if (!x->cmdVec.empty()) {
        
        x->errorType = IMAGE_EDIT_ERROR_NOERROR;
        const t_string &name = x->varName.name;
        const size_t cmdVecSize = x->cmdVec.size();
        
        if (!name.empty()) {
            
            bool isLoadedImage, isEmpty;
            const int pos = getPositionByImageName(name, isLoadedImage);

            if (pos != -1) {
                
                ofImage *img = nullptr;
                
                if (!isLoadedImage) {
                    
                    isEmpty = false;
                    img = t_ofeliaCreateImage::images[pos].get();
                }
                else {
                    
                    const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadImage::imageData[pos].paths.size())-1);
                    
                    isEmpty = t_ofeliaLoadImage::imageData[pos].paths.empty();
                    
                    if (!isEmpty) {
                        
                        if (!t_ofeliaLoadImage::images[pos].empty() &&
                            index < static_cast<int>(t_ofeliaLoadImage::images[pos].size())) {
                            
                            img = t_ofeliaLoadImage::images[pos][index].get();
                        }
                    }
                }
                if (!isEmpty) {
                    
                    if (img) {
                        
                        if (img->isAllocated()) {
                            
                            for (size_t i=0; i<cmdVecSize; ++i) {
                                
                                switch (x->cmdVec[i].state) {
                                        
                                    case IMAGE_EDIT_CMD_COLORAT:
                                    {
                                        img->setColor(getClampIntValue(x->cmdVec[i].args[0], 0, img->getWidth()-1), getClampIntValue(x->cmdVec[i].args[1], 0, img->getHeight()-1), x->cmdVec[i].color);
                                        break;
                                    }
                                    case IMAGE_EDIT_CMD_COLOR:
                                    {
                                        img->setColor(x->cmdVec[i].color);
                                        break;
                                    }
                                    case IMAGE_EDIT_CMD_GRAYSCALE:
                                    {
                                        int numChannels;
                                        
                                        switch (img->getImageType()) {
                                                
                                            case OF_IMAGE_GRAYSCALE:
                                                numChannels = 1;
                                                break;
                                            case OF_IMAGE_COLOR:
                                                numChannels = 3;
                                                break;
                                            case OF_IMAGE_COLOR_ALPHA:
                                                numChannels = 4;
                                                break;
                                            default:
                                                break;
                                        }
                                        if (numChannels == 1)
                                            break;
                                        ofPixels &pix = img->getPixels();
                                        const size_t pixSize = static_cast<size_t>(img->getWidth() * img->getHeight() * numChannels);
                                        
                                        for (size_t i=0; i<pixSize; i+=numChannels)
                                            pix[i] = pix[i+1] = pix[i+2] = static_cast<unsigned char>(0.299f*pix[i]+0.587f*pix[i+1]+0.114f*pix[i+2]);
                                        break;
                                    }
                                    case IMAGE_EDIT_CMD_INVERT:
                                    {
                                        int numChannels;
                                        
                                        switch (img->getImageType()) {
                                                
                                            case OF_IMAGE_GRAYSCALE:
                                                numChannels = 1;
                                                break;
                                            case OF_IMAGE_COLOR:
                                                numChannels = 3;
                                                break;
                                            case OF_IMAGE_COLOR_ALPHA:
                                                numChannels = 4;
                                                break;
                                            default:
                                                break;
                                        }
                                        ofPixels &pix = img->getPixels();
                                        const size_t pixSize = static_cast<size_t>(img->getWidth() * img->getHeight() * numChannels);
                                        
                                        if (numChannels == 1) {
                                            
                                            for (size_t i=0; i<pixSize; ++i)
                                                pix[i] = 255 - pix[i];
                                        }
                                        else {
                                            
                                            for (size_t i=0; i<pixSize; i+=numChannels) {
                                                
                                                pix[i] = 255 - pix[i];
                                                pix[i+1] = 255 - pix[i+1];
                                                pix[i+2] = 255 - pix[i+2];
                                            }
                                        }
                                        break;
                                    }
                                    case IMAGE_EDIT_CMD_BRIGHTNESS:
                                    {
                                        const int brightness = x->cmdVec[i].args[0];
                                        int numChannels;
                                        
                                        switch (img->getImageType()) {
                                                
                                            case OF_IMAGE_GRAYSCALE:
                                                numChannels = 1;
                                                break;
                                            case OF_IMAGE_COLOR:
                                                numChannels = 3;
                                                break;
                                            case OF_IMAGE_COLOR_ALPHA:
                                                numChannels = 4;
                                                break;
                                            default:
                                                break;
                                        }
                                        ofPixels &pix = img->getPixels();
                                        const size_t pixSize = static_cast<size_t>(img->getWidth() * img->getHeight() * numChannels);
                                        
                                        if (numChannels == 1) {
                                            
                                            for (size_t i=0; i<pixSize; ++i) {
                                                
                                                const int g = pix[i] + brightness;
                                                pix[i] = static_cast<unsigned char>(g < 0 ? 0 : g > 255 ? 255 : g);
                                            }
                                        }
                                        else {
                                            
                                            for (size_t i=0; i<pixSize; i+=numChannels) {
                                                
                                                const int r = pix[i] + brightness;
                                                const int g = pix[i+1] + brightness;
                                                const int b = pix[i+2] + brightness;
                                                pix[i] = static_cast<unsigned char>(r < 0 ? 0 : r > 255 ? 255 : r);
                                                pix[i+1] = static_cast<unsigned char>(g < 0 ? 0 : g > 255 ? 255 : g);
                                                pix[i+2] = static_cast<unsigned char>(b < 0 ? 0 : b > 255 ? 255 : b);
                                            }
                                        }
                                        break;
                                    }
                                    case IMAGE_EDIT_CMD_CONTRAST:
                                    {
                                        const int contrast = x->cmdVec[i].args[0];
                                        int numChannels;
                                        
                                        switch (img->getImageType()) {
                                                
                                            case OF_IMAGE_GRAYSCALE:
                                                numChannels = 1;
                                                break;
                                            case OF_IMAGE_COLOR:
                                                numChannels = 3;
                                                break;
                                            case OF_IMAGE_COLOR_ALPHA:
                                                numChannels = 4;
                                                break;
                                            default:
                                                break;
                                        }
                                        ofPixels &pix = img->getPixels();
                                        const size_t pixSize = static_cast<size_t>(img->getWidth() * img->getHeight() * numChannels);
                                        const float factor = (259.0f * (contrast + 255)) / (255.0f * (259 - contrast));
                                        
                                        if (numChannels == 1) {
                                            
                                            for (size_t i=0; i<pixSize; ++i) {
                                                
                                                const int g = static_cast<int>(factor * (pix[i] - 128) + 128);
                                                pix[i] = static_cast<unsigned char>(g < 0 ? 0 : g > 255 ? 255 : g);
                                            }
                                        }
                                        else {
                                            
                                            for (size_t i=0; i<pixSize; i+=numChannels) {
                                                
                                                const int r = static_cast<int>(factor * (pix[i] - 128) + 128);
                                                const int g = static_cast<int>(factor * (pix[i+1] - 128) + 128);
                                                const int b = static_cast<int>(factor * (pix[i+2] - 128) + 128);
                                                pix[i] = static_cast<unsigned char>(r < 0 ? 0 : r > 255 ? 255 : r);
                                                pix[i+1] = static_cast<unsigned char>(g < 0 ? 0 : g > 255 ? 255 : g);
                                                pix[i+2] = static_cast<unsigned char>(b < 0 ? 0 : b > 255 ? 255 : b);
                                            }
                                        }
                                        break;
                                    }
                                    case IMAGE_EDIT_CMD_GAMMA:
                                    {
                                        const float gamma = x->cmdVec[i].args[0] / 1000.0f;
                                        int numChannels;
                                        
                                        switch (img->getImageType()) {
                                                
                                            case OF_IMAGE_GRAYSCALE:
                                                numChannels = 1;
                                                break;
                                            case OF_IMAGE_COLOR:
                                                numChannels = 3;
                                                break;
                                            case OF_IMAGE_COLOR_ALPHA:
                                                numChannels = 4;
                                                break;
                                            default:
                                                break;
                                        }
                                        ofPixels &pix = img->getPixels();
                                        const size_t pixSize = static_cast<size_t>(img->getWidth() * img->getHeight() * numChannels);
                                        const float gammaCorrection = 1.0f / gamma;
                                        
                                        if (numChannels == 1) {
                                            
                                            for (size_t i=0; i<pixSize; ++i)
                                                pix[i] = static_cast<unsigned char>(255.0f * powf(pix[i] / 255.0f, gammaCorrection));
                                        }
                                        else {
                                            
                                            for (size_t i=0; i<pixSize; i+=numChannels) {
                                                
                                                pix[i] = static_cast<unsigned char>(255.0f * powf(pix[i] / 255.0f, gammaCorrection));
                                                pix[i+1] = static_cast<unsigned char>(255.0f * powf(pix[i+1] / 255.0f, gammaCorrection));
                                                pix[i+2] = static_cast<unsigned char>(255.0f * powf(pix[i+2] / 255.0f, gammaCorrection));
                                            }
                                        }
                                        break;
                                    }
                                    case IMAGE_EDIT_CMD_TYPE:
                                    {
                                        ofImageType type;
                                        
                                        switch (x->cmdVec[i].args[0]) {
                                                
                                            case 0:
                                                type = OF_IMAGE_GRAYSCALE;
                                                break;
                                            case 1:
                                                type = OF_IMAGE_COLOR;
                                                break;
                                            case 2:
                                                type = OF_IMAGE_COLOR_ALPHA;
                                                break;
                                            default:
                                                break;
                                        }
                                        img->setImageType(type);
                                        break;
                                    }
                                    case IMAGE_EDIT_CMD_RESIZE:
                                    {
                                        img->resize(x->cmdVec[i].args[0], x->cmdVec[i].args[1]);
                                        break;
                                    }
                                    case IMAGE_EDIT_CMD_CROP:
                                    {
                                        img->crop(getClampIntValue(x->cmdVec[i].args[0], 0, img->getWidth()-1),
                                                  getClampIntValue(x->cmdVec[i].args[1], 0, img->getHeight()-1),
                                                  x->cmdVec[i].args[2], x->cmdVec[i].args[3]);
                                        break;
                                    }
                                    case IMAGE_EDIT_CMD_MIRROR:
                                    {
                                        /* notice : horMode verMode switched for consistency */
                                        img->mirror(x->cmdVec[i].args[1], x->cmdVec[i].args[0]);
                                        break;
                                    }
                                    case IMAGE_EDIT_CMD_GENERATEMIPMAP:
                                    {
                                        img->getTexture().generateMipmap();
                                        break;
                                    }
                                    case IMAGE_EDIT_CMD_MINMAGFILTER:
                                    {
                                        GLint minFilter, magFilter;
                                        
                                        switch (x->cmdVec[i].args[0]) {
                                                
                                            case IMAGE_MINMAG_NEAREST:
                                                minFilter = GL_NEAREST;
                                                break;
                                            case IMAGE_MINMAG_LINEAR:
                                                minFilter = GL_LINEAR;
                                                break;
                                            case IMAGE_MINMAG_NEAREST_MIPMAP_NEAREST:
                                                minFilter = GL_NEAREST_MIPMAP_NEAREST;
                                                break;
                                            case IMAGE_MINMAG_LINEAR_MIPMAP_NEAREST:
                                                minFilter = GL_LINEAR_MIPMAP_NEAREST;
                                                break;
                                            case IMAGE_MINMAG_NEAREST_MIPMAP_LINEAR:
                                                minFilter = GL_NEAREST_MIPMAP_LINEAR;
                                                break;
                                            case IMAGE_MINMAG_LINEAR_MIPMAP_LINEAR:
                                                minFilter = GL_LINEAR_MIPMAP_LINEAR;
                                                break;
                                            default:
                                                break;
                                        }
                                        switch (x->cmdVec[i].args[1]) {
                                                
                                            case IMAGE_MINMAG_NEAREST:
                                                magFilter = GL_NEAREST;
                                                break;
                                            case IMAGE_MINMAG_LINEAR:
                                                magFilter = GL_LINEAR;
                                                break;
                                            default:
                                                break;
                                        }
                                        img->getTexture().setTextureMinMagFilter(minFilter, magFilter);
                                        break;
                                    }
                                    case IMAGE_EDIT_CMD_TEXWRAP:
                                    {
                                        GLint horMode, verMode;
                                        
                                        switch (x->cmdVec[i].args[0]) {
                                                
                                            case IMAGE_TEXWRAP_REPEAT:
                                                horMode = GL_REPEAT;
                                                break;
                                            case IMAGE_TEXWRAP_MIRRORED_REPEAT:
                                                horMode = GL_MIRRORED_REPEAT;
                                                break;
                                            case IMAGE_TEXWRAP_CLAMP_TO_EDGE:
                                                horMode = GL_CLAMP_TO_EDGE;
                                                break;
                                            default:
                                                break;
                                        }
                                        switch (x->cmdVec[i].args[1]) {
                                                
                                            case IMAGE_TEXWRAP_REPEAT:
                                                verMode = GL_REPEAT;
                                                break;
                                            case IMAGE_TEXWRAP_MIRRORED_REPEAT:
                                                verMode = GL_MIRRORED_REPEAT;
                                                break;
                                            case IMAGE_TEXWRAP_CLAMP_TO_EDGE:
                                                verMode = GL_CLAMP_TO_EDGE;
                                                break;
                                            default:
                                                break;
                                        }
                                        img->getTexture().setTextureWrap(horMode, verMode);
                                        break;
                                    }
                                    case IMAGE_EDIT_CMD_CLONE:
                                    {
                                        const t_string &cName = x->cmdVec[i].varName.name;
                                        bool cIsLoadedImage, cIsEmpty;
                                        int cPos = getPositionByImageName(cName, cIsLoadedImage);
                                        
                                        if (cPos != -1) {
                                            
                                            ofImage *cImg = nullptr;
                                            
                                            if (!cIsLoadedImage) {
                                                
                                                cIsEmpty = false;
                                                cImg = t_ofeliaCreateImage::images[cPos].get();
                                            }
                                            else {
                                                
                                                const int cIndex = min(x->cmdVec[i].varName.index, static_cast<int>(t_ofeliaLoadImage::imageData[cPos].paths.size())-1);
                                                cIsEmpty = t_ofeliaLoadImage::imageData[cPos].paths.empty();
                                                
                                                if (!cIsEmpty) {
                                                    
                                                    if (!t_ofeliaLoadImage::images[cPos].empty() &&
                                                        cIndex < static_cast<int>(t_ofeliaLoadImage::images[cPos].size())) {
                                                        
                                                        cImg = t_ofeliaLoadImage::images[cPos][cIndex].get();
                                                    }
                                                }
                                            }
                                            if (!cIsEmpty) {
                                                
                                                if (cImg) {
                                                    
                                                    if (cImg->isAllocated()) {
                                                        
                                                        img->clone(*cImg);
                                                    }
                                                    else {
                                                        
                                                        x->errorType = IMAGE_EDIT_ERROR_NOTALLOCATED;
                                                        x->errorName = cName;
                                                    }
                                                }
                                                else {
                                                    
                                                    x->errorType = IMAGE_EDIT_ERROR_NOTLOADED;
                                                    x->errorName = cName;
                                                }
                                            }
                                            else {
                                                
                                                x->errorType = IMAGE_EDIT_ERROR_EMPTY;
                                                x->errorName = cName;
                                            }
                                        }
                                        else {
                                            
                                            cPos = getPositionByFboName(cName);
                                            
                                            if (cPos != -1) {
                                                
                                                ofPixels pixels;
                                                
                                                if (t_ofeliaCreateFbo::fbos[cPos]->isAllocated()) {
                                                    
                                                    t_ofeliaCreateFbo::fbos[cPos]->readToPixels(pixels);
                                                    img->setFromPixels(pixels);
                                                }
                                                else {
                                                    
                                                    x->errorType = IMAGE_EDIT_ERROR_NOTALLOCATED;
                                                    x->errorName = cName;
                                                }
                                            }
                                            else {
                                                
                                                x->errorType = IMAGE_EDIT_ERROR_FAILTOFIND;
                                                x->errorName = cName;
                                            }
                                        }
                                        break;
                                    }
                                    case IMAGE_EDIT_CMD_ALPHAMASK:
                                    {
                                        const t_string &cName = x->cmdVec[i].varName.name;
                                        bool cIsLoadedImage, cIsEmpty;
                                        int cPos = getPositionByImageName(cName, cIsLoadedImage);
                                        
                                        if (cPos != -1) {
                                            
                                            ofImage *cImg = nullptr;
                                            
                                            if (!cIsLoadedImage) {
                                                
                                                cIsEmpty = false;
                                                cImg = t_ofeliaCreateImage::images[cPos].get();
                                            }
                                            else {
                                                
                                                const int cIndex = min(x->cmdVec[i].varName.index, static_cast<int>(t_ofeliaLoadImage::imageData[cPos].paths.size())-1);
                                                cIsEmpty = t_ofeliaLoadImage::imageData[cPos].paths.empty();
                                                
                                                if (!cIsEmpty) {
                                                    
                                                    if (!t_ofeliaLoadImage::images[cPos].empty() &&
                                                        cIndex < static_cast<int>(t_ofeliaLoadImage::images[cPos].size())) {
                                                        
                                                        cImg = t_ofeliaLoadImage::images[cPos][cIndex].get();
                                                    }
                                                }
                                            }
                                            if (!cIsEmpty) {
                                                
                                                if (cImg) {
                                                    
                                                    if (cImg->isAllocated()) {
                                                        
                                                        img->getTexture().setAlphaMask(cImg->getTexture());
                                                    }
                                                    else {
                                                        
                                                        x->errorType = IMAGE_EDIT_ERROR_NOTALLOCATED;
                                                        x->errorName = cName;
                                                    }
                                                }
                                                else {
                                                    
                                                    x->errorType = IMAGE_EDIT_ERROR_NOTLOADED;
                                                    x->errorName = cName;
                                                }
                                            }
                                            else {
                                                
                                                x->errorType = IMAGE_EDIT_ERROR_EMPTY;
                                                x->errorName = cName;
                                            }
                                        }
                                        else {
                                            
                                            cPos = getPositionByFboName(cName);
                                            
                                            if (cPos != -1) {

                                                if (t_ofeliaCreateFbo::fbos[cPos]->isAllocated()) {
                                                    
                                                    img->getTexture().setAlphaMask(t_ofeliaCreateFbo::fbos[cPos]->getTexture());
                                                }
                                                else {
                                                    
                                                    x->errorType = IMAGE_EDIT_ERROR_NOTALLOCATED;
                                                    x->errorName = cName;
                                                }
                                            }
                                            else {
                                                
                                                x->errorType = IMAGE_EDIT_ERROR_FAILTOFIND;
                                                x->errorName = cName;
                                            }
                                        }
                                        break;
                                    }
                                    case IMAGE_EDIT_CMD_GRABSCREEN:
                                    {
                                        const int retinaScale = ofeliaWindow::retinaScale;
                                        img->grabScreen(max(0, x->cmdVec[i].args[0]) * retinaScale,
                                                        max(0, x->cmdVec[i].args[1]) * retinaScale,
                                                        max(0, x->cmdVec[i].args[2]) * retinaScale,
                                                        max(0, x->cmdVec[i].args[3]) * retinaScale);
                                        break;
                                    }
                                    default:
                                    {
                                        break;
                                    }
                                }
                            }
                            img->update();
                        }
                        else {
                            
                            x->errorType = IMAGE_EDIT_ERROR_NOTALLOCATED;
                            x->errorName = name;
                        }
                    }
                    else {
                        
                        x->errorType = IMAGE_EDIT_ERROR_NOTLOADED;
                        x->errorName = name;
                    }
                }
                else {
                    
                    x->errorType = IMAGE_EDIT_ERROR_EMPTY;
                    x->errorName = name;
                }
            }
            else {
                
                x->errorType = IMAGE_EDIT_ERROR_FAILTOFIND;
                x->errorName = name;
            }
        }
        else {
            
            x->errorType = IMAGE_EDIT_ERROR_NOTASSIGNED;
        }
        if (x->cmdVec.size() > cmdVecSize) {
            
            x->cmdVec.erase(x->cmdVec.begin(), x->cmdVec.begin() + cmdVecSize);
            return;
        }
        x->cmdVec.clear();
        ofeliaEditImage_jobDoneOut(x);
    }
}

void ofeliaEditImage_jobDoneOut(t_ofeliaEditImage *x)
{
    if (x->errorType == IMAGE_EDIT_ERROR_NOERROR) {
        
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        switch (x->errorType) {
                
            case IMAGE_EDIT_ERROR_NOTASSIGNED:
                error("%s: name not assigned", t_ofeliaEditImage::objName);
                break;
            case IMAGE_EDIT_ERROR_FAILTOFIND:
                error("%s: failed to find '%s'", t_ofeliaEditImage::objName, x->errorName.c_str());
                break;
            case IMAGE_EDIT_ERROR_EMPTY:
                error("%s: '%s' is empty", t_ofeliaEditImage::objName, x->errorName.c_str());
                break;
            case IMAGE_EDIT_ERROR_NOTLOADED:
                error("%s: '%s' is not loaded", t_ofeliaEditImage::objName, x->errorName.c_str());
                break;
            case IMAGE_EDIT_ERROR_NOTALLOCATED:
                error("%s: '%s' is not allocated", t_ofeliaEditImage::objName, x->errorName.c_str());
                break;
            default:
                break;
        }
    }
}

void ofeliaEditImage_colorAt(t_ofeliaEditImage *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc > 2) {
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            t_ofeliaEditImageCmdData cmd;
            cmd.args[0] = static_cast<int>(argv[0].a_w.w_float);
            cmd.args[1] = static_cast<int>(argv[1].a_w.w_float);
            
            if (getColorAlphaFromArgs(argc-2, argv+2, cmd.color, t_ofeliaEditImage::objName)) {
                
                cmd.state = IMAGE_EDIT_CMD_COLORAT;
                x->cmdVec.push_back(cmd);
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaEditImage::objName);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaEditImage::objName);
    }
}

void ofeliaEditImage_color(t_ofeliaEditImage *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaEditImageCmdData cmd;
    getColorAlphaFromArgs(argc, argv, cmd.color, t_ofeliaEditImage::objName);
    cmd.state = IMAGE_EDIT_CMD_COLOR;
    x->cmdVec.push_back(cmd);
}

void ofeliaEditImage_grayscale(t_ofeliaEditImage *x)
{
    t_ofeliaEditImageCmdData cmd;
    cmd.state = IMAGE_EDIT_CMD_GRAYSCALE;
    x->cmdVec.push_back(cmd);
}

void ofeliaEditImage_invert(t_ofeliaEditImage *x)
{
    t_ofeliaEditImageCmdData cmd;
    cmd.state = IMAGE_EDIT_CMD_INVERT;
    x->cmdVec.push_back(cmd);
}

void ofeliaEditImage_brightness(t_ofeliaEditImage *x, t_floatarg f)
{
    const int brightness = static_cast<int>(f);
    
    if (!brightness)
        return;
    
    if (brightness < -255 || brightness > 255) {
        
        error("%s: brightness out of range", t_ofeliaEditImage::objName);
        return;
    }
    t_ofeliaEditImageCmdData cmd;
    cmd.args[0] = brightness;
    cmd.state = IMAGE_EDIT_CMD_BRIGHTNESS;
    x->cmdVec.push_back(cmd);
}

void ofeliaEditImage_contrast(t_ofeliaEditImage *x, t_floatarg f)
{
    const int contrast = static_cast<int>(f);
    
    if (!contrast)
        return;
    
    if (contrast < -255 || contrast > 255) {
        
        error("%s: contrast out of range", t_ofeliaEditImage::objName);
        return;
    }
    t_ofeliaEditImageCmdData cmd;
    cmd.args[0] = contrast;
    cmd.state = IMAGE_EDIT_CMD_CONTRAST;
    x->cmdVec.push_back(cmd);
}

void ofeliaEditImage_gamma(t_ofeliaEditImage *x, t_floatarg f)
{
    const int gamma = static_cast<int>(f * 1000.0f);
    
    if (gamma < 0) {
        
        error("%s: gamma out of range", t_ofeliaEditImage::objName);
        return;
    }
    t_ofeliaEditImageCmdData cmd;
    cmd.args[0] = gamma;
    cmd.state = IMAGE_EDIT_CMD_GAMMA;
    x->cmdVec.push_back(cmd);
}

void ofeliaEditImage_type(t_ofeliaEditImage *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc == 1) {
        
        t_ofeliaEditImageCmdData cmd;
        
        if (argv[0].a_type == A_FLOAT) {
            
            const int type = static_cast<int>(argv[0].a_w.w_float);
            
            if (type < 0 || type > 2) {
                
                error("%s: type out of range", t_ofeliaEditImage::objName);
                return;
            }
            cmd.args[0] = type;
        }
        else if (argv[0].a_type == A_SYMBOL) {
            
            const char *type = argv[0].a_w.w_symbol->s_name;
            
            if (!strcmp(type, "GRAY"))
                cmd.args[0] = 0;
            else if (!strcmp(type, "RGB"))
                cmd.args[0] = 1;
            else if (!strcmp(type, "RGBA"))
                cmd.args[0] = 2;
            else {
                
                error("%s: no method for '%s'", t_ofeliaEditImage::objName, type);
                return;
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaEditImage::objName);
            return;
        }
        cmd.state = IMAGE_EDIT_CMD_TYPE;
        x->cmdVec.push_back(cmd);
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaEditImage::objName);
    }
}

void ofeliaEditImage_resize(t_ofeliaEditImage *x, t_floatarg newWidth, t_floatarg newHeight)
{
    t_ofeliaEditImageCmdData cmd;
    cmd.args[0] = static_cast<int>(newWidth);
    cmd.args[1] = static_cast<int>(newHeight);
    cmd.state = IMAGE_EDIT_CMD_RESIZE;
    x->cmdVec.push_back(cmd);
}

void ofeliaEditImage_crop(t_ofeliaEditImage *x, t_floatarg posX, t_floatarg posY, t_floatarg width, t_floatarg height)
{
    t_ofeliaEditImageCmdData cmd;
    cmd.args[0] = static_cast<int>(posX);
    cmd.args[1] = static_cast<int>(posY);
    cmd.args[2] = static_cast<int>(width);
    cmd.args[3] = static_cast<int>(height);
    cmd.state = IMAGE_EDIT_CMD_CROP;
    x->cmdVec.push_back(cmd);
}

void ofeliaEditImage_mirror(t_ofeliaEditImage *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc == 2) {
        
        t_ofeliaEditImageCmdData cmd;
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            cmd.args[0] = argv[0].a_w.w_float != 0.0f;
            cmd.args[1] = argv[1].a_w.w_float != 0.0f;
        }
        else if (argv[0].a_type == A_SYMBOL &&
                 argv[1].a_type == A_SYMBOL) {
            
            const char *horMode = argv[0].a_w.w_symbol->s_name;
            
            if (!strcmp(horMode, "OFF"))
                cmd.args[0] = 0;
            else if (!strcmp(horMode, "ON"))
                cmd.args[0] = 1;
            else {
                
                error("%s: no method for '%s'", t_ofeliaEditImage::objName, horMode);
                return;
            }
            const char *verMode = argv[1].a_w.w_symbol->s_name;
            
            if (!strcmp(verMode, "OFF"))
                cmd.args[1] = 0;
            else if (!strcmp(verMode, "ON"))
                cmd.args[1] = 1;
            else {
                
                error("%s: no method for '%s'", t_ofeliaEditImage::objName, verMode);
                return;
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaEditImage::objName);
            return;
        }
        cmd.state = IMAGE_EDIT_CMD_MIRROR;
        x->cmdVec.push_back(cmd);
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaEditImage::objName);
    }
}

void ofeliaEditImage_generateMipmap(t_ofeliaEditImage *x)
{
    t_ofeliaEditImageCmdData cmd;
    cmd.state = IMAGE_EDIT_CMD_GENERATEMIPMAP;
    x->cmdVec.push_back(cmd);
}

void ofeliaEditImage_minMagFilter(t_ofeliaEditImage *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc == 2) {
        
        t_ofeliaEditImageCmdData cmd;
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            const int minFilter = static_cast<int>(argv[0].a_w.w_float);
            const int magFilter = static_cast<int>(argv[1].a_w.w_float);
            
            if (minFilter < 0 || minFilter > 5) {
                
                error("%s: minFilter out of range", t_ofeliaEditImage::objName);
                return;
            }
            if (magFilter < 0 || magFilter > 1) {
                
                error("%s: magFilter out of range", t_ofeliaEditImage::objName);
                return;
            }
            cmd.args[0] = minFilter;
            cmd.args[1] = magFilter;
        }
        else if (argv[0].a_type == A_SYMBOL &&
                 argv[1].a_type == A_SYMBOL) {
            
            const char *minFilter = argv[0].a_w.w_symbol->s_name;
            
            if (!strcmp(minFilter, "NEAREST"))
                cmd.args[0] = IMAGE_MINMAG_NEAREST;
            else if (!strcmp(minFilter, "LINEAR"))
                cmd.args[0] = IMAGE_MINMAG_LINEAR;
            else if (!strcmp(minFilter, "NEAREST_MIPMAP_NEAREST"))
                cmd.args[0] = IMAGE_MINMAG_NEAREST_MIPMAP_NEAREST;
            else if (!strcmp(minFilter, "LINEAR_MIPMAP_NEAREST"))
                cmd.args[0] = IMAGE_MINMAG_LINEAR_MIPMAP_NEAREST;
            else if (!strcmp(minFilter, "NEAREST_MIPMAP_LINEAR"))
                cmd.args[0] = IMAGE_MINMAG_NEAREST_MIPMAP_LINEAR;
            else if (!strcmp(minFilter, "LINEAR_MIPMAP_LINEAR"))
                cmd.args[0] = IMAGE_MINMAG_LINEAR_MIPMAP_LINEAR;
            else {
                
                error("%s: no method for '%s'", t_ofeliaEditImage::objName, minFilter);
                return;
            }
            const char *magFilter = argv[1].a_w.w_symbol->s_name;
            
            if (!strcmp(magFilter, "NEAREST"))
                cmd.args[1] = IMAGE_MINMAG_NEAREST;
            else if (!strcmp(magFilter, "LINEAR"))
                cmd.args[1] = IMAGE_MINMAG_LINEAR;
            else {
                
                error("%s: no method for '%s'", t_ofeliaEditImage::objName, magFilter);
                return;
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaEditImage::objName);
            return;
        }
        cmd.state = IMAGE_EDIT_CMD_MINMAGFILTER;
        x->cmdVec.push_back(cmd);
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaEditImage::objName);
    }
}

void ofeliaEditImage_texWrap(t_ofeliaEditImage *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc == 2) {
        
        t_ofeliaEditImageCmdData cmd;
        
        if (argv[0].a_type == A_FLOAT &&
            argv[1].a_type == A_FLOAT) {
            
            const int horMode = static_cast<int>(argv[0].a_w.w_float);
            const int verMode = static_cast<int>(argv[1].a_w.w_float);
            
            if (horMode < 0 || horMode > 2) {
                
                error("%s: horMode out of range", t_ofeliaEditImage::objName);
                return;
            }
            if (verMode < 0 || verMode > 2) {
                
                error("%s: verMode out of range", t_ofeliaEditImage::objName);
                return;
            }
            cmd.args[0] = horMode;
            cmd.args[1] = verMode;
        }
        else if (argv[0].a_type == A_SYMBOL &&
                 argv[1].a_type == A_SYMBOL) {
            
            const char *horMode = argv[0].a_w.w_symbol->s_name;
            
            if (!strcmp(horMode, "REPEAT"))
                cmd.args[0] = IMAGE_TEXWRAP_REPEAT;
            else if (!strcmp(horMode, "MIRRORED_REPEAT"))
                cmd.args[0] = IMAGE_TEXWRAP_MIRRORED_REPEAT;
            else if (!strcmp(horMode, "CLAMP_TO_EDGE"))
                cmd.args[0] = IMAGE_TEXWRAP_CLAMP_TO_EDGE;
            else {
                
                error("%s: no method for '%s'", t_ofeliaEditImage::objName, horMode);
                return;
            }
            const char *verMode = argv[1].a_w.w_symbol->s_name;
            
            if (!strcmp(verMode, "REPEAT"))
                cmd.args[1] = IMAGE_TEXWRAP_REPEAT;
            else if (!strcmp(verMode, "MIRRORED_REPEAT"))
                cmd.args[1] = IMAGE_TEXWRAP_MIRRORED_REPEAT;
            else if (!strcmp(verMode, "CLAMP_TO_EDGE"))
                cmd.args[1] = IMAGE_TEXWRAP_CLAMP_TO_EDGE;
            else {
                
                error("%s: no method for '%s'", t_ofeliaEditImage::objName, verMode);
                return;
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaEditImage::objName);
            return;
        }
        cmd.state = IMAGE_EDIT_CMD_TEXWRAP;
        x->cmdVec.push_back(cmd);
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaEditImage::objName);
    }
}

void ofeliaEditImage_clone(t_ofeliaEditImage *x, t_symbol *s)
{
    t_ofeliaEditImageCmdData cmd;
    cmd.varName = {s->s_name, x->varName.patchPrefix, x->varName.canvasPrefix, 0};
    getVarNameLocalized(cmd.varName);
    getVarNameIndexed(cmd.varName);
    cmd.state = IMAGE_EDIT_CMD_CLONE;
    x->cmdVec.push_back(cmd);
}

void ofeliaEditImage_alphaMask(t_ofeliaEditImage *x, t_symbol *s)
{
    t_ofeliaEditImageCmdData cmd;
    cmd.varName = {s->s_name, x->varName.patchPrefix, x->varName.canvasPrefix, 0};
    getVarNameLocalized(cmd.varName);
    getVarNameIndexed(cmd.varName);
    cmd.state = IMAGE_EDIT_CMD_ALPHAMASK;
    x->cmdVec.push_back(cmd);
}

void ofeliaEditImage_grabScreen(t_ofeliaEditImage *x, t_floatarg posX, t_floatarg posY, t_floatarg width, t_floatarg height)
{
    t_ofeliaEditImageCmdData cmd;
    cmd.args[0] = static_cast<int>(posX);
    cmd.args[1] = static_cast<int>(posY);
    cmd.args[2] = static_cast<int>(width);
    cmd.args[3] = static_cast<int>(height);
    cmd.state = IMAGE_EDIT_CMD_GRABSCREEN;
    x->cmdVec.push_back(cmd);
}

void ofeliaEditImage_set(t_ofeliaEditImage *x, t_symbol *s)
{
    if (x->cmdVec.empty()) {
        
        x->varName.name = s->s_name;
        getVarNameLocalized(x->varName);
        getVarNameIndexed(x->varName);
    }
    else {
        
        error("%s: cannot change name while editing", t_ofeliaEditImage::objName);
    }
}

void ofeliaEditImage_print(t_ofeliaEditImage *x)
{
    post("\n[%s]", t_ofeliaEditImage::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaEditImage_free(t_ofeliaEditImage *x)
{
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::updateSym);
}

void ofeliaEditImage_setup()
{
    ofeliaEditImage_class = class_new(gensym("ofEditImage"),
                                      reinterpret_cast<t_newmethod>(ofeliaEditImage_new),
                                      reinterpret_cast<t_method>(ofeliaEditImage_free),
                                      sizeof(t_ofeliaEditImage),
                                      CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaEditImage_class, reinterpret_cast<t_method>(ofeliaEditImage_float));
    class_addmethod(ofeliaEditImage_class, reinterpret_cast<t_method>(ofeliaEditImage_update),
                    t_ofeliaWindow::updateMess, A_NULL, 0);
    class_addmethod(ofeliaEditImage_class, reinterpret_cast<t_method>(ofeliaEditImage_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditImage_class, reinterpret_cast<t_method>(ofeliaEditImage_colorAt),
                    gensym("colorAt"), A_GIMME, 0);
    class_addmethod(ofeliaEditImage_class, reinterpret_cast<t_method>(ofeliaEditImage_color),
                    gensym("color"), A_GIMME, 0);
    class_addmethod(ofeliaEditImage_class, reinterpret_cast<t_method>(ofeliaEditImage_grayscale),
                    gensym("grayscale"), A_NULL, 0);
    class_addmethod(ofeliaEditImage_class, reinterpret_cast<t_method>(ofeliaEditImage_invert),
                    gensym("invert"), A_NULL, 0);
    class_addmethod(ofeliaEditImage_class, reinterpret_cast<t_method>(ofeliaEditImage_brightness),
                    gensym("brightness"), A_FLOAT, 0);
    class_addmethod(ofeliaEditImage_class, reinterpret_cast<t_method>(ofeliaEditImage_contrast),
                    gensym("contrast"), A_FLOAT, 0);
    class_addmethod(ofeliaEditImage_class, reinterpret_cast<t_method>(ofeliaEditImage_gamma),
                    gensym("gamma"), A_FLOAT, 0);
    class_addmethod(ofeliaEditImage_class, reinterpret_cast<t_method>(ofeliaEditImage_type),
                    gensym("type"), A_GIMME, 0);
    class_addmethod(ofeliaEditImage_class, reinterpret_cast<t_method>(ofeliaEditImage_resize),
                    gensym("resize"), A_FLOAT, A_FLOAT, 0);
    class_addmethod(ofeliaEditImage_class, reinterpret_cast<t_method>(ofeliaEditImage_crop),
                    gensym("crop"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, 0);
    class_addmethod(ofeliaEditImage_class, reinterpret_cast<t_method>(ofeliaEditImage_mirror),
                    gensym("mirror"), A_GIMME, 0);
    class_addmethod(ofeliaEditImage_class, reinterpret_cast<t_method>(ofeliaEditImage_generateMipmap),
                    gensym("generateMipmap"), A_NULL, 0);
    class_addmethod(ofeliaEditImage_class, reinterpret_cast<t_method>(ofeliaEditImage_minMagFilter),
                    gensym("minMagFilter"), A_GIMME, 0);
    class_addmethod(ofeliaEditImage_class, reinterpret_cast<t_method>(ofeliaEditImage_texWrap),
                    gensym("texWrap"), A_GIMME, 0);
    class_addmethod(ofeliaEditImage_class, reinterpret_cast<t_method>(ofeliaEditImage_clone),
                    gensym("clone"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditImage_class, reinterpret_cast<t_method>(ofeliaEditImage_alphaMask),
                    gensym("alphaMask"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditImage_class, reinterpret_cast<t_method>(ofeliaEditImage_grabScreen),
                    gensym("grabScreen"), A_FLOAT, A_FLOAT, A_FLOAT, A_FLOAT, 0);
    class_addmethod(ofeliaEditImage_class, reinterpret_cast<t_method>(ofeliaEditImage_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditImage_class, reinterpret_cast<t_method>(ofeliaEditImage_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofSaveImage object methods
 */
void ofeliaSaveImage_jobDoneOut(t_ofeliaSaveImage *x);

void *ofeliaSaveImage_new(t_symbol *s)
{
    t_ofeliaSaveImage *x = reinterpret_cast<t_ofeliaSaveImage*>(pd_new(ofeliaSaveImage_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    x->jobDoneOutClock = clock_new(x, reinterpret_cast<t_method>(ofeliaSaveImage_jobDoneOut));
    x->saver = make_unique<ofeliaImageSaver>();
    x->saver->setJobDoneOutClock(x->jobDoneOutClock);
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaSaveImage_float(t_ofeliaSaveImage *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaSaveImage_save(t_ofeliaSaveImage *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        bool isLoadedImage;
        const int pos = getPositionByImageName(name, isLoadedImage);
        
        if (pos != -1) {
            
            ofImage *img = nullptr;
            
            if (!isLoadedImage) {
                
                img = t_ofeliaCreateImage::images[pos].get();
            }
            else {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadImage::imageData[pos].paths.size())-1);
                
                if (!t_ofeliaLoadImage::imageData[pos].paths.empty()) {
                    
                    if (!t_ofeliaLoadImage::images[pos].empty() &&
                        index < static_cast<int>(t_ofeliaLoadImage::images[pos].size())) {
                        
                        img = t_ofeliaLoadImage::images[pos][index].get();
                    }
                    else {
                        
                        error("%s: '%s' is not loaded", t_ofeliaSaveImage::objName, name.c_str());
                        return;
                    }
                }
                else {
                    
                    error("%s: '%s' is empty", t_ofeliaSaveImage::objName, name.c_str());
                    return;
                }
            }
            if (img->isAllocated()) {
                
                t_string path;
                ofImageQualityType quality;
                
                switch (argc) {
                        
                    case 1:
                    {
                        if (argv[0].a_type == A_SYMBOL) {
                            
                            path = argv[0].a_w.w_symbol->s_name;
                            quality = OF_IMAGE_QUALITY_BEST;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaSaveImage::objName);
                            return;
                        }
                        break;
                    }
                    case 2:
                    {
                        if (argv[0].a_type == A_SYMBOL &&
                            argv[1].a_type == A_FLOAT) {
                            
                            path = argv[0].a_w.w_symbol->s_name;
                            const int mode = static_cast<int>(argv[1].a_w.w_float);
                            
                            if (mode < 0 || mode > 4) {
                                
                                error("%s: quality out of range", t_ofeliaSaveImage::objName);
                                return;
                            }
                            quality = static_cast<ofImageQualityType>(mode);
                        }
                        else if (argv[0].a_type == A_SYMBOL &&
                                 argv[1].a_type == A_SYMBOL) {
                            
                            path = argv[0].a_w.w_symbol->s_name;
                            const char *mode = argv[1].a_w.w_symbol->s_name;
                            
                            if (!strcmp(mode, "BEST"))
                                quality = OF_IMAGE_QUALITY_BEST;
                            else if (!strcmp(mode, "HIGH"))
                                quality = OF_IMAGE_QUALITY_HIGH;
                            else if (!strcmp(mode, "MIDIUM"))
                                quality = OF_IMAGE_QUALITY_MEDIUM;
                            else if (!strcmp(mode, "LOW"))
                                quality = OF_IMAGE_QUALITY_LOW;
                            else if (!strcmp(mode, "WORST"))
                                quality = OF_IMAGE_QUALITY_WORST;
                            else {
                                
                                error("%s: no method for '%s'", t_ofeliaSaveImage::objName, mode);
                                return;
                            }
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaSaveImage::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        error("%s: invalid number of arguments", t_ofeliaSaveImage::objName);
                        return;
                    }
                }
                x->saver->saveImage(*img, path, quality);
            }
            else {
                
                error("%s: '%s' is not allocated", t_ofeliaSaveImage::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaSaveImage::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaSaveImage::objName);
    }
}

void ofeliaSaveImage_set(t_ofeliaSaveImage *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaSaveImage_print(t_ofeliaSaveImage *x)
{
    post("\n[%s]", t_ofeliaSaveImage::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaSaveImage_jobDoneOut(t_ofeliaSaveImage *x)
{
    outlet_bang(x->x_obj.ob_outlet);
}

void ofeliaSaveImage_free(t_ofeliaSaveImage *x)
{
    clock_free(x->jobDoneOutClock);
    x->saver.reset();
}

void ofeliaSaveImage_setup()
{
    ofeliaSaveImage_class = class_new(gensym("ofSaveImage"),
                                      reinterpret_cast<t_newmethod>(ofeliaSaveImage_new),
                                      reinterpret_cast<t_method>(ofeliaSaveImage_free),
                                      sizeof(t_ofeliaSaveImage),
                                      CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaSaveImage_class, reinterpret_cast<t_method>(ofeliaSaveImage_float));
    class_addmethod(ofeliaSaveImage_class, reinterpret_cast<t_method>(ofeliaSaveImage_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaSaveImage_class, reinterpret_cast<t_method>(ofeliaSaveImage_save),
                    gensym("save"), A_GIMME, 0);
    class_addmethod(ofeliaSaveImage_class, reinterpret_cast<t_method>(ofeliaSaveImage_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaSaveImage_class, reinterpret_cast<t_method>(ofeliaSaveImage_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofBindImageTex object methods
 */
void *ofeliaBindImageTex_new(t_symbol *s)
{
    t_ofeliaBindImageTex *x = reinterpret_cast<t_ofeliaBindImageTex*>(pd_new(ofeliaBindImageTex_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaBindImageTex_bang(t_ofeliaBindImageTex *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const t_string &name = x->varName.name;
        
        if (!name.empty()) {
            
            bool isLoadedImage;
            const int pos = getPositionByImageName(name, isLoadedImage);

            if (pos != -1) {
                
                ofImage *img = nullptr;
                
                if (!isLoadedImage) {
                    
                    img = t_ofeliaCreateImage::images[pos].get();
                }
                else {
                    
                    const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadImage::imageData[pos].paths.size())-1);
                    
                    if (!t_ofeliaLoadImage::imageData[pos].paths.empty() &&
                        !t_ofeliaLoadImage::images[pos].empty() &&
                        index < static_cast<int>(t_ofeliaLoadImage::images[pos].size())) {
                        
                        img = t_ofeliaLoadImage::images[pos][index].get();
                    }
                }
                if (img && img->isAllocated()) {
                    
                    img->bind();
                    outlet_bang(x->x_obj.ob_outlet);
                    img->unbind();
                    return;
                }
            }
        }
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaBindImageTex::objName);
    }
}

void ofeliaBindImageTex_float(t_ofeliaBindImageTex *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaBindImageTex_set(t_ofeliaBindImageTex *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaBindImageTex_print(t_ofeliaBindImageTex *x)
{
    post("\n[%s]", t_ofeliaBindImageTex::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaBindImageTex_setup()
{
    ofeliaBindImageTex_class = class_new(gensym("ofBindImageTex"),
                                         reinterpret_cast<t_newmethod>(ofeliaBindImageTex_new),
                                         0, sizeof(t_ofeliaBindImageTex),
                                         CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaBindImageTex_class, reinterpret_cast<t_method>(ofeliaBindImageTex_bang));
    class_addfloat(ofeliaBindImageTex_class, reinterpret_cast<t_method>(ofeliaBindImageTex_float));
    class_addmethod(ofeliaBindImageTex_class, reinterpret_cast<t_method>(ofeliaBindImageTex_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaBindImageTex_class, reinterpret_cast<t_method>(ofeliaBindImageTex_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaBindImageTex_class, reinterpret_cast<t_method>(ofeliaBindImageTex_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofDrawImage object methods
 */
void *ofeliaDrawImage_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaDrawImage *x = reinterpret_cast<t_ofeliaDrawImage*>(pd_new(ofeliaDrawImage_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    getVarNameLocalPrefixes(x->elem.varName);
    
    if (!getNameDimen2dElemFromArgs(argc, argv, x->elem, t_ofeliaDrawImage::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("width"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("height"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaDrawImage_bang(t_ofeliaDrawImage *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const t_string &name = x->elem.varName.name;
        
        if (!name.empty()) {
            
            bool isLoadedImage;
            const int pos = getPositionByImageName(name, isLoadedImage);
            
            if (pos != -1) {
                
                ofImage *img = nullptr;
                
                if (!isLoadedImage) {
                    
                    img = t_ofeliaCreateImage::images[pos].get();
                }
                else {
                    
                    const int index = min(x->elem.varName.index, static_cast<int>(t_ofeliaLoadImage::imageData[pos].paths.size())-1);
                    
                    if (!t_ofeliaLoadImage::imageData[pos].paths.empty() &&
                        !t_ofeliaLoadImage::images[pos].empty() &&
                        index < static_cast<int>(t_ofeliaLoadImage::images[pos].size())) {
                        
                        img = t_ofeliaLoadImage::images[pos][index].get();
                    }
                }
                if (img && img->isAllocated()) {
                    
                    if (!x->elem.width && !x->elem.height)
                        img->draw(0.0f, 0.0f, 0.0f);
                    else
                        img->draw(0.0f, 0.0f, 0.0f, x->elem.width, x->elem.height);
                }
            }
        }
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaDrawImage::objName);
    }
}

void ofeliaDrawImage_float(t_ofeliaDrawImage *x, t_floatarg f)
{
    x->elem.varName.index = max(0, static_cast<int>(f));
}

void ofeliaDrawImage_name(t_ofeliaDrawImage *x, t_symbol *s)
{
    x->elem.varName.name = s->s_name;
    getVarNameLocalized(x->elem.varName);
    getVarNameIndexed(x->elem.varName);
}

void ofeliaDrawImage_width(t_ofeliaDrawImage *x, t_floatarg f)
{
    x->elem.width = f;
}

void ofeliaDrawImage_height(t_ofeliaDrawImage *x, t_floatarg f)
{
    x->elem.height = f;
}

void ofeliaDrawImage_set(t_ofeliaDrawImage *x, t_symbol *s, int argc, t_atom *argv)
{
    getNameDimen2dElemFromArgs(argc, argv, x->elem, t_ofeliaDrawImage::objName);
}

void ofeliaDrawImage_print(t_ofeliaDrawImage *x)
{
    post("\n[%s]", t_ofeliaDrawImage::objName);
    const t_string &name = x->elem.varName.name;
    float width, height;
    
    if (!x->elem.width && !x->elem.height) {
        
        if (!name.empty()) {
            
            bool isLoadedImage;
            const int pos = getPositionByImageName(name, isLoadedImage);
            
            if (pos != -1) {
                
                ofImage *img = nullptr;
                
                if (!isLoadedImage) {
                    
                    img = t_ofeliaCreateImage::images[pos].get();
                }
                else {
                    
                    const int index = min(x->elem.varName.index, static_cast<int>(t_ofeliaLoadImage::imageData[pos].paths.size())-1);
                    
                    if (!t_ofeliaLoadImage::imageData[pos].paths.empty() &&
                        !t_ofeliaLoadImage::images[pos].empty() &&
                        index < static_cast<int>(t_ofeliaLoadImage::images[pos].size())) {
                        
                        img = t_ofeliaLoadImage::images[pos][index].get();
                    }
                }
                if (img && img->isAllocated()) {
                    
                    width = img->getWidth();
                    height = img->getHeight();
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
    post("name : %s[%d]", name.c_str(), x->elem.varName.index);
    post("width : %g", width);
    post("height : %g", height);
}

void ofeliaDrawImage_setup()
{
    ofeliaDrawImage_class = class_new(gensym("ofDrawImage"),
                                      reinterpret_cast<t_newmethod>(ofeliaDrawImage_new),
                                      0, sizeof(t_ofeliaDrawImage),
                                      CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaDrawImage_class, reinterpret_cast<t_method>(ofeliaDrawImage_bang));
    class_addfloat(ofeliaDrawImage_class, reinterpret_cast<t_method>(ofeliaDrawImage_float));
    class_addmethod(ofeliaDrawImage_class, reinterpret_cast<t_method>(ofeliaDrawImage_name),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaDrawImage_class, reinterpret_cast<t_method>(ofeliaDrawImage_width),
                    gensym("width"), A_FLOAT, 0);
    class_addmethod(ofeliaDrawImage_class, reinterpret_cast<t_method>(ofeliaDrawImage_height),
                    gensym("height"), A_FLOAT, 0);
    class_addmethod(ofeliaDrawImage_class, reinterpret_cast<t_method>(ofeliaDrawImage_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaDrawImage_class, reinterpret_cast<t_method>(ofeliaDrawImage_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofDrawSubImage object methods
 */
bool getDrawSubImageElemFromArgs(const int argc, const t_atom *argv, t_ofeliaDrawSubImageElem &elem)
{
    switch (argc) {
            
        case 0:
        {
            elem.varName.name = "";
            elem.width = 0.0f;
            elem.height = 0.0f;
            elem.subX = 0.0f;
            elem.subY = 0.0f;
            elem.subWidth = 0.0f;
            elem.subHeight = 0.0f;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_SYMBOL) {
                
                elem.varName.name = argv[0].a_w.w_symbol->s_name;
                getVarNameLocalized(elem.varName);
                getVarNameIndexed(elem.varName);
                elem.width = 0.0f;
                elem.height = 0.0f;
                elem.subX = 0.0f;
                elem.subY = 0.0f;
                elem.subWidth = 0.0f;
                elem.subHeight = 0.0f;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaDrawSubImage::objName);
                return 0;
            }
            break;
        }
        case 3:
        {
            if (argv[0].a_type == A_SYMBOL &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT) {
                
                elem.varName.name = argv[0].a_w.w_symbol->s_name;
                getVarNameLocalized(elem.varName);
                getVarNameIndexed(elem.varName);
                elem.width = argv[1].a_w.w_float;
                elem.height = argv[2].a_w.w_float;
                elem.subX = 0.0f;
                elem.subY = 0.0f;
                elem.subWidth = 0.0f;
                elem.subHeight = 0.0f;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaDrawSubImage::objName);
                return 0;
            }
            break;
        }
        case 5:
        {
            if (argv[0].a_type == A_SYMBOL &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT &&
                argv[4].a_type == A_FLOAT) {
                
                elem.varName.name = argv[0].a_w.w_symbol->s_name;
                getVarNameLocalized(elem.varName);
                getVarNameIndexed(elem.varName);
                elem.width = argv[1].a_w.w_float;
                elem.height = argv[2].a_w.w_float;
                elem.subX = argv[3].a_w.w_float;
                elem.subY = argv[4].a_w.w_float;
                elem.subWidth = 0.0f;
                elem.subHeight = 0.0f;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaDrawSubImage::objName);
                return 0;
            }
            break;
        }
        case 7:
        {
            if (argv[0].a_type == A_SYMBOL &&
                argv[1].a_type == A_FLOAT &&
                argv[2].a_type == A_FLOAT &&
                argv[3].a_type == A_FLOAT &&
                argv[4].a_type == A_FLOAT &&
                argv[5].a_type == A_FLOAT &&
                argv[6].a_type == A_FLOAT) {
                
                elem.varName.name = argv[0].a_w.w_symbol->s_name;
                getVarNameLocalized(elem.varName);
                getVarNameIndexed(elem.varName);
                elem.width = argv[1].a_w.w_float;
                elem.height = argv[2].a_w.w_float;
                elem.subX = argv[3].a_w.w_float;
                elem.subY = argv[4].a_w.w_float;
                elem.subWidth = argv[5].a_w.w_float;
                elem.subHeight = argv[6].a_w.w_float;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaDrawSubImage::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaDrawSubImage::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaDrawSubImage_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaDrawSubImage *x = reinterpret_cast<t_ofeliaDrawSubImage*>(pd_new(ofeliaDrawSubImage_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    getVarNameLocalPrefixes(x->elem.varName);
    
    if (!getDrawSubImageElemFromArgs(argc, argv, x->elem))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("width"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("height"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("subX"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("subY"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("subWidth"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("subHeight"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaDrawSubImage_bang(t_ofeliaDrawSubImage *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const t_string &name = x->elem.varName.name;
        
        if (!name.empty()) {
            
            bool isLoadedImage;
            const int pos = getPositionByImageName(name, isLoadedImage);
            
            if (pos != -1) {
                
                ofImage *img = nullptr;
                
                if (!isLoadedImage) {
                    
                    img = t_ofeliaCreateImage::images[pos].get();
                }
                else {
                    
                    const int index = min(x->elem.varName.index, static_cast<int>(t_ofeliaLoadImage::imageData[pos].paths.size())-1);
                    
                    if (!t_ofeliaLoadImage::imageData[pos].paths.empty() &&
                        !t_ofeliaLoadImage::images[pos].empty() &&
                        index < static_cast<int>(t_ofeliaLoadImage::images[pos].size())) {
                        
                        img = t_ofeliaLoadImage::images[pos][index].get();
                    }
                }
                if (img && img->isAllocated()) {
                 
                    float width, height, subWidth, subHeight;
                    
                    if (!x->elem.width && !x->elem.height) {
                        
                        width = img->getWidth();
                        height = img->getHeight();
                    }
                    else {
                        
                        width = x->elem.width;
                        height = x->elem.height;
                    }
                    if (!x->elem.subWidth && !x->elem.subWidth) {
                        
                        subWidth = img->getWidth();
                        subHeight = img->getHeight();
                    }
                    else {
                        
                        subWidth = x->elem.subWidth;
                        subHeight = x->elem.subHeight;
                    }
                    img->drawSubsection(0.0f, 0.0f, 0.0f, width, height, x->elem.subX, x->elem.subY, subWidth, subHeight);
                }
            }
        }
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaDrawSubImage::objName);
    }
}

void ofeliaDrawSubImage_float(t_ofeliaDrawSubImage *x, t_floatarg f)
{
    x->elem.varName.index = max(0, static_cast<int>(f));
}

void ofeliaDrawSubImage_name(t_ofeliaDrawSubImage *x, t_symbol *s)
{
    x->elem.varName.name = s->s_name;
    getVarNameLocalized(x->elem.varName);
    getVarNameIndexed(x->elem.varName);
}

void ofeliaDrawSubImage_width(t_ofeliaDrawSubImage *x, t_floatarg f)
{
    x->elem.width = f;
}

void ofeliaDrawSubImage_height(t_ofeliaDrawSubImage *x, t_floatarg f)
{
    x->elem.height = f;
}

void ofeliaDrawSubImage_subX(t_ofeliaDrawSubImage *x, t_floatarg f)
{
    x->elem.subX = f;
}

void ofeliaDrawSubImage_subY(t_ofeliaDrawSubImage *x, t_floatarg f)
{
    x->elem.subY = f;
}

void ofeliaDrawSubImage_subWidth(t_ofeliaDrawSubImage *x, t_floatarg f)
{
    x->elem.subWidth = f;
}

void ofeliaDrawSubImage_subHeight(t_ofeliaDrawSubImage *x, t_floatarg f)
{
    x->elem.subHeight = f;
}

void ofeliaDrawSubImage_set(t_ofeliaDrawSubImage *x, t_symbol *s, int argc, t_atom *argv)
{
    getDrawSubImageElemFromArgs(argc, argv, x->elem);
}

void ofeliaDrawSubImage_print(t_ofeliaDrawSubImage *x)
{
    post("\n[%s]", t_ofeliaDrawSubImage::objName);
    const t_string &name = x->elem.varName.name;
    float width, height, subWidth, subHeight;
    float tmpWidth, tmpHeight;
    
    if (!name.empty()) {
        
        bool isLoadedImage;
        const int pos = getPositionByImageName(name, isLoadedImage);
        
        if (pos != -1) {
            
            ofImage *img = nullptr;
            
            if (!isLoadedImage) {
                
                img = t_ofeliaCreateImage::images[pos].get();
            }
            else {
                
                const int index = min(x->elem.varName.index, static_cast<int>(t_ofeliaLoadImage::imageData[pos].paths.size())-1);
                
                if (!t_ofeliaLoadImage::imageData[pos].paths.empty() &&
                    !t_ofeliaLoadImage::images[pos].empty() &&
                    index < static_cast<int>(t_ofeliaLoadImage::images[pos].size())) {
                    
                    img = t_ofeliaLoadImage::images[pos][index].get();
                }
            }
            if (img && img->isAllocated()) {
                
                tmpWidth = img->getWidth();
                tmpHeight = img->getHeight();
            }
            else {
                
                tmpWidth = 0.0f;
                tmpHeight = 0.0f;
            }
        }
        else {
            
            tmpWidth = 0.0f;
            tmpHeight = 0.0f;
        }
    }
    else {
        
        tmpWidth = 0.0f;
        tmpHeight = 0.0f;
    }
    if (!x->elem.width && !x->elem.height) {
        
        width = tmpWidth;
        height = tmpHeight;
    }
    else {
        
        width = x->elem.width;
        height = x->elem.height;
    }
    if (!x->elem.subWidth && !x->elem.subHeight) {
        
        subWidth = tmpWidth;
        subHeight = tmpHeight;
    }
    else {
        
        subWidth = x->elem.subWidth;
        subHeight = x->elem.subHeight;
    }
    post("name : %s[%d]", name.c_str(), x->elem.varName.index);
    post("width : %g", width);
    post("height : %g", height);
    post("subX : %g", x->elem.subX);
    post("subY : %g", x->elem.subY);
    post("subWidth : %g", subWidth);
    post("subHeight : %g", subHeight);
}

void ofeliaDrawSubImage_setup()
{
    ofeliaDrawSubImage_class = class_new(gensym("ofDrawSubImage"),
                                         reinterpret_cast<t_newmethod>(ofeliaDrawSubImage_new),
                                         0, sizeof(t_ofeliaDrawSubImage),
                                         CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaDrawSubImage_class, reinterpret_cast<t_method>(ofeliaDrawSubImage_bang));
    class_addfloat(ofeliaDrawSubImage_class, reinterpret_cast<t_method>(ofeliaDrawSubImage_float));
    class_addmethod(ofeliaDrawSubImage_class, reinterpret_cast<t_method>(ofeliaDrawSubImage_name),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaDrawSubImage_class, reinterpret_cast<t_method>(ofeliaDrawSubImage_width),
                    gensym("width"), A_FLOAT, 0);
    class_addmethod(ofeliaDrawSubImage_class, reinterpret_cast<t_method>(ofeliaDrawSubImage_height),
                    gensym("height"), A_FLOAT, 0);
    class_addmethod(ofeliaDrawSubImage_class, reinterpret_cast<t_method>(ofeliaDrawSubImage_subX),
                    gensym("subX"), A_FLOAT, 0);
    class_addmethod(ofeliaDrawSubImage_class, reinterpret_cast<t_method>(ofeliaDrawSubImage_subY),
                    gensym("subY"), A_FLOAT, 0);
    class_addmethod(ofeliaDrawSubImage_class, reinterpret_cast<t_method>(ofeliaDrawSubImage_subWidth),
                    gensym("subWidth"), A_FLOAT, 0);
    class_addmethod(ofeliaDrawSubImage_class, reinterpret_cast<t_method>(ofeliaDrawSubImage_subHeight),
                    gensym("subHeight"), A_FLOAT, 0);
    class_addmethod(ofeliaDrawSubImage_class, reinterpret_cast<t_method>(ofeliaDrawSubImage_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaDrawSubImage_class, reinterpret_cast<t_method>(ofeliaDrawSubImage_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofDoesImageNameExist object methods
 */
void *ofeliaDoesImageNameExist_new(t_symbol *s)
{
    t_ofeliaDoesImageNameExist *x = reinterpret_cast<t_ofeliaDoesImageNameExist*>(pd_new(ofeliaDoesImageNameExist_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaDoesImageNameExist_bang(t_ofeliaDoesImageNameExist *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        bool isLoadedImage;
        const int pos = getPositionByImageName(name, isLoadedImage);
        
        if (pos != -1)
            outlet_float(x->x_obj.ob_outlet, 1.0f);
        else
            outlet_float(x->x_obj.ob_outlet, 0.0f);
    }
    else {
        
        error("%s: name not assigned", t_ofeliaDoesImageNameExist::objName);
    }
}

void ofeliaDoesImageNameExist_set(t_ofeliaDoesImageNameExist *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaDoesImageNameExist_print(t_ofeliaDoesImageNameExist *x)
{
    post("\n[%s]", t_ofeliaDoesImageNameExist::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaDoesImageNameExist_setup()
{
    ofeliaDoesImageNameExist_class = class_new(gensym("ofDoesImageNameExist"),
                                              reinterpret_cast<t_newmethod>(ofeliaDoesImageNameExist_new),
                                              0, sizeof(t_ofeliaDoesImageNameExist),
                                              CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaDoesImageNameExist_class, reinterpret_cast<t_method>(ofeliaDoesImageNameExist_bang));
    class_addmethod(ofeliaDoesImageNameExist_class, reinterpret_cast<t_method>(ofeliaDoesImageNameExist_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesImageNameExist_class, reinterpret_cast<t_method>(ofeliaDoesImageNameExist_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesImageNameExist_class, reinterpret_cast<t_method>(ofeliaDoesImageNameExist_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetImagePath object methods
 */
void *ofeliaGetImagePath_new(t_symbol *s)
{
    t_ofeliaGetImagePath *x = reinterpret_cast<t_ofeliaGetImagePath*>(pd_new(ofeliaGetImagePath_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_symbol);
    return (x);
}

void ofeliaGetImagePath_bang(t_ofeliaGetImagePath *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        bool isLoadedImage;
        const int pos = getPositionByImageName(name, isLoadedImage);
        
        if (pos != -1) {
            
            if (isLoadedImage) {
                
                if (!t_ofeliaLoadImage::imageData[pos].paths.empty()) {
                    
                    const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadImage::imageData[pos].paths.size())-1);
                    outlet_symbol(x->x_obj.ob_outlet, gensym(t_ofeliaLoadImage::imageData[pos].paths[index].c_str()));
                }
                else {
                    
                    error("%s: '%s' is empty", t_ofeliaGetImagePath::objName, name.c_str());
                }
            }
            else {
                
                error("%s: '%s' is not loaded", t_ofeliaGetImagePath::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetImagePath::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetImagePath::objName);
    }
}

void ofeliaGetImagePath_float(t_ofeliaGetImagePath *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetImagePath_set(t_ofeliaGetImagePath *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetImagePath_print(t_ofeliaGetImagePath *x)
{
    post("\n[%s]", t_ofeliaGetImagePath::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetImagePath_setup()
{
    ofeliaGetImagePath_class = class_new(gensym("ofGetImagePath"),
                                         reinterpret_cast<t_newmethod>(ofeliaGetImagePath_new),
                                         0, sizeof(t_ofeliaGetImagePath),
                                         CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetImagePath_class, reinterpret_cast<t_method>(ofeliaGetImagePath_bang));
    class_addfloat(ofeliaGetImagePath_class, reinterpret_cast<t_method>(ofeliaGetImagePath_float));
    class_addmethod(ofeliaGetImagePath_class, reinterpret_cast<t_method>(ofeliaGetImagePath_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetImagePath_class, reinterpret_cast<t_method>(ofeliaGetImagePath_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetImagePath_class, reinterpret_cast<t_method>(ofeliaGetImagePath_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofIsImageAllocated object methods
 */
void *ofeliaIsImageAllocated_new(t_symbol *s)
{
    t_ofeliaIsImageAllocated *x = reinterpret_cast<t_ofeliaIsImageAllocated*>(pd_new(ofeliaIsImageAllocated_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaIsImageAllocated_bang(t_ofeliaIsImageAllocated *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        bool isLoadedImage;
        const int pos = getPositionByImageName(name, isLoadedImage);
        
        if (pos != -1) {
            
            ofImage *img = nullptr;
            
            if (!isLoadedImage) {
                
                img = t_ofeliaCreateImage::images[pos].get();
            }
            else {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadImage::imageData[pos].paths.size())-1);
                
                if (!t_ofeliaLoadImage::imageData[pos].paths.empty() &&
                    !t_ofeliaLoadImage::images[pos].empty() &&
                    index < static_cast<int>(t_ofeliaLoadImage::images[pos].size())) {
                    
                    img = t_ofeliaLoadImage::images[pos][index].get();
                }
            }
            if (img && img->isAllocated())
                outlet_float(x->x_obj.ob_outlet, 1.0f);
            else
                outlet_float(x->x_obj.ob_outlet, 0.0f);
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaIsImageAllocated::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaIsImageAllocated::objName);
    }
}

void ofeliaIsImageAllocated_float(t_ofeliaIsImageAllocated *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaIsImageAllocated_set(t_ofeliaIsImageAllocated *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaIsImageAllocated_print(t_ofeliaIsImageAllocated *x)
{
    post("\n[%s]", t_ofeliaIsImageAllocated::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaIsImageAllocated_setup()
{
    ofeliaIsImageAllocated_class = class_new(gensym("ofIsImageAllocated"),
                                           reinterpret_cast<t_newmethod>(ofeliaIsImageAllocated_new),
                                           0, sizeof(t_ofeliaIsImageAllocated),
                                           CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaIsImageAllocated_class, reinterpret_cast<t_method>(ofeliaIsImageAllocated_bang));
    class_addfloat(ofeliaIsImageAllocated_class, reinterpret_cast<t_method>(ofeliaIsImageAllocated_float));
    class_addmethod(ofeliaIsImageAllocated_class, reinterpret_cast<t_method>(ofeliaIsImageAllocated_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaIsImageAllocated_class, reinterpret_cast<t_method>(ofeliaIsImageAllocated_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaIsImageAllocated_class, reinterpret_cast<t_method>(ofeliaIsImageAllocated_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetImageDimen object methods
 */
void *ofeliaGetImageDimen_new(t_symbol *s)
{
    t_ofeliaGetImageDimen *x = reinterpret_cast<t_ofeliaGetImageDimen*>(pd_new(ofeliaGetImageDimen_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetImageDimen_bang(t_ofeliaGetImageDimen *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        bool isLoadedImage;
        const int pos = getPositionByImageName(name, isLoadedImage);
        
        if (pos != -1) {
            
            ofImage *img = nullptr;
            
            if (!isLoadedImage) {
                
                img = t_ofeliaCreateImage::images[pos].get();
            }
            else {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadImage::imageData[pos].paths.size())-1);
                
                if (!t_ofeliaLoadImage::imageData[pos].paths.empty()) {
                    
                    if (!t_ofeliaLoadImage::images[pos].empty() &&
                        index < static_cast<int>(t_ofeliaLoadImage::images[pos].size())) {
                        
                        img = t_ofeliaLoadImage::images[pos][index].get();
                    }
                    else {
                        
                        error("%s: '%s' is not loaded", t_ofeliaGetImageDimen::objName, name.c_str());
                        return;
                    }
                }
                else {
                    
                    error("%s: '%s' is empty", t_ofeliaGetImageDimen::objName, name.c_str());
                    return;
                }
            }
            if (img->isAllocated()) {
                
                t_atom av[2];
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = img->getWidth();
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = img->getHeight();
                outlet_list(x->x_obj.ob_outlet, &s_list, 2, av);
            }
            else {
                
                error("%s: '%s' is not allocated", t_ofeliaGetImageDimen::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetImageDimen::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetImageDimen::objName);
    }
}

void ofeliaGetImageDimen_float(t_ofeliaGetImageDimen *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetImageDimen_set(t_ofeliaGetImageDimen *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetImageDimen_print(t_ofeliaGetImageDimen *x)
{
    post("\n[%s]", t_ofeliaGetImageDimen::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetImageDimen_setup()
{
    ofeliaGetImageDimen_class = class_new(gensym("ofGetImageDimen"),
                                          reinterpret_cast<t_newmethod>(ofeliaGetImageDimen_new),
                                          0, sizeof(t_ofeliaGetImageDimen),
                                          CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetImageDimen_class, reinterpret_cast<t_method>(ofeliaGetImageDimen_bang));
    class_addfloat(ofeliaGetImageDimen_class, reinterpret_cast<t_method>(ofeliaGetImageDimen_float));
    class_addmethod(ofeliaGetImageDimen_class, reinterpret_cast<t_method>(ofeliaGetImageDimen_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetImageDimen_class, reinterpret_cast<t_method>(ofeliaGetImageDimen_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetImageDimen_class, reinterpret_cast<t_method>(ofeliaGetImageDimen_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetImageType object methods
 */
void *ofeliaGetImageType_new(t_symbol *s)
{
    t_ofeliaGetImageType *x = reinterpret_cast<t_ofeliaGetImageType*>(pd_new(ofeliaGetImageType_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetImageType_bang(t_ofeliaGetImageType *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        bool isLoadedImage;
        const int pos = getPositionByImageName(name, isLoadedImage);

        if (pos != -1) {
            
            ofImage *img = nullptr;
            
            if (!isLoadedImage) {
                
                img = t_ofeliaCreateImage::images[pos].get();
            }
            else {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadImage::imageData[pos].paths.size())-1);
                
                if (!t_ofeliaLoadImage::imageData[pos].paths.empty()) {
                    
                    if (!t_ofeliaLoadImage::images[pos].empty() &&
                        index < static_cast<int>(t_ofeliaLoadImage::images[pos].size())) {
                        
                        img = t_ofeliaLoadImage::images[pos][index].get();
                    }
                    else {
                        
                        error("%s: '%s' is not loaded", t_ofeliaGetImageType::objName, name.c_str());
                        return;
                    }
                }
                else {
                    
                    error("%s: '%s' is empty", t_ofeliaGetImageType::objName, name.c_str());
                    return;
                }
            }
            if (img->isAllocated()) {
             
                switch (img->getImageType()) {
                        
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
                
                error("%s: '%s' is not allocated", t_ofeliaGetImageType::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetImageType::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetImageType::objName);
    }
}

void ofeliaGetImageType_float(t_ofeliaGetImageType *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetImageType_set(t_ofeliaGetImageType *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetImageType_print(t_ofeliaGetImageType *x)
{
    post("\n[%s]", t_ofeliaGetImageType::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetImageType_setup()
{
    ofeliaGetImageType_class = class_new(gensym("ofGetImageType"),
                                         reinterpret_cast<t_newmethod>(ofeliaGetImageType_new),
                                         0, sizeof(t_ofeliaGetImageType),
                                         CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetImageType_class, reinterpret_cast<t_method>(ofeliaGetImageType_bang));
    class_addfloat(ofeliaGetImageType_class, reinterpret_cast<t_method>(ofeliaGetImageType_float));
    class_addmethod(ofeliaGetImageType_class, reinterpret_cast<t_method>(ofeliaGetImageType_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetImageType_class, reinterpret_cast<t_method>(ofeliaGetImageType_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetImageType_class, reinterpret_cast<t_method>(ofeliaGetImageType_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetImageColorAt object methods
 */
void *ofeliaGetImageColorAt_new(t_symbol *s)
{
    t_ofeliaGetImageColorAt *x = reinterpret_cast<t_ofeliaGetImageColorAt*>(pd_new(ofeliaGetImageColorAt_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetImageColorAt_float(t_ofeliaGetImageColorAt *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetImageColorAt_list(t_ofeliaGetImageColorAt *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        bool isLoadedImage;
        const int pos = getPositionByImageName(name, isLoadedImage);
        
        if (pos != -1) {
            
            ofImage *img = nullptr;
            
            if (!isLoadedImage) {
                
                img = t_ofeliaCreateImage::images[pos].get();
            }
            else {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadImage::imageData[pos].paths.size())-1);
                
                if (!t_ofeliaLoadImage::imageData[pos].paths.empty()) {
                    
                    if (!t_ofeliaLoadImage::images[pos].empty() &&
                        index < static_cast<int>(t_ofeliaLoadImage::images[pos].size())) {
                        
                        img = t_ofeliaLoadImage::images[pos][index].get();
                    }
                    else {
                        
                        error("%s: '%s' is not loaded", t_ofeliaGetImageColorAt::objName, name.c_str());
                        return;
                    }
                }
                else {
                    
                    error("%s: '%s' is empty", t_ofeliaGetImageColorAt::objName, name.c_str());
                    return;
                }
            }
            if (img->isAllocated()) {
                
                if (argc >= 2) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT) {
                        
                        const int posX = getClampIntValue(static_cast<int>(argv[0].a_w.w_float), 0, img->getWidth()-1);
                        const int posY = getClampIntValue(static_cast<int>(argv[1].a_w.w_float), 0, img->getHeight()-1);
                        const ofColor &color = img->getColor(posX, posY);
                        t_atom av[4];
                        av[0].a_type = A_FLOAT;
                        av[0].a_w.w_float = static_cast<t_float>(color.r);
                        av[1].a_type = A_FLOAT;
                        av[1].a_w.w_float = static_cast<t_float>(color.g);
                        av[2].a_type = A_FLOAT;
                        av[2].a_w.w_float = static_cast<t_float>(color.b);
                        av[3].a_type = A_FLOAT;
                        av[3].a_w.w_float = static_cast<t_float>(color.a);
                        outlet_list(x->x_obj.ob_outlet, &s_list, 4, av);
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaGetImageColorAt::objName);
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaGetImageColorAt::objName);
                }
            }
            else {
                
                error("%s: '%s' is not allocated", t_ofeliaGetImageColorAt::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetImageColorAt::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetImageColorAt::objName);
    }
}

void ofeliaGetImageColorAt_set(t_ofeliaGetImageColorAt *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetImageColorAt_print(t_ofeliaGetImageColorAt *x)
{
    post("\n[%s]", t_ofeliaGetImageColorAt::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetImageColorAt_setup()
{
    ofeliaGetImageColorAt_class = class_new(gensym("ofGetImageColorAt"),
                                            reinterpret_cast<t_newmethod>(ofeliaGetImageColorAt_new),
                                            0, sizeof(t_ofeliaGetImageColorAt),
                                            CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaGetImageColorAt_class, reinterpret_cast<t_method>(ofeliaGetImageColorAt_float));
    class_addlist(ofeliaGetImageColorAt_class, reinterpret_cast<t_method>(ofeliaGetImageColorAt_list));
    class_addmethod(ofeliaGetImageColorAt_class, reinterpret_cast<t_method>(ofeliaGetImageColorAt_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetImageColorAt_class, reinterpret_cast<t_method>(ofeliaGetImageColorAt_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetImageColorAt_class, reinterpret_cast<t_method>(ofeliaGetImageColorAt_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetImageTexCoord object methods
 */
void *ofeliaGetImageTexCoord_new(t_symbol *s)
{
    t_ofeliaGetImageTexCoord *x = reinterpret_cast<t_ofeliaGetImageTexCoord*>(pd_new(ofeliaGetImageTexCoord_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetImageTexCoord_float(t_ofeliaGetImageTexCoord *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetImageTexCoord_list(t_ofeliaGetImageTexCoord *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        bool isLoadedImage;
        const int pos = getPositionByImageName(name, isLoadedImage);
        
        if (pos != -1) {
            
            ofImage *img = nullptr;
            
            if (!isLoadedImage) {
                
                img = t_ofeliaCreateImage::images[pos].get();
            }
            else {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadImage::imageData[pos].paths.size())-1);
                
                if (!t_ofeliaLoadImage::imageData[pos].paths.empty()) {
                    
                    if (!t_ofeliaLoadImage::images[pos].empty() &&
                        index < static_cast<int>(t_ofeliaLoadImage::images[pos].size())) {
                        
                        img = t_ofeliaLoadImage::images[pos][index].get();
                    }
                    else {
                        
                        error("%s: '%s' is not loaded", t_ofeliaGetImageTexCoord::objName, name.c_str());
                        return;
                    }
                }
                else {
                    
                    error("%s: '%s' is empty", t_ofeliaGetImageTexCoord::objName, name.c_str());
                    return;
                }
            }
            if (img->isAllocated()) {
                
                if (argc >= 2) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT) {
                        
                        const ofVec2f &texCoord = img->getTexture().getCoordFromPoint(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        t_atom av[2];
                        av[0].a_type = A_FLOAT;
                        av[0].a_w.w_float = texCoord.x;
                        av[1].a_type = A_FLOAT;
                        av[1].a_w.w_float = texCoord.y;
                        outlet_list(x->x_obj.ob_outlet, &s_list, 2, av);
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaGetImageTexCoord::objName);
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaGetImageTexCoord::objName);
                }
            }
            else {
                
                error("%s: '%s' is not allocated", t_ofeliaGetImageTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetImageTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetImageTexCoord::objName);
    }
}

void ofeliaGetImageTexCoord_set(t_ofeliaGetImageTexCoord *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetImageTexCoord_print(t_ofeliaGetImageTexCoord *x)
{
    post("\n[%s]", t_ofeliaGetImageTexCoord::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetImageTexCoord_setup()
{
    ofeliaGetImageTexCoord_class = class_new(gensym("ofGetImageTexCoord"),
                                             reinterpret_cast<t_newmethod>(ofeliaGetImageTexCoord_new),
                                             0, sizeof(t_ofeliaGetImageTexCoord),
                                             CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaGetImageTexCoord_class, reinterpret_cast<t_method>(ofeliaGetImageTexCoord_float));
    class_addlist(ofeliaGetImageTexCoord_class, reinterpret_cast<t_method>(ofeliaGetImageTexCoord_list));
    class_addmethod(ofeliaGetImageTexCoord_class, reinterpret_cast<t_method>(ofeliaGetImageTexCoord_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetImageTexCoord_class, reinterpret_cast<t_method>(ofeliaGetImageTexCoord_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetImageTexCoord_class, reinterpret_cast<t_method>(ofeliaGetImageTexCoord_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetImageTexCoords object methods
 */
void *ofeliaGetImageTexCoords_new(t_symbol *s)
{
    t_ofeliaGetImageTexCoords *x = reinterpret_cast<t_ofeliaGetImageTexCoords*>(pd_new(ofeliaGetImageTexCoords_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetImageTexCoords_float(t_ofeliaGetImageTexCoords *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetImageTexCoords_list(t_ofeliaGetImageTexCoords *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        bool isLoadedImage;
        const int pos = getPositionByImageName(name, isLoadedImage);

        if (pos != -1) {
            
            ofImage *img = nullptr;
            
            if (!isLoadedImage) {
                
                img = t_ofeliaCreateImage::images[pos].get();
            }
            else {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadImage::imageData[pos].paths.size())-1);
                
                if (!t_ofeliaLoadImage::imageData[pos].paths.empty()) {
                    
                    if (!t_ofeliaLoadImage::images[pos].empty() &&
                        index < static_cast<int>(t_ofeliaLoadImage::images[pos].size())) {
                        
                        img = t_ofeliaLoadImage::images[pos][index].get();
                    }
                    else {
                        
                        error("%s: '%s' is not loaded", t_ofeliaGetImageTexCoords::objName, name.c_str());
                        return;
                    }
                }
                else {
                    
                    error("%s: '%s' is empty", t_ofeliaGetImageTexCoords::objName, name.c_str());
                    return;
                }
            }
            if (img->isAllocated()) {
             
                if (argc && argc%2 == 0) {
                    
                    for (int i=0; i<argc; i+=2) {
                        
                        if (argv[i].a_type == A_FLOAT &&
                            argv[i+1].a_type == A_FLOAT) {
                            
                            const ofVec2f &texCoord = img->getTexture().getCoordFromPoint(argv[i].a_w.w_float, argv[i+1].a_w.w_float);
                            argv[i].a_w.w_float = texCoord.x;
                            argv[i+1].a_w.w_float = texCoord.y;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaGetImageTexCoords::objName);
                            return;
                        }
                    }
                    outlet_list(x->x_obj.ob_outlet, &s_list, argc, argv);
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaGetImageTexCoords::objName);
                }
            }
            else {
                
                error("%s: '%s' is not allocated", t_ofeliaGetImageTexCoords::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetImageTexCoords::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetImageTexCoords::objName);
    }
}

void ofeliaGetImageTexCoords_set(t_ofeliaGetImageTexCoords *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetImageTexCoords_print(t_ofeliaGetImageTexCoords *x)
{
    post("\n[%s]", t_ofeliaGetImageTexCoords::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetImageTexCoords_setup()
{
    ofeliaGetImageTexCoords_class = class_new(gensym("ofGetImageTexCoords"),
                                              reinterpret_cast<t_newmethod>(ofeliaGetImageTexCoords_new),
                                              0, sizeof(t_ofeliaGetImageTexCoords),
                                              CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaGetImageTexCoords_class, reinterpret_cast<t_method>(ofeliaGetImageTexCoords_float));
    class_addlist(ofeliaGetImageTexCoords_class, reinterpret_cast<t_method>(ofeliaGetImageTexCoords_list));
    class_addmethod(ofeliaGetImageTexCoords_class, reinterpret_cast<t_method>(ofeliaGetImageTexCoords_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetImageTexCoords_class, reinterpret_cast<t_method>(ofeliaGetImageTexCoords_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetImageTexCoords_class, reinterpret_cast<t_method>(ofeliaGetImageTexCoords_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetImageTexID object methods
 */
void *ofeliaGetImageTexID_new(t_symbol *s)
{
    t_ofeliaGetImageTexID *x = reinterpret_cast<t_ofeliaGetImageTexID*>(pd_new(ofeliaGetImageTexID_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetImageTexID_bang(t_ofeliaGetImageTexID *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        bool isLoadedImage;
        const int pos = getPositionByImageName(name, isLoadedImage);
        
        if (pos != -1) {
            
            ofImage *img = nullptr;
            
            if (!isLoadedImage) {
                
                img = t_ofeliaCreateImage::images[pos].get();
            }
            else {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadImage::imageData[pos].paths.size())-1);
                
                if (!t_ofeliaLoadImage::imageData[pos].paths.empty()) {
                    
                    if (!t_ofeliaLoadImage::images[pos].empty() &&
                        index < static_cast<int>(t_ofeliaLoadImage::images[pos].size())) {
                        
                        img = t_ofeliaLoadImage::images[pos][index].get();
                    }
                    else {
                        
                        error("%s: '%s' is not loaded", t_ofeliaGetImageTexID::objName, name.c_str());
                        return;
                    }
                }
                else {
                    
                    error("%s: '%s' is empty", t_ofeliaGetImageTexID::objName, name.c_str());
                    return;
                }
            }
            if (img->isAllocated())
                outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(img->getTexture().getTextureData().textureID));
            else
                error("%s: '%s' is not allocated", t_ofeliaGetImageTexID::objName, name.c_str());
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetImageTexID::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetImageTexID::objName);
    }
}

void ofeliaGetImageTexID_float(t_ofeliaGetImageTexID *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetImageTexID_set(t_ofeliaGetImageTexID *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetImageTexID_print(t_ofeliaGetImageTexID *x)
{
    post("\n[%s]", t_ofeliaGetImageTexID::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetImageTexID_setup()
{
    ofeliaGetImageTexID_class = class_new(gensym("ofGetImageTexID"),
                                          reinterpret_cast<t_newmethod>(ofeliaGetImageTexID_new),
                                          0, sizeof(t_ofeliaGetImageTexID),
                                          CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetImageTexID_class, reinterpret_cast<t_method>(ofeliaGetImageTexID_bang));
    class_addfloat(ofeliaGetImageTexID_class, reinterpret_cast<t_method>(ofeliaGetImageTexID_float));
    class_addmethod(ofeliaGetImageTexID_class, reinterpret_cast<t_method>(ofeliaGetImageTexID_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetImageTexID_class, reinterpret_cast<t_method>(ofeliaGetImageTexID_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetImageTexID_class, reinterpret_cast<t_method>(ofeliaGetImageTexID_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * setup methods
 */
void ofeliaImage_setup()
{
    ofeliaCreateImage_setup();
    ofeliaLoadImage_setup();
    ofeliaEditImage_setup();
    ofeliaSaveImage_setup();
    ofeliaBindImageTex_setup();
    ofeliaDrawImage_setup();
    ofeliaDrawSubImage_setup();
    ofeliaDoesImageNameExist_setup();
    ofeliaGetImagePath_setup();
    ofeliaIsImageAllocated_setup();
    ofeliaGetImageDimen_setup();
    ofeliaGetImageType_setup();
    ofeliaGetImageColorAt_setup();
    ofeliaGetImageTexCoord_setup();
    ofeliaGetImageTexCoords_setup();
    ofeliaGetImageTexID_setup();
}

