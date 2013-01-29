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


	void setLeft(Node*);
	void setRight(Node*);

	void addOccupant(Bullet*);
	void removeOccupant(Bullet*);

	bool isFull();

	void send(vector<char>);
	vector<char> receive();

private:
	static char packetStart;
	static char packetEnd;
	vector<char> input;

	Stream *stream;
	Node *neighbour;

	Main::Direction direction;



	bool available();
};

#endif /* VIRTUALNODE_H_ */
