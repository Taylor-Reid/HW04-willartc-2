#include "Starbucks.h"

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
