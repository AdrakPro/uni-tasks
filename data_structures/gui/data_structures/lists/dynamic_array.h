#ifndef GUI_DYNAMIC_ARRAY_H
#define GUI_DYNAMIC_ARRAY_H

#include "../ds.h"
#include <cstdlib>
#include <memory>
#include <iostream>
#include <cstring>

class DynamicArray : public DS {
private:
	int* array;
	int size;
	int capacity;

	void resizeArray();
	static bool isPositionNotValid(int position, int upperBound);
public:
	DynamicArray(int* array, int capacity);
	~DynamicArray();

	bool insert(const int &element, int position) override;
	bool remove(int position) override;
	bool find(const int &element) override;

	int getSize() const;
	int getCapacity() const;
	int getElement(int position) const; // Only for testing purpose (I dont have search yet)

	void setSize(int newSize);
};

#endif //GUI_DYNAMIC_ARRAY_H
