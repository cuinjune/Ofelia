#pragma once

#include "ofxiOS.h"
#include "ofxOfelia.h"

class ofApp : public ofxiOSApp
{
public:
    void setup();
    void update();
    void draw();
    void exit();
    void touchDown(ofTouchEventArgs &e);
    void touchMoved(ofTouchEventArgs &e);
    void touchUp(ofTouchEventArgs &e);
    void touchDoubleTap(ofTouchEventArgs &e);
    void touchCancelled(ofTouchEventArgs &e);
    void lostFocus();
    void gotFocus();
    void gotMemoryWarning();
    void deviceOrientationChanged(int newOrientation);
    void launchedWithURL(std::string url);
    
    // sets the preferred sample rate, returns the *actual* samplerate
    // which may be different ie. iPhone 6S only wants 48k
    float setAVSessionSampleRate(float preferredSampleRate);
    
    // ofxOfelia
    ofxOfelia ofelia;
};


