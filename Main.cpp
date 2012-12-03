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

extern HardwareSerial Serial;

int Main::leds[] = { 3, 5, 6, 9, 10, 11 };
int Main::nLeds = sizeof(leds) / sizeof(int);

unsigned long Main::lastSmallUpdate = millis();
unsigned long Main::lastBigUpdate = millis();
int Main::smallInterval = 1;
int Main::bigInterval = 100;

int Main::timeout = 5;

long Main::baudRate = 4800;
HardwareSerial *Main::hardSerial = &Serial;
SoftwareSerial *Main::softSerial = new SoftwareSerial(SoftRX, SoftTX);

TwoWire *Main::logSerial = &Wire;
int Main::logAddress = 48;

list<Bullet*> Main::bullets;
vector<Node*> Main::nodes;

VirtualNode* Main::leftNode;
VirtualNode* Main::rightNode;

bool Main::activated = false;
int Main::leftActivated = 0;
int Main::rightActivated = 0;

Main::Main() {
}

Main::~Main() {
}

void Main::begin() {
	hardSerial->begin(baudRate);
	softSerial->begin(baudRate);
	logSerial->begin();

	pinMode(Button1, INPUT);
	pinMode(Button2, INPUT);
	pinMode(Status, OUTPUT);

	leftNode = new VirtualNode(hardSerial);
	rightNode = new VirtualNode(softSerial);

	// Create nodes for each led
	for (int i = 0; i <= nLeds + 1; i++) {
		if (i == 0) {
			nodes.push_back(leftNode);
		} else if (i == nLeds + 1) {
			nodes.push_back(rightNode);
		} else {
			pinMode(leds[i - 1], OUTPUT);
			nodes.push_back(new LedNode(leds[i - 1]));
		}

		if (i > 0) {
			nodes[i]->setLeft(nodes[i - 1]);
		}
	}
}

void Main::ping() {
	if (lastSmallUpdate + smallInterval < millis()) {
		for (int i = 0; i < nodes.size(); i++) {
			nodes[i]->ping();
		}

		lastSmallUpdate = millis();

		if (digitalRead(Button1)) {
			if (!activated) {
				//String message = "~A";
				//message.concat((char) 1);
				broadcast("~A1");
				activated = true;
			}
		} else {
			if (activated) {
				//String message = "~A";
				//message.concat((char) 0);
				broadcast("~A0");
				activated = false;
			}
		}

	}

	if (lastBigUpdate + bigInterval < millis()) {
		for (list<Bullet*>::const_iterator iterator = bullets.begin(); iterator != bullets.end(); iterator++) {
			(*iterator)->ping();
		}

		if (digitalRead(Button2) == HIGH) {
			bullets.push_back(new Bullet(nodes[3], LEFT, 255));
			bullets.push_back(new Bullet(nodes[4], RIGHT, 255));
		}

		lastBigUpdate = millis();
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

void Main::log(const char* message) {
	logSerial->beginTransmission(logAddress);
	logSerial->println(message);
	logSerial->endTransmission();
}

int Main::freeRam() {
	extern int __heap_start, *__brkval;
	int v;
	return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
