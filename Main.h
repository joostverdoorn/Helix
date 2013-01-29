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

	TouchOut = 6, TouchIn = 7,

	LedData = 4, LedClock = 5,

} Pins;

extern void begin();
extern void ping();

extern void smallPing();
extern void bigPing();
extern void giantPing();

extern void addBullet(Bullet*);
extern void removeBullet(Bullet*);

static void log();
static int freeRam();

extern void send(std::vector<char>, Direction);
extern void broadcast(std::vector<char>);

extern void slowShiftOut(uint8_t);
extern void writeByte(uint8_t);
extern void writeGuard();
extern void writeFrame(LedNode*);
extern void writeFrame();
extern void pushStrip();
extern void pingStrip();

extern Colour HSVtoColour(float, float, float);

extern std::vector<Bullet*> bullets;
extern std::vector<LedNode*> nodes;

extern uint8_t bigInterval;

extern bool activated;
extern uint8_t leftActivated;
extern uint8_t rightActivated;
extern bool soundActive;
extern Colour soundColour;

extern Colour activatedColour;
extern Colour pathColour;
extern Colour baseColour;
extern Colour debugColour;

extern Colour colours[7];

extern uint16_t id;
extern bool hasLeftNeighbour;
extern bool hasRightNeighbour;

extern uint8_t pulseThreshold;
extern uint8_t slumberThreshold;
}

#endif /* MAIN_H_ */
