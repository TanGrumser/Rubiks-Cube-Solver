#include <string>
#include <iostream>
#include <iomanip> 
#include "LookupTable.h" 
#include "../Model/RubiksCubeState.h"
#include "../Utils/FileManagement.h"
#include "../Utils/PermutationIndexer.h"

using std::string;

PermutationIndexer<12, 7> bigEgdeIndexer;

uint64_t GetE1StateIndex(RubiksCubeState& state);

uint64_t GetE1StateIndex(RubiksCubeState& state) {
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
