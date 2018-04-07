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

#include "ofeliaMesh.h"
#include "ofeliaWindow.h"

/* ________________________________________________________________________________
 * static variables
 */
const char *t_ofeliaLoadMesh2d::objName = "ofLoadMesh2d";
unsigned int t_ofeliaLoadMesh2d::counter;
vector<t_ofeliaLoadMesh2dData> t_ofeliaLoadMesh2d::meshData;
vector<unique_ptr<ofVboMesh>> t_ofeliaLoadMesh2d::meshes;
bool t_ofeliaLoadMesh2d::bInited;
const char *t_ofeliaLoadMesh3d::objName = "ofLoadMesh3d";
unsigned int t_ofeliaLoadMesh3d::counter;
vector<t_ofeliaLoadMesh3dData> t_ofeliaLoadMesh3d::meshData;
vector<unique_ptr<ofVboMesh>> t_ofeliaLoadMesh3d::meshes;
bool t_ofeliaLoadMesh3d::bInited;
const char *t_ofeliaDrawMesh2d::objName = "ofDrawMesh2d";
const char *t_ofeliaDrawMesh3d::objName = "ofDrawMesh3d";
const char *t_ofeliaDoesMesh2dNameExist::objName = "ofDoesMesh2dNameExist";
const char *t_ofeliaDoesMesh3dNameExist::objName = "ofDoesMesh3dNameExist";
const char *t_ofeliaEditMesh2dVertex::objName = "ofEditMesh2dVertex";
const char *t_ofeliaEditMesh3dVertex::objName = "ofEditMesh3dVertex";
const char *t_ofeliaEditMesh2dIndex::objName = "ofEditMesh2dIndex";
const char *t_ofeliaEditMesh3dIndex::objName = "ofEditMesh3dIndex";
const char *t_ofeliaEditMesh2dNormal::objName = "ofEditMesh2dNormal";
const char *t_ofeliaEditMesh3dNormal::objName = "ofEditMesh3dNormal";
const char *t_ofeliaEditMesh2dTexCoord::objName = "ofEditMesh2dTexCoord";
const char *t_ofeliaEditMesh3dTexCoord::objName = "ofEditMesh3dTexCoord";
const char *t_ofeliaEditMesh2dColor::objName = "ofEditMesh2dColor";
const char *t_ofeliaEditMesh3dColor::objName = "ofEditMesh3dColor";
const char *t_ofeliaGetMesh2dVertex::objName = "ofGetMesh2dVertex";
const char *t_ofeliaGetMesh3dVertex::objName = "ofGetMesh3dVertex";
const char *t_ofeliaGetMesh2dIndex::objName = "ofGetMesh2dIndex";
const char *t_ofeliaGetMesh3dIndex::objName = "ofGetMesh3dIndex";
const char *t_ofeliaGetMesh2dNormal::objName = "ofGetMesh2dNormal";
const char *t_ofeliaGetMesh3dNormal::objName = "ofGetMesh3dNormal";
const char *t_ofeliaGetMesh2dTexCoord::objName = "ofGetMesh2dTexCoord";
const char *t_ofeliaGetMesh3dTexCoord::objName = "ofGetMesh3dTexCoord";
const char *t_ofeliaGetMesh2dColor::objName = "ofGetMesh2dColor";
const char *t_ofeliaGetMesh3dColor::objName = "ofGetMesh3dColor";
const char *t_ofeliaGetMesh2dVertices::objName = "ofGetMesh2dVertices";
const char *t_ofeliaGetMesh3dVertices::objName = "ofGetMesh3dVertices";
const char *t_ofeliaGetMesh2dIndices::objName = "ofGetMesh2dIndices";
const char *t_ofeliaGetMesh3dIndices::objName = "ofGetMesh3dIndices";
const char *t_ofeliaGetMesh2dNormals::objName = "ofGetMesh2dNormals";
const char *t_ofeliaGetMesh3dNormals::objName = "ofGetMesh3dNormals";
const char *t_ofeliaGetMesh2dTexCoords::objName = "ofGetMesh2dTexCoords";
const char *t_ofeliaGetMesh3dTexCoords::objName = "ofGetMesh3dTexCoords";
const char *t_ofeliaGetMesh2dColors::objName = "ofGetMesh2dColors";
const char *t_ofeliaGetMesh3dColors::objName = "ofGetMesh3dColors";
const char *t_ofeliaGetMesh2dCommands::objName = "ofGetMesh2dCommands";
const char *t_ofeliaGetMesh3dCommands::objName = "ofGetMesh3dCommands";
const char *t_ofeliaGetMesh2dBoundingBox::objName = "ofGetMesh2dBoundingBox";
const char *t_ofeliaGetMesh3dBoundingBox::objName = "ofGetMesh3dBoundingBox";
const char *t_ofeliaGetMesh2dCentroid::objName = "ofGetMesh2dCentroid";
const char *t_ofeliaGetMesh3dCentroid::objName = "ofGetMesh3dCentroid";

/* ________________________________________________________________________________
 * common methods
 */
bool getPrimModeFromArgs(const int argc, const t_atom *argv, ofPrimitiveMode &primMode, const char *objName)
{
    switch (argc) {
            
        case 0:
        {
            primMode = OF_PRIMITIVE_TRIANGLES;
            break;
        }
        case 1:
        {
            if (argv[0].a_type == A_FLOAT) {
                
                const int mode = static_cast<int>(argv[0].a_w.w_float);
                
                if (mode < 0 || mode > 6) {
                    
                    error("%s: primMode out of range", objName);
                    return 0;
                }
                primMode = static_cast<ofPrimitiveMode>(mode);
            }
            else if (argv[0].a_type == A_SYMBOL) {
                
                const char *mode = argv[0].a_w.w_symbol->s_name;
                
                if (!strcmp(mode, "TRIANGLES"))
                    primMode = OF_PRIMITIVE_TRIANGLES;
                else if (!strcmp(mode, "TRIANGLE_STRIP"))
                    primMode = OF_PRIMITIVE_TRIANGLE_STRIP;
                else if (!strcmp(mode, "TRIANGLE_FAN"))
                    primMode = OF_PRIMITIVE_TRIANGLE_FAN;
                else if (!strcmp(mode, "LINES"))
                    primMode = OF_PRIMITIVE_LINES;
                else if (!strcmp(mode, "LINE_STRIP"))
                    primMode = OF_PRIMITIVE_LINE_STRIP;
                else if (!strcmp(mode, "LINE_LOOP"))
                    primMode = OF_PRIMITIVE_LINE_LOOP;
                else if (!strcmp(mode, "POINTS"))
                    primMode = OF_PRIMITIVE_POINTS;
                else {
                    
                    error("%s: no method for '%s'", objName, mode);
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

void printPrimMode(ofPrimitiveMode primMode)
{
    t_string mode;
    
    switch (primMode) {
            
        case OF_PRIMITIVE_TRIANGLES:
            mode = "TRIANGLES";
            break;
        case OF_PRIMITIVE_TRIANGLE_STRIP:
            mode = "TRIANGLE_STRIP";
            break;
        case OF_PRIMITIVE_TRIANGLE_FAN:
            mode = "TRIANGLE_FAN";
            break;
        case OF_PRIMITIVE_LINES:
            mode = "LINES";
            break;
        case OF_PRIMITIVE_LINE_STRIP:
            mode = "LINE_STRIP";
            break;
        case OF_PRIMITIVE_LINE_LOOP:
            mode = "LINE_LOOP";
            break;
        case OF_PRIMITIVE_POINTS:
            mode = "POINTS";
            break;
        default:
            break;
    }
    post("primMode : %s", mode.c_str());
}

void updateMesh2dProperties(const int pos)
{
    getBoundingBoxDimen2d(t_ofeliaLoadMesh2d::meshData[pos].boxDimen,
                          t_ofeliaLoadMesh2d::meshes[pos]->getVertices());
    t_ofeliaLoadMesh2d::meshData[pos].centroid = t_ofeliaLoadMesh2d::meshes[pos]->getCentroid();
    t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = false;
}

void updateMesh3dProperties(const int pos)
{
    getBoundingBoxDimen3d(t_ofeliaLoadMesh3d::meshData[pos].boxDimen,
                          t_ofeliaLoadMesh3d::meshes[pos]->getVertices());
    t_ofeliaLoadMesh3d::meshData[pos].centroid = t_ofeliaLoadMesh3d::meshes[pos]->getCentroid();
    t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = false;
}

/* ________________________________________________________________________________
 * ofLoadMesh2d object methods
 */
void ofeliaLoadMesh2d_clear(t_ofeliaLoadMesh2d *x, t_symbol *s, int argc, t_atom *argv);
void ofeliaLoadMesh2d_vecSizeOut(t_ofeliaLoadMesh2d *x);

int getPositionByMesh2dObjID(const unsigned int objID)
{
    const auto it = find_if(begin(t_ofeliaLoadMesh2d::meshData), end(t_ofeliaLoadMesh2d::meshData),
                            [&objID](const t_ofeliaLoadMesh2dData &meshData)
                            {
                                return meshData.objID == objID;
                            });
    return static_cast<int>(it - t_ofeliaLoadMesh2d::meshData.begin());
}

bool setLoadMesh2dElemType(const char *name, t_ofeliaLoadMesh2dElem &elem)
{
    if (!strcmp(name, "vertex"))  {
        
        elem.type = MESH_LOAD_CMDTYPE_VERTEX;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "index")) {
        
        elem.type = MESH_LOAD_CMDTYPE_INDEX;
        elem.numArgs = 1;
    }
    else if (!strcmp(name, "normal")) {
        
        elem.type = MESH_LOAD_CMDTYPE_NORMAL;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "texCoord")) {
        
        elem.type = MESH_LOAD_CMDTYPE_TEXCOORD;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "color")) {
        
        elem.type = MESH_LOAD_CMDTYPE_COLOR;
        elem.numArgs = 4;
    }
    else if (!strcmp(name, "circle")) {
        
        elem.type = MESH_LOAD_CMDTYPE_CIRCLE;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "ellipse")) {
        
        elem.type = MESH_LOAD_CMDTYPE_ELLIPSE;
        elem.numArgs = 3;
    }
    else if (!strcmp(name, "sector")) {
        
        elem.type = MESH_LOAD_CMDTYPE_SECTOR;
        elem.numArgs = 5;
    }
    else if (!strcmp(name, "eqTriangle") || !strcmp(name, "eqTri")) {
        
        elem.type = MESH_LOAD_CMDTYPE_EQTRIANGLE;
        elem.numArgs = 1;
    }
    else if (!strcmp(name, "isoTriangle") || !strcmp(name, "isoTri")) {
        
        elem.type = MESH_LOAD_CMDTYPE_ISOTRIANGLE;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "square")) {
        
        elem.type = MESH_LOAD_CMDTYPE_SQUARE;
        elem.numArgs = 1;
    }
    else if (!strcmp(name, "rectangle") || !strcmp(name, "rect")) {
        
        elem.type = MESH_LOAD_CMDTYPE_RECTANGLE;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "rectRounded")) {
        
        elem.type = MESH_LOAD_CMDTYPE_RECTROUNDED;
        elem.numArgs = 4;
    }
    else if (!strcmp(name, "rectRounded4")) {
        
        elem.type = MESH_LOAD_CMDTYPE_RECTROUNDED4;
        elem.numArgs = 7;
    }
    else if (!strcmp(name, "cross")) {
        
        elem.type = MESH_LOAD_CMDTYPE_CROSS;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "heart")) {
        
        elem.type = MESH_LOAD_CMDTYPE_HEART;
        elem.numArgs = 3;
    }
    else if (!strcmp(name, "moon")) {
        
        elem.type = MESH_LOAD_CMDTYPE_MOON;
        elem.numArgs = 4;
    }
    else if (!strcmp(name, "regPolygon")) {
        
        elem.type = MESH_LOAD_CMDTYPE_REGPOLYGON;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "star")) {
        
        elem.type = MESH_LOAD_CMDTYPE_STAR;
        elem.numArgs = 3;
    }
    else if (!strcmp(name, "box")) {
        
        elem.type = MESH_LOAD_CMDTYPE_BOX;
        elem.numArgs = 6;
    }
    else if (!strcmp(name, "cone")) {
        
        elem.type = MESH_LOAD_CMDTYPE_CONE;
        elem.numArgs = 6;
    }
    else if (!strcmp(name, "cylinder")) {
        
        elem.type = MESH_LOAD_CMDTYPE_CYLINDER;
        elem.numArgs = 7;
    }
    else if (!strcmp(name, "icosphere")) {
        
        elem.type = MESH_LOAD_CMDTYPE_ICOSPHERE;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "plane")) {
        
        elem.type = MESH_LOAD_CMDTYPE_PLANE;
        elem.numArgs = 5;
    }
    else if (!strcmp(name, "sphere")) {
        
        elem.type = MESH_LOAD_CMDTYPE_SPHERE;
        elem.numArgs = 3;
    }
    else {
        
        error("%s: no method for '%s'", t_ofeliaLoadMesh2d::objName, name);
        return 0;
    }
    return 1;
}

bool setLoadMesh2dModElemType(const char *name, t_ofeliaLoadMesh2dElem &elem)
{
    if (!strcmp(name, "vertex"))  {
        
        elem.type = MESH_LOAD_CMDTYPE_VERTEX;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "index")) {
        
        elem.type = MESH_LOAD_CMDTYPE_INDEX;
        elem.numArgs = 1;
    }
    else if (!strcmp(name, "normal")) {
        
        elem.type = MESH_LOAD_CMDTYPE_NORMAL;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "texCoord")) {
        
        elem.type = MESH_LOAD_CMDTYPE_TEXCOORD;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "color")) {
        
        elem.type = MESH_LOAD_CMDTYPE_COLOR;
        elem.numArgs = 4;
    }
    else if (!strcmp(name, "circle") ||
             !strcmp(name, "ellipse") ||
             !strcmp(name, "sector") ||
             !strcmp(name, "eqTriangle") ||
             !strcmp(name, "eqTri") ||
             !strcmp(name, "isoTriangle") ||
             !strcmp(name, "isoTri") ||
             !strcmp(name, "square") ||
             !strcmp(name, "rectangle") ||
             !strcmp(name, "rectRounded") ||
             !strcmp(name, "rectRounded4") ||
             !strcmp(name, "cross") ||
             !strcmp(name, "heart") ||
             !strcmp(name, "moon") ||
             !strcmp(name, "regPolygon") ||
             !strcmp(name, "star") ||
             !strcmp(name, "box") ||
             !strcmp(name, "cone") ||
             !strcmp(name, "cylinder") ||
             !strcmp(name, "icosahedron") ||
             !strcmp(name, "icosphere") ||
             !strcmp(name, "plane") ||
             !strcmp(name, "sphere")) {
        
        error("%s: '%s' can only be used with 'load', 'add' or 'append'", t_ofeliaLoadMesh2d::objName, name);
        return 0;
    }
    else {
        
        error("%s: no method for '%s'", t_ofeliaLoadMesh2d::objName, name);
        return 0;
    }
    return 1;
}

bool getLoadMesh2dElemFromArgs(const int argc, const t_atom *argv, t_ofeliaLoadMesh2dElem &elem)
{
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            const char *name = argv[0].a_w.w_symbol->s_name;
            
            if (!setLoadMesh2dElemType(name, elem))
                return 0;
            
            if (elem.numArgs + 1 > argc) {
                
                error("Bad arguments for message '%s' to object '%s'", name, t_ofeliaLoadMesh2d::objName);
                return 0;
            }
            for (int i=0; i<elem.numArgs; ++i) {
                
                const int index = i+1;
                
                if (argv[index].a_type == A_FLOAT) {
                    
                    elem.args[i] = argv[index].a_w.w_float;
                }
                else {
                    
                    error("Bad arguments for message '%s' to object '%s'", name, t_ofeliaLoadMesh2d::objName);
                    return 0;
                }
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadMesh2d::objName);
            return 0;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadMesh2d::objName);
        return 0;
    }
    return 1;
}

bool getLoadMesh2dModElemFromArgs(const int argc, const t_atom *argv, t_ofeliaLoadMesh2dElem &elem)
{
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            const char *name = argv[0].a_w.w_symbol->s_name;
            
            if (!setLoadMesh2dModElemType(name, elem))
                return 0;
            
            if (elem.numArgs + 1 > argc) {
                
                error("Bad arguments for message '%s' to object '%s'", name, t_ofeliaLoadMesh2d::objName);
                return 0;
            }
            for (int i=0; i<elem.numArgs; ++i) {
                
                const int index = i+1;
                
                if (argv[index].a_type == A_FLOAT) {
                    
                    elem.args[i] = argv[index].a_w.w_float;
                }
                else {
                    
                    error("Bad arguments for message '%s' to object '%s'", name, t_ofeliaLoadMesh2d::objName);
                    return 0;
                }
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadMesh2d::objName);
            return 0;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadMesh2d::objName);
        return 0;
    }
    return 1;
}

bool getLoadMesh2dElemsFromArgs(const int argc, const t_atom *argv, vector<t_ofeliaLoadMesh2dElem> &elems)
{
    t_string name;
    t_ofeliaLoadMesh2dElem elem;
    
    for (int i=0; i<argc; i+=elem.numArgs) {
        
        if (argv[i].a_type == A_SYMBOL) {
            
            name = argv[i].a_w.w_symbol->s_name;
            
            if (!setLoadMesh2dElemType(name.c_str(), elem))
                return 0;
            ++i;
        }
        if (name.empty()) {
            
            error("%s: wrong argument type", t_ofeliaLoadMesh2d::objName);
            return 0;
        }
        if (i+elem.numArgs > argc) {
            
            error("Bad arguments for message '%s' to object '%s'", name.c_str(), t_ofeliaLoadMesh2d::objName);
            return 0;
        }
        for (int j=0; j<elem.numArgs; ++j) {
            
            const int index = i+j;
            
            if (argv[index].a_type == A_FLOAT) {
                
                elem.args[j] = argv[index].a_w.w_float;
            }
            else {
                
                error("Bad arguments for message '%s' to object '%s'", name.c_str(), t_ofeliaLoadMesh2d::objName);
                return 0;
            }
        }
        elems.push_back(elem);
    }
    return 1;
}

bool getLoadMesh2dModElemsFromArgs(const int argc, const t_atom *argv, vector<t_ofeliaLoadMesh2dElem> &elems)
{
    t_string name;
    t_ofeliaLoadMesh2dElem elem;
    
    for (int i=0; i<argc; i+=elem.numArgs) {
        
        if (argv[i].a_type == A_SYMBOL) {
            
            name = argv[i].a_w.w_symbol->s_name;
            
            if (!setLoadMesh2dModElemType(name.c_str(), elem))
                return 0;
            ++i;
        }
        if (name.empty()) {
            
            error("%s: wrong argument type", t_ofeliaLoadMesh2d::objName);
            return 0;
        }
        if (i+elem.numArgs > argc) {
            
            error("Bad arguments for message '%s' to object '%s'", name.c_str(), t_ofeliaLoadMesh2d::objName);
            return 0;
        }
        for (int j=0; j<elem.numArgs; ++j) {
            
            const int index = i+j;
            
            if (argv[index].a_type == A_FLOAT) {
                
                elem.args[j] = argv[index].a_w.w_float;
            }
            else {
                
                error("Bad arguments for message '%s' to object '%s'", name.c_str(), t_ofeliaLoadMesh2d::objName);
                return 0;
            }
        }
        elems.push_back(elem);
    }
    return 1;
}

bool getCmdRangeFromArgs(const int argc, const t_atom *argv, t_ofeliaLoadMesh2dCmdData &cmd)
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
                
                error("%s: wrong argument type", t_ofeliaLoadMesh2d::objName);
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
                
                error("%s: wrong argument type", t_ofeliaLoadMesh2d::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaLoadMesh2d::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaLoadMesh2d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadMesh2d *x = reinterpret_cast<t_ofeliaLoadMesh2d*>(pd_new(ofeliaLoadMesh2d_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    t_ofeliaLoadMesh2dData meshData;
    getVarNameLocalPrefixes(meshData.varName);
    vector<t_ofeliaLoadMesh2dElem> elems;
    
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            meshData.varName.name = argv[0].a_w.w_symbol->s_name;
            getVarNameLocalized(meshData.varName);
            
            if (--argc) {
                
                if (!getLoadMesh2dElemsFromArgs(argc, argv+1, elems))
                    return 0;
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadMesh2d::objName);
            return 0;
        }
    }
    x->shouldOutlet = false;
    x->bInitGate = false;
    x->primMode = OF_PRIMITIVE_TRIANGLES;
    x->numVertices = 0;
    x->numIndices = 0;
    x->numNormals = 0;
    x->numTexCoords = 0;
    x->numColors = 0;
    x->objID = meshData.objID = t_ofeliaLoadMesh2d::counter++;
    t_ofeliaLoadMesh2d::meshData.push_back(meshData);
    t_ofeliaLoadMesh2d::meshes.push_back(make_unique<ofVboMesh>());
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::updateSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    x->numVertices_out = outlet_new(&x->x_obj, &s_float);
    x->numIndices_out = outlet_new(&x->x_obj, &s_float);
    x->numNormals_out = outlet_new(&x->x_obj, &s_float);
    x->numTexCoords_out = outlet_new(&x->x_obj, &s_float);
    x->numColors_out = outlet_new(&x->x_obj, &s_float);
    
    if (argc) {
        
        ofeliaLoadMesh2d_clear(x, 0, 0, 0);
        
        for (size_t i=0; i<elems.size(); ++i) {
            
            t_ofeliaLoadMesh2dCmdData cmd;
            cmd.elem = elems[i];
            cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
            cmd.state = MESH_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
    return (x);
}

void ofeliaLoadMesh2d_init(t_ofeliaLoadMesh2d *x)
{
    if (!t_ofeliaLoadMesh2d::bInited && x->bInitGate) {
        
        t_ofeliaLoadMesh2d::meshes.clear();
        
        for (size_t i=0; i<t_ofeliaLoadMesh2d::meshData.size(); ++i)
            t_ofeliaLoadMesh2d::meshes.push_back(make_unique<ofVboMesh>());
        t_ofeliaLoadMesh2d::bInited = true;
    }
}

void ofeliaLoadMesh2d_update(t_ofeliaLoadMesh2d *x)
{
    const int pos = getPositionByMesh2dObjID(x->objID);
    
    if (!x->cmdVec.empty()) {
        
        const size_t cmdVecSize = x->cmdVec.size();
        
        /* read all recorded commands */
        for (size_t i=0; i<cmdVecSize; ++i) {
            
            const t_ofeliaLoadMesh2dElem &elem = x->cmdVec[i].elem;
            
            switch (elem.type) {
                    
                case MESH_LOAD_CMDTYPE_VERTEX:
                {
                    const int numVertices = static_cast<int>(t_ofeliaLoadMesh2d::meshData[pos].mesh.getNumVertices());
                    const auto verticesBegin = t_ofeliaLoadMesh2d::meshData[pos].mesh.getVertices().begin();
                    
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, numVertices));
                            const int insertSize = static_cast<int>(max(0.0f, max(0.0f, x->cmdVec[i].toIndex) - max(0.0f, x->cmdVec[i].fromIndex))) + 1;
                            t_ofeliaLoadMesh2d::meshData[pos].mesh.getVertices().insert(verticesBegin + fromIndex,
                                                                                        insertSize,
                                                                                        ofVec2f(elem.args[0],
                                                                                                elem.args[1]));
                            break;
                        }
                        case MESH_LOAD_CMD_ERASE:
                        {
                            if (numVertices) {
                                
                                const int maxIndex = numVertices - 1;
                                const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, maxIndex));
                                const int toIndex = static_cast<int>(ofClamp(x->cmdVec[i].toIndex, fromIndex, maxIndex)) + 1;
                                t_ofeliaLoadMesh2d::meshData[pos].mesh.getVertices().erase(verticesBegin + fromIndex,
                                                                                           verticesBegin + toIndex);
                            }
                            break;
                        }
                        case MESH_LOAD_CMD_FILL:
                        {
                            if (numVertices) {
                                
                                const int maxIndex = numVertices - 1;
                                const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, maxIndex));
                                const int toIndex = static_cast<int>(ofClamp(x->cmdVec[i].toIndex, fromIndex, maxIndex)) + 1;
                                fill(verticesBegin + fromIndex, verticesBegin + toIndex, ofVec2f(elem.args[0],
                                                                                                 elem.args[1]));
                            }
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_INDEX:
                {
                    const int numIndices = static_cast<int>(t_ofeliaLoadMesh2d::meshData[pos].mesh.getNumIndices());
                    const auto indicesBegin = t_ofeliaLoadMesh2d::meshData[pos].mesh.getIndices().begin();
                    
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, numIndices));
                            const int insertSize = static_cast<int>(max(0.0f, max(0.0f, x->cmdVec[i].toIndex) - max(0.0f, x->cmdVec[i].fromIndex))) + 1;
                            t_ofeliaLoadMesh2d::meshData[pos].mesh.getIndices().insert(indicesBegin + fromIndex,
                                                                                       insertSize,
                                                                                       static_cast<ofIndexType>
                                                                                       (elem.args[0]));
                            break;
                        }
                        case MESH_LOAD_CMD_ERASE:
                        {
                            if (numIndices) {
                                
                                const int maxIndex = numIndices - 1;
                                const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, maxIndex));
                                const int toIndex = static_cast<int>(ofClamp(x->cmdVec[i].toIndex, fromIndex, maxIndex)) + 1;
                                t_ofeliaLoadMesh2d::meshData[pos].mesh.getIndices().erase(indicesBegin + fromIndex,
                                                                                          indicesBegin + toIndex);
                            }
                            break;
                        }
                        case MESH_LOAD_CMD_FILL:
                        {
                            if (numIndices) {
                                
                                const int maxIndex = numIndices - 1;
                                const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, maxIndex));
                                const int toIndex = static_cast<int>(ofClamp(x->cmdVec[i].toIndex, fromIndex, maxIndex)) + 1;
                                fill(indicesBegin + fromIndex, indicesBegin + toIndex,
                                     static_cast<ofIndexType>(elem.args[0]));
                            }
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_NORMAL:
                {
                    const int numNormals = static_cast<int>(t_ofeliaLoadMesh2d::meshData[pos].mesh.getNumNormals());
                    const auto normalsBegin = t_ofeliaLoadMesh2d::meshData[pos].mesh.getNormals().begin();
                    
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, numNormals));
                            const int insertSize = static_cast<int>(max(0.0f, max(0.0f, x->cmdVec[i].toIndex) - max(0.0f, x->cmdVec[i].fromIndex))) + 1;
                            t_ofeliaLoadMesh2d::meshData[pos].mesh.getNormals().insert(normalsBegin + fromIndex,
                                                                                       insertSize,
                                                                                       ofVec2f(elem.args[0],
                                                                                               elem.args[1]));
                            break;
                        }
                        case MESH_LOAD_CMD_ERASE:
                        {
                            if (numNormals) {
                                
                                const int maxIndex = numNormals - 1;
                                const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, maxIndex));
                                const int toIndex = static_cast<int>(ofClamp(x->cmdVec[i].toIndex, fromIndex, maxIndex)) + 1;
                                t_ofeliaLoadMesh2d::meshData[pos].mesh.getNormals().erase(normalsBegin + fromIndex,
                                                                                          normalsBegin + toIndex);
                            }
                            break;
                        }
                        case MESH_LOAD_CMD_FILL:
                        {
                            if (numNormals) {
                                
                                const int maxIndex = numNormals - 1;
                                const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, maxIndex));
                                const int toIndex = static_cast<int>(ofClamp(x->cmdVec[i].toIndex, fromIndex, maxIndex)) + 1;
                                fill(normalsBegin + fromIndex, normalsBegin + toIndex, ofVec2f(elem.args[0],
                                                                                               elem.args[1]));
                            }
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_TEXCOORD:
                {
                    const int numTexCoords = static_cast<int>(t_ofeliaLoadMesh2d::meshData[pos].mesh.getNumTexCoords());
                    const auto texCoordsBegin = t_ofeliaLoadMesh2d::meshData[pos].mesh.getTexCoords().begin();
                    
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, numTexCoords));
                            const int insertSize = static_cast<int>(max(0.0f, max(0.0f, x->cmdVec[i].toIndex) - max(0.0f, x->cmdVec[i].fromIndex))) + 1;
                            t_ofeliaLoadMesh2d::meshData[pos].mesh.getTexCoords().insert(texCoordsBegin + fromIndex,
                                                                                         insertSize,
                                                                                         ofVec2f(elem.args[0],
                                                                                                 elem.args[1]));
                            break;
                        }
                        case MESH_LOAD_CMD_ERASE:
                        {
                            if (numTexCoords) {
                                
                                const int maxIndex = numTexCoords - 1;
                                const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, maxIndex));
                                const int toIndex = static_cast<int>(ofClamp(x->cmdVec[i].toIndex, fromIndex, maxIndex)) + 1;
                                t_ofeliaLoadMesh2d::meshData[pos].mesh.getTexCoords().erase(texCoordsBegin +
                                                                                            fromIndex,
                                                                                            texCoordsBegin + toIndex);
                            }
                            break;
                        }
                        case MESH_LOAD_CMD_FILL:
                        {
                            if (numTexCoords) {
                                
                                const int maxIndex = numTexCoords - 1;
                                const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, maxIndex));
                                const int toIndex = static_cast<int>(ofClamp(x->cmdVec[i].toIndex, fromIndex, maxIndex)) + 1;
                                fill(texCoordsBegin + fromIndex, texCoordsBegin + toIndex, ofVec2f(elem.args[0],
                                                                                                   elem.args[1]));
                            }
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_COLOR:
                {
                    const int numColors = static_cast<int>(t_ofeliaLoadMesh2d::meshData[pos].mesh.getNumColors());
                    const auto colorsBegin = t_ofeliaLoadMesh2d::meshData[pos].mesh.getColors().begin();
                    
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, numColors));
                            const int insertSize = static_cast<int>(max(0.0f, max(0.0f, x->cmdVec[i].toIndex) - max(0.0f, x->cmdVec[i].fromIndex))) + 1;
                            t_ofeliaLoadMesh2d::meshData[pos].mesh.getColors().insert(colorsBegin + fromIndex,
                                                                                      insertSize,
                                                                                      ofColor(static_cast<unsigned
                                                                                              char>(elem.args[0]),
                                                                                              static_cast<unsigned
                                                                                              char>(elem.args[1]),
                                                                                              static_cast<unsigned
                                                                                              char>(elem.args[2]),
                                                                                              static_cast<unsigned
                                                                                              char>(elem.args[3])));
                            break;
                        }
                        case MESH_LOAD_CMD_ERASE:
                        {
                            if (numColors) {
                                
                                const int maxIndex = numColors - 1;
                                const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, maxIndex));
                                const int toIndex = static_cast<int>(ofClamp(x->cmdVec[i].toIndex, fromIndex, maxIndex)) + 1;
                                t_ofeliaLoadMesh2d::meshData[pos].mesh.getColors().erase(colorsBegin + fromIndex,
                                                                                         colorsBegin + toIndex);
                            }
                            break;
                        }
                        case MESH_LOAD_CMD_FILL:
                        {
                            if (numColors) {
                                
                                const int maxIndex = numColors - 1;
                                const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, maxIndex));
                                const int toIndex = static_cast<int>(ofClamp(x->cmdVec[i].toIndex, fromIndex, maxIndex)) + 1;
                                fill(colorsBegin + fromIndex, colorsBegin + toIndex, ofColor(static_cast<unsigned
                                                                                             char>(elem.args[0]),
                                                                                             static_cast<unsigned
                                                                                             char>(elem.args[1]),
                                                                                             static_cast<unsigned
                                                                                             char>(elem.args[2]),
                                                                                             static_cast<unsigned
                                                                                             char>(elem.args[3])));
                            }
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_CIRCLE:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofPath path;
                            ofPolyline polyline;
                            polyline.arc(ofVec2f(0.0f ,0.0f), elem.args[0], elem.args[0],
                                         0.0f, 360.0f, true, static_cast<int>(elem.args[1]));
                            path.moveTo(polyline.getVertices()[0]);
                            
                            for (size_t i=1; i<polyline.getVertices().size(); ++i)
                                path.lineTo(polyline.getVertices()[i]);
                            path.close();
                            ofMesh mesh = path.getTessellation();
                            
                            for (auto & v: mesh.getVertices()) {
                                
                                mesh.addNormal(ofVec3f(0.0f, 0.0f, 1.0f));
                                mesh.addTexCoord(ofVec2f((v.x + elem.args[0]) / (elem.args[0]*2.0f),
                                                         (v.y + elem.args[0]) / (elem.args[0]*2.0f)));
                            }
                            t_ofeliaLoadMesh2d::meshData[pos].mesh.append(mesh);
                            x->primMode = OF_PRIMITIVE_TRIANGLES;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_ELLIPSE:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofPath path;
                            ofPolyline polyline;
                            polyline.arc(ofVec2f(0.0f ,0.0f), elem.args[0]*0.5f, elem.args[1]*0.5f,
                                         0.0f, 360.0f, true, static_cast<int>(elem.args[2]));
                            path.moveTo(polyline.getVertices()[0]);
                            
                            for (size_t i=1; i<polyline.getVertices().size(); ++i)
                                path.lineTo(polyline.getVertices()[i]);
                            path.close();
                            ofMesh mesh = path.getTessellation();
                            
                            for (auto & v: mesh.getVertices()) {
                                
                                mesh.addNormal(ofVec3f(0.0f, 0.0f, 1.0f));
                                mesh.addTexCoord(ofVec2f((v.x + elem.args[0]*0.5f) / elem.args[0],
                                                         (v.y + elem.args[1]*0.5f) / elem.args[1]));
                            }
                            t_ofeliaLoadMesh2d::meshData[pos].mesh.append(mesh);
                            x->primMode = OF_PRIMITIVE_TRIANGLES;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_SECTOR:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofPath path;
                            ofPolyline polyline;
                            polyline.addVertex(ofVec2f(0.0f ,0.0f));
                            polyline.arc(ofVec2f(0.0f ,0.0f),
                                         elem.args[0], elem.args[1],
                                         elem.args[2], elem.args[3],
                                         elem.args[2] < elem.args[3], static_cast<int>(elem.args[4]));
                            path.moveTo(polyline.getVertices()[0]);
                            
                            for (size_t i=1; i<polyline.getVertices().size(); ++i)
                                path.lineTo(polyline.getVertices()[i]);
                            path.close();
                            ofMesh mesh = path.getTessellation();
                            
                            for (auto & v: mesh.getVertices()) {
                                
                                mesh.addNormal(ofVec3f(0.0f, 0.0f, 1.0f));
                                mesh.addTexCoord(ofVec2f((v.x + elem.args[0]) / (elem.args[0]*2.0f),
                                                         (v.y + elem.args[1]) / (elem.args[1]*2.0f)));
                            }
                            t_ofeliaLoadMesh2d::meshData[pos].mesh.append(mesh);
                            x->primMode = OF_PRIMITIVE_TRIANGLES;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_EQTRIANGLE:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofPath path;
                            const float halfSize = elem.args[0] * 0.5f;
                            const float inRadius = elem.args[0] * ofeliaConstants::heightFactor / 3;
                            path.triangle(0.0f, -inRadius*2.0f, 0.0f,
                                          halfSize, inRadius, 0.0f,
                                          -halfSize, inRadius, 0.0f);
                            ofMesh mesh = path.getTessellation();
                            
                            for (auto & v: mesh.getVertices()) {
                                
                                mesh.addNormal(ofVec3f(0.0f, 0.0f, 1.0f));
                                mesh.addTexCoord(ofVec2f((v.x + elem.args[0]*0.5f) / elem.args[0],
                                                         (v.y + elem.args[0]*0.5f) / elem.args[0]));
                            }
                            t_ofeliaLoadMesh2d::meshData[pos].mesh.append(mesh);
                            x->primMode = OF_PRIMITIVE_TRIANGLES;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_ISOTRIANGLE:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofPath path;
                            const float halfWidth = elem.args[0] * 0.5f;
                            const float halfHeight = elem.args[1] * 0.5f;
                            path.triangle(0.0f, -halfHeight, 0.0f,
                                          halfWidth, halfHeight, 0.0f,
                                          -halfWidth, halfHeight, 0.0f);
                            ofMesh mesh = path.getTessellation();
                            
                            for (auto & v: mesh.getVertices()) {
                                
                                mesh.addNormal(ofVec3f(0.0f, 0.0f, 1.0f));
                                mesh.addTexCoord(ofVec2f((v.x + elem.args[0]*0.5f) / elem.args[0],
                                                         (v.y + elem.args[1]*0.5f) / elem.args[1]));
                            }
                            t_ofeliaLoadMesh2d::meshData[pos].mesh.append(mesh);
                            x->primMode = OF_PRIMITIVE_TRIANGLES;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_SQUARE:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofPath path;
                            path.rectangle(0.0f, 0.0f, elem.args[0], elem.args[0]);
                            ofMesh mesh = path.getTessellation();
                            
                            for (auto & v: mesh.getVertices()) {
                                
                                mesh.addNormal(ofVec3f(0.0f, 0.0f, 1.0f));
                                mesh.addTexCoord(ofVec2f(v.x / elem.args[0],
                                                         v.y / elem.args[0]));
                            }
                            t_ofeliaLoadMesh2d::meshData[pos].mesh.append(mesh);
                            x->primMode = OF_PRIMITIVE_TRIANGLES;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_RECTANGLE:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofPath path;
                            path.rectangle(0.0f, 0.0f, elem.args[0], elem.args[1]);
                            ofMesh mesh = path.getTessellation();
                            
                            for (auto & v: mesh.getVertices()) {
                                
                                mesh.addNormal(ofVec3f(0.0f, 0.0f, 1.0f));
                                mesh.addTexCoord(ofVec2f(v.x / elem.args[0],
                                                         v.y / elem.args[1]));
                            }
                            t_ofeliaLoadMesh2d::meshData[pos].mesh.append(mesh);
                            x->primMode = OF_PRIMITIVE_TRIANGLES;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_RECTROUNDED:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofPath path;
                            path.setCircleResolution(static_cast<int>(elem.args[3]));
                            path.rectRounded(0.0f, 0.0f, 0.0f,
                                             elem.args[0], elem.args[1],
                                             elem.args[2], elem.args[2],
                                             elem.args[2], elem.args[2]);
                            ofMesh mesh = path.getTessellation();
                            
                            for (auto & v: mesh.getVertices()) {
                                
                                mesh.addNormal(ofVec3f(0.0f, 0.0f, 1.0f));
                                mesh.addTexCoord(ofVec2f(v.x / elem.args[0],
                                                         v.y / elem.args[1]));
                            }
                            t_ofeliaLoadMesh2d::meshData[pos].mesh.append(mesh);
                            x->primMode = OF_PRIMITIVE_TRIANGLES;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_RECTROUNDED4:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofPath path;
                            path.setCircleResolution(static_cast<int>(elem.args[6]));
                            path.rectRounded(0.0f, 0.0f, 0.0f,
                                             elem.args[0], elem.args[1],
                                             elem.args[2], elem.args[3],
                                             elem.args[4], elem.args[5]);
                            ofMesh mesh = path.getTessellation();
                            
                            for (auto & v: mesh.getVertices()) {
                                
                                mesh.addNormal(ofVec3f(0.0f, 0.0f, 1.0f));
                                mesh.addTexCoord(ofVec2f(v.x / elem.args[0],
                                                         v.y / elem.args[1]));
                            }
                            t_ofeliaLoadMesh2d::meshData[pos].mesh.append(mesh);
                            x->primMode = OF_PRIMITIVE_TRIANGLES;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_CROSS:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofPath path;
                            const float sHalf = elem.args[0] * 0.5f;
                            const float tHalf = elem.args[1] * 0.5f;
                            const float cX_tHalf_add = tHalf;
                            const float cX_tHalf_sub = -tHalf;
                            const float cX_sHalf_add = sHalf;
                            const float cX_sHalf_sub = -sHalf;
                            const float cY_tHalf_add = tHalf;
                            const float cY_tHalf_sub = -tHalf;
                            const float cY_sHalf_add = sHalf;
                            const float cY_sHalf_sub = -sHalf;
                            path.moveTo(ofVec2f(cX_tHalf_sub, cY_sHalf_sub));
                            path.lineTo(ofVec2f(cX_tHalf_add, cY_sHalf_sub));
                            path.lineTo(ofVec2f(cX_tHalf_add, cY_tHalf_sub));
                            path.lineTo(ofVec2f(cX_sHalf_add, cY_tHalf_sub));
                            path.lineTo(ofVec2f(cX_sHalf_add, cY_tHalf_add));
                            path.lineTo(ofVec2f(cX_tHalf_add, cY_tHalf_add));
                            path.lineTo(ofVec2f(cX_tHalf_add, cY_sHalf_add));
                            path.lineTo(ofVec2f(cX_tHalf_sub, cY_sHalf_add));
                            path.lineTo(ofVec2f(cX_tHalf_sub, cY_tHalf_add));
                            path.lineTo(ofVec2f(cX_sHalf_sub, cY_tHalf_add));
                            path.lineTo(ofVec2f(cX_sHalf_sub, cY_tHalf_sub));
                            path.lineTo(ofVec2f(cX_tHalf_sub, cY_tHalf_sub));
                            path.close();
                            ofMesh mesh = path.getTessellation();
                            
                            for (auto & v: mesh.getVertices()) {
                                
                                mesh.addNormal(ofVec3f(0.0f, 0.0f, 1.0f));
                                mesh.addTexCoord(ofVec2f((v.x + elem.args[0]*0.5f) / elem.args[0],
                                                         (v.y + elem.args[0]*0.5f) / elem.args[0]));
                            }
                            t_ofeliaLoadMesh2d::meshData[pos].mesh.append(mesh);
                            x->primMode = OF_PRIMITIVE_TRIANGLES;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_HEART:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofPath path;
                            path.setCurveResolution(static_cast<int>(elem.args[2]));
                            const float width = elem.args[0];
                            const float height = elem.args[1];
                            const float widthStretched = width * 1.125f;
                            const float heightHalf = height * 0.5f;
                            const ofVec2f beginEnd(0.0f, -height * 0.25f);
                            const ofVec2f bezierP1(0.0f, -height * 0.75f);
                            const ofVec2f bezierP3(0.0f, heightHalf);
                            path.moveTo(beginEnd);
                            path.bezierTo(bezierP1,
                                          ofVec2f(widthStretched, 5.0f-heightHalf),
                                          bezierP3);
                            path.moveTo(beginEnd);
                            path.bezierTo(bezierP1,
                                          ofVec2f(-widthStretched, 5.0f-heightHalf),
                                          bezierP3);
                            path.moveTo(bezierP3);
                            path.close();
                            ofMesh mesh = path.getTessellation();
                            
                            for (auto & v: mesh.getVertices()) {
                                
                                mesh.addNormal(ofVec3f(0.0f, 0.0f, 1.0f));
                                mesh.addTexCoord(ofVec2f((v.x + elem.args[0]*0.5f) / elem.args[0],
                                                         (v.y + elem.args[1]*0.5f) / elem.args[1]));
                            }
                            t_ofeliaLoadMesh2d::meshData[pos].mesh.append(mesh);
                            x->primMode = OF_PRIMITIVE_TRIANGLES;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_MOON:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofPath path;
                            const float radiusX = elem.args[0] * 0.5f;
                            const float radiusY = elem.args[1] * 0.5f;
                            ofPolyline polyline;
                            polyline.arc(ofVec2f(0.0f, 0.0f),
                                         elem.args[2], radiusY,
                                         -90.0f, 90.0f, true, static_cast<int>(elem.args[3]));
                            polyline.arc(ofVec2f(0.0f, 0.0f),
                                         radiusX, radiusY,
                                         90.0f, -90.0f, false, static_cast<int>(elem.args[3]));
                            path.moveTo(polyline.getVertices()[0]);
                            
                            for (size_t i=1; i<polyline.getVertices().size(); ++i)
                                path.lineTo(polyline.getVertices()[i]);
                            path.close();
                            ofMesh mesh = path.getTessellation();
                            
                            for (auto & v: mesh.getVertices()) {
                                
                                mesh.addNormal(ofVec3f(0.0f, 0.0f, 1.0f));
                                mesh.addTexCoord(ofVec2f((v.x + elem.args[0]*0.5f) / elem.args[0],
                                                         (v.y + elem.args[1]*0.5f) / elem.args[1]));
                            }
                            t_ofeliaLoadMesh2d::meshData[pos].mesh.append(mesh);
                            x->primMode = OF_PRIMITIVE_TRIANGLES;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_REGPOLYGON:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofPath path;
                            const float radius = elem.args[0];
                            const int numSides = static_cast<int>(elem.args[1]);
                            const float divAngle = 360.0f / numSides;
                            const float beginAngle = 90.0f - divAngle * 0.5f;
                            float posX, posY;
                            getCoordinatesFromRadAngle(posX, posY, radius, beginAngle);
                            path.moveTo(ofVec2f(posX, posY));
                            
                            for (int i=1; i<numSides; ++i) {
                                
                                getCoordinatesFromRadAngle(posX, posY, radius, beginAngle + divAngle*i);
                                path.lineTo(ofVec2f(posX, posY));
                            }
                            path.close();
                            ofMesh mesh = path.getTessellation();
                            
                            for (auto & v: mesh.getVertices()) {
                                
                                mesh.addNormal(ofVec3f(0.0f, 0.0f, 1.0f));
                                mesh.addTexCoord(ofVec2f((v.x + elem.args[0]) / (elem.args[0]*2.0f),
                                                         (v.y + elem.args[0]) / (elem.args[0]*2.0f)));
                            }
                            t_ofeliaLoadMesh2d::meshData[pos].mesh.append(mesh);
                            x->primMode = OF_PRIMITIVE_TRIANGLES;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_STAR:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofPath path;
                            const int sideNum = static_cast<int>(elem.args[2]) * 2;
                            const float divAngle = 360.0f / sideNum;
                            const float radius[] = {elem.args[0], elem.args[1]};
                            const float beginAngle = 90.0f - divAngle;
                            float posX, posY;
                            getCoordinatesFromRadAngle(posX, posY, radius[0], beginAngle);
                            path.moveTo(ofVec2f(posX, posY));
                            
                            for (int i=1; i<sideNum; ++i) {
                                
                                getCoordinatesFromRadAngle(posX, posY, radius[i%2], beginAngle + divAngle*i);
                                path.lineTo(ofVec2f(posX, posY));
                            }
                            path.close();
                            ofMesh mesh = path.getTessellation();
                            
                            for (auto & v: mesh.getVertices()) {
                                
                                mesh.addNormal(ofVec3f(0.0f, 0.0f, 1.0f));
                                mesh.addTexCoord(ofVec2f((v.x + elem.args[0]) / (elem.args[0]*2.0f),
                                                         (v.y + elem.args[0]) / (elem.args[0]*2.0f)));
                            }
                            t_ofeliaLoadMesh2d::meshData[pos].mesh.append(mesh);
                            x->primMode = OF_PRIMITIVE_TRIANGLES;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_BOX:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofMesh mesh(ofMesh::box(elem.args[0],
                                                    elem.args[1],
                                                    elem.args[2],
                                                    static_cast<int>(elem.args[3]),
                                                    static_cast<int>(elem.args[4]),
                                                    static_cast<int>(elem.args[5])));
                            t_ofeliaLoadMesh2d::meshData[pos].mesh.append(mesh);
                            x->primMode = mesh.getMode();
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_CONE:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofMesh mesh(ofMesh::cone(elem.args[0],
                                                     elem.args[1],
                                                     static_cast<int>(elem.args[2]),
                                                     static_cast<int>(elem.args[3]),
                                                     static_cast<int>(elem.args[4]),
                                                     static_cast<ofPrimitiveMode>(static_cast<int>(elem.args[5]))));
                            t_ofeliaLoadMesh2d::meshData[pos].mesh.append(mesh);
                            x->primMode = mesh.getMode();
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_CYLINDER:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofMesh mesh(ofMesh::cylinder(elem.args[0],
                                                         elem.args[1],
                                                         static_cast<int>(elem.args[2]),
                                                         static_cast<int>(elem.args[3]),
                                                         static_cast<int>(elem.args[4]),
                                                         static_cast<bool>(elem.args[5]),
                                                         static_cast<ofPrimitiveMode>(static_cast<int>(elem.args[6]))));
                            t_ofeliaLoadMesh2d::meshData[pos].mesh.append(mesh);
                            x->primMode = mesh.getMode();
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_ICOSPHERE:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofMesh mesh(ofMesh::icosphere(elem.args[0],
                                                          static_cast<size_t>(ofClamp(elem.args[1], 0.0f, 5.0f))));
                            t_ofeliaLoadMesh2d::meshData[pos].mesh.append(mesh);
                            x->primMode = mesh.getMode();
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_PLANE:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofMesh mesh(ofMesh::plane(elem.args[0],
                                                      elem.args[1],
                                                      static_cast<int>(elem.args[2]),
                                                      static_cast<int>(elem.args[3]),
                                                      static_cast<ofPrimitiveMode>(static_cast<int>(elem.args[4]))));
                            t_ofeliaLoadMesh2d::meshData[pos].mesh.append(mesh);
                            x->primMode = mesh.getMode();
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_SPHERE:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofMesh mesh(ofMesh::sphere(elem.args[0],
                                                       static_cast<int>(elem.args[1]),
                                                       static_cast<ofPrimitiveMode>(static_cast<int>(elem.args[2]))));
                            t_ofeliaLoadMesh2d::meshData[pos].mesh.append(mesh);
                            x->primMode = mesh.getMode();
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
        if (x->cmdVec.size() > cmdVecSize) {
            
            x->cmdVec.erase(x->cmdVec.begin(), x->cmdVec.begin() + cmdVecSize);
            return;
        }
        x->cmdVec.clear();
        x->shouldOutlet = true;
    }
    if (x->shouldOutlet) {
        
        t_ofeliaLoadMesh2d::meshData[pos].mesh.setMode(x->primMode);
        t_ofeliaLoadMesh2d::meshes[pos]->operator=(t_ofeliaLoadMesh2d::meshData[pos].mesh);
        t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = true;
        
        /* OFELIANOTE : trick to update color to vbo (temporary hack) */
        t_ofeliaLoadMesh2d::meshes[pos]->addVertex(ofVec2f(0.0f, 0.0f));
        t_ofeliaLoadMesh2d::meshes[pos]->draw();
        t_ofeliaLoadMesh2d::meshes[pos]->getVertices().pop_back();
        
        /* output current sizes */
        x->numVertices = static_cast<int>(t_ofeliaLoadMesh2d::meshData[pos].mesh.getNumVertices());
        x->numIndices = static_cast<int>(t_ofeliaLoadMesh2d::meshData[pos].mesh.getNumIndices());
        x->numNormals = static_cast<int>(t_ofeliaLoadMesh2d::meshData[pos].mesh.getNumNormals());
        x->numTexCoords = static_cast<int>(t_ofeliaLoadMesh2d::meshData[pos].mesh.getNumTexCoords());
        x->numColors = static_cast<int>(t_ofeliaLoadMesh2d::meshData[pos].mesh.getNumColors());
        ofeliaLoadMesh2d_vecSizeOut(x);
        x->shouldOutlet = false;
    }
}

