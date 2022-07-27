#include <iostream>
#include <algorithm>

#include "Solver.h"
#include "../Model/RubiksCubeState.h"
#include "../LookupTable/LookupTable.h"

namespace Solver {
    int heuristicMode = 0;
    int solverIndex = 0;
    int threadCount = 1;
    int searchedStates = 0;
}

int Solver::GetDistanceHeuristic(RubiksCubeState& from, int bound) {
    int max, estMoves;

    // Check the estimated moves from each database, and return it as soon
    // as one exceeds the bound.
    max = estMoves = LookupTable::GetCornerStateDistance(from);

    if (estMoves > bound)
    return estMoves;


    #ifdef USE_FULL_EDGE_LT
        estMoves = LookupTable::GetEdgeStateDistance(from);

        if (estMoves > bound)
        return estMoves;

        if (estMoves > max)
        max = estMoves;
    #else
        estMoves = LookupTable::GetE2StateDistance(from);

        if (estMoves > bound)
        return estMoves;

        if (estMoves > max)
        max = estMoves;

        // E1 State Lookup
        estMoves = LookupTable::GetE1StateDistance(from);

        if (estMoves > bound)
        return estMoves;

        if (estMoves > max)
        max = estMoves;

        // Edge Permutation Lookup
        estMoves = LookupTable::GetEdgePermutationStateDistance(from);

        if (estMoves > bound)
        return estMoves;

        if (estMoves > max)
        max = estMoves;
    #endif
    
    // Return the max estimate if none exceeds the bound.
    return max;
}

vector<Turn> Solver::GenerateTurnSequenceFromStateSequence(vector<RubiksCubeState> stateSequence) {
    vector<Turn> turnSequence = {};
    RubiksCubeState lastState = stateSequence.back();
    stateSequence.pop_back();

    while (stateSequence.size() != 0) {
        RubiksCubeState currentState = stateSequence.back();
        turnSequence.push_back(currentState.GetTurnTo(lastState));
        stateSequence.pop_back();
        lastState = currentState;
    }    

    std::reverse(turnSequence.begin(), turnSequence.end());
    return turnSequence;
}