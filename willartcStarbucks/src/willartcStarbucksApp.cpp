/*
Greetings!

Controls:
Zoom in  = . or + (on keypad)
Zoom out = , or - (on keypad)
Pan = arrow keys or w,a,s,d

*/
#include "Resources.h"
#include "cinder/app/AppBasic.h"
#include "cinder/CinderResources.h"
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

  public:
	
	willartcStarbucks* quadTree;
	Entry* appArr;
	int starbucksDataLength;
	Surface* surface_;
	uint8_t* pixels;
	Surface* zoomSurf;
	uint8_t* zoomPix;
	int zoomConst;
	int yOffset;  // For zoom
	int xOffset;  // For zoom
	static const int kAppWidth = 1024;
	static const int kAppHeight = 640;
	static const int kSurfaceSize = 1024;

	void prepareSettings(Settings* settings);
	void setup();
	void clear();
	void appendToStorage(Entry* e);
	void testGetNearest();
	void mouseDown( MouseEvent event );
	void update();
	void draw();
	void keyDown(KeyEvent event);
	void zoom();
};

// Standard prepare settings
void willartcStarbucksApp::prepareSettings(Settings* settings){
(*settings).setWindowSize(kAppWidth, kAppHeight);
(*settings).setResizable(false);
}

void willartcStarbucksApp::setup()
{
	surface_ = new Surface(kSurfaceSize,kSurfaceSize,true);
	pixels = (*surface_).getData();
	zoomSurf = new Surface(kSurfaceSize,kSurfaceSize,true);
	zoomPix = (*zoomSurf).getData();
	zoomConst = 1;
	yOffset = 0;
	xOffset = 0;
	clear();

	// Read everything in a vector
	// Then copy to an array
	// Then call create a QuadTree
	// The call build with the length of the vector
	// Then read in data from the census' and call getNearest

	quadTree = new willartcStarbucks();

	ifstream in("Starbucks_2006.csv");
	//ifstream in("SortedData.csv");
	//vector <Entry> storage;

	string row;
	double x;
	double y;
	char delimiter;
	int i = 0;

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
		console() << row;
	}

	Entry* e = new Entry[storage.size()];
	starbucksDataLength = storage.size();
	appArr = new Entry[storage.size()];

	for(int j = 0; j < ((int)storage.size()); j++) {
		appArr[j] = storage[j];
	}
	e = appArr;

	
	quadTree->build(e, storage.size());
	
	for(int z = 0; z < starbucksDataLength; z++){
		storage.erase((&storage)->begin(), (&storage)->end());
	}

	int traverseIterator = quadTree->traverse(quadTree->getSentinel());
	console() << traverseIterator << endl;

	Color c = Color(0,255,0);
	clear();
	
	for(int i = 0; i < starbucksDataLength; i++){
		Entry* temp = &appArr[i];
		double xf = temp->x;
		int xI = floor(kSurfaceSize*xf) + 10;
		double yf = (1-temp->y);
		int yI = floor(kSurfaceSize*yf*0.6) + 10;
		int index = 4*(xI + yI*kSurfaceSize);
		pixels[index] = c.r;
		pixels[index+1] = c.g;
		pixels[index+2] = c.b;
	}
}

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

void willartcStarbucksApp::mouseDown( MouseEvent event )
{
	// RED
	Color c = Color(255, 0, 0);

	// New line
	console() << endl;

	// Get current location
	int x = event.getX();
	int y = event.getY();

	console() << "Print current location as int" << endl;
	console() << x << endl;
	console() << y << endl;

	// Convert current location into 0 < double < 1
	double xd = (((double)x)-10)/kSurfaceSize;
	double yd = 1-((((double)y)-10)/(kSurfaceSize*0.6));

	console() << "Print converted current location" << endl;
	console() << xd << endl;
	console() << yd << endl;

	// Draw current location RED
	//int index = 4*(x + y*kSurfaceSize);
	int index = 4*(((x/zoomConst)+xOffset) + (((y/zoomConst)+yOffset)*kSurfaceSize));
	pixels[index] = c.r;
	pixels[index+1] = c.g;
	pixels[index+2] = c.b;

	// Note: Since we drew the current location
	// All of the local variables may be reused

	// Get nearest Starbucks
	Entry* temp = quadTree->getNearest(xd,yd);

	// Get nearest Starbucks doubles
	xd = temp->x;
	yd = (temp->y);

	console() << "Print nearest Starbucks doubles" << endl;
	console() << xd << endl;
	console() << yd << endl;

	// Convert nearest Starbucks doubles to ints (with scaling)
	x = floor(kSurfaceSize*xd) + 10;
	y = floor(kSurfaceSize*yd*0.6) + 10;

	console() << "Print nearest Starbucks ints (converted)" << endl;
	console() << x << endl;
	console() << y << endl;

	// Draw closest starbucks BLUE
	c.r = 0;
	c.b = 255;
	index = 4*(x + y*kSurfaceSize);
	pixels[index] = c.r;
	pixels[index+1] = c.g;
	pixels[index+2] = c.b;
}

