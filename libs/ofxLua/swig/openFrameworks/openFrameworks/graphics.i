// graphics folder bindings
// 2017 Dan Wilcox <danomatika@gmail.com>

// ----- ofGraphicsBaseTypes.h -----

// handled in main.i

// ----- ofGraphicsConstants.h -----

// make sure we use the actual classes
%ignore ofDefaultVertexType;
%ignore ofDefaultNormalType;
%ignore ofDefaultColorType;
%ignore ofDefaultTexCoordType;

%include "graphics/ofGraphicsConstants.h"

// ----- ofPixels.h -----

// include pixels first as it's used by most other classes

// DIFF: ofPixels.h:
// DIFF:   ignoring const & copy constructor in favor of && constructor
%ignore ofPixels_(const ofPixels_<PixelType> &);

// DIFF: ofPixels.h:
// DIFF:   fixed ambiguous ofPixels function overloads since enums = int in SWIG
// DIFF:   by renaming to allocatePixelFormat, allocateImageType, & setFromPixelsImageType
%rename(allocatePixelFormat) ofPixels_<unsigned char>::allocate(size_t, size_t, ofPixelFormat);
%rename(allocateImageType) ofPixels_<unsigned char>::allocate(size_t, size_t, ofImageType);
%rename(setFromPixelsImageType) ofPixels_<unsigned char>::setFromPixels(unsigned char const *, size_t, size_t, ofImageType);

%rename(allocatePixelFormat) ofPixels_<float>::allocate(size_t, size_t, ofPixelFormat);
%rename(allocateImageType) ofPixels_<float>::allocate(size_t, size_t, ofImageType);
%rename(setFromPixelsImageType) ofPixels_<float>::setFromPixels(float const *, size_t, size_t, ofImageType);

%rename(allocatePixelFormat) ofPixels_<unsigned short>::allocate(size_t, size_t, ofPixelFormat);
%rename(allocateImageType) ofPixels_<unsigned short>::allocate(size_t, size_t, ofImageType);
%rename(setFromPixelsImageType) ofPixels_<unsigned short>::setFromPixels(unsigned short const *, size_t, size_t, ofImageType);

// DIFF: ofPixels.h:
// DIFF:   ignore overloaded setFromPixels, setFromExternalPixels, &
// DIFF:   setFromAlignedPixels w/ channels argument,
// DIFF:   use ofPixelType overloaded functions instead
%ignore ofPixels_<unsigned char>::setFromPixels(unsigned char const *, size_t, size_t, size_t);
%ignore ofPixels_<float>::setFromPixels(float const *, size_t, size_t, size_t);
%ignore ofPixels_<unsigned short>::setFromPixels(unsigned short const *, size_t, size_t, size_t);

%ignore ofPixels_<unsigned char>::setFromExternalPixels(unsigned char *, size_t, size_t, size_t);
%ignore ofPixels_<float>::setFromExternalPixels(float *, size_t, size_t, size_t);
%ignore ofPixels_<unsigned short>::setFromExternalPixels(unsigned short *, size_t, size_t, size_t);

%ignore ofPixels_<unsigned char>::setFromAlignedPixels(const unsigned char *, size_t, size_t, size_t, size_t);
%ignore ofPixels_<float>::setFromAlignedPixels(const float *, size_t, size_t, size_t, size_t);
%ignore ofPixels_<unsigned short>::setFromAlignedPixels(const unsigned short *, size_t, size_t, size_t, size_t);

// DIFF: ofPixels.h:
// DIFF:   ignore setFromAlignedPixels with vector arguments
%ignore ofPixels_<unsigned char>::setFromAlignedPixels(const unsigned char *, size_t, size_t, ofPixelFormat, std::vector<size_t>);
%ignore ofPixels_<float>::setFromAlignedPixels(const float *, size_t, size_t, ofPixelFormat, std::vector<size_t>);
%ignore ofPixels_<unsigned short>::setFromAlignedPixels(const unsigned short *, size_t, size_t, ofPixelFormat, std::vector<size_t>);

