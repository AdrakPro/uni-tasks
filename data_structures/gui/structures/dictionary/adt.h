#ifndef GUI_DICTIONARY_ADT_H
#define GUI_DICTIONARY_ADT_H

#include "../nodes.h"
#include "../../utils/generators.h"
#include <cmath>

const double CONSTANT = (std::sqrt(5) - 1) / 2.0; // Golden ratio

class Dictionary {
	virtual void insert(std::string &key, int value) = 0;

	virtual void remove(const std::string &key) = 0;

	virtual int search(const std::string &key) = 0;

	[[nodiscard]] virtual size_t hash(const std::string &key) const = 0;
};

size_t multiplicativeHash(const std::string &key, int capacity, int multiplier = 31) {
	uint32_t hash_value = 0;

	for (char c: key) {
		hash_value = hash_value * multiplier + c;
	}

	double frac_part = hash_value * CONSTANT - std::floor(hash_value * CONSTANT);

	return static_cast<size_t>(std::floor(capacity * frac_part));
}

#endif //GUI_DICTIONARY_ADT_H
