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
	ED->Start();
	frame = ofxIlda::Frame();
}

//--------------------------------------------------------------
void ofApp::update(){
	float sinT = 0.3;
	frame.clear();
	frame.params.output.color = ofFloatColor(sinT, sinT, sinT);
	frame.drawCalibration();
	frame.update();
	ED->AddPoints(frame);
	t += 0.001;
	ofLogNotice("EtherDream",  "%f", sinT);
}

//--------------------------------------------------------------
void ofApp::draw(){
	//frame.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key)
	{
	case 'a':
		frame.params.output.endCount--;
		frame.update();
		break;
	case 'd':
		frame.params.output.endCount++;
		frame.update();
		break;
	case 'w':
		frame.params.output.blankCount++;
		frame.update();
		break;
	case 's':
		frame.params.output.blankCount--;
		frame.update();
		break;
	default:
		break;
	}
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
