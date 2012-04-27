#pragma once

#include "ofMain.h"
#include "ofxPathfinder.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	
	ofImage img;
	ofxPathfinder pathfinder;
};
