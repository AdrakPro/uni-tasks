#ifndef GUI_DOUBLY_LINKED_LIST_H
#define GUI_DOUBLY_LINKED_LIST_H

#include "../adt.h"

class DLinkedList : public ADT {
private:
	DNode* head{};
	DNode* tail{};
	int size{};

public:
	~DLinkedList();

	// Copy constructor
	DLinkedList(const DLinkedList &other);

	DLinkedList();

	bool add(const int &element, int position) override;

	bool addFront(const int &element) override;

	bool addBack(const int &element) override;

	bool remove(int position) override;

	bool removeFront() override;

	bool removeBack() override;

	bool find(const int &element) override;

	void display() const override;

	void setData(const int* data, int data_size) override;

	[[nodiscard]] bool isEmpty() const override;

	[[nodiscard]] const int &front() const;

	[[nodiscard]] const int &back() const;

	[[nodiscard]] DNode* getNode(int position, bool fromHead) const;

	[[nodiscard]] int getNodeValue(int position, bool fromHead) const;

	[[nodiscard]] int getSize() const;

};

#endif //GUI_DOUBLY_LINKED_LIST_H
