#include <string>
#include <iostream>
#include <iomanip> 
#include "LookupTable.h" 
#include "RubicsCubeState.h"
#include "FileManagement.h"

using std::string;
using LookupTable::EDGE_ROTATION_COUNT;
using LookupTable::EDGE_PERMUTATIONS_COUNT;
using LookupTable::BIG_EDGE_ROTATION_COUNT;
using LookupTable::BIG_EDGE_PERMUTATIONS_COUNT;
using LookupTable::FULL_EDGE_ROTATION_COUNT;

int GetEdgeStateIndex(RubicsCubeState* state, std::vector<int>* edgeIndicies);
int GetBigEdgeStateIndex(RubicsCubeState* state, std::vector<int>* edgeIndicies);
int GetPermutationIndex(RubicsCubeState* state, std::vector<int>* edgeIndicies);
int GetBigPermutationIndex(RubicsCubeState* state, std::vector<int>* edgeIndicies);
int GetFullPermutationIndex(RubicsCubeState* state);

std::vector<int>* UPPER_EDGE_INDICIES = new std::vector<int> {0, 1, 2, 3, 4, 5};
std::vector<int>* LOWER_EDGE_INDICIES = new std::vector<int> {6, 7, 8, 9, 10, 11};
std::vector<int>* BIG_UPPER_EDGE_INDICIES = new std::vector<int> {0, 1, 2, 3, 4, 5, 6};
std::vector<int>* BIG_LOWER_EDGE_INDICIES = new std::vector<int> {5, 6, 7, 8, 9, 10, 11};
std::vector<int>* FULL_EDGE_INDICIES = new std::vector<int> {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

const int powersOfTwo[] = {
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

int LookupTable::GetUpperEdgeLookupIndex(RubicsCubeState* state) {
    return GetEdgeStateIndex(state, UPPER_EDGE_INDICIES);
} 

int LookupTable::GetLowerEdgeLookupIndex(RubicsCubeState* state) {
    return GetEdgeStateIndex(state, LOWER_EDGE_INDICIES);
} 

int LookupTable::GetBigUpperEdgeLookupIndex(RubicsCubeState* state) {
    return GetBigEdgeStateIndex(state, BIG_UPPER_EDGE_INDICIES);
} 

int LookupTable::GetBigLowerEdgeLookupIndex(RubicsCubeState* state) {
    return GetBigEdgeStateIndex(state, BIG_LOWER_EDGE_INDICIES);
} 

uint64 LookupTable::GetFullEdgeLookupIndex(RubicsCubeState* state) {
    int rotationIndex = 0;
    int permutaionIndex = GetFullPermutationIndex(state);
    
    // We iterating only to the next-to-last element since, the rotation of the last piece is completely defined by the rotations of all pieces before.
    for (int i = 0; i < 11; i++) {
        rotationIndex += powersOfTwo[i] * state->edgePieces[i].rotation;
    }

    return permutaionIndex * FULL_EDGE_ROTATION_COUNT + rotationIndex;
}

int GetEdgeStateIndex(RubicsCubeState* state, std::vector<int>* edgeIndicies) {
    int rotationIndex = 0;
    int permutaionIndex = GetPermutationIndex(state, edgeIndicies);
    
    for (int i = 0; i < edgeIndicies->size(); i++) {
        for (int j = 0; j < 12; j++) {
            if ((*edgeIndicies)[i] == state->edgePieces[j].index) {
                rotationIndex += powersOfTwo[i] * state->edgePieces[j].rotation;
                break;
            }
        }
    }

    return permutaionIndex * EDGE_ROTATION_COUNT + rotationIndex;

}

int GetBigEdgeStateIndex(RubicsCubeState* state, std::vector<int>* edgeIndicies) {
    int rotationIndex = 0;
    int permutaionIndex = GetBigPermutationIndex(state, edgeIndicies);
    
    for (int i = 0; i < edgeIndicies->size(); i++) {
        for (int j = 0; j < 12; j++) {
            if ((*edgeIndicies)[i] == state->edgePieces[j].index) {
                rotationIndex += powersOfTwo[i] * state->edgePieces[j].rotation;
                break;
            }
        }
    }

    return permutaionIndex * BIG_EDGE_ROTATION_COUNT + rotationIndex;
}

int GetPermutationIndex(RubicsCubeState* state, std::vector<int>* edgeIndicies) {
    int result = 0;
    //const int factor = 7 * 6 * 5 * 4 * 3 * 2 * 1;

    const int coefficients[] = {
        7 * 8 * 9 * 10 * 11,
        7 * 8 * 9 * 10,
        7 * 8 * 9,
        7 * 8,
        7,
        1,
    };

    for (int i = 0; i < edgeIndicies->size(); i++) {
        int position = 0;

        // i is the index of the piece we're looking for, j is the position of the piece.
        for (int j = 0; j < 12; j++) {
            if (state->edgePieces[j].index == (*edgeIndicies)[i]) {
                result += position * coefficients[i];
                break;
            }
            
            // Since the egde indicies may not start at zero we have to check if a number is below the smallest value and count its position (2nd statement)
            if ((*edgeIndicies)[i] < state->edgePieces[j].index || state->edgePieces[j].index < (*edgeIndicies)[0]) {
                position++;
            }
        }
    }

    return result;
}

int GetBigPermutationIndex(RubicsCubeState* state, std::vector<int>* edgeIndicies) {
    int result = 0;
    //const int factor = 7 * 6 * 5 * 4 * 3 * 2 * 1;

    const int coefficients[] = {
        6 * 7 * 8 * 9 * 10 * 11,
        6 * 7 * 8 * 9 * 10,
        6 * 7 * 8 * 9,
        6 * 7 * 8,
        6 * 7,
        6,
        1
    };

    for (int i = 0; i < edgeIndicies->size(); i++) {
        int position = 0;

        // i is the index of the piece we're looking for, j is the position of the piece.
        for (int j = 0; j < 12; j++) {
            if (state->edgePieces[j].index == (*edgeIndicies)[i]) {
                result += position * coefficients[i];
                break;
            }
            
            // Since the egde indicies may not start at zero we have to check if a number is below the smallest value and count its position (2nd statement)
            if ((*edgeIndicies)[i] < state->edgePieces[j].index || state->edgePieces[j].index < (*edgeIndicies)[0]) {
                position++;
            }
        }
    }

    return result;
}

int GetFullPermutationIndex(RubicsCubeState* state) {
    int result = 0;

    const int coefficients[] = {
        1 * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9 * 10 * 11,
        1 * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9 * 10,
        1 * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9,
        1 * 2 * 3 * 4 * 5 * 6 * 7 * 8,
        1 * 2 * 3 * 4 * 5 * 6 * 7,
        1 * 2 * 3 * 4 * 5 * 6,
        1 * 2 * 3 * 4 * 5,
        1 * 2 * 3 * 4,
        1 * 2 * 3,
        1 * 2,
        1
    };

    // We iterating only to the third-last element, since the position of the last two pieces are completely defined by the positions of all other pieces.
    for (int i = 0; i < 12 - 1; i++) {
        int position = 0;

        // i is the index of the piece we're looking for, j is the position of the piece.
        for (int j = 0; j < 12; j++) {
            if (state->edgePieces[j].index == i) {
                result += position * coefficients[i];
                break;
            }
            
            if (i < state->edgePieces[j].index) {
                position++;
            }
        }
    }

    return result;
}