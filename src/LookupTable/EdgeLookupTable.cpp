#include "LookupTable.h" 
#include "LookupTableGenerator.h" 
#include "StateIndexReverser.h"
#include "../Utils/FileManagement.h"
#include "../Utils/Logging/FileLogger.h"

char* edgeLookupTable;

void LookupTable::GenerateEdgeLookupTable() {
    FileLogger logger(FileLogger::STANDARD_PATH ,"Edge Lookup Table Generation");

    LookupTableGenerator generator(
        FULL_EDGE_STATES_COUNT, 
        GetEdgeLookupIndex, 
        LookupTable::GetEdgeStateFromIndex, 
        &logger
    );

    // Generation metadata get logged to file every 10 000 ms (10 s)
    generator.StartLogger(10000);
    
    // An iterative deepenig search populates all states, that can be reached within 8 moves.
    generator.PopulateWithIterativeDeepeningDepthFirstSearch(8, 18);

    // afterward the lookup table is filled with a inverse state index search.
    generator.PopulateWithInverseStateIndexSearch(128);

    // when the lookup table is finished it is written to the disk.
    generator.WriteLookupTableToFile(EDGE_LOOKUP_TABLE_PATH);
}

void LookupTable::LoadEdgeLookupTable() {
    uint64_t* size = new uint64_t(0);
    edgeLookupTable = FileManagement::LoadBufferFromFile(EDGE_LOOKUP_TABLE_PATH, size);
    delete size;
    
    if (edgeLookupTable == nullptr) {
        GenerateEdgeLookupTable();
        LoadEdgeLookupTable();
    }
}

char LookupTable::GetEdgeStateDistance(RubiksCubeState& state) {
    uint64_t index = GetEdgeLookupIndex(state);
    return edgeLookupTable[index];
}

uint64_t LookupTable::GetEdgeLookupIndex(RubiksCubeState& state) {
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

    int rotationIndex = 0;

    // We iterating only to the next-to-last element since, the rotation of the last piece is completely defined by the rotations of all pieces before.
    for (int i = 0; i < 11; i++) {
        rotationIndex += powersOfTwo[i] * state.edges[i].rotation;
    }

    return LookupTable::edgeIndexer.rank(edgePermutation) * FULL_EDGE_ROTATION_COUNT + rotationIndex;
}

RubiksCubeState LookupTable::GetEdgeStateFromIndex(uint64_t index) {
    RubiksCubeState result;
    
    uint64_t edgePermutationIndex = index / FULL_EDGE_ROTATION_COUNT;
    uint64_t edgeRotationIndex    = index % FULL_EDGE_ROTATION_COUNT;
    
    array<unsigned, 12> edgeIndicies = StateIndexReverser::egdeIndexer.getPermutation(edgePermutationIndex);
    
    unsigned rotationSum = 0;

    for (int i = 0; i < 12; i++) {
        result.edges[i].index = edgeIndicies[i];
        result.edges[i].rotation = edgeRotationIndex >> i & 1u; // simulates dividing by 2^i but is faster

        rotationSum += result.edges[i].rotation;
    }

    result.edges[12 - 1].rotation = rotationSum & 1u; // this simulates mod2 but is faster

    return result;

}