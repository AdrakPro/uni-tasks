#ifndef GUI_COALESCED_H
#define GUI_COALESCED_H

#include "../adt.h"

class CoalescedHashTable : Dictionary {
private:
	const double LOAD_FACTOR = 0.7;

	CEntry* table{};
	int buckets{};
	int size{};

	void resize();

	[[nodiscard]] size_t hash(const std::string &key) const override;

public:
	CoalescedHashTable();

	CoalescedHashTable(const CoalescedHashTable &other);

	~CoalescedHashTable();

	void insert(const std::string &key, int value) override;

	void remove(const std::string &key) override;

	int search(const std::string &key) override;

	void setData(const int* data, int data_size);

	[[nodiscard]] int getBuckets() const;
};


#endif //GUI_COALESCED_H