void ofeliaLoadMesh2d_exit(t_ofeliaLoadMesh2d *x)
{
    if (t_ofeliaLoadMesh2d::bInited)
        t_ofeliaLoadMesh2d::bInited = false;
    x->bInitGate = true;
    x->shouldOutlet = true;
}

void ofeliaLoadMesh2d_vecSizeOut(t_ofeliaLoadMesh2d *x)
{
    outlet_float(x->numColors_out, static_cast<t_float>(x->numColors));
    outlet_float(x->numTexCoords_out, static_cast<t_float>(x->numTexCoords));
    outlet_float(x->numNormals_out, static_cast<t_float>(x->numNormals));
    outlet_float(x->numIndices_out, static_cast<t_float>(x->numIndices));
    outlet_float(x->numVertices_out, static_cast<t_float>(x->numVertices));
}

void ofeliaLoadMesh2d_name(t_ofeliaLoadMesh2d *x, t_symbol *s)
{
    const int pos = getPositionByMesh2dObjID(x->objID);
    t_ofeliaLoadMesh2d::meshData[pos].varName.name = s->s_name;
    getVarNameLocalized(t_ofeliaLoadMesh2d::meshData[pos].varName);
}

void ofeliaLoadMesh2d_primMode(t_ofeliaLoadMesh2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (getPrimModeFromArgs(argc, argv, x->primMode, t_ofeliaLoadMesh2d::objName))
        x->shouldOutlet = true;
}

