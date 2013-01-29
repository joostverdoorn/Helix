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

using namespace std;

//uint8_t Bullet::decay = 90;

Bullet::Bullet(Node *h, Main::Direction d) {
	currentNode = h;
	direction = d;

	currentNode->addOccupant(this);
	colour = Colour(254, 1, 254);
}

Bullet::Bullet(Node *h, Main::Direction d, Colour c) {
	currentNode = h;
	direction = d;

	currentNode->addOccupant(this);
	colour = c;
	colour.multiply(1.25);
}

Bullet::~Bullet() {

}

void Bullet::ping() {
	move();
	/*colour.multiply(.95);

	if(brightness() < 6) {
		die();
	}*/
}

void Bullet::die() {
	Main::removeBullet(this);
	delete this;
}

void Bullet::move() {
	if (direction == Main::LEFT && currentNode->hasLeft() && !currentNode->getLeft()->isFull()) {
		currentNode->getLeft()->addOccupant(this);
	} else if (direction == Main::RIGHT && currentNode->hasRight() && !currentNode->getRight()->isFull()) {
		currentNode->getRight()->addOccupant(this);
	}
}

Node* Bullet::getNode() {
	return currentNode;
}

void Bullet::setNode(Node *n) {
	currentNode = n;
}

Colour Bullet::getColour() {
	return colour;
}

Main::Direction Bullet::getDirection() {
	return direction;
}

uint8_t Bullet::brightness() {
	return colour.brightness();
}

