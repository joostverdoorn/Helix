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

	if(magnitude < 5) {
		die();
	}

	move();
}

void Bullet::die() {
	currentNode->removeOccupant(this);
	delete this;
}

void Bullet::move() {
	if (direction == Main::LEFT && currentNode->hasLeft() && currentNode->getLeft()->isAvailable()) {
		currentNode->removeOccupant(this);
		currentNode = currentNode->getLeft();
		currentNode->addOccupant(this);
	} else if (direction == Main::RIGHT && currentNode->hasRight() && currentNode->getRight()->isAvailable()) {
		currentNode->removeOccupant(this);
		currentNode = currentNode->getRight();
		currentNode->addOccupant(this);
	}
}

int Bullet::getMagnitude() {
	return magnitude;
}

