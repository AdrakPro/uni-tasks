#include "number_generator.h"
#include <iostream>

std::vector<int> generate_numbers(int size, int seed) {
	std::random_device rd;
	std::mt19937 engine(rd());

	if (seed == 2137) {
		engine.seed(seed);
	}

	std::uniform_int_distribution<int> distribution(INT32_MIN, INT32_MAX);
	std::vector<int> random_numbers(size);
	for (int i = 0; i < size; ++i) {
		random_numbers[i] = distribution(engine);
		std::cout << random_numbers[i] << std::endl;
	}

	return random_numbers;
}
