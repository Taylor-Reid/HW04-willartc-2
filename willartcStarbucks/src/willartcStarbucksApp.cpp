/*
Greetings!

Now as HW04-willartc-2, an unsorted array based venture, since the quad tree
refused to build more than 5 nodes, despite my best efforts.

Controls:
Zoom in  = . or + (on keypad)
Zoom out = , or - (on keypad)
Pan = arrow keys or w,a,s,d
Clicking on the surface prints a yellow pixel, and the closest starbucks location turns white.
	Note: the clicking function does not work correctly when the window is not completely zoomed out.

Color assignment:

Red:
	All starbucks locations where floor((x*y)*100)%2 is odd have 100% red,
		even numbers have 0% red.
	Census locations are colored red according to their nearest Starbucks locations.

Green:
	All Starbucks locations get 100% green.
	Census locations get green scaled by the population difference of their associated block ID.

Blue:
	All points get blue scaled according to the product of their  coordinates (x*y),
		this is an attempt to give every point a different color.

*/
#include "Resources.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "willartcStarbucks.h"
#include <cmath>

using namespace ci;
using namespace ci::app;
using namespace std;

class willartcStarbucksApp : public AppBasic {
  private:
	vector <Entry> storage;
	vector <CensusEntry> census;
	vector <CensusEntry> census2;

  public:
	willartcStarbucks* starbucks;
	int starbucksDataLength;
	CensusEntry* census2000Arr;
	int census2000DataLength;
	CensusEntry* census2010Arr;
	int census2010DataLength;
	Entry* appArr;
	Surface* surface_;
	uint8_t* pixels;
	Surface* zoomSurf;
	uint8_t* zoomPix;
	int zoomConst;
	int yOffset;  // For zoom
	int xOffset;  // For zoom
	int* blockIds2000;
	int* blockIds2010;
	static const int kAppWidth = 1024;
	static const int kAppHeight = 640;
	static const int kSurfaceSize = 1024;

	void prepareSettings(Settings* settings);
	void setup();
	void clear();
	void mouseDown( MouseEvent event );
	void update();
	void draw();
	Color* assignStarbucksColor(double x, double y);
	void keyDown(KeyEvent event);
	void zoom();
	void drawPoint(double x, double y, Color c);
};

// Standard prepare settings
void willartcStarbucksApp::prepareSettings(Settings* settings){
(*settings).setWindowSize(kAppWidth, kAppHeight);
(*settings).setResizable(false);
}

