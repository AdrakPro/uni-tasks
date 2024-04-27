#ifndef GUI_HEAP_QUEUE_H
#define GUI_HEAP_QUEUE_H

#include "../adt.h"
#include "../../../utils/number_generator.h"

#include <iostream>

// Type MAX which means we delete the element with the highest priority
class MaxHeapPriorityQueue {
private:
	QNode* heap{};
	int capacity{};
	int size{};

	void isResizeNeeded();

	void swap(int old_index, int new_index);

	void heapifyDown(int index);

	void heapifyUp(int index);


public:
	~MaxHeapPriorityQueue();

	MaxHeapPriorityQueue();

	MaxHeapPriorityQueue(const MaxHeapPriorityQueue &other);

	void insert(const QNode &node);

	QNode extractMax();

	QNode peek();

	bool modifyKey(int index, int new_priority);

	[[nodiscard]] int getSize() const;

	void display();

	void setData(const int* data, int data_size);

	QNode getDataByIndex(int index);
};

#endif //GUI_HEAP_QUEUE_H
