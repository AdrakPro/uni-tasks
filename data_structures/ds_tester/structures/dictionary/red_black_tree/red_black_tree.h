#ifndef GUI_RED_BLACK_TREE_H
#define GUI_RED_BLACK_TREE_H

#include "../../nodes.h"

class RBTree {
private:
	RBNode* nil;
	RBNode* root;
	RBNode* current;
	int size;

	void leftRotate(RBNode* x);

	void rightRotate(RBNode* x);

	void insertHelper(RBNode* z);

	void transplant(RBNode* u, RBNode* v);

	void removeHelper(RBNode* z);

	RBNode* minimum(RBNode* x) const;

	void postOrderDelete(RBNode* r);

	bool isBalancedHelper(RBNode* r, int &max_height, int &min_height);

public:
	RBTree();

	void insert(RBNode* x);

	bool isNodeNil(RBNode* x) const;

	[[nodiscard]] RBNode* containsNode(const std::string &key) const;

	void remove(RBNode* z);

	void clear();

	void destroy();

	bool isBalanced();

	void incrementSize();

	void decreaseSize();

	void setSize(int s);

	[[nodiscard]] int getSize() const;

	[[nodiscard]] RBNode* getRoot() const;

	void preOrderCopy(RBNode* r, RBNode* x);

	RBNode* getNil() const;
};

#endif //GUI_RED_BLACK_TREE_H
