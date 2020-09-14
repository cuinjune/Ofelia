# Ofelia
![alt text](doc/Logo.png)
![alt text](doc/Iteration.png)
![alt text](doc/GLSL.png)

## Description
Ofelia is a Pd external which allows you to use openFrameworks and Lua within a real-time visual programming environment for creating audiovisual artwork or multimedia applications such as games.

[openFrameworks](http://openframeworks.cc/) is an open source C++ toolkit for creative coding.<br />
[Lua](https://www.lua.org/) is a powerful, efficient, lightweight, easy-to-learn scripting language.<br />
[Pure Data](https://puredata.info/)(Pd) is a real-time visual programming language for multimedia.<br />

Thanks to Lua scripting feature, you can do text coding directly on a Pd patch or through a text editor which makes it easier to solve problems that are complicated to express in visual programming languages like Pd. And unlike compiled languages like C/C++, you can see the result immediately as you change code which enables faster workflow. Moreover, you can use openFrameworks [functions and classes](ofelia/classesAndGlobalFunctions.txt) within a Lua script.

Using Ofelia, you can flexibly choose between patching and coding style based on your preference.<br />

The external is available to be used under macOS, Windows, Linux and Raspbian.

## Installation
* Make sure you have [Pure Data](https://puredata.info/downloads/pure-data) installed on your desktop.
* Start Pd and go to `Help` -> `Find externals`, then search for `ofelia`
* Select the latest version of Ofelia to download and install.
* Try opening patches inside downloaded `ofelia/examples` directory.
* (Windows) If the library fails to load, open `vc_redist` executable file inside `ofelia` directory to install [Microsoft Visual C++ Redistributable packages](https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads). And if video examples don't work properly, try installing the [K-Lite Codec Pack](https://www.codecguide.com/download_kl.htm).
* (Linux / Raspbian) If the library fails to load, open the Terminal and run the following command to install dependencies.
  <pre>cd ofelia/scripts/distro_name
  sudo ./install_dependencies.sh</pre>

--------------

# ofxOfelia
![alt text](doc/ofxOfelia.png)

## Description

**ofxOfelia** is Ofelia packaged as an openFrameworks addon which enables you to create a standalone(macOS, Windows, Linux, Raspbian, iOS and Android) or web application that can run Pd patches made with Ofelia and Pd vanilla objects.

You can share the application with a wide range of audiences including mobile device, internet and non-Pd users.

## Build Requirements
In order to use ofxOfelia, you first need to download and install openFrameworks. Please follow the [setup guides](http://openframeworks.cc/download/) to download and install openFrameworks 0.10.1 which is the most recent stable release.

The following tools are used to build ofxOfelia project.

* macOS / iOS: [Xcode](https://developer.apple.com/xcode/).
* Linux / Raspbian: [Makefile](https://www.gnu.org/software/make/).
* Windows: [Visual Studio Community 2017](https://visualstudio.microsoft.com/vs/community/).
* Android: [Android Studio](https://developer.android.com/studio/).

## Setup guide
* Make sure you have openFrameworks setup on your desktop.
* Download [ofxOfelia_targetOS.zip](https://github.com/cuinjune/ofxOfelia/releases/latest) and rename the extracted folder to `ofxOfelia`.<br />
Place the folder within the directory `OF/addons`. ("OF" indicates the base directory of openFrameworks)
* (Linux 64bit) Open the Terminal and run the following to update openFrameworks.
  <pre>cd OF/addons/ofxOfelia/scripts/Linux64
  sudo ./updateOF.sh</pre>
  
## Building the external
* (macOS) Open `OF/addons/ofxOfelia/macOSExternal/ofelia.xcodeproj` with Xcode and build the project.
* (Windows) Open `OF/addons/ofxOfelia/WindowsExternal/ofelia.sln` with Visual Studio and build the solution.
* (Linux / Raspbian) Open the Terminal and run the following command to build the project.
  <pre>cd OF/addons/ofxOfelia/LinuxExternal
  make</pre>
* Copy the generated binaries from `OF/addons/ofxOfelia/(targetOS)/bin` into the Pd externals directory.

## Running the standalone example
* (macOS) Open `OF/addons/ofxOfelia/macOSExample/macOSExample.xcodeproj` with Xcode and run the project.
* (Windows) Open `OF/addons/ofxOfelia/WindowsExample/WindowsExample.sln` with Visual Studio and run the solution.
* (Linux / Raspbian) Open the Terminal and run the following command to run the project.
  <pre>cd OF/addons/ofxOfelia/LinuxExample
  make && make RunRelease</pre>
* (iOS) Open `OF/addons/ofxOfelia/iOSExample/iOSExample.xcodeproj` with Xcode and run the project.
* (Android) Open Android Studio, select `Open an existing Android Studio project` and open `OF/addons/ofxOfelia/AndroidExample` and run the project.

## Creating your own standalone project
* Copy an example project from `OF/addons/ofxOfelia` and paste it into `OF/apps/myApps` directory.
* Rename the project and folder to your liking.
* Replace Pd patches and data in `yourProject/bin/data/pd` with your files.
* Build the project and find the executable file in `yourProject/bin` directory.

## Running the Emscripten example in a web browser
* Download the latest [nightly build](https://openframeworks.cc/ci_server/versions/nightly/) of openFrameworks.
* Download [ofxOfelia_Emscripten.zip](https://github.com/cuinjune/ofxOfelia/releases/latest) and rename the extracted folder to `ofxOfelia`.<br />
Place the folder within the directory `OF/addons`. ("OF" indicates the base directory of openFrameworks)
* (macOS / Linux) Open the Terminal and run the following to update openFrameworks.
  <pre>cd OF/addons/ofxOfelia/scripts/Emscripten
  sudo ./updateOF.sh</pre>
* Install Emscripten following the [setup guide](https://openframeworks.cc/setup/emscripten/).
* Run the following command to run the project in your browser.
  <pre>cd OF/addons/ofxOfelia/EmscriptenExample
  emmake make
  emrun bin/EmscriptenExample.html</pre>

## Developing ofxOfelia
You can help develop ofxOfelia on GitHub: https://github.com/cuinjune/ofxOfelia<br />
Create an account, clone or fork the repo, then request a push/merge.<br />
Feel free to contact the [author](#author) if you have any questions or suggestions.

--------------

## Projects that used Ofelia
* [Bruno Rohde](http://esmeril.ufba.br/): [ESMERIL](https://youtu.be/DfyGt6fyGMg).
* [Mike Moreno](https://github.com/MikeMorenoAudio/): [Paths](https://youtu.be/Si_IbyIvPy4/).
* [Chris Niven: BILL JOBS CYBER RADIO](https://youtu.be/j3qptxxBoTE).
* [Martin Hiendl](http://martinhiendl.com/): [Ten Bullets Through One Hole](https://youtu.be/uzdXXMNWNMs).
* [Virtual Insight: World of Piano](https://www.worldofpiano.net/).
* [The Center for Haptic Audio Interaction Reseach: Instrument development](https://chair.audio/).

## Acknowledgements
* [Miller Puckette](http://msp.ucsd.edu/) and the [Pd community](http://puredata.info/community/) for developing and maintaining [Pd](http://puredata.info/);
* [Arturo Castro](http://arturocastro.net/), [Theodore Watson](http://www.theowatson.com/) and the [OF community](https://openframeworks.cc/community/) for developing and maintaining [openFrameworks](https://openframeworks.cc/);
* [Dan Wilcox](http://danomatika.com/) for developing [ofxLua](https://github.com/danomatika/ofxLua), [ofxPd](https://github.com/danomatika/ofxPd) and [ofxMidi](https://github.com/danomatika/ofxMidi) addons which Ofelia/ofxOfelia uses;
* [IOhannes M Zmölnig](https://puredata.info/author/zmoelnig) for developing [GEM](https://puredata.info/downloads/gem) which has greatly influenced Ofelia;
* [Daniel Shiffman](https://shiffman.net/) and the [Processing Foundation](https://processingfoundation.org/) for providing many [examples](https://processing.org/examples/) which Ofelia copied;
* [Henri Menke](https://www.henrimenke.com/) for answering so many questions about integrating Lua into C++;
* Many others who contributed to the development of Ofelia.

## Author
Copyright (c) 2018 Zack Lee: <cuinjune@gmail.com><br />
GNU General Public License v3.0<br />
For information on usage and redistribution, and for a DISCLAIMER OF ALL WARRANTIES, see the file, "LICENSE.txt," in this distribution.
