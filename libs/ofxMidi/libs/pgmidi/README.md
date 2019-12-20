PGMidi
======

CoreMidi made simple on iOS.

What is PGMidi?
---------------

PGMidi is a simple library for access to MIDI devices presented via the CoreMidi framework on iOS. It comes with an example project to illustrate how to use the library in your own iOS application.

It has become the de-facto iOS API for simple MIDI access, incorporated into many of the popular MIDI applications for iOS. Thanks to everyone who has used it and provided feedback.

It doesn't illustrate the entire CoreMidi API, but will give you a suitable head start into it. (I couldn't find any good examples myself when I needed some.)

The PGMidi classes are pretty full featured and easy to incorporate into your own project. Many MIDI apps have just pulled in these classes verbatim.

If you find this useful, please do let me know!

The demo application presents a message when MIDI devices are attached and detached, and prints the first three bytes of any blocks of MIDI input received.

It works on iPhones or iPads, but will only do something useful if the device is running iOS 4.2 or higher (CoreMidi is not available in earlier iOS versions).

The latest version of the project supports ARC and non-ARC projects from the same source.


What to look at
---------------

Look inside the Sources/PGMidi directory. There is a neat standalone class called `PGMidi` that does all the heavy lifting. It has a delegate interface through which it reports events.

In the PGMidi directory is also a header that will help you selectively enable MIDI functionality in your application.

Remember to link against CoreMidi weakly if you want to support devices running iOS version earlier than 4.2.


License
-------

Feel free to incorporate this code in your own applications.

I'd appreciate hearing from you if you do so. It's nice to know that I've been helpful. Attribution is welcomed, but not required.

Copyright (c) 2010-2015 Pete Goodliffe. All rights reserved.
