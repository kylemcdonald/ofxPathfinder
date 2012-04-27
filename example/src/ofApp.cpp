#include "ofApp.h"

void ofApp::setup() {
	ofSetVerticalSync(true);
	img.loadImage("path.png");
	pathfinder.setup(img);
}

void ofApp::update() {
	pathfinder.find(img.getWidth() / 2, img.getHeight() / 2, mouseX, mouseY);
}

void ofApp::draw() {
	ofBackground(0);
	
	ofSetColor(255);
	img.draw(0, 0);
	
	ofSetColor(ofColor::red);
	ofSetLineWidth(2);
	pathfinder.path.draw();
}
