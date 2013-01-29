/*
 * Colour.h
 *
 *  Created on: Nov 26, 2012
 *      Author: joost
 */

#ifndef COLOUR_H_
#define COLOUR_H_

#include <Arduino.h>

class Colour {

public:
	Colour(uint8_t, uint8_t, uint8_t);
	Colour();
	void setRGB(uint8_t, uint8_t, uint8_t);
	void approach(Colour);
	void quickApproach(Colour);
	void multiply(float);
	void average(Colour);
	uint8_t brightness();

	uint8_t red;
	uint8_t green;
	uint8_t blue;
};

#endif /* COLOUR_H_ */
