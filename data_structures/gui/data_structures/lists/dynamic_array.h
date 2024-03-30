#ifndef GUI_DYNAMIC_ARRAY_H
#define GUI_DYNAMIC_ARRAY_H

#include "../ds.h"
#include <cstdlib>
#include <cstring>

class DynamicArray : public DS {
private:
	int* array;
	int size;
	int capacity;
	int start;

	void resize();

	static bool isPositionNotValid(int position, int upperBound);

public:
	DynamicArray(int* array, int capacity);

	~DynamicArray();

	bool add(const int &element, int position) override;

	bool addFront(const int &element) override;

	bool remove(int position) override;

	bool removeFront() override;

	bool find(const int &element) override;

  // [[nodiscard]] attribute indicates that the return value should not be ignored by the caller.
	[[nodiscard]] int getCapacity() const;

	[[nodiscard]] int getElement(int position) const; // Only for testing purpose

	void setSize(int newSize);
};

#endif //GUI_DYNAMIC_ARRAY_H
