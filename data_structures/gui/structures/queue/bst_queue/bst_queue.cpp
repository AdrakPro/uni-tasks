#include "bst_queue.h"

BSTQueue::~BSTQueue() {
	destroyTree(root);
}

BSTQueue::BSTQueue() = default;

BSTQueue::BSTQueue(const BSTQueue &other) {
	root = deepCopy(other.root);
	size = other.size;
}

void BSTQueue::display() {
	displayHelper("", root, false);
}

void BSTQueue::setData(const int* data, int data_size) {
	destroyTree(root);

	this->size = 0;

	for (int i = 0; i < data_size; ++i) {
		int priority = generateNumber(0, 2 * data_size, 50 + i);
		BNode node(i, data[i], priority);
		insert(node);
	}

	balance();
}

BNode* BSTQueue::deepCopy(const BNode* node) {
	if (node == nullptr) {
		return nullptr;
	}

	auto* new_node = new BNode(node->index, node->value, node->priority);

	new_node->left = deepCopy(node->left);
	new_node->right = deepCopy(node->right);

	return new_node;
}

void BSTQueue::destroyTree(BNode* current) {
	if (current != nullptr) {
		destroyTree(current->left);
		destroyTree(current->right);
		delete current;
	}

	root = nullptr;
}

void BSTQueue::insert(const BNode &new_node) {
	root = insertHelper(root, new_node);
}

BNode* BSTQueue::insertHelper(BNode* current, const BNode &new_node) {
	if (current == nullptr) {
		++size;
		return new BNode(new_node.index, new_node.value, new_node.priority);
	}

	if (new_node.priority < current->priority ||
			(new_node.priority == current->priority && new_node.index > current->index)) {
		current->left = insertHelper(current->left, new_node);
	} else {
		current->right = insertHelper(current->right, new_node);
	}

	return current;
}

BNode BSTQueue::extractMax() {
	if (size == 0) {
		return {};
	}

	BNode* current = root;
	BNode* parent = nullptr;

	while (current->right != nullptr) {
		parent = current;
		current = current->right;
	}

	if (parent == nullptr) {
		root = deleteNode(root, current->index);
	} else {
		parent->right = deleteNode(parent->right, current->index);
	}

	return *current;
}

BNode BSTQueue::peek() const {
	if (size == 0) {
		return {};
	}

	BNode* current = root;

	while (current->right != nullptr) {
		current = current->right;
	}

	return *current;
}

void BSTQueue::displayHelper(const std::string &prefix, BNode* node, bool isLeft) const {
	if (node != nullptr) {
		std::cout << prefix;
		std::cout << (isLeft ? "├──" : "└──");

		std::cout << node->value << " (" << node->priority << ")" << std::endl;

		displayHelper(prefix + (isLeft ? "|   " : "    "), node->right, true);
		displayHelper(prefix + (isLeft ? "|   " : "    "), node->left, false);
	}
}

void BSTQueue::modifyPriority(int index, int new_priority) {
	BNode* node_to_delete = deleteNode(root, index);
	root = node_to_delete;

	BNode new_node(index, node_to_delete->value, new_priority);
	insert(new_node);
}

BNode* BSTQueue::deleteNode(BNode* current, int index) {
	if (current == nullptr) {
		return nullptr;
	}

	if (index == current->index) {
		if (current->left == nullptr) {
			BNode* tmp = current->right;

			delete current;
			--size;
			return tmp;
		} else if (current->right == nullptr) {
			BNode* tmp = current->left;

			delete current;
			--size;
			return tmp;
		}

		BNode* tmp = minValueNode(current->right);

		current->index = tmp->index;
		current->value = tmp->value;
		current->priority = tmp->priority;

		current->right = deleteNode(current->right, tmp->index);

	} else if (index < current->index) {
		current->left = deleteNode(current->left, index);
	} else {
		current->right = deleteNode(current->right, index);
	}

	return current;
}

BNode* BSTQueue::minValueNode(BNode* node) {
	BNode* current = node;

	while (current->left != nullptr)
		current = current->left;

	return current;
}

