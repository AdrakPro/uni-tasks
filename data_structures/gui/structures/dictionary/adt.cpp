#include "adt.h"

size_t Hash::multiplicativeHash(const std::string &key, int capacity, int multiplier) {
	uint32_t hash_value = 0;

	for (char c: key) {
		hash_value = hash_value * multiplier + c;
	}

	double frac_part = hash_value * CONSTANT - std::floor(hash_value * CONSTANT);

	return static_cast<size_t>(std::floor(capacity * frac_part));
}

bool Hash::isPrime(int n) {
	if (n <= 1) return false;
	if (n <= 3) return true;
	if (n % 2 == 0 || n % 3 == 0) return false;

	for (int i = 5; i * i <= n; i += 6) {
		if (n % i == 0 || n % (i + 2) == 0) return false;
	}

	return true;
}

int Hash::nextPrime(int n) {
	while (!isPrime(n)) {
		++n;
	}

	return n;
}
