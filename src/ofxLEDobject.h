
#ifndef ofxLEDobject_h
#define ofxLEDobject_h

#include <stdio.h>
#include "ofMain.h"

class ofxLEDobject{

public:
	ofxLEDobject(){}
	virtual ~ofxLEDobject(){}

	// virtual void draw(int x, int y);

	int device;
	int channel;

	int count;
	vector <ofColor> colorData();
	vector <ofVec2f> getPixelCoords();

	vector <ofColor> colors;
	vector <ofVec2f> pixelCoords;
	vector <int> pixelIDs;

private:

};

#endif