#pragma once

#include "ofMain.h"
#include "GameThread.h"
#ifdef PI
#include "wiringPi.h"
#endif

class ofxEtherdream;

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		void exit();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		ofxEtherdream* ED;
		
		GameThread* Game;
		ofxIlda::Frame frame;

#ifdef PI
		bool lastRestart = false;
		int restartPin = 15;
#endif

		int framePerUpdate = 2;
		int currentFrame = 0;
};
