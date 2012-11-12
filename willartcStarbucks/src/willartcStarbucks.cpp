#include "willartcStarbucks.h"


willartcStarbucks::willartcStarbucks(){
	arrLength = 0;
	e = new Entry();
	e->identifier = "sentinel";
	e->x = 0.5;
	e->y = 0.5;
	sentinel = new Node(e);  // Never had a problem with sentinel
}

willartcStarbucks::~willartcStarbucks(){
}

void willartcStarbucks::build(Entry* c, int n){
	
	entryArr = new Entry[n];
	arrLength = n;  

	for(int i =0; i<arrLength;i++){
		(entryArr)[i] = *(new Entry(c[i]));
	}	
}

Entry* willartcStarbucks::getNearest(double x, double y) {
	Entry* nearest = &entryArr[0];
	double shortestDistance = getDistance(x,y,&entryArr[0]);
	for(int q = 1; q<arrLength; q++){
		double dist = getDistance(x,y,&entryArr[q]);
		if(dist < shortestDistance){
			shortestDistance = dist;
			nearest = &entryArr[q];
		}
	}
	return nearest;
}

double willartcStarbucks::getDistance(double x, double y, Entry* e){
	return sqrt(((x - e->x)*(x - e->x))+((y - e->y)*(y - e->y)));
}

// same location" if both |x1 - x2| <= 0.00001 and |y1 - y2| < 0.00001
bool willartcStarbucks::checkSameLocation(double x, double y, Entry* e){

	if((abs(x-(e->x)) < 0.00001)&&(abs(y-(e->y)) < 0.00001)) { // Used to be <=
		return true;
	} else {
		return false;
	}
}

Node* willartcStarbucks::getSentinel(){
	return sentinel;
}