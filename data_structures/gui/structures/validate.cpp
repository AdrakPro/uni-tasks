#include "validate.h"

bool isIndexNotValid(int index, int upperBound) {
	return index < 0 || index > upperBound;
}