%ignore ofPixels_<unsigned char>::setFromAlignedPixels(const unsigned char *, size_t, size_t, ofPixelFormat, std::vector<int>);
%ignore ofPixels_<float>::setFromAlignedPixels(const float *, size_t, size_t, ofPixelFormat, std::vector<int>);
%ignore ofPixels_<unsigned short>::setFromAlignedPixels(const unsigned short *, size_t, size_t, ofPixelFormat, std::vector<int>);

// DIFF:   ignoring static functions
%ignore ofPixels_::pixelBitsFromPixelFormat(ofPixelFormat);
%ignore ofPixels_::bytesFromPixelFormat(size_t, size_t, ofPixelFormat);

// DIFF:   ignoring nested structs: Pixel, Line, ConstPixel, & ConstLine
%ignore ofPixels_::ConstPixel;
%ignore ofPixels_::Pixel;
%ignore ofPixels_::Pixels;
%ignore ofPixels_::Line;
%ignore ofPixels_::Lines;
%ignore ofPixels_::ConstPixels;
%ignore ofPixels_::ConstLine;
%ignore ofPixels_::ConstLines;

// DIFF:   ignoring iterators
%ignore ofPixels_::begin;
%ignore ofPixels_<unsigned char>::end;
%ignore ofPixels_<float>::end;
%ignore ofPixels_<unsigned short>::end;
%ignore ofPixels_::rbegin;
%ignore ofPixels_::rend;
%ignore ofPixels_::begin const;
%ignore ofPixels_::end const;
%ignore ofPixels_::rbegin const;
%ignore ofPixels_::rend const;
%ignore ofPixels_::getLine(size_t);
%ignore ofPixels_::getLines();
%ignore ofPixels_::getLines(size_t, size_t);
%ignore ofPixels_::getPixelsIter();
%ignore ofPixels_::getConstLine(size_t) const;
%ignore ofPixels_::getConstLines() const;
%ignore ofPixels_::getConstLines(size_t, size_t) const;
%ignore ofPixels_::getConstPixelsIter() const;

%include "graphics/ofPixels.h"

// tell SWIG about template classes
#ifdef OF_SWIG_RENAME
	%template(Pixels) ofPixels_<unsigned char>;
	%template(FloatPixels) ofPixels_<float>;
	%template(ShortPixels) ofPixels_<unsigned short>;
#else
	%template(ofPixels) ofPixels_<unsigned char>;
	%template(ofFloatPixels) ofPixels_<float>;
	%template(ofShortPixels) ofPixels_<unsigned short>;
#endif

// ----- ofPath.h -----

// tell SWIG about template vectors
namespace std {
#ifdef OF_SWIG_RENAME
	%template(PathVector) std::vector<ofPath>;
#else
	%template(ofPathVector) std::vector<ofPath>;
#endif
};

// DIFF: ofPath.h: ignoring nested Command struct
%ignore ofPath::Command;

%include "graphics/ofPath.h"

// ----- ofPolyline.h -----

// tell SWIG about template vectors
namespace std {
#ifdef OF_SWIG_RENAME
	%template(PolylineVector) std::vector<ofPolyline>;
#else
	%template(ofPolylineVector) std::vector<ofPolyline>;
#endif
};

// ignored due to default variable overload
%ignore ofPolyline_::arc(float, float, float, float, float, float, float);
%ignore ofPolyline_::arcNegative(float, float, float, float, float, float, float);

// DIFF: ofPolyline.h: ignoring iterators
%ignore ofPolyline_::begin;
%ignore ofPolyline_::end;
%ignore ofPolyline_::rbegin;
%ignore ofPolyline_::rend;

%include "graphics/ofPolyline.h"

// tell SWIG about template classes
#ifdef OF_SWIG_RENAME
	%template(Polyline) ofPolyline_<ofDefaultVertexType>;
#else
	%template(ofPolyline) ofPolyline_<ofDefaultVertexType>;
#endif

// ----- ofRendererCollection.h -----

// not needed

// ----- ofCairoRenderer.h -----

// not needed

// ----- ofBitmapFont.h -----

// not needed

// ----- ofGraphics.h -----

// no PDF or SVG export support on mobile
#if defined(TARGET_IOS) || defined(TARGET_ANDROID)
	%ignore ofBeginSaveScreenAsPDF;
	%ignore ofEndSaveScreenAsPDF();
	%ignore ofBeginSaveScreenAsSVG;
	%ignore ofEndSaveScreenAsSVG();
