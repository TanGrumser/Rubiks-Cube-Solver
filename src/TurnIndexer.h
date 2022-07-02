#pragma once

#include <vector>
#include <array>
#include "Turn.h"
#include "Turn.h"
#include "Definitions.h"

namespace TurnIndexer {
    const int MOVE_COUNT = 18;

    uint64 getIndex(int* turnIndicies, int length);
    uint64 getIndexFromVector(vector<Turn> turns);
    uint64 getIndexFromArray(array<Turn, 50> turns, int length);
}