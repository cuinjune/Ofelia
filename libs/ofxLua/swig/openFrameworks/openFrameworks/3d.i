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

%include "3d/ofMesh.h"

// tell SWIG about template classes
#ifdef OF_SWIG_RENAME
	%template(Mesh) ofMesh_<ofDefaultVertexType, ofDefaultNormalType, ofDefaultColorType, ofDefaultTexCoordType>;
	%template(MeshFace) ofMeshFace_<ofDefaultVertexType, ofDefaultNormalType, ofDefaultColorType, ofDefaultTexCoordType>;
#else
	%template(ofMesh) ofMesh_<ofDefaultVertexType, ofDefaultNormalType, ofDefaultColorType, ofDefaultTexCoordType>;
	%template(ofMeshFace) ofMeshFace_<ofDefaultVertexType, ofDefaultNormalType, ofDefaultColorType, ofDefaultTexCoordType>;
#endif

// ----- of3dPrimitives.h -----

%include "3d/of3dPrimitives.h"
