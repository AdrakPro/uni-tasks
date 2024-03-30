#include "singly_linked_list_tail.h"

SinglyLinkedListTail::SinglyLinkedListTail() {
	this->head = nullptr;
	this->tail = nullptr;
	this->size = 0;
}

SinglyLinkedListTail::~SinglyLinkedListTail() {
	Node* current = head;
	while (current != nullptr) {
		Node* next = current->next;
		delete current;
		current = next;
	}
}


bool SinglyLinkedListTail::add(const int &element, int index) {
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

// TODO: Change the bool (interfere with ADT :v)
bool SinglyLinkedListTail::addFront(const int &element) {
	Node* node = new Node;

	node->value = element;

	// Set head to new node or prepend
	if (head == nullptr) {
		head = node;
		tail = node;
	} else {
		node->next = head;
		head = node;
	}
	++size;

	return true;
}

bool SinglyLinkedListTail::addBack(const int &element) {
	Node* node = new Node;

  //	Set new node's value and mark it as end
	node->value = element;
	node->next = nullptr;

	if (head == nullptr) {
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

bool SinglyLinkedListTail::remove(int index) {
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
	old->next = temp->next;
	delete temp;

	--size;

	return true;
}

bool SinglyLinkedListTail::removeFront() {
	if (isEmpty()) {
		return false;
	}

	Node* temp = head;

  // Assign head to the next node and delete current head
	head = temp->next;
	delete temp;

	--size;

	return true;
}

bool SinglyLinkedListTail::removeBack() {
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

	// Assign new tail which points to nullptr and delete current tail
	tail = previous;
	previous->next = nullptr;
	delete current;

	--size;

	return true;
}

bool SinglyLinkedListTail::find(const int &element) {
	for (int i = 0; i < size; ++i) {
		if (getNodeValue(i) == element) {
			return true;
		}
	}

	return false;
}

const int &SinglyLinkedListTail::front() const {
	return head->value;
}

const int &SinglyLinkedListTail::back() const {
	return tail->value;
}

bool SinglyLinkedListTail::isEmpty() const {
	return head == nullptr;
}

Node* SinglyLinkedListTail::getNode(int index) const {
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

int SinglyLinkedListTail::getNodeValue(int index) const {
	Node* node = getNode(index);

	if (node == nullptr) {
		return -1;
	}

	return node->value;
}

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_FAST_COMPILE

#include "../../../tests/catch.hpp"

// g++ -o test singly_linked_list/singly_linked_list_tail.cpp ../utils.cpp -std=c++17 && ./test
TEST_CASE("Singly linked list with head and tail") {
	SinglyLinkedListTail list;
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
			REQUIRE(list.back() == 4);
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
			REQUIRE(list.remove(3));
			REQUIRE(list.back() == 2);
			REQUIRE(list.removeBack());
			REQUIRE(list.back() == 1);
		}
	}

	SECTION("Find element") {
		REQUIRE(list.find(3));
	}
}