#include <iostream>
#include <algorithm>

#include "Solver.h"
#include "RubicsCubeState.h"
#include "LookupTable.h"

namespace Solver {
    int heuristicMode = 0;
    int solverIndex = 0;
    int threadCount = 1;
    int searchedStates = 0;
}

int Solver::GetDistanceHeuristic(RubicsCubeState* from, RubicsCubeState* to) {
    int cornerStateDistance = LookupTable::GetCornerStateDistance(from);
    int upperEdgeStateDistance = LookupTable::GetBigUpperEdgeStateDistance(from);
    int lowerEdgeStateDistance = LookupTable::GetBigLowerEdgeStateDistance(from);
    
    return std::max(cornerStateDistance, std::max(upperEdgeStateDistance, lowerEdgeStateDistance));
}

vector<Turn> Solver::GenerateTurnSequenceFromStateSequence(vector<RubicsCubeState*> stateSequence) {
    vector<Turn> turnSequence = {};
    RubicsCubeState* lastState = stateSequence.back();
    stateSequence.pop_back();

    while (stateSequence.size() != 0) {
        RubicsCubeState* currentState = stateSequence.back();
        turnSequence.push_back(currentState->GetTurnTo(lastState));
        stateSequence.pop_back();
        lastState = currentState;
    }    

    std::reverse(turnSequence.begin(), turnSequence.end());
    return turnSequence;
}