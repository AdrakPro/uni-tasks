#ifndef GUI_RED_BLACK_TREE_DICT_H
#define GUI_RED_BLACK_TREE_DICT_H

#include "../nodes.h"
#include <iostream>

class RBTree {
private:
	RBNode* nil;
	RBNode* root;
	RBNode* current;

	void leftRotate(RBNode* x);
	void rightRotate(RBNode* x);
	void insertHelper(RBNode* z);
	void transplant(RBNode* u, RBNode* v);
	void removeHelper(RBNode* z);
	RBNode* minimum(RBNode* x) const;
	RBNode* maximum(RBNode* x);

public:
	RBTree() {
		nil = new RBNode("", 0);
		nil->color = Color::BLACK;
		current = nil;
		root = nil;
	}

	~RBTree() {

	}

	void insert(RBNode* x);
	bool isNodeNil(RBNode* x) const;
	[[nodiscard]] RBNode* containsNode(const std::string& key) const;

	void remove(RBNode* z);
};

class Dictionary {
private:
	RBTree tree;
	int size;

public:
	void insert(const std::string& key, int value);

	[[nodiscard]] int getSize() const;

	void incrementSize();

	void remove(const std::string &key);

	void decreaseSize();
};


#endif //GUI_RED_BLACK_TREE_DICT_H
