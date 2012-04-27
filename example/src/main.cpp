#include "ofApp.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 300, 300, OF_WINDOW);
	ofRunApp(new ofApp());
}
