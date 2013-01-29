/*
 * LedNode.h
 *
 *  Created on: Nov 26, 2012
 *      Author: joost
 */

#ifndef LEDNODE_H_
#define LEDNODE_H_

#include "Main.h"s
#include "Node.h"
#include "Bullet.h"

using namespace std;

class Bullet;

class LedNode: public Node {
	float brightness;
	Colour colour;

	//vector<char> input = vector<char>(2);

public:
	LedNode();
	virtual ~LedNode();

	void ping();
	bool isFull();

	Colour getColour();
	void setColour(Colour);
	uint8_t getBrightness();

	void addOccupant(Bullet*);
	list<Bullet*> getOccupants();
};

#endif /* LEDNODE_H_ */
