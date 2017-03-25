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
	bool setup(string address, int port);
	void update(ofEventArgs& event);
	void draw(ofEventArgs& event);
	void draw();

	void initFrameBuffer(int npixels);
	void initFrameBuffer();
	void write(vector<ofColor>);

	void cleanup();
	void close();

	bool isConnected();
	void tryConnect();


	OPCClient& getClient();
	string address;
	int port;
	
	vector <string> error;	

	void setupStage(int x0, int y0, int width, int height);
	void updateStagePixels();
	ofPixels stagePixels;

	vector<ofVec2f> allLEDcoords;
	vector<ofColor> allLEDcolors;

	std::vector <shared_ptr<ofxLEDobject>> LEDgroups;

	std::vector<uint8_t> frameBuffer;
		

	ofImage screen;

private:
	OPCClient opc;
	float timer;

	// this holds the data to be sent to the device (incl. header)
	
	int frameBytes;

	void connect();
	void disconnect();

	unsigned char * glPixels;
	ofFbo screenCapture;

	int stageX0;
	int stageY0;
	int stageWidth;
	int stageHeight;

};