void ofeliaLoadMesh2d_load(t_ofeliaLoadMesh2d *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaLoadMesh2dElem> elems;
    
    if (getLoadMesh2dElemsFromArgs(argc, argv, elems)) {
        
        ofeliaLoadMesh2d_clear(x, 0, 0, 0);
        
        for (size_t i=0; i<elems.size(); ++i) {
            
            t_ofeliaLoadMesh2dCmdData cmd;
            cmd.elem = elems[i];
            cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
            cmd.state = MESH_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadMesh2d_add(t_ofeliaLoadMesh2d *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadMesh2dCmdData cmd;
    
    if (getLoadMesh2dElemFromArgs(argc, argv, cmd.elem)) {
        
        cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
        cmd.state = MESH_LOAD_CMD_INSERT;
        x->cmdVec.push_back(cmd);
    }
}

void ofeliaLoadMesh2d_append(t_ofeliaLoadMesh2d *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaLoadMesh2dElem> elems;
    
    if (getLoadMesh2dElemsFromArgs(argc, argv, elems)) {
        
        for (size_t i=0; i<elems.size(); ++i) {
            
            t_ofeliaLoadMesh2dCmdData cmd;
            cmd.elem = elems[i];
            cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
            cmd.state = MESH_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadMesh2d_prepend(t_ofeliaLoadMesh2d *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaLoadMesh2dElem> elems;
    
    if (getLoadMesh2dModElemsFromArgs(argc, argv, elems)) {
        
        for (size_t i=0; i<elems.size(); ++i) {
            
            t_ofeliaLoadMesh2dCmdData cmd;
            cmd.elem = elems[i];
            cmd.fromIndex = cmd.toIndex = static_cast<t_float>(i);
            cmd.state = MESH_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadMesh2d_insert(t_ofeliaLoadMesh2d *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadMesh2dCmdData cmd;
    
    if (getLoadMesh2dModElemFromArgs(argc, argv, cmd.elem)) {
        
        const int ac = cmd.elem.numArgs + 1;
        
        if (getCmdRangeFromArgs(argc-ac, argv+ac, cmd)) {
            
            cmd.state = MESH_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadMesh2d_fill(t_ofeliaLoadMesh2d *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadMesh2dCmdData cmd;
    
    if (getLoadMesh2dModElemFromArgs(argc, argv, cmd.elem)) {
        
        const int ac = cmd.elem.numArgs + 1;
        
        if (getCmdRangeFromArgs(argc-ac, argv+ac, cmd)) {
            
            cmd.state = MESH_LOAD_CMD_FILL;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadMesh2d_erase(t_ofeliaLoadMesh2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            t_ofeliaLoadMesh2dCmdData cmd;
            
            if (setLoadMesh2dModElemType(argv[0].a_w.w_symbol->s_name, cmd.elem)) {
                
                if (getCmdRangeFromArgs(argc-1, argv+1, cmd)) {
                    
                    cmd.state = MESH_LOAD_CMD_ERASE;
                    x->cmdVec.push_back(cmd);
                }
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadMesh2d::objName);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadMesh2d::objName);
    }
}

void ofeliaLoadMesh2d_clear(t_ofeliaLoadMesh2d *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<LoadMeshCmdType> types;
    
    if (!argc) {
        
        types = {MESH_LOAD_CMDTYPE_VERTEX,
            MESH_LOAD_CMDTYPE_INDEX,
            MESH_LOAD_CMDTYPE_NORMAL,
            MESH_LOAD_CMDTYPE_TEXCOORD,
            MESH_LOAD_CMDTYPE_COLOR};
    }
    else {
        
        for (int i=0; i<argc; ++i) {
            
            if (argv[i].a_type == A_SYMBOL) {
                
                t_ofeliaLoadMesh2dElem elem;
                
                if (setLoadMesh2dModElemType(argv[i].a_w.w_symbol->s_name, elem))
                    types.push_back(elem.type);
                else
                    return;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaLoadMesh2d::objName);
                return;
            }
        }
    }
    for (size_t i=0; i<types.size(); ++i) {
        
        t_ofeliaLoadMesh2dCmdData cmd;
        cmd.elem.type = types[i];
        cmd.fromIndex = 0.0f;
        cmd.toIndex = numeric_limits<t_float>::max();
        cmd.state = MESH_LOAD_CMD_ERASE;
        x->cmdVec.push_back(cmd);
    }
}

void ofeliaLoadMesh2d_set(t_ofeliaLoadMesh2d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            ofeliaLoadMesh2d_name(x, argv[0].a_w.w_symbol);
            
            if (--argc)
                ofeliaLoadMesh2d_load(x, 0, argc, argv+1);
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadMesh2d::objName);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadMesh2d::objName);
    }
}

void ofeliaLoadMesh2d_print(t_ofeliaLoadMesh2d *x)
{
    post("\n[%s]", t_ofeliaLoadMesh2d::objName);
    const int pos = getPositionByMesh2dObjID(x->objID);
    post("name : %s", t_ofeliaLoadMesh2d::meshData[pos].varName.name.c_str());
    printPrimMode(x->primMode);
    post("load : ");
    
    for (size_t i=0; i<t_ofeliaLoadMesh2d::meshData[pos].mesh.getNumVertices(); ++i) {
        
        const ofVec2f &vertex = t_ofeliaLoadMesh2d::meshData[pos].mesh.getVertex(i);
        post(" vertex %g %g", vertex.x, vertex.y);
    }
    for (size_t i=0; i<t_ofeliaLoadMesh2d::meshData[pos].mesh.getNumIndices(); ++i) {
        
        const ofIndexType index = t_ofeliaLoadMesh2d::meshData[pos].mesh.getIndex(i);
        post(" index %d", index);
    }
    for (size_t i=0; i<t_ofeliaLoadMesh2d::meshData[pos].mesh.getNumNormals(); ++i) {
        
        const ofVec2f &normal = t_ofeliaLoadMesh2d::meshData[pos].mesh.getNormal(i);
        post(" normal %g %g", normal.x, normal.y);
    }
    for (size_t i=0; i<t_ofeliaLoadMesh2d::meshData[pos].mesh.getNumTexCoords(); ++i) {
        
        const ofVec2f &texCoord = t_ofeliaLoadMesh2d::meshData[pos].mesh.getTexCoord(i);
        post(" texCoord %g %g", texCoord.x, texCoord.y);
    }
    for (size_t i=0; i<t_ofeliaLoadMesh2d::meshData[pos].mesh.getNumColors(); ++i) {
        
        const ofColor &color = t_ofeliaLoadMesh2d::meshData[pos].mesh.getColor(i);
        post(" color %d %d %d %d", color.r, color.g, color.b, color.a);
    }
}

void ofeliaLoadMesh2d_free(t_ofeliaLoadMesh2d *x)
{
    const int pos = getPositionByMesh2dObjID(x->objID);
    t_ofeliaLoadMesh2d::meshData.erase(t_ofeliaLoadMesh2d::meshData.begin() + pos);
    t_ofeliaLoadMesh2d::meshes.erase(t_ofeliaLoadMesh2d::meshes.begin() + pos);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::updateSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    outlet_free(x->numVertices_out);
    outlet_free(x->numIndices_out);
    outlet_free(x->numNormals_out);
    outlet_free(x->numTexCoords_out);
    outlet_free(x->numColors_out);
}

void ofeliaLoadMesh2d_setup()
{
    ofeliaLoadMesh2d_class = class_new(gensym("ofLoadMesh2d"),
                                       reinterpret_cast<t_newmethod>(ofeliaLoadMesh2d_new),
                                       reinterpret_cast<t_method>(ofeliaLoadMesh2d_free),
                                       sizeof(t_ofeliaLoadMesh2d),
                                       CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaLoadMesh2d_class, reinterpret_cast<t_method>(ofeliaLoadMesh2d_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaLoadMesh2d_class, reinterpret_cast<t_method>(ofeliaLoadMesh2d_update),
                    t_ofeliaWindow::updateMess, A_NULL, 0);
    class_addmethod(ofeliaLoadMesh2d_class, reinterpret_cast<t_method>(ofeliaLoadMesh2d_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addmethod(ofeliaLoadMesh2d_class, reinterpret_cast<t_method>(ofeliaLoadMesh2d_name),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaLoadMesh2d_class, reinterpret_cast<t_method>(ofeliaLoadMesh2d_primMode),
                    gensym("primMode"), A_GIMME, 0);
    class_addmethod(ofeliaLoadMesh2d_class, reinterpret_cast<t_method>(ofeliaLoadMesh2d_load),
                    gensym("load"), A_GIMME, 0);
    class_addmethod(ofeliaLoadMesh2d_class, reinterpret_cast<t_method>(ofeliaLoadMesh2d_add),
                    gensym("add"), A_GIMME, 0);
    class_addmethod(ofeliaLoadMesh2d_class, reinterpret_cast<t_method>(ofeliaLoadMesh2d_append),
                    gensym("append"), A_GIMME, 0);
    class_addmethod(ofeliaLoadMesh2d_class, reinterpret_cast<t_method>(ofeliaLoadMesh2d_prepend),
                    gensym("prepend"), A_GIMME, 0);
    class_addmethod(ofeliaLoadMesh2d_class, reinterpret_cast<t_method>(ofeliaLoadMesh2d_insert),
                    gensym("insert"), A_GIMME, 0);
    class_addmethod(ofeliaLoadMesh2d_class, reinterpret_cast<t_method>(ofeliaLoadMesh2d_fill),
                    gensym("fill"), A_GIMME, 0);
    class_addmethod(ofeliaLoadMesh2d_class, reinterpret_cast<t_method>(ofeliaLoadMesh2d_erase),
                    gensym("erase"), A_GIMME, 0);
    class_addmethod(ofeliaLoadMesh2d_class, reinterpret_cast<t_method>(ofeliaLoadMesh2d_clear),
                    gensym("clear"), A_GIMME, 0);
    class_addmethod(ofeliaLoadMesh2d_class, reinterpret_cast<t_method>(ofeliaLoadMesh2d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaLoadMesh2d_class, reinterpret_cast<t_method>(ofeliaLoadMesh2d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofLoadMesh3d object methods
 */
void ofeliaLoadMesh3d_clear(t_ofeliaLoadMesh3d *x, t_symbol *s, int argc, t_atom *argv);
void ofeliaLoadMesh3d_vecSizeOut(t_ofeliaLoadMesh3d *x);

int getPositionByMesh3dObjID(const unsigned int objID)
{
    const auto it = find_if(begin(t_ofeliaLoadMesh3d::meshData), end(t_ofeliaLoadMesh3d::meshData),
                            [&objID](const t_ofeliaLoadMesh3dData &meshData)
                            {
                                return meshData.objID == objID;
                            });
    return static_cast<int>(it - t_ofeliaLoadMesh3d::meshData.begin());
}

bool setLoadMesh3dElemType(const char *name, t_ofeliaLoadMesh3dElem &elem)
{
    if (!strcmp(name, "vertex"))  {
        
        elem.type = MESH_LOAD_CMDTYPE_VERTEX;
        elem.numArgs = 3;
    }
    else if (!strcmp(name, "index")) {
        
        elem.type = MESH_LOAD_CMDTYPE_INDEX;
        elem.numArgs = 1;
    }
    else if (!strcmp(name, "normal")) {
        
        elem.type = MESH_LOAD_CMDTYPE_NORMAL;
        elem.numArgs = 3;
    }
    else if (!strcmp(name, "texCoord")) {
        
        elem.type = MESH_LOAD_CMDTYPE_TEXCOORD;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "color")) {
        
        elem.type = MESH_LOAD_CMDTYPE_COLOR;
        elem.numArgs = 4;
    }
    else if (!strcmp(name, "circle")) {
        
        elem.type = MESH_LOAD_CMDTYPE_CIRCLE;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "ellipse")) {
        
        elem.type = MESH_LOAD_CMDTYPE_ELLIPSE;
        elem.numArgs = 3;
    }
    else if (!strcmp(name, "sector")) {
        
        elem.type = MESH_LOAD_CMDTYPE_SECTOR;
        elem.numArgs = 5;
    }
    else if (!strcmp(name, "eqTriangle") || !strcmp(name, "eqTri")) {
        
        elem.type = MESH_LOAD_CMDTYPE_EQTRIANGLE;
        elem.numArgs = 1;
    }
    else if (!strcmp(name, "isoTriangle") || !strcmp(name, "isoTri")) {
        
        elem.type = MESH_LOAD_CMDTYPE_ISOTRIANGLE;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "square")) {
        
        elem.type = MESH_LOAD_CMDTYPE_SQUARE;
        elem.numArgs = 1;
    }
    else if (!strcmp(name, "rectangle") || !strcmp(name, "rect")) {
        
        elem.type = MESH_LOAD_CMDTYPE_RECTANGLE;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "rectRounded")) {
        
        elem.type = MESH_LOAD_CMDTYPE_RECTROUNDED;
        elem.numArgs = 4;
    }
    else if (!strcmp(name, "rectRounded4")) {
        
        elem.type = MESH_LOAD_CMDTYPE_RECTROUNDED4;
        elem.numArgs = 7;
    }
    else if (!strcmp(name, "cross")) {
        
        elem.type = MESH_LOAD_CMDTYPE_CROSS;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "heart")) {
        
        elem.type = MESH_LOAD_CMDTYPE_HEART;
        elem.numArgs = 3;
    }
    else if (!strcmp(name, "moon")) {
        
        elem.type = MESH_LOAD_CMDTYPE_MOON;
        elem.numArgs = 4;
    }
    else if (!strcmp(name, "regPolygon")) {
        
        elem.type = MESH_LOAD_CMDTYPE_REGPOLYGON;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "star")) {
        
        elem.type = MESH_LOAD_CMDTYPE_STAR;
        elem.numArgs = 3;
    }
    else if (!strcmp(name, "box")) {
        
        elem.type = MESH_LOAD_CMDTYPE_BOX;
        elem.numArgs = 6;
    }
    else if (!strcmp(name, "cone")) {
        
        elem.type = MESH_LOAD_CMDTYPE_CONE;
        elem.numArgs = 6;
    }
    else if (!strcmp(name, "cylinder")) {
        
        elem.type = MESH_LOAD_CMDTYPE_CYLINDER;
        elem.numArgs = 7;
    }
    else if (!strcmp(name, "icosphere")) {
        
        elem.type = MESH_LOAD_CMDTYPE_ICOSPHERE;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "plane")) {
        
        elem.type = MESH_LOAD_CMDTYPE_PLANE;
        elem.numArgs = 5;
    }
    else if (!strcmp(name, "sphere")) {
        
        elem.type = MESH_LOAD_CMDTYPE_SPHERE;
        elem.numArgs = 3;
    }
    else {
        
        error("%s: no method for '%s'", t_ofeliaLoadMesh3d::objName, name);
        return 0;
    }
    return 1;
}

bool setLoadMesh3dModElemType(const char *name, t_ofeliaLoadMesh3dElem &elem)
{
    if (!strcmp(name, "vertex"))  {
        
        elem.type = MESH_LOAD_CMDTYPE_VERTEX;
        elem.numArgs = 3;
    }
    else if (!strcmp(name, "index")) {
        
        elem.type = MESH_LOAD_CMDTYPE_INDEX;
        elem.numArgs = 1;
    }
    else if (!strcmp(name, "normal")) {
        
        elem.type = MESH_LOAD_CMDTYPE_NORMAL;
        elem.numArgs = 3;
    }
    else if (!strcmp(name, "texCoord")) {
        
        elem.type = MESH_LOAD_CMDTYPE_TEXCOORD;
        elem.numArgs = 2;
    }
    else if (!strcmp(name, "color")) {
        
        elem.type = MESH_LOAD_CMDTYPE_COLOR;
        elem.numArgs = 4;
    }
    else if (!strcmp(name, "circle") ||
             !strcmp(name, "ellipse") ||
             !strcmp(name, "sector") ||
             !strcmp(name, "eqTriangle") ||
             !strcmp(name, "eqTri") ||
             !strcmp(name, "isoTriangle") ||
             !strcmp(name, "isoTri") ||
             !strcmp(name, "square") ||
             !strcmp(name, "rectangle") ||
             !strcmp(name, "rectRounded") ||
             !strcmp(name, "rectRounded4") ||
             !strcmp(name, "cross") ||
             !strcmp(name, "heart") ||
             !strcmp(name, "moon") ||
             !strcmp(name, "regPolygon") ||
             !strcmp(name, "star") ||
             !strcmp(name, "box") ||
             !strcmp(name, "cone") ||
             !strcmp(name, "cylinder") ||
             !strcmp(name, "icosahedron") ||
             !strcmp(name, "icosphere") ||
             !strcmp(name, "plane") ||
             !strcmp(name, "sphere")) {
        
        error("%s: '%s' can only be used with 'load', 'add' or 'append'", t_ofeliaLoadMesh3d::objName, name);
        return 0;
    }
    else {
        
        error("%s: no method for '%s'", t_ofeliaLoadMesh3d::objName, name);
        return 0;
    }
    return 1;
}

bool getLoadMesh3dElemFromArgs(const int argc, const t_atom *argv, t_ofeliaLoadMesh3dElem &elem)
{
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            const char *name = argv[0].a_w.w_symbol->s_name;
            
            if (!setLoadMesh3dElemType(name, elem))
                return 0;
            
            if (elem.numArgs + 1 > argc) {
                
                error("Bad arguments for message '%s' to object '%s'", name, t_ofeliaLoadMesh3d::objName);
                return 0;
            }
            for (int i=0; i<elem.numArgs; ++i) {
                
                const int index = i+1;
                
                if (argv[index].a_type == A_FLOAT) {
                    
                    elem.args[i] = argv[index].a_w.w_float;
                }
                else {
                    
                    error("Bad arguments for message '%s' to object '%s'", name, t_ofeliaLoadMesh3d::objName);
                    return 0;
                }
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadMesh3d::objName);
            return 0;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadMesh3d::objName);
        return 0;
    }
    return 1;
}

bool getLoadMesh3dModElemFromArgs(const int argc, const t_atom *argv, t_ofeliaLoadMesh3dElem &elem)
{
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            const char *name = argv[0].a_w.w_symbol->s_name;
            
            if (!setLoadMesh3dModElemType(name, elem))
                return 0;
            
            if (elem.numArgs + 1 > argc) {
                
                error("Bad arguments for message '%s' to object '%s'", name, t_ofeliaLoadMesh3d::objName);
                return 0;
            }
            for (int i=0; i<elem.numArgs; ++i) {
                
                const int index = i+1;
                
                if (argv[index].a_type == A_FLOAT) {
                    
                    elem.args[i] = argv[index].a_w.w_float;
                }
                else {
                    
                    error("Bad arguments for message '%s' to object '%s'", name, t_ofeliaLoadMesh3d::objName);
                    return 0;
                }
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadMesh3d::objName);
            return 0;
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadMesh3d::objName);
        return 0;
    }
    return 1;
}

bool getLoadMesh3dElemsFromArgs(const int argc, const t_atom *argv, vector<t_ofeliaLoadMesh3dElem> &elems)
{
    t_string name;
    t_ofeliaLoadMesh3dElem elem;
    
    for (int i=0; i<argc; i+=elem.numArgs) {
        
        if (argv[i].a_type == A_SYMBOL) {
            
            name = argv[i].a_w.w_symbol->s_name;
            
            if (!setLoadMesh3dElemType(name.c_str(), elem))
                return 0;
            ++i;
        }
        if (name.empty()) {
            
            error("%s: wrong argument type", t_ofeliaLoadMesh3d::objName);
            return 0;
        }
        if (i+elem.numArgs > argc) {
            
            error("Bad arguments for message '%s' to object '%s'", name.c_str(), t_ofeliaLoadMesh3d::objName);
            return 0;
        }
        for (int j=0; j<elem.numArgs; ++j) {
            
            const int index = i+j;
            
            if (argv[index].a_type == A_FLOAT) {
                
                elem.args[j] = argv[index].a_w.w_float;
            }
            else {
                
                error("Bad arguments for message '%s' to object '%s'", name.c_str(), t_ofeliaLoadMesh3d::objName);
                return 0;
            }
        }
        elems.push_back(elem);
    }
    return 1;
}

bool getLoadMesh3dModElemsFromArgs(const int argc, const t_atom *argv, vector<t_ofeliaLoadMesh3dElem> &elems)
{
    t_string name;
    t_ofeliaLoadMesh3dElem elem;
    
    for (int i=0; i<argc; i+=elem.numArgs) {
        
        if (argv[i].a_type == A_SYMBOL) {
            
            name = argv[i].a_w.w_symbol->s_name;
            
            if (!setLoadMesh3dModElemType(name.c_str(), elem))
                return 0;
            ++i;
        }
        if (name.empty()) {
            
            error("%s: wrong argument type", t_ofeliaLoadMesh3d::objName);
            return 0;
        }
        if (i+elem.numArgs > argc) {
            
            error("Bad arguments for message '%s' to object '%s'", name.c_str(), t_ofeliaLoadMesh3d::objName);
            return 0;
        }
        for (int j=0; j<elem.numArgs; ++j) {
            
            const int index = i+j;
            
            if (argv[index].a_type == A_FLOAT) {
                
                elem.args[j] = argv[index].a_w.w_float;
            }
            else {
                
                error("Bad arguments for message '%s' to object '%s'", name.c_str(), t_ofeliaLoadMesh3d::objName);
                return 0;
            }
        }
        elems.push_back(elem);
    }
    return 1;
}

bool getCmdRangeFromArgs(const int argc, const t_atom *argv, t_ofeliaLoadMesh3dCmdData &cmd)
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
                
                error("%s: wrong argument type", t_ofeliaLoadMesh3d::objName);
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
                
                error("%s: wrong argument type", t_ofeliaLoadMesh3d::objName);
                return 0;
            }
            break;
        }
        default:
        {
            error("%s: invalid number of arguments", t_ofeliaLoadMesh3d::objName);
            return 0;
        }
    }
    return 1;
}

void *ofeliaLoadMesh3d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadMesh3d *x = reinterpret_cast<t_ofeliaLoadMesh3d*>(pd_new(ofeliaLoadMesh3d_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    t_ofeliaLoadMesh3dData meshData;
    getVarNameLocalPrefixes(meshData.varName);
    vector<t_ofeliaLoadMesh3dElem> elems;
    
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            meshData.varName.name = argv[0].a_w.w_symbol->s_name;
            getVarNameLocalized(meshData.varName);
            
            if (--argc) {
                
                if (!getLoadMesh3dElemsFromArgs(argc, argv+1, elems))
                    return 0;
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadMesh3d::objName);
            return 0;
        }
    }
    x->shouldOutlet = false;
    x->bInitGate = false;
    x->primMode = OF_PRIMITIVE_TRIANGLES;
    x->numVertices = 0;
    x->numIndices = 0;
    x->numNormals = 0;
    x->numTexCoords = 0;
    x->numColors = 0;
    x->objID = meshData.objID = t_ofeliaLoadMesh3d::counter++;
    t_ofeliaLoadMesh3d::meshData.push_back(meshData);
    t_ofeliaLoadMesh3d::meshes.push_back(make_unique<ofVboMesh>());
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::updateSym);
    pd_bind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    x->numVertices_out = outlet_new(&x->x_obj, &s_float);
    x->numIndices_out = outlet_new(&x->x_obj, &s_float);
    x->numNormals_out = outlet_new(&x->x_obj, &s_float);
    x->numTexCoords_out = outlet_new(&x->x_obj, &s_float);
    x->numColors_out = outlet_new(&x->x_obj, &s_float);
    
    if (argc) {
        
        ofeliaLoadMesh3d_clear(x, 0, 0, 0);
        
        for (size_t i=0; i<elems.size(); ++i) {
            
            t_ofeliaLoadMesh3dCmdData cmd;
            cmd.elem = elems[i];
            cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
            cmd.state = MESH_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
    return (x);
}

void ofeliaLoadMesh3d_init(t_ofeliaLoadMesh3d *x)
{
    if (!t_ofeliaLoadMesh3d::bInited && x->bInitGate) {
        
        t_ofeliaLoadMesh3d::meshes.clear();
        
        for (size_t i=0; i<t_ofeliaLoadMesh3d::meshData.size(); ++i)
            t_ofeliaLoadMesh3d::meshes.push_back(make_unique<ofVboMesh>());
        t_ofeliaLoadMesh3d::bInited = true;
    }
}

void ofeliaLoadMesh3d_update(t_ofeliaLoadMesh3d *x)
{
    const int pos = getPositionByMesh3dObjID(x->objID);
    
    if (!x->cmdVec.empty()) {
        
        const size_t cmdVecSize = x->cmdVec.size();
        
        /* read all recorded commands */
        for (size_t i=0; i<cmdVecSize; ++i) {
            
            const t_ofeliaLoadMesh3dElem &elem = x->cmdVec[i].elem;
            
            switch (elem.type) {
                    
                case MESH_LOAD_CMDTYPE_VERTEX:
                {
                    const int numVertices = static_cast<int>(t_ofeliaLoadMesh3d::meshData[pos].mesh.getNumVertices());
                    const auto verticesBegin = t_ofeliaLoadMesh3d::meshData[pos].mesh.getVertices().begin();
                    
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, numVertices));
                            const int insertSize = static_cast<int>(max(0.0f, max(0.0f, x->cmdVec[i].toIndex) - max(0.0f, x->cmdVec[i].fromIndex))) + 1;
                            t_ofeliaLoadMesh3d::meshData[pos].mesh.getVertices().insert(verticesBegin + fromIndex,
                                                                                        insertSize,
                                                                                        ofVec3f(elem.args[0],
                                                                                                elem.args[1],
                                                                                                elem.args[2]));
                            break;
                        }
                        case MESH_LOAD_CMD_ERASE:
                        {
                            if (numVertices) {
                                
                                const int maxIndex = numVertices - 1;
                                const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, maxIndex));
                                const int toIndex = static_cast<int>(ofClamp(x->cmdVec[i].toIndex, fromIndex, maxIndex)) + 1;
                                t_ofeliaLoadMesh3d::meshData[pos].mesh.getVertices().erase(verticesBegin + fromIndex,
                                                                                           verticesBegin + toIndex);
                            }
                            break;
                        }
                        case MESH_LOAD_CMD_FILL:
                        {
                            if (numVertices) {
                                
                                const int maxIndex = numVertices - 1;
                                const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, maxIndex));
                                const int toIndex = static_cast<int>(ofClamp(x->cmdVec[i].toIndex, fromIndex, maxIndex)) + 1;
                                fill(verticesBegin + fromIndex, verticesBegin + toIndex, ofVec3f(elem.args[0],
                                                                                                 elem.args[1],
                                                                                                 elem.args[2]));
                            }
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_INDEX:
                {
                    const int numIndices = static_cast<int>(t_ofeliaLoadMesh3d::meshData[pos].mesh.getNumIndices());
                    const auto indicesBegin = t_ofeliaLoadMesh3d::meshData[pos].mesh.getIndices().begin();
                    
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, numIndices));
                            const int insertSize = static_cast<int>(max(0.0f, max(0.0f, x->cmdVec[i].toIndex) - max(0.0f, x->cmdVec[i].fromIndex))) + 1;
                            t_ofeliaLoadMesh3d::meshData[pos].mesh.getIndices().insert(indicesBegin + fromIndex,
                                                                                       insertSize,
                                                                                       static_cast<ofIndexType>
                                                                                       (elem.args[0]));
                            break;
                        }
                        case MESH_LOAD_CMD_ERASE:
                        {
                            if (numIndices) {
                                
                                const int maxIndex = numIndices - 1;
                                const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, maxIndex));
                                const int toIndex = static_cast<int>(ofClamp(x->cmdVec[i].toIndex, fromIndex, maxIndex)) + 1;
                                t_ofeliaLoadMesh3d::meshData[pos].mesh.getIndices().erase(indicesBegin + fromIndex,
                                                                                          indicesBegin + toIndex);
                            }
                            break;
                        }
                        case MESH_LOAD_CMD_FILL:
                        {
                            if (numIndices) {
                                
                                const int maxIndex = numIndices - 1;
                                const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, maxIndex));
                                const int toIndex = static_cast<int>(ofClamp(x->cmdVec[i].toIndex, fromIndex, maxIndex)) + 1;
                                fill(indicesBegin + fromIndex, indicesBegin + toIndex,
                                     static_cast<ofIndexType>(elem.args[0]));
                            }
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_NORMAL:
                {
                    const int numNormals = static_cast<int>(t_ofeliaLoadMesh3d::meshData[pos].mesh.getNumNormals());
                    const auto normalsBegin = t_ofeliaLoadMesh3d::meshData[pos].mesh.getNormals().begin();
                    
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, numNormals));
                            const int insertSize = static_cast<int>(max(0.0f, max(0.0f, x->cmdVec[i].toIndex) - max(0.0f, x->cmdVec[i].fromIndex))) + 1;
                            t_ofeliaLoadMesh3d::meshData[pos].mesh.getNormals().insert(normalsBegin + fromIndex,
                                                                                       insertSize,
                                                                                       ofVec3f(elem.args[0],
                                                                                               elem.args[1],
                                                                                               elem.args[2]));
                            break;
                        }
                        case MESH_LOAD_CMD_ERASE:
                        {
                            if (numNormals) {
                                
                                const int maxIndex = numNormals - 1;
                                const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, maxIndex));
                                const int toIndex = static_cast<int>(ofClamp(x->cmdVec[i].toIndex, fromIndex, maxIndex)) + 1;
                                t_ofeliaLoadMesh3d::meshData[pos].mesh.getNormals().erase(normalsBegin + fromIndex,
                                                                                          normalsBegin + toIndex);
                            }
                            break;
                        }
                        case MESH_LOAD_CMD_FILL:
                        {
                            if (numNormals) {
                                
                                const int maxIndex = numNormals - 1;
                                const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, maxIndex));
                                const int toIndex = static_cast<int>(ofClamp(x->cmdVec[i].toIndex, fromIndex, maxIndex)) + 1;
                                fill(normalsBegin + fromIndex, normalsBegin + toIndex, ofVec3f(elem.args[0],
                                                                                               elem.args[1],
                                                                                               elem.args[2]));
                            }
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_TEXCOORD:
                {
                    const int numTexCoords = static_cast<int>(t_ofeliaLoadMesh3d::meshData[pos].mesh.getNumTexCoords());
                    const auto texCoordsBegin = t_ofeliaLoadMesh3d::meshData[pos].mesh.getTexCoords().begin();
                    
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, numTexCoords));
                            const int insertSize = static_cast<int>(max(0.0f, max(0.0f, x->cmdVec[i].toIndex) - max(0.0f, x->cmdVec[i].fromIndex))) + 1;
                            t_ofeliaLoadMesh3d::meshData[pos].mesh.getTexCoords().insert(texCoordsBegin + fromIndex,
                                                                                         insertSize,
                                                                                         ofVec2f(elem.args[0],
                                                                                                 elem.args[1]));
                            break;
                        }
                        case MESH_LOAD_CMD_ERASE:
                        {
                            if (numTexCoords) {
                                
                                const int maxIndex = numTexCoords - 1;
                                const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, maxIndex));
                                const int toIndex = static_cast<int>(ofClamp(x->cmdVec[i].toIndex, fromIndex, maxIndex)) + 1;
                                t_ofeliaLoadMesh3d::meshData[pos].mesh.getTexCoords().erase(texCoordsBegin +
                                                                                            fromIndex,
                                                                                            texCoordsBegin + toIndex);
                            }
                            break;
                        }
                        case MESH_LOAD_CMD_FILL:
                        {
                            if (numTexCoords) {
                                
                                const int maxIndex = numTexCoords - 1;
                                const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, maxIndex));
                                const int toIndex = static_cast<int>(ofClamp(x->cmdVec[i].toIndex, fromIndex, maxIndex)) + 1;
                                fill(texCoordsBegin + fromIndex, texCoordsBegin + toIndex, ofVec2f(elem.args[0],
                                                                                                   elem.args[1]));
                            }
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_COLOR:
                {
                    const int numColors = static_cast<int>(t_ofeliaLoadMesh3d::meshData[pos].mesh.getNumColors());
                    const auto colorsBegin = t_ofeliaLoadMesh3d::meshData[pos].mesh.getColors().begin();
                    
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, numColors));
                            const int insertSize = static_cast<int>(max(0.0f, max(0.0f, x->cmdVec[i].toIndex) - max(0.0f, x->cmdVec[i].fromIndex))) + 1;
                            t_ofeliaLoadMesh3d::meshData[pos].mesh.getColors().insert(colorsBegin + fromIndex,
                                                                                      insertSize,
                                                                                      ofColor(static_cast<unsigned
                                                                                              char>(elem.args[0]),
                                                                                              static_cast<unsigned
                                                                                              char>(elem.args[1]),
                                                                                              static_cast<unsigned
                                                                                              char>(elem.args[2]),
                                                                                              static_cast<unsigned
                                                                                              char>(elem.args[3])));
                            break;
                        }
                        case MESH_LOAD_CMD_ERASE:
                        {
                            if (numColors) {
                                
                                const int maxIndex = numColors - 1;
                                const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, maxIndex));
                                const int toIndex = static_cast<int>(ofClamp(x->cmdVec[i].toIndex, fromIndex, maxIndex)) + 1;
                                t_ofeliaLoadMesh3d::meshData[pos].mesh.getColors().erase(colorsBegin + fromIndex,
                                                                                         colorsBegin + toIndex);
                            }
                            break;
                        }
                        case MESH_LOAD_CMD_FILL:
                        {
                            if (numColors) {
                                
                                const int maxIndex = numColors - 1;
                                const int fromIndex = static_cast<int>(ofClamp(x->cmdVec[i].fromIndex, 0.0f, maxIndex));
                                const int toIndex = static_cast<int>(ofClamp(x->cmdVec[i].toIndex, fromIndex, maxIndex)) + 1;
                                fill(colorsBegin + fromIndex, colorsBegin + toIndex, ofColor(static_cast<unsigned
                                                                                             char>(elem.args[0]),
                                                                                             static_cast<unsigned
                                                                                             char>(elem.args[1]),
                                                                                             static_cast<unsigned
                                                                                             char>(elem.args[2]),
                                                                                             static_cast<unsigned
                                                                                             char>(elem.args[3])));
                            }
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_CIRCLE:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofPath path;
                            ofPolyline polyline;
                            polyline.arc(ofVec2f(0.0f ,0.0f), elem.args[0], elem.args[0],
                                         0.0f, 360.0f, true, static_cast<int>(elem.args[1]));
                            path.moveTo(polyline.getVertices()[0]);
                            
                            for (size_t i=1; i<polyline.getVertices().size(); ++i)
                                path.lineTo(polyline.getVertices()[i]);
                            path.close();
                            ofMesh mesh = path.getTessellation();
                            
                            for (auto & v: mesh.getVertices()) {
                                
                                mesh.addNormal(ofVec3f(0.0f, 0.0f, 1.0f));
                                mesh.addTexCoord(ofVec2f((v.x + elem.args[0]) / (elem.args[0]*2.0f),
                                                         (v.y + elem.args[0]) / (elem.args[0]*2.0f)));
                            }
                            t_ofeliaLoadMesh3d::meshData[pos].mesh.append(mesh);
                            x->primMode = OF_PRIMITIVE_TRIANGLES;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_ELLIPSE:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofPath path;
                            ofPolyline polyline;
                            polyline.arc(ofVec2f(0.0f ,0.0f), elem.args[0]*0.5f, elem.args[1]*0.5f,
                                         0.0f, 360.0f, true, static_cast<int>(elem.args[2]));
                            path.moveTo(polyline.getVertices()[0]);
                            
                            for (size_t i=1; i<polyline.getVertices().size(); ++i)
                                path.lineTo(polyline.getVertices()[i]);
                            path.close();
                            ofMesh mesh = path.getTessellation();
                            
                            for (auto & v: mesh.getVertices()) {
                                
                                mesh.addNormal(ofVec3f(0.0f, 0.0f, 1.0f));
                                mesh.addTexCoord(ofVec2f((v.x + elem.args[0]*0.5f) / elem.args[0],
                                                         (v.y + elem.args[1]*0.5f) / elem.args[1]));
                            }
                            t_ofeliaLoadMesh3d::meshData[pos].mesh.append(mesh);
                            x->primMode = OF_PRIMITIVE_TRIANGLES;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_SECTOR:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofPath path;
                            ofPolyline polyline;
                            polyline.addVertex(ofVec2f(0.0f ,0.0f));
                            polyline.arc(ofVec2f(0.0f ,0.0f),
                                         elem.args[0], elem.args[1],
                                         elem.args[2], elem.args[3],
                                         elem.args[2] < elem.args[3], static_cast<int>(elem.args[4]));
                            path.moveTo(polyline.getVertices()[0]);
                            
                            for (size_t i=1; i<polyline.getVertices().size(); ++i)
                                path.lineTo(polyline.getVertices()[i]);
                            path.close();
                            ofMesh mesh = path.getTessellation();
                            
                            for (auto & v: mesh.getVertices()) {
                                
                                mesh.addNormal(ofVec3f(0.0f, 0.0f, 1.0f));
                                mesh.addTexCoord(ofVec2f((v.x + elem.args[0]) / (elem.args[0]*2.0f),
                                                         (v.y + elem.args[1]) / (elem.args[1]*2.0f)));
                            }
                            t_ofeliaLoadMesh3d::meshData[pos].mesh.append(mesh);
                            x->primMode = OF_PRIMITIVE_TRIANGLES;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_EQTRIANGLE:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofPath path;
                            const float halfSize = elem.args[0] * 0.5f;
                            const float inRadius = elem.args[0] * ofeliaConstants::heightFactor / 3;
                            path.triangle(0.0f, -inRadius*2.0f, 0.0f,
                                          halfSize, inRadius, 0.0f,
                                          -halfSize, inRadius, 0.0f);
                            ofMesh mesh = path.getTessellation();
                            
                            for (auto & v: mesh.getVertices()) {
                                
                                mesh.addNormal(ofVec3f(0.0f, 0.0f, 1.0f));
                                mesh.addTexCoord(ofVec2f((v.x + elem.args[0]*0.5f) / elem.args[0],
                                                         (v.y + elem.args[0]*0.5f) / elem.args[0]));
                            }
                            t_ofeliaLoadMesh3d::meshData[pos].mesh.append(mesh);
                            x->primMode = OF_PRIMITIVE_TRIANGLES;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_ISOTRIANGLE:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofPath path;
                            const float halfWidth = elem.args[0] * 0.5f;
                            const float halfHeight = elem.args[1] * 0.5f;
                            path.triangle(0.0f, -halfHeight, 0.0f,
                                          halfWidth, halfHeight, 0.0f,
                                          -halfWidth, halfHeight, 0.0f);
                            ofMesh mesh = path.getTessellation();
                            
                            for (auto & v: mesh.getVertices()) {
                                
                                mesh.addNormal(ofVec3f(0.0f, 0.0f, 1.0f));
                                mesh.addTexCoord(ofVec2f((v.x + elem.args[0]*0.5f) / elem.args[0],
                                                         (v.y + elem.args[1]*0.5f) / elem.args[1]));
                            }
                            t_ofeliaLoadMesh3d::meshData[pos].mesh.append(mesh);
                            x->primMode = OF_PRIMITIVE_TRIANGLES;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_SQUARE:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofPath path;
                            path.rectangle(0.0f, 0.0f, elem.args[0], elem.args[0]);
                            ofMesh mesh = path.getTessellation();
                            
                            for (auto & v: mesh.getVertices()) {
                                
                                mesh.addNormal(ofVec3f(0.0f, 0.0f, 1.0f));
                                mesh.addTexCoord(ofVec2f(v.x / elem.args[0],
                                                         v.y / elem.args[0]));
                            }
                            t_ofeliaLoadMesh3d::meshData[pos].mesh.append(mesh);
                            x->primMode = OF_PRIMITIVE_TRIANGLES;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_RECTANGLE:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofPath path;
                            path.rectangle(0.0f, 0.0f, elem.args[0], elem.args[1]);
                            ofMesh mesh = path.getTessellation();
                            
                            for (auto & v: mesh.getVertices()) {
                                
                                mesh.addNormal(ofVec3f(0.0f, 0.0f, 1.0f));
                                mesh.addTexCoord(ofVec2f(v.x / elem.args[0],
                                                         v.y / elem.args[1]));
                            }
                            t_ofeliaLoadMesh3d::meshData[pos].mesh.append(mesh);
                            x->primMode = OF_PRIMITIVE_TRIANGLES;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_RECTROUNDED:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofPath path;
                            path.setCircleResolution(static_cast<int>(elem.args[3]));
                            path.rectRounded(0.0f, 0.0f, 0.0f,
                                             elem.args[0], elem.args[1],
                                             elem.args[2], elem.args[2],
                                             elem.args[2], elem.args[2]);
                            ofMesh mesh = path.getTessellation();
                            
                            for (auto & v: mesh.getVertices()) {
                                
                                mesh.addNormal(ofVec3f(0.0f, 0.0f, 1.0f));
                                mesh.addTexCoord(ofVec2f(v.x / elem.args[0],
                                                         v.y / elem.args[1]));
                            }
                            t_ofeliaLoadMesh3d::meshData[pos].mesh.append(mesh);
                            x->primMode = OF_PRIMITIVE_TRIANGLES;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_RECTROUNDED4:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofPath path;
                            path.setCircleResolution(static_cast<int>(elem.args[6]));
                            path.rectRounded(0.0f, 0.0f, 0.0f,
                                             elem.args[0], elem.args[1],
                                             elem.args[2], elem.args[3],
                                             elem.args[4], elem.args[5]);
                            ofMesh mesh = path.getTessellation();
                            
                            for (auto & v: mesh.getVertices()) {
                                
                                mesh.addNormal(ofVec3f(0.0f, 0.0f, 1.0f));
                                mesh.addTexCoord(ofVec2f(v.x / elem.args[0],
                                                         v.y / elem.args[1]));
                            }
                            t_ofeliaLoadMesh3d::meshData[pos].mesh.append(mesh);
                            x->primMode = OF_PRIMITIVE_TRIANGLES;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_CROSS:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofPath path;
                            const float sHalf = elem.args[0] * 0.5f;
                            const float tHalf = elem.args[1] * 0.5f;
                            const float cX_tHalf_add = tHalf;
                            const float cX_tHalf_sub = -tHalf;
                            const float cX_sHalf_add = sHalf;
                            const float cX_sHalf_sub = -sHalf;
                            const float cY_tHalf_add = tHalf;
                            const float cY_tHalf_sub = -tHalf;
                            const float cY_sHalf_add = sHalf;
                            const float cY_sHalf_sub = -sHalf;
                            path.moveTo(ofVec2f(cX_tHalf_sub, cY_sHalf_sub));
                            path.lineTo(ofVec2f(cX_tHalf_add, cY_sHalf_sub));
                            path.lineTo(ofVec2f(cX_tHalf_add, cY_tHalf_sub));
                            path.lineTo(ofVec2f(cX_sHalf_add, cY_tHalf_sub));
                            path.lineTo(ofVec2f(cX_sHalf_add, cY_tHalf_add));
                            path.lineTo(ofVec2f(cX_tHalf_add, cY_tHalf_add));
                            path.lineTo(ofVec2f(cX_tHalf_add, cY_sHalf_add));
                            path.lineTo(ofVec2f(cX_tHalf_sub, cY_sHalf_add));
                            path.lineTo(ofVec2f(cX_tHalf_sub, cY_tHalf_add));
                            path.lineTo(ofVec2f(cX_sHalf_sub, cY_tHalf_add));
                            path.lineTo(ofVec2f(cX_sHalf_sub, cY_tHalf_sub));
                            path.lineTo(ofVec2f(cX_tHalf_sub, cY_tHalf_sub));
                            path.close();
                            ofMesh mesh = path.getTessellation();
                            
                            for (auto & v: mesh.getVertices()) {
                                
                                mesh.addNormal(ofVec3f(0.0f, 0.0f, 1.0f));
                                mesh.addTexCoord(ofVec2f((v.x + elem.args[0]*0.5f) / elem.args[0],
                                                         (v.y + elem.args[0]*0.5f) / elem.args[0]));
                            }
                            t_ofeliaLoadMesh3d::meshData[pos].mesh.append(mesh);
                            x->primMode = OF_PRIMITIVE_TRIANGLES;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_HEART:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofPath path;
                            path.setCurveResolution(static_cast<int>(elem.args[2]));
                            const float width = elem.args[0];
                            const float height = elem.args[1];
                            const float widthStretched = width * 1.125f;
                            const float heightHalf = height * 0.5f;
                            const ofVec2f beginEnd(0.0f, -height * 0.25f);
                            const ofVec2f bezierP1(0.0f, -height * 0.75f);
                            const ofVec2f bezierP3(0.0f, heightHalf);
                            path.moveTo(beginEnd);
                            path.bezierTo(bezierP1,
                                          ofVec2f(widthStretched, 5.0f-heightHalf),
                                          bezierP3);
                            path.moveTo(beginEnd);
                            path.bezierTo(bezierP1,
                                          ofVec2f(-widthStretched, 5.0f-heightHalf),
                                          bezierP3);
                            path.moveTo(bezierP3);
                            path.close();
                            ofMesh mesh = path.getTessellation();
                            
                            for (auto & v: mesh.getVertices()) {
                                
                                mesh.addNormal(ofVec3f(0.0f, 0.0f, 1.0f));
                                mesh.addTexCoord(ofVec2f((v.x + elem.args[0]*0.5f) / elem.args[0],
                                                         (v.y + elem.args[1]*0.5f) / elem.args[1]));
                            }
                            t_ofeliaLoadMesh3d::meshData[pos].mesh.append(mesh);
                            x->primMode = OF_PRIMITIVE_TRIANGLES;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_MOON:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofPath path;
                            const float radiusX = elem.args[0] * 0.5f;
                            const float radiusY = elem.args[1] * 0.5f;
                            ofPolyline polyline;
                            polyline.arc(ofVec2f(0.0f, 0.0f),
                                         elem.args[2], radiusY,
                                         -90.0f, 90.0f, true, static_cast<int>(elem.args[3]));
                            polyline.arc(ofVec2f(0.0f, 0.0f),
                                         radiusX, radiusY,
                                         90.0f, -90.0f, false, static_cast<int>(elem.args[3]));
                            path.moveTo(polyline.getVertices()[0]);
                            
                            for (size_t i=1; i<polyline.getVertices().size(); ++i)
                                path.lineTo(polyline.getVertices()[i]);
                            path.close();
                            ofMesh mesh = path.getTessellation();
                            
                            for (auto & v: mesh.getVertices()) {
                                
                                mesh.addNormal(ofVec3f(0.0f, 0.0f, 1.0f));
                                mesh.addTexCoord(ofVec2f((v.x + elem.args[0]*0.5f) / elem.args[0],
                                                         (v.y + elem.args[1]*0.5f) / elem.args[1]));
                            }
                            t_ofeliaLoadMesh3d::meshData[pos].mesh.append(mesh);
                            x->primMode = OF_PRIMITIVE_TRIANGLES;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_REGPOLYGON:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofPath path;
                            const float radius = elem.args[0];
                            const int numSides = static_cast<int>(elem.args[1]);
                            const float divAngle = 360.0f / numSides;
                            const float beginAngle = 90.0f - divAngle * 0.5f;
                            float posX, posY;
                            getCoordinatesFromRadAngle(posX, posY, radius, beginAngle);
                            path.moveTo(ofVec2f(posX, posY));
                            
                            for (int i=1; i<numSides; ++i) {
                                
                                getCoordinatesFromRadAngle(posX, posY, radius, beginAngle + divAngle*i);
                                path.lineTo(ofVec2f(posX, posY));
                            }
                            path.close();
                            ofMesh mesh = path.getTessellation();
                            
                            for (auto & v: mesh.getVertices()) {
                                
                                mesh.addNormal(ofVec3f(0.0f, 0.0f, 1.0f));
                                mesh.addTexCoord(ofVec2f((v.x + elem.args[0]) / (elem.args[0]*2.0f),
                                                         (v.y + elem.args[0]) / (elem.args[0]*2.0f)));
                            }
                            t_ofeliaLoadMesh3d::meshData[pos].mesh.append(mesh);
                            x->primMode = OF_PRIMITIVE_TRIANGLES;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_STAR:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofPath path;
                            const int sideNum = static_cast<int>(elem.args[2]) * 2;
                            const float divAngle = 360.0f / sideNum;
                            const float radius[] = {elem.args[0], elem.args[1]};
                            const float beginAngle = 90.0f - divAngle;
                            float posX, posY;
                            getCoordinatesFromRadAngle(posX, posY, radius[0], beginAngle);
                            path.moveTo(ofVec2f(posX, posY));
                            
                            for (int i=1; i<sideNum; ++i) {
                                
                                getCoordinatesFromRadAngle(posX, posY, radius[i%2], beginAngle + divAngle*i);
                                path.lineTo(ofVec2f(posX, posY));
                            }
                            path.close();
                            ofMesh mesh = path.getTessellation();
                            
                            for (auto & v: mesh.getVertices()) {
                                
                                mesh.addNormal(ofVec3f(0.0f, 0.0f, 1.0f));
                                mesh.addTexCoord(ofVec2f((v.x + elem.args[0]) / (elem.args[0]*2.0f),
                                                         (v.y + elem.args[0]) / (elem.args[0]*2.0f)));
                            }
                            t_ofeliaLoadMesh3d::meshData[pos].mesh.append(mesh);
                            x->primMode = OF_PRIMITIVE_TRIANGLES;
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_BOX:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofMesh mesh(ofMesh::box(elem.args[0],
                                                    elem.args[1],
                                                    elem.args[2],
                                                    static_cast<int>(elem.args[3]),
                                                    static_cast<int>(elem.args[4]),
                                                    static_cast<int>(elem.args[5])));
                            t_ofeliaLoadMesh3d::meshData[pos].mesh.append(mesh);
                            x->primMode = mesh.getMode();
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_CONE:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofMesh mesh(ofMesh::cone(elem.args[0],
                                                     elem.args[1],
                                                     static_cast<int>(elem.args[2]),
                                                     static_cast<int>(elem.args[3]),
                                                     static_cast<int>(elem.args[4]),
                                                     static_cast<ofPrimitiveMode>(static_cast<int>(elem.args[5]))));
                            t_ofeliaLoadMesh3d::meshData[pos].mesh.append(mesh);
                            x->primMode = mesh.getMode();
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_CYLINDER:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofMesh mesh(ofMesh::cylinder(elem.args[0],
                                                         elem.args[1],
                                                         static_cast<int>(elem.args[2]),
                                                         static_cast<int>(elem.args[3]),
                                                         static_cast<int>(elem.args[4]),
                                                         static_cast<bool>(elem.args[5]),
                                                         static_cast<ofPrimitiveMode>(static_cast<int>(elem.args[6]))));
                            t_ofeliaLoadMesh3d::meshData[pos].mesh.append(mesh);
                            x->primMode = mesh.getMode();
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_ICOSPHERE:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofMesh mesh(ofMesh::icosphere(elem.args[0],
                                                          static_cast<size_t>(ofClamp(elem.args[1], 0.0f, 5.0f))));
                            t_ofeliaLoadMesh3d::meshData[pos].mesh.append(mesh);
                            x->primMode = mesh.getMode();
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_PLANE:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofMesh mesh(ofMesh::plane(elem.args[0],
                                                      elem.args[1],
                                                      static_cast<int>(elem.args[2]),
                                                      static_cast<int>(elem.args[3]),
                                                      static_cast<ofPrimitiveMode>(static_cast<int>(elem.args[4]))));
                            t_ofeliaLoadMesh3d::meshData[pos].mesh.append(mesh);
                            x->primMode = mesh.getMode();
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case MESH_LOAD_CMDTYPE_SPHERE:
                {
                    switch (x->cmdVec[i].state) {
                            
                        case MESH_LOAD_CMD_INSERT:
                        {
                            ofMesh mesh(ofMesh::sphere(elem.args[0],
                                                       static_cast<int>(elem.args[1]),
                                                       static_cast<ofPrimitiveMode>(static_cast<int>(elem.args[2]))));
                            t_ofeliaLoadMesh3d::meshData[pos].mesh.append(mesh);
                            x->primMode = mesh.getMode();
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
        if (x->cmdVec.size() > cmdVecSize) {
            
            x->cmdVec.erase(x->cmdVec.begin(), x->cmdVec.begin() + cmdVecSize);
            return;
        }
        x->cmdVec.clear();
        x->shouldOutlet = true;
    }
    if (x->shouldOutlet) {
        
        t_ofeliaLoadMesh3d::meshData[pos].mesh.setMode(x->primMode);
        t_ofeliaLoadMesh3d::meshes[pos]->operator=(t_ofeliaLoadMesh3d::meshData[pos].mesh);
        t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
        
        /* OFELIANOTE : trick to update color to vbo (temporary hack) */
        t_ofeliaLoadMesh3d::meshes[pos]->addVertex(ofVec3f(0.0f, 0.0f, 0.0f));
        t_ofeliaLoadMesh3d::meshes[pos]->draw();
        t_ofeliaLoadMesh3d::meshes[pos]->getVertices().pop_back();
        
        /* output current sizes */
        x->numVertices = static_cast<int>(t_ofeliaLoadMesh3d::meshData[pos].mesh.getNumVertices());
        x->numIndices = static_cast<int>(t_ofeliaLoadMesh3d::meshData[pos].mesh.getNumIndices());
        x->numNormals = static_cast<int>(t_ofeliaLoadMesh3d::meshData[pos].mesh.getNumNormals());
        x->numTexCoords = static_cast<int>(t_ofeliaLoadMesh3d::meshData[pos].mesh.getNumTexCoords());
        x->numColors = static_cast<int>(t_ofeliaLoadMesh3d::meshData[pos].mesh.getNumColors());
        ofeliaLoadMesh3d_vecSizeOut(x);
        x->shouldOutlet = false;
    }
}

void ofeliaLoadMesh3d_exit(t_ofeliaLoadMesh3d *x)
{
    if (t_ofeliaLoadMesh3d::bInited)
        t_ofeliaLoadMesh3d::bInited = false;
    x->bInitGate = true;
    x->shouldOutlet = true;
}

void ofeliaLoadMesh3d_vecSizeOut(t_ofeliaLoadMesh3d *x)
{
    outlet_float(x->numColors_out, static_cast<t_float>(x->numColors));
    outlet_float(x->numTexCoords_out, static_cast<t_float>(x->numTexCoords));
    outlet_float(x->numNormals_out, static_cast<t_float>(x->numNormals));
    outlet_float(x->numIndices_out, static_cast<t_float>(x->numIndices));
    outlet_float(x->numVertices_out, static_cast<t_float>(x->numVertices));
}

void ofeliaLoadMesh3d_name(t_ofeliaLoadMesh3d *x, t_symbol *s)
{
    const int pos = getPositionByMesh3dObjID(x->objID);
    t_ofeliaLoadMesh3d::meshData[pos].varName.name = s->s_name;
    getVarNameLocalized(t_ofeliaLoadMesh3d::meshData[pos].varName);
}

void ofeliaLoadMesh3d_primMode(t_ofeliaLoadMesh3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (getPrimModeFromArgs(argc, argv, x->primMode, t_ofeliaLoadMesh3d::objName))
        x->shouldOutlet = true;
}

void ofeliaLoadMesh3d_load(t_ofeliaLoadMesh3d *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaLoadMesh3dElem> elems;
    
    if (getLoadMesh3dElemsFromArgs(argc, argv, elems)) {
        
        ofeliaLoadMesh3d_clear(x, 0, 0, 0);
        
        for (size_t i=0; i<elems.size(); ++i) {
            
            t_ofeliaLoadMesh3dCmdData cmd;
            cmd.elem = elems[i];
            cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
            cmd.state = MESH_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadMesh3d_add(t_ofeliaLoadMesh3d *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadMesh3dCmdData cmd;
    
    if (getLoadMesh3dElemFromArgs(argc, argv, cmd.elem)) {
        
        cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
        cmd.state = MESH_LOAD_CMD_INSERT;
        x->cmdVec.push_back(cmd);
    }
}

void ofeliaLoadMesh3d_append(t_ofeliaLoadMesh3d *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaLoadMesh3dElem> elems;
    
    if (getLoadMesh3dElemsFromArgs(argc, argv, elems)) {
        
        for (size_t i=0; i<elems.size(); ++i) {
            
            t_ofeliaLoadMesh3dCmdData cmd;
            cmd.elem = elems[i];
            cmd.fromIndex = cmd.toIndex = numeric_limits<t_float>::max();
            cmd.state = MESH_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadMesh3d_prepend(t_ofeliaLoadMesh3d *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<t_ofeliaLoadMesh3dElem> elems;
    
    if (getLoadMesh3dModElemsFromArgs(argc, argv, elems)) {
        
        for (size_t i=0; i<elems.size(); ++i) {
            
            t_ofeliaLoadMesh3dCmdData cmd;
            cmd.elem = elems[i];
            cmd.fromIndex = cmd.toIndex = static_cast<t_float>(i);
            cmd.state = MESH_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadMesh3d_insert(t_ofeliaLoadMesh3d *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadMesh3dCmdData cmd;
    
    if (getLoadMesh3dModElemFromArgs(argc, argv, cmd.elem)) {
        
        const int ac = cmd.elem.numArgs + 1;
        
        if (getCmdRangeFromArgs(argc-ac, argv+ac, cmd)) {
            
            cmd.state = MESH_LOAD_CMD_INSERT;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadMesh3d_fill(t_ofeliaLoadMesh3d *x, t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaLoadMesh3dCmdData cmd;
    
    if (getLoadMesh3dModElemFromArgs(argc, argv, cmd.elem)) {
        
        const int ac = cmd.elem.numArgs + 1;
        
        if (getCmdRangeFromArgs(argc-ac, argv+ac, cmd)) {
            
            cmd.state = MESH_LOAD_CMD_FILL;
            x->cmdVec.push_back(cmd);
        }
    }
}

void ofeliaLoadMesh3d_erase(t_ofeliaLoadMesh3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            t_ofeliaLoadMesh3dCmdData cmd;
            
            if (setLoadMesh3dModElemType(argv[0].a_w.w_symbol->s_name, cmd.elem)) {
                
                if (getCmdRangeFromArgs(argc-1, argv+1, cmd)) {
                    
                    cmd.state = MESH_LOAD_CMD_ERASE;
                    x->cmdVec.push_back(cmd);
                }
            }
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadMesh3d::objName);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadMesh3d::objName);
    }
}

void ofeliaLoadMesh3d_clear(t_ofeliaLoadMesh3d *x, t_symbol *s, int argc, t_atom *argv)
{
    vector<LoadMeshCmdType> types;
    
    if (!argc) {
        
        types = {MESH_LOAD_CMDTYPE_VERTEX,
            MESH_LOAD_CMDTYPE_INDEX,
            MESH_LOAD_CMDTYPE_NORMAL,
            MESH_LOAD_CMDTYPE_TEXCOORD,
            MESH_LOAD_CMDTYPE_COLOR};
    }
    else {
        
        for (int i=0; i<argc; ++i) {
            
            if (argv[i].a_type == A_SYMBOL) {
                
                t_ofeliaLoadMesh3dElem elem;
                
                if (setLoadMesh3dModElemType(argv[i].a_w.w_symbol->s_name, elem))
                    types.push_back(elem.type);
                else
                    return;
            }
            else {
                
                error("%s: wrong argument type", t_ofeliaLoadMesh3d::objName);
                return;
            }
        }
    }
    for (size_t i=0; i<types.size(); ++i) {
        
        t_ofeliaLoadMesh3dCmdData cmd;
        cmd.elem.type = types[i];
        cmd.fromIndex = 0.0f;
        cmd.toIndex = numeric_limits<t_float>::max();
        cmd.state = MESH_LOAD_CMD_ERASE;
        x->cmdVec.push_back(cmd);
    }
}

void ofeliaLoadMesh3d_set(t_ofeliaLoadMesh3d *x, t_symbol *s, int argc, t_atom *argv)
{
    if (argc) {
        
        if (argv[0].a_type == A_SYMBOL) {
            
            ofeliaLoadMesh3d_name(x, argv[0].a_w.w_symbol);
            
            if (--argc)
                ofeliaLoadMesh3d_load(x, 0, argc, argv+1);
        }
        else {
            
            error("%s: wrong argument type", t_ofeliaLoadMesh3d::objName);
        }
    }
    else {
        
        error("%s: invalid number of arguments", t_ofeliaLoadMesh3d::objName);
    }
}

void ofeliaLoadMesh3d_print(t_ofeliaLoadMesh3d *x)
{
    post("\n[%s]", t_ofeliaLoadMesh3d::objName);
    const int pos = getPositionByMesh3dObjID(x->objID);
    post("name : %s", t_ofeliaLoadMesh3d::meshData[pos].varName.name.c_str());
    printPrimMode(x->primMode);
    post("load : ");
    
    for (size_t i=0; i<t_ofeliaLoadMesh3d::meshData[pos].mesh.getNumVertices(); ++i) {
        
        const ofVec3f &vertex = t_ofeliaLoadMesh3d::meshData[pos].mesh.getVertex(i);
        post(" vertex %g %g %g", vertex.x, vertex.y, vertex.z);
    }
    for (size_t i=0; i<t_ofeliaLoadMesh3d::meshData[pos].mesh.getNumIndices(); ++i) {
        
        const ofIndexType index = t_ofeliaLoadMesh3d::meshData[pos].mesh.getIndex(i);
        post(" index %d", index);
    }
    for (size_t i=0; i<t_ofeliaLoadMesh3d::meshData[pos].mesh.getNumNormals(); ++i) {
        
        const ofVec3f &normal = t_ofeliaLoadMesh3d::meshData[pos].mesh.getNormal(i);
        post(" normal %g %g %g", normal.x, normal.y, normal.z);
    }
    for (size_t i=0; i<t_ofeliaLoadMesh3d::meshData[pos].mesh.getNumTexCoords(); ++i) {
        
        const ofVec2f &texCoord = t_ofeliaLoadMesh3d::meshData[pos].mesh.getTexCoord(i);
        post(" texCoord %g %g", texCoord.x, texCoord.y);
    }
    for (size_t i=0; i<t_ofeliaLoadMesh3d::meshData[pos].mesh.getNumColors(); ++i) {
        
        const ofColor &color = t_ofeliaLoadMesh3d::meshData[pos].mesh.getColor(i);
        post(" color %d %d %d %d", color.r, color.g, color.b, color.a);
    }
}

void ofeliaLoadMesh3d_free(t_ofeliaLoadMesh3d *x)
{
    const int pos = getPositionByMesh3dObjID(x->objID);
    t_ofeliaLoadMesh3d::meshData.erase(t_ofeliaLoadMesh3d::meshData.begin() + pos);
    t_ofeliaLoadMesh3d::meshes.erase(t_ofeliaLoadMesh3d::meshes.begin() + pos);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::initSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::updateSym);
    pd_unbind(&x->x_obj.ob_pd, t_ofeliaWindow::exitSym);
    outlet_free(x->numVertices_out);
    outlet_free(x->numIndices_out);
    outlet_free(x->numNormals_out);
    outlet_free(x->numTexCoords_out);
    outlet_free(x->numColors_out);
}

void ofeliaLoadMesh3d_setup()
{
    ofeliaLoadMesh3d_class = class_new(gensym("ofLoadMesh3d"),
                                       reinterpret_cast<t_newmethod>(ofeliaLoadMesh3d_new),
                                       reinterpret_cast<t_method>(ofeliaLoadMesh3d_free),
                                       sizeof(t_ofeliaLoadMesh3d),
                                       CLASS_DEFAULT, A_GIMME, 0);
    class_addmethod(ofeliaLoadMesh3d_class, reinterpret_cast<t_method>(ofeliaLoadMesh3d_init),
                    t_ofeliaWindow::initMess, A_NULL, 0);
    class_addmethod(ofeliaLoadMesh3d_class, reinterpret_cast<t_method>(ofeliaLoadMesh3d_update),
                    t_ofeliaWindow::updateMess, A_NULL, 0);
    class_addmethod(ofeliaLoadMesh3d_class, reinterpret_cast<t_method>(ofeliaLoadMesh3d_exit),
                    t_ofeliaWindow::exitMess, A_NULL, 0);
    class_addmethod(ofeliaLoadMesh3d_class, reinterpret_cast<t_method>(ofeliaLoadMesh3d_name),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaLoadMesh3d_class, reinterpret_cast<t_method>(ofeliaLoadMesh3d_primMode),
                    gensym("primMode"), A_GIMME, 0);
    class_addmethod(ofeliaLoadMesh3d_class, reinterpret_cast<t_method>(ofeliaLoadMesh3d_load),
                    gensym("load"), A_GIMME, 0);
    class_addmethod(ofeliaLoadMesh3d_class, reinterpret_cast<t_method>(ofeliaLoadMesh3d_add),
                    gensym("add"), A_GIMME, 0);
    class_addmethod(ofeliaLoadMesh3d_class, reinterpret_cast<t_method>(ofeliaLoadMesh3d_append),
                    gensym("append"), A_GIMME, 0);
    class_addmethod(ofeliaLoadMesh3d_class, reinterpret_cast<t_method>(ofeliaLoadMesh3d_prepend),
                    gensym("prepend"), A_GIMME, 0);
    class_addmethod(ofeliaLoadMesh3d_class, reinterpret_cast<t_method>(ofeliaLoadMesh3d_insert),
                    gensym("insert"), A_GIMME, 0);
    class_addmethod(ofeliaLoadMesh3d_class, reinterpret_cast<t_method>(ofeliaLoadMesh3d_fill),
                    gensym("fill"), A_GIMME, 0);
    class_addmethod(ofeliaLoadMesh3d_class, reinterpret_cast<t_method>(ofeliaLoadMesh3d_erase),
                    gensym("erase"), A_GIMME, 0);
    class_addmethod(ofeliaLoadMesh3d_class, reinterpret_cast<t_method>(ofeliaLoadMesh3d_clear),
                    gensym("clear"), A_GIMME, 0);
    class_addmethod(ofeliaLoadMesh3d_class, reinterpret_cast<t_method>(ofeliaLoadMesh3d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaLoadMesh3d_class, reinterpret_cast<t_method>(ofeliaLoadMesh3d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * common methods
 */
int getPositionByMesh2dName(const t_string &name)
{
    auto it = find_if(begin(t_ofeliaLoadMesh2d::meshData), end(t_ofeliaLoadMesh2d::meshData),
                      [&name] (const t_ofeliaLoadMesh2dData &meshData) {
                          return (meshData.varName.name == name);
                      });
    if (it == end(t_ofeliaLoadMesh2d::meshData))
        return -1;
    return static_cast<int>(it - begin(t_ofeliaLoadMesh2d::meshData));
}

int getPositionByMesh3dName(const t_string &name)
{
    auto it = find_if(begin(t_ofeliaLoadMesh3d::meshData), end(t_ofeliaLoadMesh3d::meshData),
                      [&name] (const t_ofeliaLoadMesh3dData &meshData) {
                          return (meshData.varName.name == name);
                      });
    if (it == end(t_ofeliaLoadMesh3d::meshData))
        return -1;
    return static_cast<int>(it - begin(t_ofeliaLoadMesh3d::meshData));
}

/* ________________________________________________________________________________
 * ofDrawMesh2d object methods
 */
void *ofeliaDrawMesh2d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaDrawMesh2d *x = reinterpret_cast<t_ofeliaDrawMesh2d*>(pd_new(ofeliaDrawMesh2d_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    getVarNameLocalPrefixes(x->elem.varName);
    
    if (!getNameDimen2dElemFromArgs(argc, argv, x->elem, t_ofeliaDrawMesh2d::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("width"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("height"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaDrawMesh2d_bang(t_ofeliaDrawMesh2d *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const t_string &name = x->elem.varName.name;
        
        if (!name.empty()) {
            
            const int pos = getPositionByMesh2dName(name);
            
            if (pos != -1) {
                
                if (t_ofeliaLoadMesh2d::meshes[pos]->hasVertices()) {
                    
                    const bool bFill = ofGetStyle().bFill;
                    
                    if (!x->elem.width && !x->elem.height) {
                        
                        if (!bFill)
                            t_ofeliaLoadMesh2d::meshes[pos]->drawWireframe();
                        else
                            t_ofeliaLoadMesh2d::meshes[pos]->drawFaces();
                    }
                    else {
                        
                        if (t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties)
                            updateMesh2dProperties(pos);
                        ofGetCurrentRenderer()->pushMatrix();
                        const float widthScale = x->elem.width / t_ofeliaLoadMesh2d::meshData[pos].boxDimen.x;
                        const float heightScale = x->elem.height / t_ofeliaLoadMesh2d::meshData[pos].boxDimen.y;
                        ofGetCurrentRenderer()->scale(widthScale, heightScale, 1.0f);
                        
                        if (!bFill)
                            t_ofeliaLoadMesh2d::meshes[pos]->drawWireframe();
                        else
                            t_ofeliaLoadMesh2d::meshes[pos]->drawFaces();
                        ofGetCurrentRenderer()->popMatrix();
                    }
                }
            }
        }
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaDrawMesh2d::objName);
    }
}

void ofeliaDrawMesh2d_name(t_ofeliaDrawMesh2d *x, t_symbol *s)
{
    x->elem.varName.name = s->s_name;
    getVarNameLocalized(x->elem.varName);
}

void ofeliaDrawMesh2d_width(t_ofeliaDrawMesh2d *x, t_floatarg f)
{
    x->elem.width = f;
}

void ofeliaDrawMesh2d_height(t_ofeliaDrawMesh2d *x, t_floatarg f)
{
    x->elem.height = f;
}

void ofeliaDrawMesh2d_set(t_ofeliaDrawMesh2d *x, t_symbol *s, int argc, t_atom *argv)
{
    getNameDimen2dElemFromArgs(argc, argv, x->elem, t_ofeliaDrawMesh2d::objName);
}

void ofeliaDrawMesh2d_print(t_ofeliaDrawMesh2d *x)
{
    post("\n[%s]", t_ofeliaDrawMesh2d::objName);
    const t_string &name = x->elem.varName.name;
    float width, height;
    
    if (!x->elem.width && !x->elem.height) {
        
        if (!name.empty()) {
            
            const int pos = getPositionByMesh2dName(name);
            
            if (pos != -1) {
                
                if (t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties)
                    updateMesh2dProperties(pos);
                width = t_ofeliaLoadMesh2d::meshData[pos].boxDimen.x;
                height = t_ofeliaLoadMesh2d::meshData[pos].boxDimen.y;
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

void ofeliaDrawMesh2d_setup()
{
    ofeliaDrawMesh2d_class = class_new(gensym("ofDrawMesh2d"),
                                       reinterpret_cast<t_newmethod>(ofeliaDrawMesh2d_new),
                                       0, sizeof(t_ofeliaDrawMesh2d),
                                       CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaDrawMesh2d_class, reinterpret_cast<t_method>(ofeliaDrawMesh2d_bang));
    class_addmethod(ofeliaDrawMesh2d_class, reinterpret_cast<t_method>(ofeliaDrawMesh2d_name),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaDrawMesh2d_class, reinterpret_cast<t_method>(ofeliaDrawMesh2d_width),
                    gensym("width"), A_FLOAT, 0);
    class_addmethod(ofeliaDrawMesh2d_class, reinterpret_cast<t_method>(ofeliaDrawMesh2d_height),
                    gensym("height"), A_FLOAT, 0);
    class_addmethod(ofeliaDrawMesh2d_class, reinterpret_cast<t_method>(ofeliaDrawMesh2d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaDrawMesh2d_class, reinterpret_cast<t_method>(ofeliaDrawMesh2d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofDrawMesh3d object methods
 */
void *ofeliaDrawMesh3d_new(t_symbol *s, int argc, t_atom *argv)
{
    t_ofeliaDrawMesh3d *x = reinterpret_cast<t_ofeliaDrawMesh3d*>(pd_new(ofeliaDrawMesh3d_class));
    
    if (atom_getsymbolarg(0, argc, argv)->s_name[0] == ';')
        removeSemicolonFromArgs(argc, argv);
    getVarNameLocalPrefixes(x->elem.varName);
    
    if (!getNameDimen3dElemFromArgs(argc, argv, x->elem, t_ofeliaDrawMesh3d::objName))
        return 0;
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("width"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("height"));
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_float, gensym("depth"));
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaDrawMesh3d_bang(t_ofeliaDrawMesh3d *x)
{
    if (t_ofeliaWindow::bRenderGate) {
        
        const t_string &name = x->elem.varName.name;
        
        if (!name.empty()) {
            
            const int pos = getPositionByMesh3dName(name);
            
            if (pos != -1) {
                
                if (t_ofeliaLoadMesh3d::meshes[pos]->hasVertices()) {
                    
                    const bool bFill = ofGetStyle().bFill;
                    
                    if (!x->elem.width && !x->elem.height && !x->elem.depth) {
                        
                        if (!bFill)
                            t_ofeliaLoadMesh3d::meshes[pos]->drawWireframe();
                        else
                            t_ofeliaLoadMesh3d::meshes[pos]->drawFaces();
                    }
                    else {
                        
                        if (t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties)
                            updateMesh3dProperties(pos);
                        ofGetCurrentRenderer()->pushMatrix();
                        const float widthScale = x->elem.width / t_ofeliaLoadMesh3d::meshData[pos].boxDimen.x;
                        const float heightScale = x->elem.height / t_ofeliaLoadMesh3d::meshData[pos].boxDimen.y;
                        const float depthScale = x->elem.depth / t_ofeliaLoadMesh3d::meshData[pos].boxDimen.z;
                        ofGetCurrentRenderer()->scale(widthScale, heightScale, depthScale);
                        
                        if (!bFill)
                            t_ofeliaLoadMesh3d::meshes[pos]->drawWireframe();
                        else
                            t_ofeliaLoadMesh3d::meshes[pos]->drawFaces();
                        ofGetCurrentRenderer()->popMatrix();
                    }
                }
            }
        }
        outlet_bang(x->x_obj.ob_outlet);
    }
    else {
        
        error("%s: 'bang' can only be received through a rendering chain", t_ofeliaDrawMesh3d::objName);
    }
}

void ofeliaDrawMesh3d_name(t_ofeliaDrawMesh3d *x, t_symbol *s)
{
    x->elem.varName.name = s->s_name;
    getVarNameLocalized(x->elem.varName);
}

void ofeliaDrawMesh3d_width(t_ofeliaDrawMesh3d *x, t_floatarg f)
{
    x->elem.width = f;
}

void ofeliaDrawMesh3d_height(t_ofeliaDrawMesh3d *x, t_floatarg f)
{
    x->elem.height = f;
}

void ofeliaDrawMesh3d_depth(t_ofeliaDrawMesh3d *x, t_floatarg f)
{
    x->elem.depth = f;
}

void ofeliaDrawMesh3d_set(t_ofeliaDrawMesh3d *x, t_symbol *s, int argc, t_atom *argv)
{
    getNameDimen3dElemFromArgs(argc, argv, x->elem, t_ofeliaDrawMesh3d::objName);
}

void ofeliaDrawMesh3d_print(t_ofeliaDrawMesh3d *x)
{
    post("\n[%s]", t_ofeliaDrawMesh3d::objName);
    const t_string &name = x->elem.varName.name;
    float width, height, depth;
    
    if (!x->elem.width && !x->elem.height && !x->elem.depth) {
        
        if (!name.empty()) {
            
            const int pos = getPositionByMesh3dName(name);
            
            if (pos != -1) {
                
                if (t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties)
                    updateMesh3dProperties(pos);
                width = t_ofeliaLoadMesh3d::meshData[pos].boxDimen.x;
                height = t_ofeliaLoadMesh3d::meshData[pos].boxDimen.y;
                depth = t_ofeliaLoadMesh3d::meshData[pos].boxDimen.z;
            }
            else {
                
                width = 0.0f;
                height = 0.0f;
                depth = 0.0f;
            }
        }
        else {
            
            width = 0.0f;
            height = 0.0f;
            depth = 0.0f;
        }
    }
    else {
        
        width = x->elem.width;
        height = x->elem.height;
        depth = x->elem.depth;
    }
    post("name : %s", name.c_str());
    post("width : %g", width);
    post("height : %g", height);
    post("depth : %g", depth);
}

void ofeliaDrawMesh3d_setup()
{
    ofeliaDrawMesh3d_class = class_new(gensym("ofDrawMesh3d"),
                                       reinterpret_cast<t_newmethod>(ofeliaDrawMesh3d_new),
                                       0, sizeof(t_ofeliaDrawMesh3d),
                                       CLASS_DEFAULT, A_GIMME, 0);
    class_addbang(ofeliaDrawMesh3d_class, reinterpret_cast<t_method>(ofeliaDrawMesh3d_bang));
    class_addmethod(ofeliaDrawMesh3d_class, reinterpret_cast<t_method>(ofeliaDrawMesh3d_name),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaDrawMesh3d_class, reinterpret_cast<t_method>(ofeliaDrawMesh3d_width),
                    gensym("width"), A_FLOAT, 0);
    class_addmethod(ofeliaDrawMesh3d_class, reinterpret_cast<t_method>(ofeliaDrawMesh3d_height),
                    gensym("height"), A_FLOAT, 0);
    class_addmethod(ofeliaDrawMesh3d_class, reinterpret_cast<t_method>(ofeliaDrawMesh3d_depth),
                    gensym("depth"), A_FLOAT, 0);
    class_addmethod(ofeliaDrawMesh3d_class, reinterpret_cast<t_method>(ofeliaDrawMesh3d_set),
                    gensym("set"), A_GIMME, 0);
    class_addmethod(ofeliaDrawMesh3d_class, reinterpret_cast<t_method>(ofeliaDrawMesh3d_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofDoesMesh2dNameExist object methods
 */
void *ofeliaDoesMesh2dNameExist_new(t_symbol *s)
{
    t_ofeliaDoesMesh2dNameExist *x = reinterpret_cast<t_ofeliaDoesMesh2dNameExist*>(pd_new(ofeliaDoesMesh2dNameExist_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaDoesMesh2dNameExist_bang(t_ofeliaDoesMesh2dNameExist *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        if (getPositionByMesh2dName(name) != -1)
            outlet_float(x->x_obj.ob_outlet, 1.0f);
        else
            outlet_float(x->x_obj.ob_outlet, 0.0f);
    }
    else {
        
        error("%s: name not assigned", t_ofeliaDoesMesh2dNameExist::objName);
    }
}

void ofeliaDoesMesh2dNameExist_set(t_ofeliaDoesMesh2dNameExist *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaDoesMesh2dNameExist_print(t_ofeliaDoesMesh2dNameExist *x)
{
    post("\n[%s]", t_ofeliaDoesMesh2dNameExist::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaDoesMesh2dNameExist_setup()
{
    ofeliaDoesMesh2dNameExist_class = class_new(gensym("ofDoesMesh2dNameExist"),
                                               reinterpret_cast<t_newmethod>(ofeliaDoesMesh2dNameExist_new),
                                               0, sizeof(t_ofeliaDoesMesh2dNameExist),
                                               CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaDoesMesh2dNameExist_class, reinterpret_cast<t_method>(ofeliaDoesMesh2dNameExist_bang));
    class_addmethod(ofeliaDoesMesh2dNameExist_class, reinterpret_cast<t_method>(ofeliaDoesMesh2dNameExist_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesMesh2dNameExist_class, reinterpret_cast<t_method>(ofeliaDoesMesh2dNameExist_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesMesh2dNameExist_class, reinterpret_cast<t_method>(ofeliaDoesMesh2dNameExist_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofDoesMesh3dNameExist object methods
 */
void *ofeliaDoesMesh3dNameExist_new(t_symbol *s)
{
    t_ofeliaDoesMesh3dNameExist *x = reinterpret_cast<t_ofeliaDoesMesh3dNameExist*>(pd_new(ofeliaDoesMesh3dNameExist_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaDoesMesh3dNameExist_bang(t_ofeliaDoesMesh3dNameExist *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        if (getPositionByMesh3dName(name) != -1)
            outlet_float(x->x_obj.ob_outlet, 1.0f);
        else
            outlet_float(x->x_obj.ob_outlet, 0.0f);
    }
    else {
        
        error("%s: name not assigned", t_ofeliaDoesMesh3dNameExist::objName);
    }
}

void ofeliaDoesMesh3dNameExist_set(t_ofeliaDoesMesh3dNameExist *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaDoesMesh3dNameExist_print(t_ofeliaDoesMesh3dNameExist *x)
{
    post("\n[%s]", t_ofeliaDoesMesh3dNameExist::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaDoesMesh3dNameExist_setup()
{
    ofeliaDoesMesh3dNameExist_class = class_new(gensym("ofDoesMesh3dNameExist"),
                                               reinterpret_cast<t_newmethod>(ofeliaDoesMesh3dNameExist_new),
                                               0, sizeof(t_ofeliaDoesMesh3dNameExist),
                                               CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaDoesMesh3dNameExist_class, reinterpret_cast<t_method>(ofeliaDoesMesh3dNameExist_bang));
    class_addmethod(ofeliaDoesMesh3dNameExist_class, reinterpret_cast<t_method>(ofeliaDoesMesh3dNameExist_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesMesh3dNameExist_class, reinterpret_cast<t_method>(ofeliaDoesMesh3dNameExist_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaDoesMesh3dNameExist_class, reinterpret_cast<t_method>(ofeliaDoesMesh3dNameExist_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofEditMesh2dVertex object methods
 */
void *ofeliaEditMesh2dVertex_new(t_symbol *s)
{
    t_ofeliaEditMesh2dVertex *x = reinterpret_cast<t_ofeliaEditMesh2dVertex*>(pd_new(ofeliaEditMesh2dVertex_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaEditMesh2dVertex_float(t_ofeliaEditMesh2dVertex *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaEditMesh2dVertex_assign(t_ofeliaEditMesh2dVertex *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh2d::meshes[pos]->getVertices()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh2dVertex::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            vertex = ofVec2f(argv[0].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dVertex::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            vertex = ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dVertex::objName);
                            return;
                        }
                        break;
                    }
                }
                t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dVertex::objName);
    }
}

void ofeliaEditMesh2dVertex_assignX(t_ofeliaEditMesh2dVertex *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh2d::meshes[pos]->getVertices()[index];
                vertex.x = f;
                t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dVertex::objName);
    }
}

void ofeliaEditMesh2dVertex_assignY(t_ofeliaEditMesh2dVertex *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh2d::meshes[pos]->getVertices()[index];
                vertex.y = f;
                t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dVertex::objName);
    }
}

void ofeliaEditMesh2dVertex_add(t_ofeliaEditMesh2dVertex *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh2d::meshes[pos]->getVertices()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh2dVertex::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            vertex += argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dVertex::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            vertex += ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dVertex::objName);
                            return;
                        }
                        break;
                    }
                }
                t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dVertex::objName);
    }
}

void ofeliaEditMesh2dVertex_sub(t_ofeliaEditMesh2dVertex *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh2d::meshes[pos]->getVertices()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh2dVertex::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            vertex -= argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dVertex::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            vertex -= ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dVertex::objName);
                            return;
                        }
                        break;
                    }
                }
                t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dVertex::objName);
    }
}

void ofeliaEditMesh2dVertex_mult(t_ofeliaEditMesh2dVertex *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh2d::meshes[pos]->getVertices()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh2dVertex::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            vertex *= argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dVertex::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            vertex *= ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dVertex::objName);
                            return;
                        }
                        break;
                    }
                }
                t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dVertex::objName);
    }
}

void ofeliaEditMesh2dVertex_div(t_ofeliaEditMesh2dVertex *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh2d::meshes[pos]->getVertices()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh2dVertex::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            vertex /= argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dVertex::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            vertex /= ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dVertex::objName);
                            return;
                        }
                        break;
                    }
                }
                t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dVertex::objName);
    }
}

void ofeliaEditMesh2dVertex_lerp(t_ofeliaEditMesh2dVertex *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh2d::meshes[pos]->getVertices()[index];
                
                if (argc == 3) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT) {
                        
                        vertex.interpolate(ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float), argv[2].a_w.w_float);
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditMesh2dVertex::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditMesh2dVertex::objName);
                    return;
                }
                t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dVertex::objName);
    }
}

void ofeliaEditMesh2dVertex_limit(t_ofeliaEditMesh2dVertex *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh2d::meshes[pos]->getVertices()[index];
                vertex.limit(f);
                t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dVertex::objName);
    }
}

void ofeliaEditMesh2dVertex_map(t_ofeliaEditMesh2dVertex *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh2d::meshes[pos]->getVertices()[index];
                
                if (argc == 6) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT &&
                        argv[3].a_type == A_FLOAT &&
                        argv[4].a_type == A_FLOAT &&
                        argv[5].a_type == A_FLOAT) {
                        
                        vertex.map(ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, 0.0f),
                                   ofVec3f(argv[2].a_w.w_float, argv[3].a_w.w_float, 0.0f),
                                   ofVec3f(argv[4].a_w.w_float, argv[5].a_w.w_float, 0.0f),
                                   ofVec3f(0.0f, 0.0f, 0.0f));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditMesh2dVertex::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditMesh2dVertex::objName);
                    return;
                }
                t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dVertex::objName);
    }
}

void ofeliaEditMesh2dVertex_middle(t_ofeliaEditMesh2dVertex *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh2d::meshes[pos]->getVertices()[index];
                
                if (argc == 2) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT) {
                        
                        vertex.middle(ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, 0.0f));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditMesh2dVertex::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditMesh2dVertex::objName);
                    return;
                }
                t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dVertex::objName);
    }
}

void ofeliaEditMesh2dVertex_normalize(t_ofeliaEditMesh2dVertex *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh2d::meshes[pos]->getVertices()[index];
                vertex.normalize();
                t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dVertex::objName);
    }
}

void ofeliaEditMesh2dVertex_perpendicular(t_ofeliaEditMesh2dVertex *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh2d::meshes[pos]->getVertices()[index];
                /* ofVec2f::perpendicular() */
                const float length = static_cast<float>(sqrt(vertex.x * vertex.x + vertex.y * vertex.y));
                
                if (length > 0) {
                    
                    const float _x = vertex.x;
                    vertex.x = -(vertex.y / length);
                    vertex.y = _x/length;
                }
                t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dVertex::objName);
    }
}

