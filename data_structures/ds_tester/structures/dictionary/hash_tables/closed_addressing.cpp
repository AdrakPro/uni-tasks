#include "closed_addressing.h"

CHashTable::CHashTable() = default;

CHashTable::CHashTable(const CHashTable &other) {
	buckets = other.buckets;
	table = new RBTree[buckets];

	for (int i = 0; i < buckets; ++i) {
		preOrderCopy(other.table[i].getRoot(), other.table[i].getNil());
	}
}

void CHashTable::setData(const int* data, int data_size) {
	delete[] table;

	this->buckets = data_size;
	this->table = new RBTree[buckets];

	for (int i = 0; i < data_size; ++i) {
		insert(generateString(12, 50 + i), data[i]);
	}
}

CHashTable::~CHashTable() {
	for (int i = 0; i < buckets; ++i) {
		table[i].destroy();
	}

	delete[] table;
}

void CHashTable::insert(const std::string &key, int value) {
	size_t index = hash(key);
	auto* node = new RBNode(key, value);

	this->table[index].insert(node);
	this->table[index].incrementSize();
}

void CHashTable::remove(const std::string &key) {
	size_t index = hash(key);
	auto* node = table[index].containsNode(key);

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
	return Hash::multiplicativeHash(key, buckets);
}

int CHashTable::getBuckets() const {
	return buckets;
}

// todo
int CHashTable::search(const std::string &key) {
	return -1;
}