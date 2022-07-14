//TODO rename to something more reasonable and rubiks cube related.

#pragma once

#include <iostream>

const int CORNER_ROTATIONS_COUNT = 3 * 3 * 3 * 3 * 3 * 3 * 3; // 3^7
const int CORNER_PERMUTATIONS_COUNT = 8 * 7 * 6 * 5 * 4 * 3 * 2 * 1; // 8!
const int CORNER_STATES_COUNT = CORNER_ROTATIONS_COUNT * CORNER_PERMUTATIONS_COUNT; // 8! * 3^7

const int BIG_EDGE_ROTATION_COUNT = 2 * 2 * 2 * 2 * 2 * 2 * 2; // 2^7
const int BIG_EDGE_PERMUTATIONS_COUNT = 12 * 11 * 10 * 9 * 8 * 7 * 6; // 12! / 5!
const int BIG_EDGE_STATES_COUNT = BIG_EDGE_ROTATION_COUNT * BIG_EDGE_PERMUTATIONS_COUNT; // (12! / 5!) * 2^7

const uint64_t FULL_EDGE_ROTATION_COUNT = 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2; // 2^11
const uint64_t FULL_EDGE_PERMUTATIONS_COUNT = 12 * 11 * 10 * 9 * 8 * 7 * 6 * 5 * 4 * 3 * 2 * 1; // 12!
const uint64_t FULL_EDGE_STATES_COUNT = FULL_EDGE_ROTATION_COUNT * FULL_EDGE_PERMUTATIONS_COUNT; // (12!) * 2^11

const unsigned long long ALL_STATES_COUNT = FULL_EDGE_STATES_COUNT * CORNER_STATES_COUNT;

const int powersOfThree[8] = {
    1, //3^0
    3, 
    3 * 3,
    3 * 3 * 3,
    3 * 3 * 3 * 3,
    3 * 3 * 3 * 3 * 3,
    3 * 3 * 3 * 3 * 3 * 3,
    3 * 3 * 3 * 3 * 3 * 3 * 3, // 3^7
};

const unsigned int powersOfTwo[12] = {
    1,
    2, 
    2 * 2,
    2 * 2 * 2,
    2 * 2 * 2 * 2,
    2 * 2 * 2 * 2 * 2,
    2 * 2 * 2 * 2 * 2 * 2,
    2 * 2 * 2 * 2 * 2 * 2 * 2,
    2 * 2 * 2 * 2 * 2 * 2 * 2 * 2,
    2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2,
    2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2,
    2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2,
};

const int cornerPermutationCoefficients[7] = {
    1 * 2 * 3 * 4 * 5 * 6 * 7, // 7!
    1 * 2 * 3 * 4 * 5 * 6,
    1 * 2 * 3 * 4 * 5,
    1 * 2 * 3 * 4,
    1 * 2 * 3,
    1 * 2,
    1
};