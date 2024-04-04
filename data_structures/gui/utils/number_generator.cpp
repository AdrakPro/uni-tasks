#include "number_generator.h"

int generateNumber(int lower_bound, int upper_bound, int seed) {
	std::mt19937 engine(seed);
	std::uniform_int_distribution<int> dist(lower_bound, upper_bound);

	return dist(engine);
}

int* generateNumbers(int capacity, int seed) {
	int* array = new int[capacity];

	for (int i = 0; i < capacity; ++i) {
		seed += 1;
		array[i] = generateNumber(INT32_MIN, INT32_MAX, seed);
	}

	return array;
}
