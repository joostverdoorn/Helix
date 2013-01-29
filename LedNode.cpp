/*
 * LedNode.cpp
 *
 *  Created on: Nov 26, 2012
 *      Author: joost
 */

#include "LedNode.h"
#include <Arduino.h>

LedNode::LedNode() {
	brightness = 0;
	colour = Main::baseColour;
}

LedNode::~LedNode() {
}

void LedNode::ping() {
	list<Bullet*> occupants = getOccupants();
	//Serial.println(":Ping");

	if (occupants.size() == maxOccupants && maxOccupants != 0) {
		full = true;
	} else {
		full = false;
	}

	if (Main::activated) {
		colour.approach(Main::activatedColour);
	} else if (occupants.size() > 0) {
		Colour c = Colour(occupants.front()->getColour());
		for (list<Bullet*>::iterator it = occupants.begin(); it != occupants.end(); it++) {
			c.average((*it)->getColour());
		}

		colour.quickApproach(c);

	} else if (Main::leftActivated > 0 && Main::rightActivated > 0) {
		colour.approach(Main::pathColour);
	} else {
		colour.approach(Main::baseColour);
	}
}

bool LedNode::isFull() {
	return full;
}

uint8_t LedNode::getBrightness() {
	return brightness;
}

Colour LedNode::getColour() {
	return colour;
}

void LedNode::setColour(Colour c) {
	colour = c;
}

void LedNode::addOccupant(Bullet *b) {
	b->setNode(this);
}

list<Bullet*> LedNode::getOccupants() {
	list<Bullet*> bs;

	for (int i = 0; i < Main::bullets.size(); i++) {
		if (Main::bullets[i]->getNode() == this) {
			bs.push_back(Main::bullets[i]);
		}
	}

	return bs;
}

