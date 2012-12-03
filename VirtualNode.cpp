/*
 * VirtualNode.cpp
 *
 *  Created on: Nov 26, 2012
 *      Author: joost
 */

#include <Arduino.h>
#include "VirtualNode.h"
#include "Bullet.h"

VirtualNode::VirtualNode(Stream *s) {
	stream = s;
	neighbour = 0;

	lastUpdate = millis();
	interval = 500;

	full = false;

	packetStart = "";
	packetStart.concat((char) 60);
	packetStart.concat((char) 60);

	packetEnd = "";
	packetEnd.concat((char) 62);
	packetEnd.concat((char) 62);
}

VirtualNode::~VirtualNode() {
}

void VirtualNode::ping() {

	String in = receive();
	if (in != NULL) {

		char op = in.charAt(0);
		char arg1 = in.charAt(1);

		switch (op) {
		case '?':
			switch (arg1) {
			case 'F':
				String message = "!F";
				message.concat(neighbour->isFull() ? '1' : '0');
				send(message);
				break;
			}
			break;

		case '!':
			switch (arg1) {
			case 'O':
				Main::bullets.push_back(new Bullet(neighbour, direction, in.charAt(2)));
				break;

			case 'F':
				if(in.charAt(2) == '1') {
					full = true;
				} else {
					full = false;
				}
				break;
			}

			break;

		case '~':
			Main::send(in, direction);

			switch (arg1) {
			case 'A':
				if (in.charAt(2) == '1') {
					digitalWrite(13, HIGH);
					if (direction == Main::LEFT) {
						Main::rightActivated++;
					} else {
						Main::leftActivated++;
					}
				} else {
					if (direction == Main::LEFT) {
						Main::rightActivated--;
					} else {
						Main::leftActivated--;
					}
				}
				break;
			}
			break;
		}
	}
}

bool VirtualNode::available() {
	return stream->available();
}

void VirtualNode::send(String message) {
	String output = packetStart;
	output.concat((char) message.length());
	output.concat(message);
	output.concat(packetEnd);

	stream->print(output);
}

String VirtualNode::receive() {
	while (stream->available()) {
		input.concat((char) stream->read());
	}

	String packet;

	int startPos = input.indexOf(packetStart);
	int endPos = input.indexOf(packetEnd, startPos);

	if (endPos != -1) {
		int length = input.charAt(startPos + 2);

		if (endPos == startPos + 3 + length) {
			packet = input.substring(startPos + 3, endPos);
		}

		input = input.substring(endPos + 2);
		return packet;
	} else {
		return NULL;
	}
}

bool VirtualNode::isFull() {
	return false;
}

void VirtualNode::setLeft(Node *u) {
	if (right == 0) {
		Node::setLeft(u);

		neighbour = u;
		direction = Main::LEFT;
	}
}

void VirtualNode::setRight(Node *u) {
	if (left == 0) {
		Node::setRight(u);

		neighbour = u;
		direction = Main::RIGHT;
	}
}

void VirtualNode::addOccupant(Bullet *b) {
	String message = "!O";
	message.concat((char) b->getMagnitude());

	send(message);
	b->die();
}

void VirtualNode::removeOccupant(Bullet *b) {

}

