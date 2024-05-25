#ifndef GUI_CLOSED_ADDRESSING_H
#define GUI_CLOSED_ADDRESSING_H

#include "../red_black_tree/red_black_tree.h"
#include "../adt.h"

class CHashTable : Dictionary {
private:
	RBTree* table;
	int capacity;

	[[nodiscard]] size_t hash(const std::string& key) const override;

public:
	explicit CHashTable(int capacity);

	CHashTable(const CHashTable &dict);

	~CHashTable();

	void insert(const std::string &key, int value) override;

	void remove(const std::string &key) override;

	void setData(const int* data, int data_size);

	void preOrderCopy(RBNode* r, RBNode* n);
};


#endif //GUI_CLOSED_ADDRESSING_H
