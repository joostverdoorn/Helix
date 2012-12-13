/*
 * VirtualNode.h
 *
 *  Created on: Nov 26, 2012
 *      Author: joost
 */

#ifndef VIRTUALNODE_H_
#define VIRTUALNODE_H_

#include "Main.h"
#include "Node.h"
#include "Bullet.h"
#include "/usr/share/arduino/libraries/SoftwareSerial/SoftwareSerial.h"

class Main;

class VirtualNode: public Node {

public:
	VirtualNode(Stream*);
	virtual ~VirtualNode();

	void ping();
	bool full;

	void setLeft(Node*);
	void setRight(Node*);

	void addOccupant(Bullet*);
	void removeOccupant(Bullet*);

	void send(String message);

private:
	static const char* packetStart;
	static const char* packetEnd;

	Stream *stream;
	Node *neighbour;

	Main::Direction direction;

	bool isFull();

	bool available();
	String receive();

	String input;
};

#endif /* VIRTUALNODE_H_ */
