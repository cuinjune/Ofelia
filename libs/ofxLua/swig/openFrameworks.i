// SWIG (http://www.swig.org) interface wrapper for the openFrameworks core API
// 2014-17 Dan Wilcox <danomatika@gmail.com>

// workaround when compiling in MinGW (Python)
%begin %{
#if defined( __WIN32__ ) || defined( _WIN32 )
	#include <cmath>
#endif
%}

// main MODULE
%module MODULE_NAME
%{
	#include "ofMain.h"
	#undef check
%}
%include <attribute.i>
%include <typemaps.i>

// custom attribute wrapper for nested union var access
%define %attributeVar(Class, AttributeType, AttributeName, GetVar, SetVar...)
	#if #SetVar != ""
		%attribute_custom(%arg(Class), %arg(AttributeType), AttributeName, GetVar, SetVar, self_->GetVar, self_->SetVar = val_)
	#else
		%attribute_readonly(%arg(Class), %arg(AttributeType), AttributeName, GetVar, self_->GetVar)
	#endif
%enddef

// ----- C++ -----

// SWIG doesn't understand C++ streams
%ignore operator <<;
%ignore operator >>;

// expanded primitives
%typedef unsigned int size_t;
%typedef long long int64_t;
%typedef unsigned long long uint64_t;

%include <stl.i>
%include <std_string.i>
%include <std_vector.i>

// ----- Renaming -----

#ifdef OF_SWIG_RENAME

	// strip "of" prefix from classes
	%rename("%(strip:[of])s", %$isclass) "";

	// strip "of" prefix from global functions & make first char lowercase
	%rename("%(regex:/of(.*)/\\l\\1/)s", %$isfunction) "";

	// strip "OF_" from constants & enums
	%rename("%(strip:[OF_])s", %$isconstant) "";
	%rename("%(strip:[OF_])s", %$isenumitem) "";

#endif

// ----- Lang specifics ------

#ifdef SWIGLUA
%include "openFrameworks/lang/lua/lua.i"
#endif

#ifdef SWIGPYTHON
%include "openFrameworks/lang/python/python.i"
#endif

// ----- Deprecated ------

#ifndef OF_SWIG_DEPRECATED
	%include "openFrameworks/deprecated.i"
#endif

// ----- Bindings------

// Look for the follow notes:
//     TODO: something that might need to be updated or fixed in the future
//     DIFF: an important difference as compared to the OF C++ API
//
// Make sure to %rename & %ignore *before* %including a header.
//
// The order of class declarations is important! This is why a number of
// classes are %included at the beginning of main.i before ofBaseTypes.h.
//
// If a header forward declares a class like (AnotherClass.h):
//
//     class SomeClass;
//     class AnotherClass {
//     ...
//
// and this header is %included before the actual class implementation
// (SomeClass.h), SWIG will wrap the empty declaration and *not* the actual
// class! This basically allows you to create the class in the bound language,
// but it does nothing and has no attributes or functions. Bummer.
//
// What needs to be done is either %include SomeClass.h before AnotherClass.h
//
//    %include "SomeClass.h"
//    %include "AnotherClass.h"
//
// or make a forward declaration before %including SomeClass.h:
//
//    class SomeClass {};
//    %include AnotherClass.h
//
// This forward declaration is then overridden by the actual implementation after
// %include "SomeClass.h" later on.

%include "openFrameworks/openFrameworks/main.i"
%include "openFrameworks/openFrameworks/3d.i"
%include "openFrameworks/openFrameworks/app.i"
#if !defined(TARGET_IOS) && !defined(TARGET_ANDROID)
%include "openFrameworks/openFrameworks/communication.i"
#endif
%include "openFrameworks/openFrameworks/math.i"
%include "openFrameworks/openFrameworks/events.i"
%include "openFrameworks/openFrameworks/gl.i"
%include "openFrameworks/openFrameworks/graphics.i"
%include "openFrameworks/openFrameworks/sound.i"
%include "openFrameworks/openFrameworks/types.i"
%include "openFrameworks/openFrameworks/utils.i"
%include "openFrameworks/openFrameworks/video.i"

// ----- Attributes ------

#ifdef OF_SWIG_ATTRIBUTES
	%include "openFrameworks/attributes.i"
#endif

// ----- Lang code ------

#ifdef SWIGLUA
%include "openFrameworks/lang/lua/lua_code.i"
#endif

#ifdef SWIGPYTHON
%include "openFrameworks/lang/python/python_code.i"
#endif
