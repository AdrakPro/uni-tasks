#include "qnodes.h"

QNode::QNode(int index, int value, int priority) {
	this->index = index;
	this->value = value;
	this->priority = priority;
}

int QNode::getValue() const {
	return this->value;
}

int QNode::getPriority() const {
	return this->priority;
}

void QNode::setPriority(int new_priority) {
	this->priority = new_priority;
}

int QNode::getIndex() const {
	return this->index;
}

