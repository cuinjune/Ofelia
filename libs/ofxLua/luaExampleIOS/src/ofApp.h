/*
 * Copyright (c) 2012 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxLua for documentation
 *
 */
#pragma once

#include "ofMain.h"
#include "ofxiOS.h"
#include "ofxiOSExtras.h"

#include "ofxLua.h"

// this example app is designed for an iPad in landscape mode,
// in the project settings Deployment Info set the following:
//   * Devices: iPad
//   * Device Orientation: disable Portrait and Upside Down
class ofApp : public ofxiOSApp, ofxLuaListener {

	public:

		// main
		void setup();
		void update();
		void draw();
        void exit();
		
		// input
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		
		void touchDown(ofTouchEventArgs &touch);
		void touchMoved(ofTouchEventArgs &touch);
		void touchUp(ofTouchEventArgs &touch);
		void touchDoubleTap(ofTouchEventArgs &touch);
		void touchCancelled(ofTouchEventArgs &touch);

		void lostFocus();
		void gotFocus();
		void gotMemoryWarning();
		void deviceOrientationChanged(int newOrientation);
		
		// ofxLua error callback
		void errorReceived(string& msg);
		
		// script control
		void reloadScript();
		void nextScript();
		void prevScript();
		
		ofxLua lua;
		vector<string> scripts;
		int currentScript;
		long doubleTapTimestamp;
};
