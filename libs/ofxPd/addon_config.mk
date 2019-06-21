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
	ADDON_NAME = ofxPd
	ADDON_DESCRIPTION = Addon for running the Pure Data audio engine in an OF app
	ADDON_AUTHOR = Dan Wilcox
	ADDON_TAGS = "sound" "audio" "computer music"
	ADDON_URL = http://github.com/danomatika/ofxPd

common:
	# required for libpd
	ADDON_CFLAGS = -DHAVE_UNISTD_H -DUSEAPI_DUMMY -DPD -DLIBPD_EXTRA -DLIBPD_USE_STD_MUTEX

linux64:
	# support dynamic loading
	ADDON_CFLAGS += -DHAVE_LIBDL

linux:
	# support dynamic loading
	ADDON_CFLAGS += -DHAVE_LIBDL

linuxarmv6l:
	# support dynamic loading
	ADDON_CFLAGS += -DHAVE_LIBDL

linuxarmv7l:
	# support dynamic loading
	ADDON_CFLAGS += -DHAVE_LIBDL

msys2:
	# not sure if pd~ is working on Windows, so leave it out
	ADDON_SOURCES_EXCLUDE = libs/libpd/pure-data/extra/pd~
	ADDON_INCLUDES_EXCLUDE = libs/libpd/pure-data/extra/pd~

vs:

android/armeabi:
	# support dynamic loading
	ADDON_CFLAGS += -DHAVE_LIBDL

android/armeabi-v7a:
	# support dynamic loading
	ADDON_CFLAGS += -DHAVE_LIBDL

osx:
	# support dynamic loading
	ADDON_CFLAGS += -DHAVE_LIBDL
	
	# fix dispatch.h error with macOS SDK 10.13+
	ADDON_CPPFLAGS += -DHAVE_UNISTD_H=1

ios:
	# set No common blocks option to avoid duplicate symbols link error
	ADDON_CFLAGS += -fcommon
	
	# fix dispatch.h error with macOS SDK 10.13+
	ADDON_CPPFLAGS += -DHAVE_UNISTD_H=1
