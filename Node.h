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

	static int maxOccupants;
	list<Bullet*> occupants;

public:
	Node();
	virtual ~Node();

	virtual void ping() = 0;
	virtual bool isAvailable() = 0;

	virtual void setLeft(Node*);
	virtual void setRight(Node*);

	Node* getLeft();
	Node* getRight();

	bool hasLeft();
	bool hasRight();

	virtual void addOccupant(Bullet*);
	void removeOccupant(Bullet*);

	bool isEmpty();
};

#endif /* NODE_H_ */
