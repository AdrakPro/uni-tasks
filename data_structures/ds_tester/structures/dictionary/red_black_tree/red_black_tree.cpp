#include "red_black_tree.h"

RBTree::RBTree() {
	auto* node = new RBNode("", -1);
	node->parent = node;
	node->left = node;
	node->right = node;
	node->color = Color::RED;

	nil = node;
	root = nil;
	current = nil;

	size = 0;
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

void RBTree::removeHelper(RBNode* x) {
	RBNode* w;

	while (x != root && x->color == Color::BLACK) {
		if (x == x->parent->left) {
			w = x->parent->right;

			if (w->color == Color::RED) {
				w->color = Color::BLACK;
				x->parent->color = Color::RED;
				leftRotate(x->parent);
				w = x->parent->right;
			}

			if ((w->left->color == Color::BLACK) && w->right->color == Color::BLACK) {
				w->color = Color::RED;
				x = x->parent;
			} else {
				if (w->right->color == Color::BLACK) {
					w->left->color = Color::BLACK;
					w->color = Color::RED;
					rightRotate(w);
					w = x->parent->right;
				}

				w->color = x->parent->color;
				x->parent->color = Color::BLACK;
				w->right->color = Color::BLACK;
				leftRotate(x->parent);
				x = root;
			}
		} else {
			w = x->parent->left;

			if (w->color == Color::RED) {
				w->color = Color::BLACK;
				x->parent->color = Color::RED;
				rightRotate(x->parent);
				w = x->parent->left;
			}

			if ((w->right->color == Color::BLACK) && w->left->color == Color::BLACK) {
				w->color = Color::RED;
				x = x->parent;
			} else {
				if (w->left->color == Color::BLACK) {
					w->right->color = Color::BLACK;
					w->color = Color::RED;
					leftRotate(w);
					w = x->parent->left;
				}

				w->color = x->parent->color;
				x->parent->color = Color::BLACK;
				w->left->color = Color::BLACK;
				rightRotate(x->parent);
				x = root;
			}
		}
	}

	x->color = Color::BLACK;
}

void RBTree::remove(RBNode* z) {
	RBNode* x;
	RBNode* y = z;
	Color y_original_color = y->color;

	if (isNodeNil(z->left)) {
		x = z->right;
		transplant(z, z->right);
	} else if (isNodeNil(z->right)) {
		x = z->left;
		transplant(z, z->left);
	} else {
		y = minimum(z->right);
		y_original_color = y->color;
		x = y->right;

		if (y->parent == z) {
			x->parent = y;
		} else {
			transplant(y, y->right);
			y->right = z->right;
			y->right->parent = y;
		}

		transplant(z, y);
		y->left = z->left;
		y->left->parent = y;
		y->color = z->color;
	}

	if (z == current) {
		current = nil;
	}

	delete z;

	if (y_original_color == Color::BLACK) {
		removeHelper(x);
	}
}

void RBTree::destroy() {
	postOrderDelete(root);
	delete nil;
}

void RBTree::postOrderDelete(RBNode* r) {
	if (isNodeNil(r)) {
		return;
	}

	if (!isNodeNil(r->left)) {
		postOrderDelete(r->left);
	}

	if (!isNodeNil(r->right)) {
		postOrderDelete(r->right);
	}

	if (isNodeNil(r->parent)) {
		root = nil;
	} else if (r->key == r->parent->left->key) {
		r->parent->left = nil;
	} else {
		r->parent->right = nil;
	}

	--size;

	delete r;
}

void RBTree::clear() {
	postOrderDelete(root);
	root = nil;
	current = nil;
}

bool RBTree::isBalancedHelper(RBNode* r, int &max_height, int &min_height) {
	if (isNodeNil(r)) {
		max_height = min_height = 0;
		return true;
	}

	int left_max_height, left_min_height;
	int right_max_height, right_min_height;

	if (!isBalancedHelper(r->left, left_max_height, left_min_height)) {
		return false;
	}

	if (!isBalancedHelper(r->right, right_max_height, right_min_height)) {
		return false;
	}

	max_height = std::max(left_max_height, right_max_height) + 1;
	min_height = std::max(left_min_height, right_min_height) + 1;

	if (max_height <= 2 * min_height) {
		return true;
	}

	return false;
}

bool RBTree::isBalanced() {
	int max_height, min_height;

	return isBalancedHelper(root, max_height, min_height);
}

bool RBTree::isNodeNil(RBNode* x) const {
	return x == nil;
}

void RBTree::leftRotate(RBNode* x) {
	RBNode* y = x->right;

	x->right = y->left;

	if (!isNodeNil(y->left)) {
		y->left->parent = x;
	}

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

	y->right->parent = x;

	y->parent = x->parent;

	if (isNodeNil(x->parent)) {
		root = y;
	} else if (x == x->parent->right) {
		x->parent->right = y;
	} else {
		x->parent->left = y;
	}
	y->right = x;
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

int RBTree::getSize() const {
	return size;
}

void RBTree::incrementSize() {
	this->size++;
}

void RBTree::decreaseSize() {
	this->size--;
}

void RBTree::setSize(int s) {
	this->size = s;
}

RBNode* RBTree::getRoot() const {
	return root;
}

RBNode* RBTree::getNil() const {
	return nil;
}

void RBTree::preOrderCopy(RBNode* r, RBNode* x) {
	if (r->key == x->key) {
		return;
	}

	auto* z = new RBNode(r->key, r->value);
	insert(z);

	preOrderCopy(r->left, x);
	preOrderCopy(r->right, x);
}


//#define CATCH_CONFIG_MAIN
//
//#include "../../../tests/catch.hpp"
//
//TEST_CASE("RBTree Remove Node") {
//	RBTree tree;
//
//// Insert nodes
//	tree.insert(new RBNode("a", 1));
//	tree.incrementSize();
//
//	tree.insert(new RBNode("b", 2));
//	tree.incrementSize();
//
//
//	tree.insert(new RBNode("c", 3));
//	tree.incrementSize();
//
//
//// Remove node "b"
//	RBNode* node = tree.containsNode("b");
//	REQUIRE(node != tree.nil);
//	tree.remove(node);
//
//// Check the tree structure
//	REQUIRE(tree.containsNode("b") == tree.nil);
//	REQUIRE(tree.getSize() == 2);
//
//// Check other nodes
//	REQUIRE(tree.containsNode("a") != tree.nil);
//	REQUIRE(tree.containsNode("c") != tree.nil);
//}
//
//TEST_CASE("RBTree Remove Node From Vector") {
//	std::vector<RBTree> trees;
//	trees.emplace_back();
//	trees.emplace_back();
//
//// Insert nodes into the second tree
//	trees[1].insert(new RBNode("a", 1));
//	trees[1].incrementSize();
//
//	trees[1].insert(new RBNode("b", 2));
//	trees[1].incrementSize();
//
//	trees[1].insert(new RBNode("c", 3));
//	trees[1].incrementSize();
//
//
//
//// Remove node "b" from the second tree
//	RBNode* node = trees[1].containsNode("b");
//	REQUIRE(node != trees[1].nil);
//	trees[1].remove(node);
//
//// Check the tree structure
//	REQUIRE(trees[1].containsNode("b") == trees[1].nil);
//	REQUIRE(trees[1].getSize() == 2);
//
//// Check other nodes
//	REQUIRE(trees[1].containsNode("a") != trees[1].nil);
//	REQUIRE(trees[1].containsNode("c") != trees[1].nil);
//}
//
