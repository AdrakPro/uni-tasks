#ifndef GUI_SINGLY_LINKED_LIST_H
#define GUI_SINGLY_LINKED_LIST_H

#include "../adt.h"
#include "../../utils.h"

struct Node {
	int value;
	Node* next;
};

class SLinkedList : public ADT {
private:
	Node* head;
	int size;

public:
	SLinkedList();

	~SLinkedList();

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

	[[nodiscard]] Node* getNode(int index) const;

	[[nodiscard]] int getNodeValue(int index) const;
};


#endif //GUI_SINGLY_LINKED_LIST_H
