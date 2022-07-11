#include "LookupTable.h"
#include "../Utils/FileManagement.h"

char* edgePermutationLookupTable;

void LookupTable::GenerateEdgePermutationLookupTable() { 
    GenerateLookupTable(LookupTable::EDGE_PERMUTATION_LOOKUP_TABLE_PATH, GetEdgePermutationLookupIndex, FULL_EDGE_PERMUTATIONS_COUNT); 
}

void LookupTable::LoadEdgePermutationLookupTable() {
    uint64_t* size;
    edgePermutationLookupTable = FileManagement::LoadBufferFromFile(EDGE_PERMUTATION_LOOKUP_TABLE_PATH, size);
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
