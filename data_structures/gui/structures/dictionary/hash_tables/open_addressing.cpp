#include "open_addressing.h"

OHashTable::OHashTable() = default;

OHashTable::OHashTable(const OHashTable &other) {
	capacity = other.capacity;
	size = other.size;

	table = new Entry[capacity];

	for (int i = 0; i < capacity; ++i) {
		table[i] = other.table[i];
	}
}

void OHashTable::setData(const int* data, int data_size) {
	delete[] table;

	this->capacity = data_size;
	this->table = new Entry[capacity];
	this->size = 0;

	for (int i = 0; i < data_size; ++i) {
		insert(generateString(12, 50 + i), data[i]);
	}
}


OHashTable::~OHashTable() {
	delete[] table;
}

void OHashTable::insert(const std::string &key, int value) {
	if (static_cast<double>(size) / capacity >= LOAD_FACTOR) {
		resize();
	}

	size_t index = hash(key) % capacity;

	while (table[index].is_occupied && table[index].key != key) {
		index = (index + 1) % capacity;
	}

	if (!table[index].is_occupied) {
		++size;
	}

	table[index] = Entry(key, value, true);
}

void OHashTable::remove(const std::string &key) {
	size_t index = hash(key) % capacity;

	while (table[index].is_occupied) {
		if (table[index].key == key) {
			table[index].is_occupied = false;
			--size;

			return;
		}

		index = (index + 1) % capacity;
	}
}

size_t OHashTable::hash(const std::string &key) const {
	return Hash::multiplicativeHash(key, capacity);
}

int OHashTable::search(const std::string &key) {
	size_t index = hash(key) % capacity;

	while (table[index].is_occupied) {
		if (table[index].key == key) {
			return table[index].value;
		}

		index = (index + 1) % capacity;
	}

	return -1;
}

void OHashTable::resize() {
	int new_capacity = capacity * 2;
	auto* tmp = new Entry[new_capacity];

	for (int i = 0; i < capacity; ++i) {
		if (table[i].is_occupied) {
			size_t index = hash(table[i].key) % new_capacity;

			while (tmp[index].is_occupied) {
				index = (index + 1) % new_capacity;
			}

			tmp[index] = table[i];
		}
	}

	delete[] table;
	table = tmp;
	capacity = new_capacity;
}

