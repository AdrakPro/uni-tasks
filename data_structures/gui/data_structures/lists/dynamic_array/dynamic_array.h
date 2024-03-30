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

	void resize();
public:
	DynamicArray(int* array, int capacity);

	~DynamicArray();

	bool add(const int &element, int position) override;

	bool addFront(const int &element) override;

	bool remove(int position) override;

	bool removeFront() override;

	bool find(const int &element) override;

	void setSize(int newSize);

	// [[nodiscard]] attribute indicates that the return value should not be ignored by the caller.
	[[nodiscard]] bool isEmpty() const override;

	[[nodiscard]] int getCapacity() const;

	[[nodiscard]] int getElement(int position) const; // Only for testing purpose
};

#endif //GUI_DYNAMIC_ARRAY_H