#endif

// DIFF: ofGraphics.h:
// DIFF:   ignoring ofDrawBitmapString() template functions in favor of
// DIFF:   string versions, target languages can handle the string conversions
%ignore ofDrawBitmapString(const T &, float, float);
%ignore ofDrawBitmapString(const T &, const glm::vec3 &);
%ignore ofDrawBitmapString(const T &, const glm::vec2 &);
%ignore ofDrawBitmapString(const T &, float, float, float);

// manually define string functions here otherwise they get redefined by SWIG & then ignored
void ofDrawBitmapString(const std::string & textString, float x, float y);
void ofDrawBitmapString(const std::string & textString, const glm::vec3 & p);
void ofDrawBitmapString(const std::string & textString, const glm::vec2 & p);
void ofDrawBitmapString(const std::string & textString, float x, float y, float z);

%include "graphics/ofGraphics.h"

// ----- of3dGraphics.h -----

// ignore base classes
%ignore of3dGraphics;

%include "graphics/of3dGraphics.h"

// ----- ofImage.h -----

// handled in main.i

// ----- ofTessellator.h -----

// not needed

// ----- ofTrueTypeFont.h -----

// DIFF: ofTrueTypeFont.h:
//         ignore internal font structs
%ignore FT_Face;

// strip "of" from following ofAlphabet enums
#ifdef OF_SWIG_RENAME
	%rename("%(regex:/of(Alphabet.*)/\\1/)s", %$isenumitem) "";
#endif

// replace std::initializer with an enum that SWIG understands
%inline %{
	enum ofAlphabetEnum : int {
		ofAlphabet_Emoji,
		ofAlphabet_Japanese,
		ofAlphabet_Chinese,
		ofAlphabet_Korean,
		ofAlphabet_Arabic,
		ofAlphabet_Devanagari,
		ofAlphabet_Latin,
		ofAlphabet_Greek,
		ofAlphabet_Cyrillic
	};
%}
%ignore ofAlphabet;

// ignore ofUnicode::range nested struct warning
%warnfilter(325) ofUnicode::range;

// DIFF:   ignoring ofTrueTypeShutdown() & ofExitCallback() friend
%ignore ofTrueTypeShutdown;
%ignore ofExitCallback;

// ignore std::initializer
%ignore ofTrueTypeFontSettings::addRanges(std::initializer_list<ofUnicode::range>);

// DIFF:   replaced ofAlphabet static instances with afAlphabet_ enums
%extend ofTrueTypeFontSettings {
	void addRanges(ofAlphabetEnum alphabet) {
		switch(alphabet) {
			case ofAlphabet_Emoji:
				$self->addRanges(ofAlphabet::Emoji);
				break;
			case ofAlphabet_Japanese:
				$self->addRanges(ofAlphabet::Japanese);
				break;
			case ofAlphabet_Chinese:
				$self->addRanges(ofAlphabet::Chinese);
				break;
			case ofAlphabet_Korean:
				$self->addRanges(ofAlphabet::Korean);
				break;
			case ofAlphabet_Arabic:
				$self->addRanges(ofAlphabet::Arabic);
				break;
			case ofAlphabet_Devanagari:
				$self->addRanges(ofAlphabet::Devanagari);
				break;
			case ofAlphabet_Latin:
				$self->addRanges(ofAlphabet::Latin);
				break;
			case ofAlphabet_Greek:
				$self->addRanges(ofAlphabet::Greek);
				break;
			case ofAlphabet_Cyrillic:
				$self->addRanges(ofAlphabet::Cyrillic);
				break;
			default:
				break;
		}
	}
}

// DIFF:   ignoring const & copy constructor in favor of && constructor
%ignore ofTrueTypeFont::ofTrueTypeFont(ofTrueTypeFont const &);

// DIFF:   ignoring protected structs
%ignore ofTrueTypeFont::range;
%ignore ofTrueTypeFont::glyph;
%ignore ofTrueTypeFont::glyphProps;

// TODO:   find a way to release ofRectangle returned by getStringBoundingBox()

%include "graphics/ofTrueTypeFont.h"
