#include "dynamic_array.h"

DynamicArray::DynamicArray(int* array, int capacity) {
	this->size = capacity;
	this->start = 10;
	this->capacity = capacity + start;
	this->array = new int[capacity];
	std::memcpy(this->array + start, array, this->capacity * sizeof(int));
}

bool DynamicArray::add(const int &element, int position) {
	if (size == capacity) {
		resize();
	}

	if (isPositionNotValid(position, capacity)) {
		return false;
	}

	for (int i = size; i > position; --i) {
		array[start + i] = array[start + i - 1];
	}

	array[start + position] = element;
	++size;

	return true;
}

bool DynamicArray::addFront(const int &element) {
	if (start == 0) {
		int newCapacity = capacity + start;
		int* tmp = new int[newCapacity];

		// Offset array to right by start making space for front
		std::memcpy(tmp + capacity, array, size * sizeof(int));
		delete[] array;

		array = tmp;
		capacity = newCapacity;
		start = capacity - size;
	}

	--start;
	array[start] = element;
	++size;

	return true;
}

void DynamicArray::resize() {
	int newCapacity = 2 * capacity;
	int* tmp = new int[newCapacity];

	delete[] array;
	array = tmp;
	capacity = newCapacity;
}

bool DynamicArray::remove(int position) {}

bool DynamicArray::find(const int &element) {}


DynamicArray::~DynamicArray() {
	free(array);
}

void DynamicArray::setSize(int newSize) {
	this->size = newSize;
}

bool DynamicArray::isPositionNotValid(int position, int upperBound) {
	return position < 0 && position > upperBound;
}

int DynamicArray::getElement(int position) const {
	return array[start + position];
}

int DynamicArray::getSize() const {
	return size;
}

int DynamicArray::getCapacity() const {
	return capacity;
}

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_FAST_COMPILE

#include "../../tests/catch.hpp"

TEST_CASE("DynamicArray Test") {
	int data[10] = {-2, -1, 0, 1, 2, 3, 4, 5, 6};
	DynamicArray dynamicArray(data, 10);
	dynamicArray.setSize(9);

	SECTION("Insert element") {
		SECTION("Insert element at beginning") {
			REQUIRE(dynamicArray.addFront(2137) == true);
			REQUIRE(dynamicArray.getElement(0) == 2137);
			REQUIRE(dynamicArray.getElement(1) == -2);
			REQUIRE(dynamicArray.addFront(2000) == true);
			REQUIRE(dynamicArray.getElement(0) == 2000);
			REQUIRE(dynamicArray.getElement(1) == 2137);
		}

		SECTION("Insert element in mid and shift all other elements") {
			REQUIRE(dynamicArray.add(55, 3) == true);
			REQUIRE(dynamicArray.getElement(3) == 55);
			REQUIRE(dynamicArray.getElement(4) == 1);
		}

		SECTION("Insert element at the end") {
			REQUIRE(dynamicArray.add(22, 8) == true);
			REQUIRE(dynamicArray.getElement(8) == 22);
			REQUIRE(dynamicArray.getElement(9) == 6);
		}

		SECTION("Double capacity if array is full") {
			REQUIRE(dynamicArray.add(22, 9) == true);
			REQUIRE(dynamicArray.add(23, 10) == true); // Resize
			REQUIRE(dynamicArray.getElement(10) == 23);
			REQUIRE(dynamicArray.getCapacity() == 20);
		}
	}
}