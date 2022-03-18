#pragma once

#include <vector>
#include "RubicsCubeState.h"

namespace Solver {
    extern int heuristicMode;

    vector<Turn> IterativeDeepeningAStar(RubicsCubeState* startState);
    int GetDistanceHeuristic(RubicsCubeState* state, RubicsCubeState* destination);
    int** GetEdgeNeighbourIndiciesRotations(RubicsCubeState* state);
}