#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
#ifdef LASER
	ED = new ofxEtherdream();
	ED->setPPS(40000);
	Game = new GameThread(ED);
#else
	Game = new GameThread(nullptr);
#endif //LASER == 1
#ifdef PI
	WiringPi::pinMode(restartPin, WiringPi::INPUT);
#endif
}

//--------------------------------------------------------------
void ofApp::update(){
#ifdef PI
	bool restart = (WiringPi::digitalRead(restartPin) == WiringPi::HIGH);
	if(restart != lastRestart)
	{
		ofLogNotice() << "Restart change " << restart;
		if(restart)
			Game->Restart();
		lastRestart = restart;
	}
	ofSleepMillis(10);
#endif

}

//--------------------------------------------------------------
void ofApp::draw(){
#ifdef DRAW
	Game->Draw();
	ofSleepMillis(4);
#endif
#ifdef LASER
	Game->SendData();
	ofSleepMillis(15);
#endif // LASER
}

//--------------------------------------------------------------
void ofApp::exit()
{
	WiringPi::clear();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
#ifndef PI
	if(key == 'r')
		Game->Restart();
	Game->QueueInput(key);
#endif
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
