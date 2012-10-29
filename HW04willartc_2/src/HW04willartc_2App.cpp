#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "willartcStarbucks.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class HW04willartc_2App : public AppBasic {
  public:

	willartcStarbucks* quadTree;

	void setup();
	void mouseDown( MouseEvent event );
	void update();
	void draw();
};

void HW04willartc_2App::setup()
{

	quadTree = new willartcStarbucks();

	ifstream in("Starbucks_2006.csv");
	vector <Entry> storage;

	string row;
	double x;
	double y;
	char delimiter;
	int i = 0;

	while(in.good()){
		Entry* e = new Entry;
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

	for(int j = 0; j < storage.size(); j++) {
		e[j] = storage[j];
	}

	quadTree->build(e, storage.size());
	
	string temp = (quadTree->getNearest(0.7,0.7))->identifier;
	console() << temp << endl;

}

void HW04willartc_2App::mouseDown( MouseEvent event )
{
}

void HW04willartc_2App::update()
{
}

void HW04willartc_2App::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_BASIC( HW04willartc_2App, RendererGl )
