#include <iostream>
#include <algorithm>

#include "Solver.h"
#include "RubicsCubeState.h"
#include "LookupTable.h"

const int EDGE_CORNER_NEIGHBOR_INIDICIES[12][2] = {
    {0, 1},
    {1, 2},
    {2, 3},
    {3, 0},
    {4, 0},
    {5, 1},
    {6, 2},
    {7, 3},
    {5, 4},
    {6, 5},
    {7, 6},
    {4, 7},
};

const int EDGE_CORNER_ROTATION_OFFSET[12][4] = {
    {0, 0, 1, 2},
    {0, 0, 1, 2},
    {0, 0, 1, 2},
    {0, 0, 1, 2},
   
    {2, 1, 2, 1},
    {2, 1, 2, 1},
    {2, 1, 2, 1},
    {2, 1, 2, 1},
   
    {0, 0, 1, 2},
    {0, 0, 1, 2},
    {0, 0, 1, 2},
    {0, 0, 1, 2}
};

namespace Solver {
    int heuristicMode = 0;
    int solverIndex = 0;
    int threadCount = 1;
    int searchedStates = 0;
}

int GetNeighbourHeuristic(RubicsCubeState* from, RubicsCubeState* to);

int Solver::GetDistanceHeuristic(RubicsCubeState* from, RubicsCubeState* to) {
    if (Solver::heuristicMode == 0) {
        //int neihbhourDistanceHeuristic = GetNeighbourHeuristic(from, to);
        int cornerStateDistance = LookupTable::GetCornerStateDistance(from);
        int upperEdgeStateDistance = LookupTable::GetBigUpperEdgeStateDistance(from);
        int lowerEdgeStateDistance = LookupTable::GetBigLowerEdgeStateDistance(from);
        
        return std::max(cornerStateDistance, std::max(upperEdgeStateDistance, lowerEdgeStateDistance));
    } else if (Solver::heuristicMode == 1) {
        int neihbhourDistanceHeuristic = GetNeighbourHeuristic(from, to);
        int cornerStateDistance = LookupTable::GetCornerStateDistance(from);
        int upperEdgeStateDistance = LookupTable::GetUpperEdgeStateDistance(from);
        
        return std::max(neihbhourDistanceHeuristic, std::max(cornerStateDistance, upperEdgeStateDistance));
    } else if (Solver::heuristicMode == 2) {
        return GetNeighbourHeuristic(from, to);
    } else {
        return LookupTable::GetCornerStateDistance(from);
    }
}

int GetNeighbourHeuristic(RubicsCubeState* from, RubicsCubeState* to) {
    int progress = 0;
    unsigned int** otherEdgeNeighbourIndiciesRotation = Solver::GetEdgeNeighbourIndiciesRotations(to);
    int leftNeightbourIndex = -1;
    int rightNeightbourIndex = -1;
    int leftNeightbourRotation= -1;
    int rightNeightbourRotation = -1;

    for (int i = 0 ; i < 12; i++) {
        int lookupIndex = from->edgePermutaion[i];
        unsigned int* otherNeighboursPositionRotations = otherEdgeNeighbourIndiciesRotation[lookupIndex];
        leftNeightbourIndex = from->cornerPermutaion[EDGE_CORNER_NEIGHBOR_INIDICIES[i][from->edgeRotation[i] == 0 ? 0 : 1]];
        rightNeightbourIndex = from->cornerPermutaion[EDGE_CORNER_NEIGHBOR_INIDICIES[i][from->edgeRotation[i] == 0 ? 1 : 0]];

        leftNeightbourRotation = (from->cornerRotation[EDGE_CORNER_NEIGHBOR_INIDICIES[i][from->edgeRotation[i] == 0 ? 0 : 1]]
            + EDGE_CORNER_ROTATION_OFFSET[i][0 + (from->edgeRotation[i] == 0 ? 0 : 2)]) % 3;
        rightNeightbourRotation = (from->cornerRotation[EDGE_CORNER_NEIGHBOR_INIDICIES[i][from->edgeRotation[i] == 0 ? 1 : 0]]
            + EDGE_CORNER_ROTATION_OFFSET[i][1 + (from->edgeRotation[i] == 0 ? 0 : 2)]) % 3;
        
        

        if (leftNeightbourIndex == otherNeighboursPositionRotations[0] // are edges correct
            && leftNeightbourRotation == otherNeighboursPositionRotations[2] // are they correclty rotated
        ) {

            progress++;
        }

        
        if (rightNeightbourIndex == otherNeighboursPositionRotations[1]
            && rightNeightbourRotation == otherNeighboursPositionRotations[3]
        ) {
            progress++;
        }
    }

    return 6 - progress / 4;
}


unsigned int** Solver::GetEdgeNeighbourIndiciesRotations(RubicsCubeState* state) {
    if (state->edgeNeighbourIndicieRotations == nullptr) {
        unsigned int** edgeNeighbourIndicieRotations = (unsigned int**) malloc(sizeof(int) * 12 * 4);
        state->edgeNeighbourIndicieRotations = edgeNeighbourIndicieRotations;

        for (int i = 0; i < 12; i++) {
            int leftCornerNeighbourIndex = EDGE_CORNER_NEIGHBOR_INIDICIES[i][state->edgeRotation[i] == 0 ? 0 : 1];
            int rightCornerNeighbourIndex = EDGE_CORNER_NEIGHBOR_INIDICIES[i][state->edgeRotation[i] == 0 ? 1 : 0];
            int leftCornerRotationOffset = EDGE_CORNER_ROTATION_OFFSET[i][0 + (state->edgeRotation[i] == 0 ? 0 : 2)];
            int rightCornerRotationOffset = EDGE_CORNER_ROTATION_OFFSET[i][1 + (state->edgeRotation[i] == 0 ? 0 : 2)];

            edgeNeighbourIndicieRotations[state->edgePermutaion[i]] = new unsigned int[4] {
                state->cornerPermutaion[leftCornerNeighbourIndex],
                state->cornerPermutaion[rightCornerNeighbourIndex],
                (state->cornerRotation[leftCornerNeighbourIndex] + leftCornerRotationOffset) % 3,
                (state->cornerRotation[rightCornerNeighbourIndex] + rightCornerRotationOffset) % 3
            };
        }
    }
    
    return state->edgeNeighbourIndicieRotations;
}


vector<Turn> Solver::GenerateTurnSequenceFromStateSequence(vector<RubicsCubeState*> stateSequence) {
    vector<Turn> turnSequence = {};
    RubicsCubeState* lastState = stateSequence.back();
    stateSequence.pop_back();

    while (stateSequence.size() != 0) {
        RubicsCubeState* currentState = stateSequence.back();
        turnSequence.push_back(currentState->GetTurnTo(lastState));
        stateSequence.pop_back();
        delete lastState;
        lastState = currentState;
    }    

    std::reverse(turnSequence.begin(), turnSequence.end());
    return turnSequence;
}