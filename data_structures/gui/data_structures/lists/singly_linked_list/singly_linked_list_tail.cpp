#include "singly_linked_list_tail.h"

SLinkedListWithTail::SLinkedListWithTail() {
	this->head = nullptr;
	this->tail = nullptr;
	this->size = 0;
}

SLinkedListWithTail::~SLinkedListWithTail() {
	Node* current = head;

	while (current != nullptr) {
		Node* next = current->next;
		delete current;
		current = next;
	}
}

bool SLinkedListWithTail::add(const int &element, int position) {
	if (position <= 0) {
		return addFront(element);
	}

	if (position >= size - 1) {
		return addBack(element);
	}

	Node* old = getNode(position - 1);

	if (old == nullptr) {
		return false;
	}

	// Set new node's value and link it after old node
	Node* node = new Node;
	node->value = element;
	node->next = old->next;
	old->next = node;

	++size;

	return true;
}

bool SLinkedListWithTail::addFront(const int &element) {
	Node* node = new Node;
	node->value = element;

	// Set head to new node or prepend
	if (isEmpty()) {
		head = node;
		tail = node;
	} else {
		node->next = head;
		head = node;
	}

	++size;

	return true;
}

bool SLinkedListWithTail::addBack(const int &element) {
  //	Set new node's value and mark it as end
	Node* node = new Node;
	node->value = element;
	node->next = nullptr;

	if (isEmpty()) {
		head = node;
		tail = node;
	} else {
    // Update tail's pointer and link old tail to new
		tail->next = node;
		tail = tail->next;
	}

	++size;

	return true;
}

bool SLinkedListWithTail::remove(int position) {
	if (position <= 0) {
		return removeFront();
	}

	if (position >= size - 1) {
		return removeBack();
	}

	// Get old's previous and node to remove
	Node* old = getNode(position - 1);

	if (old == nullptr) {
		return false;
	}

  // Skip removed node by linking old's previous with next
	Node* temp = old->next;
	old->next = temp->next;
	delete temp;

	--size;

	return true;
}

bool SLinkedListWithTail::removeFront() {
	if (isEmpty()) {
		return false;
	}

  // Assign head to the next node and delete current head
	Node* temp = head;
	head = temp->next;
	delete temp;

	--size;

	return true;
}

bool SLinkedListWithTail::removeBack() {
	if (isEmpty()) {
		return false;
	}

	Node* current = head;
	Node* previous = new Node;

	while (current->next != nullptr) {
		previous = current;
		current = current->next;
	}

	// Assign new tail which points to nullptr and delete current tail
	tail = previous;
	previous->next = nullptr;
	delete current;

	--size;

	return true;
}

bool SLinkedListWithTail::find(const int &element) {
	for (int i = 0; i < size; ++i) {
		if (getNodeValue(i) == element) {
			return true;
		}
	}

	return false;
}

const int &SLinkedListWithTail::front() const {
	return head->value;
}

const int &SLinkedListWithTail::back() const {
	return tail->value;
}

bool SLinkedListWithTail::isEmpty() const {
	return head == nullptr;
}

Node* SLinkedListWithTail::getNode(int position) const {
	if (isIndexNotValid(position, size)) {
		return nullptr;
	}

	Node* current = head;
	int i = 0;

	while (i < position) {
		current = current->next;
		++i;
	}

	return current;
}

int SLinkedListWithTail::getNodeValue(int position) const {
	Node* node = getNode(position);

	if (node == nullptr) {
		return -1;
	}

	return node->value;
}

int SLinkedListWithTail::getSize() const {
	return size;
}

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_FAST_COMPILE

#include "../../../tests/catch.hpp"

// g++ -o test singly_linked_list/singly_linked_list_tail.cpp ../utils.cpp -std=c++17 && ./test
TEST_CASE("Singly linked list with head and tail") {
	SLinkedListWithTail list;
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
			REQUIRE(list.add(4, 3));
			REQUIRE(list.back() == 4);
			REQUIRE(list.addBack(5));
			REQUIRE(list.back() == 5);
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
			REQUIRE(list.back() == 2);
			REQUIRE(list.removeBack());
			REQUIRE(list.back() == 1);
		}
	}

	SECTION("Find element") {
		REQUIRE(list.find(3));
	}
}