void willartcStarbucksApp::keyDown(KeyEvent event)
{
	if((event.getCode() == KeyEvent::KEY_KP_PLUS)||(event.getCode() == KeyEvent::KEY_PERIOD)){
		console() << "Zoom in" << endl;
		zoomConst *= 2;
	} 
	if((event.getCode() == KeyEvent::KEY_KP_MINUS)||(event.getCode() == KeyEvent::KEY_COMMA)){
		console() << "Zoom out" << endl;
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
		console() << "Pan up" << endl;
		if(yOffset > 0){
			yOffset -= 16;
		}
	}
	// Pan down
	if((event.getCode() == KeyEvent::KEY_DOWN)||(event.getCode() == KeyEvent::KEY_s)){
		console() << "Pan down" << endl;
		if(yOffset < (kSurfaceSize - (kSurfaceSize/zoomConst))){
			yOffset += 16;
		}
	}
	// Pan right
	if((event.getCode() == KeyEvent::KEY_RIGHT)||(event.getCode() == KeyEvent::KEY_d)){
		console() << "Pan right" << endl;
		if(xOffset < (kSurfaceSize - (kSurfaceSize/zoomConst))){
			xOffset += 16;
		}
	}
	// Pan left
	if((event.getCode() == KeyEvent::KEY_LEFT)||(event.getCode() == KeyEvent::KEY_a)){
		console() << "Pan left" << endl;
		if(xOffset > 0){
			xOffset -= 16;
		}
	}

}

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

void testGetNearest(){
	console() << "TEST RESULTS" << endl;
	willartcStarbucks* tempTree = new willartcStarbucks();
	Entry* e15 = tempTree->getNearest(0.0, 0.0);
	console() << "Identifier = " << e15->identifier << ", X = " << e15->x << ", Y = " << e15->y << endl;
	Entry* e1 = tempTree->getNearest(0.5, 0.0);
	console() << "Identifier = " << e1->identifier << ", X = " << e1->x << ", Y = " << e1->y << endl;
	Entry* e2 = tempTree->getNearest(0.0, 0.5);
	console() << "Identifier = " << e2->identifier << ", X = " << e2->x << ", Y = " << e2->y << endl;
	Entry* e3 = tempTree->getNearest(1.0, 0.0);
	console() << "Identifier = " << e3->identifier << ", X = " << e3->x << ", Y = " << e3->y << endl;
	Entry* e4 = tempTree->getNearest(0.0, 1.0);
	console() << "Identifier = " << e4->identifier << ", X = " << e4->x << ", Y = " << e4->y << endl;
	Entry* e5 = tempTree->getNearest(1.0, 0.5);
	console() << "Identifier = " << e5->identifier << ", X = " << e5->x << ", Y = " << e5->y << endl;
	Entry* e6 = tempTree->getNearest(0.5, 1.0);
	console() << "Identifier = " << e6->identifier << ", X = " << e6->x << ", Y = " << e6->y << endl;
	Entry* e7 = tempTree->getNearest(0.7, 0.7);
	console() << "Identifier = " << e7->identifier << ", X = " << e7->x << ", Y = " << e7->y << endl;
	Entry* e8 = tempTree->getNearest(0.8, 0.8);
	console() << "Identifier = " << e8->identifier << ", X = " << e8->x << ", Y = " << e8->y << endl;
	Entry* e9 = tempTree->getNearest(0.9, 0.9);
	console() << "Identifier = " << e9->identifier << ", X = " << e9->x << ", Y = " << e9->y << endl;
	Entry* e10 = tempTree->getNearest(1.0, 1.0);
	console() << "Identifier = " << e10->identifier << ", X = " << e10->x << ", Y = " << e10->y << endl;
}

CINDER_APP_BASIC( willartcStarbucksApp, RendererGl )
