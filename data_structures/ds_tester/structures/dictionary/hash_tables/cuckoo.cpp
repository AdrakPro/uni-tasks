#include "cuckoo.h"

CuckooHashTable::CuckooHashTable() = default;

CuckooHashTable::CuckooHashTable(const CuckooHashTable &other) {
	buckets = other.buckets;
	size = other.size;

	table = new Entry* [2];
	table[0] = new Entry[buckets];
	table[1] = new Entry[buckets];

	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < buckets; ++j) {
			table[i][j] = other.table[i][j];
		}
	}
}

void CuckooHashTable::setData(const int* data, int data_size) {
	if (table != nullptr) {
		delete[] table[0];
		delete[] table[1];
		delete[] table;
	}

	this->buckets = data_size;
	this->table = new Entry* [2];
	this->table[0] = new Entry[buckets];
	this->table[1] = new Entry[buckets];
	this->size = 0;

	for (int i = 0; i < data_size; ++i) {
		insert(generateString(12, 50 + i), data[i]);
	}

	if (static_cast<double>(size) / buckets >= LOAD_FACTOR) {
		resize();
	}
}

CuckooHashTable::~CuckooHashTable() {
	delete[] table[0];
	delete[] table[1];
	delete[] table;
}

size_t CuckooHashTable::hash(const std::string &key) const {
	return Hash::multiplicativeHash(key, buckets);
}

size_t CuckooHashTable::secondHash(const std::string &key) const {
	return Hash::multiplicativeHash(key, buckets, 37);
}

void
CuckooHashTable::insertHelper(std::string &key, int value, Entry* table1, Entry* table2) {
	size_t index1 = hash(key) % buckets;
	size_t index2 = secondHash(key) % buckets;

	Entry element = Entry(key, value, true);

	for (int i = 0; i < buckets; ++i) {
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

		index1 = hash(key) % buckets;
		std::swap(key, table2[index2].key);
		std::swap(value, table2[index2].value);

		index2 = secondHash(key) % buckets;
	}
}


void CuckooHashTable::insert(const std::string &key, int value) {
	// Dirty fix for keeping adt key to be const but insert helper std::swap requires non-const key
	std::string k = key;

	insertHelper(k, value, table[0], table[1]);
	++size;
}

void CuckooHashTable::remove(const std::string &key) {
	size_t index1 = hash(key) % buckets;
	size_t index2 = secondHash(key) % buckets;

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
	size_t index1 = hash(key) % buckets;
	size_t index2 = secondHash(key) % buckets;

	if (table[0][index1].is_occupied && table[0][index1].key == key) {
		return table[0][index1].value;
	}

	if (table[1][index2].is_occupied && table[1][index2].key == key) {
		return table[1][index2].value;
	}

	return -1;
}

void CuckooHashTable::resize() {
	int old_buckets = buckets;
	buckets = Hash::nextPrime(old_buckets * 2);
	auto* first_table = new Entry[buckets];
	auto* second_table = new Entry[buckets];

	for (int i = 0; i < old_buckets; ++i) {
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

int CuckooHashTable::getBuckets() const {
	return buckets;
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
