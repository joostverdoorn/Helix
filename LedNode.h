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
	Colour* colour;

public:
	LedNode(int);
	LedNode();
	virtual ~LedNode();

	void ping();
	bool isFull();

	Colour* getColour();
	uint8_t getBrightness();
};

#endif /* LEDNODE_H_ */
