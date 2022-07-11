#include "LookupTable.h" 
#include "LookupTableGenerator.h" 
#include "StateIndexReverser.h"
#include "../Utils/FileManagement.h"
#include "../Utils/Logging/FileLogger.h"

char* edgeLookupTable;

void LookupTable::GenerateEdgeLookupTable() {
    FileLogger logger("log.txt" ,"Edge Lookup Table Generation");

    LookupTableGenerator generator(
        FULL_EDGE_STATES_COUNT, 
        GetEdgeLookupIndex, 
        LookupTable::GetEdgeStateFromIndex, 
        &logger
    );

    generator.StartLogger(100);
    generator.PopulateWithIterativeDeepeningDepthFirstSearch(8, 18);
    generator.PopulateWithInverseStateIndexSearch(128);
    generator.WriteLookupTableToFile(EDGE_LOOKUP_TABLE_PATH);
}

void LookupTable::LoadEdgeLookupTable() {
    uint64_t* size = new uint64_t(0);
    edgeLookupTable = FileManagement::LoadBufferFromFile(EDGE_LOOKUP_TABLE_PATH, size);
    delete size;
}

char LookupTable::GetEdgeStateDistance(RubiksCubeState& state) {
    int index = GetEdgeLookupIndex(state);
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
    StateIndex stateIndex;
    stateIndex.cornerIndex = 0;
    stateIndex.edgeIndex = index;

    return StateIndexReverser::GetStateFromIndex(stateIndex);
}