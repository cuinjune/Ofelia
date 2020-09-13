// math folder bindings
// 2017 Dan Wilcox <danomatika@gmail.com>

// ----- ofMath.h -----

// ignore the template functions
%ignore ofInterpolateCosine;
%ignore ofInterpolateCubic;
%ignore ofInterpolateCatmullRom;
%ignore ofInterpolateHermite;

// declare float functions
ofInterpolateCosine(float y1, float y2, float pct);
ofInterpolateCubic(float y1, float y2, float pct);
ofInterpolateCatmullRom(float y1, float y2, float pct);
ofInterpolateHermite(float y1, float y2, float pct);

%include "math/ofMath.h"

// tell SWIG about template functions
#ifdef OF_SWIG_RENAME
	%template(interpolateCosine) ofInterpolateCosine<float>;
	%template(interpolateCubic) ofInterpolateCubic<float>;
	%template(interpolateCatmullRom) ofInterpolateCatmullRom<float>;
	%template(interpolateHermite) ofInterpolateHermite<float>;
#else
	%template(ofInterpolateCosine) ofInterpolateCosine<float>;
	%template(ofInterpolateCubic) ofInterpolateCubic<float>;
	%template(ofInterpolateCatmullRom) ofInterpolateCatmullRom<float>;
	%template(ofInterpolateHermite) ofInterpolateHermite<float>;
#endif

// ----- ofMathConstants.h -----

// handled in main.i

// ----- ofMatrix3x3.h -----

// DIFF: ofMatrix3x3.h: ignoring glm::mat3 operator
%ignore ofMatrix3x3::operator glm::mat3;

%include "math/ofMatrix3x3.h"

%extend ofMatrix3x3 {

	glm::mat3 mat3() {
		return (*$self);
	}

	const char* __str__() {
		ostringstream str;
		str << (*$self);
		return str.str().c_str();
	}

};

// ----- ofMatrix4x4.h -----

// DIFF: ofMatrix4x4.h: ignoring operator(size_t, size_t) const overload
%ignore ofMatrix4x4::operator()(std::size_t, std::size_t) const;

// DIFF: ofMatrix4x4.h: ignoring glm::mat4 operator
%ignore ofMatrix4x4::operator glm::mat4;

%include "math/ofMatrix4x4.h"

%extend ofMatrix4x4 {

	glm::mat4 mat4() {
		return (*$self);
	}

	const char* __str__() {
		ostringstream str;
		str << (*$self);
		return str.str().c_str();
	}

};

// ----- ofQuaternion.h -----

// DIFF: ofQuaternion.h: ignoring glm::quat operator
%ignore ofQuaternion::operator glm::quat;

// silence warning as SWIG ignores these anyway
// since it uses the non-const versions
%ignore ofQuaternion::x() const;
%ignore ofQuaternion::y() const;
%ignore ofQuaternion::z() const;
%ignore ofQuaternion::w() const;

%include "math/ofQuaternion.h"

%extend ofQuaternion {

	glm::quat quat() {
		return (*$self);
	}

	const char* __str__() {
		ostringstream str;
		str << (*$self);
		return str.str().c_str();
	}
};

// ----- ofVec2f.h -----

// DIFF: ofVec2f.h: ignoring glm::vec2 operator,
// DIFF::           use vec2() accessor instead
%ignore ofVec2f::operator glm::vec2;

%include "math/ofVec2f.h"

%extend ofVec2f {

	glm::vec2 vec2() {
		return (*$self);
	}

	const char* __str__() {
		ostringstream str;
		str << (*$self);
		return str.str().c_str();
	}
};

// ----- ofVec3f.h -----

// DIFF: ofVec3f.h: ignoring glm::vec3 operator,
// DIFF::           use vec3() accessor instead
%ignore ofVec3f::operator glm::vec3;

%include "math/ofVec3f.h"

%extend ofVec3f {

	glm::vec3 vec3() {
		return (*$self);
	}

	const char* __str__() {
		ostringstream str;
		str << (*$self);
		return str.str().c_str();
	}
};

// ----- ofVec4f.h -----

// DIFF: ofVec4f.h: ignoring glm::vec4 operator,
// DIFF::           use vec4() accessor instead
%ignore ofVec4f::operator glm::vec4;

%include "math/ofVec4f.h"

%extend ofVec4f {

	glm::vec4 vec4() {
		return (*$self);
	}

	const char* __str__() {
		ostringstream str;
		str << (*$self);
		return str.str().c_str();
	}
};

// ----- ofVectorMath.h-----

// not needed
