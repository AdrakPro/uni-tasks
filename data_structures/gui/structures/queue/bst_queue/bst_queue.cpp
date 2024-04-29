#include "bst_queue.h"

BSTQueue::~BSTQueue() {
	destroyTree(root);
}

BSTQueue::BSTQueue() = default;

BSTQueue::BSTQueue(const BSTQueue &other) {
	root = deepCopy(other.root);
	size = other.size;
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

void BSTQueue::insert(const BNode &new_node) {
	root = insertHelper(root, new_node);
}

BNode* BSTQueue::extractMaxHelper(BNode* node) {
	if (node == nullptr)
		return nullptr;

	if (node->right == nullptr) {
		--size;
		BNode* temp = node->left;
		delete node;

		return temp;
	}

	node->right = extractMaxHelper(node->right);

	return node;
}

BNode BSTQueue::extractMax() {
	if (size == 0) {
		return {};
	}

	BNode max = *root;
	root = extractMaxHelper(root);

	return max;
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

int BSTQueue::getSize() const {
	return size;
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

void BSTQueue::display() {
	displayHelper("", root, false);
}

void BSTQueue::setData(const int* data, int data_size) {
	destroyTree(root);

	this->size = 0;

	for (int i = 0; i < data_size; ++i) {
		int priority = generateNumber(0, 2 * data_size, 2137 + i);
		BNode node(i, data[i], priority);
		insert(node);
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
		// Node with only one child or no child
		if (current->left == nullptr) {
			BNode* temp = current->right;
			delete current;
			--size;
			return temp;
		} else if (current->right == nullptr) {
			BNode* temp = current->left;
			delete current;
			--size;
			return temp;
		}

		BNode* temp = minValueNode(current->right);

		current->index = temp->index;
		current->value = temp->value;
		current->priority = temp->priority;

		current->right = deleteNode(current->right, temp->index);

	} else if (index < current->index) {
		current->left = deleteNode(current->left, index);
	}
		// If the index to be deleted is greater, go to right subtree
	else {
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

//
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
//		REQUIRE(empty_node.index == 0); // Assuming default constructor sets index to 0
//		REQUIRE(empty_node.priority == 0); // Assuming default constructor sets priority to 0
//	}
//
//	SECTION("Peeking into an empty BSTQueue returns empty node") {
//		BNode empty_node = queue.peek();
//		REQUIRE(empty_node.index == 0);
//		REQUIRE(empty_node.priority == 0); // Assuming default constructor sets priority to 0
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
//
//
//
