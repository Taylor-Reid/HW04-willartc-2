#include "willartcStarbucks.h"


willartcStarbucks::willartcStarbucks(){
	arrLength = 0;
	traverseIterator = 0;
	e = new Entry();
	e->identifier = "sentinel";
	e->x = 0.5;
	e->y = 0.5;
	sentinel = new Node(e);  // Never had a problem with sentinel
}

willartcStarbucks::~willartcStarbucks(){


}

int willartcStarbucks::traverse(Node* s){
	if(s == NULL){ return NULL;}
	if(s->left != NULL){
		traverseIterator = traverse(s->left);
	}
	if(s->right != NULL){
		traverseIterator = traverse(s->right);
	}
	if(s->up != NULL){
		traverseIterator = traverse(s->up);
	}
	if(s->down != NULL){
		traverseIterator = traverse(s->down);
	}
	traverseIterator++;
	return traverseIterator;
}

Node* willartcStarbucks::search(Node* n, double x, double y){
	// If there is no node
	if(n == NULL) {	return NULL; }
	/*
	// If this node is close enough
	if(checkSameLocation(x,y,n->e)){
		return n;
	}
	*/
	double shortestDistance = 5000;		//  Declare shortestDistance variable
	Node* nearestNode = NULL;
	if(((x) < (n->e->x))&&((y) >= (n->e->y))) {
		Node* temp = search(n->left, x, y);  // Left xY
		if(temp != NULL){
			int distance = dist(temp,x,y);
			if(distance < shortestDistance){
				shortestDistance = distance;
				nearestNode = temp;
			}
		}
	}
	if(((x) >= n->e->x)&&((y) < (n->e->y))) {
		Node* temp = search(n->right, x, y);  // Right Xy
		if(temp != NULL){
			int distance = dist(temp,x,y);
			if(distance < shortestDistance){
				shortestDistance = distance;
				nearestNode = temp;
			}
		}
	}
	if(((x) >= (n->e->x))&&((y) >= (n->e->y))) {
		Node* temp = search(n->up, x, y);  // Up XY
		if(temp != NULL){
			int distance = dist(temp,x,y);
			if(distance < shortestDistance){
				shortestDistance = distance;
				nearestNode = temp;
			}
		}
		
	} 
	if((x >= (n->e->x))&&(y >= (n->e->y))) {
		Node* temp = search(n->down, x, y);  // Down xy
		if(temp != NULL){
			int distance = dist(temp,x,y);
			if(distance < shortestDistance){
				shortestDistance = distance;
				nearestNode = temp;
			}
		}
	}
	return nearestNode;
}

Node* willartcStarbucks::insert(Entry* k, Node* n) {  // k is for key
	if(n == NULL) {
		return new Node(k);
	}
	/*
	if(checkSameLocation(k->x,k->y,n->e)){
		return n;
	}
	*/
	if(((k->x) < (n->e->x))&&((k->y) >= (n->e->y))) {
		n->left = insert(k, n->left);  // Left xY
		n->left->parent = n;
		return n->left;
		
	} else if(((k->x) >= n->e->x)&&((k->y) < (n->e->y))) {
		n->right = insert(k, n->right);  // Right Xy
		n->right->parent = n;
		return n->right;
		
	} else if(((k->x) >= (n->e->x))&&((k->y) >= (n->e->y))) {
		n->up = insert(k, n->up);  // Up XY
		n->up->parent = n;
		return n->up;
		
	} else if((k->x < n->e->x)&&(k->y < n->e->y)) { // Overkill.  Maybe even bad practice.
		n->down = insert(k, n->down);  // Down xy
		n->down->parent = n;
		return n->down;
	}
	
	return n;  //  Never gets called
}

void willartcStarbucks::build(Entry* c, int n){
	
	entryArr = new Entry[n];
	arrLength = n;  

	for(int i =0; i<arrLength;i++){
		(entryArr)[i] = *(new Entry(c[i]));
	}
	
	for(int q=0; q<arrLength; q++){
		insert(&entryArr[q],sentinel);
	}
}


Entry* willartcStarbucks::getNearest(double x, double y) {
	return search(sentinel, x,y)->e;
}

double willartcStarbucks::getDistance(double x, double y, Entry* e){
	return sqrt(((x - e->x)*(x - e->x))+((y - e->y)*(y - e->y)));
}

double willartcStarbucks::dist(Node* n, double x, double y){
	return getDistance(x, y, n->e);
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