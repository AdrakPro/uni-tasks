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
																							left(nullptr), right(nullptr) {}
};

enum class Color {
	RED, BLACK
};

struct RBNode {
	std::string key;
	int value;
	RBNode* parent;
	RBNode* left;
	RBNode* right;
	Color color;

	RBNode(std::string key, int value, Color color = Color::BLACK)
			: key(std::move(key)), value(value), parent(nullptr), left(nullptr), right(nullptr),
				color(color) {}
};

struct Entry {
	std::string key;
	int value;
	bool is_occupied;

	Entry() : value(0), is_occupied(false) {}

	Entry(std::string key, int value, int is_occupied) : key(std::move(key)), value(value),
																											 is_occupied(is_occupied) {}
};

struct CEntry {
	std::string key;
	int value;
	int next;
	bool is_occupied;

	CEntry() : value(0), next(-1), is_occupied(false) {}

	CEntry(std::string key, int value, int next, int is_occupied) : key(std::move(key)),
																																	value(value),
																																	next(next),
																																	is_occupied(
																																			is_occupied
																																	) {}
};

#endif //GUI_NODES_H
