#include <iostream>
#include <algorithm>

#include "Solver.h"
#include "../Model/RubicsCubeState.h"
#include "../LookupTable/LookupTable.h"

namespace Solver {
    int heuristicMode = 0;
    int solverIndex = 0;
    int threadCount = 1;
    int searchedStates = 0;
}

int Solver::GetDistanceHeuristic(RubicsCubeState& from, int bound) {
    int max, estMoves;

    // Check the estimated moves from each database, and return it as soon
    // as one exceeds the bound.
    max = estMoves = LookupTable::GetCornerStateDistance(from);

    if (estMoves > bound)
    return estMoves;

    estMoves = LookupTable::GetBigUpperEdgeStateDistance(from);

    if (estMoves > bound)
    return estMoves;

    if (estMoves > max)
    max = estMoves;

    estMoves = LookupTable::GetBigLowerEdgeStateDistance(from);

    if (estMoves > bound)
    return estMoves;

    if (estMoves > max)
    max = estMoves;

    /*
    estMoves = LookupTable::GetEdgePermutationStateDistance(from);

    if (estMoves > bound)
    return estMoves;

    if (estMoves > max)
    max = estMoves;
    */

    // Return the max estimate if none exceeds the bound.
    return max;
}

vector<Turn> Solver::GenerateTurnSequenceFromStateSequence(vector<RubicsCubeState> stateSequence) {
    vector<Turn> turnSequence = {};
    RubicsCubeState lastState = stateSequence.back();
    stateSequence.pop_back();

    while (stateSequence.size() != 0) {
        RubicsCubeState currentState = stateSequence.back();
        turnSequence.push_back(currentState.GetTurnTo(lastState));
        stateSequence.pop_back();
        lastState = currentState;
    }    

    std::reverse(turnSequence.begin(), turnSequence.end());
    return turnSequence;
}