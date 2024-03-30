#ifndef GUI_SINGLY_LINKED_LIST_TAIL_H
#define GUI_SINGLY_LINKED_LIST_TAIL_H

#include "../adt.h"
#include "../../utils.h"

class Node {
private:
	int value;
	Node* next;

	// Provide SinglyLinkedList access
	friend class SinglyLinkedListTail;
};

class SinglyLinkedListTail : public ADT {
private:
	Node* head;
	Node* tail;
	int size;

public:
	SinglyLinkedListTail();

	~SinglyLinkedListTail();

	bool add(const int &element, int index) override;

	bool addFront(const int &element) override;

	bool addBack(const int &element) override;

	bool remove(int index) override;

	bool removeFront() override;

	bool removeBack() override;

	bool find(const int &element) override;

	[[nodiscard]] bool isEmpty() const override;

	// First const means the value referred to by the return value cannot be modified.
	// Second const means it doesn't modify the object's state when called.
	[[nodiscard]] const int &front() const;

	[[nodiscard]] const int &back() const;

	[[nodiscard]] Node* getNode(int index) const;

	[[nodiscard]] int getNodeValue(int index) const;

	[[nodiscard]] int getSize() const;

};


#endif //GUI_SINGLY_LINKED_LIST_TAIL_H
