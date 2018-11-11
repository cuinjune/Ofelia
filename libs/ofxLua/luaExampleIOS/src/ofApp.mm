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
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	// register touch events
	ofRegisterTouchEvents(this);
	
	// iOSAlerts will be sent to this.
	ofxiOSAlerts.addListener(this);
	
	// landscape oreintation
	ofSetOrientation(OF_ORIENTATION_90_RIGHT);

	ofSetVerticalSync(true);
	ofSetLogLevel("ofxLua", OF_LOG_VERBOSE);
		
	// scripts to run
	scripts.push_back("scripts/touchExample.lua");
	scripts.push_back("scripts/graphicsExample.lua");
	scripts.push_back("scripts/imageLoaderExample.lua");
	scripts.push_back("scripts/polygonExample.lua");
	scripts.push_back("scripts/fontsExample.lua");
	currentScript = 0;
	doubleTapTimestamp = 0;
	
	// init the lua state
	lua.init(true); // true to stop on script error
	
	// listen to error events
	lua.addListener(this);
	
	// run a script
	lua.doScript(scripts[currentScript]);
	
	// call the script's setup() function
	lua.scriptSetup();
}

//--------------------------------------------------------------
void ofApp::update() {
	// call the script's update() function
	lua.scriptUpdate();
}

//--------------------------------------------------------------
void ofApp::draw() {
	// call the script's draw() function
	lua.scriptDraw();
	
	ofSetColor(0);
	ofDrawBitmapString("double tap to change between scripts", 10, ofGetHeight()-22);
	ofDrawBitmapString(scripts[currentScript], 10, ofGetHeight()-10);
}

//--------------------------------------------------------------
void ofApp::exit() {
	// call the script's exit() function
	lua.scriptExit();
	
	// clear the lua state
	lua.clear();
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
	lua.scriptMouseMoved(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	lua.scriptMouseDragged(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	lua.scriptMousePressed(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	lua.scriptMouseReleased(x, y, button);
}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs &touch) {
	lua.scriptMousePressed(touch.x, touch.y, OF_MOUSE_BUTTON_LEFT); // dummy
	lua.scriptTouchDown(touch);
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs &touch) {
	lua.scriptTouchMoved(touch);
}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs &touch) {
	lua.scriptTouchUp(touch);
}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs &touch) {
	// trigger debounce
	if(ofGetElapsedTimeMillis() - doubleTapTimestamp > 1000) {
		nextScript();
		doubleTapTimestamp = ofGetElapsedTimeMillis();
	}
	else {
		// the script will probably never get this,
		// it's here more for completeness
		lua.scriptTouchDoubleTap(touch);
	}
}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs& touch) {
	lua.scriptTouchCancelled(touch);
}

//--------------------------------------------------------------
void ofApp::lostFocus() {

}

//--------------------------------------------------------------
void ofApp::gotFocus() {

}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning() {

}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation) {

}

//--------------------------------------------------------------
void ofApp::errorReceived(string& msg) {
	ofLogNotice() << "got a script error: " << msg;
}

//--------------------------------------------------------------
void ofApp::reloadScript() {
	// exit, reinit the lua state, and reload the current script
	lua.scriptExit();
	lua.init(true);
	lua.doScript(scripts[currentScript]);
	lua.scriptSetup();
}

void ofApp::nextScript() {
	currentScript++;
	if(currentScript > scripts.size()-1) {
		currentScript = 0;
	}
	ofLogNotice() << "current script now " << currentScript;
	reloadScript();
}

void ofApp::prevScript() {
	currentScript--;
	if(currentScript < 0) {
		currentScript = scripts.size()-1;
	}
	reloadScript();
}
