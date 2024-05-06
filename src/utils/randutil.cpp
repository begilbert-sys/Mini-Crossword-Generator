#include "randutil.h"

int randutil::randint(int lower, int upper) {
    return lower + (rand() % ((upper + 1) - lower));
}