void willartcStarbucksApp::setup()
{	
	// Master surface
	surface_ = new Surface(kSurfaceSize,kSurfaceSize,true);
	pixels = (*surface_).getData();

	// Surface for zoom function, this is the surface that the user views
	zoomSurf = new Surface(kSurfaceSize,kSurfaceSize,true);
	zoomPix = (*zoomSurf).getData();
	zoomConst = 1;
	yOffset = 0;
	xOffset = 0;
	clear();

	// Initialize Starbucks data structure
	starbucks = new willartcStarbucks();

	// Read in Starbucks_2006.csv
	ifstream in("Starbucks_2006.csv");

	string row;
	double x;
	double y;
	char delimiter;
	int i = 0;

	// Read starbucks locations into the storage vector
	while(in.good()){
		Entry* e = new Entry();
		storage.push_back(*e);
		getline(in, row, ',');
		
		storage[i].identifier = row;
		in >> x;
		storage[i].x = x;
		in >> delimiter;
		in >> y;
		storage[i].y = y;
		i++;
	}

	// Reads the vector into an array in order to call build()
	appArr = new Entry[storage.size()];
	starbucksDataLength = storage.size();

	for(int j = 0; j < ((int)storage.size()); j++) {
		appArr[j] = storage[j];
	}
	
	starbucks->build(appArr, storage.size());  // Build()
	
	// Read in Census 2000
	ifstream intwo("Census_2000.csv");
	
	x = 0.0;
	y = 0.0;
	delimiter = ',';
	i = 0;
	int state = 0;
	int county = 0;
	int censusTract = 0;
	int block = 0;
	int popn = 0;
	blockIds2000 = new int[10];

	// Read census 2000 locations into the storage vector
	while(intwo.good()){
		CensusEntry* ce = new CensusEntry();
		census.push_back(*ce);
		intwo >> state;	// 4 times, 4 columns
		intwo >> delimiter;
		census[i].stateId = state;
		intwo >> county;
		intwo >> delimiter;
		census[i].countyId = county;
		intwo >> censusTract;
		intwo >> delimiter;
		census[i].censusId = censusTract;
		intwo >> block;
		intwo >> delimiter;
		census[i].blockId = block;
		intwo >> popn;
		census[i].population = popn;
		intwo >> delimiter;
		intwo >> x;
		census[i].x = x;
		intwo >> delimiter;
		intwo >> y;
		census[i].y = y;

		blockIds2000[block] += popn;

		i++;
	}

	// Reads the Census_2000 vector into an array
	// Probably unnecessary
	census2000DataLength = census.size();
	census2000Arr = new CensusEntry[census2000DataLength];
	for(int q = 0; q < census2000DataLength; q++){
		census2000Arr[q] = census[q];
	}

	// Read in Census 2010

	ifstream inthree("Census_2010.csv");
	i=0;
	blockIds2010 = new int[10];

	// Read census 2010 locations into the storage vector
	while(inthree.good()){
		CensusEntry* ce = new CensusEntry();
		census2.push_back(*ce);
		inthree >> state;	// 4 times, 4 columns
		inthree >> delimiter;
		census2[i].stateId = state;
		inthree >> county;
		inthree >> delimiter;
		census2[i].countyId = county;
		inthree >> censusTract;
		inthree >> delimiter;
		census2[i].censusId = censusTract;
		inthree >> block;
		inthree >> delimiter;
		census2[i].blockId = block;
		inthree >> popn;
		census2[i].population = popn;
		inthree >> delimiter;
		inthree >> x;
		census2[i].x = x;
		inthree >> delimiter;
		inthree >> y;
		census2[i].y = y;
		blockIds2010[block] += popn;

		i++;
	}

	// Reads the Census_2000 vector into an array
	// Probably unnecessary
	census2010DataLength = census2.size();
	census2010Arr = new CensusEntry[census2010DataLength];
	for(int q = 0; q < census2010DataLength; q++){
		census2010Arr[q] = census2[q];
	}
	
	/*
	// Used to get the maximum population differences, which were then
	// hardcoded.  Which is very, very, very, bad practice.
	int diff = 0;
	for(int d = 1; d<10; d++){
		diff = blockIds2010[d] - blockIds2000[d];
	}
	*/

	// Prepare to draw points
	Color c = Color(0,0,0);
	clear();

	// Get the color values and draw the census points
	for(int i = 0; i < census2000DataLength; i++){
		CensusEntry* temp = &census2000Arr[i];
		Entry* starTemp = starbucks->getNearest(temp->x,temp->y);
		Color* k = assignStarbucksColor(starTemp->x,starTemp->y);
			int diff = blockIds2010[temp->blockId]-blockIds2000[temp->blockId];
				k->g = 127;
			if(diff >= 0){
				k->g += (127*(diff/17694259))/4;
			} else {
				k->g += (127*(diff/-3107888))/4;
			}
		drawPoint(temp->x,temp->y,*k);
	}

	for(int i = 0; i < census2010DataLength; i++){
		CensusEntry* temp = &census2010Arr[i];
		Entry* starTemp = starbucks->getNearest(temp->x,temp->y);
		Color* k = assignStarbucksColor(starTemp->x,starTemp->y);
			int diff = blockIds2010[temp->blockId]-blockIds2000[temp->blockId];
				k->g = 127;
			if(diff >= 0){
				k->g += (127*(diff/17694259))/4;
			} else {
				k->g += (127*(diff/-3107888))/4;
			}
		drawPoint(temp->x,temp->y,*k);
	}
	
	// Draw the Starbucks locations
	for(int i = 0; i < starbucksDataLength; i++){
		Entry* temp = &appArr[i];
		Color* k = assignStarbucksColor(temp->x, temp->y);
		k->g = 255;
		drawPoint(temp->x,temp->y,*k);
	}

}
	// Streamlines the drawing process
void willartcStarbucksApp::drawPoint(double x, double y, Color c){
		int xI = floor(kSurfaceSize*x) + 10;
		int yI = floor(kSurfaceSize*(1-y)*0.6) + 10;
		int index = 4*(xI + yI*kSurfaceSize);
		pixels[index] = c.r;
		pixels[index+1] = c.g;
		pixels[index+2] = c.b;
}

	// I've had better luck with my own clear() function
void willartcStarbucksApp::clear(){
	Color c = Color(0,0,0);
	for(int y = 0; y < kSurfaceSize; y++){
		for(int x = 0; x < kSurfaceSize; x++){
			int index = 4*(x + y*kSurfaceSize);
			pixels[index] = c.r;
			pixels[index+1] = c.g;
			pixels[index+2] = c.b;
		}
	}
}

	//  Creates short-term color data for drawing points
Color* willartcStarbucksApp::assignStarbucksColor(double x, double y){
	int r = 0;
	int isRed = (floor((x*y)*100));

	if(isRed%2 == 1){
		r = 255;
	} else {
		r = 0;
	}

	//floor((1-x)*(1-y)*255);
	int g = 0; //floor((sin(3.14159*x)*sin(3.14159*y)-0.5)*255);
	if(g < 0) { g = 0;}
	int b = floor(x*y*255);
	return &Color(r,g,b);
}


	// Tests getNearest()
