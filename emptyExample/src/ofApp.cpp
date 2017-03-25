#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	screenW = ofGetScreenWidth();
	screenH = ofGetScreenHeight();
	ofSetWindowPosition(0,0);
	ofSetWindowShape(screenW, screenH);
	ofSetBackgroundColor(0);
	ofSetLogLevel(OF_LOG_NOTICE);

	fc.setup("127.0.0.1",7890);
	fc.setupStage(100,100,800,600);

	Nstrips = 16;
	LEDs_per_strip = 30;

	int id=0;
	for (int i=0; i<8; i++)
	{
		strips.push_back(make_shared<ofxLEDstrip>(id, LEDs_per_strip, 200, 50*(i+3),5,0,false));
		// fc.LEDgroups.push_back(strips.back());
		id+=30;
	}
	for (int i=0; i<8; i++)
	{
		strips.push_back(make_shared<ofxLEDstrip>(id, LEDs_per_strip, 600, 50*(i+3),5,0,false));
		// fc.LEDgroups.push_back(strips.back());
		id+=30;
	}


	int LEDcount = 0;
	for (int i=0; i<Nstrips; i++)
	{
		for (int j=0; j<strips[i]->count; j++)
		{
			fc.allLEDcoords.push_back(strips[i]->pixelCoords[j]);
			LEDcount++;
		}
	}
	fc.allLEDcolors.resize(LEDcount);


	fc.initFrameBuffer();

}

//--------------------------------------------------------------
void ofApp::update(){


}

//--------------------------------------------------------------
void ofApp::draw(){

	ofLog(OF_LOG_NOTICE,"draw main");
	//fc.write(strip1->colors);
	
	ofSetColor(0,255,0);
	ofFill();
	ofDrawCircle(mouseX,mouseY,42,42);

	for (int i=0; i<Nstrips; i++)
	{
		strips[i]->draw();
	}

	if (ofGetFrameNum()>1)
	{
		// ofDrawBitmapString()
		ofColor c = fc.stagePixels.getColor(1,1);
		ofSetColor(c);
		ofFill();
		ofDrawRectangle(0,0,10,10);
	}

	ofFill();
	for (int i=0; i<fc.allLEDcoords.size(); i++ )
	{
		ofSetColor(fc.allLEDcolors[i]);
		ofDrawCircle(fc.allLEDcoords[i].x+820,fc.allLEDcoords[i].y,2,2);
	}

	fc.write(fc.allLEDcolors);

	winTitle = "fps=" + ofToString((int)ofGetFrameRate());
	ofSetWindowTitle(winTitle);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == OF_KEY_F11)
	{
		ofToggleFullscreen();
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
