#include <iostream>
#include "ofMain.h"
#include "ofxLEDobject.h"

class ofxLEDstrip: public ofxLEDobject {

public:
	ofxLEDstrip();
	virtual ~ofxLEDstrip();

	ofxLEDstrip(int startIndex, int count, int x, int y, int spacing, float angle_rad, bool reversed);

	void setup(int startIndex, int count, int x, int y, int spacing, float angle_rad, bool reversed);

	void draw();

private:
	

};