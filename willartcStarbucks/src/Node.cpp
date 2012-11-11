
#include "Node.h"

Node::Node(){
	left = NULL;
	right = NULL;
	up = NULL;
	down = NULL;
	parent = NULL;
	e = NULL;
}

Node::Node(Entry* nodeEntry){
	e = nodeEntry;
	left = NULL;
	right = NULL;
	up = NULL;
	down = NULL;
	parent = NULL;
}