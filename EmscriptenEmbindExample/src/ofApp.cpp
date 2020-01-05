#include "ofApp.h"
#include <emscripten/bind.h>

float embind_bind_1;
float function_1(float a) 
{
   embind_bind_1 = a;
   return embind_bind_1;
}

float embind_bind_2;
float function_2(float a) 
{
   embind_bind_2 = a;
   return embind_bind_2;
}

float embind_bind_3;
float function_3(float a) 
{
   embind_bind_3 = a;
   return embind_bind_3;
}

EMSCRIPTEN_BINDINGS(my_module) {
 emscripten::function("function_1", &function_1);
 emscripten::function("function_2", &function_2);
 emscripten::function("function_3", &function_3);
}

//--------------------------------------------------------------
void ofApp::setup()
{
    // basic settings
    const int numInChannels = 0; // number of audio input channels to use
    const int numOutChannels = 2; // number of audio output channels to use
    const int sampleRate = 44100; // audio sample rate
#ifdef TARGET_EMSCRIPTEN
    const int ticksPerBuffer = 32; // used to compute the audio buffer len: tpb * blocksize (always 64)
#else
    const int ticksPerBuffer = 8; // used to compute the audio buffer len: tpb * blocksize (always 64)
#endif
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
void ofApp::exit()
{
    ofelia.exit();
    
    // clear resources
    ofelia.clear();
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
void ofApp::mouseMoved(ofMouseEventArgs &e)
{
    ofelia.mouseMoved(e);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(ofMouseEventArgs &e)
{
    ofelia.mouseDragged(e);
}

//--------------------------------------------------------------
void ofApp::mousePressed(ofMouseEventArgs &e)
{
    ofelia.mousePressed(e);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(ofMouseEventArgs &e)
{
    ofelia.mouseReleased(e);
}

//--------------------------------------------------------------
void ofApp::mouseEntered(ofMouseEventArgs &e)
{
    ofelia.mouseEntered(e);
}

//--------------------------------------------------------------
void ofApp::mouseExited(ofMouseEventArgs &e)
{
    ofelia.mouseExited(e);
}

//--------------------------------------------------------------
void ofApp::windowResized(ofResizeEventArgs &e)
{
    ofelia.windowResized(e);
}

//--------------------------------------------------------------
void ofApp::messageReceived(ofMessage &e)
{
    ofelia.messageReceived(e);
}

//--------------------------------------------------------------
void ofApp::dragged(ofDragInfo &e)
{
    ofelia.dragged(e);
}
