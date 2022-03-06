#pragma once

#include <vector>
#include "RubicsCubeState.h"

namespace Solver {
    vector<Turn> IterativeDeepeningAStar(RubicsCubeState* startState);
    int GetDistanceHeuristic(RubicsCubeState* state, RubicsCubeState* destination);
}