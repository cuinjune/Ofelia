# ofelia
![alt text](doc/screenshot1.png)
![alt text](doc/screenshot2.png)

## Description
**ofelia** is an external library which allows you to use openFrameworks and Lua within Pure Data to create cross-platform multimedia applications.

[openFrameworks](http://openframeworks.cc/)(aka OF) is an open source C++ toolkit for creative coding.<br />
[Lua](https://www.lua.org/) is a powerful, efficient, lightweight, embeddable scripting language.<br />
[Pure Data](https://puredata.info/)(aka Pd) is a real-time visual programming language for multimedia.<br />

Thanks to the Lua scripting feature, you can do text coding directly on a Pd patch or through a text editor which makes it easier to solve problems that are complicated to express in visual programming languages like Pd. And unlike compiled languages like C/C++, you can see the result immediately as you change code which enables faster workflow.

Using ofelia, you can flexibly choose between patching and coding style depending on the situation.<br />
For patching style, ofelia currently provides over 300 abstactions which cover the following features of openFrameworks.

* [ofAppRunner](https://openframeworks.cc/documentation/application/ofAppRunner/): contains functions for getting and setting the properties of the window.
* [ofBaseApp](https://openframeworks.cc/documentation/application/ofBaseApp/): contains callback functions such as setup, update, draw and keyPressed.
* [ofGraphics](https://openframeworks.cc/documentation/graphics/ofGraphics/): has several functions to change the state of the graphics pipeline and allows to quickly draw 2d shapes.
* [ofImage](https://openframeworks.cc/documentation/graphics/ofImage/): allows to load, save and draw images and do basic transformations on them.
* [ofTrueTypeFont](https://openframeworks.cc/documentation/graphics/ofTrueTypeFont/): can load true type fonts in different formats and draw text using the loaded font.
* [of3dGraphics](https://openframeworks.cc/documentation/3d/of3dGraphics/): contains some functions to quickly draw the most used 3d shapes like spheres, cones and boxes.
* [of3dUtils](https://openframeworks.cc/documentation/3d/of3dUtils/): allows to draw 3d utililty objects such as arrows, axis and grid.
* [ofMath](https://openframeworks.cc/documentation/math/ofMath/): contains functions for various mathematical operations like calculating the distance between two points.
* [ofUtils](https://openframeworks.cc/documentation/utils/ofUtils/): contains different functions for varied purposes, from string manipulation to time functions.
* [ofSystemUtils](https://openframeworks.cc/documentation/utils/ofSystemUtils/): contains functions for creating message and file opening and saving dialogs.

The abstractions use the same function names, arguments and return types as the original openFrameworks API so users can easily learn and transfer from one to another when needed.

There is only one slight difference in function and class names. For exmaple,

* `ofDrawCircle` in openFrameworks becomes `of.drawCircle` in ofelia.
* `ofImage` in openFrameworks becomes `of.Image` in ofelia.

The help files for the abstractions currently don't exist but you can learn about them from one of the openFrameworks documentation pages above or you can simply google the name of the function to find the page.<br /> 
(e.g. google `ofDrawCircle` to learn about `of.drawCircle` abstraction in ofelia)

Although more abstractions will be added later, you can already use all openFrameworks features using [ofelia] object which allows you to code Lua and [OF API bindings](https://github.com/danomatika/ofxLua#of-api-bindings) on a Pd patch or create such abstractions yourself. See the object help file to learn how to use it.

The library is available to be used under macOS, Windows, Linux and Raspberry Pi.

## Installation
* Make sure you have [Pure Data](https://puredata.info/downloads/pure-data) installed on your desktop.
* Start Pd and go to `Help` -> `Find externals`, then search for `ofelia`
* Select the proper version of ofelia for your system to download and install.
* Try opening patches inside the downloaded `ofelia/examples` directory.
* (Windows) If the library fails to load, open `vc_redist` executable file inside `ofelia` directory to install [Microsoft Visual C++ Redistributable packages](https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads).
* (Linux, Raspberry Pi) If the library fails to load, open the Terminal and run the following command to install dependencies.
  <pre>cd ofelia/scripts/distro_name
  sudo ./install_dependencies.sh</pre>
  
## Contributing to ofelia
You can contribute to ofelia by reporting bugs, creating the missing `of` abstractions or by sharing your own custom abstractions. Please open an [issue](https://github.com/cuinjune/ofxOfelia/issues) or contact the [author](#author) if you have any questions or suggestions.
  
# ofxOfelia
![alt text](doc/standalone.png)

## Description

**ofxOfelia** is ofelia packaged as an openFrameworks addon which enables you to create a standalone application for macOS, Windows, Linux, Raspberry Pi, iOS and Android that can run Pd patches made with ofelia and Pd vanilla objects.

You can share the application with a wide range of audiences including non-Pd users and mobile device users.

## Build Requirements
In order to use ofxOfelia, you first need to download and install openFrameworks. Please follow the [setup guides](http://openframeworks.cc/download/) to download and install openFrameworks. The following tools are used to build ofxOfelia project.

* macOS / iOS : [Xcode](https://developer.apple.com/xcode/).
* Linux / Raspberry Pi : [Makefile](https://www.gnu.org/software/make/).
* Windows : [Visual Studio Community 2017](https://visualstudio.microsoft.com/vs/community/).
* Android : [Android Studio](https://developer.android.com/studio/).

## Setup guide
The setup guide and examples for creating standalone project will be ready soon.

## Developing ofxOfelia
You can help developing ofxOfelia on GitHub: https://github.com/cuinjune/ofxOfelia<br />
Create an account, clone or fork the repo, then request a push/merge.<br />
Feel free to contact the [author](#author) if you have any questions or suggestions.

--------------

## Projects that used ofelia/ofxOfelia
* [Bruno Rohde](esmeril.ufba.br): [ESMERIL](https://youtu.be/MOz3MCIBDzM).
* [Virual Insight: World of Piano](https://gust.com/companies/virtual-insight).
* [The Center for Haptic Audio Interaction Reseach: instrument development](https://chair.audio/).
* [Martin Hiendl](http://martinhiendl.com/): [Ten Bullets Through One Hole](http://martinhiendl.com/?section=ten-bullets-through-one-hole).
* [POLR Technologies](https://www.facebook.com/polrtech/): [LIQUID GRID](https://youtu.be/L5zdNc3NvRg).

## Acknowledgments
I would like to thank [Miller Puckette](http://msp.ucsd.edu/) and the [Pd community](http://puredata.info/community) for developing and maintaining [Pd](http://puredata.info/), [Dan Wilcox](http://danomatika.com/) for developing [ofxLua](https://github.com/danomatika/ofxLua), [ofxPd](https://github.com/danomatika/ofxPd) and [ofxMidi](https://github.com/danomatika/ofxMidi) which ofelia/ofxOfelia uses, [IOhannes M Zmölnig](https://puredata.info/author/zmoelnig) for developing [GEM](https://puredata.info/downloads/gem/) which has greatly influenced ofelia, [Arturo Castro](http://arturocastro.net/), [Theodore Watson](http://www.theowatson.com/) and the [OF community](https://openframeworks.cc/community/) for developing and maintaining [openFrameworks](https://openframeworks.cc/) and answering so many questions, [Henri Menke](https://www.henrimenke.com/) for consulting on Lua design matters, and so many others
who contributed to the development of ofelia.

## Author
Copyright (c) 2018 Zack Lee: <cuinjune@gmail.com><br />
GNU General Public License v3.0<br />
For information on usage and redistribution, and for a DISCLAIMER OF ALL WARRANTIES, see the file, "LICENSE.txt," in this distribution.

