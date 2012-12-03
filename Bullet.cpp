/*
 * Bullet.cpp
 *
 *  Created on: Nov 26, 2012
 *      Author: joost
 */

#include <Arduino.h>
#include <StandardCplusplus.h>
#include <vector>
#include "Bullet.h"

Bullet::Bullet(Node *h, Main::Direction d, uint8_t m) {
	currentNode = h;
	direction = d;
	magnitude = m;

	decay = .9;

	currentNode->addOccupant(this);
}

Bullet::~Bullet() {

}

void Bullet::ping() {
	magnitude *= decay;

	if (magnitude < 1) {
		die();
	} else {
		move();
	}
}

void Bullet::die() {
	Main::bullets.remove(this);
	currentNode->removeOccupant(this);

	delete this;
}

void Bullet::move() {
	if (direction == Main::LEFT && currentNode->hasLeft() && !currentNode->getLeft()->isFull()) {
		currentNode->removeOccupant(this);
		currentNode = currentNode->getLeft();
		currentNode->addOccupant(this);
	} else if (direction == Main::RIGHT && currentNode->hasRight() && !currentNode->getRight()->isFull()) {
		currentNode->removeOccupant(this);
		currentNode = currentNode->getRight();
		currentNode->addOccupant(this);
	}
}

int Bullet::getMagnitude() {
	return magnitude;
}

