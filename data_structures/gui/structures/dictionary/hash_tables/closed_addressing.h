#ifndef GUI_CLOSED_ADDRESSING_H
#define GUI_CLOSED_ADDRESSING_H

#include "../red_black_tree/red_black_tree.h"
#include "../adt.h"

class CHashTable : Dictionary {
private:
	RBTree* table{};
	int buckets{};

	[[nodiscard]] size_t hash(const std::string &key) const override;

public:
	CHashTable();

	CHashTable(const CHashTable &other);

	~CHashTable();

	void insert(const std::string &key, int value) override;

	void remove(const std::string &key) override;

	int search(const std::string &key) override;

	void setData(const int* data, int data_size);

	void preOrderCopy(RBNode* r, RBNode* n);

	[[nodiscard]] int getBuckets() const;
};


#endif //GUI_CLOSED_ADDRESSING_H
