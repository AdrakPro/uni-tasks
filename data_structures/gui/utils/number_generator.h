#ifndef GUI_NUMBER_GENERATOR_H
#define GUI_NUMBER_GENERATOR_H

#include <random>

int generateNumber(int lower_bound, int upper_bound, int seed = 2137);

int* generateNumbers(int capacity, int seed = 2137);


#endif //GUI_NUMBER_GENERATOR_H
