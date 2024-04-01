#ifndef GUI_DOUBLY_LINKED_LIST_H
#define GUI_DOUBLY_LINKED_LIST_H

#include "../adt.h"
#include "../../utils.h"

struct Node {
	int value;
	Node* next;
	Node* prev;
};

class DLinkedList : public ADT {
private:
	Node* head;
	Node* tail;
	int size;

public:
	DLinkedList();

	~DLinkedList();

	bool add(const int &element, int position) override;

	bool addFront(const int &element) override;

	bool addBack(const int &element) override;

	bool remove(int position) override;

	bool removeFront() override;

	bool removeBack() override;

	bool find(const int &element) override;

	[[nodiscard]] bool isEmpty() const override;

	[[nodiscard]] const int &front() const;

	[[nodiscard]] const int &back() const;

	[[nodiscard]] Node* getNode(int position, bool fromHead) const;

	[[nodiscard]] int getNodeValue(int position, bool fromHead) const;

	[[nodiscard]] int getSize() const;

};

#endif //GUI_DOUBLY_LINKED_LIST_H
