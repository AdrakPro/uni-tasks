#include "dynamic_array.h"

DynamicArray::DynamicArray(int* array, int capacity) {
	this->size = capacity;
	this->start = 10;
	this->capacity = capacity + start;
	this->array = new int[capacity];
	// Copy elements from array to this->array, leaving the space in front
	std::memcpy(this->array + start, array, this->capacity * sizeof(int));
}

DynamicArray::~DynamicArray() {
	delete[] array;
}

bool DynamicArray::add(const int &element, int index) {
	if (size == capacity) {
		expand();
	}

	if (isIndexNotValid(index, capacity)) {
		return false;
	}

	// Shift elements to the right to make space for new element
	for (int i = size; i > index; --i) {
		array[start + i] = array[start + i - 1];
	}

	array[start + index] = element;
	++size;

	return true;
}

bool DynamicArray::addFront(const int &element) {
	if (start == 0) {
		expandWithOffset();
	}

	--start;
	array[start] = element;
	++size;

	return true;
}

bool DynamicArray::addBack(const int &element) {
	if (size == capacity) {
		expand();
	}

	array[start + size] = element;
	++size;

	return true;
}

bool DynamicArray::remove(int index) {
	if (isIndexNotValid(index, capacity)) {
		return false;
	}

	// Shift elements to the left to remove the element at index
	for (int i = index; i < size; ++i) {
		array[start + i] = array[start + i + 1];
	}

	--size;

	return true;
}

bool DynamicArray::removeFront() {
	bool result = remove(0);

	if (result) {
		++start;
	}

	return result;
}

bool DynamicArray::removeBack() {
	return remove(size - 1);
}


bool DynamicArray::find(const int &element) {
	for (int i = 0; i < size; ++i) {
		if (array[start + i] == element) {
			return true;
		}
	}
	return false;
}

void DynamicArray::expand() {
	int newCapacity = 2 * capacity;
	int* tmp = new int[newCapacity];

	delete[] array;
	array = tmp;
	capacity = newCapacity;
}

void DynamicArray::expandWithOffset() {
	int newCapacity = capacity + start;
	int* tmp = new int[newCapacity];

	// Offset array to right by start making space for front
	std::memcpy(tmp + capacity, array, size * sizeof(int));
	delete[] array;

	array = tmp;
	capacity = newCapacity;
	start = capacity - size;
}

void DynamicArray::setSize(int newSize) {
	this->size = newSize;
}

int DynamicArray::getElement(int index) const {
	return array[start + index];
}

int DynamicArray::getCapacity() const {
	return capacity;
}

bool DynamicArray::isEmpty() const {
	return size == 0;
}

int DynamicArray::getSize() const {
	return size;
}

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_FAST_COMPILE

#include "../../../tests/catch.hpp"

TEST_CASE("DynamicArray Test") {
	int data[10] = {-2, -1, 0, 1, 2, 3, 4, 5, 6};
	DynamicArray dynamicArray(data, 10);
	dynamicArray.setSize(9);

	SECTION("Add element") {
		SECTION("Add element at beginning") {
			REQUIRE(dynamicArray.addFront(2137));
			REQUIRE(dynamicArray.getElement(0) == 2137);
			REQUIRE(dynamicArray.getElement(1) == -2);
			REQUIRE(dynamicArray.addFront(2000));
			REQUIRE(dynamicArray.getElement(0) == 2000);
			REQUIRE(dynamicArray.getElement(1) == 2137);
		}

		SECTION("Add element in mid and shift all other elements") {
			REQUIRE(dynamicArray.add(55, 3));
			REQUIRE(dynamicArray.getElement(3) == 55);
			REQUIRE(dynamicArray.getElement(4) == 1);
		}

		SECTION("Add element at the end") {
			REQUIRE(dynamicArray.addBack(22));
			REQUIRE(dynamicArray.getElement(9) == 22);
		}

		SECTION("Double capacity if array is full") {
			REQUIRE(dynamicArray.addBack(22));
			REQUIRE(dynamicArray.addBack(23)); // Resize
			REQUIRE(dynamicArray.getElement(10) == 23);
			REQUIRE(dynamicArray.getCapacity() == 20);
		}
	}

	SECTION("Remove element") {
		SECTION("Remove element at beginning") {
			REQUIRE(dynamicArray.removeFront());
			REQUIRE(dynamicArray.getElement(0) != -2);
		}

		SECTION("Remove at the middle") {
			REQUIRE(dynamicArray.remove(3));
			REQUIRE(dynamicArray.getElement(3) == 2);
		}

		SECTION("Remove element at the end") {
			REQUIRE(dynamicArray.removeBack());
			REQUIRE(dynamicArray.getElement(8) == 0);
		}
	}

	SECTION("Find element") {
		REQUIRE(dynamicArray.find(5));
		REQUIRE(dynamicArray.find(100) == false);
	}
}