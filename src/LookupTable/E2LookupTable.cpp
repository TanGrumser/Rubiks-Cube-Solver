#include "LookupTable.h"
#include "../Utils/FileManagement.h"

char* e2LookupTable;

void LookupTable::LoadE2LookupTable() {
    uint64_t* size = new uint64_t(0);
    e2LookupTable = FileManagement::LoadBufferFromFile(E2_LOOKUP_TABLE_PATH, size);
    delete size;
}

void LookupTable::GenerateE2LookupTable() { 
    GenerateLookupTable(LookupTable::E2_LOOKUP_TABLE_PATH, GetE2LookupIndex, EDGE_STATES_COUNT); 
}

char LookupTable::GetE2StateDistance(RubiksCubeState& state) {
    uint64_t index = GetE2LookupIndex(state);
    return e2LookupTable[index];
}

uint64_t LookupTable::GetE2LookupIndex(RubiksCubeState& state) {
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