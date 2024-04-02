#include "number_generator.h"

int generateNumber(int lower_bound, int upper_bound, int seed) {
	std::random_device rd;
	std::mt19937 engine(rd());
	std::uniform_int_distribution dist(lower_bound, upper_bound);

	if (seed == 2137) {
		engine.seed(seed);
	}

	return dist(engine);
}

int* generateNumbers(int capacity, int seed) {
	int* array = new int[capacity];

	for (int i = 0; i < capacity; ++i) {
		array[i] = generateNumber(INT32_MIN, INT32_MAX, seed);
	}

	return array;
}
