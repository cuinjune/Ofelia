ofxMidi
=======
<p align="center">
<img src="https://raw.github.com/danomatika/ofxMidi/master/res/midi_din.png"/>
</p>

MIDI input and output addon for openFrameworks

Copyright (c) [Dan Wilcox](danomatika.com) 2011-2020  
(original implementation by Chris O'Shea, Arturo Castro, Kyle McDonald)

BSD Simplified License.

For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "LICENSE.txt," in this distribution.

See Documentation on [Github](https://github.com/danomatika/ofxMidi) and the [openFrameworks Forum post](https://forum.openframeworks.cc/t/ofxmidi-updates/2435).

openFrameworks is a cross platform open source toolkit for creative coding in C++.

[http://www.openframeworks.cc](http://www.openframeworks.cc)

This project has been supported by the CMU [Frank-Ratchye STUDIO for Creative Inquiry](http://studioforcreativeinquiry.org), the DU [Emergent Digital Practices Program](https://www.du.edu/ahss/edp/), and my time at the [ZKM | Hertz-Lab](https://zkm.de/en/about-the-zkm/organisation/hertz-lab).

Description
-----------

ofxMidi provides [Music Instrument Digital Interface](http://en.wikipedia.org/wiki/Musical_Instrument_Digital_Interface) IO capability to an openFrameworks app

* ofxMidiIn: a single MIDI input port, derive from the ofxMidiListener class to receive messages
* ofxMidiMessage: a received MIDI message
* ofxMidiOut: a single MIDI output port, includes a stream << interface

This project utilizes [RtMidi](http://www.music.mcgill.ca/~gary/rtmidi/) for Mac, Windows, & Linux and [PGMidi](https://github.com/petegoodliffe/PGMidi) on iOS.

Installation
------------

To use ofxMidi, first you need to download and install openFrameworks. ofxMidi is currently developed against the current stable version of openFrameworks on github.

To get a copy of the repository you can download the source from [http://github.com/danomatika/ofxMidi/zipball/master](http://github.com/danomatika/ofxMidi/zipball/master) or, alternatively, you can use git clone:

    git clone git://github.com/danomatika/ofxMidi.git

The addon should sit in `openFrameworks/addons/ofxMidi/`.

### Which version to use?

The master branch of ofxMidi will work with the current stable version of openFrameworks and can be considered *relatively* stable.

Previous versions are tagged using [Semantic Versioning](http://semver.org) with the updates to newer versions of openFrameworks and MIDI libraries noted in the changelog, CHANGES.txt. You can select the tag in the Github "Current Branch" menu or clone and check it out using git.

If you want to use ofxMidi with a previous version of openFrameworks, checkout the corresponding version tag after cloning:

    git clone git://github.com/danomatika/ofxMidi.git
    cd ofxMidi
    git checkout 1.0.5

MIDI Routing
------------

### macOS

Checkout a useful app for MIDI port routing called [MIDI Patchbay](http://notahat.com/midi_patchbay).

### Linux

Check out the Alsa utility apps aconnect & aconnectgui as well as the qjackctl gui for MIDI port routing control.

### Windows

Windows does not come with a virtual MIDI routing system like Linux (ALSA) and macOS (CoreMIDI).

If you want to connect your ofxMidi app to other software (synths, DAWs, etc) check out [loopMIDI](http://www.tobias-erichsen.de/loopMIDI.html). Run the app and create a few virtual ports which you can then connect to within your software.

Running an Example Projects
---------------------------

The example projects are in the `ofxMidi/midiInputExample`, `ofxMidi/midiOutputExample`, & `ofxMidi/midiExampleIOS` folders.

Project files for the examples are not included so you will need to generate the project files for your operating system and development environment using the OF ProjectGenerator which is included with the openFrameworks distribution.

To (re)generate project files for an *existing* project:

* Click the "Import" button in the ProjectGenerator
* Navigate to the project's parent folder ie. "ofxMidi", select the base folder for the example project ie. "midiOutputExample", and click the Open button
* Click the "Update" button

If everything went ok, you should now be able to open the generated project and build/run the example.

### macOS

Open the Xcode project, select the project scheme, and hit "Run".

### Linux

Open the Code::Blocks .workspace and hit F9 to build. Optionally, you can build the example with the Makefile.

To built and run it on the terminal:

    make
    make run

### Windows

An example Visual Studio solution as well as a Codeblocks workspace are included.

Creating a New ofxMidi Project
------------------------------

_Note: These instructions are for manually creating a new project. You do not need to follow these steps if you use the ProjecGenerator app._

To develop your own project based on ofxMidi, simply copy an example project and rename it. You probably want to put it in your apps folder, for example, after copying:

    openFrameworks/addons/ofxMidi/midiExampleInput/ => openFrameworks/apps/myApps/midiExampleInput/

It must be 3 levels down in the openFrameworks folder structure.

Then rename the folder:

    openFrameworks/apps/myApps/myMidiProject/

#### Xcode

Rename the project in Xcode (do not rename the .xcodeproj file in Finder!):

* Xcode Menu->Project->Rename

#### Codeblocks (Win & Linux)

* Rename the \*.cbp and \*.workspace files
* Open the workspace and readd the renamed project file by dragging it onto the project tree (it will complain about the missing project you renamed)
* If you renamed the project *folder* make sure to set the project name to this folder name or C::B will not be able to run the binary:
  - Right click on project in the tree (not the workspace)
  - Properties...->Title

#### Visual Studio

* Rename the \*.sln, \*.vcxproj, & \*.vcxproj.filters files
* Open the solution and delete the old project from the projects tree
* Go to File->Add->Existing Projects/Solutions and select the \*.vcxproj file
* Right-click on the project in the projects tree and rename it

Adding ofxMidi to an Existing Project
-------------------------------------

_Note: These instructions are for manually adding ofxMidi to a project. You do not need to follow these steps if you use the ProjecGenerator app to regenerate your project files._

### Xcode

* Create a new group "ofxMidi" in the "addons" group
* Drag these directories from ofxMidi into this new group: `ofxMidi/src` & `ofxMidi/libs`
  - In the Add dialog: add to your current project target, uncheck "Copy items if needed" & select "Create groups"
  - If building for macOS, remove the src/ios & libs/pgmidi folder references
  - If building for iOS, remove the src/desktop & libs/rtmidi folder references
* Add the CoreMIDI framework to your project
  - Click on your project in the sidebar
  - Select the Summary tab
  - Click the + under Linked Frameworks & Libraries
  - Search for and select the CoreMIDI.framework from the list
* Add the following directories to your search path in your project's Project.xconfig file (see the Project.xconfig of the example project):
<pre>
../../../addons/ofxMidi/src
../../../addons/ofxMidi/libs/rtmidi
</pre>

### Linux Makefiles/CodeBlocks

Edit addons.make in your project folder and add the following line to the end of the file:
<pre>
ofxMidi
</pre>

### Win Codeblocks & Visual Studio

* Add the ofxMidi sources to the project tree
    ofxMidi/src
    ofxMidi/libs/rtmidi
  * Codeblocks: right click on the project in the project tree and select Add Files Recursively...
  * Visual Studio: drag the ofxMidi/src & ofxMidi/libs/rtmidi folder onto the project tree
* Add the following search paths:
<pre>
..\\..\\..\addons\ofxMidi\src
..\\..\\..\addons\ofxMidi\libs\rtmidi
</pre>
* Codeblocks
  - Right-click on the project in the project tree and select Build Options...
  - Select the project name in the tree, not release or debug
  - Search directories tab->Add
* Visual Studio
  - Right click on the project in the project tree and select Properties
  - Set the Configuration to All Configurations
  - Configuration Properties->C/C++->General->Additional Directories

KNOWN ISSUES
------------

### Help, app crashes when receiving MIDI messages

If you are sub-classing `ofxMidiListener` and receiving MIDI messages via the
`newMidiMessage()` callback function, there is a chance of segmentation faults
(and crashes) if you share the received messages between multiple threads (ie.
main GUI, OSC receiver, etc).

Depending upon the design of your application, you may need to place a mutex
object or shared lock around access to these resources shared between threads.

For example, in ofApp.h:
```cpp
class ofApp : public ofBaseApp, public ofxMidiListener {
...
ofxMidiIn midiIn;
std::vector<ofxMidiMessage> midiMessages; //< received messages
ofMutex midiMutex; //< MIDI message access mutex 
```

and ofApp.cpp:
```cpp
void draw() {
  // draw current messages, lock in case of incoming messages
  midiMutex.lock();
  // do something with midiMessages
  midiMutex.unlock();
}

void newMidiMessage(ofxMidiMessage& msg) {
    // lock and add new messages
    midiMutex.lock();
    midiMessages.push_back(msg);
    midiMutex.unlock();
}
```

This should stop multiple-thread access crashes, however may stutter incoming
message timing as adding new messages will have to wait until the current frame
is done. Another option is to use a lock-free design using a ring-buffer.

### Undefined symbols for architecture x86_64 on iOS

_Steps contributed by Zach Lee._

There is a bug with the OF ProjectGenerator in 0.10 and 0.11 which may result in
an iOS project using ofxMidi which will have build errors like the following:

~~~
Undefined symbols for architecture x86_64:
  "_glBufferSubData", referenced from:
      ofBufferObject::updateData(long, long, void const*) in libofxiOS_iphonesimulator_Debug.a(ofBufferObject.o)
  "_glBufferData", referenced from:
      ofBufferObject::setData(long, void const*, unsigned int) in libofxiOS_iphonesimulator_Debug.a(ofBufferObject.o)
  "_glDeleteBuffers", referenced from:
      ofBufferObject::Data::~Data() in libofxiOS_iphonesimulator_Debug.a(ofBufferObject.o)
  "_glUniformMatrix4fv", referenced from:
...
  "_OBJC_CLASS_$_EAGLContext", referenced from:
      objc-class-ref in libofxiOS_iphonesimulator_Debug.a(ES1Renderer.o)
      objc-class-ref in libofxiOS_iphonesimulator_Debug.a(ES2Renderer.o)
      objc-class-ref in libofxiOS_iphonesimulator_Debug.a(EAGLKView.o)
  "_glCheckFramebufferStatus", referenced from:
      -[ES2Renderer createFramebuffer:] in libofxiOS_iphonesimulator_Debug.a(ES2Renderer.o)
     (maybe you meant: _glCheckFramebufferStatusFunc)
  "_OBJC_CLASS_$_MIDINetworkSession", referenced from:
      objc-class-ref in PGMidi.o
  "_AVAudioSessionInterruptionTypeKey", referenced from:
      -[SoundStream handleInterruption:] in libofxiOS_iphonesimulator_Debug.a(SoundStream.o)
ld: symbol(s) not found for architecture x86_64
clang: error: linker command failed with exit code 1 (use -v to see invocation)
Showing first 200 notices only
Showing first 200 errors only
~~~

The fix is to remove a search path added by the PG which is causing the problem:

1. click on your project in the sidebar
2. select the project under TARGETS
3. select the Build Settings tab
4. scroll down to or find Framework Search Paths and remove the last entry:

![iOS Framework search paths](res/ios-framework-search-paths.png)

### Using static ofxMidi objects on Linux causes segmentation faults

Avoid creating static ofxMidiIn / ofxMidiOut objects on Linux as the compiler seems to set creation order so they are created *before* ALSA is ready. This leads to a confirmed segmentation fault on Ubuntu and probably all other flavors of Linux using ALSA. The midi apis on Windows and macOS do not share this problem.

Instead, create a static shared_ptr and initialize it later. For example, in .h:
```cpp
class MyClass {

    ...

    static std::shared_ptr<ofxMidiOut> s_midiOut;
}
```

and in .cpp:
```cpp
std::shared_ptr<ofxMidiOut> MyClass::s_midiOut;

...

// initialize somewhere else
void MyClass::setup() {
  if(s_midiOut == NULL) {
      s_midiOut = std::shared_ptr<ofxMidiOut>(new ofxMidiOut("ofxMidi Client"));
  }
}
```

### ofxMidi classes created in constructors don't seem to work

This is related to the issue above, in that the ofxMidi classes are being created too early in the app startup process and the back end MIDI library is not being set up correctly. The easiest & best solution is to call the ofxMidi class setup code as part of your ofApp's setup() function, whether there directly or within a subclass. This way you have direct control over when things are happening as opposed to within a constructor which may be called at an arbitrarily early point.

DEVELOPING
----------

You can help develop ofxMidi on GitHub: [https://github.com/danomatika/ofxMidi](https://github.com/danomatika/ofxMidi)

Create an account, clone or fork the repo, then request a push/merge. Please use the *develop* branch of updates and pull requests.

If you find any bugs or suggestions please log them to GitHub as well.

### Adding a Midi Back End

If you want to add a new midi back end (Android, Jack, etc), you'll need two classes derived from ofxBaseMidiIn & ofxBaseMidiOut.

Place your source files in a new folder named after your platform/library and add new include #ifdef flags to ofxMidiIn.h & ofxMidiIn.cpp.

Last, you'll need to add specific #ifdef flags to the static port info ofxMidiIn/Out functions (listPorts, getPortName, etc).

### Updating Midi Libraries

RtMidi & PGMidi can be updated by running the `update_rtmidi.sh` or `update_pgmidi.sh` shell scripts in the scripts folder.

For RtMidi, edit the version setting in the script header and run the script to download and place the RtMidi sources into `libs/rtmidi`.

PGMidi sources are placed in `libs/pgmidi`.

#### RtMidi.cpp include

Next, make sure to add the following include to `RtMidi.cpp` at around line 38 or there will be link errors:

    #include "ofxMidiConstants.h"
