// forward declarations and headers which need to be handled first
// 2017 Dan Wilcox <danomatika@gmail.com>

// ignore everything in the private namespace
%ignore of::priv;

// TODO: wrap nested structs and classes?
//%feature ("flatnested");

// TODO: make sure returned class instances are freed by using %newobject

// tell SWIG about template vectors,
// needed for functions and return types
namespace std {
	%template(IntVector) std::vector<int>;
	%template(FloatVector) std::vector<float>;
	%template(StringVector) std::vector<std::string>;
	%template(UCharVector) std::vector<unsigned char>;
#ifdef OF_SWIG_RENAME
	%template(TextureVector) std::vector<ofTexture>;
	%template(VideoDeviceVector) std::vector<ofVideoDevice>;
#else
	%template(ofTextureVector) std::vector<ofTexture>;
	%template(ofVideoDeviceVector) std::vector<ofVideoDevice>;
#endif
};

// SWIG needs to know about boost::filesystem or it throws an error when it
// gets to ofFileUtils.h
namespace boost {
	namespace filesystem {}
}

// DIFF: std::filesystem::path is converted to string in the target language
%ignore std::filesystem::path;

// ----- ofConstants.h -----

// GL types used as OF arguments, etc so SWIG needs to know about them
typedef int GLint;
typedef unsigned int GLenum;
typedef unsigned int GLuint;
typedef float GLfloat;

// this in tesselator.h but SWIG needs to know about it for ofIndexType
#if defined(TARGET_OPENGLES)
	typedef unsigned short TESSindex;
#else
	typedef unsigned int TESSindex;
#endif

%include "utils/ofConstants.h"

// tell SWIG about template vectors
namespace std {
#ifdef OF_SWIG_RENAME
	%template(IndexTypeVector) std::vector<ofIndexType>;
#else
	%template(ofIndexTypeVector) std::vector<ofIndexType>;
#endif
};

// ----- ofMathConstants.h -----

%ignore ofDefaultVec2;
%ignore ofDefaultVec3;
%ignore ofDefaultVec4;
%ignore ofDefaultTexCoordType;

// import the glm types
%import(module="glm") "../../glm.i"

// include early for glm::vec* declarations,
// edit: not quite needed for now as this is handled below...
// ... needed for math constants
%include "math/ofMathConstants.h"

// ----- ofUtils.h -----

// TODO: ignore ofTime std::chrono stuff?

// DIFF: ofUtils.h:
// DIFF:   ignoring ofFromString as templating results in too much overloading
%ignore ofFromString;

// DIFF:   variable argument support is painful, safer to ignore
// see http://www.swig.org/Doc2.0/Varargs.html
%ignore ofVAArgsToString;

// DIFF:   ignoring ofUTF8Iterator
%ignore ofUTF8Iterator;

// manually rename these otherwise the initial U in UTF ends up lowercase
#ifdef OF_SWIG_RENAME
	%rename(ofUTF8Append) UTF8Append;
	%rename(ofUTF8Insert) UTF8Insert;
	%rename(ofUTF8Erase) UTF8Erase;
	%rename(ofUTF8Substring) UTF8Substring;
	%rename(ofUTF8ToString) UTF8ToString;
	%rename(ofUTF8Length) UTF8Length;
#endif

// include early for ofToString template declaration
%include "utils/ofUtils.h"

// ignore further redefinitions
%ignore ofToString(const T &);

// ----- ofFbo.h -----

// need to forward declare these for ofFbo
%ignore ofBaseDraws;
class ofBaseDraws {};

%ignore ofBaseHasTexture;
class ofBaseHasTexture {};

%ignore ofBaseHasPixels;
class ofBaseHasPixels {};

// DIFF:   ignoring const & copy constructor in favor of && constructor
%ignore ofFbo::ofFbo(ofFbo const &);

// DIFF:   setUseTexture & isUsingTexture are "irrelevant", so ignoring
%ignore ofFbo::setUseTexture;
%ignore ofFbo::isUsingTexture;

