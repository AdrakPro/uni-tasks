#ifndef GUI_GENERATORS_H
#define GUI_GENERATORS_H

#include <random>

int generateNumber(int lower_bound, int upper_bound, int seed = 2137);

int* generateNumbers(int data_size, int seed = 2137);

std::string generateString(int length, int seed = 2137);


#endif //GUI_GENERATORS_H
