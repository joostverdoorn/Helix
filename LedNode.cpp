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
	colour = *Main::baseColour;
}

LedNode::~LedNode() {
}

void LedNode::ping() {
	/*float targetBrightness = 0, diff = 0;

	if (Main::activated || (Main::activated && Main::rightActivated > 0) || (Main::leftActivated > 0 && Main::rightActivated > 0)) {
		targetBrightness += 30;
	}

	for (list<Bullet*>::const_iterator iterator = occupants.begin(); iterator != occupants.end(); iterator++) {
		targetBrightness += (*iterator)->getMagnitude();
	}

	if (targetBrightness > 254) {
		targetBrightness = 254;
	}

	diff = targetBrightness - brightness;

	if (targetBrightness > brightness) {
		brightness += diff / 5;
	} else if (targetBrightness < brightness) {
		brightness += diff / 15;
	}*/

	if(!isEmpty()) {
		colour = occupants.front()->getColour();
	} else if(Main::activated || (Main::leftActivated > 0 && Main::rightActivated > 0)) {
		colour = *Main::activatedColour;
	} else {
		colour = *Main::baseColour;
	}
}

bool LedNode::isFull() {
	if (maxOccupants == 0 || maxOccupants >= occupants.size() + 1) {
		return false;
	} else {
		return true;
	}
}

uint8_t LedNode::getBrightness() {
	return brightness;
}

Colour LedNode::getColour() {
	return colour;
}

