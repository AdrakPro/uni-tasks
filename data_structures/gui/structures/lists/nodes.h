#ifndef GUI_NODES_H
#define GUI_NODES_H

struct SNode {
	int value;
	SNode* next;
};

struct DNode {
	int value;
	DNode* next;
	DNode* prev;
};

#endif //GUI_NODES_H
