#ifndef GUI_CUCKOO_H
#define GUI_CUCKOO_H

#include "../adt.h"

class CuckooHashTable : Dictionary {
private:
	const double LOAD_FACTOR = 0.5;

	Entry** table{};
	int buckets{};
	int size{};

	[[nodiscard]] size_t hash(const std::string &key) const override;

	[[nodiscard]] size_t second_hash(const std::string &key) const;

	void insertHelper(std::string &key, int value, Entry* table1, Entry* table2);

	void resize();

public:
	CuckooHashTable();

	CuckooHashTable(const CuckooHashTable &other);

	~CuckooHashTable();

	void insert(const std::string &key, int value) override;

	void remove(const std::string &key) override;

	int search(const std::string &key) override;

	void setData(const int* data, int data_size);

	[[nodiscard]] int getBuckets() const;
};


#endif //GUI_CUCKOO_H