void willartcStarbucksApp::mouseDown( MouseEvent event )
{
	// RED
	Color c = Color(255, 255, 0);

	// New line
	//console() << endl;

	// Get current location
	int x = event.getX();
	int y = event.getY();

	//console() << "Print current location as int" << endl;
	//console() << x << endl;
	//console() << y << endl;

	// Convert current location into 0 < double < 1
	double xd = (((double)x)-10)/kSurfaceSize;
	double yd = 1-((((double)y)-10)/(kSurfaceSize*0.6));

	//console() << "Print converted current location" << endl;
	//console() << xd << endl;
	//console() << yd << endl;

	// Draw current location RED
	//int index = 4*(x + y*kSurfaceSize);
	int index = 4*(((x/zoomConst)+xOffset) + (((y/zoomConst)+yOffset)*kSurfaceSize));
	pixels[index] = c.r;
	pixels[index+1] = c.g;
	pixels[index+2] = c.b;

	// Note: Since we drew the current location
	// All of the local variables may be reused

	// Get nearest Starbucks
	Entry* temp = starbucks->getNearest(xd,yd);

	// Get nearest Starbucks doubles
	xd = temp->x;
	yd = 1-(temp->y);

	//console() << "Print nearest Starbucks doubles" << endl;
	//console() << xd << endl;
	//console() << yd << endl;

	// Convert nearest Starbucks doubles to ints (with scaling)

	//double xd = (((double)x)-10)/kSurfaceSize;
	//double yd = 1-((((double)y)-10)/(kSurfaceSize*0.6));

	x = floor(kSurfaceSize*xd) + 10;
	y = floor(kSurfaceSize*yd*0.6) + 10;

	//console() << "Print nearest Starbucks ints (converted)" << endl;
	//console() << x << endl;
	//console() << y << endl;

	// Draw closest starbucks BLUE
	c.r = 255;
	c.g = 255;
	c.b = 255;
	index = 4*(x + y*kSurfaceSize);
	pixels[index] = c.r;
	pixels[index+1] = c.g;
	pixels[index+2] = c.b;
}

	// For zoom controls
void willartcStarbucksApp::keyDown(KeyEvent event)
{
	if((event.getCode() == KeyEvent::KEY_KP_PLUS)||(event.getCode() == KeyEvent::KEY_PERIOD)){
		//console() << "Zoom in" << endl;
		zoomConst *= 2;
	} 
	if((event.getCode() == KeyEvent::KEY_KP_MINUS)||(event.getCode() == KeyEvent::KEY_COMMA)){
		//console() << "Zoom out" << endl;
		if(zoomConst != 1){
			zoomConst /= 2;
			if(xOffset > (kSurfaceSize - (kSurfaceSize/zoomConst))){
				xOffset = (kSurfaceSize - (kSurfaceSize/zoomConst));
			}
			if(yOffset > (kSurfaceSize - (kSurfaceSize/zoomConst))){
				yOffset = (kSurfaceSize - (kSurfaceSize/zoomConst));
			}
		}
	}
	// Pan up
	if((event.getCode() == KeyEvent::KEY_UP)||(event.getCode() == KeyEvent::KEY_w)){
		//console() << "Pan up" << endl;
		if(yOffset > 0){
			yOffset -= 16;
		}
	}
	// Pan down
	if((event.getCode() == KeyEvent::KEY_DOWN)||(event.getCode() == KeyEvent::KEY_s)){
		//console() << "Pan down" << endl;
		if(yOffset < (kSurfaceSize - (kSurfaceSize/zoomConst))){
			yOffset += 16;
		}
	}
	// Pan right
	if((event.getCode() == KeyEvent::KEY_RIGHT)||(event.getCode() == KeyEvent::KEY_d)){
		//console() << "Pan right" << endl;
		if(xOffset < (kSurfaceSize - (kSurfaceSize/zoomConst))){
			xOffset += 16;
		}
	}
	// Pan left
	if((event.getCode() == KeyEvent::KEY_LEFT)||(event.getCode() == KeyEvent::KEY_a)){
		//console() << "Pan left" << endl;
		if(xOffset > 0){
			xOffset -= 16;
		}
	}

}
	// Transfers a section the master surface to another zoom surface for zooming and panning
void willartcStarbucksApp::zoom(){
	for(int g = 0; g < kSurfaceSize; g++){
		for(int i = 0; i < kSurfaceSize; i++){
			int x = i;
			int y = g;
			int index = 4*(x + y*kSurfaceSize);
			int index2 = 4*(((x/zoomConst)+xOffset) + (((y/zoomConst)+yOffset)*kSurfaceSize));
			
			zoomPix[index] = pixels[index2];
			zoomPix[index+1] = pixels[index2+1];
			zoomPix[index+2] = pixels[index2+2];
		}
	}
}

void willartcStarbucksApp::update()
{
	//clear();
	zoom();
}

void willartcStarbucksApp::draw()
{
	// clear out the window with black
	//gl::clear( Color( 0, 0, 0 ) );
	//gl::draw(*surface_);
	gl::draw(*zoomSurf);
}

CINDER_APP_BASIC( willartcStarbucksApp, RendererGl )
