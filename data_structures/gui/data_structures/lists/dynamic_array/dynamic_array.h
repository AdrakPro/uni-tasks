#ifndef GUI_DYNAMIC_ARRAY_H
#define GUI_DYNAMIC_ARRAY_H

#include "../adt.h"
#include "../../utils.h"
#include <cstdlib>
#include <cstring>

class DynamicArray : public ADT {
private:
	int* array;
	int size;
	int capacity;
	int start;

	void expand();

	void expandWithOffset();

public:
	DynamicArray(int* array, int capacity);

	~DynamicArray();

	bool add(const int &element, int index) override;

	bool addFront(const int &element) override;

	bool addBack(const int &element) override;

	bool remove(int index) override;

	bool removeFront() override;

	bool removeBack() override;

	bool find(const int &element) override;

	void setSize(int newSize);

	// [[nodiscard]] attribute indicates that the return value should not be ignored by the caller.
	[[nodiscard]] bool isEmpty() const override;

	[[nodiscard]] int getCapacity() const;

	[[nodiscard]] int getSize() const;

	[[nodiscard]] int getElement(int index) const; // Only for testing purpose
};

#endif //GUI_DYNAMIC_ARRAY_H