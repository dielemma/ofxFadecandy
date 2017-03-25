 #include "ofxFadecandy.h"

ofxFadecandy::ofxFadecandy()
{

}

ofxFadecandy::~ofxFadecandy()
{
}

bool ofxFadecandy::setup(string address, int port, bool autoUpdate, bool autoDraw )
{
	this->address = address;
	this->port = port;

	this->autoUpdate = autoUpdate;
	this->autoDraw = autoDraw;
	// register update and draw methods to get called automatically
	if (autoUpdate)
	{	
		ofAddListener(ofEvents().update, this, &ofxFadecandy::update);
	}
	if (autoDraw)
	{
		ofAddListener(ofEvents().draw, this, &ofxFadecandy::draw);	
	}	

	// these values flag that the stage has not been setup
	stageX0 = -1;
	stageY0 = -1;
	stageWidth = -1;
	stageHeight = -1;

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

void ofxFadecandy::toggleAutoUpdate()
{
	if (autoUpdate)
	{	
		ofRemoveListener(ofEvents().update, this, &ofxFadecandy::update);
	}
	else
	{
		ofAddListener(ofEvents().update, this, &ofxFadecandy::update);	
	}
	autoUpdate = !autoUpdate;
}

bool ofxFadecandy::getAutoUpdateState()
{
	return autoUpdate;
}

void ofxFadecandy::toggleAutoDraw()
{
	if (autoDraw)
	{	
		ofRemoveListener(ofEvents().draw, this, &ofxFadecandy::draw);
	}
	else
	{
		ofAddListener(ofEvents().draw, this, &ofxFadecandy::draw);	
	}
	autoDraw = !autoDraw;
}

bool ofxFadecandy::getAutoDrawState()
{
	return autoDraw;
}

// initialize framebuffer to accomodate data for specified number of LED pixels
void ofxFadecandy::initFrameBuffer(int npixels)
{
	// number of bytes needed for RGB color data
	const int frameBytes = npixels*3;

	// resize capacity 
	frameBuffer.resize(sizeof(OPCClient::Header) + frameBytes);

	// construct header
	OPCClient::Header::view(frameBuffer).init(0, opc.SET_PIXEL_COLORS, frameBytes);

}

// count number of LEDs and initialize frame buffer
void ofxFadecandy::initFrameBuffer()
{
	initFrameBuffer(allLEDcoords.size());
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

	//glPixels = new unsigned char [stageWidth * stageHeight * 4];
    // screenCapture.allocate(stageWidth, stageHeight, GL_RGBA);
    // screenCapture.begin();
    //     ofClear(0);
    // screenCapture.end();

    stageImg.allocate(stageWidth,stageHeight,ofImageType::OF_IMAGE_COLOR_ALPHA);
}

// Get pixel data from entire capture region 
void ofxFadecandy::updateStagePixels()
{

	if (stageX0==-1 || stageY0==-1 || stageWidth ==-1 || stageHeight == -1)
	{
		string errMsg = "Stage has not been configured. Did you call \"setupStage\"?\n";
		ofLog(OF_LOG_ERROR) << errMsg;
		error.push_back(errMsg);
		ofSystemAlertDialog(errMsg);
	}

	try
	{
		// get stage pixel data from OpenGL screen data
		stageImg.getTexture().loadScreenData(stageX0, stageY0, stageWidth, stageHeight);
		stageImg.getTexture().readToPixels(stagePixels);
		
		// flip vertically; OpenGL uses opposite convention than OFX
		stagePixels.mirror(true, false);

		const int LEDcount = allLEDcoords.size();
		for (int i=0; i<LEDcount; i++)
		{		
			const int x = allLEDcoords[i].x - stageX0;
			const int y = allLEDcoords[i].y - stageY0;

			allLEDcolors[i] = stagePixels.getColor(x, y);
		}
	}
	catch (const std::exception& e)
	{
		string errMsg = "Error updating stage pixels. Verify stage dimensions are sensible.\n" + ofToString(e.what());
		ofLog(OF_LOG_ERROR) << errMsg;
		error.push_back(errMsg);
		ofSystemAlertDialog(errMsg);
	}
	

}


// automatically called by ofApp update event.
void ofxFadecandy::update(ofEventArgs& event)
{
	// clear errors
	error.clear();	

}

// draw method called every loop.  This gets called AFTER ofApp::draw
void ofxFadecandy::draw(ofEventArgs& event)
{
	// ofLog(OF_LOG_NOTICE,"draw fc");
	ofNoFill();
	// draw a rectangle around the grab region; just to be clear
	ofSetColor(255,20,147);
	ofDrawRectangle(stageX0, stageY0, stageWidth, stageHeight);

	this->updateStagePixels();

}

// send color data to OPC server.  "pixels" must have data for all LEDs in frame
void ofxFadecandy::write(vector<ofColor> pixels)
{
	if (opc.tryConnect())
	{
		int N = pixels.size();
		
		// use pointer arithmetic to populate frameBuffer data using
		// 	Header struct in opc_client.  
		//	(Same method as used by EffectRunner in fadecandy cpp example code)
		uint8_t *dest = OPCClient::Header::view(frameBuffer).data();
		
		// add rgb color data for each pixel, sequentially
		for (int p=0; p<N; p++)
		{
			*(dest++) = std::min<int>(255, std::max<int>(0, pixels[p].r));
			*(dest++) = std::min<int>(255, std::max<int>(0, pixels[p].g));
			*(dest++) = std::min<int>(255, std::max<int>(0, pixels[p].b));		
		}

		// write data to device
		opc.write(frameBuffer);
	}
}

void ofxFadecandy::connect()
{
	opc.tryConnect();
}


OPCClient& ofxFadecandy::getClient()
{
	return opc;
}