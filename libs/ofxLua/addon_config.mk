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
	ADDON_NAME = ofxLua
	ADDON_DESCRIPTION = run a Lua scripting interface inside OF apps, create your own bindings
	ADDON_AUTHOR = Dan Wilcox
	ADDON_TAGS = "lua" "scripting" "bindings" "swig"
	ADDON_URL = http://github.com/danomatika/ofxLua

common:
	ADDON_SOURCES_EXCLUDE += swig/%
	ADDON_INCLUDES_EXCLUDE += swig/%
	
linux64:
	ADDON_CFLAGS += -DLUA_USE_LINUX
	ADDON_SOURCES_EXCLUDE += src/bindings/ios/%
	ADDON_SOURCES_EXCLUDE += src/bindings/linuxarm/%
	ADDON_INCLUDES_EXCLUDE += src/bindings/ios/%
	ADDON_INCLUDES_EXCLUDE += src/bindings/linuxarm/%
	
linux:
	ADDON_CFLAGS += -DLUA_USE_LINUX
	ADDON_SOURCES_EXCLUDE += src/bindings/ios/%
	ADDON_SOURCES_EXCLUDE += src/bindings/linuxarm/%
	ADDON_INCLUDES_EXCLUDE += src/bindings/ios/%
	ADDON_INCLUDES_EXCLUDE += src/bindings/linuxarm/%
	
msys2:
	ADDON_CFLAGS += -DLUA_USE_WINDOWS
	ADDON_SOURCES_EXCLUDE += src/bindings/ios/%
	ADDON_SOURCES_EXCLUDE += src/bindings/linuxarm/%
	ADDON_INCLUDES_EXCLUDE += src/bindings/ios/%
	ADDON_INCLUDES_EXCLUDE += src/bindings/linuxarm/%
	
vs:
	ADDON_CFLAGS += -DLUA_USE_WINDOWS
	ADDON_SOURCES_EXCLUDE += src/bindings/ios/%
	ADDON_SOURCES_EXCLUDE += src/bindings/linuxarm/%
	ADDON_INCLUDES_EXCLUDE += src/bindings/ios/%
	ADDON_INCLUDES_EXCLUDE += src/bindings/linuxarm/%
	
linuxarmv6l:
	ADDON_CFLAGS += -DLUA_USE_LINUX
	ADDON_SOURCES_EXCLUDE += src/bindings/desktop/%
	ADDON_SOURCES_EXCLUDE += src/bindings/ios/%
	ADDON_INCLUDES_EXCLUDE += src/bindings/desktop/%
	ADDON_INCLUDES_EXCLUDE += src/bindings/ios/%
	
linuxarmv7l:
	ADDON_CFLAGS += -DLUA_USE_LINUX
	ADDON_SOURCES_EXCLUDE += src/bindings/desktop/%
	ADDON_SOURCES_EXCLUDE += src/bindings/ios/%
	ADDON_INCLUDES_EXCLUDE += src/bindings/desktop/%
	ADDON_INCLUDES_EXCLUDE += src/bindings/ios/%

android/armeabi:
	ADDON_CFLAGS += -DLUA_USE_ANDROID
	ADDON_SOURCES_EXCLUDE += src/bindings/desktop/%
	ADDON_SOURCES_EXCLUDE += src/bindings/linuxarm/%
	ADDON_INCLUDES_EXCLUDE += src/bindings/desktop/%
	ADDON_INCLUDES_EXCLUDE += src/bindings/linuxarm/%

android/armeabi-v7a:	
	ADDON_CFLAGS += -DLUA_USE_ANDROID
	ADDON_SOURCES_EXCLUDE += src/bindings/desktop/%
	ADDON_SOURCES_EXCLUDE += src/bindings/linuxarm/%
	ADDON_INCLUDES_EXCLUDE += src/bindings/desktop/%
	ADDON_INCLUDES_EXCLUDE += src/bindings/linuxarm/%

osx:
	ADDON_CFLAGS += -DLUA_USE_MACOSX
	ADDON_SOURCES_EXCLUDE += src/bindings/ios/%
	ADDON_SOURCES_EXCLUDE += src/bindings/linuxarm/%
	ADDON_INCLUDES_EXCLUDE += src/bindings/ios/%
	ADDON_INCLUDES_EXCLUDE += src/bindings/linuxarm/%

ios:
	ADDON_SOURCES_EXCLUDE += src/bindings/desktop/%
	ADDON_SOURCES_EXCLUDE += src/bindings/linuxarm/%
	ADDON_INCLUDES_EXCLUDE += src/bindings/desktop/%
	ADDON_INCLUDES_EXCLUDE += src/bindings/linuxarm/%
