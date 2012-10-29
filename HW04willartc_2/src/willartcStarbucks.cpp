#include "willartcStarbucks.h"


willartcStarbucks::willartcStarbucks(){
	arrLength = 0;
}

void willartcStarbucks::build(Entry* c, int n){
	
	arr = new Entry[n];
	for(int i =0; i<n;i++){
		arr[i] = c[i];
	}
	arrLength = n;
	c = arr;
}


Entry* willartcStarbucks::getNearest(double x, double y) {
	Entry* e = &arr[0];
	Entry* b;
	double best = getDistance(x,y,&arr[0]);
	double temp = 0;
	for(int i = 1; i < arrLength;i++){
		temp = getDistance(x,y,&arr[i]);
		if(temp < best) {
			best = temp;
			b = &arr[i];
		}
	}
	return b;
}

double willartcStarbucks::getDistance(double x, double y, Entry* e){
	return sqrt(((x - e->x)*(x - e->x))+((y - e->y)*(y - e->y)));
}