#include "dynamic_array.h"

DynamicArray::DynamicArray(int* array, int capacity) {
	this->size = 0;
	this->capacity = capacity;
	// Consider	reallocarray() when there is multiplication in the size argument of malloc() or realloc().
  // It checks for integer overflow in multiplication of capacity and sizeof(int)
	// https://man.freebsd.org/cgi/man.cgi?query=reallocarray&sektion=3&manpath=freebsd-release-ports
	this->array = (int*) reallocarray(nullptr, capacity, sizeof(int));
	std::memcpy(this->array, array, capacity * sizeof(int));
}

bool DynamicArray::insert(const int &element, int position) {
	if (size == capacity) {
		resizeArray();
	}

	if (isPositionNotValid(position, capacity)) {
		return false;
	}

	for (int i = size; i > position; --i) {
		array[i] = array[i - 1];
	}

	array[position] = element;
	++size;

	return true;
}

void DynamicArray::resizeArray() {
	int newCapacity = 2 * capacity;
	int* tmp = (int*) reallocarray(array, newCapacity, sizeof(int));

	if (tmp == nullptr) {
		std::cerr << "Failed to resize array." << std::endl;
		exit(EXIT_FAILURE);
	}

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
	return array[position];
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
		SECTION("Insert element at beginning and shift all other elements") {
			REQUIRE(dynamicArray.insert(2137, 0) == true);
			REQUIRE(dynamicArray.getElement(0) == 2137);
			REQUIRE(dynamicArray.getElement(1) == -2);
		}

		SECTION("Insert element in mid and shift all other elements") {
			REQUIRE(dynamicArray.insert(55, 3) == true);
			REQUIRE(dynamicArray.getElement(3) == 55);
			REQUIRE(dynamicArray.getElement(4) == 1);
		}

		SECTION("Insert element at the end") {
			REQUIRE(dynamicArray.insert(22, 8) == true);
			REQUIRE(dynamicArray.getElement(8) == 22);
			REQUIRE(dynamicArray.getElement(9) == 6);
		}

		SECTION("Double capacity if array is full") {
			REQUIRE(dynamicArray.insert(22, 9) == true);
			REQUIRE(dynamicArray.insert(23, 10) == true); // Resize
			REQUIRE(dynamicArray.getElement(10) == 23);
			REQUIRE(dynamicArray.getCapacity() == 20);
		}
	}
}