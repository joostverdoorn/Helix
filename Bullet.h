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
	uint8_t magnitude;
	float decay;

	void move();

public:
	Bullet(Node*, Main::Direction, uint8_t);
	virtual ~Bullet();

	void ping();
	void die();

	int getMagnitude();
};

#endif /* BULLET_H_ */
