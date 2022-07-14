#include "LookupTable.h"
#include "LookupTableGenerator.h"
#include "StateIndexReverser.h"
#include "../Utils/FileManagement.h"
#include "../Utils/Logging/ConsoleLogger.h"

char* e1LookupTable;

void LookupTable::GenerateE1LookupTable() { 
    ConsoleLogger logger("E1 Lookup Table Generation");

    LookupTableGenerator generator(
        BIG_EDGE_STATES_COUNT, 
        GetE1LookupIndex,
        GetE1StateFromIndex, // unused, since it won't work
        &logger
    );

    generator.StartLogger(100);
    generator.PopulateWithIterativeDeepeningDepthFirstSearch(10, 6);
    generator.WriteLookupTableToFile(E1_LOOKUP_TABLE_PATH);
}

void LookupTable::LoadE1LookupTable() {
    uint64_t* size = new uint64_t(0);
    e1LookupTable = FileManagement::LoadBufferFromFile(E1_LOOKUP_TABLE_PATH, size);
    delete size;

    if (e1LookupTable == nullptr) {
        GenerateE1LookupTable();
        LoadE1LookupTable();
    }
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

RubiksCubeState LookupTable::GetE1StateFromIndex(uint64_t index) {
    throw std::runtime_error("Unitilized exception");
}