void ofeliaEditMesh2dVertex_rotate(t_ofeliaEditMesh2dVertex *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh2d::meshes[pos]->getVertices()[index];
                
                switch (argc) {
                        
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            /* ofVec2f::rotate(float angle) */
                            const float angle = argv[0].a_w.w_float;
                            const float a = static_cast<float>(angle * DEG_TO_RAD);
                            const float xrot = vertex.x * cos(a) - vertex.y * sin(a);
                            vertex.y = vertex.x * sin(a) + vertex.y * cos(a);
                            vertex.x = xrot;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dVertex::objName);
                            return;
                        }
                        break;
                    }
                    case 3:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            /* ofVec2f::rotate(float angle, const ofVec2f& pivot) */
                            const float angle = argv[0].a_w.w_float;
                            const ofVec2f &pivot = ofVec2f(argv[1].a_w.w_float,
                                                           argv[2].a_w.w_float);
                            const float a = static_cast<float>(angle * DEG_TO_RAD);
                            const float xrot = ((vertex.x-pivot.x)*cos(a) - (vertex.y-pivot.y)*sin(a)) + pivot.x;
                            vertex.y = ((vertex.x-pivot.x)*sin(a) + (vertex.y-pivot.y)*cos(a)) + pivot.y;
                            vertex.x = xrot;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dVertex::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh2dVertex::objName);
                        return;
                    }
                }
                t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dVertex::objName);
    }
}

void ofeliaEditMesh2dVertex_rotateRad(t_ofeliaEditMesh2dVertex *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh2d::meshes[pos]->getVertices()[index];
                
                switch (argc) {
                        
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            /* ofVec2f::rotateRad(float angle) */
                            const float angle = argv[0].a_w.w_float;
                            const float a = angle;
                            float xrot = vertex.x * cos(a) - vertex.y * sin(a);
                            vertex.y = vertex.x * sin(a) + vertex.y * cos(a);
                            vertex.x = xrot;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dVertex::objName);
                            return;
                        }
                        break;
                    }
                    case 3:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            /* ofVec2f::rotateRad(float angle, const ofVec2f& pivot) */
                            const float angle = argv[0].a_w.w_float;
                            const ofVec2f &pivot = ofVec2f(argv[1].a_w.w_float,
                                                           argv[2].a_w.w_float);
                            const float a = angle;
                            const float xrot = ((vertex.x-pivot.x)*cos(a) - (vertex.y-pivot.y)*sin(a)) + pivot.x;
                            vertex.y = ((vertex.x-pivot.x)*sin(a) + (vertex.y-pivot.y)*cos(a)) + pivot.y;
                            vertex.x = xrot;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dVertex::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh2dVertex::objName);
                        return;
                    }
                }
                t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dVertex::objName);
    }
}

void ofeliaEditMesh2dVertex_scale(t_ofeliaEditMesh2dVertex *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh2d::meshes[pos]->getVertices()[index];
                vertex.scale(f);
                t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dVertex::objName);
    }
}

void ofeliaEditMesh2dVertex_set(t_ofeliaEditMesh2dVertex *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaEditMesh2dVertex_print(t_ofeliaEditMesh2dVertex *x)
{
    post("\n[%s]", t_ofeliaEditMesh2dVertex::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaEditMesh2dVertex_setup()
{
    ofeliaEditMesh2dVertex_class = class_new(gensym("ofEditMesh2dVertex"),
                                             reinterpret_cast<t_newmethod>(ofeliaEditMesh2dVertex_new),
                                             0, sizeof(t_ofeliaEditMesh2dVertex),
                                             CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaEditMesh2dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh2dVertex_float));
    class_addmethod(ofeliaEditMesh2dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh2dVertex_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditMesh2dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh2dVertex_assign),
                    gensym("assign"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh2dVertex_assignX),
                    gensym("assignX"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh2dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh2dVertex_assignY),
                    gensym("assignY"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh2dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh2dVertex_add),
                    gensym("add"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh2dVertex_sub),
                    gensym("sub"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh2dVertex_mult),
                    gensym("mult"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh2dVertex_div),
                    gensym("div"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh2dVertex_lerp),
                    gensym("lerp"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh2dVertex_limit),
                    gensym("limit"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh2dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh2dVertex_map),
                    gensym("map"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh2dVertex_middle),
                    gensym("middle"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh2dVertex_normalize),
                    gensym("normalize"), A_NULL, 0);
    class_addmethod(ofeliaEditMesh2dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh2dVertex_perpendicular),
                    gensym("perpendicular"), A_NULL, 0);
    class_addmethod(ofeliaEditMesh2dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh2dVertex_rotate),
                    gensym("rotate"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh2dVertex_rotateRad),
                    gensym("rotateRad"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh2dVertex_scale),
                    gensym("scale"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh2dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh2dVertex_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditMesh2dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh2dVertex_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofEditMesh3dVertex object methods
 */
void *ofeliaEditMesh3dVertex_new(t_symbol *s)
{
    t_ofeliaEditMesh3dVertex *x = reinterpret_cast<t_ofeliaEditMesh3dVertex*>(pd_new(ofeliaEditMesh3dVertex_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaEditMesh3dVertex_float(t_ofeliaEditMesh3dVertex *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaEditMesh3dVertex_assign(t_ofeliaEditMesh3dVertex *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh3d::meshes[pos]->getVertices()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh3dVertex::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            vertex = ofVec3f(argv[0].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dVertex::objName);
                            return;
                        }
                        break;
                    }
                    case 2:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            vertex = ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dVertex::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            vertex = ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dVertex::objName);
                            return;
                        }
                        break;
                    }
                }
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dVertex::objName);
    }
}

void ofeliaEditMesh3dVertex_assignX(t_ofeliaEditMesh3dVertex *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh3d::meshes[pos]->getVertices()[index];
                vertex.x = f;
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dVertex::objName);
    }
}

void ofeliaEditMesh3dVertex_assignY(t_ofeliaEditMesh3dVertex *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh3d::meshes[pos]->getVertices()[index];
                vertex.y = f;
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dVertex::objName);
    }
}

void ofeliaEditMesh3dVertex_assignZ(t_ofeliaEditMesh3dVertex *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh3d::meshes[pos]->getVertices()[index];
                vertex.z = f;
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dVertex::objName);
    }
}

