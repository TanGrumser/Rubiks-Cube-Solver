#include "PermutationIndexer.h"

const int factorialCoefficients[] = {
    1,
    1,
    1 * 2,
    1 * 2 * 3,
    1 * 2 * 3 * 4,
    1 * 2 * 3 * 4 * 5,
    1 * 2 * 3 * 4 * 5 * 6,
    1 * 2 * 3 * 4 * 5 * 6 * 7, 
    1 * 2 * 3 * 4 * 5 * 6 * 7 * 8, 
    1 * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9, 
    1 * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9 * 10, 
    1 * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9 * 10 * 11, 
    1 * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9 * 10 * 11 * 12, 
};

unsigned pick(unsigned n, unsigned k) {
  return factorialCoefficients[n] / factorialCoefficients[n - k];
}