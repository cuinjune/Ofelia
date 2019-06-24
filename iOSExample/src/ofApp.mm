#include "ofApp.h"
#import <AVFoundation/AVFoundation.h>

//--------------------------------------------------------------
void ofApp::setup()
{
    // register touch events
    ofRegisterTouchEvents(this);
    
    // iOSAlerts will be sent to this
    ofxiOSAlerts.addListener(this);
    
    // basic settings
    const int numInChannels = 0; // number of audio input channels to use
    const int numOutChannels = 2; // number of audio output channels to use
    const float sampleRate = setAVSessionSampleRate(44100); // audio sample rate
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
void ofApp::exit()
{
    ofelia.exit();
    
    // clear resources
    ofelia.clear();
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
void ofApp::lostFocus()
{
    ofelia.lostFocus();
}

//--------------------------------------------------------------
void ofApp::gotFocus()
{
    ofelia.gotFocus();
}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning()
{
    ofelia.gotMemoryWarning();
}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation)
{
    ofelia.deviceOrientationChanged(newOrientation);
}

//--------------------------------------------------------------
void ofApp::launchedWithURL(std::string url)
{
    ofelia.launchedWithURL(url);
}

//--------------------------------------------------------------
// set the samplerate the Apple approved way since newer devices
// like the iPhone 6S only allow certain sample rates,
// the following code may not be needed once this functionality is
// incorporated into the ofxiOSSoundStream
// thanks to Seth aka cerupcat
float ofApp::setAVSessionSampleRate(float preferredSampleRate)
{
    NSError *audioSessionError = nil;
    AVAudioSession *session = [AVAudioSession sharedInstance];
    
    // disable active
    [session setActive:NO error:&audioSessionError];
    if (audioSessionError)
        NSLog(@"Error %ld, %@", (long)audioSessionError.code, audioSessionError.localizedDescription);
    
    // set category
    [session setCategory:AVAudioSessionCategoryPlayAndRecord withOptions:AVAudioSessionCategoryOptionAllowBluetooth|AVAudioSessionCategoryOptionMixWithOthers|AVAudioSessionCategoryOptionDefaultToSpeaker error:&audioSessionError];
    if (audioSessionError)
        NSLog(@"Error %ld, %@", (long)audioSessionError.code, audioSessionError.localizedDescription);
    
    // try to set the preferred sample rate
    [session setPreferredSampleRate:preferredSampleRate error:&audioSessionError];
    if (audioSessionError)
        NSLog(@"Error %ld, %@", (long)audioSessionError.code, audioSessionError.localizedDescription);
    
    // *** Activate the audio session before asking for the "current" values ***
    [session setActive:YES error:&audioSessionError];
    if (audioSessionError)
        NSLog(@"Error %ld, %@", (long)audioSessionError.code, audioSessionError.localizedDescription);

    ofLogNotice() << "AVSession samplerate: " << session.sampleRate << ", I/O buffer duration: " << session.IOBufferDuration;
    
    // our actual samplerate, might be differnt aka 48k on iPhone 6S
    return session.sampleRate;
}
