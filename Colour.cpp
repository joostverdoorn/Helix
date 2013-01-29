/*
 * Colour.cpp
 *
 *  Created on: Nov 26, 2012
 *      Author: joost
 */

#include <Arduino.h>
#include "Colour.h"

Colour::Colour() {
	setRGB(0, 0, 0);
}

Colour::Colour(uint8_t r, uint8_t g, uint8_t b) {
	setRGB(r, g, b);
}

void Colour::setRGB(uint8_t r, uint8_t g, uint8_t b) {
	red = r;
	green = g;
	blue = b;
}

uint8_t Colour::brightness() {
	return (red + green + blue) / 3;
}

void Colour::approach(Colour c) {
	if (abs(c.red - red) <= 2 && abs(c.green - green) <= 2 && abs(c.blue - blue) <= 2) {
		red = c.red;
		green = c.green;
		blue = c.blue;

		return;
	}

	if (c.brightness() > brightness()) {
		red = (c.red + red * 2) / 3;
		green = (c.green + green * 2) / 3;
		blue = (c.blue + blue * 2) / 3;
	} else {
		red = (c.red + red * 2) / 3;
		green = (c.green + green * 2) / 3;
		blue = (c.blue + blue * 2) / 3;
	}
}

void Colour::quickApproach(Colour c) {
	for (uint8_t i = 0; i < 3; i++) {
		approach(c);
	}
}

void Colour::average(Colour c) {
	red = (red + c.red) / 2;
	green = (green + c.green) / 2;
	blue = (blue + c.blue) / 2;
}

void Colour::multiply(float factor) {
	uint16_t r, g, b;
	r = (float) red * factor;
	g = (float) green * factor;
	b = (float) blue * factor;

	red = r > 255 ? 255 : r;
	green = g > 255 ? 255 : g;
	blue = b > 255 ? 255 : b;
}
