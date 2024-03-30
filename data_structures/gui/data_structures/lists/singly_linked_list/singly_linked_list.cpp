#include "singly_linked_list.h"

SLinkedList::SLinkedList() {
	this->head = nullptr;
	this->size = 0;
}

SLinkedList::~SLinkedList() {
	Node* current = head;
	while (current != nullptr) {
		Node* next = current->next;
		delete current;
		current = next;
	}
}

bool SLinkedList::add(const int &element, int index) {
	if (isIndexNotValid(index, size)) {
		return false;
	}

	if (index == 0) {
		return addFront(element);
	}

	if (index == size) {
		return addBack(element);
	}

	Node* node = new Node;
	Node* old = getNode(index - 1);

	// Set new node's value and link it after old node
	node->value = element;
	node->next = old->next;
	old->next = node;

	++size;

	return true;
}

bool SLinkedList::addFront(const int &element) {
	Node* node = new Node;

	node->value = element;

	// Set head to new node or prepend
	if (head == nullptr) {
		head = node;
	} else {
		node->next = head;
		head = node;
	}

	++size;

	return true;
}

bool SLinkedList::addBack(const int &element) {
	Node* node = new Node;
	Node* old = getNode(size - 1);

	//	Set new node's value and mark it as end
	node->value = element;
	node->next = nullptr;

	if (head == nullptr) {
		head = node;
	} else {
		// Update old's pointer
		old->next = node;

	}

	++size;

	return true;
}

bool SLinkedList::remove(int index) {
	if (isIndexNotValid(index, size)) {
		return false;
	}

	if (index == 0) {
		return removeFront();
	}

	if (index == size) {
		return removeBack();
	}

	// Get old's previous and node to remove
	Node* old = getNode(index - 1);
	Node* temp = old->next;


	// Skip removed node by linking old's previous with next
	old->next = old->next->next;
	delete temp;

	--size;

	return true;
}

bool SLinkedList::removeFront() {
	if (isEmpty()) {
		return false;
	}

	Node* temp = head;

	// Assign head to the next node and delete current head
	head = head->next;
	delete temp;

	--size;

	return true;
}

bool SLinkedList::removeBack() {
	if (isEmpty()) {
		return false;
	}

	Node* current = head;
	Node* previous = new Node;

	// At each step we set the previous pointer to the current node, and the current pointer at the next node.
	while (current->next != nullptr) {
		previous = current;
		current = current->next;
	}

	// Previous node which points to nullptr and delete current
	previous->next = nullptr;
	delete current;

	--size;

	return true;
}

bool SLinkedList::find(const int &element) {
	for (int i = 0; i < size; ++i) {
		if (getNodeValue(i) == element) {
			return true;
		}
	}

	return false;
}

bool SLinkedList::isEmpty() const {
	return head == nullptr;
}

Node* SLinkedList::getNode(int index) const {
	if (isIndexNotValid(index, size)) {
		return nullptr;
	}

	Node* current = head;
	int i = 0;

	while (i < index) {
		current = current->next;
		++i;
	}

	return current;
}

int SLinkedList::getNodeValue(int index) const {
	Node* node = getNode(index);

	if (node == nullptr) {
		return -1;
	}

	return node->value;
}

const int &SLinkedList::front() const {
	return head->value;
}

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_FAST_COMPILE

#include "../../../tests/catch.hpp"

TEST_CASE("Singly linked list with head") {
	SLinkedList list;
	list.add(1, 0);
	list.add(2, 1);
	list.add(3, 2);

	SECTION("Add element") {
		SECTION("Add element to the front") {
			REQUIRE(list.addFront(4));
			REQUIRE(list.front() == 4);
			REQUIRE(list.add(5, 0));
			REQUIRE(list.front() == 5);
		}

			// In singly linked list it is not advised to add elements in middle
		SECTION("Add element in middle") {
			REQUIRE(list.add(4, 1));
			REQUIRE(list.getNodeValue(1) == 4);
			REQUIRE(list.getNodeValue(2) == 2);
		}

		SECTION("Add element to the end") {
			REQUIRE(list.addBack(4));
			REQUIRE(list.getNodeValue(3) == 4);
		}
	}

	SECTION("Remove element") {
		SECTION("Remove element from the front") {
			REQUIRE(list.removeFront());
			REQUIRE(list.front() == 2);
			REQUIRE(list.removeFront());
			REQUIRE(list.front() == 3);
		}

			// In singly linked list it is not advised to add elements in middle
		SECTION("Remove element in middle") {
			REQUIRE(list.remove(1));
			REQUIRE(list.getNodeValue(1) == 3);
			REQUIRE(list.remove(1));
			REQUIRE(list.getNodeValue(0) == 1);

		}

		SECTION("Remove element from the end") {
			REQUIRE(list.remove(2));
			REQUIRE(list.getNodeValue(1) == 2);
			REQUIRE(list.removeBack());
			REQUIRE(list.getNodeValue(1) == -1);
			REQUIRE(list.getNodeValue(0) == 1);
		}
	}

	SECTION("Find element") {
		REQUIRE(list.find(3));
	}
}