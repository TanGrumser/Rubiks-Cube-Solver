#include <string>
#include <iostream>
#include <iomanip> 
#include "LookupTable.h" 
#include "RubicsCubeState.h"
#include "FileManagement.h"

using std::string;
using LookupTable::EDGE_ROTATION_STATES_COUNT;
using LookupTable::EDGE_PERMUTATIONS_COUNT;
using LookupTable::CORNER_LOOKUP_TABLE_PATH;

int GetPermutationIndex(RubicsCubeState* state, std::vector<int> edgeIndicies);

int GetEdgeLookupIndexFromState(RubicsCubeState* state, const std::vector<int> edgeIndicies) {
    int powersOfTwo[] = {
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

    int rotationIndex = 0;
    int permutaionIndex = GetPermutationIndex(state, edgeIndicies);
    
    for (int i = 0; i < edgeIndicies.size(); i++) {
        rotationIndex += powersOfTwo[i] * state->edgePieces[edgeIndicies[i]].rotation;
    }

    return permutaionIndex * EDGE_ROTATION_STATES_COUNT + rotationIndex;
} 

int GetPermutationIndex(RubicsCubeState* state, std::vector<int> edgeIndicies) {
    int result = 0;

    const int coefficients[] = {
        5 * 6 * 7 * 8 * 9 * 10 * 11,
        5 * 6 * 7 * 8 * 9 * 10,
        5 * 6 * 7 * 8 * 9,
        5 * 6 * 7 * 8,
        5 * 6 * 7,
        5 * 6,
        5 
    };

    for (int i = 0; i < edgeIndicies.size(); i++) {
        int position = 0;

        // i is the index of the piece we're looking for, j is the position of the piece.
        for (int j = 0; j < 12; j++) {
            if (state->edgePieces[j].index == edgeIndicies[i]) {
                result += (position * coefficients[i]) / 5;
                break;
            }
            
            if (edgeIndicies[i] < state->edgePieces[j].index) {
                position++;
            }
        }
    }

    return result;
}