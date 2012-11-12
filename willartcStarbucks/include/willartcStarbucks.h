/*
Builds an unsorted array data structure, stores Starbucks Entrys in it, 
and returns the nearest Starbucks location to any given pair of coordinates 
that are doubles between 0 and 1.
*/
#include "Node.h"
#include <iostream>
#include <math.h>
#include <fstream>
#include <stdio.h>

using namespace std;

class willartcStarbucks : public Starbucks {
private:
	Node* sentinel;
	Entry* entryArr;
	int arrLength;
	Entry* e; // For sentinel

public:

	willartcStarbucks();
	~willartcStarbucks();
	
	Node* getSentinel();

	void build(Entry* c, int n);  // build calls insert
	
	Entry* getNearest(double x, double y);  // Calls search
	
	double getDistance(double x, double y, Entry* e);
	bool checkSameLocation(double x, double y, Entry* e);
};