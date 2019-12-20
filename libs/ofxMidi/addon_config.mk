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
	ADDON_NAME = ofxMidi
	ADDON_DESCRIPTION = MIDI interface for OpenFrameworks
	ADDON_AUTHOR = Dan Wilcox (original implementation by Chris OShea, Arturo Castro, Kyle McDonald)
	ADDON_TAGS = "MIDI" "audio" "control"
	ADDON_URL = http://github.com/danomatika/ofxMidi

common:
	
linux64:
	ADDON_PKG_CONFIG_LIBRARIES = alsa
	ADDON_PKG_CONFIG_LIBRARIES += jack
	ADDON_SOURCES_EXCLUDE = libs/pgmidi/%
	ADDON_SOURCES_EXCLUDE += src/ios/%
	ADDON_INCLUDES_EXCLUDE = libs/pgmidi/%
	ADDON_INCLUDES_EXCLUDE += src/ios/%

linux:
	ADDON_PKG_CONFIG_LIBRARIES = alsa
	ADDON_PKG_CONFIG_LIBRARIES += jack
	ADDON_SOURCES_EXCLUDE = libs/pgmidi/%
	ADDON_SOURCES_EXCLUDE += src/ios/%
	ADDON_INCLUDES_EXCLUDE = libs/pgmidi/%
	ADDON_INCLUDES_EXCLUDE += src/ios/%

linuxarmv6l:
	ADDON_PKG_CONFIG_LIBRARIES = alsa
	ADDON_SOURCES_EXCLUDE = libs/pgmidi/%
	ADDON_SOURCES_EXCLUDE += src/ios/%
	ADDON_INCLUDES_EXCLUDE = libs/pgmidi/%
	ADDON_INCLUDES_EXCLUDE += src/ios/%

linuxarmv7l:
	ADDON_PKG_CONFIG_LIBRARIES = alsa
	ADDON_SOURCES_EXCLUDE = libs/pgmidi/%
	ADDON_SOURCES_EXCLUDE += src/ios/%
	ADDON_INCLUDES_EXCLUDE = libs/pgmidi/%
	ADDON_INCLUDES_EXCLUDE += src/ios/%

msys2:
	ADDON_SOURCES_EXCLUDE = libs/pgmidi/%
	ADDON_SOURCES_EXCLUDE += src/ios/%
	ADDON_INCLUDES_EXCLUDE = libs/pgmidi/%
	ADDON_INCLUDES_EXCLUDE += src/ios/%

vs:
	ADDON_SOURCES_EXCLUDE = libs/pgmidi/%
	ADDON_SOURCES_EXCLUDE += src/ios/%
	ADDON_INCLUDES_EXCLUDE = libs/pgmidi/%
	ADDON_INCLUDES_EXCLUDE += src/ios/%

android/armeabi:
	ADDON_SOURCES_EXCLUDE = libs/pgmidi/%
	ADDON_SOURCES_EXCLUDE += src/ios/%
	ADDON_INCLUDES_EXCLUDE = libs/pgmidi/%
	ADDON_INCLUDES_EXCLUDE += src/ios/%

android/armeabi-v7a:
	ADDON_SOURCES_EXCLUDE = libs/pgmidi/%
	ADDON_SOURCES_EXCLUDE += src/ios/%
	ADDON_INCLUDES_EXCLUDE = libs/pgmidi/%
	ADDON_INCLUDES_EXCLUDE += src/ios/%

osx:
	ADDON_FRAMEWORKS = CoreMIDI
	ADDON_SOURCES_EXCLUDE = libs/pgmidi/%
	ADDON_SOURCES_EXCLUDE += src/ios/%
	ADDON_INCLUDES_EXCLUDE = libs/pgmidi/%
	ADDON_INCLUDES_EXCLUDE += src/ios/%

ios:
	ADDON_FRAMEWORKS = CoreMIDI
	ADDON_SOURCES_EXCLUDE = libs/rtmidi/%
	ADDON_SOURCES_EXCLUDE += libs/portmidi/%
	ADDON_SOURCES_EXCLUDE += src/desktop/%
	ADDON_INCLUDES_EXCLUDE = libs/rtmidi/%
	ADDON_INCLUDES_EXCLUDE += libs/portmidi/%
	ADDON_INCLUDES_EXCLUDE += src/desktop/%
