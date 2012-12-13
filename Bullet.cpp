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

uint8_t Bullet::decay = 90;

Bullet::Bullet(Node *h, Main::Direction d) {
	currentNode = h;
	direction = d;

	currentNode->addOccupant(this);
	colour = new Colour(254, 0, 254);
}

Bullet::Bullet(Node *h, Main::Direction d, Colour* c) {
	currentNode = h;
	direction = d;

	currentNode->addOccupant(this);
	colour = c;
}

Bullet::~Bullet() {

}

void Bullet::ping() {
	// Decay a little
	//uint8_t red = (float) colour->red * ((float) decay / 100.0F);
	//uint8_t green = (float) colour->red * ((float) decay / 100.0F);
	//uint8_t blue = (float) colour->red * ((float) decay / 100.0F);

	//colour->setRGB(red, green, blue);

	if (getMagnitude() < 3) {
		die();
	} else {
		move();
	}
}

void Bullet::die() {
	Main::bullets.remove(this);
	currentNode->removeOccupant(this);

	delete colour;

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

Colour* Bullet::getColour() {
	return colour;
}

uint8_t Bullet::getMagnitude() {
	return colour->getMagnitude();
}

