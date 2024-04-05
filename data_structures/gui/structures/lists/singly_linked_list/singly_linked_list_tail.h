#ifndef GUI_SINGLY_LINKED_LIST_TAIL_H
#define GUI_SINGLY_LINKED_LIST_TAIL_H

#include "../adt.h"

class SLinkedListWithTail : public ADT {
private:
	SNode* head;
	SNode* tail;
	int size;

public:
	~SLinkedListWithTail();

	// Copy constructor
	SLinkedListWithTail(const SLinkedListWithTail &other);

	SLinkedListWithTail();

	SLinkedListWithTail(const int* data, int size);

	bool add(const int &element, int position) override;

	bool addFront(const int &element) override;

	bool addBack(const int &element) override;

	bool remove(int position) override;

	bool removeFront() override;

	bool removeBack() override;

	bool find(const int &element) override;

	void display() const;

	[[nodiscard]] bool isEmpty() const override;

	// First const means the value referred to by the return value cannot be modified.
	// Second const means it doesn't modify the object's state when called.
	[[nodiscard]] const int &front() const;

	[[nodiscard]] const int &back() const;

	[[nodiscard]] SNode* getNode(int position) const;

	[[nodiscard]] int getNodeValue(int position) const;

	[[nodiscard]] int getSize() const;
};


#endif //GUI_SINGLY_LINKED_LIST_TAIL_H
