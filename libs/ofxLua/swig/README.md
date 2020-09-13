swig-openframeworks
-------------------

a SWIG interface for openFrameworks with included Makefile

Copyright (c) [Dan Wilcox](danomatika.com) 2015-2018

BSD Simplified License.

For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "LICENSE.txt," in this distribution.

See <https://github.com/danomatika/swig-openframeworks> for documentation

Description
-----------

swig-openframeworks provides a centralized OpenFrameworks API interface file for the Simple Wrapper and Interface Generator (SWIG) tool which can be used to generate C++ bindings for various scripting languages, including Lua, Python, and Javascript.

[SWIG](http://www.swig.org) is a software development tool that connects programs written in C and C++ with a variety of high-level programming languages.

Installation
------------

This repository is designed to be used as a submodule within OpenFrameworks addons which provide scripting access to OF.

The suggested installation is to the `swig` folder in your main repo:

    git submodule add https://github.com/danomatika/swig-openframeworks.git swig

Git users of your addon will then need to checkout this submodule when they clone your addon:

    git submodule init
    git submodule update

### Which version to use?

If you are using a stable version (0.8.4, 0.9.0, ...) of OpenFrameworks then you want to use a git tag of swig-openframeworks for that version. You can select the tag in the Github "Current Branch" menu or clone and check it out using git.

The master branch of swig-openframeworks will work with the current stable version.

OF API Bindings
---------------

Currently the swig interface file covers *most* of the api while leaving out base classes. It creates a module containing all the imported functions, classes, constants & enums.

To see the main differences with the OF C++ API run the following:

    find . -name "*.i" -exec grep "// DIFF" {} \;
     
To see work to be done on the bindings run:

    find . -name "*.i" -exec grep "// TODO" {} \;

To see attributes which can be added to various classes:

    find . -name "*.i" -exec grep "// ATTR" {} \;

Currently supported (aka tested) scripting languages are:

* Lua
* Python

Other language bindings supported by swig can be generated. Feel free to create a PR adding required updates to the Makefile and interface file.

### Lua

In Lua, all wrapped functions, classes, defines, and enums are added to an "of" module (aka table) like a Lua library.

The contents of the module are renamed by default:

* **function**: ofBackground -> of.background
* **class**: ofColor -> of.Color
* **constant**: OF_LOG_VERBOSE -> of.LOG_VERBOSE
* **enum**: ofShader::POSITION_ATTRIBUTE -> of.Shader.POSITION_ATTRIBUTE

### Python

In Python, the module (aka library) is called "openframeworks" and its members retain the "of" prefix:

    from openframeworks import *
    
    ofBackground(255)
    color = ofColor()
    ...

glm Bindings
------------

As of openFrameworks 0.10.0, the glm library math types (vec3, mat3, quat, etc) have become integral to the OF API and are now wrapped via a swig interface as well. The resulting module is named "glm" and associated functions and types are accessed from within this name space.

For example, in Lua:

    -- constructors
    local v1 = glm.vec3(1, 2, 3)
    local v2 = glm.vec3(4, 5, 6)
    
    -- operators
    local v3 = v1 + v2
    v3 = v2 / v1
    
    -- functions
    local dot = glm.dot(v1, v2)

One **important point**: Most scripting languages cannot support the casting operators which allow the OF math types (ofVec3f, ofMatrix3x3, etc) to be used with functions which take glm type arguments. To get around this problem, each math type has a special member function which returns the associated glm type:

* ofVec2f -> glm::vec2: vec2()
* ofVec3f -> glm::vec3: vec3()
* ofVec4f -> glm::vec4: vec4()
* ofQuaternion -> glm::quat: quat()
* ofMatrix3x3 -> glm::mat4: mat3()
* ofMatrix4x4 -> glm::mat4: mat4()

Essentially, the following will *not* work as it does in C++:

    -- error!
    local v = of.Vec2f(100, 100)
    of.drawRectangle(v, 20, 20) -- needs a glm.vec2

Either convert the OF math type to a glm type or use a glm type directly:

    -- convert
    local v = of.Vec2f(100, 100)
    of.drawRectangle(v.vec2(), 20, 20)
    
    -- use glm::vec2
    local v = glm.vec2(100, 100)
    of.drawRectangle(v, 20, 20)

Usage
-----

A Makefile is provided to handle SWIG and platform specific details. It generates both a .cpp binding implementation as well as a .h header for SWIG helper functions.

Basic usage is:

    make

which generates Lua bindings for desktop OSs and places them in `../src/bindings/desktop`.	

### Languages

Available scripting languages are listed by the SWIG help: `swig -h`. The language can be set by the LANG makefile variable and is `lua` by default.

Example: To generate python bindings for desktop:

    make LANG=python

### Platforms

The Makefile current supports generating bindings for the following target platforms:

* **desktop**: win, linux, & mac osx
* **ios**: apple iOS using OpenGL ES
* **linuxarm**: embedded linux using OpenGL ES

Generated bindings are placed within platform subfolders in the destination directory.

Example: To generate python bindings for iOS:

    make ios LANG=python

### Destination Dir

The destination directory where the bindings are installed can be set by the DEST_DIR makefile variable and is `../src/bindings` by default.

Example: To generate python bindings for desktop and place them in a nearby test dir:

    make desktop LANG=python DEST_DIR=test

### Bindings Filename

The Makefile generates bindings files using the NAME makefile variable and is `openFrameworks_wrap` by default.

Example: To generate python bindings for desktop with the name "ofxPythonBindings":

    make desktop LANG=python NAME=ofxPythonBindings

### Module Name

The scripting language bindings use the "of" module by default. In the case of Lua, this refers to the parent "of" table that contains all warped functions, classes, and defines. This may not be desirable for particular scripting languages (ie. Python), so the module name can be set using the MODULE_NAME makefile variable.

Example: To generate python bindings with the module name "openframeworks":

    make LANG=python MODULE_NAME=openframeworks

### Renaming

By default, functions, classes, enums, & defines are reamed to strip the "of" prefix. This may not be desirable for particular scripting languages (ie. Python) and can be disabled by setting the MODULE_NAME makefile variable to false:

    make RENAME=false

### Deprecated Functions & Classes

By default, functions & classes marked as deprecated in the OF api are ignored when generating bindings. If you want to allow deprecations, set the DEPRECATED makefile variable to true:

    make DEPRECATED=true

### Attributes

Many target scripting languages support attributes using getters/setters, ie:

    -- lua
    image = of.Image("hello.png")
    print("width: "..image:getWidth())
    print("width: "..image.width) -- same as above using a getter attribute

To enable attribute bindings, set the ATTRIBUTES makefile variable to true:

    make ATTRIBUTES=true

### SWIG Flags

SWIG has a large number of flags for customizing bindings generation, many of which are specific to each generated language. The Makefile uses the SWIG_FLAGS makefile variable for these extra options, for instance generating Python bindings uses the `-modern` option to focus on newer versions of Python.

### Scripting

Generating bindings for all platform targets can be done easily using a script which calls the Makefile. See [generate_bindings.sh](https://github.com/danomatika/ofxLua/blob/swig/scripts/generate_bindings.sh) in ofxLua for an example.

### Debugging

Debugging scripting language bindings can be a pain, so SWIG can output the binding symbols for the target language. The Makefile provides a target that creates a `of_LANG_symbols.txt` so you can see which classes, functions, & enums are being wrapped and how.

Example: To debug python bindings:

    make symbols LANG=python

generates a file called `of_python_symbols.txt`.

Example
-------

swig-openframeworks was originally developed for ofxLua which is using it as a submodule in the "swig" branch: <https://github.com/danomatika/ofxLua/tree/swig>

Developing
----------

You can help develop swig-openframeworks on GitHub: <https://github.com/danomatika/swig-openframeworks>

Create an account, clone or fork the repo, then request a push/merge.

If you find any bugs or suggestions please log them to GitHub as well.
