#ifndef GUI_BST_QUEUE_H
#define GUI_BST_QUEUE_H

#include "../../nodes.h"
#include "../../../utils/number_generator.h"

#include <iostream>

class BSTQueue {
private:
	BNode* root{};
	int size{};

	BNode* insertHelper(BNode* current, const BNode &new_node);

	void displayHelper(const std::string &prefix, BNode* node, bool isLeft) const;

	BNode* deepCopy(const BNode* node);

	static BNode* minValueNode(BNode* node);

	void destroyTree(BNode* current);

	BNode* vineToTree(BNode* root);

	void treeToVine(BNode* grand);

	int height(BNode* node);

	BNode* deleteNode(BNode* node, int index);
public:
	virtual ~BSTQueue();

	BSTQueue();

	BSTQueue(const BSTQueue &other);

	void insert(const BNode &new_node);

	BNode extractMax();

	[[nodiscard]] int getSize() const;

	[[nodiscard]] BNode peek() const;

	void display();

	void setData(const int* data, int data_size);

	void modifyPriority(int index, int new_priority);

	void balance();

	int getRootHeight();
};

#endif //GUI_BST_QUEUE_H
