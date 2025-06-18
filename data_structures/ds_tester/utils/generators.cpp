#include "generators.h"

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

std::string generateString(int length, int seed) {
	const std::string CHARACTERS
			= "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuv"
				"wxyz0123456789";

	std::mt19937 gen(seed);

	std::uniform_int_distribution<int> dist(
			0, CHARACTERS.size() - 1
	);

	std::string random_string;

	for (int i = 0; i < length; ++i) {
		seed += 1;
		gen.seed(seed);
		random_string += CHARACTERS[dist(gen)];
	}

	return random_string;
}
