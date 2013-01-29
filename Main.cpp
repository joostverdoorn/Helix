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
unsigned long lastColourUpdate = millis();
unsigned long lastActivation = 0;
uint8_t ledDelay = 1;
uint8_t smallInterval = 1;
uint8_t bigInterval = 80;
uint16_t giantInterval = 200;
uint16_t colourInterval = 60000;
uint16_t activationInterval = 10000;

bool pushing = false;

uint16_t baudRate = 19200;
HardwareSerial *hardSerial = &Serial;
SoftwareSerial *softSerial = new SoftwareSerial(SoftRX, SoftTX);

vector<Bullet*> bullets;
vector<LedNode*> nodes;

VirtualNode* leftNode;
VirtualNode* rightNode;

bool activated = false;
uint8_t leftActivated = 0;
uint8_t rightActivated = 0;
bool soundActive = false;
Colour soundColour = Colour(254, 0, 0);

vector<uint8_t> soundLevels;
CapacitiveSensor touchSensor = CapacitiveSensor(TouchOut, TouchIn);

Colour activatedColour = Colour(254, 254, 0);
Colour pathColour = Colour(10, 10, 20);
Colour baseColour = Colour(0, 0, 0);
Colour debugColour = Colour(254, 0, 0);

uint16_t avgTouch;
uint16_t maxTouch;

uint8_t experience = 0;

uint16_t id = 0;
bool hasLeftNeighbour = false;
bool hasRightNeighbour = false;

bool isHead = false;

uint8_t pulseThreshold = 50;
uint8_t slumberThreshold = 30;

Colour colours[7] = { Colour(255, 0, 0), Colour(255, 80, 0), Colour(255, 220, 0), Colour(80, 255, 60), Colour(10, 184, 255), Colour(0, 90, 255), Colour(220, 0,
		240) };

void begin() {
	// Set corrent pinmodes
	pinMode(LedData, OUTPUT);
	pinMode(LedClock, OUTPUT);

	pinMode(13, OUTPUT);

	// Init left and right streams
	hardSerial->begin(baudRate);
	softSerial->begin(baudRate);

	// Create VirtualNodes for streams
	rightNode = new VirtualNode(hardSerial);
	leftNode = new VirtualNode(softSerial);

	touchSensor.set_CS_AutocaL_Millis(0xFFFFFFFF);

	// Create nodes for each led
	for (int i = 0; i < nLeds; i++) {
		nodes.push_back(new LedNode);

		if (i > 0) {
			nodes[i]->setLeft(nodes[i - 1]);
		}

		nodes[i]->setColour(debugColour); // Make 'm all red
	}

	// Push colours to the strip
	pushStrip();

	// Set the neighbours for our left and right nodes
	nodes[0]->setLeft(leftNode);
	rightNode->setLeft(nodes[nLeds - 1]);

	// Attach a timed interrupt to call pingStrip every 32 microseconds. This is required to drive the PWN for the leds.
	Timer1.initialize(32);
	Timer1.attachInterrupt(pingStrip);

	// Calibration time!
	uint16_t samples = 0;
	avgTouch = 0;
	maxTouch = 0;

	bool hasExplored = false;
	bool hasAnnounced = false;

	while (millis() < 10000) {
		/*if (millis() > 4000 && !hasExplored) {
		 hasExplored = true;

		 vector<char> message;
		 message.push_back('X');
		 broadcast(message);
		 }

		 if (millis() > 9000 && !hasAnnounced && !hasLeftNeighbour) {
		 hasAnnounced = true;

		 id = 0;
		 activatedColour = colours[id % 7];

		 vector<char> message;
		 message.push_back('Y');
		 message.push_back((char) 0);
		 send(message, RIGHT);
		 }*/

		if (millis() > 4000 && !hasExplored) {
			hasExplored = true;

			vector<char> message;
			message.push_back('X');
			send(message, LEFT);
		}

		uint16_t level = touchSensor.capacitiveSensor(50);
		avgTouch = ((uint32_t) avgTouch * (uint32_t) samples + (uint32_t) level) / (uint32_t) (samples + 1);
		maxTouch = level > maxTouch ? level : maxTouch;

		//Serial.println(level);

		samples++;
	}

	if (id == 0) {
		isHead = true;
	}

	activatedColour = colours[id % 7];

	Serial.print("avg: ");
	Serial.println(avgTouch);

	Serial.print("max: ");
	Serial.println(maxTouch);
	//activatedColour = colours[3];
}

void ping() {
	if (lastSmallUpdate + smallInterval < millis()) {
		smallPing();
		lastSmallUpdate = millis();
	}

	if (lastBigUpdate + bigInterval < millis()) {
		bigPing();
		lastBigUpdate = millis();
	}

	if (lastGiantUpdate + giantInterval < millis()) {
		giantPing();
		lastGiantUpdate = millis();
	}

	if (lastColourUpdate + colourInterval < millis()) {
		id++;
		if (!activated) {
			activatedColour = colours[id % 7];
		}
		lastColourUpdate = millis();
	}
}