// Convert tree to sorted linked list
void BSTQueue::treeToVine(BNode* grand) {
	BNode* tmp = grand->right;

	while (tmp != nullptr) {
		// Right rotate
		if (tmp->left != nullptr) {
			BNode* old_tmp = tmp;
			tmp = tmp->left;
			old_tmp->left = tmp->right;
			tmp->right = old_tmp;
			grand->right = tmp;
		} else {
			grand = tmp;
			tmp = tmp->right;
		}
	}
}

// Compress the vine
void compress(BNode* grand, int count) {
	BNode* tmp = grand->right;

	// Traverse and left rotate 	
	for (int i = 0; i < count; ++i) {
		BNode* old_tmp = tmp;
		tmp = tmp->right;
		grand->right = tmp;
		old_tmp->right = tmp->left;
		tmp->left = old_tmp;
		grand = tmp;
		tmp = tmp->right;
	}
}

// DSW algorithm
BNode* BSTQueue::vineToTree(BNode* root) {
	auto* grand = new BNode();
	grand->right = root;

	treeToVine(grand);

	// Height of tree
	int h = log2(size + 1);
	// Number of nodes until second last level
	int m = pow(2, h) - 1;

	compress(grand, size - m);

	for (int i = m / 2; i > 0; i /= 2) {
		compress(grand, i);
	}

	return grand->right;
}

int BSTQueue::height(BNode* node) {
	if (node == nullptr) {
		return 0;
	}

	int left_height = height(node->left);
	int right_height = height(node->right);

	if (left_height > right_height) {
		return left_height + 1;
	} else {
		return right_height + 1;
	}
}

int BSTQueue::getRootHeight() {
	if (root == nullptr) {
		return 0;
	}

	return height(root);
}

void BSTQueue::balance() {
	// Height is greater than log2(size), balance tree
	if (ceil(log2(size)) < height(root)) {
		root = vineToTree(root);
	}
}

int BSTQueue::getSize() const {
	return size;
}

//#define CATCH_CONFIG_MAIN
//#define CATCH_CONFIG_FAST_COMPILE
//
//#include "../../../tests/catch.hpp"
//
//TEST_CASE("BSTQueue Operations") {
//	BSTQueue queue;
//
//	SECTION("Inserting nodes into BSTQueue maintains priority order") {
//		queue.insert(BNode(1, 10, 20));
//		queue.insert(BNode(2, 20, 5));
//		queue.insert(BNode(3, 30, 15));
//
//		queue.balance();
//
//		REQUIRE(queue.peek().priority == 20);
//		queue.extractMax();
//		REQUIRE(queue.peek().priority == 15);
//		queue.extractMax();
//		REQUIRE(queue.peek().priority == 5);
//	}
//
//	SECTION("Inserting nodes into BSTQueue increases size") {
//		queue.insert(BNode(1, 10, 10));
//		queue.insert(BNode(2, 20, 5));
//		queue.insert(BNode(3, 30, 15));
//
//		REQUIRE(queue.getSize() == 3);
//	}
//
//	SECTION("Inserting nodes with same priority maintains insertion order") {
//		queue.insert(BNode(1, 10, 10));
//		queue.insert(BNode(2, 20, 10));
//		queue.insert(BNode(3, 30, 10));
//
//		REQUIRE(queue.peek().index == 1);
//		queue.extractMax();
//		REQUIRE(queue.peek().index == 2);
//		queue.extractMax();
//		REQUIRE(queue.peek().index == 3);
//	}
//
//	SECTION("Extracting nodes from an empty BSTQueue returns empty node") {
//		BNode empty_node = queue.extractMax();
//		REQUIRE(empty_node.index == 0);
//		REQUIRE(empty_node.priority == 0);
//	}
//
//	SECTION("Peeking into an empty BSTQueue returns empty node") {
//		BNode empty_node = queue.peek();
//		REQUIRE(empty_node.index == 0);
//		REQUIRE(empty_node.priority == 0);
//	}
//
//	SECTION("Modifying key updates priority of existing node") {
//		queue.insert(BNode(0, 10, 10));
//		queue.insert(BNode(1, 20, 5));
//		queue.insert(BNode(2, 30, 15));
//
//		queue.modifyPriority(1, 25);
//
//		REQUIRE(queue.peek().priority == 25);
//	}
//}
