#include "ofMain.h"

#include "opc_client.h"
#include "ofxLEDobject.h"

typedef struct OPCPacket_Header
{
	uint8_t channel;
	uint8_t command;
	uint16_t data_length;

} OPCPacket_Header_t;

typedef struct OPCPacket
{
	OPCPacket_Header_t header;
	unsigned char data[];
} *OPCPacket_t;

class ofxFadecandy
{
public:
	ofxFadecandy();
	~ofxFadecandy();

	bool setup(string address, int port, bool autoUpdate = true, bool autoDraw = true);
	void update(ofEventArgs& event);
	void draw(ofEventArgs& event);


	void toggleAutoDraw();
	void toggleAutoUpdate();
	bool getAutoUpdateState();
	bool getAutoDrawState();

	void cleanup();
	void close();

	OPCClient& getClient();
	string address;
	int port;
	bool isConnected();
	void tryConnect();

	
	vector <string> error;	

	void setupStage(int x0, int y0, int width, int height);
	void updateStagePixels();
	
	ofPixels stagePixels;
	ofImage stageImg;

	vector<ofVec2f> allLEDcoords;
	vector<ofColor> allLEDcolors;

	std::vector<uint8_t> frameBuffer;
	void initFrameBuffer(int npixels);
	void initFrameBuffer();
	void write(vector<ofColor>);
		

	

private:
	bool autoDraw, autoUpdate;
	OPCClient opc;
	//float timer;

	void connect();

	//unsigned char * glPixels;
	//ofFbo screenCapture;

	int stageX0;
	int stageY0;
	int stageWidth;
	int stageHeight;

};