void smallPing() {
	// Ping all LedNodes
	for (int i = 0; i < nodes.size(); i++) {
		nodes[i]->ping();
	}

	// Ping VirtualNodes
	leftNode->ping();
	rightNode->ping();

	// Determine soundlevels
	uint8_t level = abs(analogRead(A0) - 512) / 2;
	if (soundLevels.size() >= 16) {
		soundLevels.erase(soundLevels.begin());
	}
	soundLevels.push_back(level);

	// Slumber mode light effects
	// For this we need two values: a threshold value, from which point the strip will start emiting light,
	// and a maximum value, at which point the led strip will not get any brighter
	if (!activated && leftActivated == 0 && rightActivated == 0 && (lastActivation + activationInterval < millis() || lastActivation == 0)) {
		uint8_t level = 0;
		for (int i = soundLevels.size() - 4; i < soundLevels.size(); i++) {
			level += soundLevels[i] / 4;
		}

		level = level < slumberThreshold ? 0 : level - slumberThreshold;

		baseColour = activatedColour;
		baseColour.multiply((float) level / ((float) slumberThreshold * 4.0F) + 0.05F);
	} else {
		baseColour = Colour(0, 0, 0);
	}

	// Push Led colours to the strip
	pushStrip();
}

void bigPing() {
	// Ping every bullet
	uint8_t level = 0;
	for (int i = 0; i < soundLevels.size(); i++) {
		level += soundLevels[i] / soundLevels.size();
	}

	//Serial.print("Sound: ");
	//Serial.println(level);

	// If this module is activated and soundlevels are above threshold, create a bullet
	if (activated) {

		if (level > pulseThreshold) {
			addBullet(new Bullet(nodes[0], LEFT, activatedColour));
			addBullet(new Bullet(nodes[nodes.size() - 1], RIGHT, activatedColour));
		}
	}

	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->ping();
	}
}

void giantPing() {
	//Serial.println(freeMemory());
	int level = touchSensor.capacitiveSensor(50);

	//Serial.print("Touch: ");
	//Serial.println(level);
	if (activated || leftActivated > 0 || rightActivated > 0) {
		lastActivation = millis();
	}

	if (level > maxTouch * 3 + avgTouch) {
		if (!activated) {
			activated = true;

			vector<char> message;

			message.push_back('A');
			message.push_back((char) rightActivated + 1);
			send(message, LEFT);

			message.clear();
			message.push_back('A');
			message.push_back((char) leftActivated + 1);
			send(message, RIGHT);

			message.clear();
			message.push_back('F');
			message.push_back((char) 1);
			broadcast(message);
		}
	} else {
		if (activated) {
			activated = false;

			vector<char> message;

			message.push_back('A');
			message.push_back((char) rightActivated);
			send(message, LEFT);

			message.clear();
			message.push_back('A');
			message.push_back((char) leftActivated);
			send(message, RIGHT);

			message.clear();
			message.push_back('F');
			message.push_back((char) 0);
			broadcast(message);

			activatedColour = colours[id % 7];

		}
	}
}

void addBullet(Bullet *b) {
	if (b->getDirection() == LEFT) {
		if (leftNode->isFull()) {
			b->die();
			return;
		}
	} else if (b->getDirection() == RIGHT) {
		if (rightNode->isFull()) {
			b->die();
			return;
		}
	}

	if (bullets.size() > 20) {
		uint8_t minBrightness = 255;
		Bullet* minBullet;

		for (uint8_t i = 0; i < bullets.size(); i++) {
			if (bullets[i]->brightness() < minBrightness) {
				minBullet = bullets[i];
				minBrightness = minBullet->brightness();
			}
		}

		minBullet->die();
	}

	bullets.push_back(b);
}

void removeBullet(Bullet *b) {
	for (vector<Bullet*>::iterator iterator = bullets.begin(); iterator != bullets.end(); iterator++) {
		if (*iterator == b) {
			bullets.erase(iterator);
			break;
		}
	}
}

void send(vector<char> message, Direction d) {
	if (d == LEFT) {
		leftNode->send(message);
	} else {
		rightNode->send(message);
	}
}

void broadcast(vector<char> message) {
	send(message, LEFT);
	send(message, RIGHT);
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

	writeByte(c.green % 2 == 0 ? c.green : c.green - 1); 	//G
	writeByte(c.blue % 2 == 0 ? c.blue : c.blue - 1);		//B
	writeByte(c.red % 2 == 0 ? c.red : c.red - 1);			//R
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

Colour HSVtoColour(float h, float s, float v) {
	int i;
	float f, p, q, t;
	float r, g, b;
	if (s == 0) {
		// achromatic (grey)
		return Colour(v * 255, v * 255, v * 255);
	}
	h /= 60;			// sector 0 to 5
	i = floor(h);
	f = h - i;			// factorial part of h
	p = v * (1 - s);
	q = v * (1 - s * f);
	t = v * (1 - s * (1 - f));
	switch (i) {
	case 0:
		r = v;
		g = t;
		b = p;
		break;
	case 1:
		r = q;
		g = v;
		b = p;
		break;
	case 2:
		r = p;
		g = v;
		b = t;
		break;
	case 3:
		r = p;
		g = q;
		b = v;
		break;
	case 4:
		r = t;
		g = p;
		b = v;
		break;
	default:		// case 5:
		r = v;
		g = p;
		b = q;
		break;
	}

	return Colour(r * 255, g * 255, b * 255);
}

int freeRam() {
	int __heap_start, *__brkval;
	int v;
	return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
}
