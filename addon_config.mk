# All variables and this file are optional, if they are not present the PG and the
# makefiles will try to parse the correct values from the file system.
#
# Variables that specify exclusions can use % as a wildcard to specify that anything in
# that position will match. A partial path can also be specified to, for example, exclude
# a whole folder from the parsed paths from the file system
#
# Variables can be specified using = or +=
# = will clear the contents of that variable both specified from the file or the ones parsed
# from the file system
# += will add the values to the previous ones in the file or the ones parsed from the file 
# system
# 
# The PG can be used to detect errors in this file, just create a new project with this addon 
# and the PG will write to the console the kind of error and in which line it is

meta:
	ADDON_NAME = ofxOfelia
	ADDON_DESCRIPTION = Addon for running the ofelia patch in an OF app
	ADDON_AUTHOR = Zack Lee
	ADDON_TAGS = "ofelia" "pd" "lua"
	ADDON_URL = http://github.com/cuinjune/ofxOfelia

common:
	ADDON_CFLAGS = -DHAVE_UNISTD_H 
	ADDON_CFLAGS += -DUSEAPI_DUMMY
	ADDON_CFLAGS += -DPD
	ADDON_CFLAGS += -DLIBPD_EXTRA
	ADDON_CFLAGS += -DLIBPD_USE_STD_MUTEX
	ADDON_CPPFLAGS += -DTARGET_STANDALONE
	ADDON_SOURCES_EXCLUDE += libs/ofxLua/swig/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxLua/swig/%
	
linux64:
	ADDON_PKG_CONFIG_LIBRARIES = alsa
	ADDON_PKG_CONFIG_LIBRARIES += jack
	ADDON_SOURCES_EXCLUDE += libs/ofxLua/src/bindings/ios/%
	ADDON_SOURCES_EXCLUDE += libs/ofxLua/src/bindings/linuxarm/%
	ADDON_SOURCES_EXCLUDE += libs/ofxMidi/libs/pgmidi/%
	ADDON_SOURCES_EXCLUDE += libs/ofxMidi/src/ios/%
	ADDON_SOURCES_EXCLUDE += libs/ofxPd/libs/libpd/lib/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxLua/src/bindings/ios/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxLua/src/bindings/linuxarm/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxMidi/libs/pgmidi/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxMidi/src/ios/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxPd/libs/libpd/lib/%
	ADDON_CFLAGS += -DLUA_USE_LINUX
	ADDON_CFLAGS += -DHAVE_LIBDL

linux:
	ADDON_PKG_CONFIG_LIBRARIES = alsa
	ADDON_PKG_CONFIG_LIBRARIES += jack
	ADDON_SOURCES_EXCLUDE += libs/ofxLua/src/bindings/ios/%
	ADDON_SOURCES_EXCLUDE += libs/ofxLua/src/bindings/linuxarm/%
	ADDON_SOURCES_EXCLUDE += libs/ofxMidi/libs/pgmidi/%
	ADDON_SOURCES_EXCLUDE += libs/ofxMidi/src/ios/%
	ADDON_SOURCES_EXCLUDE += libs/ofxPd/libs/libpd/lib/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxLua/src/bindings/ios/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxLua/src/bindings/linuxarm/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxMidi/libs/pgmidi/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxMidi/src/ios/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxPd/libs/libpd/lib/%
	ADDON_CFLAGS += -DLUA_USE_LINUX
	ADDON_CFLAGS += -DHAVE_LIBDL

linuxarmv6l:
	ADDON_PKG_CONFIG_LIBRARIES = alsa
	ADDON_SOURCES_EXCLUDE += libs/ofxLua/src/bindings/desktop/%
	ADDON_SOURCES_EXCLUDE += libs/ofxLua/src/bindings/ios/%
	ADDON_SOURCES_EXCLUDE += libs/ofxMidi/libs/pgmidi/%
	ADDON_SOURCES_EXCLUDE += libs/ofxMidi/src/ios/%
	ADDON_SOURCES_EXCLUDE += libs/ofxPd/libs/libpd/lib/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxLua/src/bindings/desktop/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxLua/src/bindings/ios/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxMidi/libs/pgmidi/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxMidi/src/ios/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxPd/libs/libpd/lib/%
	ADDON_CFLAGS += -DLUA_USE_LINUX
	ADDON_CFLAGS += -DHAVE_LIBDL

linuxarmv7l:
	ADDON_PKG_CONFIG_LIBRARIES = alsa
	ADDON_SOURCES_EXCLUDE += libs/ofxLua/src/bindings/desktop/%
	ADDON_SOURCES_EXCLUDE += libs/ofxLua/src/bindings/ios/%
	ADDON_SOURCES_EXCLUDE += libs/ofxMidi/libs/pgmidi/%
	ADDON_SOURCES_EXCLUDE += libs/ofxMidi/src/ios/%
	ADDON_SOURCES_EXCLUDE += libs/ofxPd/libs/libpd/lib/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxLua/src/bindings/desktop/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxLua/src/bindings/ios/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxMidi/libs/pgmidi/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxMidi/src/ios/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxPd/libs/libpd/lib/%
	ADDON_CFLAGS += -DLUA_USE_LINUX
	ADDON_CFLAGS += -DHAVE_LIBDL

msys2:
	ADDON_SOURCES_EXCLUDE += libs/ofxLua/src/bindings/ios/%
	ADDON_SOURCES_EXCLUDE += libs/ofxLua/src/bindings/linuxarm/%
	ADDON_SOURCES_EXCLUDE += libs/ofxMidi/libs/pgmidi/%
	ADDON_SOURCES_EXCLUDE += libs/ofxMidi/src/ios/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxLua/src/bindings/ios/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxLua/src/bindings/linuxarm/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxMidi/libs/pgmidi/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxMidi/src/ios/%
	ADDON_CFLAGS += -DLUA_USE_WINDOWS
	ADDON_CFLAGS += -DPD_INTERNAL
	ADDON_CFLAGS += -DHAVE_STRUCT_TIMESPEC

