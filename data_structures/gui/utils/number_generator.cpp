#include "number_generator.h"

int generateNumber(int lower_bound, int upper_bound, int seed) {
	std::mt19937 engine(seed);
	std::uniform_int_distribution<int> dist(lower_bound, upper_bound);

	return dist(engine);
}

int* generateNumbers(int data_size, int seed) {
	int* array = new int[data_size];

	for (int i = 0; i < data_size; ++i) {
		seed += 1;
		array[i] = generateNumber(INT32_MIN, INT32_MAX, seed);
	}

	return array;
}
