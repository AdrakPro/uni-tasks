#ifndef GUI_DS_H
#define GUI_DS_H

class DS {
public:
	virtual bool add(const int &element, int index) = 0;

	virtual bool addFront(const int &element) = 0;

	virtual bool remove(int index) = 0;

	virtual bool removeFront() = 0;

	virtual bool find(const int &element) = 0;
};

#endif //GUI_DS_H
