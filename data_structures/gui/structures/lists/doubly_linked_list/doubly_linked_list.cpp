#include "doubly_linked_list.h"

DLinkedList::~DLinkedList() {
	DNode* current = head;
	while (current) {
		DNode* next = current->next;
		delete current;
		current = next;
	}
}

// Copy constructor
DLinkedList::DLinkedList(const DLinkedList &other) {
	this->head = nullptr;
	this->tail = nullptr;
	this->size = other.size;

	if (other.head == nullptr) {
		return;
	}

	DNode* temp = other.head;
	auto* new_node = new DNode();
	new_node->value = temp->value;

	head = new_node;
	tail = new_node;
	temp = temp->next;

	while (temp) {
		new_node->next = new DNode();
		new_node->next->value = temp->value;

		new_node->next->prev = new_node;
		new_node = new_node->next;
		tail = new_node;
		temp = temp->next;
	}
}

DLinkedList::DLinkedList() = default;

void DLinkedList::setData(const int* data, int data_size) {
	DNode* current = head;
	while (current) {
		DNode* next = current->next;
		delete current;
		current = next;
	}

	this->head = nullptr;
	this->tail = nullptr;
	this->size = 0;

	for (int i = 0; i < data_size; ++i) {
		add(data[i], i);
	}
}

bool DLinkedList::add(const int &element, int position) {
	if (position == 0) {
		return addFront(element);
	}

	if (position == size) {
		return addBack(element);
	}

	DNode* old = getNode(position - 1, true);

	if (old == nullptr) {
		return false;
	}

	auto* node = new DNode;
	node->value = element;
	node->next = old->next;

	old->next = node;
	node->prev = old;
	node->next->prev = node;

	++size;

	return true;
}

bool DLinkedList::addFront(const int &element) {
	auto* node = new DNode;
	node->value = element;

	if (isEmpty()) {
		head = node;
		tail = head;
	} else {
		node->next = head;
		head->prev = node;
		head = node;
	}

	++size;

	return true;
}

bool DLinkedList::addBack(const int &element) {
	auto* node = new DNode;

	node->value = element;
	node->next = nullptr;

	if (isEmpty()) {
		head = node;
		tail = head;
	} else {
		tail->next = node;
		node->prev = tail;
		tail = node;
	}

	++size;

	return true;
}

bool DLinkedList::remove(int position) {
	if (position == 0) {
		return removeFront();
	}

	if (position == size - 1) {
		return removeBack();
	}

	DNode* old = getNode(position, true);

	if (old == nullptr) {
		return false;
	}


	DNode* next = old->next;
	DNode* prev = old->prev;

	if (prev == nullptr || next == nullptr) {
		return false;
	}

	prev->next = next;
	next->prev = prev;
	delete old;

	return true;
}

bool DLinkedList::removeFront() {
	if (isEmpty()) {
		return false;
	}

	DNode* temp = head;

	if (size == 1) {
		head = nullptr;
		tail = nullptr;
	} else {
		head = temp->next;
		head->prev = nullptr;
	}

	delete temp;

	--size;

	return true;
}

bool DLinkedList::removeBack() {
	if (isEmpty()) {
		return false;
	}

	DNode* temp = tail;

	if (size == 1) {
		head = nullptr;
		tail = nullptr;
	} else {
		tail = temp->prev;
		tail->next = nullptr;
	}

	delete temp;

	--size;

	return true;
}

DNode* DLinkedList::getNode(int position, bool fromHead) const {
	if (isIndexNotValid(position, size)) {
		return nullptr;
	}

	DNode* current;
	int i;

	if (fromHead) {
		i = 0;
		current = head;

		while (i < position) {
			current = current->next;
			++i;
		}
	} else {
		i = size - 1;
		current = tail;

		while (i > position) {
			current = current->prev;
			--i;
		}
	}

	return current;
}

int DLinkedList::getNodeValue(int position, bool fromHead) const {
	DNode* node = getNode(position, fromHead);

	if (node == nullptr) {
		return -1;
	}

	return node->value;
}

bool DLinkedList::isEmpty() const {
	return head == nullptr;
}

const int &DLinkedList::front() const {
	return head->value;
}

const int &DLinkedList::back() const {
	return tail->value;
}

bool DLinkedList::find(const int &element) {
	DNode* current = head;

	while (current != nullptr) {
		if (current->value == element) {
			return true;
		}

		current = current->next;
	}

	return false;
}

int DLinkedList::getSize() const {
	return size;
}

void DLinkedList::display() const {
	DNode* current = head;

	std::cout << " <=> ";
	for (int i = 0; i < size; ++i) {
		std::cout << current->value << " <=> ";
		current = current->next;
	}

	std::cout << std::endl;
}

//#define CATCH_CONFIG_MAIN
//#define CATCH_CONFIG_FAST_COMPILE
//
//#include "../../../tests/catch.hpp"
//
//TEST_CASE("Double linked list") {
//	DLinkedList list;
//	list.add(1, 0);
//	list.add(2, 1);
//	list.add(3, 2);
//
//	SECTION("Add element") {
//		SECTION("Add element to the front") {
//			REQUIRE(list.addFront(4));
//			REQUIRE(list.front() == 4);
//			REQUIRE(list.add(5, 0));
//			REQUIRE(list.front() == 5);
//		}
//
//		SECTION("Add element in middle") {
//			REQUIRE(list.add(4, 1));
//			REQUIRE(list.getNodeValue(1, true) == 4);
//			REQUIRE(list.getNodeValue(2, true) == 2);
//		}
//
//		SECTION("Add element to the end") {
//			REQUIRE(list.addBack(4));
//			REQUIRE(list.getNodeValue(3, false) == 4);
//			REQUIRE(list.back() == 4);
//			REQUIRE(list.addBack(5));
//			REQUIRE(list.back() == 5);
//		}
//	}
//
//	SECTION("Remove element") {
//		SECTION("Remove element from the front") {
//			REQUIRE(list.removeFront());
//			REQUIRE(list.front() == 2);
//			REQUIRE(list.removeFront());
//			REQUIRE(list.front() == 3);
//		}
//
//		SECTION("Remove element in middle") {
//			REQUIRE(list.remove(1));
//			REQUIRE(list.getNodeValue(1, true) == 3);
//			REQUIRE(list.remove(1));
//			REQUIRE(list.getNodeValue(0, true) == 1);
//			list.addBack(5);
//			list.addBack(6);
//			list.addBack(7);
////			list.addBack(6);
////			list.remove();
////			REQUIRE(list.back() == 5);
//			std::cout << list.getSize() << std::endl;
//			std::cout << list.getSize() << std::endl;
//			list.display();
//			list.remove(3);
//			list.display();
//
//		}
//
//		SECTION("Remove element from the end") {
//			REQUIRE(list.remove(2));
//			REQUIRE(list.back() == 2);
//			REQUIRE(list.removeBack());
//			REQUIRE(list.back() == 1);
//		}
//	}
//
//	SECTION("Find element") {
//		REQUIRE(list.find(3));
//	}
//}
