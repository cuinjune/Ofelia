#pragma once

#include "ofMain.h"
#include "ofxOfelia.h"

class ofApp : public ofBaseApp
{
public:
    void setup();
    void update();
    void draw();
    void exit();
    void keyPressed(ofKeyEventArgs &e);
    void keyReleased(ofKeyEventArgs &e);
    void mouseMoved(ofMouseEventArgs &e);
    void mouseDragged(ofMouseEventArgs &e);
    void mousePressed(ofMouseEventArgs &e);
    void mouseReleased(ofMouseEventArgs &e);
    void mouseEntered(ofMouseEventArgs &e);
    void mouseExited(ofMouseEventArgs &e);
    void windowResized(ofResizeEventArgs &e);
    void messageReceived(ofMessage &e);
    void dragged(ofDragInfo &e);
    
    // ofxOfelia
    ofxOfelia ofelia;
};
