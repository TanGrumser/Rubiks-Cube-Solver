#pragma once

#include <vector>
#include <array>
#include "../Model/Turn.h"

namespace TurnIndexer {
    const int MOVE_COUNT = 18;

    uint64_t getIndex(int* turnIndicies, int length);
    uint64_t getIndexFromVector(vector<Turn> turns);
    uint64_t getIndexFromArray(array<Turn, 50> turns, int length);
}