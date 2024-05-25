#include "closed_addressing.h"

CHashTable::CHashTable(int capacity) {
	this->table = new RBTree[capacity];
	this->capacity = capacity;
}

CHashTable::CHashTable(const CHashTable &dict) {
	capacity = dict.capacity;
	table = new RBTree[capacity];

	for (int i = 0; i < capacity; ++i) {
		table[i] = dict.table[i];
	}
}

CHashTable::~CHashTable() {
	for (int i = 0; i < capacity; ++i) {
		table[i].destroy();
	}
}

void CHashTable::insert(const std::string &key, int value) {
	size_t index = hash(key);
	auto* node = new RBNode(key, value);

	table[index].insert(node);
	table[index].incrementSize();
}

void CHashTable::remove(const std::string &key) {
	size_t index = hash(key);
	RBNode* node = table[index].containsNode(key);

	table[index].remove(node);
	table[index].decreaseSize();
}

void CHashTable::preOrderCopy(RBNode* r, RBNode* n) {
	if (r->key == n->key) {
		return;
	}

	insert(r->key, r->value);

	preOrderCopy(r->left, n);
	preOrderCopy(r->right, n);
}

size_t CHashTable::hash(const std::string &key) const {
	return multiplicativeHash(key, capacity);
}

void CHashTable::setData(const int* data, int data_size) {
	for (int i = 0; i < capacity; ++i) {
		table[i].destroy();
	}

	delete[] table;

	for (int i = 0; i < data_size; ++i) {
		insert(generateString(12, 50 + i), data[i]);
	}
}
