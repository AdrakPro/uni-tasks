#ifndef GUI_ADT_H
#define GUI_ADT_H

#include "../validate.h"
#include <iostream>
#include "../nodes.h"

class ADT {
public:
	virtual bool add(const int &element, int index) = 0;

	virtual bool addFront(const int &element) = 0;

	virtual bool addBack(const int &element) = 0;

	virtual bool remove(int index) = 0;

	virtual bool removeFront() = 0;

	virtual bool removeBack() = 0;

	virtual bool find(const int &element) = 0;

	virtual void setData(const int* data, int data_size) = 0;

	virtual void display() const = 0;

	[[nodiscard]] virtual bool isEmpty() const = 0;
};

#endif //GUI_ADT_H
