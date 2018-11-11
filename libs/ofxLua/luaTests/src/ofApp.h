/*
 * Copyright (c) 2018 Dan Wilcox <danomatika@gmail.com>
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

		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);

		void errorReceived(std::string& msg);

		void runTests();
		void loadScript();
		
		ofxLua lua;
};
