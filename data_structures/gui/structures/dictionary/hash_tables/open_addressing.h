#ifndef GUI_OPEN_ADDRESSING_H
#define GUI_OPEN_ADDRESSING_H

#include "../adt.h"

class OHashTable : Dictionary {
private:
	const double LOAD_FACTOR = 0.7;

	Entry* table{};
	int buckets{};
	int size{};

	void resize();

	[[nodiscard]] size_t hash(const std::string &key) const override;

public:
	OHashTable();

	OHashTable(const OHashTable &other);

	~OHashTable();

	void insert(const std::string &key, int value) override;

	void remove(const std::string &key) override;

	int search(const std::string &key) override;

	void setData(const int* data, int data_size);

	[[nodiscard]] int getBuckets() const;
};


#endif //GUI_OPEN_ADDRESSING_H
