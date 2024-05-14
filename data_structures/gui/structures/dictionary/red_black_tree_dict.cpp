#include "red_black_tree_dict.h"

bool RBTree::isNodeNil(RBNode* x) const {
	return x == nil;
}

void RBTree::leftRotate(RBNode* x) {
	RBNode* y = x->right;

	x->right = y->left;
	y->parent = x->parent;

	if (isNodeNil(x->parent)) {
		root = y;
	} else if (x == x->parent->left) {
		x->parent->left = y;
	} else {
		x->parent->right = y;
	}

	y->left = x;
	x->parent = y;
}

void RBTree::rightRotate(RBNode* x) {
	RBNode* y = x->left;

	x->left = y->right;
	x->parent = x->parent;

	if (isNodeNil(x->parent)) {
		root = x;
	} else if (x == x->parent->right) {
		x->parent->right = y;
	} else {
		x->parent->left = y;
	}

	x->right = x;
	x->parent = y;
}

void RBTree::transplant(RBNode* u, RBNode* v) {
	if (u->parent == nil) {
		root = v;
	} else if (u == u->parent->left) {
		u->parent->left = v;
	} else {
		u->parent->right = v;
	}
	v->parent = u->parent;
}

RBNode* RBTree::minimum(RBNode* x) const {
	RBNode* curr = x;

	while (!isNodeNil(curr->left)) {
		curr = curr->left;
	}

	return curr;
}

void RBTree::insertHelper(RBNode* z) {
	RBNode* y;

	while (!isNodeNil(z->parent) && z->parent->color == Color::RED) {
		if (z->parent == z->parent->parent->left) {
			y = z->parent->parent->right;

			// Case 1
			if (y->color == Color::RED) {
				z->parent->color = Color::BLACK;
				y->color = Color::BLACK;
				z->parent->parent->color = Color::RED;
				z = z->parent->parent;
			} else {
				// Case 2
				if (z == z->parent->right) {
					z = z->parent;
					leftRotate(z);
				}

				// Case 3
				z->parent->color = Color::BLACK;
				z->parent->parent->color = Color::RED;
				rightRotate(z->parent->parent);
			}
		} else {
			y = z->parent->parent->left;

			// Case 4
			if (y->color == Color::RED) {
				z->parent->color = Color::BLACK;
				y->color = Color::BLACK;
				z->parent->parent->color = Color::RED;
				z = z->parent->parent;
			} else {
				// Case 5
				if (z == z->parent->left) {
					z = z->parent;
					rightRotate(z);
				}

				// Case 6
				z->parent->color = Color::BLACK;
				z->parent->parent->color = Color::RED;
				leftRotate(z->parent->parent);
			}
		}
	}

	root->color = Color::BLACK;
}

void RBTree::insert(RBNode* z) {
	RBNode* x = root;
	RBNode* y = nil;

	while (!isNodeNil(x)) {
		y = x;

		if (z->key.compare(x->key) < 0) {
			x = x->left;
		} else {
			x = x->right;
		}
	}

	z->parent = y;

	if (isNodeNil(y)) {
		root = z;
	} else if (z->key.compare(y->key) < 0) {
		y->left = z;
	} else {
		y->right = z;
	}

	z->left = nil;
	z->right = nil;
	z->color = Color::RED;

	insertHelper(z);
}

RBNode* RBTree::containsNode(const std::string &key) const {
	RBNode* curr = root;

	if (isNodeNil(curr)) {
		return nil;
	}

	while (true) {
		if (key.compare(curr->key) < 0) {
			if (isNodeNil(curr->left)) {
				return nil;
			}

			curr = curr->left;
		} else if (key.compare(curr->key) > 0) {
			if (isNodeNil(curr->right)) {
				return nil;
			}

			curr = curr->right;
		} else {
			return curr;
		}
	}
}

void Dictionary::insert(const std::string &key, int value) {
	RBNode* n = tree.containsNode(key);

	if (!tree.isNodeNil(n)) {
		n->value = value;
	} else {
		auto* z = new RBNode(key, value);
		incrementSize();
		tree.insert(z);
	}
}

void Dictionary::remove(const std::string &key) {
	RBNode* z = tree.containsNode(key);

	if (tree.isNodeNil(z)) {
		return;
	}

	decreaseSize();

	tree.remove(z);
}

int Dictionary::getSize() const {
	return size;
}

void Dictionary::incrementSize() {
	this->size++;
}

void Dictionary::decreaseSize() {
	this->size--;
}
