#include "cuckoo.h"

CuckooHashTable::CuckooHashTable(int capacity) {
	this->capacity = capacity;
	this->size = 0;
	this->table = new Entry* [2];
	this->table[0] = new Entry[capacity];
	this->table[1] = new Entry[capacity];
}

CuckooHashTable::CuckooHashTable(CuckooHashTable &other) {
	capacity = other.capacity;
	size = other.size;

	table = new Entry* [2];
	table[0] = new Entry[capacity];
	table[1] = new Entry[capacity];

	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < capacity; ++j) {
			table[i][j] = other.table[i][j];
		}
	}
}

void CuckooHashTable::setData(const int* data, int data_size) {
	delete[] table[0];
	delete[] table[1];
	delete[] table;

	this->capacity = data_size;
	this->table = new Entry* [2];
	this->table[0] = new Entry[capacity];
	this->table[1] = new Entry[capacity];
	this->size = 0;

	for (int i = 0; i < data_size; ++i) {
		std::string random_key = generateString(12, 50 + i);
		insert(random_key, data[i]);
	}
}

CuckooHashTable::~CuckooHashTable() {
	delete[] table[0];
	delete[] table[1];
	delete[] table;
}

size_t CuckooHashTable::hash(const std::string &key) const {
	return multiplicativeHash(key, capacity);
}

size_t CuckooHashTable::second_hash(const std::string &key) const {
	return multiplicativeHash(key, capacity, 37);
}

void
CuckooHashTable::insertHelper(std::string &key, int value, Entry* table1, Entry* table2) {
	size_t index1 = hash(key) % capacity;
	size_t index2 = second_hash(key) % capacity;

	Entry element = Entry(key, value, true);

	for (int i = 0; i < capacity; ++i) {
		if (!table1[index1].is_occupied) {
			table1[index1] = element;
			return;
		}
		if (!table2[index2].is_occupied) {
			table2[index2] = element;
			return;
		}

		std::swap(key, table1[index1].key);
		std::swap(value, table1[index1].value);

		index1 = hash(key) % capacity;
		std::swap(key, table2[index2].key);
		std::swap(value, table2[index2].value);

		index2 = second_hash(key) % capacity;
	}
}


void CuckooHashTable::insert(const std::string &key, int value) {
	if (static_cast<double>(size) / capacity >= LOAD_FACTOR) {
		resize();
	}

	// Dirty fix for keeping adt key to be const but insert helper std::swap requires non-const key
	std::string k = key;

	insertHelper(k, value, table[0], table[1]);
	++size;
}

void CuckooHashTable::remove(const std::string &key) {
	size_t index1 = hash(key) % capacity;
	size_t index2 = second_hash(key) % capacity;

	if (table[0][index1].is_occupied && table[0][index1].key == key) {
		table[0][index1].is_occupied = false;
		--size;
		return;
	}

	if (table[1][index2].is_occupied && table[1][index2].key == key) {
		table[1][index2].is_occupied = false;
		--size;
		return;
	}
}

int CuckooHashTable::search(const std::string &key) {
	size_t index1 = hash(key) % capacity;
	size_t index2 = second_hash(key) % capacity;

	if (table[0][index1].is_occupied && table[0][index1].key == key) {
		return table[0][index1].value;
	}

	if (table[1][index2].is_occupied && table[1][index2].key == key) {
		return table[1][index2].value;
	}

	return -1;
}

void CuckooHashTable::resize() {
	capacity *= 2;
	auto* first_table = new Entry[capacity];
	auto* second_table = new Entry[capacity];

	for (int i = 0; i < capacity / 2; ++i) {
		if (table[0][i].is_occupied) {
			insertHelper(
					table[0][i].key, table[0][i].value, first_table, second_table
			);
		}

		if (table[1][i].is_occupied) {
			insertHelper(
					table[1][i].key, table[1][i].value, first_table, second_table
			);
		}
	}

	delete[] table[0];
	delete[] table[1];
	table[0] = first_table;
	table[1] = second_table;
}

//#define CATCH_CONFIG_MAIN
//#define CATCH_CONFIG_FAST_COMPILE
//
//#include "../../../tests/catch.hpp"
//
//TEST_CASE("CuckooHashTable Basic Functionality") {
//	CuckooHashTable table(10);
//
//	SECTION("Insertion and Search") {
//		std::string key1 = "key1";
//		std::string key2 = "key2";
//		std::string key3 = "key3";
//
//		table.insert(key1, 100);
//		table.insert(key2, 200);
//		table.insert(key3, 300);
//
//		REQUIRE(table.search(key1) == 100);
//		REQUIRE(table.search(key2) == 200);
//		REQUIRE(table.search(key3) == 300);
//		REQUIRE(table.search("key4") == -1); // key not present
//	}
//
//	SECTION("Insertion and Resize") {
//		for (int i = 0; i < 15; ++i) {
//			std::string key = "key" + std::to_string(i);
//			table.insert(key, i * 10);
//		}
//
//		for (int i = 0; i < 15; ++i) {
//			std::string key = "key" + std::to_string(i);
//			REQUIRE(table.search(key) == i * 10);
//		}
//	}
//
//	SECTION("Remove") {
//		std::string key1 = "key1";
//		std::string key2 = "key2";
//		std::string key3 = "key3";
//
//		table.insert(key1, 100);
//		table.insert(key2, 200);
//		table.insert(key3, 300);
//
//		REQUIRE(table.search(key2) == 200);
//		table.remove(key2);
//		REQUIRE(table.search(key2) == -1);
//		REQUIRE(table.search(key1) == 100);
//		REQUIRE(table.search(key3) == 300);
//	}
//
//	SECTION("Set Data") {
//		int data[] = {10, 20, 30, 40, 50};
//		table.setData(data, 5);
//
//		for (int i = 0; i < 5; ++i) {
//			REQUIRE(table.search(generateString(12, 50 + i)) == data[i]);
//		}
//	}
//
//	SECTION("Copy Constructor") {
//		std::string key1 = "key1";
//		std::string key2 = "key2";
//
//		table.insert(key1, 100);
//		table.insert(key2, 200);
//
//		CuckooHashTable copyTable(table);
//
//		REQUIRE(copyTable.search(key1) == 100);
//		REQUIRE(copyTable.search(key2) == 200);
//		REQUIRE(copyTable.search("key3") == -1);
//	}
//}
//
