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
}

VirtualNode::~VirtualNode() {
}

void VirtualNode::ping() {
	if (stream->available()) {
		char operation = stream->read();

		String input = "";
		while (stream->available()) {
			input.concat(stream->read());
		}

		switch (isAvailable) {
		case '?': // Request
			switch (input.charAt(0)) {
			case 'A':
				stream->print("!A");
				stream->print(neighbour->isAvailable());
				break;

			}
			break;

		case '!': // Response / Command
			switch (input.charAt(0)) {
			case 'A':
				available = (bool) input.charAt(1);
				break;

			case 'O':
				break;
			}
			break;

		case '&': // Broadcast
			break;

		}
	}

	if (lastUpdate + interval < millis()) {
		stream->print("?A");
	}
}

bool VirtualNode::isAvailable() {
	return available;
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
	stream->print("O");
	stream->print(b->getMagnitude());

	b->die();
}

