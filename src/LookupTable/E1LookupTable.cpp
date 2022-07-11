#include "LookupTable.h"
#include "../Utils/FileManagement.h"

char* e1LookupTable;

void LookupTable::GenerateE1LookupTable() { 
    GenerateLookupTable(LookupTable::E1_LOOKUP_TABLE_PATH, GetE1LookupIndex, EDGE_STATES_COUNT); 
}

void LookupTable::LoadE1LookupTable() {
    uint64_t* size = new uint64_t(0);
    e1LookupTable = FileManagement::LoadBufferFromFile(E1_LOOKUP_TABLE_PATH, size);
    delete size;
}

char LookupTable::GetE1StateDistance(RubiksCubeState& state) {
    uint64_t index = GetE1LookupIndex(state);
    return e1LookupTable[index];
}

uint64_t LookupTable::GetE1LookupIndex(RubiksCubeState& state) {
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
