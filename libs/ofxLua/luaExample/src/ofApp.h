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
#include "ofxLua.h"

class ofApp : public ofBaseApp, ofxLuaListener {

	public:

		// main
		void setup();
		void update();
		void draw();
		void exit();
		
		// input
		void keyPressed(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		
		// ofxLua error callback
		void errorReceived(std::string& msg);
		
		// script control
		void reloadScript();
		void nextScript();
		void prevScript();
		
		ofxLua lua;
		vector<string> scripts;
		size_t currentScript;
};
