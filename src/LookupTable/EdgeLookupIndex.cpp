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

PermutationIndexer<12, 6> smallEgdeIndexer;
PermutationIndexer<12> fullEgdeIndexer;

uint64_t LookupTable::GetE2LookupIndex(RubiksCubeState& state) {
    return GetBigLowerEdgeStateIndex(state);
} 

uint64_t LookupTable::GetE1LookupIndex(RubiksCubeState& state) {
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

uint64_t LookupTable::GetEdgeLookupIndex(RubiksCubeState& state) {
    int rotationIndex = 0;
    
    // We iterating only to the next-to-last element since, the rotation of the last piece is completely defined by the rotations of all pieces before.
    for (int i = 0; i < 11; i++) {
        rotationIndex += powersOfTwo[i] * state.edges[i].rotation;
    }

    return GetEdgePermutationLookupIndex(state) * FULL_EDGE_ROTATION_COUNT + rotationIndex;
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

    int permutaionIndex = LookupTable::eGroupIndexer.rank(edgePerm);

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

    int permutaionIndex = LookupTable::eGroupIndexer.rank(edgePerm);

    return permutaionIndex * BIG_EDGE_ROTATION_COUNT + rotationIndex;
}