void ofeliaEditMesh3dVertex_add(t_ofeliaEditMesh3dVertex *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh3d::meshes[pos]->getVertices()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh3dVertex::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            vertex += argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dVertex::objName);
                            return;
                        }
                        break;
                    }
                    case 2:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            vertex += ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dVertex::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            vertex += ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dVertex::objName);
                            return;
                        }
                        break;
                    }
                }
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dVertex::objName);
    }
}

void ofeliaEditMesh3dVertex_sub(t_ofeliaEditMesh3dVertex *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh3d::meshes[pos]->getVertices()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh3dVertex::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            vertex -= argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dVertex::objName);
                            return;
                        }
                        break;
                    }
                    case 2:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            vertex -= ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dVertex::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            vertex -= ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dVertex::objName);
                            return;
                        }
                        break;
                    }
                }
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dVertex::objName);
    }
}

void ofeliaEditMesh3dVertex_mult(t_ofeliaEditMesh3dVertex *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh3d::meshes[pos]->getVertices()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh3dVertex::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            vertex *= argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dVertex::objName);
                            return;
                        }
                        break;
                    }
                    case 2:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            vertex *= ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dVertex::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            vertex *= ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dVertex::objName);
                            return;
                        }
                        break;
                    }
                }
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dVertex::objName);
    }
}

void ofeliaEditMesh3dVertex_div(t_ofeliaEditMesh3dVertex *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh3d::meshes[pos]->getVertices()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh3dVertex::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            vertex /= argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dVertex::objName);
                            return;
                        }
                        break;
                    }
                    case 2:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            vertex /= ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dVertex::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            vertex /= ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dVertex::objName);
                            return;
                        }
                        break;
                    }
                }
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dVertex::objName);
    }
}

void ofeliaEditMesh3dVertex_cross(t_ofeliaEditMesh3dVertex *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh3d::meshes[pos]->getVertices()[index];
                
                if (argc == 3) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT) {
                        
                        vertex.cross(ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditMesh3dVertex::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditMesh3dVertex::objName);
                    return;
                }
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dVertex::objName);
    }
}

void ofeliaEditMesh3dVertex_lerp(t_ofeliaEditMesh3dVertex *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh3d::meshes[pos]->getVertices()[index];
                
                if (argc == 4) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT &&
                        argv[3].a_type == A_FLOAT) {
                        
                        vertex.interpolate(ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float), argv[3].a_w.w_float);
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditMesh3dVertex::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditMesh3dVertex::objName);
                    return;
                }
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dVertex::objName);
    }
}

void ofeliaEditMesh3dVertex_limit(t_ofeliaEditMesh3dVertex *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh3d::meshes[pos]->getVertices()[index];
                vertex.limit(f);
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dVertex::objName);
    }
}

void ofeliaEditMesh3dVertex_map(t_ofeliaEditMesh3dVertex *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh3d::meshes[pos]->getVertices()[index];
                
                if (argc == 12) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT &&
                        argv[3].a_type == A_FLOAT &&
                        argv[4].a_type == A_FLOAT &&
                        argv[5].a_type == A_FLOAT &&
                        argv[6].a_type == A_FLOAT &&
                        argv[7].a_type == A_FLOAT &&
                        argv[8].a_type == A_FLOAT &&
                        argv[9].a_type == A_FLOAT &&
                        argv[10].a_type == A_FLOAT &&
                        argv[11].a_type == A_FLOAT) {
                        
                        vertex.map(ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float),
                                   ofVec3f(argv[3].a_w.w_float, argv[4].a_w.w_float, argv[5].a_w.w_float),
                                   ofVec3f(argv[6].a_w.w_float, argv[7].a_w.w_float, argv[8].a_w.w_float),
                                   ofVec3f(argv[9].a_w.w_float, argv[10].a_w.w_float, argv[11].a_w.w_float));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditMesh3dVertex::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditMesh3dVertex::objName);
                    return;
                }
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dVertex::objName);
    }
}

void ofeliaEditMesh3dVertex_middle(t_ofeliaEditMesh3dVertex *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh3d::meshes[pos]->getVertices()[index];
                
                if (argc == 3) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT) {
                        
                        vertex.middle(ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditMesh3dVertex::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditMesh3dVertex::objName);
                    return;
                }
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dVertex::objName);
    }
}

void ofeliaEditMesh3dVertex_normalize(t_ofeliaEditMesh3dVertex *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh3d::meshes[pos]->getVertices()[index];
                vertex.normalize();
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dVertex::objName);
    }
}

void ofeliaEditMesh3dVertex_perpendicular(t_ofeliaEditMesh3dVertex *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh3d::meshes[pos]->getVertices()[index];
                
                if (argc == 3) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT) {
                        
                        vertex.perpendicular(ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditMesh3dVertex::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditMesh3dVertex::objName);
                    return;
                }
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dVertex::objName);
    }
}

void ofeliaEditMesh3dVertex_rotate(t_ofeliaEditMesh3dVertex *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh3d::meshes[pos]->getVertices()[index];
                
                switch (argc) {
                        
                    case 3:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            vertex.rotate(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dVertex::objName);
                            return;
                        }
                        break;
                    }
                    case 4:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT &&
                            argv[3].a_type == A_FLOAT) {
                            
                            vertex.rotate(argv[0].a_w.w_float, ofVec3f(argv[1].a_w.w_float,
                                                                       argv[2].a_w.w_float,
                                                                       argv[3].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dVertex::objName);
                            return;
                        }
                        break;
                    }
                    case 7:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT &&
                            argv[3].a_type == A_FLOAT &&
                            argv[4].a_type == A_FLOAT &&
                            argv[5].a_type == A_FLOAT &&
                            argv[6].a_type == A_FLOAT) {
                            
                            vertex.rotate(argv[0].a_w.w_float,
                                          ofVec3f(argv[1].a_w.w_float,
                                                  argv[2].a_w.w_float,
                                                  argv[3].a_w.w_float),
                                          ofVec3f(argv[4].a_w.w_float,
                                                  argv[5].a_w.w_float,
                                                  argv[6].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dVertex::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh3dVertex::objName);
                        return;
                    }
                }
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dVertex::objName);
    }
}

void ofeliaEditMesh3dVertex_rotateRad(t_ofeliaEditMesh3dVertex *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh3d::meshes[pos]->getVertices()[index];
                
                switch (argc) {
                        
                    case 3:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            vertex.rotateRad(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dVertex::objName);
                            return;
                        }
                        break;
                    }
                    case 4:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT &&
                            argv[3].a_type == A_FLOAT) {
                            
                            vertex.rotateRad(argv[0].a_w.w_float, ofVec3f(argv[1].a_w.w_float,
                                                                          argv[2].a_w.w_float,
                                                                          argv[3].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dVertex::objName);
                            return;
                        }
                        break;
                    }
                    case 7:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT &&
                            argv[3].a_type == A_FLOAT &&
                            argv[4].a_type == A_FLOAT &&
                            argv[5].a_type == A_FLOAT &&
                            argv[6].a_type == A_FLOAT) {
                            
                            vertex.rotateRad(argv[0].a_w.w_float,
                                             ofVec3f(argv[1].a_w.w_float,
                                                     argv[2].a_w.w_float,
                                                     argv[3].a_w.w_float),
                                             ofVec3f(argv[4].a_w.w_float,
                                                     argv[5].a_w.w_float,
                                                     argv[6].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dVertex::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh3dVertex::objName);
                        return;
                    }
                }
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dVertex::objName);
    }
}

void ofeliaEditMesh3dVertex_scale(t_ofeliaEditMesh3dVertex *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumVertices())-1);
                ofVec3f &vertex = t_ofeliaLoadMesh3d::meshes[pos]->getVertices()[index];
                vertex.scale(f);
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dVertex::objName);
    }
}

void ofeliaEditMesh3dVertex_set(t_ofeliaEditMesh3dVertex *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaEditMesh3dVertex_print(t_ofeliaEditMesh3dVertex *x)
{
    post("\n[%s]", t_ofeliaEditMesh3dVertex::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaEditMesh3dVertex_setup()
{
    ofeliaEditMesh3dVertex_class = class_new(gensym("ofEditMesh3dVertex"),
                                             reinterpret_cast<t_newmethod>(ofeliaEditMesh3dVertex_new),
                                             0, sizeof(t_ofeliaEditMesh3dVertex),
                                             CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaEditMesh3dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh3dVertex_float));
    class_addmethod(ofeliaEditMesh3dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh3dVertex_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditMesh3dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh3dVertex_assign),
                    gensym("assign"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh3dVertex_assignX),
                    gensym("assignX"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh3dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh3dVertex_assignY),
                    gensym("assignY"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh3dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh3dVertex_assignZ),
                    gensym("assignZ"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh3dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh3dVertex_add),
                    gensym("add"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh3dVertex_sub),
                    gensym("sub"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh3dVertex_mult),
                    gensym("mult"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh3dVertex_div),
                    gensym("div"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh3dVertex_cross),
                    gensym("cross"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh3dVertex_lerp),
                    gensym("lerp"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh3dVertex_limit),
                    gensym("limit"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh3dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh3dVertex_map),
                    gensym("map"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh3dVertex_middle),
                    gensym("middle"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh3dVertex_normalize),
                    gensym("normalize"), A_NULL, 0);
    class_addmethod(ofeliaEditMesh3dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh3dVertex_perpendicular),
                    gensym("perpendicular"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh3dVertex_rotate),
                    gensym("rotate"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh3dVertex_rotateRad),
                    gensym("rotateRad"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh3dVertex_scale),
                    gensym("scale"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh3dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh3dVertex_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditMesh3dVertex_class, reinterpret_cast<t_method>(ofeliaEditMesh3dVertex_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofEditMesh2dIndex object methods
 */
void *ofeliaEditMesh2dIndex_new(t_symbol *s)
{
    t_ofeliaEditMesh2dIndex *x = reinterpret_cast<t_ofeliaEditMesh2dIndex*>(pd_new(ofeliaEditMesh2dIndex_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaEditMesh2dIndex_float(t_ofeliaEditMesh2dIndex *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaEditMesh2dIndex_assign(t_ofeliaEditMesh2dIndex *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasIndices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumIndices())-1);
                t_ofeliaLoadMesh2d::meshes[pos]->getIndices()[index] = f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dIndex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dIndex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dIndex::objName);
    }
}

void ofeliaEditMesh2dIndex_add(t_ofeliaEditMesh2dIndex *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasIndices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumIndices())-1);
                t_ofeliaLoadMesh2d::meshes[pos]->getIndices()[index] += f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dIndex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dIndex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dIndex::objName);
    }
}

void ofeliaEditMesh2dIndex_sub(t_ofeliaEditMesh2dIndex *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasIndices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumIndices())-1);
                t_ofeliaLoadMesh2d::meshes[pos]->getIndices()[index] -= f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dIndex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dIndex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dIndex::objName);
    }
}

void ofeliaEditMesh2dIndex_mult(t_ofeliaEditMesh2dIndex *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasIndices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumIndices())-1);
                t_ofeliaLoadMesh2d::meshes[pos]->getIndices()[index] *= f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dIndex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dIndex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dIndex::objName);
    }
}

void ofeliaEditMesh2dIndex_div(t_ofeliaEditMesh2dIndex *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasIndices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumIndices())-1);
                t_ofeliaLoadMesh2d::meshes[pos]->getIndices()[index] /= f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dIndex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dIndex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dIndex::objName);
    }
}

void ofeliaEditMesh2dIndex_lerp(t_ofeliaEditMesh2dIndex *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasIndices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumIndices())-1);
                ofIndexType &idx = t_ofeliaLoadMesh2d::meshes[pos]->getIndices()[index];
                
                if (argc == 2) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT) {
                        
                        const float p = argv[1].a_w.w_float;
                        idx = static_cast<ofIndexType>(idx * (1-p) + argv[0].a_w.w_float * p);
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditMesh2dIndex::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditMesh2dIndex::objName);
                    return;
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dIndex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dIndex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dIndex::objName);
    }
}

void ofeliaEditMesh2dIndex_middle(t_ofeliaEditMesh2dIndex *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasIndices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumIndices())-1);
                ofIndexType &idx = t_ofeliaLoadMesh2d::meshes[pos]->getIndices()[index];
                idx = static_cast<ofIndexType>((idx + f) * 0.5f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dIndex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dIndex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dIndex::objName);
    }
}

void ofeliaEditMesh2dIndex_set(t_ofeliaEditMesh2dIndex *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaEditMesh2dIndex_print(t_ofeliaEditMesh2dIndex *x)
{
    post("\n[%s]", t_ofeliaEditMesh2dIndex::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaEditMesh2dIndex_setup()
{
    ofeliaEditMesh2dIndex_class = class_new(gensym("ofEditMesh2dIndex"),
                                            reinterpret_cast<t_newmethod>(ofeliaEditMesh2dIndex_new),
                                            0, sizeof(t_ofeliaEditMesh2dIndex),
                                            CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaEditMesh2dIndex_class, reinterpret_cast<t_method>(ofeliaEditMesh2dIndex_float));
    class_addmethod(ofeliaEditMesh2dIndex_class, reinterpret_cast<t_method>(ofeliaEditMesh2dIndex_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditMesh2dIndex_class, reinterpret_cast<t_method>(ofeliaEditMesh2dIndex_assign),
                    gensym("assign"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh2dIndex_class, reinterpret_cast<t_method>(ofeliaEditMesh2dIndex_add),
                    gensym("add"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh2dIndex_class, reinterpret_cast<t_method>(ofeliaEditMesh2dIndex_sub),
                    gensym("sub"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh2dIndex_class, reinterpret_cast<t_method>(ofeliaEditMesh2dIndex_mult),
                    gensym("mult"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh2dIndex_class, reinterpret_cast<t_method>(ofeliaEditMesh2dIndex_div),
                    gensym("div"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh2dIndex_class, reinterpret_cast<t_method>(ofeliaEditMesh2dIndex_lerp),
                    gensym("lerp"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dIndex_class, reinterpret_cast<t_method>(ofeliaEditMesh2dIndex_middle),
                    gensym("middle"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh2dIndex_class, reinterpret_cast<t_method>(ofeliaEditMesh2dIndex_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditMesh2dIndex_class, reinterpret_cast<t_method>(ofeliaEditMesh2dIndex_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofEditMesh3dIndex object methods
 */
void *ofeliaEditMesh3dIndex_new(t_symbol *s)
{
    t_ofeliaEditMesh3dIndex *x = reinterpret_cast<t_ofeliaEditMesh3dIndex*>(pd_new(ofeliaEditMesh3dIndex_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaEditMesh3dIndex_float(t_ofeliaEditMesh3dIndex *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaEditMesh3dIndex_assign(t_ofeliaEditMesh3dIndex *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasIndices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumIndices())-1);
                t_ofeliaLoadMesh3d::meshes[pos]->getIndices()[index] = f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dIndex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dIndex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dIndex::objName);
    }
}

void ofeliaEditMesh3dIndex_add(t_ofeliaEditMesh3dIndex *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasIndices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumIndices())-1);
                t_ofeliaLoadMesh3d::meshes[pos]->getIndices()[index] += f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dIndex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dIndex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dIndex::objName);
    }
}

void ofeliaEditMesh3dIndex_sub(t_ofeliaEditMesh3dIndex *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasIndices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumIndices())-1);
                t_ofeliaLoadMesh3d::meshes[pos]->getIndices()[index] -= f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dIndex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dIndex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dIndex::objName);
    }
}

void ofeliaEditMesh3dIndex_mult(t_ofeliaEditMesh3dIndex *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasIndices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumIndices())-1);
                t_ofeliaLoadMesh3d::meshes[pos]->getIndices()[index] *= f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dIndex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dIndex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dIndex::objName);
    }
}

void ofeliaEditMesh3dIndex_div(t_ofeliaEditMesh3dIndex *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasIndices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumIndices())-1);
                t_ofeliaLoadMesh3d::meshes[pos]->getIndices()[index] /= f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dIndex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dIndex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dIndex::objName);
    }
}

void ofeliaEditMesh3dIndex_lerp(t_ofeliaEditMesh3dIndex *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasIndices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumIndices())-1);
                ofIndexType &idx = t_ofeliaLoadMesh3d::meshes[pos]->getIndices()[index];
                
                if (argc == 2) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT) {
                        
                        const float p = argv[1].a_w.w_float;
                        idx = static_cast<ofIndexType>(idx * (1-p) + argv[0].a_w.w_float * p);
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditMesh3dIndex::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditMesh3dIndex::objName);
                    return;
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dIndex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dIndex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dIndex::objName);
    }
}

void ofeliaEditMesh3dIndex_middle(t_ofeliaEditMesh3dIndex *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasIndices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumIndices())-1);
                ofIndexType &idx = t_ofeliaLoadMesh3d::meshes[pos]->getIndices()[index];
                idx = static_cast<ofIndexType>((idx + f) * 0.5f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dIndex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dIndex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dIndex::objName);
    }
}

void ofeliaEditMesh3dIndex_set(t_ofeliaEditMesh3dIndex *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaEditMesh3dIndex_print(t_ofeliaEditMesh3dIndex *x)
{
    post("\n[%s]", t_ofeliaEditMesh3dIndex::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaEditMesh3dIndex_setup()
{
    ofeliaEditMesh3dIndex_class = class_new(gensym("ofEditMesh3dIndex"),
                                            reinterpret_cast<t_newmethod>(ofeliaEditMesh3dIndex_new),
                                            0, sizeof(t_ofeliaEditMesh3dIndex),
                                            CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaEditMesh3dIndex_class, reinterpret_cast<t_method>(ofeliaEditMesh3dIndex_float));
    class_addmethod(ofeliaEditMesh3dIndex_class, reinterpret_cast<t_method>(ofeliaEditMesh3dIndex_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditMesh3dIndex_class, reinterpret_cast<t_method>(ofeliaEditMesh3dIndex_assign),
                    gensym("assign"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh3dIndex_class, reinterpret_cast<t_method>(ofeliaEditMesh3dIndex_add),
                    gensym("add"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh3dIndex_class, reinterpret_cast<t_method>(ofeliaEditMesh3dIndex_sub),
                    gensym("sub"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh3dIndex_class, reinterpret_cast<t_method>(ofeliaEditMesh3dIndex_mult),
                    gensym("mult"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh3dIndex_class, reinterpret_cast<t_method>(ofeliaEditMesh3dIndex_div),
                    gensym("div"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh3dIndex_class, reinterpret_cast<t_method>(ofeliaEditMesh3dIndex_lerp),
                    gensym("lerp"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dIndex_class, reinterpret_cast<t_method>(ofeliaEditMesh3dIndex_middle),
                    gensym("middle"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh3dIndex_class, reinterpret_cast<t_method>(ofeliaEditMesh3dIndex_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditMesh3dIndex_class, reinterpret_cast<t_method>(ofeliaEditMesh3dIndex_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofEditMesh2dNormal object methods
 */
void *ofeliaEditMesh2dNormal_new(t_symbol *s)
{
    t_ofeliaEditMesh2dNormal *x = reinterpret_cast<t_ofeliaEditMesh2dNormal*>(pd_new(ofeliaEditMesh2dNormal_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaEditMesh2dNormal_float(t_ofeliaEditMesh2dNormal *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaEditMesh2dNormal_assign(t_ofeliaEditMesh2dNormal *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh2d::meshes[pos]->getNormals()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh2dNormal::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            normal = ofVec2f(argv[0].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dNormal::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            normal = ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dNormal::objName);
                            return;
                        }
                        break;
                    }
                }
                t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dNormal::objName);
    }
}

void ofeliaEditMesh2dNormal_assignX(t_ofeliaEditMesh2dNormal *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh2d::meshes[pos]->getNormals()[index];
                normal.x = f;
                t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dNormal::objName);
    }
}

void ofeliaEditMesh2dNormal_assignY(t_ofeliaEditMesh2dNormal *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh2d::meshes[pos]->getNormals()[index];
                normal.y = f;
                t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dNormal::objName);
    }
}

void ofeliaEditMesh2dNormal_add(t_ofeliaEditMesh2dNormal *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh2d::meshes[pos]->getNormals()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh2dNormal::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            normal += argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dNormal::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            normal += ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dNormal::objName);
                            return;
                        }
                        break;
                    }
                }
                t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dNormal::objName);
    }
}

void ofeliaEditMesh2dNormal_sub(t_ofeliaEditMesh2dNormal *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh2d::meshes[pos]->getNormals()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh2dNormal::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            normal -= argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dNormal::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            normal -= ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dNormal::objName);
                            return;
                        }
                        break;
                    }
                }
                t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dNormal::objName);
    }
}

void ofeliaEditMesh2dNormal_mult(t_ofeliaEditMesh2dNormal *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh2d::meshes[pos]->getNormals()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh2dNormal::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            normal *= argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dNormal::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            normal *= ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dNormal::objName);
                            return;
                        }
                        break;
                    }
                }
                t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dNormal::objName);
    }
}

void ofeliaEditMesh2dNormal_div(t_ofeliaEditMesh2dNormal *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh2d::meshes[pos]->getNormals()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh2dNormal::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            normal /= argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dNormal::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            normal /= ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dNormal::objName);
                            return;
                        }
                        break;
                    }
                }
                t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dNormal::objName);
    }
}

void ofeliaEditMesh2dNormal_lerp(t_ofeliaEditMesh2dNormal *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh2d::meshes[pos]->getNormals()[index];
                
                if (argc == 3) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT) {
                        
                        normal.interpolate(ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float), argv[2].a_w.w_float);
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditMesh2dNormal::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditMesh2dNormal::objName);
                    return;
                }
                t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dNormal::objName);
    }
}

void ofeliaEditMesh2dNormal_limit(t_ofeliaEditMesh2dNormal *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh2d::meshes[pos]->getNormals()[index];
                normal.limit(f);
                t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dNormal::objName);
    }
}

void ofeliaEditMesh2dNormal_map(t_ofeliaEditMesh2dNormal *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh2d::meshes[pos]->getNormals()[index];
                
                if (argc == 6) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT &&
                        argv[3].a_type == A_FLOAT &&
                        argv[4].a_type == A_FLOAT &&
                        argv[5].a_type == A_FLOAT) {
                        
                        normal.map(ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, 0.0f),
                                   ofVec3f(argv[2].a_w.w_float, argv[3].a_w.w_float, 0.0f),
                                   ofVec3f(argv[4].a_w.w_float, argv[5].a_w.w_float, 0.0f),
                                   ofVec3f(0.0f, 0.0f, 0.0f));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditMesh2dNormal::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditMesh2dNormal::objName);
                    return;
                }
                t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dNormal::objName);
    }
}

void ofeliaEditMesh2dNormal_middle(t_ofeliaEditMesh2dNormal *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh2d::meshes[pos]->getNormals()[index];
                
                if (argc == 2) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT) {
                        
                        normal.middle(ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, 0.0f));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditMesh2dNormal::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditMesh2dNormal::objName);
                    return;
                }
                t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dNormal::objName);
    }
}

void ofeliaEditMesh2dNormal_normalize(t_ofeliaEditMesh2dNormal *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh2d::meshes[pos]->getNormals()[index];
                normal.normalize();
                t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dNormal::objName);
    }
}

void ofeliaEditMesh2dNormal_perpendicular(t_ofeliaEditMesh2dNormal *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh2d::meshes[pos]->getNormals()[index];
                /* ofVec2f::perpendicular() */
                const float length = static_cast<float>(sqrt(normal.x * normal.x + normal.y * normal.y));
                
                if (length > 0) {
                    
                    const float _x = normal.x;
                    normal.x = -(normal.y / length);
                    normal.y = _x/length;
                }
                t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dNormal::objName);
    }
}

void ofeliaEditMesh2dNormal_rotate(t_ofeliaEditMesh2dNormal *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh2d::meshes[pos]->getNormals()[index];
                
                switch (argc) {
                        
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            /* ofVec2f::rotate(float angle) */
                            const float angle = argv[0].a_w.w_float;
                            const float a = static_cast<float>(angle * DEG_TO_RAD);
                            const float xrot = normal.x * cos(a) - normal.y * sin(a);
                            normal.y = normal.x * sin(a) + normal.y * cos(a);
                            normal.x = xrot;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dNormal::objName);
                            return;
                        }
                        break;
                    }
                    case 3:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            /* ofVec2f::rotate(float angle, const ofVec2f& pivot) */
                            const float angle = argv[0].a_w.w_float;
                            const ofVec2f &pivot = ofVec2f(argv[1].a_w.w_float,
                                                           argv[2].a_w.w_float);
                            const float a = static_cast<float>(angle * DEG_TO_RAD);
                            const float xrot = ((normal.x-pivot.x)*cos(a) - (normal.y-pivot.y)*sin(a)) + pivot.x;
                            normal.y = ((normal.x-pivot.x)*sin(a) + (normal.y-pivot.y)*cos(a)) + pivot.y;
                            normal.x = xrot;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dNormal::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh2dNormal::objName);
                        return;
                    }
                }
                t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dNormal::objName);
    }
}

void ofeliaEditMesh2dNormal_rotateRad(t_ofeliaEditMesh2dNormal *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh2d::meshes[pos]->getNormals()[index];
                
                switch (argc) {
                        
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            /* ofVec2f::rotateRad(float angle) */
                            const float angle = argv[0].a_w.w_float;
                            const float a = angle;
                            float xrot = normal.x * cos(a) - normal.y * sin(a);
                            normal.y = normal.x * sin(a) + normal.y * cos(a);
                            normal.x = xrot;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dNormal::objName);
                            return;
                        }
                        break;
                    }
                    case 3:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            /* ofVec2f::rotateRad(float angle, const ofVec2f& pivot) */
                            const float angle = argv[0].a_w.w_float;
                            const ofVec2f &pivot = ofVec2f(argv[1].a_w.w_float,
                                                           argv[2].a_w.w_float);
                            const float a = angle;
                            const float xrot = ((normal.x-pivot.x)*cos(a) - (normal.y-pivot.y)*sin(a)) + pivot.x;
                            normal.y = ((normal.x-pivot.x)*sin(a) + (normal.y-pivot.y)*cos(a)) + pivot.y;
                            normal.x = xrot;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dNormal::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh2dNormal::objName);
                        return;
                    }
                }
                t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dNormal::objName);
    }
}

void ofeliaEditMesh2dNormal_scale(t_ofeliaEditMesh2dNormal *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh2d::meshes[pos]->getNormals()[index];
                normal.scale(f);
                t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dNormal::objName);
    }
}

void ofeliaEditMesh2dNormal_set(t_ofeliaEditMesh2dNormal *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaEditMesh2dNormal_print(t_ofeliaEditMesh2dNormal *x)
{
    post("\n[%s]", t_ofeliaEditMesh2dNormal::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaEditMesh2dNormal_setup()
{
    ofeliaEditMesh2dNormal_class = class_new(gensym("ofEditMesh2dNormal"),
                                             reinterpret_cast<t_newmethod>(ofeliaEditMesh2dNormal_new),
                                             0, sizeof(t_ofeliaEditMesh2dNormal),
                                             CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaEditMesh2dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh2dNormal_float));
    class_addmethod(ofeliaEditMesh2dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh2dNormal_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditMesh2dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh2dNormal_assign),
                    gensym("assign"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh2dNormal_assignX),
                    gensym("assignX"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh2dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh2dNormal_assignY),
                    gensym("assignY"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh2dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh2dNormal_add),
                    gensym("add"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh2dNormal_sub),
                    gensym("sub"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh2dNormal_mult),
                    gensym("mult"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh2dNormal_div),
                    gensym("div"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh2dNormal_lerp),
                    gensym("lerp"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh2dNormal_limit),
                    gensym("limit"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh2dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh2dNormal_map),
                    gensym("map"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh2dNormal_middle),
                    gensym("middle"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh2dNormal_normalize),
                    gensym("normalize"), A_NULL, 0);
    class_addmethod(ofeliaEditMesh2dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh2dNormal_perpendicular),
                    gensym("perpendicular"), A_NULL, 0);
    class_addmethod(ofeliaEditMesh2dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh2dNormal_rotate),
                    gensym("rotate"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh2dNormal_rotateRad),
                    gensym("rotateRad"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh2dNormal_scale),
                    gensym("scale"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh2dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh2dNormal_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditMesh2dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh2dNormal_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofEditMesh3dNormal object methods
 */
void *ofeliaEditMesh3dNormal_new(t_symbol *s)
{
    t_ofeliaEditMesh3dNormal *x = reinterpret_cast<t_ofeliaEditMesh3dNormal*>(pd_new(ofeliaEditMesh3dNormal_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaEditMesh3dNormal_float(t_ofeliaEditMesh3dNormal *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaEditMesh3dNormal_assign(t_ofeliaEditMesh3dNormal *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh3d::meshes[pos]->getNormals()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh3dNormal::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            normal = ofVec3f(argv[0].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dNormal::objName);
                            return;
                        }
                        break;
                    }
                    case 2:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            normal = ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dNormal::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            normal = ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dNormal::objName);
                            return;
                        }
                        break;
                    }
                }
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dNormal::objName);
    }
}

void ofeliaEditMesh3dNormal_assignX(t_ofeliaEditMesh3dNormal *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh3d::meshes[pos]->getNormals()[index];
                normal.x = f;
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dNormal::objName);
    }
}

void ofeliaEditMesh3dNormal_assignY(t_ofeliaEditMesh3dNormal *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh3d::meshes[pos]->getNormals()[index];
                normal.y = f;
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dNormal::objName);
    }
}

void ofeliaEditMesh3dNormal_assignZ(t_ofeliaEditMesh3dNormal *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh3d::meshes[pos]->getNormals()[index];
                normal.z = f;
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dNormal::objName);
    }
}

void ofeliaEditMesh3dNormal_add(t_ofeliaEditMesh3dNormal *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh3d::meshes[pos]->getNormals()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh3dNormal::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            normal += argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dNormal::objName);
                            return;
                        }
                        break;
                    }
                    case 2:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            normal += ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dNormal::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            normal += ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dNormal::objName);
                            return;
                        }
                        break;
                    }
                }
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dNormal::objName);
    }
}

void ofeliaEditMesh3dNormal_sub(t_ofeliaEditMesh3dNormal *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh3d::meshes[pos]->getNormals()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh3dNormal::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            normal -= argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dNormal::objName);
                            return;
                        }
                        break;
                    }
                    case 2:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            normal -= ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dNormal::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            normal -= ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dNormal::objName);
                            return;
                        }
                        break;
                    }
                }
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dNormal::objName);
    }
}

void ofeliaEditMesh3dNormal_mult(t_ofeliaEditMesh3dNormal *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh3d::meshes[pos]->getNormals()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh3dNormal::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            normal *= argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dNormal::objName);
                            return;
                        }
                        break;
                    }
                    case 2:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            normal *= ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dNormal::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            normal *= ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dNormal::objName);
                            return;
                        }
                        break;
                    }
                }
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dNormal::objName);
    }
}

void ofeliaEditMesh3dNormal_div(t_ofeliaEditMesh3dNormal *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh3d::meshes[pos]->getNormals()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh3dNormal::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            normal /= argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dNormal::objName);
                            return;
                        }
                        break;
                    }
                    case 2:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            normal /= ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dNormal::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            normal /= ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dNormal::objName);
                            return;
                        }
                        break;
                    }
                }
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dNormal::objName);
    }
}

void ofeliaEditMesh3dNormal_cross(t_ofeliaEditMesh3dNormal *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh3d::meshes[pos]->getNormals()[index];
                
                if (argc == 3) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT) {
                        
                        normal.cross(ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditMesh3dNormal::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditMesh3dNormal::objName);
                    return;
                }
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dNormal::objName);
    }
}

void ofeliaEditMesh3dNormal_lerp(t_ofeliaEditMesh3dNormal *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh3d::meshes[pos]->getNormals()[index];
                
                if (argc == 4) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT &&
                        argv[3].a_type == A_FLOAT) {
                        
                        normal.interpolate(ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float), argv[3].a_w.w_float);
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditMesh3dNormal::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditMesh3dNormal::objName);
                    return;
                }
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dNormal::objName);
    }
}

void ofeliaEditMesh3dNormal_limit(t_ofeliaEditMesh3dNormal *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh3d::meshes[pos]->getNormals()[index];
                normal.limit(f);
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dNormal::objName);
    }
}

void ofeliaEditMesh3dNormal_map(t_ofeliaEditMesh3dNormal *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh3d::meshes[pos]->getNormals()[index];
                
                if (argc == 12) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT &&
                        argv[3].a_type == A_FLOAT &&
                        argv[4].a_type == A_FLOAT &&
                        argv[5].a_type == A_FLOAT &&
                        argv[6].a_type == A_FLOAT &&
                        argv[7].a_type == A_FLOAT &&
                        argv[8].a_type == A_FLOAT &&
                        argv[9].a_type == A_FLOAT &&
                        argv[10].a_type == A_FLOAT &&
                        argv[11].a_type == A_FLOAT) {
                        
                        normal.map(ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float),
                                   ofVec3f(argv[3].a_w.w_float, argv[4].a_w.w_float, argv[5].a_w.w_float),
                                   ofVec3f(argv[6].a_w.w_float, argv[7].a_w.w_float, argv[8].a_w.w_float),
                                   ofVec3f(argv[9].a_w.w_float, argv[10].a_w.w_float, argv[11].a_w.w_float));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditMesh3dNormal::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditMesh3dNormal::objName);
                    return;
                }
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dNormal::objName);
    }
}

void ofeliaEditMesh3dNormal_middle(t_ofeliaEditMesh3dNormal *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh3d::meshes[pos]->getNormals()[index];
                
                if (argc == 3) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT) {
                        
                        normal.middle(ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditMesh3dNormal::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditMesh3dNormal::objName);
                    return;
                }
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dNormal::objName);
    }
}

void ofeliaEditMesh3dNormal_normalize(t_ofeliaEditMesh3dNormal *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh3d::meshes[pos]->getNormals()[index];
                normal.normalize();
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dNormal::objName);
    }
}

void ofeliaEditMesh3dNormal_perpendicular(t_ofeliaEditMesh3dNormal *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh3d::meshes[pos]->getNormals()[index];
                
                if (argc == 3) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT) {
                        
                        normal.perpendicular(ofVec3f(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditMesh3dNormal::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditMesh3dNormal::objName);
                    return;
                }
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dNormal::objName);
    }
}

void ofeliaEditMesh3dNormal_rotate(t_ofeliaEditMesh3dNormal *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh3d::meshes[pos]->getNormals()[index];
                
                switch (argc) {
                        
                    case 3:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            normal.rotate(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dNormal::objName);
                            return;
                        }
                        break;
                    }
                    case 4:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT &&
                            argv[3].a_type == A_FLOAT) {
                            
                            normal.rotate(argv[0].a_w.w_float, ofVec3f(argv[1].a_w.w_float,
                                                                       argv[2].a_w.w_float,
                                                                       argv[3].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dNormal::objName);
                            return;
                        }
                        break;
                    }
                    case 7:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT &&
                            argv[3].a_type == A_FLOAT &&
                            argv[4].a_type == A_FLOAT &&
                            argv[5].a_type == A_FLOAT &&
                            argv[6].a_type == A_FLOAT) {
                            
                            normal.rotate(argv[0].a_w.w_float,
                                          ofVec3f(argv[1].a_w.w_float,
                                                  argv[2].a_w.w_float,
                                                  argv[3].a_w.w_float),
                                          ofVec3f(argv[4].a_w.w_float,
                                                  argv[5].a_w.w_float,
                                                  argv[6].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dNormal::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh3dNormal::objName);
                        return;
                    }
                }
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dNormal::objName);
    }
}

void ofeliaEditMesh3dNormal_rotateRad(t_ofeliaEditMesh3dNormal *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh3d::meshes[pos]->getNormals()[index];
                
                switch (argc) {
                        
                    case 3:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            normal.rotateRad(argv[0].a_w.w_float, argv[1].a_w.w_float, argv[2].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dNormal::objName);
                            return;
                        }
                        break;
                    }
                    case 4:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT &&
                            argv[3].a_type == A_FLOAT) {
                            
                            normal.rotateRad(argv[0].a_w.w_float, ofVec3f(argv[1].a_w.w_float,
                                                                          argv[2].a_w.w_float,
                                                                          argv[3].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dNormal::objName);
                            return;
                        }
                        break;
                    }
                    case 7:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT &&
                            argv[3].a_type == A_FLOAT &&
                            argv[4].a_type == A_FLOAT &&
                            argv[5].a_type == A_FLOAT &&
                            argv[6].a_type == A_FLOAT) {
                            
                            normal.rotateRad(argv[0].a_w.w_float,
                                             ofVec3f(argv[1].a_w.w_float,
                                                     argv[2].a_w.w_float,
                                                     argv[3].a_w.w_float),
                                             ofVec3f(argv[4].a_w.w_float,
                                                     argv[5].a_w.w_float,
                                                     argv[6].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dNormal::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh3dNormal::objName);
                        return;
                    }
                }
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dNormal::objName);
    }
}

