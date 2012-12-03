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
	float diff = targetBrightness - brightness;

	if (targetBrightness > brightness) {
		brightness += diff / 40;
	} else if (targetBrightness < brightness) {
		brightness += diff / 100;
	}

	/*if (targetBrightness > brightness) {
	 if (brightness < 1) {
	 brightness = 1;
	 }


	 brightness *= 1.05;

	 if (brightness > 255) {
	 brightness = 255;
	 }
	 } else if (targetBrightness < brightness) {
	 brightness /= 1.01;

	 if (brightness < 1) {
	 brightness = 0;
	 }

	 if (brightness < 0) {
	 brightness = 0;
	 }
	 }*/

	analogWrite(ledPin, brightness);
}

void LedNode::ping() {

	float b = 0;

	if (Main::activated || Main::leftActivated > 0 || Main::rightActivated > 0) {
		digitalWrite(13, HIGH);
	} else {
		digitalWrite(13, LOW);
	}

	if ((Main::activated && Main::leftActivated > 0) || (Main::activated && Main::rightActivated > 0)
			|| (Main::leftActivated > 0 && Main::rightActivated > 0)) {
		b += 30;
	}

	for (list<Bullet*>::const_iterator iterator = occupants.begin(); iterator != occupants.end(); iterator++) {
		b += (*iterator)->getMagnitude();
	}

	if (b > 255) {
		b = 255;
	}

	targetBrightness = b;

	emit();
}

bool LedNode::isFull() {
	if (maxOccupants == 0 || maxOccupants >= occupants.size() + 1) {
		return false;
	} else {
		return true;
	}
}

