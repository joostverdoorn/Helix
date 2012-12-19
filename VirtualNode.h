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
#include <SoftwareSerial.h>

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

	void send(const unsigned char*, uint8_t size);
	const unsigned char* receive();

private:
	static char packetStart[2];
	static char packetEnd[2];
	static vector<char> input;

	Stream *stream;
	Node *neighbour;

	Main::Direction direction;

	bool isFull();

	bool available();
};

#endif /* VIRTUALNODE_H_ */
