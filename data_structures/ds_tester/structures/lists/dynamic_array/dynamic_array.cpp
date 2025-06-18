#include "dynamic_array.h"

DynamicArray::~DynamicArray() {
	delete[] array;
}

// Copy constructor
DynamicArray::DynamicArray(const DynamicArray &other) {
	this->size = other.size;
	this->capacity = other.capacity;
	this->starting_size = other.starting_size;
	this->array = new int[other.capacity];

	for (int i = 0; i < other.size; ++i) {
		this->array[i] = other.array[i];
	}
}

DynamicArray::DynamicArray() = default;

void DynamicArray::setData(const int* data, int data_size) {
	delete[] array;

	this->starting_size = data_size;
	this->capacity = data_size;
	this->array = new int[data_size];
	this->size = 0;

	for (int i = 0; i < data_size; ++i) {
		this->array[i] = data[i];
		this->size++;
	}
}

bool DynamicArray::add(const int &element, int index) {
	if (isIndexNotValid(index, capacity)) {
		return false;
	}

	isResizeNeeded();

	for (int i = size; i > index; --i) {
		array[i] = array[i - 1];
	}

	array[index] = element;

	++size;

	return true;
}

bool DynamicArray::addFront(const int &element) {
	return add(element, 0);
}

bool DynamicArray::addBack(const int &element) {
	isResizeNeeded();

	array[size] = element;

	++size;

	return true;
}

bool DynamicArray::remove(int index) {
	if (isIndexNotValid(index, capacity)) {
		return false;
	}

	for (int i = index; i < size - 1; ++i) {
		array[i] = array[i + 1];
	}

	--size;

	isResizeNeeded();

	return true;
}

bool DynamicArray::removeFront() {
	return remove(0);
}

bool DynamicArray::removeBack() {
	if (isEmpty()) {
		return false;
	}

	--size;

	isResizeNeeded();

	return true;
}


bool DynamicArray::find(const int &element) {
	for (int i = 0; i < size; ++i) {
		if (array[i] == element) {
			return true;
		}
	}

	return false;
}


void DynamicArray::isResizeNeeded() {
	if (capacity == size) {
		capacity *= 2;
		int* tmp = new int[capacity];

		for (int i = 0; i < size; ++i) {
			tmp[i] = array[i];
		}

		array = tmp;

		return;
	}

	if (size <= capacity / 4 && capacity >= starting_size * 2) {
		capacity /= 2;
		int* tmp = new int[capacity];

		for (int j = 0; j < size; ++j) {
			tmp[j] = array[j];
		}

		return;
	}
}

int DynamicArray::getElement(int index) const {
	return array[index];
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

void DynamicArray::display() const {
	for (int i = 0; i < size; ++i) {
		std::cout << "[" << i << "] " << array[i] << " ";
	}

	std::cout << std::endl;
}

//#define CATCH_CONFIG_MAIN
//#define CATCH_CONFIG_FAST_COMPILE
//
//#include "../../../tests/catch.hpp"
//
//TEST_CASE("DynamicArray Test") {
//	int data[10] = {-2, -1, 0, 1, 2, 3, 4, 5, 6, 7};
//	DynamicArray dynamicArray(data, 10);
//
//	SECTION("Add element") {
//		SECTION("Add element at beginning") {
//			REQUIRE(dynamicArray.addFront(2137));
//			REQUIRE(dynamicArray.getElement(0) == 2137);
//			REQUIRE(dynamicArray.getElement(1) == -2);
//			REQUIRE(dynamicArray.addFront(2000));
//			REQUIRE(dynamicArray.getElement(0) == 2000);
//			REQUIRE(dynamicArray.getElement(1) == 2137);
//		}
//
//		SECTION("Add element in mid and shift all other elements") {
//			REQUIRE(dynamicArray.add(55, 3));
//			REQUIRE(dynamicArray.getElement(3) == 55);
//			REQUIRE(dynamicArray.getElement(4) == 1);
//		}
//
//		SECTION("Add element at the end") {
//			REQUIRE(dynamicArray.addBack(22));
//			REQUIRE(dynamicArray.getElement(10) == 22);
//		}
//
//		SECTION("Double capacity if array is full") {
//			REQUIRE(dynamicArray.addBack(23)); // Resize
//			REQUIRE(dynamicArray.getElement(10) == 23);
//			REQUIRE(dynamicArray.getCapacity() == 20);
//		}
//	}
//
//	SECTION("Remove element") {
//		SECTION("Remove element at beginning") {
//			REQUIRE(dynamicArray.removeFront());
//			REQUIRE(dynamicArray.getElement(0) == -1);
//			REQUIRE(dynamicArray.remove(0));
//			REQUIRE(dynamicArray.getElement(0) == 0);
//		}
//
//		SECTION("Remove at the middle") {
//			REQUIRE(dynamicArray.remove(3));
//			REQUIRE(dynamicArray.getElement(3) == 2);
//		}
//
//		SECTION("Remove element at the end") {
//			REQUIRE(dynamicArray.removeBack());
//			REQUIRE(dynamicArray.getSize() == 9);
//		}
//	}
//
//	SECTION("Find element") {
//		REQUIRE(dynamicArray.find(5));
//		REQUIRE(dynamicArray.find(100) == false);
//	}
//}