void ofeliaEditMesh3dNormal_scale(t_ofeliaEditMesh3dNormal *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumNormals())-1);
                ofVec3f &normal = t_ofeliaLoadMesh3d::meshes[pos]->getNormals()[index];
                normal.scale(f);
                t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties = true;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dNormal::objName);
    }
}

void ofeliaEditMesh3dNormal_set(t_ofeliaEditMesh3dNormal *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaEditMesh3dNormal_print(t_ofeliaEditMesh3dNormal *x)
{
    post("\n[%s]", t_ofeliaEditMesh3dNormal::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaEditMesh3dNormal_setup()
{
    ofeliaEditMesh3dNormal_class = class_new(gensym("ofEditMesh3dNormal"),
                                             reinterpret_cast<t_newmethod>(ofeliaEditMesh3dNormal_new),
                                             0, sizeof(t_ofeliaEditMesh3dNormal),
                                             CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaEditMesh3dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh3dNormal_float));
    class_addmethod(ofeliaEditMesh3dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh3dNormal_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditMesh3dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh3dNormal_assign),
                    gensym("assign"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh3dNormal_assignX),
                    gensym("assignX"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh3dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh3dNormal_assignY),
                    gensym("assignY"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh3dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh3dNormal_assignZ),
                    gensym("assignZ"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh3dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh3dNormal_add),
                    gensym("add"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh3dNormal_sub),
                    gensym("sub"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh3dNormal_mult),
                    gensym("mult"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh3dNormal_div),
                    gensym("div"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh3dNormal_cross),
                    gensym("cross"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh3dNormal_lerp),
                    gensym("lerp"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh3dNormal_limit),
                    gensym("limit"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh3dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh3dNormal_map),
                    gensym("map"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh3dNormal_middle),
                    gensym("middle"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh3dNormal_normalize),
                    gensym("normalize"), A_NULL, 0);
    class_addmethod(ofeliaEditMesh3dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh3dNormal_perpendicular),
                    gensym("perpendicular"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh3dNormal_rotate),
                    gensym("rotate"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh3dNormal_rotateRad),
                    gensym("rotateRad"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh3dNormal_scale),
                    gensym("scale"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh3dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh3dNormal_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditMesh3dNormal_class, reinterpret_cast<t_method>(ofeliaEditMesh3dNormal_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofEditMesh2dTexCoord object methods
 */
void *ofeliaEditMesh2dTexCoord_new(t_symbol *s)
{
    t_ofeliaEditMesh2dTexCoord *x = reinterpret_cast<t_ofeliaEditMesh2dTexCoord*>(pd_new(ofeliaEditMesh2dTexCoord_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaEditMesh2dTexCoord_float(t_ofeliaEditMesh2dTexCoord *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaEditMesh2dTexCoord_assign(t_ofeliaEditMesh2dTexCoord *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumTexCoords())-1);
                ofVec2f &texCoord = t_ofeliaLoadMesh2d::meshes[pos]->getTexCoords()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh2dTexCoord::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            texCoord = ofVec2f(argv[0].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dTexCoord::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            texCoord = ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dTexCoord::objName);
                            return;
                        }
                        break;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dTexCoord::objName);
    }
}

void ofeliaEditMesh2dTexCoord_assignX(t_ofeliaEditMesh2dTexCoord *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumTexCoords())-1);
                ofVec2f &texCoord = t_ofeliaLoadMesh2d::meshes[pos]->getTexCoords()[index];
                texCoord.x = f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dTexCoord::objName);
    }
}

void ofeliaEditMesh2dTexCoord_assignY(t_ofeliaEditMesh2dTexCoord *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumTexCoords())-1);
                ofVec2f &texCoord = t_ofeliaLoadMesh2d::meshes[pos]->getTexCoords()[index];
                texCoord.y = f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dTexCoord::objName);
    }
}

void ofeliaEditMesh2dTexCoord_add(t_ofeliaEditMesh2dTexCoord *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumTexCoords())-1);
                ofVec2f &texCoord = t_ofeliaLoadMesh2d::meshes[pos]->getTexCoords()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh2dTexCoord::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            texCoord += argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dTexCoord::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            texCoord += ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dTexCoord::objName);
                            return;
                        }
                        break;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dTexCoord::objName);
    }
}

void ofeliaEditMesh2dTexCoord_sub(t_ofeliaEditMesh2dTexCoord *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumTexCoords())-1);
                ofVec2f &texCoord = t_ofeliaLoadMesh2d::meshes[pos]->getTexCoords()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh2dTexCoord::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            texCoord -= argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dTexCoord::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            texCoord -= ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dTexCoord::objName);
                            return;
                        }
                        break;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dTexCoord::objName);
    }
}

void ofeliaEditMesh2dTexCoord_mult(t_ofeliaEditMesh2dTexCoord *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumTexCoords())-1);
                ofVec2f &texCoord = t_ofeliaLoadMesh2d::meshes[pos]->getTexCoords()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh2dTexCoord::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            texCoord *= argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dTexCoord::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            texCoord *= ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dTexCoord::objName);
                            return;
                        }
                        break;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dTexCoord::objName);
    }
}

void ofeliaEditMesh2dTexCoord_div(t_ofeliaEditMesh2dTexCoord *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumTexCoords())-1);
                ofVec2f &texCoord = t_ofeliaLoadMesh2d::meshes[pos]->getTexCoords()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh2dTexCoord::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            texCoord /= argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dTexCoord::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            texCoord /= ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dTexCoord::objName);
                            return;
                        }
                        break;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dTexCoord::objName);
    }
}

void ofeliaEditMesh2dTexCoord_lerp(t_ofeliaEditMesh2dTexCoord *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumTexCoords())-1);
                ofVec2f &texCoord = t_ofeliaLoadMesh2d::meshes[pos]->getTexCoords()[index];
                
                if (argc == 3) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT) {
                        
                        texCoord.interpolate(ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float), argv[2].a_w.w_float);
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditMesh2dTexCoord::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditMesh2dTexCoord::objName);
                    return;
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dTexCoord::objName);
    }
}

void ofeliaEditMesh2dTexCoord_limit(t_ofeliaEditMesh2dTexCoord *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumTexCoords())-1);
                ofVec2f &texCoord = t_ofeliaLoadMesh2d::meshes[pos]->getTexCoords()[index];
                texCoord.limit(f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dTexCoord::objName);
    }
}

void ofeliaEditMesh2dTexCoord_map(t_ofeliaEditMesh2dTexCoord *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumTexCoords())-1);
                ofVec2f &texCoord = t_ofeliaLoadMesh2d::meshes[pos]->getTexCoords()[index];
                
                if (argc == 6) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT &&
                        argv[3].a_type == A_FLOAT &&
                        argv[4].a_type == A_FLOAT &&
                        argv[5].a_type == A_FLOAT) {
                        
                        texCoord.map(ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float),
                                     ofVec2f(argv[2].a_w.w_float, argv[3].a_w.w_float),
                                     ofVec2f(argv[4].a_w.w_float, argv[5].a_w.w_float));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditMesh2dTexCoord::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditMesh2dTexCoord::objName);
                    return;
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dTexCoord::objName);
    }
}

void ofeliaEditMesh2dTexCoord_middle(t_ofeliaEditMesh2dTexCoord *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumTexCoords())-1);
                ofVec2f &texCoord = t_ofeliaLoadMesh2d::meshes[pos]->getTexCoords()[index];
                
                if (argc == 2) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT) {
                        
                        texCoord.middle(ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditMesh2dTexCoord::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditMesh2dTexCoord::objName);
                    return;
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dTexCoord::objName);
    }
}

void ofeliaEditMesh2dTexCoord_normalize(t_ofeliaEditMesh2dTexCoord *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumTexCoords())-1);
                ofVec2f &texCoord = t_ofeliaLoadMesh2d::meshes[pos]->getTexCoords()[index];
                texCoord.normalize();
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dTexCoord::objName);
    }
}

void ofeliaEditMesh2dTexCoord_perpendicular(t_ofeliaEditMesh2dTexCoord *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumTexCoords())-1);
                ofVec2f &texCoord = t_ofeliaLoadMesh2d::meshes[pos]->getTexCoords()[index];
                texCoord.perpendicular();
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dTexCoord::objName);
    }
}

void ofeliaEditMesh2dTexCoord_rotate(t_ofeliaEditMesh2dTexCoord *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumTexCoords())-1);
                ofVec2f &texCoord = t_ofeliaLoadMesh2d::meshes[pos]->getTexCoords()[index];
                
                switch (argc) {
                        
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            texCoord.rotate(argv[0].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dTexCoord::objName);
                            return;
                        }
                        break;
                    }
                    case 3:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            texCoord.rotate(argv[0].a_w.w_float, ofVec2f(argv[1].a_w.w_float,
                                                                         argv[2].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dTexCoord::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh2dTexCoord::objName);
                        return;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dTexCoord::objName);
    }
}

void ofeliaEditMesh2dTexCoord_rotateRad(t_ofeliaEditMesh2dTexCoord *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumTexCoords())-1);
                ofVec2f &texCoord = t_ofeliaLoadMesh2d::meshes[pos]->getTexCoords()[index];
                
                switch (argc) {
                        
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            texCoord.rotateRad(argv[0].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dTexCoord::objName);
                            return;
                        }
                        break;
                    }
                    case 3:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            texCoord.rotateRad(argv[0].a_w.w_float, ofVec2f(argv[1].a_w.w_float,
                                                                            argv[2].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dTexCoord::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh2dTexCoord::objName);
                        return;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dTexCoord::objName);
    }
}

void ofeliaEditMesh2dTexCoord_scale(t_ofeliaEditMesh2dTexCoord *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumTexCoords())-1);
                ofVec2f &texCoord = t_ofeliaLoadMesh2d::meshes[pos]->getTexCoords()[index];
                texCoord.scale(f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dTexCoord::objName);
    }
}

void ofeliaEditMesh2dTexCoord_set(t_ofeliaEditMesh2dTexCoord *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaEditMesh2dTexCoord_print(t_ofeliaEditMesh2dTexCoord *x)
{
    post("\n[%s]", t_ofeliaEditMesh2dTexCoord::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaEditMesh2dTexCoord_setup()
{
    ofeliaEditMesh2dTexCoord_class = class_new(gensym("ofEditMesh2dTexCoord"),
                                               reinterpret_cast<t_newmethod>(ofeliaEditMesh2dTexCoord_new),
                                               0, sizeof(t_ofeliaEditMesh2dTexCoord),
                                               CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaEditMesh2dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh2dTexCoord_float));
    class_addmethod(ofeliaEditMesh2dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh2dTexCoord_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditMesh2dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh2dTexCoord_assign),
                    gensym("assign"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh2dTexCoord_assignX),
                    gensym("assignX"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh2dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh2dTexCoord_assignY),
                    gensym("assignY"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh2dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh2dTexCoord_add),
                    gensym("add"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh2dTexCoord_sub),
                    gensym("sub"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh2dTexCoord_mult),
                    gensym("mult"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh2dTexCoord_div),
                    gensym("div"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh2dTexCoord_lerp),
                    gensym("lerp"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh2dTexCoord_limit),
                    gensym("limit"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh2dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh2dTexCoord_map),
                    gensym("map"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh2dTexCoord_middle),
                    gensym("middle"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh2dTexCoord_normalize),
                    gensym("normalize"), A_NULL, 0);
    class_addmethod(ofeliaEditMesh2dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh2dTexCoord_perpendicular),
                    gensym("perpendicular"), A_NULL, 0);
    class_addmethod(ofeliaEditMesh2dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh2dTexCoord_rotate),
                    gensym("rotate"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh2dTexCoord_rotateRad),
                    gensym("rotateRad"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh2dTexCoord_scale),
                    gensym("scale"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh2dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh2dTexCoord_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditMesh2dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh2dTexCoord_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofEditMesh3dTexCoord object methods
 */
void *ofeliaEditMesh3dTexCoord_new(t_symbol *s)
{
    t_ofeliaEditMesh3dTexCoord *x = reinterpret_cast<t_ofeliaEditMesh3dTexCoord*>(pd_new(ofeliaEditMesh3dTexCoord_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaEditMesh3dTexCoord_float(t_ofeliaEditMesh3dTexCoord *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaEditMesh3dTexCoord_assign(t_ofeliaEditMesh3dTexCoord *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumTexCoords())-1);
                ofVec2f &texCoord = t_ofeliaLoadMesh3d::meshes[pos]->getTexCoords()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh3dTexCoord::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            texCoord = ofVec2f(argv[0].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dTexCoord::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            texCoord = ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dTexCoord::objName);
                            return;
                        }
                        break;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dTexCoord::objName);
    }
}

void ofeliaEditMesh3dTexCoord_assignX(t_ofeliaEditMesh3dTexCoord *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumTexCoords())-1);
                ofVec2f &texCoord = t_ofeliaLoadMesh3d::meshes[pos]->getTexCoords()[index];
                texCoord.x = f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dTexCoord::objName);
    }
}

void ofeliaEditMesh3dTexCoord_assignY(t_ofeliaEditMesh3dTexCoord *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumTexCoords())-1);
                ofVec2f &texCoord = t_ofeliaLoadMesh3d::meshes[pos]->getTexCoords()[index];
                texCoord.y = f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dTexCoord::objName);
    }
}

void ofeliaEditMesh3dTexCoord_add(t_ofeliaEditMesh3dTexCoord *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumTexCoords())-1);
                ofVec2f &texCoord = t_ofeliaLoadMesh3d::meshes[pos]->getTexCoords()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh3dTexCoord::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            texCoord += argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dTexCoord::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            texCoord += ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dTexCoord::objName);
                            return;
                        }
                        break;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dTexCoord::objName);
    }
}

void ofeliaEditMesh3dTexCoord_sub(t_ofeliaEditMesh3dTexCoord *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumTexCoords())-1);
                ofVec2f &texCoord = t_ofeliaLoadMesh3d::meshes[pos]->getTexCoords()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh3dTexCoord::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            texCoord -= argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dTexCoord::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            texCoord -= ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dTexCoord::objName);
                            return;
                        }
                        break;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dTexCoord::objName);
    }
}

void ofeliaEditMesh3dTexCoord_mult(t_ofeliaEditMesh3dTexCoord *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumTexCoords())-1);
                ofVec2f &texCoord = t_ofeliaLoadMesh3d::meshes[pos]->getTexCoords()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh3dTexCoord::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            texCoord *= argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dTexCoord::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            texCoord *= ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dTexCoord::objName);
                            return;
                        }
                        break;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dTexCoord::objName);
    }
}

void ofeliaEditMesh3dTexCoord_div(t_ofeliaEditMesh3dTexCoord *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumTexCoords())-1);
                ofVec2f &texCoord = t_ofeliaLoadMesh3d::meshes[pos]->getTexCoords()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh3dTexCoord::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            texCoord /= argv[0].a_w.w_float;
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dTexCoord::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            texCoord /= ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dTexCoord::objName);
                            return;
                        }
                        break;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dTexCoord::objName);
    }
}

void ofeliaEditMesh3dTexCoord_lerp(t_ofeliaEditMesh3dTexCoord *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumTexCoords())-1);
                ofVec2f &texCoord = t_ofeliaLoadMesh3d::meshes[pos]->getTexCoords()[index];
                
                if (argc == 3) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT) {
                        
                        texCoord.interpolate(ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float), argv[2].a_w.w_float);
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditMesh3dTexCoord::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditMesh3dTexCoord::objName);
                    return;
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dTexCoord::objName);
    }
}

void ofeliaEditMesh3dTexCoord_limit(t_ofeliaEditMesh3dTexCoord *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumTexCoords())-1);
                ofVec2f &texCoord = t_ofeliaLoadMesh3d::meshes[pos]->getTexCoords()[index];
                texCoord.limit(f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dTexCoord::objName);
    }
}

void ofeliaEditMesh3dTexCoord_map(t_ofeliaEditMesh3dTexCoord *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumTexCoords())-1);
                ofVec2f &texCoord = t_ofeliaLoadMesh3d::meshes[pos]->getTexCoords()[index];
                
                if (argc == 6) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT &&
                        argv[3].a_type == A_FLOAT &&
                        argv[4].a_type == A_FLOAT &&
                        argv[5].a_type == A_FLOAT) {
                        
                        texCoord.map(ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float),
                                     ofVec2f(argv[2].a_w.w_float, argv[3].a_w.w_float),
                                     ofVec2f(argv[4].a_w.w_float, argv[5].a_w.w_float));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditMesh3dTexCoord::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditMesh3dTexCoord::objName);
                    return;
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dTexCoord::objName);
    }
}

void ofeliaEditMesh3dTexCoord_middle(t_ofeliaEditMesh3dTexCoord *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumTexCoords())-1);
                ofVec2f &texCoord = t_ofeliaLoadMesh3d::meshes[pos]->getTexCoords()[index];
                
                if (argc == 2) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT) {
                        
                        texCoord.middle(ofVec2f(argv[0].a_w.w_float, argv[1].a_w.w_float));
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditMesh3dTexCoord::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditMesh3dTexCoord::objName);
                    return;
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dTexCoord::objName);
    }
}

void ofeliaEditMesh3dTexCoord_normalize(t_ofeliaEditMesh3dTexCoord *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumTexCoords())-1);
                ofVec2f &texCoord = t_ofeliaLoadMesh3d::meshes[pos]->getTexCoords()[index];
                texCoord.normalize();
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dTexCoord::objName);
    }
}

void ofeliaEditMesh3dTexCoord_perpendicular(t_ofeliaEditMesh3dTexCoord *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumTexCoords())-1);
                ofVec2f &texCoord = t_ofeliaLoadMesh3d::meshes[pos]->getTexCoords()[index];
                texCoord.perpendicular();
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dTexCoord::objName);
    }
}

void ofeliaEditMesh3dTexCoord_rotate(t_ofeliaEditMesh3dTexCoord *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumTexCoords())-1);
                ofVec2f &texCoord = t_ofeliaLoadMesh3d::meshes[pos]->getTexCoords()[index];
                
                switch (argc) {
                        
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            texCoord.rotate(argv[0].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dTexCoord::objName);
                            return;
                        }
                        break;
                    }
                    case 3:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            texCoord.rotate(argv[0].a_w.w_float, ofVec2f(argv[1].a_w.w_float,
                                                                         argv[2].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dTexCoord::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh3dTexCoord::objName);
                        return;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dTexCoord::objName);
    }
}

void ofeliaEditMesh3dTexCoord_rotateRad(t_ofeliaEditMesh3dTexCoord *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumTexCoords())-1);
                ofVec2f &texCoord = t_ofeliaLoadMesh3d::meshes[pos]->getTexCoords()[index];
                
                switch (argc) {
                        
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            texCoord.rotateRad(argv[0].a_w.w_float);
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dTexCoord::objName);
                            return;
                        }
                        break;
                    }
                    case 3:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            texCoord.rotateRad(argv[0].a_w.w_float, ofVec2f(argv[1].a_w.w_float,
                                                                            argv[2].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dTexCoord::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh3dTexCoord::objName);
                        return;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dTexCoord::objName);
    }
}

void ofeliaEditMesh3dTexCoord_scale(t_ofeliaEditMesh3dTexCoord *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumTexCoords())-1);
                ofVec2f &texCoord = t_ofeliaLoadMesh3d::meshes[pos]->getTexCoords()[index];
                texCoord.scale(f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dTexCoord::objName);
    }
}

void ofeliaEditMesh3dTexCoord_set(t_ofeliaEditMesh3dTexCoord *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaEditMesh3dTexCoord_print(t_ofeliaEditMesh3dTexCoord *x)
{
    post("\n[%s]", t_ofeliaEditMesh3dTexCoord::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaEditMesh3dTexCoord_setup()
{
    ofeliaEditMesh3dTexCoord_class = class_new(gensym("ofEditMesh3dTexCoord"),
                                               reinterpret_cast<t_newmethod>(ofeliaEditMesh3dTexCoord_new),
                                               0, sizeof(t_ofeliaEditMesh3dTexCoord),
                                               CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaEditMesh3dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh3dTexCoord_float));
    class_addmethod(ofeliaEditMesh3dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh3dTexCoord_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditMesh3dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh3dTexCoord_assign),
                    gensym("assign"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh3dTexCoord_assignX),
                    gensym("assignX"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh3dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh3dTexCoord_assignY),
                    gensym("assignY"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh3dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh3dTexCoord_add),
                    gensym("add"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh3dTexCoord_sub),
                    gensym("sub"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh3dTexCoord_mult),
                    gensym("mult"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh3dTexCoord_div),
                    gensym("div"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh3dTexCoord_lerp),
                    gensym("lerp"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh3dTexCoord_limit),
                    gensym("limit"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh3dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh3dTexCoord_map),
                    gensym("map"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh3dTexCoord_middle),
                    gensym("middle"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh3dTexCoord_normalize),
                    gensym("normalize"), A_NULL, 0);
    class_addmethod(ofeliaEditMesh3dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh3dTexCoord_perpendicular),
                    gensym("perpendicular"), A_NULL, 0);
    class_addmethod(ofeliaEditMesh3dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh3dTexCoord_rotate),
                    gensym("rotate"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh3dTexCoord_rotateRad),
                    gensym("rotateRad"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh3dTexCoord_scale),
                    gensym("scale"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh3dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh3dTexCoord_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditMesh3dTexCoord_class, reinterpret_cast<t_method>(ofeliaEditMesh3dTexCoord_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofEditMesh2dColor object methods
 */
void *ofeliaEditMesh2dColor_new(t_symbol *s)
{
    t_ofeliaEditMesh2dColor *x = reinterpret_cast<t_ofeliaEditMesh2dColor*>(pd_new(ofeliaEditMesh2dColor_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaEditMesh2dColor_float(t_ofeliaEditMesh2dColor *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaEditMesh2dColor_assign(t_ofeliaEditMesh2dColor *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumColors())-1);
                ofFloatColor &color = t_ofeliaLoadMesh2d::meshes[pos]->getColors()[index];
                
                if (!getFloatColorAlphaFromArgs(argc, argv, color, t_ofeliaEditMesh2dColor::objName))
                    return;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dColor::objName);
    }
}

void ofeliaEditMesh2dColor_assignR(t_ofeliaEditMesh2dColor *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumColors())-1);
                ofFloatColor &color = t_ofeliaLoadMesh2d::meshes[pos]->getColors()[index];
                color.r = ofWrap(f / 255.0f, 0.0f, 1.0f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dColor::objName);
    }
}

void ofeliaEditMesh2dColor_assignG(t_ofeliaEditMesh2dColor *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumColors())-1);
                ofFloatColor &color = t_ofeliaLoadMesh2d::meshes[pos]->getColors()[index];
                color.g = ofWrap(f / 255.0f, 0.0f, 1.0f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dColor::objName);
    }
}

void ofeliaEditMesh2dColor_assignB(t_ofeliaEditMesh2dColor *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumColors())-1);
                ofFloatColor &color = t_ofeliaLoadMesh2d::meshes[pos]->getColors()[index];
                color.b = ofWrap(f / 255.0f, 0.0f, 1.0f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dColor::objName);
    }
}

void ofeliaEditMesh2dColor_assignA(t_ofeliaEditMesh2dColor *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumColors())-1);
                ofFloatColor &color = t_ofeliaLoadMesh2d::meshes[pos]->getColors()[index];
                color.a = ofWrap(f / 255.0f, 0.0f, 1.0f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dColor::objName);
    }
}

void ofeliaEditMesh2dColor_add(t_ofeliaEditMesh2dColor *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumColors())-1);
                ofFloatColor &color = t_ofeliaLoadMesh2d::meshes[pos]->getColors()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh2dColor::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            color += ofColor(static_cast<unsigned char>(argv[0].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dColor::objName);
                            return;
                        }
                        break;
                    }
                    case 2:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            color += ofColor(static_cast<unsigned char>(argv[0].a_w.w_float),
                                             static_cast<unsigned char>(argv[1].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dColor::objName);
                            return;
                        }
                        break;
                    }
                    case 3:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            color += ofColor(static_cast<unsigned char>(argv[0].a_w.w_float),
                                             static_cast<unsigned char>(argv[1].a_w.w_float),
                                             static_cast<unsigned char>(argv[2].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dColor::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT &&
                            argv[3].a_type == A_FLOAT) {
                            
                            color += ofColor(static_cast<unsigned char>(argv[0].a_w.w_float),
                                             static_cast<unsigned char>(argv[1].a_w.w_float),
                                             static_cast<unsigned char>(argv[2].a_w.w_float),
                                             static_cast<unsigned char>(argv[3].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dColor::objName);
                            return;
                        }
                        break;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dColor::objName);
    }
}

void ofeliaEditMesh2dColor_sub(t_ofeliaEditMesh2dColor *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumColors())-1);
                ofFloatColor &color = t_ofeliaLoadMesh2d::meshes[pos]->getColors()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh2dColor::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            color -= ofColor(static_cast<unsigned char>(argv[0].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dColor::objName);
                            return;
                        }
                        break;
                    }
                    case 2:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            color -= ofColor(static_cast<unsigned char>(argv[0].a_w.w_float),
                                             static_cast<unsigned char>(argv[1].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dColor::objName);
                            return;
                        }
                        break;
                    }
                    case 3:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            color -= ofColor(static_cast<unsigned char>(argv[0].a_w.w_float),
                                             static_cast<unsigned char>(argv[1].a_w.w_float),
                                             static_cast<unsigned char>(argv[2].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dColor::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT &&
                            argv[3].a_type == A_FLOAT) {
                            
                            color -= ofColor(static_cast<unsigned char>(argv[0].a_w.w_float),
                                             static_cast<unsigned char>(argv[1].a_w.w_float),
                                             static_cast<unsigned char>(argv[2].a_w.w_float),
                                             static_cast<unsigned char>(argv[3].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh2dColor::objName);
                            return;
                        }
                        break;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dColor::objName);
    }
}

void ofeliaEditMesh2dColor_mult(t_ofeliaEditMesh2dColor *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumColors())-1);
                ofFloatColor &color = t_ofeliaLoadMesh2d::meshes[pos]->getColors()[index];
                color *= f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dColor::objName);
    }
}

void ofeliaEditMesh2dColor_div(t_ofeliaEditMesh2dColor *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumColors())-1);
                ofFloatColor &color = t_ofeliaLoadMesh2d::meshes[pos]->getColors()[index];
                color /= f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dColor::objName);
    }
}

void ofeliaEditMesh2dColor_invert(t_ofeliaEditMesh2dColor *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumColors())-1);
                ofFloatColor &color = t_ofeliaLoadMesh2d::meshes[pos]->getColors()[index];
                color.invert();
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dColor::objName);
    }
}

void ofeliaEditMesh2dColor_lerp(t_ofeliaEditMesh2dColor *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumColors())-1);
                ofFloatColor &color = t_ofeliaLoadMesh2d::meshes[pos]->getColors()[index];
                
                if (argc == 5) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT &&
                        argv[3].a_type == A_FLOAT &&
                        argv[4].a_type == A_FLOAT) {
                        
                        color.lerp(ofColor(static_cast<unsigned char>(argv[0].a_w.w_float),
                                           static_cast<unsigned char>(argv[1].a_w.w_float),
                                           static_cast<unsigned char>(argv[2].a_w.w_float),
                                           static_cast<unsigned char>(argv[3].a_w.w_float)),
                                   argv[4].a_w.w_float);
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditMesh2dColor::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditMesh2dColor::objName);
                    return;
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dColor::objName);
    }
}

void ofeliaEditMesh2dColor_normalize(t_ofeliaEditMesh2dColor *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumColors())-1);
                ofFloatColor &color = t_ofeliaLoadMesh2d::meshes[pos]->getColors()[index];
                color.normalize();
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dColor::objName);
    }
}

void ofeliaEditMesh2dColor_hue(t_ofeliaEditMesh2dColor *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumColors())-1);
                ofFloatColor &color = t_ofeliaLoadMesh2d::meshes[pos]->getColors()[index];
                color.setHue(f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dColor::objName);
    }
}

void ofeliaEditMesh2dColor_hueAngle(t_ofeliaEditMesh2dColor *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumColors())-1);
                ofFloatColor &color = t_ofeliaLoadMesh2d::meshes[pos]->getColors()[index];
                color.setHueAngle(f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dColor::objName);
    }
}

void ofeliaEditMesh2dColor_brightness(t_ofeliaEditMesh2dColor *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumColors())-1);
                ofFloatColor &color = t_ofeliaLoadMesh2d::meshes[pos]->getColors()[index];
                color.setBrightness(f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dColor::objName);
    }
}

void ofeliaEditMesh2dColor_saturation(t_ofeliaEditMesh2dColor *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumColors())-1);
                ofFloatColor &color = t_ofeliaLoadMesh2d::meshes[pos]->getColors()[index];
                color.setSaturation(f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh2dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh2dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh2dColor::objName);
    }
}

void ofeliaEditMesh2dColor_set(t_ofeliaEditMesh2dColor *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaEditMesh2dColor_print(t_ofeliaEditMesh2dColor *x)
{
    post("\n[%s]", t_ofeliaEditMesh2dColor::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaEditMesh2dColor_setup()
{
    ofeliaEditMesh2dColor_class = class_new(gensym("ofEditMesh2dColor"),
                                            reinterpret_cast<t_newmethod>(ofeliaEditMesh2dColor_new),
                                            0, sizeof(t_ofeliaEditMesh2dColor),
                                            CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaEditMesh2dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh2dColor_float));
    class_addmethod(ofeliaEditMesh2dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh2dColor_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditMesh2dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh2dColor_assign),
                    gensym("assign"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh2dColor_assignR),
                    gensym("assignR"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh2dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh2dColor_assignG),
                    gensym("assignG"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh2dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh2dColor_assignB),
                    gensym("assignB"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh2dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh2dColor_assignA),
                    gensym("assignA"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh2dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh2dColor_add),
                    gensym("add"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh2dColor_sub),
                    gensym("sub"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh2dColor_mult),
                    gensym("mult"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh2dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh2dColor_div),
                    gensym("div"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh2dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh2dColor_invert),
                    gensym("invert"), A_NULL, 0);
    class_addmethod(ofeliaEditMesh2dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh2dColor_lerp),
                    gensym("lerp"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh2dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh2dColor_normalize),
                    gensym("normalize"), A_NULL, 0);
    class_addmethod(ofeliaEditMesh2dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh2dColor_hue),
                    gensym("hue"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh2dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh2dColor_hueAngle),
                    gensym("hueAngle"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh2dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh2dColor_brightness),
                    gensym("brightness"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh2dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh2dColor_saturation),
                    gensym("saturation"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh2dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh2dColor_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditMesh2dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh2dColor_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofEditMesh3dColor object methods
 */
void *ofeliaEditMesh3dColor_new(t_symbol *s)
{
    t_ofeliaEditMesh3dColor *x = reinterpret_cast<t_ofeliaEditMesh3dColor*>(pd_new(ofeliaEditMesh3dColor_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_bang);
    return (x);
}

void ofeliaEditMesh3dColor_float(t_ofeliaEditMesh3dColor *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaEditMesh3dColor_assign(t_ofeliaEditMesh3dColor *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumColors())-1);
                ofFloatColor &color = t_ofeliaLoadMesh3d::meshes[pos]->getColors()[index];
                
                if (!getFloatColorAlphaFromArgs(argc, argv, color, t_ofeliaEditMesh3dColor::objName))
                    return;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dColor::objName);
    }
}

void ofeliaEditMesh3dColor_assignR(t_ofeliaEditMesh3dColor *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumColors())-1);
                ofFloatColor &color = t_ofeliaLoadMesh3d::meshes[pos]->getColors()[index];
                color.r = ofWrap(f / 255.0f, 0.0f, 1.0f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dColor::objName);
    }
}

void ofeliaEditMesh3dColor_assignG(t_ofeliaEditMesh3dColor *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumColors())-1);
                ofFloatColor &color = t_ofeliaLoadMesh3d::meshes[pos]->getColors()[index];
                color.g = ofWrap(f / 255.0f, 0.0f, 1.0f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dColor::objName);
    }
}

void ofeliaEditMesh3dColor_assignB(t_ofeliaEditMesh3dColor *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumColors())-1);
                ofFloatColor &color = t_ofeliaLoadMesh3d::meshes[pos]->getColors()[index];
                color.b = ofWrap(f / 255.0f, 0.0f, 1.0f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dColor::objName);
    }
}

void ofeliaEditMesh3dColor_assignA(t_ofeliaEditMesh3dColor *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumColors())-1);
                ofFloatColor &color = t_ofeliaLoadMesh3d::meshes[pos]->getColors()[index];
                color.a = ofWrap(f / 255.0f, 0.0f, 1.0f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dColor::objName);
    }
}

void ofeliaEditMesh3dColor_add(t_ofeliaEditMesh3dColor *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumColors())-1);
                ofFloatColor &color = t_ofeliaLoadMesh3d::meshes[pos]->getColors()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh3dColor::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            color += ofColor(static_cast<unsigned char>(argv[0].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dColor::objName);
                            return;
                        }
                        break;
                    }
                    case 2:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            color += ofColor(static_cast<unsigned char>(argv[0].a_w.w_float),
                                             static_cast<unsigned char>(argv[1].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dColor::objName);
                            return;
                        }
                        break;
                    }
                    case 3:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            color += ofColor(static_cast<unsigned char>(argv[0].a_w.w_float),
                                             static_cast<unsigned char>(argv[1].a_w.w_float),
                                             static_cast<unsigned char>(argv[2].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dColor::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT &&
                            argv[3].a_type == A_FLOAT) {
                            
                            color += ofColor(static_cast<unsigned char>(argv[0].a_w.w_float),
                                             static_cast<unsigned char>(argv[1].a_w.w_float),
                                             static_cast<unsigned char>(argv[2].a_w.w_float),
                                             static_cast<unsigned char>(argv[3].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dColor::objName);
                            return;
                        }
                        break;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dColor::objName);
    }
}

void ofeliaEditMesh3dColor_sub(t_ofeliaEditMesh3dColor *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumColors())-1);
                ofFloatColor &color = t_ofeliaLoadMesh3d::meshes[pos]->getColors()[index];
                
                switch (argc) {
                        
                    case 0:
                    {
                        error("%s: invalid number of arguments", t_ofeliaEditMesh3dColor::objName);
                        return;
                    }
                    case 1:
                    {
                        if (argv[0].a_type == A_FLOAT) {
                            
                            color -= ofColor(static_cast<unsigned char>(argv[0].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dColor::objName);
                            return;
                        }
                        break;
                    }
                    case 2:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT) {
                            
                            color -= ofColor(static_cast<unsigned char>(argv[0].a_w.w_float),
                                             static_cast<unsigned char>(argv[1].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dColor::objName);
                            return;
                        }
                        break;
                    }
                    case 3:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT) {
                            
                            color -= ofColor(static_cast<unsigned char>(argv[0].a_w.w_float),
                                             static_cast<unsigned char>(argv[1].a_w.w_float),
                                             static_cast<unsigned char>(argv[2].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dColor::objName);
                            return;
                        }
                        break;
                    }
                    default:
                    {
                        if (argv[0].a_type == A_FLOAT &&
                            argv[1].a_type == A_FLOAT &&
                            argv[2].a_type == A_FLOAT &&
                            argv[3].a_type == A_FLOAT) {
                            
                            color -= ofColor(static_cast<unsigned char>(argv[0].a_w.w_float),
                                             static_cast<unsigned char>(argv[1].a_w.w_float),
                                             static_cast<unsigned char>(argv[2].a_w.w_float),
                                             static_cast<unsigned char>(argv[3].a_w.w_float));
                        }
                        else {
                            
                            error("%s: wrong argument type", t_ofeliaEditMesh3dColor::objName);
                            return;
                        }
                        break;
                    }
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dColor::objName);
    }
}

