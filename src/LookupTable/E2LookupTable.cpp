#include "LookupTable.h"
#include "LookupTableGenerator.h"
#include "StateIndexReverser.h"
#include "../Utils/FileManagement.h"
#include "../Utils/Logging/ConsoleLogger.h"

char* e2LookupTable;

void LookupTable::LoadE2LookupTable() {
    uint64_t* size = new uint64_t(0);
    e2LookupTable = FileManagement::LoadBufferFromFile(E2_LOOKUP_TABLE_PATH, size);
    delete size;
    
    if (e2LookupTable == nullptr) {
        GenerateE2LookupTable();
        LoadE2LookupTable();
    }
}

void LookupTable::GenerateE2LookupTable() { 
    ConsoleLogger logger("E2 Lookup Table Generation");

    LookupTableGenerator generator(
        BIG_EDGE_STATES_COUNT, 
        GetE2LookupIndex,
        GetE2StateFromIndex,
        &logger
    );

    generator.StartLogger(1000);
    generator.PopulateWithIterativeDeepeningDepthFirstSearch(7, 6);
    generator.PopulateWithInverseStateIndexSearch(8);
    generator.WriteLookupTableToFile(E2_LOOKUP_TABLE_PATH);
}

char LookupTable::GetE2StateDistance(RubiksCubeState& state) {
    uint64_t index = GetE2LookupIndex(state);
    return e2LookupTable[index];
}

uint64_t LookupTable::GetE2LookupIndex(RubiksCubeState& state) {
    int rotationIndex = 0;
    array<unsigned, 7> edgePerm;
    
    // TODO this can be optimized by couting the entries in edgePerm set and stopping if all edges have been found already.
    for (int i = 0; i < 12; i++) {
        unsigned& edgeIndex = state.edges[i].index;

        if (edgeIndex > 4) {
            edgePerm[edgeIndex - 5] = i;
            rotationIndex += powersOfTwo[edgeIndex - 5] * state.edges[i].rotation;
        }
    }

    unsigned permutaionIndex = LookupTable::eGroupIndexer.rank(edgePerm);

    return permutaionIndex * BIG_EDGE_ROTATION_COUNT + rotationIndex;
}

RubiksCubeState LookupTable::GetE2StateFromIndex(uint64_t index) {
    const unsigned UNUSED_EDGE_INDEX = 0;

    RubiksCubeState result;
    
    uint64_t edgePermutationIndex = index / BIG_EDGE_ROTATION_COUNT;
    uint64_t edgeRotationIndex    = index % BIG_EDGE_ROTATION_COUNT;
    
    array<unsigned, 7> edgeIndicies = StateIndexReverser::eGroupIndexer.getPermutation(edgePermutationIndex);

    // Since the E2 state is only dependent on the position and rotations of edges with an index greater than 4, 
    // we can set all other edges to some arbitrary index, that does not interfere with the relevenat indicies.
    // in this case the index 0 is used.
    for (int i = 0; i < 12; i++) {
        result.edges[i].index = UNUSED_EDGE_INDEX;
    }

    for (int i = 0; i < 7; i++) {
        result.edges[edgeIndicies[i]].index = i + 5;
        result.edges[edgeIndicies[i]].rotation = edgeRotationIndex >> i & 1u; // simulates dividing by 2^i but is faster
    }

    return result;
}