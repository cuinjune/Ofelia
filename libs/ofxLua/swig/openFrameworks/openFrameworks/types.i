// types folder bindings
// 2017 Dan Wilcox <danomatika@gmail.com>

// ----- ofParameter.h -----

// not needed

// ----- ofParameterGroup.h -----

// not needed

// ----- ofBaseTypes.h -----

// handled in main.i

// ----- ofColor.h -----

// DIFF: ofColor.h:
// TODO:   find a way to release static named ofColor instances

// ignore SWIG Warning 312 from nested union that provides r, g, b, & a access
// ignore SWIG Warning 320 from extern template classes
#pragma SWIG nowarn=312,320

%include "types/ofColor.h"

// reenable
#pragma SWIG nowarn=

// DIFF:   added ofColor_ pixel channel getters getR(), getG(), getB(), getA()
// DIFF:   added ofColor_ pixel channel setters setR(), setG(), setB(), setA()
// DIFF:   added target language tostring wrapper for ofColor_::operator <<
%extend ofColor_ {

	// pixel channel getters
	PixelType getR() {return $self->r;}
	PixelType getG() {return $self->g;}
	PixelType getB() {return $self->b;}
	PixelType getA() {return $self->a;}

	// pixel channel setters
	void setR(PixelType r) {$self->r = r;}
	void setG(PixelType g) {$self->g = g;}
	void setB(PixelType b) {$self->b = b;}
	void setA(PixelType a) {$self->a = a;}

	const char* __str__() {
		stringstream str;
		str << (*$self);
		return str.str().c_str();
	}
};

%attributeVar(ofColor_<unsigned char>, unsigned char, r, r, r);
%attributeVar(ofColor_<unsigned char>, unsigned char, g, g, g);
%attributeVar(ofColor_<unsigned char>, unsigned char, b, b, b);
%attributeVar(ofColor_<unsigned char>, unsigned char, a, a, a);

%attributeVar(ofColor_<float>, float, r, r, r);
%attributeVar(ofColor_<float>, float, g, g, g);
%attributeVar(ofColor_<float>, float, b, b, b);
%attributeVar(ofColor_<float>, float, a, a, a);

%attributeVar(ofColor_<unsigned short>, unsigned short, r, r, r);
%attributeVar(ofColor_<unsigned short>, unsigned short, g, g, g);
%attributeVar(ofColor_<unsigned short>, unsigned short, b, b, b);
%attributeVar(ofColor_<unsigned short>, unsigned short, a, a, a);

// tell SWIG about template classes
#ifdef OF_SWIG_RENAME
	%template(Color) ofColor_<unsigned char>;
	%template(FloatColor) ofColor_<float>;
	%template(ShortColor) ofColor_<unsigned short>;
#else
	%template(ofColor) ofColor_<unsigned char>;
	%template(ofFloatColor) ofColor_<float>;
	%template(ofShortColor) ofColor_<unsigned short>;
#endif

// ----- ofPoint.h -----

// NOTE: ofPoint is just a typedef which swig cannot wrap, so the types need to
// be handled in the scripting language, see the Lua, Python, etc code in lang.i
%include "types/ofPoint.h"

// ----- ofRectangle.h -----

// DIFF: ofRectangle.h:
// DIFF:   renamed functions due to ambiguous overloading:
// DIFF:   scaleToScaleMode() & scaleToAspectRatio()
%rename(scaleToScaleMode) ofRectangle::scaleTo(ofRectangle const &, ofScaleMode);
%rename(scaleToAspectRatio) ofRectangle::scaleTo(ofRectangle const &, ofAspectRatioMode);

// TODO:   find a way to release returned ofRectangle instances

// ignore SWIG Warning 302 due to manual override of x & y attributes
%warnfilter(302) ofRectangle::x;
%warnfilter(302) ofRectangle::y;

%extend ofRectangle {

	// override these since they are float references in the orig file and we
	// want to access them as floats
	float x;
	float y;

	const char* __str__() {
		stringstream str;
		str << (*$self);
		return str.str().c_str();
	}
};

%include "types/ofRectangle.h"

// SWIG converts the x & y float& types into pointers,
// so specify x & y as attributes using the get & set functions
%attribute(ofRectangle, float, x, getX, setX);
%attribute(ofRectangle, float, y, getY, setY);

// ----- ofTypes.h -----

// DIFF: ofTypes.h:
// DIFF:   ignoring video format and video device classes
%ignore ofVideoFormat;
%ignore ofVideoDevice;

// DIFF:   mutex, scoped lock, & ptr are probably too low level
%ignore ofMutex;
%ignore ofScopedLock;
%ignore ofPtr;

%include "types/ofTypes.h"
