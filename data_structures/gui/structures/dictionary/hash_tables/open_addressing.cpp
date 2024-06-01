#include "open_addressing.h"

OHashTable::OHashTable() = default;

OHashTable::OHashTable(const OHashTable &other) {
	buckets = other.buckets;
	size = other.size;

	table = new Entry[buckets];

	for (int i = 0; i < buckets; ++i) {
		table[i] = other.table[i];
	}
}

void OHashTable::setData(const int* data, int data_size) {
	delete[] table;

	this->buckets = data_size;
	this->table = new Entry[buckets];
	this->size = 0;

	for (int i = 0; i < data_size; ++i) {
		insert(generateString(12, 50 + i), data[i]);
	}

	if (static_cast<double>(size) / buckets >= LOAD_FACTOR) {
		resize();
	}
}

OHashTable::~OHashTable() {
	delete[] table;
}

void OHashTable::insert(const std::string &key, int value) {
	size_t index = hash(key);

	while (table[index].is_occupied && table[index].key != key) {
		index = (index + 1) % buckets;
	}

	if (!table[index].is_occupied) {
		++size;
	}

	table[index] = Entry(key, value, true);
}

void OHashTable::remove(const std::string &key) {
	size_t index = hash(key);

	while (table[index].is_occupied) {
		if (table[index].key == key) {
			table[index].is_occupied = false;
			--size;

			return;
		}

		index = (index + 1) % buckets;
	}
}

size_t OHashTable::hash(const std::string &key) const {
	return Hash::multiplicativeHash(key, buckets);
}

int OHashTable::search(const std::string &key) {
	size_t index = hash(key);

	while (table[index].is_occupied) {
		if (table[index].key == key) {
			return table[index].value;
		}

		index = (index + 1) % buckets;
	}

	return -1;
}

void OHashTable::resize() {
	int old_buckets = buckets;
	buckets = Hash::nextPrime(old_buckets * 2);
	auto* tmp = new Entry[buckets];

	for (int i = 0; i < old_buckets; ++i) {
		if (table[i].is_occupied) {
			size_t index = hash(table[i].key);

			while (tmp[index].is_occupied) {
				index = (index + 1) % buckets;
			}

			tmp[index] = table[i];
		}
	}

	delete[] table;
	table = tmp;
}

int OHashTable::getBuckets() const {
	return buckets;
}
