/*
 * Colour.cpp
 *
 *  Created on: Nov 26, 2012
 *      Author: joost
 */

#include <Arduino.h>
#include "Colour.h"

Colour::Colour(uint8_t r, uint8_t g, uint8_t b) {
	setRGB(r, g, b);
}

void Colour::setRGB(uint8_t r, uint8_t g, uint8_t b) {
	red = r;
	green = g;
	blue = b;
}

uint8_t Colour::getMagnitude() {
	return (red + green + blue) / 3;
}
