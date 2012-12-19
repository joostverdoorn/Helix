/*
 * Main.h
 *
 *  Created on: Nov 27, 2012
 *      Author: joost
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <HardwareSerial.h>
#include "CapacitiveSensor.h"

#include <StandardCplusplus.h>
#include <vector>
#include <list>

class Bullet;
class Node;
class VirtualNode;
class LedNode;
class CapacitiveSensor;
class Colour;

namespace Main {
typedef enum Direction {
	LEFT, RIGHT
} Direction;

typedef enum Pins {
	SoftRX = 2, SoftTX = 3,

	Button1 = 8, Button2 = 9,

	Status = 13,

	LedData = 4, LedClock = 5, TouchOut = 6, TouchIn = 7,

} Pins;

extern void begin();
extern void ping();

static void log();
static int freeRam();

extern void send(const unsigned char*, uint8_t, Direction);
static void broadcast(const unsigned char*, uint8_t);

static void slowShiftOut(uint8_t);
static void writeByte(uint8_t);
static void writeGuard();
static void writeFrame(LedNode*);
static void writeFrame();
static void pushStrip();
static void pingStrip();

extern std::list<Bullet*> bullets;
extern std::vector<LedNode*> nodes;

extern bool activated;
extern uint8_t leftActivated;
extern uint8_t rightActivated;

extern Colour* activatedColour;
extern Colour* baseColour;

//static
/*static uint16_t baudRate;
 static SoftwareSerial *softSerial;
 static HardwareSerial *hardSerial;

 static unsigned long lastSmallUpdate;
 static unsigned long lastBigUpdate;
 static unsigned long lastGiantUpdate;
 static uint8_t ledDelay;
 static uint8_t smallInterval;
 static uint8_t bigInterval;
 static uint16_t giantInterval;

 static bool pushing;

 static uint8_t nLeds;



 static VirtualNode* leftNode;
 static VirtualNode* rightNode;



 static std::list<uint8_t> soundLevels;
 static CapacitiveSensor touchSensor;

 ;/**/
}

#endif /* MAIN_H_ */
