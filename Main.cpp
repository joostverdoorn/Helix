/*
 * Main.cpp
 *
 *  Created on: Nov 27, 2012
 *      Author: joost
 */

#include "Main.h"
#include "Node.h"
#include "LedNode.h"
#include "VirtualNode.h"
#include "Bullet.h"
#include "CapacitiveSensor.h"
#include "TimerOne.h"
#include "MemoryFree.h"
#include "fix_fft.h"

extern HardwareSerial Serial;

namespace Main {

uint8_t nLeds = 12;

unsigned long lastSmallUpdate = millis();
unsigned long lastBigUpdate = millis();
unsigned long lastGiantUpdate = millis();
uint8_t ledDelay = 1;
uint8_t smallInterval = 1;
uint8_t bigInterval = 60;
uint16_t giantInterval = 500;

bool pushing = false;

uint16_t baudRate = 57600;
HardwareSerial *hardSerial = &Serial;
SoftwareSerial *softSerial = new SoftwareSerial(SoftRX, SoftTX);

list<Bullet*> bullets;
vector<LedNode*> nodes;

VirtualNode* leftNode;
VirtualNode* rightNode;

bool activated = false;
uint8_t leftActivated = 0;
uint8_t rightActivated = 0;

list<uint8_t> soundLevels;
CapacitiveSensor touchSensor = CapacitiveSensor(TouchOut, TouchIn);

Colour* activatedColour;
Colour* baseColour;

void begin() {
	hardSerial->begin(baudRate);
	softSerial->begin(baudRate);
	//logSerial->begin();

	pinMode(Button1, INPUT);
	pinMode(Button2, INPUT);
	pinMode(Status, OUTPUT);

	pinMode(LedData, OUTPUT);
	pinMode(LedClock, OUTPUT);

	leftNode = new VirtualNode(hardSerial);
	rightNode = new VirtualNode(softSerial);

	touchSensor.set_CS_AutocaL_Millis(0xFFFFFFFF);

	activatedColour = new Colour(10, 10, 10);
	baseColour = new Colour(0, 0, 0);

	// Create nodes for each led
	for (int i = 0; i < nLeds; i++) {
		nodes.push_back(new LedNode);

		if (i > 0) {
			nodes[i]->setLeft(nodes[i - 1]);
		}

	}

	// Set the neighbours for our left and right nodes
	nodes[0]->setLeft(leftNode);

	rightNode->setLeft(nodes[nLeds - 1]);

	Timer1.initialize(32);
	Timer1.attachInterrupt(pingStrip);
}

void ping() {
	if (lastSmallUpdate + smallInterval < millis()) {
		// Ping all LedNodes
		for (int i = 0; i < nodes.size(); i++) {
			nodes[i]->ping();
		}

		// Ping VirtualNodes
		leftNode->ping();
		rightNode->ping();

		// Determine soundlevels
		uint8_t level = abs(analogRead(A0) - 512) / 2;
		if (soundLevels.size() >= 5) {
			soundLevels.pop_front();
		}
		soundLevels.push_back(level);

		// Push Led colours to the strip
		pushStrip();

		lastSmallUpdate = millis();
	}

	if (lastBigUpdate + bigInterval < millis()) {
		// Ping every bullet
		for (list<Bullet*>::const_iterator iterator = bullets.begin(); iterator != bullets.end(); iterator++) {
			(*iterator)->ping();
		}

		// If this module is activated and soundlevels are above threshold, create a bullet
		if (activated) {
			uint8_t level = 0;
			for (list<uint8_t>::const_iterator iterator = soundLevels.begin(); iterator != soundLevels.end(); iterator++) {
				level += *iterator / soundLevels.size();
			}

			if (level > 60) {
				char data[128];

				for (uint8_t i = 0; i < 128; i++) {
					uint16_t val = analogRead(A0);  //This will fill the buffer with 128 samples in the form of instantaneous voltage

					data[i] = val / 4 - 128;      //This rescales data from 0-1023 to -128 to +128; required to be 8-bit char; and places in the CHAR data array
												  //note that rounding error is introduced by the scaling; the program rounds down, even negative numbers

				}

				fix_fftr(data, 7, 0);

				for (int i = 0; i < 64; i++) { //This will only return the absolute value of the transformation; the second 64 values are identical to the first.
					data[i] = sqrt((data[i] * data[i] * 2));  //convert data to RMS and place it back in data
				}

				uint8_t bands[8];

				uint8_t maxI = 0;
				uint8_t max = 0;

				for (uint8_t i = 0; i < 8; i++) {
					bands[i] = 0;
					for (uint8_t j = 0; j < 8; j++) {
						bands[i] += data[i + j + 2];
					}

					if (bands[i] > max) {
						max = bands[i];
						maxI = i;
					}
				}

				Colour c;

				switch (maxI) {
				case 0:
					c = Colour(254, 0, 0);
					break;
				case 1:
					c = Colour(254, 128, 0);
					break;
				case 2:
					c = Colour(128, 254, 0);
					break;
				case 3:
					c = Colour(0, 254, 0);
					break;
				case 4:
					c = Colour(0, 254, 254);
					break;
				case 5:
					c = Colour(0, 0, 254);
					break;
				case 6:
					c = Colour(128, 0, 254);
					break;
				case 7:
					c = Colour(254, 0, 128);
					break;
				}

				bullets.push_back(new Bullet(nodes[nodes.size() / 2 - 1], LEFT, c));
				bullets.push_back(new Bullet(nodes[nodes.size() / 2], RIGHT, c));
			}

		}
		//Serial.println(freeMemory());
		lastBigUpdate = millis();
	}

	if (lastGiantUpdate + giantInterval < millis()) {
		int level = touchSensor.capacitiveSensor(10);

		if (level > 300) {
			if (!activated) {
				activated = true;

				//unsigned char message[3] = { '~', 'A', rightActivated + 1 };
				//send(message, 3, LEFT);

				//message[2] = leftActivated + 1;
				//send(message, 3, RIGHT);
			}
		} else {
			if (activated) {
				activated = false;

				//unsigned char message[3] = { '~', 'A', rightActivated };
				//send(message, 3, LEFT);

				//message[2] = leftActivated;
				//send(message, 3, RIGHT);
			}
		}
		lastGiantUpdate = millis();

		//Serial.println(freeMemory());
		//Serial.println(bullets.size());
		//Serial.println(sizeof(Colour));
	}
}

void send(const unsigned char *message, uint8_t size, Direction d) {
	if (d == LEFT) {
		leftNode->send(message, size);
	} else {
		rightNode->send(message, size);
	}
}

void broadcast(const unsigned char* message, uint8_t size) {
	send(message, size, LEFT);
	send(message, size, RIGHT);
}

void slowShiftOut(uint8_t val) {
	for (uint8_t i = 0; i < 8; i++) {
		digitalWrite(LedData, !!(val & (1 << (7 - i))));
		delayMicroseconds(ledDelay);

		digitalWrite(LedClock, HIGH);
		delayMicroseconds(ledDelay);

		digitalWrite(LedClock, LOW);
		delayMicroseconds(ledDelay);
	}
}

void writeByte(uint8_t b) {
	slowShiftOut(b);
}

void writeGuard() {
	digitalWrite(LedClock, HIGH);
	digitalWrite(LedData, 0);
	delayMicroseconds(ledDelay);
	digitalWrite(LedClock, LOW);
	delayMicroseconds(ledDelay);

	writeByte(0xff);
	writeByte(0xff);
	writeByte(0xff);
}

void writeFrame() {
	writeByte(0); 	//G
	writeByte(0);	//B
	writeByte(0);	//R
}

void writeFrame(LedNode* u) {
	Colour c = u->getColour();

	writeByte(c.green); 	//G
	writeByte(c.blue);	//B
	writeByte(c.red);	//R
}

void pushStrip() {
	pushing = true;
	for (int i = 49; i >= 0; i--) {
		if (i < nLeds) {
			writeFrame(nodes[i]);
		} else {
			writeFrame();
		}
	}

	writeGuard();
	pushing = false;
}

void pingStrip() {
	if (!pushing) {
		digitalWrite(LedClock, HIGH);
		delayMicroseconds(ledDelay);

		digitalWrite(LedClock, LOW);
		delayMicroseconds(ledDelay);
	}
}

int freeRam() {
	int __heap_start, *__brkval;
	int v;
	return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
}
