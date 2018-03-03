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

#ifndef ofeliaMesh_h
#define ofeliaMesh_h

#include "ofeliaBase.h"

extern "C" {
    
    /* enumerations */
    enum LoadMeshCmdType {
        
        MESH_LOAD_CMDTYPE_VERTEX,
        MESH_LOAD_CMDTYPE_INDEX,
        MESH_LOAD_CMDTYPE_NORMAL,
        MESH_LOAD_CMDTYPE_TEXCOORD,
        MESH_LOAD_CMDTYPE_COLOR,
        MESH_LOAD_CMDTYPE_CIRCLE,
        MESH_LOAD_CMDTYPE_ELLIPSE,
        MESH_LOAD_CMDTYPE_SECTOR,
        MESH_LOAD_CMDTYPE_EQTRIANGLE,
        MESH_LOAD_CMDTYPE_ISOTRIANGLE,
        MESH_LOAD_CMDTYPE_SQUARE,
        MESH_LOAD_CMDTYPE_RECTANGLE,
        MESH_LOAD_CMDTYPE_RECTROUNDED,
        MESH_LOAD_CMDTYPE_RECTROUNDED4,
        MESH_LOAD_CMDTYPE_CROSS,
        MESH_LOAD_CMDTYPE_HEART,
        MESH_LOAD_CMDTYPE_MOON,
        MESH_LOAD_CMDTYPE_REGPOLYGON,
        MESH_LOAD_CMDTYPE_STAR,
        MESH_LOAD_CMDTYPE_BOX,
        MESH_LOAD_CMDTYPE_CONE,
        MESH_LOAD_CMDTYPE_CYLINDER,
        MESH_LOAD_CMDTYPE_ICOSPHERE,
        MESH_LOAD_CMDTYPE_PLANE,
        MESH_LOAD_CMDTYPE_SPHERE
    };
    enum LoadMeshCmdState {
        
        MESH_LOAD_CMD_INSERT,
        MESH_LOAD_CMD_ERASE,
        MESH_LOAD_CMD_FILL
    };
    
    /* ofLoadMesh2d object class */
    static t_class *ofeliaLoadMesh2d_class;
    
    typedef struct _ofeliaLoadMesh2dElem {
        
        LoadMeshCmdType type;
        int numArgs;
        float args[7];
        
    } t_ofeliaLoadMesh2dElem;
    
    typedef struct _ofeliaLoadMesh2dData {
        
        unsigned int objID;
        t_ofeliaVarName varName;
        ofMesh mesh;
        ofVec2f boxDimen, centroid;
        bool shouldUpdateProperties;
        
    } t_ofeliaLoadMesh2dData;
    
    typedef struct _ofeliaLoadMesh2dCmdData {
        
        LoadMeshCmdState state;
        t_ofeliaLoadMesh2dElem elem;
        t_float fromIndex, toIndex;
        
    } t_ofeliaLoadMesh2dCmdData;
    
    typedef struct _ofeliaLoadMesh2d
    {
        t_object x_obj;
        static const char *objName;
        static unsigned int counter;
        static vector<t_ofeliaLoadMesh2dData> meshData;
        static vector<unique_ptr<ofVboMesh>> meshes;
        vector<t_ofeliaLoadMesh2dCmdData> cmdVec;
        ofPrimitiveMode primMode;
        int numVertices;
        int numIndices;
        int numNormals;
        int numTexCoords;
        int numColors;
        static bool bInited;
        bool bInitGate;
        unsigned int objID;
        bool shouldOutlet;
        t_outlet *numVertices_out;
        t_outlet *numIndices_out;
        t_outlet *numNormals_out;
        t_outlet *numTexCoords_out;
        t_outlet *numColors_out;
        
    } t_ofeliaLoadMesh2d;
    
    /* ofLoadMesh3d object class */
    static t_class *ofeliaLoadMesh3d_class;
    
    typedef struct _ofeliaLoadMesh3dElem {
        
        LoadMeshCmdType type;
        int numArgs;
        float args[7];
        
    } t_ofeliaLoadMesh3dElem;
    
    typedef struct _ofeliaLoadMesh3dData {
        
        unsigned int objID;
        t_ofeliaVarName varName;
        ofMesh mesh;
        ofVec3f boxDimen, centroid;
        bool shouldUpdateProperties;
        
    } t_ofeliaLoadMesh3dData;
    
    typedef struct _ofeliaLoadMesh3dCmdData {
        
        LoadMeshCmdState state;
        t_ofeliaLoadMesh3dElem elem;
        t_float fromIndex, toIndex;
        
    } t_ofeliaLoadMesh3dCmdData;
    
    typedef struct _ofeliaLoadMesh3d
    {
        t_object x_obj;
        static const char *objName;
        static unsigned int counter;
        static vector<t_ofeliaLoadMesh3dData> meshData;
        static vector<unique_ptr<ofVboMesh>> meshes;
        vector<t_ofeliaLoadMesh3dCmdData> cmdVec;
        ofPrimitiveMode primMode;
        int numVertices;
        int numIndices;
        int numNormals;
        int numTexCoords;
        int numColors;
        static bool bInited;
        bool bInitGate;
        unsigned int objID;
        bool shouldOutlet;
        t_outlet *numVertices_out;
        t_outlet *numIndices_out;
        t_outlet *numNormals_out;
        t_outlet *numTexCoords_out;
        t_outlet *numColors_out;
        
    } t_ofeliaLoadMesh3d;
    
    /* ofDrawMesh2d object class */
    static t_class *ofeliaDrawMesh2d_class;
    
    typedef struct _ofeliaDrawMesh2d
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaNameDimen2dElem elem;
        
    } t_ofeliaDrawMesh2d;
    
    /* ofDrawMesh3d object class */
    static t_class *ofeliaDrawMesh3d_class;
    
    typedef struct _ofeliaDrawMesh3d
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaNameDimen3dElem elem;
        
    } t_ofeliaDrawMesh3d;
    
    /* ofDoesMesh2dNameExist object class */
    static t_class *ofeliaDoesMesh2dNameExist_class;
    
    typedef struct _ofeliaDoesMesh2dNameExist
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaDoesMesh2dNameExist;
    
    /* ofDoesMesh3dNameExist object class */
    static t_class *ofeliaDoesMesh3dNameExist_class;
    
    typedef struct _ofeliaDoesMesh3dNameExist
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaDoesMesh3dNameExist;
    
    /* ofEditMesh2dVertex object class */
    static t_class *ofeliaEditMesh2dVertex_class;
    
    typedef struct _ofeliaEditMesh2dVertex
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaEditMesh2dVertex;
    
    /* ofEditMesh3dVertex object class */
    static t_class *ofeliaEditMesh3dVertex_class;
    
    typedef struct _ofeliaEditMesh3dVertex
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaEditMesh3dVertex;
    
    /* ofEditMesh2dIndex object class */
    static t_class *ofeliaEditMesh2dIndex_class;
    
    typedef struct _ofeliaEditMesh2dIndex
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaEditMesh2dIndex;
    
    /* ofEditMesh3dIndex object class */
    static t_class *ofeliaEditMesh3dIndex_class;
    
    typedef struct _ofeliaEditMesh3dIndex
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaEditMesh3dIndex;
    
    /* ofEditMesh2dNormal object class */
    static t_class *ofeliaEditMesh2dNormal_class;
    
    typedef struct _ofeliaEditMesh2dNormal
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaEditMesh2dNormal;
    
    /* ofEditMesh3dNormal object class */
    static t_class *ofeliaEditMesh3dNormal_class;
    
    typedef struct _ofeliaEditMesh3dNormal
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaEditMesh3dNormal;
    
    /* ofEditMesh2dTexCoord object class */
    static t_class *ofeliaEditMesh2dTexCoord_class;
    
    typedef struct _ofeliaEditMesh2dTexCoord
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaEditMesh2dTexCoord;
    
    /* ofEditMesh3dTexCoord object class */
    static t_class *ofeliaEditMesh3dTexCoord_class;
    
    typedef struct _ofeliaEditMesh3dTexCoord
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaEditMesh3dTexCoord;
    
    /* ofEditMesh2dColor object class */
    static t_class *ofeliaEditMesh2dColor_class;
    
    typedef struct _ofeliaEditMesh2dColor
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaEditMesh2dColor;
    
    /* ofEditMesh3dColor object class */
    static t_class *ofeliaEditMesh3dColor_class;
    
    typedef struct _ofeliaEditMesh3dColor
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaEditMesh3dColor;
    
    /* ofGetMesh2dVertex object class */
    static t_class *ofeliaGetMesh2dVertex_class;
    
    typedef struct _ofeliaGetMesh2dVertex
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetMesh2dVertex;
    
    /* ofGetMesh3dVertex object class */
    static t_class *ofeliaGetMesh3dVertex_class;
    
    typedef struct _ofeliaGetMesh3dVertex
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetMesh3dVertex;
    
    /* ofGetMesh2dIndex object class */
    static t_class *ofeliaGetMesh2dIndex_class;
    
    typedef struct _ofeliaGetMesh2dIndex
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetMesh2dIndex;
    
    /* ofGetMesh3dIndex object class */
    static t_class *ofeliaGetMesh3dIndex_class;
    
    typedef struct _ofeliaGetMesh3dIndex
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetMesh3dIndex;
    
    /* ofGetMesh2dNormal object class */
    static t_class *ofeliaGetMesh2dNormal_class;
    
    typedef struct _ofeliaGetMesh2dNormal
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetMesh2dNormal;
    
    /* ofGetMesh3dNormal object class */
    static t_class *ofeliaGetMesh3dNormal_class;
    
    typedef struct _ofeliaGetMesh3dNormal
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetMesh3dNormal;
    
    /* ofGetMesh2dTexCoord object class */
    static t_class *ofeliaGetMesh2dTexCoord_class;
    
    typedef struct _ofeliaGetMesh2dTexCoord
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetMesh2dTexCoord;
    
    /* ofGetMesh3dTexCoord object class */
    static t_class *ofeliaGetMesh3dTexCoord_class;
    
    typedef struct _ofeliaGetMesh3dTexCoord
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetMesh3dTexCoord;
    
    /* ofGetMesh2dColor object class */
    static t_class *ofeliaGetMesh2dColor_class;
    
    typedef struct _ofeliaGetMesh2dColor
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetMesh2dColor;
    
    /* ofGetMesh3dColor object class */
    static t_class *ofeliaGetMesh3dColor_class;
    
    typedef struct _ofeliaGetMesh3dColor
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetMesh3dColor;
    
    /* ofGetMesh2dVertices object class */
    static t_class *ofeliaGetMesh2dVertices_class;
    
    typedef struct _ofeliaGetMesh2dVertices
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetMesh2dVertices;
    
    /* ofGetMesh3dVertices object class */
    static t_class *ofeliaGetMesh3dVertices_class;
    
    typedef struct _ofeliaGetMesh3dVertices
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetMesh3dVertices;
    
    /* ofGetMesh2dIndices object class */
    static t_class *ofeliaGetMesh2dIndices_class;
    
    typedef struct _ofeliaGetMesh2dIndices
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetMesh2dIndices;
    
    /* ofGetMesh3dIndices object class */
    static t_class *ofeliaGetMesh3dIndices_class;
    
    typedef struct _ofeliaGetMesh3dIndices
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetMesh3dIndices;
    
    /* ofGetMesh2dNormals object class */
    static t_class *ofeliaGetMesh2dNormals_class;
    
    typedef struct _ofeliaGetMesh2dNormals
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetMesh2dNormals;
    
    /* ofGetMesh3dNormals object class */
    static t_class *ofeliaGetMesh3dNormals_class;
    
    typedef struct _ofeliaGetMesh3dNormals
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetMesh3dNormals;
    
    /* ofGetMesh2dTexCoords object class */
    static t_class *ofeliaGetMesh2dTexCoords_class;
    
    typedef struct _ofeliaGetMesh2dTexCoords
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetMesh2dTexCoords;
    
    /* ofGetMesh3dTexCoords object class */
    static t_class *ofeliaGetMesh3dTexCoords_class;
    
    typedef struct _ofeliaGetMesh3dTexCoords
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetMesh3dTexCoords;
    
    /* ofGetMesh2dColors object class */
    static t_class *ofeliaGetMesh2dColors_class;
    
    typedef struct _ofeliaGetMesh2dColors
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetMesh2dColors;
    
    /* ofGetMesh3dColors object class */
    static t_class *ofeliaGetMesh3dColors_class;
    
    typedef struct _ofeliaGetMesh3dColors
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetMesh3dColors;
    
    /* ofGetMesh2dCommands object class */
    static t_class *ofeliaGetMesh2dCommands_class;
    
    typedef struct _ofeliaGetMesh2dCommands
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetMesh2dCommands;
    
    /* ofGetMesh3dCommands object class */
    static t_class *ofeliaGetMesh3dCommands_class;
    
    typedef struct _ofeliaGetMesh3dCommands
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetMesh3dCommands;
    
    /* ofGetMesh2dBoundingBox object class */
    static t_class *ofeliaGetMesh2dBoundingBox_class;
    
    typedef struct _ofeliaGetMesh2dBoundingBox
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetMesh2dBoundingBox;
    
    /* ofGetMesh3dBoundingBox object class */
    static t_class *ofeliaGetMesh3dBoundingBox_class;
    
    typedef struct _ofeliaGetMesh3dBoundingBox
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetMesh3dBoundingBox;
    
    /* ofGetMesh2dCentroid object class */
    static t_class *ofeliaGetMesh2dCentroid_class;
    
    typedef struct _ofeliaGetMesh2dCentroid
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetMesh2dCentroid;
    
    /* ofGetMesh3dCentroid object class */
    static t_class *ofeliaGetMesh3dCentroid_class;
    
    typedef struct _ofeliaGetMesh3dCentroid
    {
        t_object x_obj;
        static const char *objName;
        t_ofeliaVarName varName;
        
    } t_ofeliaGetMesh3dCentroid;
    
    /* setup method */
    void ofeliaMesh_setup();
}

#endif /* ofeliaMesh_h */
