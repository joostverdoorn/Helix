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
	typedef enum State {
		NORMAL, AVAILABLE_REQUEST_SENT
	} State;

	VirtualNode(Stream*);
	virtual ~VirtualNode();

	void ping();
	bool available;

	void setLeft(Node*);
	void setRight(Node*);

	void addOccupant(Bullet*);

private:
	Stream *stream;
	Node *neighbour;

	Main::Direction direction;
	State state;

	bool isAvailable();
	long lastUpdate();
	int interval();

};

#endif /* VIRTUALNODE_H_ */