vs:
	ADDON_SOURCES_EXCLUDE += libs/ofxLua/src/bindings/ios/%
	ADDON_SOURCES_EXCLUDE += libs/ofxLua/src/bindings/linuxarm/%
	ADDON_SOURCES_EXCLUDE += libs/ofxMidi/libs/pgmidi/%
	ADDON_SOURCES_EXCLUDE += libs/ofxMidi/src/ios/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxLua/src/bindings/ios/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxLua/src/bindings/linuxarm/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxMidi/libs/pgmidi/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxMidi/src/ios/%
	ADDON_CFLAGS += -DLUA_USE_WINDOWS
	ADDON_CFLAGS += -DPD_INTERNAL
	ADDON_CFLAGS += -DHAVE_STRUCT_TIMESPEC

android/armeabi:
	ADDON_SOURCES_EXCLUDE += libs/ofxLua/src/bindings/desktop/%
	ADDON_SOURCES_EXCLUDE += libs/ofxLua/src/bindings/linuxarm/%
	ADDON_SOURCES_EXCLUDE += libs/ofxMidi/libs/pgmidi/%
	ADDON_SOURCES_EXCLUDE += libs/ofxMidi/src/ios/%
	ADDON_SOURCES_EXCLUDE += libs/ofxPd/libs/libpd/lib/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxLua/src/bindings/desktop/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxLua/src/bindings/linuxarm/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxMidi/libs/pgmidi/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxMidi/src/ios/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxPd/libs/libpd/lib/%
	ADDON_CFLAGS += -DLUA_USE_ANDROID
	ADDON_CFLAGS += -DHAVE_LIBDL

android/armeabi-v7a:
	ADDON_SOURCES_EXCLUDE += libs/ofxLua/src/bindings/desktop/%
	ADDON_SOURCES_EXCLUDE += libs/ofxLua/src/bindings/linuxarm/%
	ADDON_SOURCES_EXCLUDE += libs/ofxMidi/libs/pgmidi/%
	ADDON_SOURCES_EXCLUDE += libs/ofxMidi/src/ios/%
	ADDON_SOURCES_EXCLUDE += libs/ofxPd/libs/libpd/lib/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxLua/src/bindings/desktop/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxLua/src/bindings/linuxarm/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxMidi/libs/pgmidi/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxMidi/src/ios/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxPd/libs/libpd/lib/%
	ADDON_CFLAGS += -DLUA_USE_ANDROID
	ADDON_CFLAGS += -DHAVE_LIBDL

osx:
	ADDON_FRAMEWORKS = CoreMIDI
	ADDON_SOURCES_EXCLUDE += libs/ofxLua/src/bindings/ios/%
	ADDON_SOURCES_EXCLUDE += libs/ofxLua/src/bindings/linuxarm/%
	ADDON_SOURCES_EXCLUDE += libs/ofxMidi/libs/pgmidi/%
	ADDON_SOURCES_EXCLUDE += libs/ofxMidi/src/ios/%
	ADDON_SOURCES_EXCLUDE += libs/ofxPd/libs/libpd/lib/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxLua/src/bindings/ios/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxLua/src/bindings/linuxarm/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxMidi/libs/pgmidi/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxMidi/src/ios/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxPd/libs/libpd/lib/%
	ADDON_CFLAGS += -DLUA_USE_MACOSX
	ADDON_CFLAGS += -DHAVE_LIBDL
	ADDON_CPPFLAGS += -DHAVE_UNISTD_H=1

ios:
	ADDON_FRAMEWORKS = CoreMIDI
	ADDON_SOURCES_EXCLUDE += libs/ofxLua/src/bindings/desktop/%
	ADDON_SOURCES_EXCLUDE += libs/ofxLua/src/bindings/linuxarm/%
	ADDON_SOURCES_EXCLUDE += libs/ofxMidi/libs/rtmidi/%
	ADDON_SOURCES_EXCLUDE += libs/ofxMidi/libs/portmidi/%
	ADDON_SOURCES_EXCLUDE += libs/ofxMidi/src/desktop/%
	ADDON_SOURCES_EXCLUDE += libs/ofxPd/libs/libpd/lib/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxLua/src/bindings/desktop/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxLua/src/bindings/linuxarm/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxMidi/libs/rtmidi/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxMidi/libs/portmidi/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxMidi/src/desktop/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxPd/libs/libpd/lib/%
	ADDON_CFLAGS += -fcommon
	ADDON_CPPFLAGS += -DHAVE_UNISTD_H=1

emscripten:
	ADDON_SOURCES_EXCLUDE += libs/ofxLua/src/bindings/desktop/%
	ADDON_SOURCES_EXCLUDE += libs/ofxLua/src/bindings/linuxarm/%
	ADDON_SOURCES_EXCLUDE += libs/ofxMidi/libs/pgmidi/%
	ADDON_SOURCES_EXCLUDE += libs/ofxMidi/src/ios/%
	ADDON_SOURCES_EXCLUDE += libs/ofxPd/libs/libpd/lib/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxLua/src/bindings/desktop/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxLua/src/bindings/linuxarm/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxMidi/libs/pgmidi/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxMidi/src/ios/%
	ADDON_INCLUDES_EXCLUDE += libs/ofxPd/libs/libpd/lib/%
	ADDON_CFLAGS += -DHAVE_LIBDL