// DIFF:   ignoring setActiveDrawBufers() due to std::vector
%ignore setActiveDrawBuffers(const vector<int>& i);

// DIFF:   ignoring ofFbo::Settings struct
%ignore ofFbo::Settings;

%include "gl/ofFbo.h"

// ----- ofTexture.h -----

// DIFF: ofTexture.h: ignoring const & copy constructor in favor of && constructor
%ignore ofTexture::ofTexture(ofTexture const &);

%include "gl/ofTexture.h"

// ----- ofImage.h -----

// forward declare needed types
%ignore ofBaseImage_;
template<typename T> class ofBaseImage_ {};

// forward declare base template classes
%ignore ofBaseImage;
%ignore ofBaseFloatImage;
%ignore ofBaseShortImage;
#ifdef OF_SWIG_RENAME
	%template(BaseImage) ofBaseImage_<unsigned char>;
	%template(BaseFloatImage) ofBaseImage_<float>;
	%template(BaseShortImage) ofBaseImage_<unsigned short>;
#else
	%template(ofBaseImage) ofBaseImage_<unsigned char>;
	%template(ofBaseFloatImage) ofBaseImage_<float>;
	%template(ofBaseShortImage) ofBaseImage_<unsigned short>;
#endif

// DIFF: ofImage.h:
// DIFF:   ignore ofCloseFreeImage()
%ignore ofCloseFreeImage;

// DIFF:   ignoring ofPixels operator
%ignore ofImage_::operator ofPixels_<PixelType>&();

// DIFF:   ignoring const & copy constructor in favor of && constructor
%ignore ofImage_(const ofImage_<PixelType>&);

// TODO:   find a way to release ofColor instances returned by getColor()

%include "graphics/ofImage.h"

// handle template implementations
#ifdef OF_SWIG_RENAME
	%template(Image) ofImage_<unsigned char>;
	%template(FloatImage) ofImage_<float>;
	%template(ShortImage) ofImage_<unsigned short>;
#else
	%template(ofImage) ofImage_<unsigned char>;
	%template(ofFloatImage) ofImage_<float>;
	%template(ofShortImage) ofImage_<unsigned short>;
#endif

// ----- ofGraphicsBaseTypes.h -----

// DIFF: ofGraphicsBaseTypes.h: ignore all abstract and base types
%ignore ofAbstractParameter;
%ignore ofBaseDraws;
%ignore ofBaseUpdates;
%ignore ofBaseHasTexture;
%ignore ofBaseHasTexturePlanes;

%ignore ofAbstractHasPixels;
%ignore ofBaseHasPixels_;
%ignore ofBaseHasPixels;
%ignore ofBaseHasFloatPixels;
%ignore ofBaseHasShortPixels;

%ignore ofAbstractImage;
%ignore ofBaseImage_;
%ignore ofBaseImage;
%ignore ofBaseFloatImage;
%ignore ofBaseShortImage;

%ignore ofBaseRenderer;
%ignore ofBaseGLRenderer;

%ignore ofBaseSerializer;
%ignore ofBaseFileSerializer;
%ignore ofBaseURLFileLoader;
%ignore ofBaseMaterial;

// include header for derived classes
%include "graphics/ofGraphicsBaseTypes.h"

// ----- ofSoundBaseTypes.h -----

// DIFF: ofSoundBaseTypes.h: ignore all abstract and base types

%ignore ofBaseSoundInput;
%ignore ofBaseSoundOutput;

// include header for derived classes
%include "sound/ofSoundBaseTypes.h"

// ----- ofVideoBaseTypes.h -----

// DIFF: ofVideoBaseTypes.h: ignore all abstract and base types

%ignore ofBaseVideo;
%ignore ofBaseVideoDraws;
%ignore ofBaseVideoGrabber;
%ignore ofBaseVideoPlayer;

// include header for derived classes
%include "video/ofVideoBaseTypes.h"
