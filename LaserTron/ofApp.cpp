#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	listener = new DacListener();
	listener->startThread();
	while (listener->GetDacCount() < 1)
		ofSleepMillis(100);
	DAC = listener->GetDac(0);
	ED = new EtherDreamController(DAC, false);
	ED->SetPPS(30000);
	ED->SetWaitBeforeSend(true);
	ED->SetAutoConnect(true);
	Game = new GameThread(ED);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	Game->Draw();
	ofSleepMillis(20);
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
