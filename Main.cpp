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

extern HardwareSerial Serial;

uint8_t Main::nLeds = 12;

unsigned long Main::lastSmallUpdate = millis();
unsigned long Main::lastBigUpdate = millis();
unsigned long Main::lastGiantUpdate = millis();
uint8_t Main::ledDelay = 1;
uint8_t Main::smallInterval = 1;
uint8_t Main::bigInterval = 60;
uint16_t Main::giantInterval = 500;

bool Main::pushing = false;

uint16_t Main::baudRate = 4800;
HardwareSerial *Main::hardSerial = &Serial;
SoftwareSerial *Main::softSerial = new SoftwareSerial(SoftRX, SoftTX);

//TwoWire *Main::logSerial = &Wire;
//int Main::logAddress = 48;

list<Bullet*> Main::bullets;
vector<LedNode*> Main::nodes;

VirtualNode* Main::leftNode;
VirtualNode* Main::rightNode;

bool Main::activated = false;
uint8_t Main::leftActivated = 0;
uint8_t Main::rightActivated = 0;

list<uint8_t> Main::soundLevels;
CapacitiveSensor Main::touchSensor = CapacitiveSensor(TouchOut, TouchIn);

Colour* Main::activatedColour;
Colour* Main::baseColour;

/*void Main::log(const char* message) {
 //logSerial->beginTransmission(logAddress);
 //logSerial->println(message);
 //logSerial->endTransmission();
 }*/

Main::Main() {
}

Main::~Main() {
}

void Main::begin() {
	Serial.println(freeRam());
	hardSerial->begin(baudRate);
	softSerial->begin(baudRate);
	//logSerial->begin();
	Serial.println(freeRam());

	pinMode(Button1, INPUT);
	pinMode(Button2, INPUT);
	pinMode(Status, OUTPUT);

	pinMode(LedData, OUTPUT);
	pinMode(LedClock, OUTPUT);

	leftNode = new VirtualNode(hardSerial);
	rightNode = new VirtualNode(softSerial);

	touchSensor.set_CS_AutocaL_Millis(0xFFFFFFFF);

	activatedColour = new Colour(0, 30, 40);
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

void Main::ping() {
	// Hammer the clock
	//pingStrip();

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
		if(soundLevels.size() >= 5) {
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

			Serial.println(level);

			if(level > 60) {
				bullets.push_back(new Bullet(nodes[nodes.size() / 2 - 1], LEFT));
				bullets.push_back(new Bullet(nodes[nodes.size() / 2 ], RIGHT));
			}

		}

		//Serial.println("blaat");

		lastBigUpdate = millis();
	}

	if (lastGiantUpdate + giantInterval < millis()) {
		//Serial.println(touchSensor.capacitiveSensor(10));

		//Serial.println("Wadup");
		if (touchSensor.capacitiveSensor(10) > 300) {
			if (!activated) {
				activated = true;
			}
		} else {
			if (activated) {
				activated = false;
			}
		}
		lastGiantUpdate = millis();
	}
}

void Main::send(String message, Direction d) {
	if (d == LEFT) {
		leftNode->send(message);
	} else {
		rightNode->send(message);
	}
}

void Main::broadcast(String message) {
	send(message, LEFT);
	send(message, RIGHT);
}

void Main::slowShiftOut(uint8_t val) {
	for (uint8_t i = 0; i < 8; i++) {
		digitalWrite(LedData, !!(val & (1 << (7 - i))));
		delayMicroseconds(ledDelay);

		digitalWrite(LedClock, HIGH);
		delayMicroseconds(ledDelay);

		digitalWrite(LedClock, LOW);
		delayMicroseconds(ledDelay);
	}
}

void Main::writeByte(uint8_t b) {
	slowShiftOut(b);
}

void Main::writeGuard() {
	digitalWrite(LedClock, HIGH);
	digitalWrite(LedData, 0);
	delayMicroseconds(ledDelay);
	digitalWrite(LedClock, LOW);
	delayMicroseconds(ledDelay);

	writeByte(0xff);
	writeByte(0xff);
	writeByte(0xff);
}

void Main::writeFrame() {
	writeByte(0); 	//G
	writeByte(0);	//B
	writeByte(0);	//R
}

void Main::writeFrame(LedNode* u) {
	Colour* c = u->getColour();

	writeByte(c->green); 	//G
	writeByte(c->blue);	//B
	writeByte(c->red);	//R
}

void Main::pushStrip() {
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

void Main::pingStrip() {
	if(!pushing) {
		digitalWrite(LedClock, HIGH);
		delayMicroseconds(ledDelay);

		digitalWrite(LedClock, LOW);
		delayMicroseconds(ledDelay);
	}

	//Serial.println("bla");
}

int Main::freeRam() {
	extern int __heap_start, *__brkval;
	int v;
	return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
