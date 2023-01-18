// 3d folder bindings
// 2017 Dan Wilcox <danomatika@gmail.com>

// ----- ofNode.h -----

// DIFF: ofNode.h: ignoring const & copy constructor in favor of && constructor
%ignore ofNode::ofNode(ofNode const &);

// process ofNode first since it's a base class
%include "3d/ofNode.h"

// ----- of3dUtils.h -----

%include "3d/of3dUtils.h"

// ----- ofCamera.h -----

%include "3d/ofCamera.h"

// ----- ofEasyCam.h -----

// DIFF: ofEasyCam.h: ignoring hasInteraction(int, int) in favor of
//                    hasInteraction(TransformType, int, int)
%ignore ofEasyCam::hasInteraction(int, int);

%include "3d/ofEasyCam.h"

// ----- ofMesh.h -----

// tell SWIG about template vectors
namespace std {
#ifdef OF_SWIG_RENAME
	%template(DefaultVertexTypeVector) std::vector<ofDefaultVertexType>;
	%template(DefaultNormalTypeVector) std::vector<ofDefaultNormalType>;
	%template(DefaultColorTypeVector) std::vector<ofDefaultColorType>;
	%template(DefaultTexCoordTypeVector) std::vector<ofDefaultTexCoordType>;
	%template(MeshFaceVector) std::vector<ofMeshFace_<ofDefaultVertexType,
		ofDefaultNormalType, ofDefaultColorType, ofDefaultTexCoordType>>;
#else
	%template(ofDefaultVertexTypeVector) std::vector<ofDefaultVertexType>;
	%template(ofDefaultNormalTypeVector) std::vector<ofDefaultNormalType>;
	%template(ofDefaultColorTypeVector) std::vector<ofDefaultColorType>;
	%template(ofDefaultTexCoordTypeVector) std::vector<ofDefaultTexCoordType>;
	%template(ofMeshFaceVector) std::vector<ofMeshFace_<ofDefaultVertexType,
		ofDefaultNormalType, ofDefaultColorType, ofDefaultTexCoordType>>;
#endif
}

// DIFF: ofMesh.h: ignoring pointer functions
%ignore addVertices(const V*, std::size_t);
%ignore getVerticesPointer();
%ignore addNormals(const N*, std::size_t);
%ignore getNormalsPointer();
%ignore addColors(const C*, std::size_t);
%ignore getColorsPointer();
%ignore addTexCoords(const T*, std::size_t);
%ignore getTexCoordsPointer();
%ignore addIndices(const ofIndexType*, std::size_t);
%ignore getIndexPointer();

%include "3d/ofMesh.h"

// tell SWIG about template classes
%template(Mesh) ofMesh_<ofDefaultVertexType, ofDefaultNormalType, ofDefaultColorType, ofDefaultTexCoordType>;
%template(MeshFace) ofMeshFace_<ofDefaultVertexType, ofDefaultNormalType, ofDefaultColorType, ofDefaultTexCoordType>;

// ----- of3dPrimitives.h -----

%include "3d/of3dPrimitives.h"
