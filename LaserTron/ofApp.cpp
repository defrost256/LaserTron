#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
#ifdef LASER
	listener = new DacListener();
	listener->startThread();
	while (listener->GetDacCount() < 1)
		ofSleepMillis(100);
	DAC = listener->GetDac(0);
	ED = new EtherDreamController(DAC, true);
	ED->SetPPS(30000);
	ED->SetWaitBeforeSend(true);
	ED->SetAutoConnect(true);
	Game = new GameThread(ED);
#else
	Game = new GameThread(nullptr);
#endif //LASER == 1
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
#ifdef DRAW
	Game->Draw();
	ofSleepMillis(20);
#endif
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	Game->QueueInput(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
