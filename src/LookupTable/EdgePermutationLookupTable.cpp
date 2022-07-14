#include "LookupTable.h"
#include "StateIndexReverser.h"
#include "LookupTableGenerator.h"
#include "StateIndexReverser.h"
#include "../Utils/FileManagement.h"
#include "../Utils/Logging/ConsoleLogger.h"

char* edgePermutationLookupTable;

void LookupTable::GenerateEdgePermutationLookupTable() { 
    ConsoleLogger logger("Edge Permutation Lookup Table Generation");

    LookupTableGenerator generator(
        FULL_EDGE_PERMUTATIONS_COUNT, 
        GetEdgePermutationLookupIndex, 
        GetEdgePermutationStateFromIndex, 
        &logger
    );

    generator.StartLogger(100);
    generator.PopulateWithIterativeDeepeningDepthFirstSearch(7, 6);
    generator.PopulateWithInverseStateIndexSearch(8);
    generator.WriteLookupTableToFile(EDGE_PERMUTATION_LOOKUP_TABLE_PATH);
}

void LookupTable::LoadEdgePermutationLookupTable() {
    uint64_t* size = new uint64_t(0);
    edgePermutationLookupTable = FileManagement::LoadBufferFromFile(EDGE_PERMUTATION_LOOKUP_TABLE_PATH, size);
    delete size;
    
    if (edgePermutationLookupTable == nullptr) {
        GenerateEdgePermutationLookupTable();
        LoadEdgePermutationLookupTable();
    }
}

char LookupTable::GetEdgePermutationStateDistance(RubiksCubeState& state) {
    int index = GetEdgePermutationLookupIndex(state);
    return edgePermutationLookupTable[index];
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

    return LookupTable::edgeIndexer.rank(edgePermutation);
}

RubiksCubeState LookupTable::GetEdgePermutationStateFromIndex(uint64_t index) {
    RubiksCubeState result;
    array<unsigned, 12> edgeIndicies = StateIndexReverser::egdeIndexer.getPermutation(index);
 
    for (int i = 0; i < 12; i++) {
        result.edges[i].index = edgeIndicies[i];
    }

    return result;
}