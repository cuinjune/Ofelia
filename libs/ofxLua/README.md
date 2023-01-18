ofxLua
======
<p align="center">
<img src="https://raw.github.com/danomatika/ofxLua/master/res/logo.png"/>
</p>

A Lua instance addon

Copyright (c) [Dan Wilcox](http://danomatika.com) 2011-2020

BSD Simplified License.

For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "LICENSE.txt," in this distribution.

See <https://github.com/danomatika/ofxLua> and the [Openframeworks Forum post](http://forum.openframeworks.cc/t/ofxlua/6493) for documentation

_If you want to use Lua + openFrameworks without building your own app, check out **loaf**: [danomatika.com/code/loaf](http://danomatika.com/code/loaf)._

This project has been supported by the CMU [Frank-Ratchey STUDIO for Creative Inquiry](http://studioforcreativeinquiry.org), the DU [Emergent Digital Practices Program](https://www.du.edu/ahss/edp/), and my time at the [ZKM | Hertz-Lab](https://zkm.de/en/about-the-zkm/organisation/hertz-lab).

Description
-----------

ofxLua is an openFrameworks addon for running a Lua embedded scripting interpreter within an openFrameworks application. Using the SWIG (Simple Wrapper and Interface Generator) tool, C++ functions and classes can be bound to the Lua api allowing them to be called within a Lua script. This is useful in separating the upper level logic from the lower level application and is utilized in numerous video games and applications.

In addition, ofxLua provides bindings for the openFrameworks API.

[Lua](http://www.lua.org/) combines simple procedural syntax with powerful data description constructs based on associative arrays and extensible semantics. Lua is dynamically typed, runs by interpreting bytecode for a register-based virtual machine, and has automatic memory management with incremental garbage collection, making it ideal for configuration, scripting, and rapid prototyping.

[SWIG](http://www.swig.org) is a software development tool that connects programs written in C and C++ with a variety of high-level programming languages. It is used to generate the C++ bindings which wrap the openFrameworks API for Lua.

(Optional) [LuaJIT](http://luajit.org/luajit.html) is a Just-In-Time Compiler (JIT) for the Lua programming language. It implements the Lua API but is optimized for performance over the standard Lua distribution. It is recommended to use LuaJIT when speed is a concern and it is enabled on embedded Linux in `addon_config.mk` for this reason.

[openFrameworks](http://www.openframeworks.cc) is a cross platform open source toolkit for creative coding in C++.

### Why not Javascript?

Those coming from a Javascript background may ask "Why Lua? Why not Javascript? I like Javascript!"

Lua has a long history of use as a embedded scripting language and, as such, is both smaller & much easier to embed on pretty much every platform. The compiled language adds less than 500kb to your app binary versus many Mbs for Javascript or Python. Speed-wise, the base Lua interpreter is oftentimes faster than either language due to it's simplicity and using LuaJIT may add a speed increase of many orders of magnitude. As an embedded language, Lua makes it relatively easy to bind C/C++ functions & objects to it's scripting environment.

For these reasons, Lua has been used for game development for many years:

* [World of Warcraft scripting](http://wowwiki.wikia.com/wiki/Lua)
* [LuaCraft](http://luacraft.com) = Minecraft + Lua
* [love2d](https://love2d.org) game engine

Those coming from an embedded computing or game development background are probably familiar with Lua while those coming from design and/or web development are used to Javascript. In many ways, both languages share a number of similarities and the rest is due to simple syntax or design differences. When it comes down to it, no one language or environment is *better* than another, they just have different focuses and design backgrounds. Do not dismiss Lua because you are unfamiliar with it as not every nail needs the same hammer.

Lua is not scary, trust me :)

See:

* [About Lua](http://www.lua.org/about.html#why)
* [Lua wikipedia page](https://en.wikipedia.org/wiki/Lua_(programming_language))
* [Programming in Lua](http://www.lua.org/pil)
* [Learning Lua from JS](http://phrogz.net/lua/LearningLua_FromJS.html)

Build Requirements
------------------

To use ofxLua, first you need to download and install openFrameworks. The examples are developed against the latest release version of openFrameworks on <http://openframeworks.cc/download>.

[OF github repository](https://github.com/openframeworks/openFrameworks)

Currently, ofxLua is being developed on macOS and has been tested on macOS, iOS, & Linux. Android should work but has not been tested extensively.

Installation and Build
----------------------

Place ofxLua within a folder in the apps folder of the OF directory tree:

    openframeworks/addons/ofxLua

The easiest way to do this is via cloning with git:

    cd openframeworks/addons/
    git clone git://github.com/danomatika/ofxLua.git

You'll need to checkout the swig-openframeworks submodule as well using:

    cd ofxLua
    git submodule init
    git submodule update

### Which version to use?

The master branch of ofxLua will work with the current stable version of openFrameworks and can be considered *relatively* stable.

Previous versions are tagged using [Semantic Versioning](http://semver.org) with the updates to newer versions of openFrameworks and Lua noted in the changelog, CHANGES.txt. You can select the tag in the Github "Current Branch" menu or clone and check it out using git.

If you want to use ofxLua with a previous version of openFrameworks, find the tag corresponding to your OF version by looking at [the changelog](https://github.com/danomatika/ofxLua/blob/master/CHANGES.txt) or [releases](https://github.com/danomatika/ofxLua/releases). Note that the ofxLua tag and OF version do not match.

For example,

    git clone git://github.com/danomatika/ofxLua.git
    cd ofxLua
    git checkout 1.3.0

will checkout a version that's compatible with OF 0.11.0.

### Dependencies

For embedded Linux (arm, Raspberry Pi, etc), [LuaJIT](http://luajit.org/luajit.html) is used for better performance. Make sure you have the **luajit-5.1** development package installed.

### Visual Studio

If you run into compilation issues in Visual Studio (ie. "unresolved external symbol" errors with "\_lua_\*" library functions), you may need to tell the VS to build the Lua library using the C compiler instead of the C++ compiler:

In the Solution Explorer:

1. Select the Lua C files in addons/ofxLua/libs/lua, e.g. lapi.c + lauxlib.c ...
2. Right click -> choose Properties
3. Select C/C++ -> Advanced from left tree
4. Change Compile As -> Compile as C Code (/TC)

*If there is an alternate solution to this issue in the addons_config.mk, please let us know!*

### Android Studio

_Contributed by Zach Lee_

In Android Studio, the `addon_config.mk` exclude flags may not have an effect when using the OF ProjectGenerator app, and the following platform-specific bindings folders may need to be manually removed from the AS project:

* ofxLua/src/bindings/desktop
* ofxLua/src/bindings/linuxarm

Next, to make Android Studio read the C flags from the `addon_config.mk` file, add the following code below `cppFlags.addAll(addonCppFlags(abi, ofRoot()))` in `build.gradle` file:

~~~
CFlags.addAll(addonCppFlags(abi, ofRoot()))
~~~

After the fix, the ofxLua project should *hopefully* build and run on an Android device.

Running the Example Projects
----------------------------

The example projects are located in the `luaExample` & `luaExampleIOS` folders.

Project files for the examples are not included so you will need to generate the project files for your operating system and development environment using the OF ProjectGenerator which is included with the openFrameworks distribution.

To (re)generate project files for an *existing* project:

* Click the "Import" button in the ProjectGenerator
* Navigate to the project's parent folder ie. "ofxLua", select the base folder for the example project ie. "luaExample", and click the Open button
* Click the "Update" button

If everything went Ok, you should now be able to open the generated project and build/run the example.

### macOS

Open the Xcode project, select the "luaExample Debug" scheme, and hit "Run".

### Linux

Open the Code::Blocks .cbp and hit F9 to build. Optionally, you can build the example with the Makefile.

To build and run it on the terminal:

    make
    make run

How to Create a New ofxLua Project
----------------------------------

### ProjectGenerator

Simply select ofxLua from the available addons in the ProjectGenerator before generating a new project.

### Manual Method

To develop your own project based on ofxLua, simply copy an example project and rename it. You probably want to put it in your apps folder, for example, after copying:

    openFrameworks/addons/ofxLua/example/ => openFrameworks/apps/myApps/example/

It must be 3 levels down in the openframeworks folder structure.

Then after renaming:

    openFrameworks/apps/myApps/myLuaProject/

On Mac, rename the project in Xcode (do not rename the .xcodeproj file in Finder!): Long click on the project name in the project tree.

Adding ofxLua to an Existing Project
------------------------------------

### ProjectGenerator

Select ofxLua and other addons used by your project from the available addons in the ProjectGenerator, select the parent folder of your project, and set the *exact* name of the existing project in the text box. This will overwrite the existing project files with new ones that now include ofxLua.

*Note: you will lose any custom settings you've added manually to your project.*

### Manual Method

If you want to add ofxLua to another project, you need to make sure you add the following src files:

    openFrameworks/addons/ofxLua/src/ofxLua.h
    openFrameworks/addons/ofxLua/src/ofxLua.cpp
    openFrameworks/addons/ofxLua/src/ofxLuaFileWriter.h
    openFrameworks/addons/ofxLua/src/ofxLuaFileWriter.cpp

and optionally

    openFrameworks/addons/ofxLua/src/bindings/ofBindings.h
    openFrameworks/addons/ofxLua/src/bindings/YOURPLATFORM/ofBindings.cpp
    openFrameworks/addons/ofxLua/src/bindings/glmBindings.h

You also need to add the Lua library files in the libs directory:

    openFrameworks/addons/ofxLua/libs/lua

#### For Xcode:

* Right-click and create a new group "ofxLua"
* Drag ofxLua/src into "ofxLua"
* Drag ofxLua/libs into "ofxLua"
* Remove bindings files that do not match your platform aka remove `src/bindings/desktop` for iOS

On older macOS versions (pre 10.8), a header file which is included with the OS contains some macros which conflict with several lua macros. They can be renamed by setting this CFLAG:

    -D__ASSERT_MACROS_DEFINE_VERSIONS_WITHOUT_UNDERSCORES=0

OF API Bindings
---------------

SWIG generated bindings for the OF API can be found in `src/bindings`. Currently it covers *most* of the api while leaving out base classes. More specific documentation may come at a future date, but for now check the example scripts on usage.

### Basic documentation:

There is a main "of" module and functions, classes, constants, & enums are renamed:
	
* **function**: ofBackground -> of.background
* **class**: ofColor -> of.Color
* **constant**: OF_LOG_VERBOSE -> of.LOG_VERBOSE
* **enum**: ofShader::POSITION_ATTRIBUTE -> of.Shader.POSITION_ATTRIBUTE
* **`begin` & `end`**: these are lua keywords, so functions with this name are renamed in the following classes:
  * _ofFbo_: beginFbo() & endFbo()
  * _ofCamera_: beginCamera() & endCamera()
  * _ofMaterial_: beginMaterial() & endMaterial() 
  * _ofShader_: beginShader() & endShader()

Base classes, deprecations, variable arguments (...), ofThread, ofPtr, ofMutex, & ofScopedLock are ignored for now.

Functions that return a std::vector return a wrapped std::vector in Lua. As with Lua tables, indexes start at 1.

#### glm

As of OF 0.10.0, there is also a "glm" module for the glm types and math functions. Note that the OF math types **cannot be implicitly cast** to glm types in Lua as they are in C++, so you need to use special conversion functions:

    -- error!
    local v = of.Vec2f(100, 100)
    of.drawRectangle(v, 20, 20) -- needs a glm::vec2
    
    -- convert
    of.drawRectangle(v:vec2(), 20, 20) -- ofVec2f -> glm::vec2
    
    -- or use the ofVec2f attributes directly
    of.drawRectangle(v.x, v.y, 20, 20)

It looks as those ofVec\*, ofMatrix\*, and ofQuaternion may be deprecated in the future, so it's probably best to transition to using glm::vec\*, glm::mat\*, and glm::quat over time:

    -- using glm::vec2
    local v = glm.vec2(100, 100)
    of.drawRectangle(v, 20, 20)

See `swig/README.txt` for details.

#### Math & String

The basic string and math functions are provided by built-in Lua libraries:

* [math](http://lua-users.org/wiki/MathLibraryTutorial)
* [string](http://lua-users.org/wiki/StringLibraryTutorial)

Other standard Lua libraries are: [table](http://lua-users.org/wiki/TableLibraryTutorial), [io](http://lua-users.org/wiki/IoLibraryTutorial), and [os](http://lua-users.org/wiki/OsLibraryTutorial).

#### Comparing Key Values

OF uses integers for the key event values so comparisons can be made using character literals in C++:

    if(key == 'f') {
        ofToggleFullscreen();
    }
    else if(key == OF_KEY_UP) {
        ofLog() << "up pressed";
    }

Lua does not have character literals so 'f' is treated as a string. In order to do the same comparison, convert 'f' to a number using the string library string.byte function:

    if key == string.byte("f") then
        of.toggleFullscreen()
    elseif key == of.KEY_UP then
        print("up pressed")
    end

#### Working with Class Instances

Calling class member functions requires using a : character -> image:draw(20, 100, 100, 100)

Calling class member variables requires using a . character -> print(image.width) 

Mixing up : and . is probably the most common pitfall when coming from C++ to Lua:

    -- create instance of a class
    image = of.Image()
        
    -- this will cause an error
    image.load("helloworld.jpg") -- using . to call member function

    -- this will work
    image:load("helloworld.jpg") -- use a : instead

    -- access class instance properties with a .
    print("Image size: "..image.width.."x"..image.height)

#### Details

To see the detailed differences with the OF C++ API run the following:

    grep DIFF swig/openFrameworks.i
   
To see work to be done on the bindings run:

    grep TODO swig/openFrameworks.i

### Classes

Simple Lua class support is provided by the class() function from the [Lua Users wiki](http://lua-users.org/wiki/SimpleLuaClasses):

    -- class declaration
    MyClass = class()
    
    -- constructor & attributes
    function MyClass:__init(x, y)
       self.x = x
       self.y = y
       self.radius = 4
    end

    -- class function
    function MyClass:draw()
        of.drawEllipse(self.x, self.y, self.radius, self.radius)
    end

    -- create instance & access attribute
    myclass = MyClass(10, 10)
    myclass.x = 100

    -- calling a class function, note use of : for instance function instead of .
    myclass:draw()

This implementation allows for inheritance and usage is as follows:

    -- inherit first class and add an attribute
    OtherClass = class(MyClass)
    function OtherClass:__init(x, y, z)
        MyClass.__init(self, x, y) -- call super constructor
    	self.z = z
    end
    
    -- create instance of derived class & access attributes
    otherclass = OtherClass(10, 10, 5)
    otherclass.x = 100
    otherclass.z = 100

	-- calling a class function, note use of : for instance function instead of .
	otherclass:draw()

### Checking Types

Lua comes with the built-in `type()` function which returns a string denoting the argument's type: "nil", "number", "string", "function", "table", "userdata", etc. Calling `type()` on a wrapped object will return a "userdata" string, ie. `type(of.Mesh())`, as it is basically an object pointer to something Lua doesn't know about but is handled by the language bindings.

For more detailed type info for objects wrapped by SWIG, use the special `swig_type()` function which is not native to Lua, but added by SWIG:

    local mesh = of.Mesh()
    print(type(mesh))
    print(swig_type(mesh))

will print the following:

~~~
userdata
ofMesh_< ofDefaultVertexType,ofDefaultNormalType,ofDefaultColorType,ofDefaultTexCoordType > *|ofMesh *
~~~

Making Your Own Bindings
------------------------

### SWIG Interface

Create a SWIG interface file (\*.i) with includes headers for the functions and classes which you want to bind. You then run SWIG with this file to generate the \*.cpp wrapper.

It could be as simple as the following:.

*MyCode.h*, your custom code:

    #pragma once
    
	#include "ofMain.h"
    
    float myFunction(float aNumber) {
        return aNumber+1;
	}
    
    class MyCoolClass {
    	public:
    		
    		MyCoolClass() {
			    myFloat = 0;
				myInt = 0;
				aString = "hello world";
			}
    		MyCoolClass(float aNumber) {
			    myFloat = aNumber;
				myInt = (int) aNumber;
				aString = "foo bar";
			}
    		
    		// member function
    		void doSomething() {
				ofLog() << "did something: " << aString;
			}
    		
    		// getter/setter
    		void setInt(int newInt) {myInt = newInt;}
    		int getInt() {return myInt;}
    		
    		// member variable, read/write
    		string aString;

		protected:

            // some protected member variables, won't be wrapped
		    float myFloat;
			int myInt;
    };

*MyBindings.i*, your custom SWIG interface file:

   	%module my
    %{
        // include any needed headers here
		#include "MyCode.h"
    %}
   
    // include support for some common C++ STL types you might be using,
	// see http://swig.org/Doc3.0/Library.html#Library_stl_cpp_library
	%include <stl.i>
	%include <std_string.i>
	%include <std_vector.i>
	%include <std_map.i>

    // if you're using "string" instead of "std::string" (like the openFrameworks source code),
	// you'll need this so swig will know that a "string" can be considered a "std::string"
	typedef std::string string;

    // include custom code you want to be wrapped,
    // note the '%' instead of '#' as '%include' is a SWIG
    // command to wrap code in the given header
    %include "MyCode.h"

That's it, SWIG will handle the rest! Of course this is a simple example but there are lots more options for specific bindings settings such as generating properties from getters & setters, etc.

### Generate .cpp Wrapper

Now call SWIG to generate your .cpp wrapper:

    swig -c++ -lua -fcompact -fvirtual -I../../../libs/openFrameworks MyBindings.i
    
_Make sure to add search paths to headers used by your code (aka the "-I" line in the command above)._

If all went well, SWIG will have generated the `MyBindings_wrap.cxx` C++ file. Put this into your project's source directory and build it with your project.

### Opening Your Lua Library

You will need to open your new Lua library provided by the SWIG-generated .cpp file in order to use it in your lua state.

SWIG creates a "luaopen" C function using your module name which, in this case, will be "luaopen_my". This function needs to be defined in C++ in order to be used, so add it to the top of the .cpp file where you initialize your ofxLua object:

    // declare the module bindings
    extern "C" {
	    int luaopen_my(lua_State* L);
    }

Then call this function after initing ofxLua:

    lua.init(true);
	luaopen_my(lua); // open bindings

### Using Your Lua Library
    
If everything is working, you should be able to call your bindings in Lua using your new "my" module:
    
    aNumber = my.myFunction(3.45)
    
    -- class
    coolClass = my.MyCoolClass() -- constructor
    coolClass = my.MyCoolClass(1.23) -- constructor with float
    
    -- note: use ':' not '.' to call class instance member functions
    coolClass:doSomething()

	-- call the setter function
	coolClass:setInt(4)
    
    -- properties are accessed with a '.'
    coolClass.aString = "hello world"

See the SWIG interface file in `swig` and the [SWIG and Lua](http://swig.org/Doc1.3/Lua.html) documentation for more information. SWIG will handle most general cases for you, but there are plenty of details to get into if you want greater customization.

If you end up having lots of custom code to bind, it's recommended to create multiple SWIG interface files which are included into a single \*.i using the %include command. Do not create separate files with the same module name, only set the module in the main file as SWIG is designed for 1 module per main interface.

**Do not** open issues or bug reports if the problem is in writing your own bindings as this is all handled by SWIG. Be sure to search online for similar errors with "swig" as part of your search. More likely than not, it's an issue with your bindings and not with ofxLua.

## Debugging Symbols and Syntax

When writing bindings, it's often helpful to see what C++ symbols (classes, functions, etc) are being bound to Lua by SWIG. You have SWIG generate a list of language symbols it sees while processing using the `-debug-lsymbols` option. It's often helpful to pipe this into a file:

    swig -c++ -lua -fcompact -fvirtual -I../../../libs/openFrameworks -debug-lsymbols MyBindings.i > my_symbols.txt

This file can also be used as the basis for generating a list of keywords for autocompletion. For instance, the `scripts/lua_syntax.py` script parses the SWIG symbol output format into a simple list of all available symbols in lua module. The following creates a text file listing the global symbols as well as per class symbols found in the "my" module from the `my_symbol.txt` file generated by SWIG:

    ofxLua/scripts/lua_syntax.py my my_symbols.txt

This generates a `my_syntax.txt` which is easier to use than the base SWIG output.

Lua Modules
-----------

For platforms that support it, an ofxLua project can load Lua dynamic modules via Lua's `require` mechanism. For this to work, the module needs to be built with the same Lua version.

For info on building your own Lua modules, see `modules/README.md`.

Lua require 
-----------

Lua's `require` command will import scripts but doesn't know about the OF data path, so it will fail when using a relative path since the working directory for an OF app is the executable directory by default:

    YourApp/bin/data/scriptA.lua
    YourApp/bin/data/scriptB.lua

scriptA.lua:

    require "scriptB"
    
require will fail since the current working directory is *not* `YourApp/bin/data`.

The easiest fix for this is to change the current working directory of the app to the directory of the script which is calling require. Setting the changeDir argument to true when calling ofxLua::doScript() will change the directory for you:

    lua.doScript("scriptA.lua"); // doesn't change path, require will fail
    
    lua.doScript("scriptA.lua", true); // changes path to script's parent dir, require should work

This will not effect the OF data path.

Building with LuaJIT
--------------------

Building with LuaJIT instead of Lua simply requires ignoring the Lua sources in `libs/lua`, installing LuaJIT, and setting the correct compiler & linker flags. One issue to know is that LuaJIT nominally targets the Lua 5.1 API, so keep this in mind when writing scripts you may want to run in both Lua 5.1+ and LuaJIT. 

This is handled automatically in the for embedded Linux in the ofxLua addon_config.mk.

For macOS, install Luajit using [Homebrew](brew.sh):

    brew install luajit

Uncomment the commented osx lines in the ofxLua/addon_config.mk and (re)generate your project using the OF ProjecGenerator. Now build.

Developing
----------

You can help develop ofxLua on GitHub: <https://github.com/danomatika/ofxLua>

Create an account, clone or fork the repo, then request a push/merge.

If you find any bugs or suggestions please log them to GitHub as well.

Known Issues
------------

### ofxLuaBindings.cpp fails to compile on Linux ARM (RPI 3B+)

(Contributed by Steven Noreyko)

The single OF API ofxLuaBindings.cpp bindings file generated by SWIG is relatively large. Some embedded systems may not have enough memory to build it completely resulting in strange errors with the compiler always stopping on

     ofxLua/src/bindings/linuxarm/ofxLuaBindings.cpp

The fix may be to increase the swap size to complete to build. On the RPI 3B+,
first install dphys-swapfile

    sudo apt-get install dphys-swapfile

then edit `/etc/dphys-swapfile` and uncomment and set

    CONF_SWAPFILE=/var/swap, CONF_SWAPSIZE=1024 and CONF_MAXSWAP=2048

then save and reboot.

Another option is to cross-compile on another Linux system with more resources.

### 'system' is unavailable: not available on iOS

iOS essentially ignores the C `system()` function and newer versions now throw a compiler error if it is used (iOS 11+). Until a fix comes with Lua, the simplest solution is to comment it out. If you were using the Lua os.system() function before, it wasn't working anyway. 

In `libs/lua/loslib.c`, find the `os_execute()` function, comment out the call to `system()`, and set `stat` to -1:

    int stat = -1;//system(cmd);

### ld: -pie error on iOS

If you get the following error after generating an iOS app:

    ld: -pie can only be used when targeting iOS 4.2 or later

change the deployment target in your project settings under the General Tab to something higher than 4.2, say 7.0.
