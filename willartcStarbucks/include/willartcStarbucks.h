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
	int traverseIterator;
	int arrLength;
	Entry* e; // For sentinel

public:

	willartcStarbucks();
	~willartcStarbucks();
	
	Node* insert(Entry* k, Node* n); // insert adds a Node with recursion
	void build(Entry* c, int n);  // build calls insert
	
	Node* search(Node* n, double x, double y);
	Entry* getNearest(double x, double y);  // Calls search
	
	double getDistance(double x, double y, Entry* e);
	bool checkSameLocation(double x, double y, Entry* e);
	
	double dist(Node* n, double x, double y);
	Node* getSentinel();
	
	int traverse(Node* s);  // counts the total number of nodes
	


};