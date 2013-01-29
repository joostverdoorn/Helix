/*
 * Node.h
 *
 *  Created on: Nov 27, 2012
 *      Author: joost
 */

#ifndef NODE_H_
#define NODE_H_

using namespace std;

#include <StandardCplusplus.h>
#include <list>
#include <vector>

class Bullet;

class Node {
protected:
	Node* left;
	Node* right;

	bool full;

	static uint8_t maxOccupants;

public:
	Node();
	virtual ~Node();

	virtual void ping() = 0;
	virtual bool isFull() = 0;

	virtual void setLeft(Node*);
	virtual void setRight(Node*);

	Node* getLeft();
	Node* getRight();

	bool hasLeft();
	bool hasRight();

	virtual void addOccupant(Bullet*);
};

#endif /* NODE_H_ */
