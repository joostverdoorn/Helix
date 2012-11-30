/*
 * LedNode.cpp
 *
 *  Created on: Nov 26, 2012
 *      Author: joost
 */

#include "LedNode.h"
#include <Arduino.h>

LedNode::LedNode(int p) {
	ledPin = p;
	pinMode(ledPin, OUTPUT);

	brightness = 0;
	deltaBrightness = .5;
	targetBrightness = 0;
}

LedNode::~LedNode() {
}

void LedNode::emit() {
	if(targetBrightness > brightness) {
		brightness += 4 * deltaBrightness;
	} else if(targetBrightness < brightness) {
		brightness -= deltaBrightness;
	}

	analogWrite(ledPin, (uint8_t) brightness);
}

void LedNode::ping() {
	if (isEmpty()) {
		targetBrightness = 0;

	} else {
		//int targetBrightness = 0;

		float b = 0;

		for (list<Bullet*>::const_iterator iterator = occupants.begin();
				iterator != occupants.end(); iterator++) {
			b += (*iterator)->getMagnitude();
		}

		if (b > 255) {
			b = 255;
		}

		targetBrightness = b;
	}

	emit();
}

bool LedNode::available() {
	if (maxOccupants == 0 || maxOccupants >= occupants.size() + 1) {
		return true;
	} else {
		return false;
	}
}

