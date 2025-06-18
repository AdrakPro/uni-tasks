#ifndef GUI_SINGLY_LINKED_LIST_H
#define GUI_SINGLY_LINKED_LIST_H

#include "../adt.h"

class SLinkedList : public ADT {
private:
	SNode* head{};
	int size{};

public:
	virtual ~SLinkedList();

	// Copy constructor
	SLinkedList(const SLinkedList &other);

	SLinkedList();

	bool add(const int &element, int position) override;

	bool addFront(const int &element) override;

	bool addBack(const int &element) override;

	bool remove(int position) override;

	bool removeFront() override;

	bool removeBack() override;

	bool find(const int &element) override;

	[[nodiscard]] bool isEmpty() const override;

	void setData(const int* data, int data_size) override;

	void display() const override;

	// First const means the value referred to by the return value cannot be modified.
	// Second const means it doesn't modify the object's state when called.
	[[nodiscard]] const int &front() const;

	[[nodiscard]] SNode* getNode(int position) const;

	[[nodiscard]] int getNodeValue(int position) const;

	[[nodiscard]] int getSize() const;
};


#endif //GUI_SINGLY_LINKED_LIST_H
