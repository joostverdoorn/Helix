/*
 * Node.cpp
 *
 *  Created on: Nov 27, 2012
 *      Author: joost
 */

#include <Arduino.h>
#include "Node.h"
#include "Bullet.h"

using namespace std;

uint8_t Node::maxOccupants = 0;

Node::Node() {
	left = 0;
	right = 0;
}

Node::~Node() {
}

void Node::setLeft(Node *u) {
	left = u;

	if (left->getRight() != this) {
		left->setRight(this);
	}
}

void Node::setRight(Node *u) {
	right = u;

	if (right->getLeft() != this) {
		right->setLeft(this);
	}
}

Node* Node::getLeft() {
	return left;
}

Node* Node::getRight() {
	return right;
}

bool Node::hasLeft() {
	return left != 0;
}

bool Node::hasRight() {
	return right != 0;
}

void Node::addOccupant(Bullet *b) {
	occupants.push_back(b);
}

void Node::removeOccupant(Bullet *b) {
	occupants.remove(b);
}

bool Node::isEmpty() {
	return occupants.size() == 0;
}
