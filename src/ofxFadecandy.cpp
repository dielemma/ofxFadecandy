 #include "ofxFadecandy.h"

bool ofxFadecandy::setup(string address, int port)
{
	// register update method to get called automatically
	ofAddListener(ofEvents().update, this, &ofxFadecandy::update);
	ofAddListener(ofEvents().draw, this, &ofxFadecandy::draw);

	this->address = address;
	this->port = port;

	try
	{
		// resolve server
		opc.resolve(address.c_str(), port);

		// connect to server
		connect();
	}
	catch (const std::exception& e)
	{
		string errMsg = "Error initializing OPC client.\n" + ofToString(e.what());
		ofLog(OF_LOG_ERROR) << errMsg;
		error.push_back(errMsg);
		ofSystemAlertDialog(errMsg);
		return false;
	}
	return true;
}

// initialize framebuffer to accomodate data for specified number of LED pixels
void ofxFadecandy::initFrameBuffer(int npixels)
{
	this->frameBytes = npixels*3;

	// resize capacity 
	frameBuffer.resize(sizeof(OPCClient::Header) + frameBytes);
	// construct header
	OPCClient::Header::view(frameBuffer).init(0, opc.SET_PIXEL_COLORS, frameBytes);

	// ofLog(OF_LOG_NOTICE) << "headLength" + ofToString(sizeof(OPCClient::Header));

	// ofLog(OF_LOG_NOTICE) << "fbLength=" + ofToString(frameBuffer.size());
	// ofLog(OF_LOG_NOTICE) << "fb0=" + ofToString(frameBuffer[0]);
	// ofLog(OF_LOG_NOTICE) << "fb1=" + ofToString(frameBuffer[1]);
	// ofLog(OF_LOG_NOTICE) << "fb2=" + ofToString(frameBuffer[2]);
	// ofLog(OF_LOG_NOTICE) << "fb3=" + ofToString(frameBuffer[3]);
}

// count number of LEDs in each LED group, and initialize frame buffer
void ofxFadecandy::initFrameBuffer()
{
	int npx = 0;
	for (uint i=0; i< LEDgroups.size(); i++)
	{
		npx += LEDgroups[i]->pixelCoords.size();		
	}

	initFrameBuffer(npx);
}

/*
Setup "stage" region for capturing LED pixels, and create
screen capture object.
	"x0" is the minimum x-coord of the capture region
	"y0" is the minimum y-coord of the capture region
	"width" is the width of the capture region
	"height" is the height of the capture region
*/
void ofxFadecandy::setupStage(int x0, int y0, int width, int height)
{
	stageX0 = x0;
	stageY0 = y0;
	stageWidth = width;
	stageHeight = height;

	glPixels = new unsigned char [stageWidth * stageHeight * 4];
    // screenCapture.allocate(stageWidth, stageHeight, GL_RGBA);
    // screenCapture.begin();
    //     ofClear(0);
    // screenCapture.end();

    screen.allocate(stageWidth,stageHeight,ofImageType::OF_IMAGE_COLOR_ALPHA);
}

// Get pixel data from entire capture region 
void ofxFadecandy::updateStagePixels()
{
	// read block of pixels from frame buffer
	glReadPixels(stageX0, stageY0, stageWidth, stageHeight, GL_RGBA,
	 GL_UNSIGNED_BYTE, glPixels);

	screen.getTexture().loadScreenData(stageX0, stageY0, stageWidth, stageHeight);
	screen.getTexture().readToPixels(stagePixels);
	stagePixels.mirror(true, false);

	const int LEDcount = allLEDcoords.size();
	for (int i=0; i<LEDcount; i++)
	{		
		const int x = allLEDcoords[i].x - stageX0;
		const int y = allLEDcoords[i].y - stageY0;
		// ofLog(OF_LOG_NOTICE,"x="+ofToString(x)+"; y="+ofToString(y));

		allLEDcolors[i] = stagePixels.getColor(x, y);
	}

	//screenCapture.end();

	

	// store data in stagePixels
	// stagePixels.setFromPixels(glPixels, stageWidth, stageHeight, OF_IMAGE_COLOR_ALPHA);


}


// automatically called by ofApp update event.
void ofxFadecandy::update(ofEventArgs& event)
{
	// clear errors
	error.clear();
	
	// update screen capture
	//screenCapture.begin();	
	// ofLog(OF_LOG_NOTICE,"--scBEGIN--");

}

void ofxFadecandy::draw(ofEventArgs& event)
{
	// ofLog(OF_LOG_NOTICE,"draw fc");
	ofNoFill();
	// draw a rectangle around the grab region; just to be clear
	ofSetColor(255,20,147);
	ofDrawRectangle(stageX0, stageY0, stageWidth, stageHeight);

	this->updateStagePixels();
	//screenCapture.draw(stageX0,stageY0);


	//**TODO: add call to updateStagePixels?

}

void ofxFadecandy::write(vector<ofColor> pixels)
{
	int N = pixels.size();
	
	// // vector to hold color data
	// std::vector<uint8_t> data;

	// // store r,g,b data sequentially for each pixel
	// for (int p=0; p<N; p++)
	// {
	// 	data.push_back(pixels[p].r);
	// 	data.push_back(pixels[p].g);
	// 	data.push_back(pixels[p].b);
	// }


	// use pointer arithmetic to populate frameBuffer data
	uint8_t *dest = OPCClient::Header::view(frameBuffer).data();
	for (int p=0; p<N; p++)
	{
		*(dest++) = std::min<int>(255, std::max<int>(0, pixels[p].r));
		*(dest++) = std::min<int>(255, std::max<int>(0, pixels[p].g));
		*(dest++) = std::min<int>(255, std::max<int>(0, pixels[p].b));		
	}

	// ofLog(OF_LOG_NOTICE) << ofToString(frameBuffer[5]);

	// write data to device
	opc.write(frameBuffer);
}

void ofxFadecandy::connect()
{
	opc.tryConnect();
}

OPCClient& ofxFadecandy::getClient()
{
	return opc;
}