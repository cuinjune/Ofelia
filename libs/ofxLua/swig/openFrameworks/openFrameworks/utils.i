// utils folder bindings
// 2017 Dan Wilcox <danomatika@gmail.com>

// ----- ofFpsCounter.h -----

%include "utils/ofFpsCounter.h"

// ----- ofTimer.h -----

// not needed

// ----- ofXml.h -----

// DIFF: ofXml.h: ignoring find() as it returns a pugi::xpath_node_set
%ignore ofXml::find;

// DIFF: ofXml.h: ignoring iterators and nested structs
%ignore ofXmlIterator;
%ignore ofXmlSearchIterator;
%ignore ofXml::Search;
%ignore ofXml::Attribute;
%ignore ofXml::Range;
%ignore ofXml::Search::end;
%ignore ofXml::Range::end;

// DIFF: ofXml.h: ignore removeAttribute(&&)
%ignore ofXml::removeAttribute(Attribute &&);

// DIFF: ofXml.h: ignoring bool operator
%ignore ofXml::operator bool;

%include "utils/ofXml.h"

// ----- ofMatrixStack.h -----

%include "utils/ofMatrixStack.h"

// ----- ofConstants.h -----

// handled in main.i

// ----- ofFileUtils.h -----

// forward declare fstream for ofFile
%ignore std::fstream;
class std::fstream {};

// DIFF: ofFileUtils.h:
// DIFF:   ignoring iterators
%ignore ofBuffer::begin;
%ignore ofBuffer::end;
%ignore ofBuffer::rbegin;
%ignore ofBuffer::rend;
%ignore ofDirectory::begin;
%ignore ofDirectory::end;
%ignore ofDirectory::rbegin;
%ignore ofDirectory::rend;

// DIFF:   ignoring ofBuffer istream & ostream functions
%ignore ofBuffer::ofBuffer(std::istream &);
%ignore ofBuffer::ofBuffer(std::istream &, size_t);
%ignore ofBuffer::set(std::istream &);
%ignore ofBuffer::set(std::istream &, size_t);
%ignore ofBuffer::writeTo(std::ostream &) const;

%ignore ofBuffer::ofBuffer(const std::string &);
%ignore ofBuffer::set(const std::string &);
%ignore ofBuffer::append(const std::string&);

// ignore char* getData() in preference to const char* getData() whose return
// type is overriden below
%ignore ofBuffer::getData();

// DIFF:   pass binary data to ofBuffer as full char strings
// pass binary data & byte length as a single argument for ofBuffer
%apply(char *STRING, size_t LENGTH) {(const char * buffer, std::size_t size)};

// DIFF:   ignoring nested ofBuffer Line & Lines structs
%ignore ofBuffer::Line;
%ignore ofBuffer::Lines;
%ignore ofBuffer::getLines();

// DIFF:   ignoring nested ofBuffer RLine & RLines structs
%ignore ofBuffer::RLine;
%ignore ofBuffer::RLines;
%ignore ofBuffer::getReverseLines();

// DIFF:   ignoring string, filebuf, & std::filesystem::path operators
%ignore ofBuffer::operator std::string() const;
%ignore ofFile::getFileBuffer() const;
%ignore ofFile::operator std::filesystem::path();
%ignore ofFile::operator const std::filesystem::path() const;
%ignore ofDirectory::operator std::filesystem::path();
%ignore ofDirectory::operator const std::filesystem::path() const;

%include "utils/ofFileUtils.h"

// clear typemaps
%clear(const char *buffer, std::size_t size);
%clear(const char *_buffer, std::size_t _size);

// ----- ofLog.h -----

// function wrapper for ofLog class
%inline %{
	void log(ofLogLevel level, const std::string & message) {
		ofLog(level, message);
	}
%}

// DIFF: ofLog.h:
// DIFF:   ignore stream-based log classes since target languages won't support it
%ignore ofLog;
%ignore ofLogVerbose;
%ignore ofLogNotice;
%ignore ofLogWarning;
%ignore ofLogError;
%ignore ofLogFatalError;

// DIFF:   ignore logger channel classes
%ignore ofBaseLoggerChannel;
%ignore ofSetLoggerChannel;
%ignore ofConsoleLoggerChannel;
%ignore ofFileLoggerChannel;

%include "utils/ofLog.h"

// ----- ofNoise.h -----

// not needed

// ----- ofSystemUtils.h -----

%include "utils/ofSystemUtils.h"

// ----- ofThread.h -----

// not needed

// ----- ofThreadChannel.h -----

// not needed

// ----- ofURLFileLoader.h -----

// DIFF: ofURLFileLoader.h: ignoring ofHttpResponse ofBuffer operator
%ignore ofHttpResponse::operator ofBuffer&();

// DIFF: ofURLFileLoader.h: ignoring ofURLResponseEvent()
%ignore ofURLResponseEvent();

%include "utils/ofURLFileLoader.h"

// ----- ofUtils.h -----

// handled in main.i
