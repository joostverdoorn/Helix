/*
 * Bullet.h
 *
 *  Created on: Nov 26, 2012
 *      Author: joost
 */

#ifndef BULLET_H_
#define BULLET_H_

#include <StandardCplusplus.h>
#include <vector>
#include <list>

#include "Main.h"
#include "Node.h"
#include "Colour.h"

using namespace std;

class Node;

class Bullet {
private:
	Node* currentNode;
	Main::Direction direction;

	Colour colour;

	void move();

	static uint8_t decay; // is a percentage.. less memory than a float!

public:
	Bullet(Node*, Main::Direction);
	Bullet(Node*, Main::Direction, Colour);
	virtual ~Bullet();

	void ping();
	void die();

	uint8_t getMagnitude();
	Colour getColour();
};

#endif /* BULLET_H_ */