void ofeliaEditMesh3dColor_mult(t_ofeliaEditMesh3dColor *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumColors())-1);
                ofFloatColor &color = t_ofeliaLoadMesh3d::meshes[pos]->getColors()[index];
                color *= f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dColor::objName);
    }
}

void ofeliaEditMesh3dColor_div(t_ofeliaEditMesh3dColor *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumColors())-1);
                ofFloatColor &color = t_ofeliaLoadMesh3d::meshes[pos]->getColors()[index];
                color /= f;
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dColor::objName);
    }
}

void ofeliaEditMesh3dColor_invert(t_ofeliaEditMesh3dColor *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumColors())-1);
                ofFloatColor &color = t_ofeliaLoadMesh3d::meshes[pos]->getColors()[index];
                color.invert();
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dColor::objName);
    }
}

void ofeliaEditMesh3dColor_lerp(t_ofeliaEditMesh3dColor *x, t_symbol *s, int argc, t_atom *argv)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumColors())-1);
                ofFloatColor &color = t_ofeliaLoadMesh3d::meshes[pos]->getColors()[index];
                
                if (argc == 5) {
                    
                    if (argv[0].a_type == A_FLOAT &&
                        argv[1].a_type == A_FLOAT &&
                        argv[2].a_type == A_FLOAT &&
                        argv[3].a_type == A_FLOAT &&
                        argv[4].a_type == A_FLOAT) {
                        
                        color.lerp(ofColor(static_cast<unsigned char>(argv[0].a_w.w_float),
                                           static_cast<unsigned char>(argv[1].a_w.w_float),
                                           static_cast<unsigned char>(argv[2].a_w.w_float),
                                           static_cast<unsigned char>(argv[3].a_w.w_float)),
                                   argv[4].a_w.w_float);
                    }
                    else {
                        
                        error("%s: wrong argument type", t_ofeliaEditMesh3dColor::objName);
                        return;
                    }
                }
                else {
                    
                    error("%s: invalid number of arguments", t_ofeliaEditMesh3dColor::objName);
                    return;
                }
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dColor::objName);
    }
}

void ofeliaEditMesh3dColor_normalize(t_ofeliaEditMesh3dColor *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumColors())-1);
                ofFloatColor &color = t_ofeliaLoadMesh3d::meshes[pos]->getColors()[index];
                color.normalize();
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dColor::objName);
    }
}

void ofeliaEditMesh3dColor_hue(t_ofeliaEditMesh3dColor *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumColors())-1);
                ofFloatColor &color = t_ofeliaLoadMesh3d::meshes[pos]->getColors()[index];
                color.setHue(f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dColor::objName);
    }
}

void ofeliaEditMesh3dColor_hueAngle(t_ofeliaEditMesh3dColor *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumColors())-1);
                ofFloatColor &color = t_ofeliaLoadMesh3d::meshes[pos]->getColors()[index];
                color.setHueAngle(f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dColor::objName);
    }
}

void ofeliaEditMesh3dColor_brightness(t_ofeliaEditMesh3dColor *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumColors())-1);
                ofFloatColor &color = t_ofeliaLoadMesh3d::meshes[pos]->getColors()[index];
                color.setBrightness(f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dColor::objName);
    }
}

void ofeliaEditMesh3dColor_saturation(t_ofeliaEditMesh3dColor *x, t_floatarg f)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumColors())-1);
                ofFloatColor &color = t_ofeliaLoadMesh3d::meshes[pos]->getColors()[index];
                color.setSaturation(f);
                outlet_bang(x->x_obj.ob_outlet);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaEditMesh3dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaEditMesh3dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaEditMesh3dColor::objName);
    }
}

