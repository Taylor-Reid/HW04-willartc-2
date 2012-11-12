/*
A class for the original quad tree data structure, which failed and was 
replaced by an unsorted array.
*/
#include "Starbucks.h"
#include "CensusEntry.h"

class Node {
public:
	Entry* e;
	Node* left;
	Node* right;
	Node* up;
	Node* down;
	Node* parent;
	
	Node();
	Node(Entry* nodeEntry);
	~Node();
};
