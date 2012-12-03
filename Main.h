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
#include "/usr/share/arduino/libraries/Wire/Wire.h"


#include <StandardCplusplus.h>
#include <vector>
#include <list>

class Bullet;
class Node;
class VirtualNode;

using namespace std;

class Main {
public:
	typedef enum Direction {
		LEFT, RIGHT
	} Direction;

	typedef enum Pins {
		SoftRX = 2, SoftTX = 4,

		Button1 = 7, Button2 = 8,

		Status = 13
	} Pins;


	virtual ~Main();

	static void begin();
	static void ping();

	static void log();
	static int freeRam();

	static void send(String, Direction);
	static void broadcast(String);

	static long baudRate;
	static SoftwareSerial *softSerial;
	static HardwareSerial *hardSerial;

	static TwoWire *logSerial;
	static int logAddress;
	static void log(const char*);

	static unsigned long lastSmallUpdate;
	static unsigned long lastBigUpdate;
	static int smallInterval;
	static int bigInterval;

	static int timeout;

	static int leds[];
	static int nLeds;

	static list<Bullet*> bullets;
	static vector<Node*> nodes;

	static VirtualNode* leftNode;
	static VirtualNode* rightNode;

	static bool activated;
	static int leftActivated;
	static int rightActivated;

private:
	Main();

};

#endif /* MAIN_H_ */
