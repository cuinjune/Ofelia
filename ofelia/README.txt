## Description

ofelia is a Pure Data(aka Pd) external library written with openFrameworks for creating cross-platform multimedia applications.

The library consists of over 400 objects covering most features of openFrameworks core API, designed to ease the development of multimedia applications such as visual arts, musical apps and interactive games.

And thanks to the real-time nature of Pure Data, one can make changes and see the result immediately without having to compile.

The library includes the following features:

* interactive output window

* various getters and event listeners

* 2d/3d shapes drawing

* image and font loading

* camera, lighting, material

* framebuffer object

* various data types (vec3f, color..)

* various utilities to speed up development (new expr, counter..)

* bandlimited oscillators and resonant filters

The library is currently available to be used under macOS, Linux(64bit) and Windows. It will also be available on Raspberry Pi soon.

## Installation

* Make sure you have Pd installed on your desktop.
* Start Pd and go to `Help` -> `Find externals`, then search for `ofelia`
* Select the proper version of ofelia for your system to download and install.
* Try opening any Pd patch inside the downloaded `ofelia` directory to check if the library loads successfully.
* (Linux only) If the library fails to load, open the Terminal and run the following command to install dependencies.
  cd ofelia/scripts/distro_name
  sudo ./install_dependencies.sh

## Explore

* Try the example patches inside `ofelia/examples` directory.
* Open `ofelia/help-intro.pd` to see the list of built-in objects in ofelia.
* Open the help files to learn more about each object.
* Create something cool and share it with other people.
* Feel free to contact the author if you have any questions or suggestions.

## Upcoming features

* GLSL shader loader
* Video player
* Video grabber

## Author

Copyright: (c) 2018 Zack Lee: cuinjune@gmail.com
Website: https://github.com/cuinjune/ofxOfelia
License: GNU General Public License v3.0
For information on usage and redistribution, and for a DISCLAIMER OF ALL WARRANTIES, see the file, "LICENSE.txt," in this distribution.
