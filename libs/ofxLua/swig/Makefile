#
# Makefile to generate OF bindings using SWIG
#
# 2014-2018 Dan Wilcox <danomatika@gmail.com>
#
# running `make` generates desktop os lua bindings and puts them in ./lua,
# running `make ios` generates ios lua bindings, etc
#
# override any of the following variables using make, i.e. to generate Python 
# bindings with a different filename and dest location:
#
#     make LANG=python NAME=ofxPythonBindings DEST_DIR=../src/bindings
#

# swig command
SWIG = swig

# default output language, see swig -h for more
LANG = lua

# default platform target, available targets are:
#   * desktop: win, linux, & mac osx
#   * ios: apple iOS using OpenGL ES
#   * linuxarm: embedded linux using OpenGL ES
TARGET = desktop

# where to place the generated bindings
DEST_DIR = $(LANG)

# any extra SWIG flags per-language, etc
SWIG_FLAGS = -O -small

##############
# main targets
##############

.PHONY: all clean desktop ios linuxarm header libs libs-clean \
openFrameworks openFrameorks-header openFrameorks-symbols openFrameworks-clean \
glm glm-symbols glm-clean

all: desktop header libs

clean: openFrameworks-clean libs-clean

# desktop OS generation
desktop: desktop-prepare openFrameworks

desktop-prepare:
	$(eval TARGET := desktop)
	$(eval CFLAGS := $(CFLAGS))

# iOS specific generation
ios: ios-prepare openFrameworks

ios-prepare:
	$(eval TARGET := ios)
	$(eval CFLAGS := $(CFLAGS) -DTARGET_OPENGLES -DTARGET_IOS)

# embedded linux specific generation
linuxarm: linuxarm-prepare openFrameworks

linuxarm-prepare:
	$(eval TARGET := linuxarm)
	$(eval CFLAGS := $(CFLAGS) -DTARGET_OPENGLES)

# runtime header generation
header: openFrameworks-header

# additional library generation
libs: glm

libs-clean: glm-clean

#######################
# openFrameworks module
#######################

# module name
MODULE_NAME = of

# strip "of" from function, class, & define/enum names?
RENAME = true

# allow deprecated functions? otherwise, ignore
DEPRECATED = false

# generated bindings filename
NAME = ofBindings

# OF libs header path
OF_HEADERS = -I../../../libs

# Python specific preferences
# typically, long names are used in Python,
# and function names remain unaltered (see pyOpenGL for instance)
ifeq ($(LANG), python)
	MODULE_NAME = openframeworks
	RENAME = false
	SWIG_FLAGS += -modern
endif

# populate CFLAGS for openFrameworks
OF_CFLAGS = $(OF_HEADERS)/openFrameworks -DMODULE_NAME=$(MODULE_NAME)

ifeq ($(RENAME), true)
	OF_CFLAGS += -DOF_SWIG_RENAME
endif

ifeq ($(DEPRECATED), true)
	OF_CFLAGS += -DOF_SWIG_DEPRECATED
endif

ifeq ($(ATTRIBUTES), true)
	OF_CFLAGS += -DOF_SWIG_ATTRIBUTES
endif

# generates bindings
openFrameworks:
	@echo "### Generating: openFrameworks $(TARGET)"
	@mkdir -p $(DEST_DIR)/$(TARGET)
	$(SWIG) -c++ -$(LANG) $(SWIG_FLAGS) $(CFLAGS) $(OF_CFLAGS) -o $(DEST_DIR)/$(TARGET)/$(NAME).cpp openFrameworks.i

# generates swig runtime header
openFrameworks-header:
	@echo "### Generating: openFrameworks header"
	$(SWIG) -c++ -$(LANG) -external-runtime $(DEST_DIR)/$(NAME).h

# outputs swig language symbols
openFrameworks-symbols:
	$(SWIG) -c++ -$(LANG) $(SWIG_FLAGS) -debug-lsymbols $(CFLAGS) $(OF_CFLAGS) openFrameworks.i > $(MODULE_NAME)_$(LANG)_symbols.txt
	rm -f *.cxx
	if [ $(LANG) = "python" ]; then rm -f *.py; fi

# clean dest dir
openFrameworks-clean:
	rm -rf $(DEST_DIR)/desktop
	rm -rf $(DEST_DIR)/ios
	rm -rf $(DEST_DIR)/linuxarm
	rm -f $(DEST_DIR)/$(NAME).h
	rm -f $(MODULE_NAME)_*_symbols.txt

############
# glm module
############

# generates glm bindings
glm:
	@echo "### Generating: glm"
	@mkdir -p $(DEST_DIR)
	$(SWIG) -c++ -$(LANG) $(SWIG_FLAGS) $(OF_HEADERS)/glm/include -o $(DEST_DIR)/glmBindings.cpp glm.i

# outputs swig language symbols
glm-symbols:
	$(SWIG) -c++ -$(LANG) $(SWIG_FLAGS) -debug-lsymbols $(OF_HEADERS)/glm/include glm.i > glm_$(LANG)_symbols.txt
	rm -f *.cxx
	if [ $(LANG) = "python" ]; then rm -f *.py; fi

# clean dest dir
glm-clean:
	rm -f $(DEST_DIR)/glmBindings.cpp
	rm -f glm_*_symbols.txt
