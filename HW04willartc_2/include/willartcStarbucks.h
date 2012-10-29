#include "Starbucks.h"
#include <iostream>
#include <math.h>

class willartcStarbucks : public Starbucks {
private:


public:
	Entry* arr;
	int arrLength;

	willartcStarbucks();
	void build(Entry* c, int n);  // build calls insert
	Entry* getNearest(double x, double y);  // Calls search
	double getDistance(double x, double y, Entry* e);
};