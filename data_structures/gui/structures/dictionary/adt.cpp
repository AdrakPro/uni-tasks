#include "adt.h"

size_t Hash::multiplicativeHash(const std::string &key, int capacity, int multiplier) {
	uint32_t hash_value = 0;

	for (char c: key) {
		hash_value = hash_value * multiplier + c;
	}

	double frac_part = hash_value * CONSTANT - std::floor(hash_value * CONSTANT);

	return static_cast<size_t>(std::floor(capacity * frac_part));
}
