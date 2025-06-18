#include "coalesced.h"

CoalescedHashTable::CoalescedHashTable() = default;

CoalescedHashTable::CoalescedHashTable(const CoalescedHashTable &other) {
	buckets = other.buckets;
	size = other.size;
	table = new CEntry[buckets];

	for (int i = 0; i < buckets; ++i) {
		table[i] = other.table[i];
	}
}

void CoalescedHashTable::setData(const int* data, int data_size) {
	delete[] table;

	this->buckets = data_size;
	this->table = new CEntry[buckets];
	this->size = 0;

	for (int i = 0; i < data_size; ++i) {
		insert(generateString(12, 50 + i), data[i]);
	}

	if (static_cast<double>(size) / buckets >= LOAD_FACTOR) {
		resize();
	}
}

CoalescedHashTable::~CoalescedHashTable() {
	delete[] table;
}

size_t CoalescedHashTable::hash(const std::string &key) const {
	return Hash::multiplicativeHash(key, buckets);
}

void CoalescedHashTable::resize() {
	int old_buckets = buckets;
	buckets = Hash::nextPrime(old_buckets * 2);
	auto* new_table = new CEntry[buckets];

	for (int i = 0; i < old_buckets; ++i) {
		if (table[i].is_occupied) {
			size_t index = hash(table[i].key);

			while (new_table[index].is_occupied) {
				index = (index + 1) % buckets;
			}

			new_table[index] = table[i];
		}
	}

	delete[] table;
	table = new_table;
}

void CoalescedHashTable::insert(const std::string &key, int value) {
	size_t index = hash(key);

	if (!table[index].is_occupied) {
		table[index] = CEntry(key, value, -1, true);
	} else {
		size_t previous_index = index;

		while (table[index].is_occupied && table[index].key != key) {
			previous_index = index;
			index = (index + 1) % buckets;
		}

		table[previous_index].next = static_cast<int>(index);
		table[index] = CEntry(key, value, -1, true);
	}

	++size;
}

void CoalescedHashTable::remove(const std::string &key) {
	size_t index = hash(key) % buckets;
	size_t previous_index = -1;

	while (table[index].is_occupied) {
		if (table[index].key == key) {
			table[index].is_occupied = false;

			if (previous_index != -1) {
				table[previous_index].next = table[index].next;
			}

			--size;
			return;
		}

		previous_index = index;
		index = table[index].next;
	}
}

int CoalescedHashTable::search(const std::string &key) {
	size_t idx = hash(key) % buckets;

	while (table[idx].is_occupied) {
		if (table[idx].key == key) {
			return table[idx].value;
		}
		idx = table[idx].next;
	}

	return -1;
}

int CoalescedHashTable::getBuckets() const {
	return buckets;
}

//#define CATCH_CONFIG_MAIN
//#define CATCH_CONFIG_FAST_COMPILE
//
//#include "../../../tests/catch.hpp"
//
//TEST_CASE("CoalescedHashTable") {
//	CoalescedHashTable table(10);
//
//	SECTION("Insertion and Search") {
//		table.insert("key1", 100);
//		table.insert("key2", 200);
//		table.insert("key3", 300);
//
//		REQUIRE(table.search("key1") == 100);
//		REQUIRE(table.search("key2") == 200);
//		REQUIRE(table.search("key3") == 300);
//		REQUIRE(table.search("key4") == -1); // key not present
//	}
//
//	SECTION("Insertion and Resize") {
//		for (int i = 0; i < 15; ++i) {
//			table.insert("key" + std::to_string(i), i * 10);
//		}
//
//		for (int i = 0; i < 15; ++i) {
//			REQUIRE(table.search("key" + std::to_string(i)) == i * 10);
//		}
//	}
//
//	SECTION("Remove") {
//		table.insert("key1", 100);
//		table.insert("key2", 200);
//		table.insert("key3", 300);
//
//		REQUIRE(table.search("key2") == 200);
//		table.remove("key2");
//		REQUIRE(table.search("key2") == -1); // key should be removed
//		REQUIRE(table.search("key1") == 100); // other keys should remain
//		REQUIRE(table.search("key3") == 300);
//	}
//
//}
