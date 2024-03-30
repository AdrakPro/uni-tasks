#include "utils.h"

bool isPositionNotValid(int position, int upperBound) {
	return position < 0 || position > upperBound;
}