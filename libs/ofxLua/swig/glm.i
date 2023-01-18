// minimal SWIG (http://www.swig.org) interface wrapper for the glm library
// defines only the types used in openFrameworks: vec2, vec3, mat3, mat4, quat
// 2018 Dan Wilcox <danomatika@gmail.com>
// some parts adapted from https://github.com/IndiumGames/swig-wrapper-glm

// main MODULE
%module glm
%{
#include <stdexcept>
#include <string>
#include <sstream>

// init default values in constructors
#define GLM_FORCE_CTOR_INIT

// these included in math/ofVectorMath.h
// we declare some things manually, so some includes are commented out
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat3x3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/geometric.hpp"
#include "glm/common.hpp"
#include "glm/trigonometric.hpp"
#include "glm/exponential.hpp"
//#include "glm/vector_relational.hpp"
//#include "glm/ext.hpp"

//#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
//#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/epsilon.hpp"
#include "glm/gtx/norm.hpp"
#include "glm/gtx/perpendicular.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/gtx/spline.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/vector_angle.hpp"
//#include "glm/gtx/scalar_multiplication.hpp"
//#include <glm/gtc/type_ptr.hpp>

// extras included via glm/ext.h
#include <glm/gtc/matrix_access.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_projection.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/fast_square_root.hpp>
%}

// ----- C++ -----

%include <std_except.i>
%include <std_string.i>

// expanded primitives
%typedef unsigned int std::size_t;

// ----- Bindings------

namespace glm {

#ifdef SWIGLUA
%rename(add) operator+;
%rename(sub) operator-;
%rename(mul) operator*;
%rename(div) operator/;
%rename(eq) operator==;
#endif

%typedef int length_t;

%include "glm/vec2.i"
%include "glm/vec3.i"
%include "glm/vec4.i"
%include "glm/mat3.i"
%include "glm/mat4.i"
%include "glm/quat.i"
%include "glm/constants.i"
%include "glm/functions.i"

} // namespace
