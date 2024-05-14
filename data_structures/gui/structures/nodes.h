#ifndef GUI_NODES_H
#define GUI_NODES_H

#include <string>
#include <utility>

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

struct BNode {
	int index;
	int value;
	int priority;
	BNode* left;
	BNode* right;

	BNode() = default;

	BNode(int index, int value, int priority) : index(index),
																							value(value),
																							priority(priority),
																							left(nullptr),
																							right(nullptr) {}
};

enum class Color { RED, BLACK };

struct RBNode {
	std::string key;
	int value;
	RBNode* parent;
	RBNode* left;
	RBNode* right;
	Color color;

	RBNode(std::string k, int v, Color c = Color::BLACK)
			: key(std::move(k)), value(v), parent(nullptr), left(nullptr), right(nullptr), color(c) {}
};

#endif //GUI_NODES_H
