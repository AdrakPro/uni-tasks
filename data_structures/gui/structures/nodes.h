#ifndef GUI_NODES_H
#define GUI_NODES_H

struct SNode {
	int value;
	SNode* next;

	SNode() = default;
};

struct DNode {
	int value;
	DNode* next;
	DNode* prev;

	DNode() = default;
};

struct QNode {
	int index;
	int value;
	int priority;

	QNode() = default;

	QNode(int index, int value, int priority) : index(index), value(value),
																							priority(priority) {}
};

#endif //GUI_NODES_H
