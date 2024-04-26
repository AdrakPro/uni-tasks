#include "heap_queue.h"

MaxHeapPriorityQueue::~MaxHeapPriorityQueue() {
	delete[] heap;
}

MaxHeapPriorityQueue::MaxHeapPriorityQueue(const MaxHeapPriorityQueue &other) {
	this->size = other.size;
	this->capacity = other.capacity;
	this->heap = new QNode[other.capacity];

	for (int i = 0; i < other.size; ++i) {
		this->heap[i] = other.heap[i];
	}
}

MaxHeapPriorityQueue::MaxHeapPriorityQueue() = default;

void MaxHeapPriorityQueue::isResizeNeeded() {
	if (capacity == size) {
		capacity *= 2;
		auto* tmp = new QNode[capacity];

		for (int i = 0; i < size; ++i) {
			tmp[i] = heap[i];
		}

		heap = tmp;
	}
}

void MaxHeapPriorityQueue::swap(int old_index, int new_index) {
	if (old_index != new_index) {
		QNode tmp = heap[old_index];
		heap[old_index] = heap[new_index];
		heap[new_index] = tmp;
	}
}

void MaxHeapPriorityQueue::display() {
	int power = 0;
	int value = 1;
	for (int i = 0; i < size; i++) {
		if (i == value) {
			std::cout << std::endl;
			power += 1;
			value += (1 << power);
		}
		std::cout << heap[i].getValue() << " (" << heap[i].getPriority() << ") ";
	}
	std::cout << std::endl;
}

void MaxHeapPriorityQueue::setData(const int* data, int data_size) {
	delete[] heap;

	this->capacity = data_size;
	this->heap = new QNode[capacity];
	this->size = 0;

	for (int i = 0; i < data_size; ++i) {
		int priority = generateNumber(0, 2 * data_size, 2137 * i);
		QNode node(i, data[i], priority);
		insert(node);
	}
}

void MaxHeapPriorityQueue::heapifyUp(int index) {
	while (index > 0) {
		int parent = (index - 1) / 2;
		if (heap[parent].getPriority() < heap[index].getPriority() ||
				(heap[parent].getPriority() == heap[index].getPriority() &&
				 heap[parent].getIndex() < heap[index].getIndex())) {
			swap(parent, index);
			index = parent;
		} else {
			break;
		}
	}
}

void MaxHeapPriorityQueue::heapifyDown(int index) {
	int max = index;
	int left = 2 * index + 1;
	int right = 2 * index + 2;

	if (left < size &&
			(heap[left].getPriority() > heap[max].getPriority() ||
			 (heap[left].getPriority() == heap[max].getPriority() &&
				heap[left].getIndex() > heap[max].getIndex()))) {
		max = left;
	}

	if (right < size &&
			(heap[right].getPriority() > heap[max].getPriority() ||
			 (heap[right].getPriority() == heap[max].getPriority() &&
				heap[right].getIndex() > heap[max].getIndex()))) {
		max = right;
	}

	if (index != max) {
		swap(index, max);
		heapifyDown(max);
	}
}

void MaxHeapPriorityQueue::insert(const QNode &node) {
	isResizeNeeded();
	++size;
	heap[size - 1] = node;
	heapifyUp(size - 1);
}

QNode MaxHeapPriorityQueue::extractMax() {
	QNode max{};

	if (size == 0) {
		return max;
	}

	max = heap[0];
	heap[0] = heap[size - 1];
	--size;
	heapifyDown(0);

	return max;
}

// change to isIndexNotValid
bool MaxHeapPriorityQueue::modifyKey(int index, int new_priority) {
	if (index < 0 || index >= size) {
		return false;
	}

	int old_priority = heap[index].getPriority();
	heap[index].setPriority(new_priority);

	if (new_priority > old_priority) {
		heapifyUp(index);
	} else {
		heapifyDown(index);
	}

	return true;
}


QNode MaxHeapPriorityQueue::getDataByIndex(int index) {
	return heap[index];
}

int MaxHeapPriorityQueue::getSize() const {
	return size;
}

QNode MaxHeapPriorityQueue::peek() {
	QNode peeked{};

	if (size == 0) {
		return peeked;
	}

	return heap[0];
}


//#define CATCH_CONFIG_MAIN
//#define CATCH_CONFIG_FAST_COMPILE
//
//#include "../../../tests/catch.hpp"
//
//TEST_CASE("Heap queue") {
//	const int data[] = {30, 3, 2, 4, 7};
//	const int data_size = 5;
//
//	SECTION("Extract max") {
//		MaxHeapPriorityQueue heap_queue;
//		heap_queue.setData(data, data_size);
//
//		heap_queue.display();
//		REQUIRE(heap_queue.extractMax().getPriority() == 8);
//		REQUIRE(heap_queue.getSize() == data_size - 1);
//
//		REQUIRE(heap_queue.extractMax().getPriority() == 8);
//		REQUIRE(heap_queue.getSize() == data_size - 2);
//
//		REQUIRE(heap_queue.peek().getPriority() == 6);
//	}
//
//	SECTION("Modify key") {
//		MaxHeapPriorityQueue heap_queue;
//		heap_queue.setData(data, data_size);
//
//		REQUIRE(heap_queue.modifyKey(3, 90) == true);
//		REQUIRE(heap_queue.peek().getPriority() == 90);
//
//		// Attempting to modify out of range index
//		REQUIRE(heap_queue.modifyKey(10, 50) == false);
//	}
//
//	SECTION("Get data by index") {
//		MaxHeapPriorityQueue heap_queue;
//		heap_queue.setData(data, data_size);
//
//		REQUIRE(heap_queue.getDataByIndex(3).getPriority() == 1);
//	}
//
//	SECTION("Peek") {
//		MaxHeapPriorityQueue heap_queue;
//		heap_queue.setData(data, data_size);
//
//		REQUIRE(heap_queue.peek().getPriority() == 8);
//
//		heap_queue.extractMax();
//		REQUIRE(heap_queue.peek().getPriority() == 8);
//
//		heap_queue.extractMax();
//		REQUIRE(heap_queue.peek().getPriority() == 6);
//	}
//}