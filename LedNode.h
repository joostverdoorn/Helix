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
	int ledPin;

	float brightness;
	float deltaBrightness;
	float targetBrightness;

public:
	LedNode(int);
	virtual ~LedNode();

	void ping();
	bool available();

	void emit();
};

#endif /* LEDNODE_H_ */