void ofeliaEditMesh3dColor_set(t_ofeliaEditMesh3dColor *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaEditMesh3dColor_print(t_ofeliaEditMesh3dColor *x)
{
    post("\n[%s]", t_ofeliaEditMesh3dColor::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaEditMesh3dColor_setup()
{
    ofeliaEditMesh3dColor_class = class_new(gensym("ofEditMesh3dColor"),
                                            reinterpret_cast<t_newmethod>(ofeliaEditMesh3dColor_new),
                                            0, sizeof(t_ofeliaEditMesh3dColor),
                                            CLASS_DEFAULT, A_DEFSYM, 0);
    class_addfloat(ofeliaEditMesh3dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh3dColor_float));
    class_addmethod(ofeliaEditMesh3dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh3dColor_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditMesh3dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh3dColor_assign),
                    gensym("assign"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh3dColor_assignR),
                    gensym("assignR"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh3dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh3dColor_assignG),
                    gensym("assignG"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh3dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh3dColor_assignB),
                    gensym("assignB"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh3dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh3dColor_assignA),
                    gensym("assignA"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh3dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh3dColor_add),
                    gensym("add"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh3dColor_sub),
                    gensym("sub"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh3dColor_mult),
                    gensym("mult"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh3dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh3dColor_div),
                    gensym("div"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh3dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh3dColor_invert),
                    gensym("invert"), A_NULL, 0);
    class_addmethod(ofeliaEditMesh3dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh3dColor_lerp),
                    gensym("lerp"), A_GIMME, 0);
    class_addmethod(ofeliaEditMesh3dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh3dColor_normalize),
                    gensym("normalize"), A_NULL, 0);
    class_addmethod(ofeliaEditMesh3dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh3dColor_hue),
                    gensym("hue"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh3dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh3dColor_hueAngle),
                    gensym("hueAngle"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh3dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh3dColor_brightness),
                    gensym("brightness"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh3dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh3dColor_saturation),
                    gensym("saturation"), A_FLOAT, 0);
    class_addmethod(ofeliaEditMesh3dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh3dColor_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaEditMesh3dColor_class, reinterpret_cast<t_method>(ofeliaEditMesh3dColor_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetMesh2dVertex object methods
 */
void *ofeliaGetMesh2dVertex_new(t_symbol *s)
{
    t_ofeliaGetMesh2dVertex *x = reinterpret_cast<t_ofeliaGetMesh2dVertex*>(pd_new(ofeliaGetMesh2dVertex_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetMesh2dVertex_bang(t_ofeliaGetMesh2dVertex *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumVertices())-1);
                const ofVec2f &vertex = t_ofeliaLoadMesh2d::meshes[pos]->getVertex(index);
                t_atom av[2];
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = vertex.x;
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = vertex.y;
                outlet_list(x->x_obj.ob_outlet, &s_list, 2, av);
            }
            else {
                
                error("%s: '%s' has no vertices", t_ofeliaGetMesh2dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetMesh2dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetMesh2dVertex::objName);
    }
}

void ofeliaGetMesh2dVertex_float(t_ofeliaGetMesh2dVertex *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetMesh2dVertex_set(t_ofeliaGetMesh2dVertex *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetMesh2dVertex_print(t_ofeliaGetMesh2dVertex *x)
{
    post("\n[%s]", t_ofeliaGetMesh2dVertex::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetMesh2dVertex_setup()
{
    ofeliaGetMesh2dVertex_class = class_new(gensym("ofGetMesh2dVertex"),
                                            reinterpret_cast<t_newmethod>(ofeliaGetMesh2dVertex_new),
                                            0, sizeof(t_ofeliaGetMesh2dVertex),
                                            CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetMesh2dVertex_class, reinterpret_cast<t_method>(ofeliaGetMesh2dVertex_bang));
    class_addfloat(ofeliaGetMesh2dVertex_class, reinterpret_cast<t_method>(ofeliaGetMesh2dVertex_float));
    class_addmethod(ofeliaGetMesh2dVertex_class, reinterpret_cast<t_method>(ofeliaGetMesh2dVertex_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh2dVertex_class, reinterpret_cast<t_method>(ofeliaGetMesh2dVertex_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh2dVertex_class, reinterpret_cast<t_method>(ofeliaGetMesh2dVertex_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetMesh3dVertex object methods
 */
void *ofeliaGetMesh3dVertex_new(t_symbol *s)
{
    t_ofeliaGetMesh3dVertex *x = reinterpret_cast<t_ofeliaGetMesh3dVertex*>(pd_new(ofeliaGetMesh3dVertex_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetMesh3dVertex_bang(t_ofeliaGetMesh3dVertex *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasVertices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumVertices())-1);
                const ofVec3f &vertex = t_ofeliaLoadMesh3d::meshes[pos]->getVertex(index);
                t_atom av[3];
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = vertex.x;
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = vertex.y;
                av[2].a_type = A_FLOAT;
                av[2].a_w.w_float = vertex.z;
                outlet_list(x->x_obj.ob_outlet, &s_list, 3, av);
            }
            else {
                
                error("%s: '%s' has no vertices", t_ofeliaGetMesh3dVertex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetMesh3dVertex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetMesh3dVertex::objName);
    }
}

void ofeliaGetMesh3dVertex_float(t_ofeliaGetMesh3dVertex *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetMesh3dVertex_set(t_ofeliaGetMesh3dVertex *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetMesh3dVertex_print(t_ofeliaGetMesh3dVertex *x)
{
    post("\n[%s]", t_ofeliaGetMesh3dVertex::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetMesh3dVertex_setup()
{
    ofeliaGetMesh3dVertex_class = class_new(gensym("ofGetMesh3dVertex"),
                                            reinterpret_cast<t_newmethod>(ofeliaGetMesh3dVertex_new),
                                            0, sizeof(t_ofeliaGetMesh3dVertex),
                                            CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetMesh3dVertex_class, reinterpret_cast<t_method>(ofeliaGetMesh3dVertex_bang));
    class_addfloat(ofeliaGetMesh3dVertex_class, reinterpret_cast<t_method>(ofeliaGetMesh3dVertex_float));
    class_addmethod(ofeliaGetMesh3dVertex_class, reinterpret_cast<t_method>(ofeliaGetMesh3dVertex_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh3dVertex_class, reinterpret_cast<t_method>(ofeliaGetMesh3dVertex_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh3dVertex_class, reinterpret_cast<t_method>(ofeliaGetMesh3dVertex_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetMesh2dIndex object methods
 */
void *ofeliaGetMesh2dIndex_new(t_symbol *s)
{
    t_ofeliaGetMesh2dIndex *x = reinterpret_cast<t_ofeliaGetMesh2dIndex*>(pd_new(ofeliaGetMesh2dIndex_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetMesh2dIndex_bang(t_ofeliaGetMesh2dIndex *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasIndices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumIndices())-1);
                const ofIndexType idx = t_ofeliaLoadMesh2d::meshes[pos]->getIndex(index);
                outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(idx));
            }
            else {
                
                error("%s: '%s' has no indices", t_ofeliaGetMesh2dIndex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetMesh2dIndex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetMesh2dIndex::objName);
    }
}

void ofeliaGetMesh2dIndex_float(t_ofeliaGetMesh2dIndex *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetMesh2dIndex_set(t_ofeliaGetMesh2dIndex *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetMesh2dIndex_print(t_ofeliaGetMesh2dIndex *x)
{
    post("\n[%s]", t_ofeliaGetMesh2dIndex::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetMesh2dIndex_setup()
{
    ofeliaGetMesh2dIndex_class = class_new(gensym("ofGetMesh2dIndex"),
                                           reinterpret_cast<t_newmethod>(ofeliaGetMesh2dIndex_new),
                                           0, sizeof(t_ofeliaGetMesh2dIndex),
                                           CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetMesh2dIndex_class, reinterpret_cast<t_method>(ofeliaGetMesh2dIndex_bang));
    class_addfloat(ofeliaGetMesh2dIndex_class, reinterpret_cast<t_method>(ofeliaGetMesh2dIndex_float));
    class_addmethod(ofeliaGetMesh2dIndex_class, reinterpret_cast<t_method>(ofeliaGetMesh2dIndex_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh2dIndex_class, reinterpret_cast<t_method>(ofeliaGetMesh2dIndex_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh2dIndex_class, reinterpret_cast<t_method>(ofeliaGetMesh2dIndex_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetMesh3dIndex object methods
 */
void *ofeliaGetMesh3dIndex_new(t_symbol *s)
{
    t_ofeliaGetMesh3dIndex *x = reinterpret_cast<t_ofeliaGetMesh3dIndex*>(pd_new(ofeliaGetMesh3dIndex_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_float);
    return (x);
}

void ofeliaGetMesh3dIndex_bang(t_ofeliaGetMesh3dIndex *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasIndices()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumIndices())-1);
                const ofIndexType idx = t_ofeliaLoadMesh3d::meshes[pos]->getIndex(index);
                outlet_float(x->x_obj.ob_outlet, static_cast<t_float>(idx));
            }
            else {
                
                error("%s: '%s' has no indices", t_ofeliaGetMesh3dIndex::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetMesh3dIndex::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetMesh3dIndex::objName);
    }
}

void ofeliaGetMesh3dIndex_float(t_ofeliaGetMesh3dIndex *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetMesh3dIndex_set(t_ofeliaGetMesh3dIndex *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetMesh3dIndex_print(t_ofeliaGetMesh3dIndex *x)
{
    post("\n[%s]", t_ofeliaGetMesh3dIndex::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetMesh3dIndex_setup()
{
    ofeliaGetMesh3dIndex_class = class_new(gensym("ofGetMesh3dIndex"),
                                           reinterpret_cast<t_newmethod>(ofeliaGetMesh3dIndex_new),
                                           0, sizeof(t_ofeliaGetMesh3dIndex),
                                           CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetMesh3dIndex_class, reinterpret_cast<t_method>(ofeliaGetMesh3dIndex_bang));
    class_addfloat(ofeliaGetMesh3dIndex_class, reinterpret_cast<t_method>(ofeliaGetMesh3dIndex_float));
    class_addmethod(ofeliaGetMesh3dIndex_class, reinterpret_cast<t_method>(ofeliaGetMesh3dIndex_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh3dIndex_class, reinterpret_cast<t_method>(ofeliaGetMesh3dIndex_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh3dIndex_class, reinterpret_cast<t_method>(ofeliaGetMesh3dIndex_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetMesh2dNormal object methods
 */
void *ofeliaGetMesh2dNormal_new(t_symbol *s)
{
    t_ofeliaGetMesh2dNormal *x = reinterpret_cast<t_ofeliaGetMesh2dNormal*>(pd_new(ofeliaGetMesh2dNormal_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetMesh2dNormal_bang(t_ofeliaGetMesh2dNormal *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumNormals())-1);
                const ofVec2f &normal = t_ofeliaLoadMesh2d::meshes[pos]->getNormal(index);
                t_atom av[2];
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = normal.x;
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = normal.y;
                outlet_list(x->x_obj.ob_outlet, &s_list, 2, av);
            }
            else {
                
                error("%s: '%s' has no normals", t_ofeliaGetMesh2dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetMesh2dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetMesh2dNormal::objName);
    }
}

void ofeliaGetMesh2dNormal_float(t_ofeliaGetMesh2dNormal *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetMesh2dNormal_set(t_ofeliaGetMesh2dNormal *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetMesh2dNormal_print(t_ofeliaGetMesh2dNormal *x)
{
    post("\n[%s]", t_ofeliaGetMesh2dNormal::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetMesh2dNormal_setup()
{
    ofeliaGetMesh2dNormal_class = class_new(gensym("ofGetMesh2dNormal"),
                                            reinterpret_cast<t_newmethod>(ofeliaGetMesh2dNormal_new),
                                            0, sizeof(t_ofeliaGetMesh2dNormal),
                                            CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetMesh2dNormal_class, reinterpret_cast<t_method>(ofeliaGetMesh2dNormal_bang));
    class_addfloat(ofeliaGetMesh2dNormal_class, reinterpret_cast<t_method>(ofeliaGetMesh2dNormal_float));
    class_addmethod(ofeliaGetMesh2dNormal_class, reinterpret_cast<t_method>(ofeliaGetMesh2dNormal_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh2dNormal_class, reinterpret_cast<t_method>(ofeliaGetMesh2dNormal_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh2dNormal_class, reinterpret_cast<t_method>(ofeliaGetMesh2dNormal_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetMesh3dNormal object methods
 */
void *ofeliaGetMesh3dNormal_new(t_symbol *s)
{
    t_ofeliaGetMesh3dNormal *x = reinterpret_cast<t_ofeliaGetMesh3dNormal*>(pd_new(ofeliaGetMesh3dNormal_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetMesh3dNormal_bang(t_ofeliaGetMesh3dNormal *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasNormals()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumNormals())-1);
                const ofVec3f &normal = t_ofeliaLoadMesh3d::meshes[pos]->getNormal(index);
                t_atom av[3];
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = normal.x;
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = normal.y;
                av[2].a_type = A_FLOAT;
                av[2].a_w.w_float = normal.z;
                outlet_list(x->x_obj.ob_outlet, &s_list, 3, av);
            }
            else {
                
                error("%s: '%s' has no normals", t_ofeliaGetMesh3dNormal::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetMesh3dNormal::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetMesh3dNormal::objName);
    }
}

void ofeliaGetMesh3dNormal_float(t_ofeliaGetMesh3dNormal *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetMesh3dNormal_set(t_ofeliaGetMesh3dNormal *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetMesh3dNormal_print(t_ofeliaGetMesh3dNormal *x)
{
    post("\n[%s]", t_ofeliaGetMesh3dNormal::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetMesh3dNormal_setup()
{
    ofeliaGetMesh3dNormal_class = class_new(gensym("ofGetMesh3dNormal"),
                                            reinterpret_cast<t_newmethod>(ofeliaGetMesh3dNormal_new),
                                            0, sizeof(t_ofeliaGetMesh3dNormal),
                                            CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetMesh3dNormal_class, reinterpret_cast<t_method>(ofeliaGetMesh3dNormal_bang));
    class_addfloat(ofeliaGetMesh3dNormal_class, reinterpret_cast<t_method>(ofeliaGetMesh3dNormal_float));
    class_addmethod(ofeliaGetMesh3dNormal_class, reinterpret_cast<t_method>(ofeliaGetMesh3dNormal_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh3dNormal_class, reinterpret_cast<t_method>(ofeliaGetMesh3dNormal_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh3dNormal_class, reinterpret_cast<t_method>(ofeliaGetMesh3dNormal_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetMesh2dTexCoord object methods
 */
void *ofeliaGetMesh2dTexCoord_new(t_symbol *s)
{
    t_ofeliaGetMesh2dTexCoord *x = reinterpret_cast<t_ofeliaGetMesh2dTexCoord*>(pd_new(ofeliaGetMesh2dTexCoord_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetMesh2dTexCoord_bang(t_ofeliaGetMesh2dTexCoord *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumTexCoords())-1);
                const ofVec2f &texCoord = t_ofeliaLoadMesh2d::meshes[pos]->getTexCoord(index);
                t_atom av[2];
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = texCoord.x;
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = texCoord.y;
                outlet_list(x->x_obj.ob_outlet, &s_list, 2, av);
            }
            else {
                
                error("%s: '%s' has no texCoords", t_ofeliaGetMesh2dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetMesh2dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetMesh2dTexCoord::objName);
    }
}

void ofeliaGetMesh2dTexCoord_float(t_ofeliaGetMesh2dTexCoord *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetMesh2dTexCoord_set(t_ofeliaGetMesh2dTexCoord *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetMesh2dTexCoord_print(t_ofeliaGetMesh2dTexCoord *x)
{
    post("\n[%s]", t_ofeliaGetMesh2dTexCoord::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetMesh2dTexCoord_setup()
{
    ofeliaGetMesh2dTexCoord_class = class_new(gensym("ofGetMesh2dTexCoord"),
                                              reinterpret_cast<t_newmethod>(ofeliaGetMesh2dTexCoord_new),
                                              0, sizeof(t_ofeliaGetMesh2dTexCoord),
                                              CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetMesh2dTexCoord_class, reinterpret_cast<t_method>(ofeliaGetMesh2dTexCoord_bang));
    class_addfloat(ofeliaGetMesh2dTexCoord_class, reinterpret_cast<t_method>(ofeliaGetMesh2dTexCoord_float));
    class_addmethod(ofeliaGetMesh2dTexCoord_class, reinterpret_cast<t_method>(ofeliaGetMesh2dTexCoord_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh2dTexCoord_class, reinterpret_cast<t_method>(ofeliaGetMesh2dTexCoord_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh2dTexCoord_class, reinterpret_cast<t_method>(ofeliaGetMesh2dTexCoord_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetMesh3dTexCoord object methods
 */
void *ofeliaGetMesh3dTexCoord_new(t_symbol *s)
{
    t_ofeliaGetMesh3dTexCoord *x = reinterpret_cast<t_ofeliaGetMesh3dTexCoord*>(pd_new(ofeliaGetMesh3dTexCoord_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetMesh3dTexCoord_bang(t_ofeliaGetMesh3dTexCoord *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasTexCoords()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumTexCoords())-1);
                const ofVec2f &texCoord = t_ofeliaLoadMesh3d::meshes[pos]->getTexCoord(index);
                t_atom av[2];
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = texCoord.x;
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = texCoord.y;
                outlet_list(x->x_obj.ob_outlet, &s_list, 2, av);
            }
            else {
                
                error("%s: '%s' has no texCoords", t_ofeliaGetMesh3dTexCoord::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetMesh3dTexCoord::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetMesh3dTexCoord::objName);
    }
}

void ofeliaGetMesh3dTexCoord_float(t_ofeliaGetMesh3dTexCoord *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetMesh3dTexCoord_set(t_ofeliaGetMesh3dTexCoord *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetMesh3dTexCoord_print(t_ofeliaGetMesh3dTexCoord *x)
{
    post("\n[%s]", t_ofeliaGetMesh3dTexCoord::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetMesh3dTexCoord_setup()
{
    ofeliaGetMesh3dTexCoord_class = class_new(gensym("ofGetMesh3dTexCoord"),
                                              reinterpret_cast<t_newmethod>(ofeliaGetMesh3dTexCoord_new),
                                              0, sizeof(t_ofeliaGetMesh3dTexCoord),
                                              CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetMesh3dTexCoord_class, reinterpret_cast<t_method>(ofeliaGetMesh3dTexCoord_bang));
    class_addfloat(ofeliaGetMesh3dTexCoord_class, reinterpret_cast<t_method>(ofeliaGetMesh3dTexCoord_float));
    class_addmethod(ofeliaGetMesh3dTexCoord_class, reinterpret_cast<t_method>(ofeliaGetMesh3dTexCoord_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh3dTexCoord_class, reinterpret_cast<t_method>(ofeliaGetMesh3dTexCoord_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh3dTexCoord_class, reinterpret_cast<t_method>(ofeliaGetMesh3dTexCoord_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetMesh2dColor object methods
 */
void *ofeliaGetMesh2dColor_new(t_symbol *s)
{
    t_ofeliaGetMesh2dColor *x = reinterpret_cast<t_ofeliaGetMesh2dColor*>(pd_new(ofeliaGetMesh2dColor_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetMesh2dColor_bang(t_ofeliaGetMesh2dColor *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumColors())-1);
                const ofColor &color = t_ofeliaLoadMesh2d::meshes[pos]->getColor(index);
                t_atom av[4];
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = color.r;
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = color.g;
                av[2].a_type = A_FLOAT;
                av[2].a_w.w_float = color.b;
                av[3].a_type = A_FLOAT;
                av[3].a_w.w_float = color.a;
                outlet_list(x->x_obj.ob_outlet, &s_list, 4, av);
            }
            else {
                
                error("%s: '%s' has no colors", t_ofeliaGetMesh2dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetMesh2dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetMesh2dColor::objName);
    }
}

void ofeliaGetMesh2dColor_float(t_ofeliaGetMesh2dColor *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetMesh2dColor_set(t_ofeliaGetMesh2dColor *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetMesh2dColor_print(t_ofeliaGetMesh2dColor *x)
{
    post("\n[%s]", t_ofeliaGetMesh2dColor::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetMesh2dColor_setup()
{
    ofeliaGetMesh2dColor_class = class_new(gensym("ofGetMesh2dColor"),
                                           reinterpret_cast<t_newmethod>(ofeliaGetMesh2dColor_new),
                                           0, sizeof(t_ofeliaGetMesh2dColor),
                                           CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetMesh2dColor_class, reinterpret_cast<t_method>(ofeliaGetMesh2dColor_bang));
    class_addfloat(ofeliaGetMesh2dColor_class, reinterpret_cast<t_method>(ofeliaGetMesh2dColor_float));
    class_addmethod(ofeliaGetMesh2dColor_class, reinterpret_cast<t_method>(ofeliaGetMesh2dColor_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh2dColor_class, reinterpret_cast<t_method>(ofeliaGetMesh2dColor_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh2dColor_class, reinterpret_cast<t_method>(ofeliaGetMesh2dColor_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetMesh3dColor object methods
 */
void *ofeliaGetMesh3dColor_new(t_symbol *s)
{
    t_ofeliaGetMesh3dColor *x = reinterpret_cast<t_ofeliaGetMesh3dColor*>(pd_new(ofeliaGetMesh3dColor_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetMesh3dColor_bang(t_ofeliaGetMesh3dColor *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasColors()) {
                
                const int index = min(x->varName.index, static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumColors())-1);
                const ofColor &color = t_ofeliaLoadMesh3d::meshes[pos]->getColor(index);
                t_atom av[4];
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = color.r;
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = color.g;
                av[2].a_type = A_FLOAT;
                av[2].a_w.w_float = color.b;
                av[3].a_type = A_FLOAT;
                av[3].a_w.w_float = color.a;
                outlet_list(x->x_obj.ob_outlet, &s_list, 4, av);
            }
            else {
                
                error("%s: '%s' has no colors", t_ofeliaGetMesh3dColor::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetMesh3dColor::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetMesh3dColor::objName);
    }
}

void ofeliaGetMesh3dColor_float(t_ofeliaGetMesh3dColor *x, t_floatarg f)
{
    x->varName.index = max(0, static_cast<int>(f));
}

void ofeliaGetMesh3dColor_set(t_ofeliaGetMesh3dColor *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    getVarNameIndexed(x->varName);
}

void ofeliaGetMesh3dColor_print(t_ofeliaGetMesh3dColor *x)
{
    post("\n[%s]", t_ofeliaGetMesh3dColor::objName);
    post("name : %s[%d]", x->varName.name.c_str(), x->varName.index);
}

void ofeliaGetMesh3dColor_setup()
{
    ofeliaGetMesh3dColor_class = class_new(gensym("ofGetMesh3dColor"),
                                           reinterpret_cast<t_newmethod>(ofeliaGetMesh3dColor_new),
                                           0, sizeof(t_ofeliaGetMesh3dColor),
                                           CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetMesh3dColor_class, reinterpret_cast<t_method>(ofeliaGetMesh3dColor_bang));
    class_addfloat(ofeliaGetMesh3dColor_class, reinterpret_cast<t_method>(ofeliaGetMesh3dColor_float));
    class_addmethod(ofeliaGetMesh3dColor_class, reinterpret_cast<t_method>(ofeliaGetMesh3dColor_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh3dColor_class, reinterpret_cast<t_method>(ofeliaGetMesh3dColor_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh3dColor_class, reinterpret_cast<t_method>(ofeliaGetMesh3dColor_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetMesh2dVertices object methods
 */
void *ofeliaGetMesh2dVertices_new(t_symbol *s)
{
    t_ofeliaGetMesh2dVertices *x = reinterpret_cast<t_ofeliaGetMesh2dVertices*>(pd_new(ofeliaGetMesh2dVertices_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetMesh2dVertices_bang(t_ofeliaGetMesh2dVertices *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasVertices()) {
                
                const int ac = static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumVertices()*2);
                t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
                
                for (int i=0; i<ac; i+=2) {
                    
                    const ofVec2f &vertex = t_ofeliaLoadMesh2d::meshes[pos]->getVertex(i/2);
                    av[i].a_type = A_FLOAT;
                    av[i].a_w.w_float = vertex.x;
                    av[i+1].a_type = A_FLOAT;
                    av[i+1].a_w.w_float = vertex.y;
                }
                outlet_list(x->x_obj.ob_outlet, &s_list, ac, av);
                freebytes(av, ac * sizeof(t_atom));
            }
            else {
                
                error("%s: '%s' has no vertices", t_ofeliaGetMesh2dVertices::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetMesh2dVertices::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetMesh2dVertices::objName);
    }
}

void ofeliaGetMesh2dVertices_set(t_ofeliaGetMesh2dVertices *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetMesh2dVertices_print(t_ofeliaGetMesh2dVertices *x)
{
    post("\n[%s]", t_ofeliaGetMesh2dVertices::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetMesh2dVertices_setup()
{
    ofeliaGetMesh2dVertices_class = class_new(gensym("ofGetMesh2dVertices"),
                                              reinterpret_cast<t_newmethod>(ofeliaGetMesh2dVertices_new),
                                              0, sizeof(t_ofeliaGetMesh2dVertices),
                                              CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetMesh2dVertices_class, reinterpret_cast<t_method>(ofeliaGetMesh2dVertices_bang));
    class_addmethod(ofeliaGetMesh2dVertices_class, reinterpret_cast<t_method>(ofeliaGetMesh2dVertices_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh2dVertices_class, reinterpret_cast<t_method>(ofeliaGetMesh2dVertices_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh2dVertices_class, reinterpret_cast<t_method>(ofeliaGetMesh2dVertices_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetMesh3dVertices object methods
 */
void *ofeliaGetMesh3dVertices_new(t_symbol *s)
{
    t_ofeliaGetMesh3dVertices *x = reinterpret_cast<t_ofeliaGetMesh3dVertices*>(pd_new(ofeliaGetMesh3dVertices_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetMesh3dVertices_bang(t_ofeliaGetMesh3dVertices *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasVertices()) {
                
                const int ac = static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumVertices()*3);
                t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
                
                for (int i=0; i<ac; i+=3) {
                    
                    const ofVec3f &vertex = t_ofeliaLoadMesh3d::meshes[pos]->getVertex(i/3);
                    av[i].a_type = A_FLOAT;
                    av[i].a_w.w_float = vertex.x;
                    av[i+1].a_type = A_FLOAT;
                    av[i+1].a_w.w_float = vertex.y;
                    av[i+2].a_type = A_FLOAT;
                    av[i+2].a_w.w_float = vertex.z;
                }
                outlet_list(x->x_obj.ob_outlet, &s_list, ac, av);
                freebytes(av, ac * sizeof(t_atom));
            }
            else {
                
                error("%s: '%s' has no vertices", t_ofeliaGetMesh3dVertices::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetMesh3dVertices::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetMesh3dVertices::objName);
    }
}

void ofeliaGetMesh3dVertices_set(t_ofeliaGetMesh3dVertices *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetMesh3dVertices_print(t_ofeliaGetMesh3dVertices *x)
{
    post("\n[%s]", t_ofeliaGetMesh3dVertices::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetMesh3dVertices_setup()
{
    ofeliaGetMesh3dVertices_class = class_new(gensym("ofGetMesh3dVertices"),
                                              reinterpret_cast<t_newmethod>(ofeliaGetMesh3dVertices_new),
                                              0, sizeof(t_ofeliaGetMesh3dVertices),
                                              CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetMesh3dVertices_class, reinterpret_cast<t_method>(ofeliaGetMesh3dVertices_bang));
    class_addmethod(ofeliaGetMesh3dVertices_class, reinterpret_cast<t_method>(ofeliaGetMesh3dVertices_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh3dVertices_class, reinterpret_cast<t_method>(ofeliaGetMesh3dVertices_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh3dVertices_class, reinterpret_cast<t_method>(ofeliaGetMesh3dVertices_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetMesh2dIndices object methods
 */
void *ofeliaGetMesh2dIndices_new(t_symbol *s)
{
    t_ofeliaGetMesh2dIndices *x = reinterpret_cast<t_ofeliaGetMesh2dIndices*>(pd_new(ofeliaGetMesh2dIndices_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetMesh2dIndices_bang(t_ofeliaGetMesh2dIndices *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasIndices()) {
                
                const int ac = static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumIndices());
                t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
                
                for (int i=0; i<ac; ++i) {
                    
                    const ofIndexType &idx = t_ofeliaLoadMesh2d::meshes[pos]->getIndex(i);
                    av[i].a_type = A_FLOAT;
                    av[i].a_w.w_float = static_cast<t_float>(idx);
                }
                outlet_list(x->x_obj.ob_outlet, &s_list, ac, av);
                freebytes(av, ac * sizeof(t_atom));
            }
            else {
                
                error("%s: '%s' has no indices", t_ofeliaGetMesh2dIndices::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetMesh2dIndices::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetMesh2dIndices::objName);
    }
}

void ofeliaGetMesh2dIndices_set(t_ofeliaGetMesh2dIndices *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetMesh2dIndices_print(t_ofeliaGetMesh2dIndices *x)
{
    post("\n[%s]", t_ofeliaGetMesh2dIndices::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetMesh2dIndices_setup()
{
    ofeliaGetMesh2dIndices_class = class_new(gensym("ofGetMesh2dIndices"),
                                             reinterpret_cast<t_newmethod>(ofeliaGetMesh2dIndices_new),
                                             0, sizeof(t_ofeliaGetMesh2dIndices),
                                             CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetMesh2dIndices_class, reinterpret_cast<t_method>(ofeliaGetMesh2dIndices_bang));
    class_addmethod(ofeliaGetMesh2dIndices_class, reinterpret_cast<t_method>(ofeliaGetMesh2dIndices_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh2dIndices_class, reinterpret_cast<t_method>(ofeliaGetMesh2dIndices_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh2dIndices_class, reinterpret_cast<t_method>(ofeliaGetMesh2dIndices_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetMesh3dIndices object methods
 */
void *ofeliaGetMesh3dIndices_new(t_symbol *s)
{
    t_ofeliaGetMesh3dIndices *x = reinterpret_cast<t_ofeliaGetMesh3dIndices*>(pd_new(ofeliaGetMesh3dIndices_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetMesh3dIndices_bang(t_ofeliaGetMesh3dIndices *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasIndices()) {
                
                const int ac = static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumIndices());
                t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
                
                for (int i=0; i<ac; ++i) {
                    
                    const ofIndexType &idx = t_ofeliaLoadMesh3d::meshes[pos]->getIndex(i);
                    av[i].a_type = A_FLOAT;
                    av[i].a_w.w_float = static_cast<t_float>(idx);
                }
                outlet_list(x->x_obj.ob_outlet, &s_list, ac, av);
                freebytes(av, ac * sizeof(t_atom));
            }
            else {
                
                error("%s: '%s' has no indices", t_ofeliaGetMesh3dIndices::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetMesh3dIndices::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetMesh3dIndices::objName);
    }
}

void ofeliaGetMesh3dIndices_set(t_ofeliaGetMesh3dIndices *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetMesh3dIndices_print(t_ofeliaGetMesh3dIndices *x)
{
    post("\n[%s]", t_ofeliaGetMesh3dIndices::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetMesh3dIndices_setup()
{
    ofeliaGetMesh3dIndices_class = class_new(gensym("ofGetMesh3dIndices"),
                                             reinterpret_cast<t_newmethod>(ofeliaGetMesh3dIndices_new),
                                             0, sizeof(t_ofeliaGetMesh3dIndices),
                                             CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetMesh3dIndices_class, reinterpret_cast<t_method>(ofeliaGetMesh3dIndices_bang));
    class_addmethod(ofeliaGetMesh3dIndices_class, reinterpret_cast<t_method>(ofeliaGetMesh3dIndices_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh3dIndices_class, reinterpret_cast<t_method>(ofeliaGetMesh3dIndices_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh3dIndices_class, reinterpret_cast<t_method>(ofeliaGetMesh3dIndices_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetMesh2dNormals object methods
 */
void *ofeliaGetMesh2dNormals_new(t_symbol *s)
{
    t_ofeliaGetMesh2dNormals *x = reinterpret_cast<t_ofeliaGetMesh2dNormals*>(pd_new(ofeliaGetMesh2dNormals_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetMesh2dNormals_bang(t_ofeliaGetMesh2dNormals *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasNormals()) {
                
                const int ac = static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumNormals()*2);
                t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
                
                for (int i=0; i<ac; i+=2) {
                    
                    const ofVec2f &normal = t_ofeliaLoadMesh2d::meshes[pos]->getNormal(i/2);
                    av[i].a_type = A_FLOAT;
                    av[i].a_w.w_float = normal.x;
                    av[i+1].a_type = A_FLOAT;
                    av[i+1].a_w.w_float = normal.y;
                }
                outlet_list(x->x_obj.ob_outlet, &s_list, ac, av);
                freebytes(av, ac * sizeof(t_atom));
            }
            else {
                
                error("%s: '%s' has no normals", t_ofeliaGetMesh2dNormals::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetMesh2dNormals::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetMesh2dNormals::objName);
    }
}

void ofeliaGetMesh2dNormals_set(t_ofeliaGetMesh2dNormals *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetMesh2dNormals_print(t_ofeliaGetMesh2dNormals *x)
{
    post("\n[%s]", t_ofeliaGetMesh2dNormals::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetMesh2dNormals_setup()
{
    ofeliaGetMesh2dNormals_class = class_new(gensym("ofGetMesh2dNormals"),
                                             reinterpret_cast<t_newmethod>(ofeliaGetMesh2dNormals_new),
                                             0, sizeof(t_ofeliaGetMesh2dNormals),
                                             CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetMesh2dNormals_class, reinterpret_cast<t_method>(ofeliaGetMesh2dNormals_bang));
    class_addmethod(ofeliaGetMesh2dNormals_class, reinterpret_cast<t_method>(ofeliaGetMesh2dNormals_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh2dNormals_class, reinterpret_cast<t_method>(ofeliaGetMesh2dNormals_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh2dNormals_class, reinterpret_cast<t_method>(ofeliaGetMesh2dNormals_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetMesh3dNormals object methods
 */
void *ofeliaGetMesh3dNormals_new(t_symbol *s)
{
    t_ofeliaGetMesh3dNormals *x = reinterpret_cast<t_ofeliaGetMesh3dNormals*>(pd_new(ofeliaGetMesh3dNormals_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetMesh3dNormals_bang(t_ofeliaGetMesh3dNormals *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasNormals()) {
                
                const int ac = static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumNormals()*3);
                t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
                
                for (int i=0; i<ac; i+=3) {
                    
                    const ofVec3f &normal = t_ofeliaLoadMesh3d::meshes[pos]->getNormal(i/3);
                    av[i].a_type = A_FLOAT;
                    av[i].a_w.w_float = normal.x;
                    av[i+1].a_type = A_FLOAT;
                    av[i+1].a_w.w_float = normal.y;
                    av[i+2].a_type = A_FLOAT;
                    av[i+2].a_w.w_float = normal.z;
                }
                outlet_list(x->x_obj.ob_outlet, &s_list, ac, av);
                freebytes(av, ac * sizeof(t_atom));
            }
            else {
                
                error("%s: '%s' has no normals", t_ofeliaGetMesh3dNormals::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetMesh3dNormals::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetMesh3dNormals::objName);
    }
}

void ofeliaGetMesh3dNormals_set(t_ofeliaGetMesh3dNormals *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetMesh3dNormals_print(t_ofeliaGetMesh3dNormals *x)
{
    post("\n[%s]", t_ofeliaGetMesh3dNormals::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetMesh3dNormals_setup()
{
    ofeliaGetMesh3dNormals_class = class_new(gensym("ofGetMesh3dNormals"),
                                             reinterpret_cast<t_newmethod>(ofeliaGetMesh3dNormals_new),
                                             0, sizeof(t_ofeliaGetMesh3dNormals),
                                             CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetMesh3dNormals_class, reinterpret_cast<t_method>(ofeliaGetMesh3dNormals_bang));
    class_addmethod(ofeliaGetMesh3dNormals_class, reinterpret_cast<t_method>(ofeliaGetMesh3dNormals_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh3dNormals_class, reinterpret_cast<t_method>(ofeliaGetMesh3dNormals_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh3dNormals_class, reinterpret_cast<t_method>(ofeliaGetMesh3dNormals_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetMesh2dTexCoords object methods
 */
void *ofeliaGetMesh2dTexCoords_new(t_symbol *s)
{
    t_ofeliaGetMesh2dTexCoords *x = reinterpret_cast<t_ofeliaGetMesh2dTexCoords*>(pd_new(ofeliaGetMesh2dTexCoords_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetMesh2dTexCoords_bang(t_ofeliaGetMesh2dTexCoords *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasTexCoords()) {
                
                const int ac = static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumTexCoords()*2);
                t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
                
                for (int i=0; i<ac; i+=2) {
                    
                    const ofVec2f &texCoord = t_ofeliaLoadMesh2d::meshes[pos]->getTexCoord(i/2);
                    av[i].a_type = A_FLOAT;
                    av[i].a_w.w_float = texCoord.x;
                    av[i+1].a_type = A_FLOAT;
                    av[i+1].a_w.w_float = texCoord.y;
                }
                outlet_list(x->x_obj.ob_outlet, &s_list, ac, av);
                freebytes(av, ac * sizeof(t_atom));
            }
            else {
                
                error("%s: '%s' has no texCoords", t_ofeliaGetMesh2dTexCoords::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetMesh2dTexCoords::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetMesh2dTexCoords::objName);
    }
}

void ofeliaGetMesh2dTexCoords_set(t_ofeliaGetMesh2dTexCoords *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetMesh2dTexCoords_print(t_ofeliaGetMesh2dTexCoords *x)
{
    post("\n[%s]", t_ofeliaGetMesh2dTexCoords::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetMesh2dTexCoords_setup()
{
    ofeliaGetMesh2dTexCoords_class = class_new(gensym("ofGetMesh2dTexCoords"),
                                               reinterpret_cast<t_newmethod>(ofeliaGetMesh2dTexCoords_new),
                                               0, sizeof(t_ofeliaGetMesh2dTexCoords),
                                               CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetMesh2dTexCoords_class, reinterpret_cast<t_method>(ofeliaGetMesh2dTexCoords_bang));
    class_addmethod(ofeliaGetMesh2dTexCoords_class, reinterpret_cast<t_method>(ofeliaGetMesh2dTexCoords_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh2dTexCoords_class, reinterpret_cast<t_method>(ofeliaGetMesh2dTexCoords_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh2dTexCoords_class, reinterpret_cast<t_method>(ofeliaGetMesh2dTexCoords_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetMesh3dTexCoords object methods
 */
void *ofeliaGetMesh3dTexCoords_new(t_symbol *s)
{
    t_ofeliaGetMesh3dTexCoords *x = reinterpret_cast<t_ofeliaGetMesh3dTexCoords*>(pd_new(ofeliaGetMesh3dTexCoords_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetMesh3dTexCoords_bang(t_ofeliaGetMesh3dTexCoords *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasTexCoords()) {
                
                const int ac = static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumTexCoords()*2);
                t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
                
                for (int i=0; i<ac; i+=2) {
                    
                    const ofVec2f &texCoord = t_ofeliaLoadMesh3d::meshes[pos]->getTexCoord(i/2);
                    av[i].a_type = A_FLOAT;
                    av[i].a_w.w_float = texCoord.x;
                    av[i+1].a_type = A_FLOAT;
                    av[i+1].a_w.w_float = texCoord.y;
                }
                outlet_list(x->x_obj.ob_outlet, &s_list, ac, av);
                freebytes(av, ac * sizeof(t_atom));
            }
            else {
                
                error("%s: '%s' has no texCoords", t_ofeliaGetMesh3dTexCoords::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetMesh3dTexCoords::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetMesh3dTexCoords::objName);
    }
}

void ofeliaGetMesh3dTexCoords_set(t_ofeliaGetMesh3dTexCoords *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetMesh3dTexCoords_print(t_ofeliaGetMesh3dTexCoords *x)
{
    post("\n[%s]", t_ofeliaGetMesh3dTexCoords::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetMesh3dTexCoords_setup()
{
    ofeliaGetMesh3dTexCoords_class = class_new(gensym("ofGetMesh3dTexCoords"),
                                               reinterpret_cast<t_newmethod>(ofeliaGetMesh3dTexCoords_new),
                                               0, sizeof(t_ofeliaGetMesh3dTexCoords),
                                               CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetMesh3dTexCoords_class, reinterpret_cast<t_method>(ofeliaGetMesh3dTexCoords_bang));
    class_addmethod(ofeliaGetMesh3dTexCoords_class, reinterpret_cast<t_method>(ofeliaGetMesh3dTexCoords_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh3dTexCoords_class, reinterpret_cast<t_method>(ofeliaGetMesh3dTexCoords_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh3dTexCoords_class, reinterpret_cast<t_method>(ofeliaGetMesh3dTexCoords_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetMesh2dColors object methods
 */
void *ofeliaGetMesh2dColors_new(t_symbol *s)
{
    t_ofeliaGetMesh2dColors *x = reinterpret_cast<t_ofeliaGetMesh2dColors*>(pd_new(ofeliaGetMesh2dColors_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetMesh2dColors_bang(t_ofeliaGetMesh2dColors *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasColors()) {
                
                const int ac = static_cast<int>(t_ofeliaLoadMesh2d::meshes[pos]->getNumColors()*4);
                t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
                
                for (int i=0; i<ac; i+=4) {
                    
                    const ofColor &color = t_ofeliaLoadMesh2d::meshes[pos]->getColor(i/4);
                    av[i].a_type = A_FLOAT;
                    av[i].a_w.w_float = color.r;
                    av[i+1].a_type = A_FLOAT;
                    av[i+1].a_w.w_float = color.g;
                    av[i+2].a_type = A_FLOAT;
                    av[i+2].a_w.w_float = color.b;
                    av[i+3].a_type = A_FLOAT;
                    av[i+3].a_w.w_float = color.a;
                }
                outlet_list(x->x_obj.ob_outlet, &s_list, ac, av);
                freebytes(av, ac * sizeof(t_atom));
            }
            else {
                
                error("%s: '%s' has no colors", t_ofeliaGetMesh2dColors::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetMesh2dColors::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetMesh2dColors::objName);
    }
}

void ofeliaGetMesh2dColors_set(t_ofeliaGetMesh2dColors *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetMesh2dColors_print(t_ofeliaGetMesh2dColors *x)
{
    post("\n[%s]", t_ofeliaGetMesh2dColors::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetMesh2dColors_setup()
{
    ofeliaGetMesh2dColors_class = class_new(gensym("ofGetMesh2dColors"),
                                            reinterpret_cast<t_newmethod>(ofeliaGetMesh2dColors_new),
                                            0, sizeof(t_ofeliaGetMesh2dColors),
                                            CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetMesh2dColors_class, reinterpret_cast<t_method>(ofeliaGetMesh2dColors_bang));
    class_addmethod(ofeliaGetMesh2dColors_class, reinterpret_cast<t_method>(ofeliaGetMesh2dColors_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh2dColors_class, reinterpret_cast<t_method>(ofeliaGetMesh2dColors_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh2dColors_class, reinterpret_cast<t_method>(ofeliaGetMesh2dColors_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetMesh3dColors object methods
 */
void *ofeliaGetMesh3dColors_new(t_symbol *s)
{
    t_ofeliaGetMesh3dColors *x = reinterpret_cast<t_ofeliaGetMesh3dColors*>(pd_new(ofeliaGetMesh3dColors_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetMesh3dColors_bang(t_ofeliaGetMesh3dColors *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasColors()) {
                
                const int ac = static_cast<int>(t_ofeliaLoadMesh3d::meshes[pos]->getNumColors()*4);
                t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
                
                for (int i=0; i<ac; i+=4) {
                    
                    const ofColor &color = t_ofeliaLoadMesh3d::meshes[pos]->getColor(i/4);
                    av[i].a_type = A_FLOAT;
                    av[i].a_w.w_float = color.r;
                    av[i+1].a_type = A_FLOAT;
                    av[i+1].a_w.w_float = color.g;
                    av[i+2].a_type = A_FLOAT;
                    av[i+2].a_w.w_float = color.b;
                    av[i+3].a_type = A_FLOAT;
                    av[i+3].a_w.w_float = color.a;
                }
                outlet_list(x->x_obj.ob_outlet, &s_list, ac, av);
                freebytes(av, ac * sizeof(t_atom));
            }
            else {
                
                error("%s: '%s' has no colors", t_ofeliaGetMesh3dColors::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetMesh3dColors::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetMesh3dColors::objName);
    }
}

void ofeliaGetMesh3dColors_set(t_ofeliaGetMesh3dColors *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetMesh3dColors_print(t_ofeliaGetMesh3dColors *x)
{
    post("\n[%s]", t_ofeliaGetMesh3dColors::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetMesh3dColors_setup()
{
    ofeliaGetMesh3dColors_class = class_new(gensym("ofGetMesh3dColors"),
                                            reinterpret_cast<t_newmethod>(ofeliaGetMesh3dColors_new),
                                            0, sizeof(t_ofeliaGetMesh3dColors),
                                            CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetMesh3dColors_class, reinterpret_cast<t_method>(ofeliaGetMesh3dColors_bang));
    class_addmethod(ofeliaGetMesh3dColors_class, reinterpret_cast<t_method>(ofeliaGetMesh3dColors_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh3dColors_class, reinterpret_cast<t_method>(ofeliaGetMesh3dColors_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh3dColors_class, reinterpret_cast<t_method>(ofeliaGetMesh3dColors_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetMesh2dCommands object methods
 */
void *ofeliaGetMesh2dCommands_new(t_symbol *s)
{
    t_ofeliaGetMesh2dCommands *x = reinterpret_cast<t_ofeliaGetMesh2dCommands*>(pd_new(ofeliaGetMesh2dCommands_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_anything);
    return (x);
}

void ofeliaGetMesh2dCommands_bang(t_ofeliaGetMesh2dCommands *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            vector<t_ofeliaAtomElem> elems;
            const size_t numVertices = t_ofeliaLoadMesh2d::meshes[pos]->getNumVertices();
            const size_t numIndices = t_ofeliaLoadMesh2d::meshes[pos]->getNumIndices();
            const size_t numNormals = t_ofeliaLoadMesh2d::meshes[pos]->getNumNormals();
            const size_t numTexCoords = t_ofeliaLoadMesh2d::meshes[pos]->getNumTexCoords();
            const size_t numColors = t_ofeliaLoadMesh2d::meshes[pos]->getNumColors();
            
            if (numVertices) {
                
                elems.push_back({ATOM_ELEM_TYPE_SYMBOL, 0.0f, "vertex"});
                
                for (size_t i=0; i<numVertices; ++i) {
                    
                    const ofVec2f &vertex = t_ofeliaLoadMesh2d::meshes[pos]->getVertex(i);
                    elems.push_back({ATOM_ELEM_TYPE_FLOAT, vertex.x});
                    elems.push_back({ATOM_ELEM_TYPE_FLOAT, vertex.y});
                }
            }
            if (numIndices) {
                
                elems.push_back({ATOM_ELEM_TYPE_SYMBOL, 0.0f, "index"});
                
                for (size_t i=0; i<numIndices; ++i) {
                    
                    const ofIndexType idx = t_ofeliaLoadMesh2d::meshes[pos]->getIndex(i);
                    elems.push_back({ATOM_ELEM_TYPE_FLOAT, static_cast<float>(idx)});
                }
            }
            if (numNormals) {
                
                elems.push_back({ATOM_ELEM_TYPE_SYMBOL, 0.0f, "normal"});
                
                for (size_t i=0; i<numNormals; ++i) {
                    
                    const ofVec2f &normal = t_ofeliaLoadMesh2d::meshes[pos]->getNormal(i);
                    elems.push_back({ATOM_ELEM_TYPE_FLOAT, normal.x});
                    elems.push_back({ATOM_ELEM_TYPE_FLOAT, normal.y});
                }
            }
            if (numTexCoords) {
                
                elems.push_back({ATOM_ELEM_TYPE_SYMBOL, 0.0f, "texCoord"});
                
                for (size_t i=0; i<numTexCoords; ++i) {
                    
                    const ofVec2f &texCoord = t_ofeliaLoadMesh2d::meshes[pos]->getTexCoord(i);
                    elems.push_back({ATOM_ELEM_TYPE_FLOAT, texCoord.x});
                    elems.push_back({ATOM_ELEM_TYPE_FLOAT, texCoord.y});
                }
            }
            if (numColors) {
                
                elems.push_back({ATOM_ELEM_TYPE_SYMBOL, 0.0f, "color"});
                
                for (size_t i=0; i<numColors; ++i) {
                    
                    const ofColor &color = t_ofeliaLoadMesh2d::meshes[pos]->getColor(i);
                    elems.push_back({ATOM_ELEM_TYPE_FLOAT, static_cast<float>(color.r)});
                    elems.push_back({ATOM_ELEM_TYPE_FLOAT, static_cast<float>(color.g)});
                    elems.push_back({ATOM_ELEM_TYPE_FLOAT, static_cast<float>(color.b)});
                    elems.push_back({ATOM_ELEM_TYPE_FLOAT, static_cast<float>(color.a)});
                }
            }
            const int ac = static_cast<int>(elems.size());
            
            if (ac) {
                
                t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
                
                for (int i=0; i<ac; ++i) {
                    
                    if (elems[i].type == ATOM_ELEM_TYPE_FLOAT) {
                        
                        av[i].a_type = A_FLOAT;
                        av[i].a_w.w_float = elems[i].value;
                    }
                    else if (elems[i].type == ATOM_ELEM_TYPE_SYMBOL) {
                        
                        av[i].a_type = A_SYMBOL;
                        av[i].a_w.w_symbol = gensym(elems[i].name.c_str());
                    }
                }
                outlet_anything(x->x_obj.ob_outlet, av[0].a_w.w_symbol, ac-1, av+1);
                freebytes(av, ac * sizeof(t_atom));
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetMesh2dCommands::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetMesh2dCommands::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetMesh2dCommands::objName);
    }
}

void ofeliaGetMesh2dCommands_set(t_ofeliaGetMesh2dCommands *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetMesh2dCommands_print(t_ofeliaGetMesh2dCommands *x)
{
    post("\n[%s]", t_ofeliaGetMesh2dCommands::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetMesh2dCommands_setup()
{
    ofeliaGetMesh2dCommands_class = class_new(gensym("ofGetMesh2dCommands"),
                                              reinterpret_cast<t_newmethod>(ofeliaGetMesh2dCommands_new),
                                              0, sizeof(t_ofeliaGetMesh2dCommands),
                                              CLASS_DEFAULT, A_DEFSYM, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaGetMesh2dCommands_new),
                     gensym("ofGetMesh2dCmds"), A_DEFSYM, 0);
    class_addbang(ofeliaGetMesh2dCommands_class, reinterpret_cast<t_method>(ofeliaGetMesh2dCommands_bang));
    class_addmethod(ofeliaGetMesh2dCommands_class, reinterpret_cast<t_method>(ofeliaGetMesh2dCommands_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh2dCommands_class, reinterpret_cast<t_method>(ofeliaGetMesh2dCommands_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh2dCommands_class, reinterpret_cast<t_method>(ofeliaGetMesh2dCommands_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetMesh3dCommands object methods
 */
void *ofeliaGetMesh3dCommands_new(t_symbol *s)
{
    t_ofeliaGetMesh3dCommands *x = reinterpret_cast<t_ofeliaGetMesh3dCommands*>(pd_new(ofeliaGetMesh3dCommands_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_anything);
    return (x);
}

void ofeliaGetMesh3dCommands_bang(t_ofeliaGetMesh3dCommands *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            vector<t_ofeliaAtomElem> elems;
            const size_t numVertices = t_ofeliaLoadMesh3d::meshes[pos]->getNumVertices();
            const size_t numIndices = t_ofeliaLoadMesh3d::meshes[pos]->getNumIndices();
            const size_t numNormals = t_ofeliaLoadMesh3d::meshes[pos]->getNumNormals();
            const size_t numTexCoords = t_ofeliaLoadMesh3d::meshes[pos]->getNumTexCoords();
            const size_t numColors = t_ofeliaLoadMesh3d::meshes[pos]->getNumColors();
            
            if (numVertices) {
                
                elems.push_back({ATOM_ELEM_TYPE_SYMBOL, 0.0f, "vertex"});
                
                for (size_t i=0; i<numVertices; ++i) {
                    
                    const ofVec3f &vertex = t_ofeliaLoadMesh3d::meshes[pos]->getVertex(i);
                    elems.push_back({ATOM_ELEM_TYPE_FLOAT, vertex.x});
                    elems.push_back({ATOM_ELEM_TYPE_FLOAT, vertex.y});
                    elems.push_back({ATOM_ELEM_TYPE_FLOAT, vertex.z});
                }
            }
            if (numIndices) {
                
                elems.push_back({ATOM_ELEM_TYPE_SYMBOL, 0.0f, "index"});
                
                for (size_t i=0; i<numIndices; ++i) {
                    
                    const ofIndexType idx = t_ofeliaLoadMesh3d::meshes[pos]->getIndex(i);
                    elems.push_back({ATOM_ELEM_TYPE_FLOAT, static_cast<float>(idx)});
                }
            }
            if (numNormals) {
                
                elems.push_back({ATOM_ELEM_TYPE_SYMBOL, 0.0f, "normal"});
                
                for (size_t i=0; i<numNormals; ++i) {
                    
                    const ofVec3f &normal = t_ofeliaLoadMesh3d::meshes[pos]->getNormal(i);
                    elems.push_back({ATOM_ELEM_TYPE_FLOAT, normal.x});
                    elems.push_back({ATOM_ELEM_TYPE_FLOAT, normal.y});
                    elems.push_back({ATOM_ELEM_TYPE_FLOAT, normal.z});
                }
            }
            if (numTexCoords) {
                
                elems.push_back({ATOM_ELEM_TYPE_SYMBOL, 0.0f, "texCoord"});
                
                for (size_t i=0; i<numTexCoords; ++i) {
                    
                    const ofVec2f &texCoord = t_ofeliaLoadMesh3d::meshes[pos]->getTexCoord(i);
                    elems.push_back({ATOM_ELEM_TYPE_FLOAT, texCoord.x});
                    elems.push_back({ATOM_ELEM_TYPE_FLOAT, texCoord.y});
                }
            }
            if (numColors) {
                
                elems.push_back({ATOM_ELEM_TYPE_SYMBOL, 0.0f, "color"});
                
                for (size_t i=0; i<numColors; ++i) {
                    
                    const ofColor &color = t_ofeliaLoadMesh3d::meshes[pos]->getColor(i);
                    elems.push_back({ATOM_ELEM_TYPE_FLOAT, static_cast<float>(color.r)});
                    elems.push_back({ATOM_ELEM_TYPE_FLOAT, static_cast<float>(color.g)});
                    elems.push_back({ATOM_ELEM_TYPE_FLOAT, static_cast<float>(color.b)});
                    elems.push_back({ATOM_ELEM_TYPE_FLOAT, static_cast<float>(color.a)});
                }
            }
            const int ac = static_cast<int>(elems.size());
            
            if (ac) {
                
                t_atom *av = (t_atom *)getbytes(ac * sizeof(t_atom));
                
                for (int i=0; i<ac; ++i) {
                    
                    if (elems[i].type == ATOM_ELEM_TYPE_FLOAT) {
                        
                        av[i].a_type = A_FLOAT;
                        av[i].a_w.w_float = elems[i].value;
                    }
                    else if (elems[i].type == ATOM_ELEM_TYPE_SYMBOL) {
                        
                        av[i].a_type = A_SYMBOL;
                        av[i].a_w.w_symbol = gensym(elems[i].name.c_str());
                    }
                }
                outlet_anything(x->x_obj.ob_outlet, av[0].a_w.w_symbol, ac-1, av+1);
                freebytes(av, ac * sizeof(t_atom));
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetMesh3dCommands::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetMesh3dCommands::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetMesh3dCommands::objName);
    }
}

void ofeliaGetMesh3dCommands_set(t_ofeliaGetMesh3dCommands *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetMesh3dCommands_print(t_ofeliaGetMesh3dCommands *x)
{
    post("\n[%s]", t_ofeliaGetMesh3dCommands::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetMesh3dCommands_setup()
{
    ofeliaGetMesh3dCommands_class = class_new(gensym("ofGetMesh3dCommands"),
                                              reinterpret_cast<t_newmethod>(ofeliaGetMesh3dCommands_new),
                                              0, sizeof(t_ofeliaGetMesh3dCommands),
                                              CLASS_DEFAULT, A_DEFSYM, 0);
    class_addcreator(reinterpret_cast<t_newmethod>(ofeliaGetMesh3dCommands_new),
                     gensym("ofGetMesh3dCmds"), A_DEFSYM, 0);
    class_addbang(ofeliaGetMesh3dCommands_class, reinterpret_cast<t_method>(ofeliaGetMesh3dCommands_bang));
    class_addmethod(ofeliaGetMesh3dCommands_class, reinterpret_cast<t_method>(ofeliaGetMesh3dCommands_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh3dCommands_class, reinterpret_cast<t_method>(ofeliaGetMesh3dCommands_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh3dCommands_class, reinterpret_cast<t_method>(ofeliaGetMesh3dCommands_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetMesh2dBoundingBox object methods
 */
void *ofeliaGetMesh2dBoundingBox_new(t_symbol *s)
{
    t_ofeliaGetMesh2dBoundingBox *x = reinterpret_cast<t_ofeliaGetMesh2dBoundingBox*>(pd_new(ofeliaGetMesh2dBoundingBox_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetMesh2dBoundingBox_bang(t_ofeliaGetMesh2dBoundingBox *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasVertices()) {
                
                if (t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties)
                    updateMesh2dProperties(pos);
                t_atom av[2];
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = t_ofeliaLoadMesh2d::meshData[pos].boxDimen.x;
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = t_ofeliaLoadMesh2d::meshData[pos].boxDimen.y;
                outlet_list(x->x_obj.ob_outlet, &s_list, 2, av);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetMesh2dBoundingBox::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetMesh2dBoundingBox::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetMesh2dBoundingBox::objName);
    }
}

void ofeliaGetMesh2dBoundingBox_set(t_ofeliaGetMesh2dBoundingBox *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetMesh2dBoundingBox_print(t_ofeliaGetMesh2dBoundingBox *x)
{
    post("\n[%s]", t_ofeliaGetMesh2dBoundingBox::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetMesh2dBoundingBox_setup()
{
    ofeliaGetMesh2dBoundingBox_class = class_new(gensym("ofGetMesh2dBoundingBox"),
                                                 reinterpret_cast<t_newmethod>(ofeliaGetMesh2dBoundingBox_new),
                                                 0, sizeof(t_ofeliaGetMesh2dBoundingBox),
                                                 CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetMesh2dBoundingBox_class, reinterpret_cast<t_method>(ofeliaGetMesh2dBoundingBox_bang));
    class_addmethod(ofeliaGetMesh2dBoundingBox_class, reinterpret_cast<t_method>(ofeliaGetMesh2dBoundingBox_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh2dBoundingBox_class, reinterpret_cast<t_method>(ofeliaGetMesh2dBoundingBox_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh2dBoundingBox_class, reinterpret_cast<t_method>(ofeliaGetMesh2dBoundingBox_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetMesh3dBoundingBox object methods
 */
void *ofeliaGetMesh3dBoundingBox_new(t_symbol *s)
{
    t_ofeliaGetMesh3dBoundingBox *x = reinterpret_cast<t_ofeliaGetMesh3dBoundingBox*>(pd_new(ofeliaGetMesh3dBoundingBox_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetMesh3dBoundingBox_bang(t_ofeliaGetMesh3dBoundingBox *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasVertices()) {
                
                if (t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties)
                    updateMesh3dProperties(pos);
                t_atom av[3];
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = t_ofeliaLoadMesh3d::meshData[pos].boxDimen.x;
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = t_ofeliaLoadMesh3d::meshData[pos].boxDimen.y;
                av[2].a_type = A_FLOAT;
                av[2].a_w.w_float = t_ofeliaLoadMesh3d::meshData[pos].boxDimen.z;
                outlet_list(x->x_obj.ob_outlet, &s_list, 3, av);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetMesh3dBoundingBox::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetMesh3dBoundingBox::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetMesh3dBoundingBox::objName);
    }
}

void ofeliaGetMesh3dBoundingBox_set(t_ofeliaGetMesh3dBoundingBox *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetMesh3dBoundingBox_print(t_ofeliaGetMesh3dBoundingBox *x)
{
    post("\n[%s]", t_ofeliaGetMesh3dBoundingBox::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetMesh3dBoundingBox_setup()
{
    ofeliaGetMesh3dBoundingBox_class = class_new(gensym("ofGetMesh3dBoundingBox"),
                                                 reinterpret_cast<t_newmethod>(ofeliaGetMesh3dBoundingBox_new),
                                                 0, sizeof(t_ofeliaGetMesh3dBoundingBox),
                                                 CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetMesh3dBoundingBox_class, reinterpret_cast<t_method>(ofeliaGetMesh3dBoundingBox_bang));
    class_addmethod(ofeliaGetMesh3dBoundingBox_class, reinterpret_cast<t_method>(ofeliaGetMesh3dBoundingBox_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh3dBoundingBox_class, reinterpret_cast<t_method>(ofeliaGetMesh3dBoundingBox_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh3dBoundingBox_class, reinterpret_cast<t_method>(ofeliaGetMesh3dBoundingBox_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetMesh2dCentroid object methods
 */
void *ofeliaGetMesh2dCentroid_new(t_symbol *s)
{
    t_ofeliaGetMesh2dCentroid *x = reinterpret_cast<t_ofeliaGetMesh2dCentroid*>(pd_new(ofeliaGetMesh2dCentroid_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetMesh2dCentroid_bang(t_ofeliaGetMesh2dCentroid *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh2dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh2d::meshes[pos]->hasVertices()) {
                
                if (t_ofeliaLoadMesh2d::meshData[pos].shouldUpdateProperties)
                    updateMesh2dProperties(pos);
                t_atom av[2];
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = t_ofeliaLoadMesh2d::meshData[pos].centroid.x;
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = t_ofeliaLoadMesh2d::meshData[pos].centroid.y;
                outlet_list(x->x_obj.ob_outlet, &s_list, 2, av);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetMesh2dCentroid::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetMesh2dCentroid::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetMesh2dCentroid::objName);
    }
}

void ofeliaGetMesh2dCentroid_set(t_ofeliaGetMesh2dCentroid *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetMesh2dCentroid_print(t_ofeliaGetMesh2dCentroid *x)
{
    post("\n[%s]", t_ofeliaGetMesh2dCentroid::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetMesh2dCentroid_setup()
{
    ofeliaGetMesh2dCentroid_class = class_new(gensym("ofGetMesh2dCentroid"),
                                              reinterpret_cast<t_newmethod>(ofeliaGetMesh2dCentroid_new),
                                              0, sizeof(t_ofeliaGetMesh2dCentroid),
                                              CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetMesh2dCentroid_class, reinterpret_cast<t_method>(ofeliaGetMesh2dCentroid_bang));
    class_addmethod(ofeliaGetMesh2dCentroid_class, reinterpret_cast<t_method>(ofeliaGetMesh2dCentroid_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh2dCentroid_class, reinterpret_cast<t_method>(ofeliaGetMesh2dCentroid_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh2dCentroid_class, reinterpret_cast<t_method>(ofeliaGetMesh2dCentroid_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * ofGetMesh3dCentroid object methods
 */
void *ofeliaGetMesh3dCentroid_new(t_symbol *s)
{
    t_ofeliaGetMesh3dCentroid *x = reinterpret_cast<t_ofeliaGetMesh3dCentroid*>(pd_new(ofeliaGetMesh3dCentroid_class));
    getVarNameLocalPrefixes(x->varName);
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
    outlet_new(&x->x_obj, &s_list);
    return (x);
}

void ofeliaGetMesh3dCentroid_bang(t_ofeliaGetMesh3dCentroid *x)
{
    const t_string &name = x->varName.name;
    
    if (!name.empty()) {
        
        const int pos = getPositionByMesh3dName(name);
        
        if (pos != -1) {
            
            if (t_ofeliaLoadMesh3d::meshes[pos]->hasVertices()) {
                
                if (t_ofeliaLoadMesh3d::meshData[pos].shouldUpdateProperties)
                    updateMesh3dProperties(pos);
                t_atom av[3];
                av[0].a_type = A_FLOAT;
                av[0].a_w.w_float = t_ofeliaLoadMesh3d::meshData[pos].centroid.x;
                av[1].a_type = A_FLOAT;
                av[1].a_w.w_float = t_ofeliaLoadMesh3d::meshData[pos].centroid.y;
                av[2].a_type = A_FLOAT;
                av[2].a_w.w_float = t_ofeliaLoadMesh3d::meshData[pos].centroid.z;
                outlet_list(x->x_obj.ob_outlet, &s_list, 3, av);
            }
            else {
                
                error("%s: '%s' is empty", t_ofeliaGetMesh3dCentroid::objName, name.c_str());
            }
        }
        else {
            
            error("%s: failed to find '%s'", t_ofeliaGetMesh3dCentroid::objName, name.c_str());
        }
    }
    else {
        
        error("%s: name not assigned", t_ofeliaGetMesh3dCentroid::objName);
    }
}

void ofeliaGetMesh3dCentroid_set(t_ofeliaGetMesh3dCentroid *x, t_symbol *s)
{
    x->varName.name = s->s_name;
    getVarNameLocalized(x->varName);
}

void ofeliaGetMesh3dCentroid_print(t_ofeliaGetMesh3dCentroid *x)
{
    post("\n[%s]", t_ofeliaGetMesh3dCentroid::objName);
    post("name : %s", x->varName.name.c_str());
}

void ofeliaGetMesh3dCentroid_setup()
{
    ofeliaGetMesh3dCentroid_class = class_new(gensym("ofGetMesh3dCentroid"),
                                              reinterpret_cast<t_newmethod>(ofeliaGetMesh3dCentroid_new),
                                              0, sizeof(t_ofeliaGetMesh3dCentroid),
                                              CLASS_DEFAULT, A_DEFSYM, 0);
    class_addbang(ofeliaGetMesh3dCentroid_class, reinterpret_cast<t_method>(ofeliaGetMesh3dCentroid_bang));
    class_addmethod(ofeliaGetMesh3dCentroid_class, reinterpret_cast<t_method>(ofeliaGetMesh3dCentroid_set),
                    gensym("name"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh3dCentroid_class, reinterpret_cast<t_method>(ofeliaGetMesh3dCentroid_set),
                    gensym("set"), A_SYMBOL, 0);
    class_addmethod(ofeliaGetMesh3dCentroid_class, reinterpret_cast<t_method>(ofeliaGetMesh3dCentroid_print),
                    gensym("print"), A_NULL, 0);
}

/* ________________________________________________________________________________
 * setup methods
 */
void ofeliaMesh_setup()
{
    ofeliaLoadMesh2d_setup();
    ofeliaLoadMesh3d_setup();
    ofeliaDrawMesh2d_setup();
    ofeliaDrawMesh3d_setup();
    ofeliaDoesMesh2dNameExist_setup();
    ofeliaDoesMesh3dNameExist_setup();
    ofeliaEditMesh2dVertex_setup();
    ofeliaEditMesh3dVertex_setup();
    ofeliaEditMesh2dIndex_setup();
    ofeliaEditMesh3dIndex_setup();
    ofeliaEditMesh2dNormal_setup();
    ofeliaEditMesh3dNormal_setup();
    ofeliaEditMesh2dTexCoord_setup();
    ofeliaEditMesh3dTexCoord_setup();
    ofeliaEditMesh2dColor_setup();
    ofeliaEditMesh3dColor_setup();
    ofeliaGetMesh2dVertex_setup();
    ofeliaGetMesh3dVertex_setup();
    ofeliaGetMesh2dIndex_setup();
    ofeliaGetMesh3dIndex_setup();
    ofeliaGetMesh2dNormal_setup();
    ofeliaGetMesh3dNormal_setup();
    ofeliaGetMesh2dTexCoord_setup();
    ofeliaGetMesh3dTexCoord_setup();
    ofeliaGetMesh2dColor_setup();
    ofeliaGetMesh3dColor_setup();
    ofeliaGetMesh2dVertices_setup();
    ofeliaGetMesh3dVertices_setup();
    ofeliaGetMesh2dIndices_setup();
    ofeliaGetMesh3dIndices_setup();
    ofeliaGetMesh2dNormals_setup();
    ofeliaGetMesh3dNormals_setup();
    ofeliaGetMesh2dTexCoords_setup();
    ofeliaGetMesh3dTexCoords_setup();
    ofeliaGetMesh2dColors_setup();
    ofeliaGetMesh3dColors_setup();
    ofeliaGetMesh2dCommands_setup();
    ofeliaGetMesh3dCommands_setup();
    ofeliaGetMesh2dBoundingBox_setup();
    ofeliaGetMesh3dBoundingBox_setup();
    ofeliaGetMesh2dCentroid_setup();
    ofeliaGetMesh3dCentroid_setup();
}

