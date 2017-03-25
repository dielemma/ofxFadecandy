#include "ofxLEDstrip.h"

ofxLEDstrip::ofxLEDstrip()
{

}

ofxLEDstrip::~ofxLEDstrip()
{
	
}

ofxLEDstrip::ofxLEDstrip(int startIndex, int count, int x, int y, int spacing, float angle_rad, bool reversed)
{
	this->setup(startIndex, count, x, y, spacing, angle_rad, reversed);
	
}

void ofxLEDstrip::setup(int startIndex, int count, int x, int y, int spacing, float angle_rad, bool reversed)
{
	this-> count = count;

	float s = sin(angle_rad);
	float c = cos(angle_rad);

	pixelCoords.resize(count);	

	for (int i = 0; i< count; i++)
	{
		int j = reversed ? count - 1 - i : i;
		
		pixelCoords[j] = ofVec2f((int)(x + (i - (count-1)/2.0) * spacing * c + 0.5), 
			(int)(y + (i - (count-1)/2.0) * spacing * s + 0.5));

		// ofLog(OF_LOG_ERROR, ofToString(pixelCoords[j].x));		

	}

	colors.resize(count);
	std::fill(colors.begin(), colors.end(), ofColor(255,0,0));

	// ofLog(OF_LOG_ERROR, "stripcount = " + ofToString(pixelCoords.size()));

}

void ofxLEDstrip::draw()
{
	// ofLog(OF_LOG_NOTICE,"draw strip");
	
	ofNoFill();
	ofSetColor(255,255,255);
	for (int i=0; i<count; i++ )
	{
		ofDrawCircle(pixelCoords[i],2);
	}

}