/*
 * VirtualNode.cpp
 *
 *  Created on: Nov 26, 2012
 *      Author: joost
 */

#include <Arduino.h>
#include <StandardCplusplus.h>
#include <algorithm>
#include "VirtualNode.h"
#include "Bullet.h"
#include "Main.h"

using namespace std;
using Main::bullets;

char VirtualNode::packetStart[2] = { '<', '<' };
char VirtualNode::packetEnd[2] = { '>', '>' };
vector<char> VirtualNode::input;

VirtualNode::VirtualNode(Stream *s) {
	stream = s;
	neighbour = 0;

	full = false;
}

VirtualNode::~VirtualNode() {
}

void VirtualNode::ping() {

	const unsigned char* in = receive();
	if (in != NULL) {

		char op = in[0];
		char arg1 = in[1];

		switch (op) {
		case '?':
			switch (arg1) {
			case 'F':
				unsigned char message[3] = { '!', 'F', (char) neighbour->isFull() };
				send(message, 3);
				break;
			}
			break;

		case '!':
			switch (arg1) {
			case 'O':
				Main::bullets.push_back(new Bullet(neighbour, direction, Colour(in[2], in[3], in[4])));
				break;

			case 'F':
				if (in[2] == '1') {
					full = true;
				} else {
					full = false;
				}
				break;
			}

			break;

		case '~':
			switch (arg1) {
			case 'A':
				if (direction == Main::LEFT) {
					Main::rightActivated = in[2];

					const unsigned char message[3] = { '~', 'A', Main::rightActivated + Main::activated };
					Main::send(message, 3, Main::LEFT);
				} else {
					Main::leftActivated = in[2];

					const unsigned char message[3] = { '~', 'A', Main::leftActivated + Main::activated };
					Main::send(message, 3, Main::RIGHT);
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

void VirtualNode::send(const unsigned char *message, uint8_t size) {
	stream->write(packetStart[0]);
	stream->write(packetStart[1]);
	stream->write(size);
	for (int i = 0; i < size; i++) {
		stream->write(message[i]);
	}
	stream->write(packetEnd[0]);
	stream->write(packetEnd[1]);
}

const unsigned char* VirtualNode::receive() {
	while (stream->available()) {
		input.push_back(stream->read());
	}

	uint8_t packetStartSize = sizeof(packetStart) / sizeof(char);
	uint8_t packetEndSize = sizeof(packetEnd) / sizeof(char);

	if (input.size() > +packetStartSize + packetEndSize + 1) {
		vector<char>::iterator startPos = find(input.begin(), input.end(), packetStart[0]);
		if (startPos != input.end() && *(startPos + 1) == packetStart[1]) {
			uint8_t packetSize = *(startPos + packetStartSize);

			if (*(startPos + packetStartSize + 1 + packetSize) == packetEnd[0] && *(startPos + packetStartSize + 1 + packetSize + 1) == packetEnd[1]) {
				unsigned char packet[packetSize];

				for (uint8_t i = 0; i < packetSize; i++) {
					packet[i] = *(startPos + packetStartSize + 1 + i);
				}

				input.erase(input.begin(), startPos + packetStartSize + 1 + packetSize + 1);
				return packet;

			}
		} else if (input.size() > 8 && startPos == input.end()) {
			input.clear();
		}
	}

	return NULL;
}

bool VirtualNode::isFull() {
	return full;
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
	const unsigned char message[5] = { '!', 'O', b->getColour().red, b->getColour().green, b->getColour().blue };
	send(message, 5);
	b->die();
}

void VirtualNode::removeOccupant(Bullet *b) {

}

