#pragma once

#include "ofMain.h"
#include "ofxAndroid.h"
#include "ofxOfelia.h"

class ofApp : public ofxAndroidApp
{
public:
    void setup();
	void update();
	void draw();
	void keyPressed(ofKeyEventArgs &e);
	void keyReleased(ofKeyEventArgs &e);
	void windowResized(ofResizeEventArgs &e);
	void touchDown(ofTouchEventArgs &e);
	void touchMoved(ofTouchEventArgs &e);
	void touchUp(ofTouchEventArgs &e);
	void touchDoubleTap(ofTouchEventArgs &e);
	void touchCancelled(ofTouchEventArgs &e);
	void swipe(ofxAndroidSwipeDir swipeDir, int id);
	void pause();
	void stop();
	void resume();
	void reloadTextures();
	bool backPressed();
	void okPressed();
	void cancelPressed();

	// ofxOfelia
    ofxOfelia ofelia;
};
