#ifndef GUI_DYNAMIC_ARRAY_H
#define GUI_DYNAMIC_ARRAY_H

#include "../adt.h"

class DynamicArray : public ADT {
private:
	int* array;
	int capacity;
	int size;
	int starting_size;

	void isResizeNeeded();

public:
	~DynamicArray();

	// Copy constructor
	DynamicArray(const DynamicArray& other);

	DynamicArray(const int* array, int capacity);

	bool add(const int &element, int index) override;

	bool addFront(const int &element) override;

	bool addBack(const int &element) override;

	bool remove(int index) override;

	bool removeFront() override;

	bool removeBack() override;

	bool find(const int &element) override;

	void display() const;

	// [[nodiscard]] attribute indicates that the return value should not be ignored by the caller.
	[[nodiscard]] bool isEmpty() const override;

	[[nodiscard]] int getCapacity() const;

	[[nodiscard]] int getSize() const;

	[[nodiscard]] int getElement(int index) const;
};

#endif //GUI_DYNAMIC_ARRAY_H
