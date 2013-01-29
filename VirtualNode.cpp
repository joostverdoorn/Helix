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

char VirtualNode::packetStart = '<';
char VirtualNode::packetEnd = '>';

VirtualNode::VirtualNode(Stream *s) {
	stream = s;
	neighbour = 0;

	full = false;
}

VirtualNode::~VirtualNode() {
}

void VirtualNode::ping() {

	vector<char> in = receive();
	if (in.size() > 0) {
		char arg1 = in[0];

		switch (arg1) {
		case 'O':
			if (in.size() >= 4) {
				Main::addBullet(new Bullet(neighbour, direction, Colour(in[1], in[2], in[3])));
			}
			break;

		case 'F':
			if (in.size() >= 2) {
				if (in[1] == 1) {
					full = true;
				} else {
					full = false;
				}
			}
			break;
		case 'A':
			if (in.size() >= 2) {
				if (direction == Main::LEFT) {
					Main::rightActivated = in[1];

					vector<char> message;
					message.push_back('A');
					message.push_back((char) Main::rightActivated + Main::activated);
					Main::send(message, Main::LEFT);
				} else {
					Main::leftActivated = in[1];

					vector<char> message;
					message.push_back('A');
					message.push_back((char) Main::leftActivated + Main::activated);
					Main::send(message, Main::RIGHT);
				}
			}
			break;
		case 'X': { // Exploration
			Main::id++;
			Main::activatedColour = Main::colours[Main::id % 7];

			vector<char> message;
			message.push_back('X');
			Main::send(message, Main::LEFT);
		}
			break;

		case 'U': // Slumber sensitivity
			if (in.size() >= 2) {
				Main::slumberThreshold = in[1];

				vector<char> message;
				message.push_back('U');
				message.push_back(in[1]);
				Main::send(message, direction);
			}
			break;

		case 'V': // Pulse sensitivity
			if (in.size() >= 2) {
				Main::pulseThreshold = in[1];

				vector<char> message;
				message.push_back('V');
				message.push_back(in[1]);
				Main::send(message, direction);
			}
			break;

		case 'W': // Pulse velocity
			if (in.size() >= 2) {
				Main::bigInterval = in[1];

				vector<char> message;
				message.push_back('W');
				message.push_back(in[1]);
				Main::send(message, direction);
			}

			break;

			/*case 'Y':
			 if (in.size() >= 2) {
			 Main::id = in[1] + 1;
			 Main::activatedColour = Main::colours[Main::id % 7];

			 vector<char> message;
			 message.push_back('Y');
			 message.push_back((char) Main::id);

			 Main::send(message, Main::RIGHT);
			 }
			 break;*/
		}
	}
}

bool VirtualNode::available() {
	return stream->available();
}

void VirtualNode::send(vector<char> message) {
	stream->write(packetStart);

	for (uint8_t i = 0; i < message.size(); i++) {
		stream->write(message[i]);
	}

	stream->write(packetEnd);
}

vector<char> VirtualNode::receive() {
	// Insert every item we read into the input vector
	while (stream->available() > 0) {
		input.push_back(stream->read());
	}

	vector<char> result;

	vector<char>::iterator itStart = find(input.begin(), input.end(), packetStart);
	if (itStart != input.end()) {
		vector<char>::iterator itEnd = find(itStart, input.end(), packetEnd);
		if (itEnd != input.end()) {
			for (vector<char>::iterator it = itStart + 1; it != itEnd; it++) {
				result.push_back(*it);
			}

			input.erase(input.begin(), itEnd);
		} else if (input.end() > itStart + 32) {
			input.clear();
		}
	} else {
		input.clear();
	}
	return result;

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
	vector<char> message;
	message.push_back('O');
	message.push_back((char) b->getColour().red);
	message.push_back((char) b->getColour().green);
	message.push_back((char) b->getColour().blue);

	send(message);
	b->die();
}

