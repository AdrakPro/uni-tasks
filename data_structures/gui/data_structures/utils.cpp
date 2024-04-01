#include "utils.h"
//TODO: check if upperbound size should be size - 1
bool isIndexNotValid(int index, int upperBound) {
	return index < 0 || index > upperBound;
}