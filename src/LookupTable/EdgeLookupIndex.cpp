#include <string>
#include <iostream>
#include <iomanip> 
#include "LookupTable.h" 
#include "../Model/RubiksCubeState.h"
#include "../Utils/FileManagement.h"
#include "../Utils/PermutationIndexer.h"

using std::string;


int GetSmallEdgeStateIndex(RubiksCubeState& state, std::vector<int>* edgeIndicies);
int GetBigUpperEdgeStateIndex(RubiksCubeState& state);
int GetBigLowerEdgeStateIndex(RubiksCubeState& state);
int GetFullPermutationIndex(RubiksCubeState& state);

std::vector<int>* SMALL_UPPER_EDGE_INDICIES = new std::vector<int> {0, 1, 2, 3, 4, 5};
std::vector<int>* SMALL_LOWER_EDGE_INDICIES = new std::vector<int> {6, 7, 8, 9, 10, 11};
std::vector<int>* BIG_UPPER_EDGE_INDICIES = new std::vector<int> {0, 1, 2, 3, 4, 5, 6};
std::vector<int>* BIG_LOWER_EDGE_INDICIES = new std::vector<int> {5, 6, 7, 8, 9, 10, 11};
std::vector<int>* FULL_EDGE_INDICIES = new std::vector<int> {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

PermutationIndexer<12, 6> smallEgdeIndexer;
PermutationIndexer<12, 7> bigEgdeIndexer;
PermutationIndexer<12> fullEgdeIndexer;

uint64_t LookupTable::GetUpperEdgeLookupIndex(RubiksCubeState& state) {
    return GetSmallEdgeStateIndex(state, SMALL_UPPER_EDGE_INDICIES);
} 

uint64_t LookupTable::GetLowerEdgeLookupIndex(RubiksCubeState& state) {
    return GetSmallEdgeStateIndex(state, SMALL_LOWER_EDGE_INDICIES);
} 

uint64_t LookupTable::GetBigUpperEdgeLookupIndex(RubiksCubeState& state) {
    return GetBigLowerEdgeStateIndex(state);
} 

uint64_t LookupTable::GetBigLowerEdgeLookupIndex(RubiksCubeState& state) {
    return GetBigUpperEdgeStateIndex(state);
} 

uint64_t LookupTable::GetEdgePermutationLookupIndex(RubiksCubeState& state) {
    array<unsigned int, 12> edgePermutation = {
        state.edges[0].index,
        state.edges[1].index,
        state.edges[2].index,
        state.edges[3].index,
        state.edges[4].index,
        state.edges[5].index,
        state.edges[6].index,
        state.edges[7].index,
        state.edges[8].index,
        state.edges[9].index,
        state.edges[10].index,
        state.edges[11].index
    };

    return fullEgdeIndexer.rank(edgePermutation);
}

uint64_t LookupTable::GetFullEdgeLookupIndex(RubiksCubeState& state) {
    int rotationIndex = 0;
    
    // We iterating only to the next-to-last element since, the rotation of the last piece is completely defined by the rotations of all pieces before.
    for (int i = 0; i < 11; i++) {
        rotationIndex += powersOfTwo[i] * state.edges[i].rotation;
    }

    return GetEdgePermutationLookupIndex(state) * FULL_EDGE_ROTATION_COUNT + rotationIndex;
}

int GetSmallEdgeStateIndex(RubiksCubeState& state, std::vector<int>* edgeIndicies) {
    int rotationIndex = 0;
    array<unsigned int, 6> edgePerm;

    // This is quit innefficient. Try to improve this.
    for (int i = 0; i < edgeIndicies->size(); i++) {
        for (int j = 0; j < 12; j++) {
            if ((*edgeIndicies)[i] == state.edges[j].index) {
                edgePerm[i] = (*edgeIndicies)[i];
                rotationIndex += powersOfTwo[i] * state.edges[j].rotation;
                break;
            }
        }
    }

    int permutaionIndex = smallEgdeIndexer.rank(edgePerm);
    return permutaionIndex * EDGE_ROTATION_COUNT + rotationIndex;

}

// Only uses the edges with indicies 5 to 11
int GetBigLowerEdgeStateIndex(RubiksCubeState& state) {
    int rotationIndex = 0;
    array<unsigned int , 7> edgePerm;
    
    // TODO this can be optimized by couting the entries in edgePerm set and stopping if all edges have been found already.
    for (int i = 0; i < 12; i++) {
        unsigned int& edgeIndex = state.edges[i].index;

        if (edgeIndex > 4) {
            edgePerm[edgeIndex - 5] = i;
            rotationIndex += powersOfTwo[edgeIndex - 5] * state.edges[i].rotation;
        }
    }

    int permutaionIndex = bigEgdeIndexer.rank(edgePerm);

    return permutaionIndex * BIG_EDGE_ROTATION_COUNT + rotationIndex;
}

// Only uses the edges with indicies 0 to 6
int GetBigUpperEdgeStateIndex(RubiksCubeState& state) {
    int rotationIndex = 0;
    array<unsigned int , 7> edgePerm;
    
    // TODO this can be optimized by couting the entries in edgePerm set and stopping if all edges have been found already.
    for (int i = 0; i < 12; i++) {
        unsigned int& edgeIndex = state.edges[i].index;

        if (edgeIndex < 7) {
            edgePerm[edgeIndex] = i;
            rotationIndex += powersOfTwo[edgeIndex] * state.edges[i].rotation;
        }
    }

    int permutaionIndex = bigEgdeIndexer.rank(edgePerm);

    return permutaionIndex * BIG_EDGE_ROTATION_COUNT + rotationIndex;
}