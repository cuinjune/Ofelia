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
	ADDON_NAME = ofxOsc
	ADDON_DESCRIPTION = Addon for communicating with the OSC protocol
	ADDON_AUTHOR = OF Team
	ADDON_TAGS = "networking"
	ADDON_URL = http://github.com/openframeworks/openFrameworks

common:

linux64:
	ADDON_SOURCES_EXCLUDE += libs/oscpack/src/ip/win32/%
	ADDON_INCLUDES_EXCLUDE += libs/oscpack/src/ip/win32/%

linux:
	ADDON_SOURCES_EXCLUDE += libs/oscpack/src/ip/win32/%
	ADDON_INCLUDES_EXCLUDE += libs/oscpack/src/ip/win32/%

linuxarmv6l:
	ADDON_SOURCES_EXCLUDE += libs/oscpack/src/ip/win32/%
	ADDON_INCLUDES_EXCLUDE += libs/oscpack/src/ip/win32/%

linuxarmv7l:
	ADDON_SOURCES_EXCLUDE += libs/oscpack/src/ip/win32/%
	ADDON_INCLUDES_EXCLUDE += libs/oscpack/src/ip/win32/%

msys2:
	ADDON_SOURCES_EXCLUDE += libs/oscpack/src/ip/posix/%
	ADDON_INCLUDES_EXCLUDE += libs/oscpack/src/ip/posix/%

vs:
	ADDON_SOURCES_EXCLUDE += libs/oscpack/src/ip/posix/%
	ADDON_INCLUDES_EXCLUDE += libs/oscpack/src/ip/posix/%

android/armeabi:
	ADDON_SOURCES_EXCLUDE += libs/oscpack/src/ip/win32/%
	ADDON_INCLUDES_EXCLUDE += libs/oscpack/src/ip/win32/%

android/armeabi-v7a:
	ADDON_SOURCES_EXCLUDE += libs/oscpack/src/ip/win32/%
	ADDON_INCLUDES_EXCLUDE += libs/oscpack/src/ip/win32/%

osx:
	ADDON_SOURCES_EXCLUDE += libs/oscpack/src/ip/win32/%
	ADDON_INCLUDES_EXCLUDE += libs/oscpack/src/ip/win32/%

ios:
	ADDON_SOURCES_EXCLUDE += libs/oscpack/src/ip/win32/%
	ADDON_INCLUDES_EXCLUDE += libs/oscpack/src/ip/win32/%
