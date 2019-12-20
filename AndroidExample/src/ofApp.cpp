#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
    // register touch events
    ofRegisterTouchEvents(this);

    // basic settings
    const int numInChannels = 0; // number of audio input channels to use
    const int numOutChannels = 2; // number of audio output channels to use
    const float sampleRate = 44100; // audio sample rate
    const int ticksPerBuffer = 8; // used to compute the audio buffer len: tpb * blocksize (always 64)
    const bool bOpenMidiInPort = false; // whether to open midi input port in init()
    const bool bOpenMidiOutPort = false; // whether to open midi output port in init()
    const int midiInPortNum = 0; // midi input port number to open
    const int midiOutPortNum = 0; // midi output port number to open
    const bool bOpenPatch = true; // whether to open a patch in init()
    const string &patchName = "pd/main.pd"; // path of the patch to open (relative to "project/bin/data")

    // init ofelia
    if (!ofelia.init(numInChannels, numOutChannels,
                     sampleRate, ticksPerBuffer,
                     bOpenMidiInPort, bOpenMidiOutPort,
                     midiInPortNum, midiOutPortNum,
                     bOpenPatch, patchName))
    {
        OF_EXIT_APP(1);
    }
    ofelia.setup();
}

//--------------------------------------------------------------
void ofApp::update()
{
    ofelia.update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofelia.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(ofKeyEventArgs &e)
{
	ofelia.keyPressed(e);
}

//--------------------------------------------------------------
void ofApp::keyReleased(ofKeyEventArgs &e)
{
    ofelia.keyReleased(e);
}

//--------------------------------------------------------------
void ofApp::windowResized(ofResizeEventArgs &e)
{
    ofelia.windowResized(e);
}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs &e)
{
    ofelia.touchDown(e);
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs &e)
{
    ofelia.touchMoved(e);
}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs &e)
{
    ofelia.touchUp(e);
}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs &e)
{
    ofelia.touchDoubleTap(e);
}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs &e)
{
    ofelia.touchCancelled(e);
}

//--------------------------------------------------------------
void ofApp::swipe(ofxAndroidSwipeDir swipeDir, int id)
{
    ofelia.swipe(std::make_pair(static_cast<int>(swipeDir), id));
}

//--------------------------------------------------------------
void ofApp::pause()
{
    ofelia.pause();
}

//--------------------------------------------------------------
void ofApp::stop()
{
    ofelia.stop();
}

//--------------------------------------------------------------
void ofApp::resume()
{
    ofelia.resume();
}

//--------------------------------------------------------------
void ofApp::reloadTextures()
{
    ofelia.reloadTextures();
}

//--------------------------------------------------------------
bool ofApp::backPressed()
{
	return ofelia.backPressed();
}

//--------------------------------------------------------------
void ofApp::okPressed()
{
    ofelia.okPressed();
}

//--------------------------------------------------------------
void ofApp::cancelPressed()
{
    ofelia.cancelPressed();
}
