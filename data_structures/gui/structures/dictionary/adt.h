#ifndef GUI_DICTIONARY_ADT_H
#define GUI_DICTIONARY_ADT_H

#include "../nodes.h"
#include "../../utils/generators.h"
#include <cmath>

const double CONSTANT = (std::sqrt(5) - 1) / 2.0; // Golden ratio

class Dictionary {
	virtual void insert(const std::string &key, int value) = 0;

	virtual void remove(const std::string &key) = 0;

	virtual int search(const std::string &key) = 0;

	[[nodiscard]] virtual size_t hash(const std::string &key) const = 0;
};

class Hash {
public:
	static size_t multiplicativeHash(const std::string &key, int capacity, int multiplier = 31);
};
#endif //GUI_DICTIONARY_ADT_H
