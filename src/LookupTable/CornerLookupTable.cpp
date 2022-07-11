#include <string>
#include <iostream>
#include <iomanip> 
#include "LookupTable.h" 
#include "LookupTableGenerator.h" 
#include "../Model/RubiksCubeState.h"
#include "../Utils/FileManagement.h"
#include "../Utils/PermutationIndexer.h"
#include "../Utils/Logging/ConsoleLogger.h"

char* cornerLookupTable;

void LookupTable::GenerateCornerLookupTable() { 
    ConsoleLogger logger("Corner Lookup Table Generation");

    LookupTableGenerator generator(
        CORNER_STATES_COUNT, 
        GetCornerLookupIndex, 
        0, 
        &logger
    );

    generator.StartLogger(1000);
    generator.PopulateWithIterativeDeepeningDFS(9, 6); 
    generator.PopulateWithInverseIndexSearch(6); 
    generator.WriteLookupTableToFile(CORNER_LOOKUP_TABLE_PATH);
}

void LookupTable::LoadCornerLookupTable() {
    uint64_t* size = new uint64_t(0);
    cornerLookupTable = FileManagement::LoadBufferFromFile(CORNER_LOOKUP_TABLE_PATH, size);
}

char LookupTable::GetCornerStateDistance(RubiksCubeState& state) {
    uint64_t index = GetCornerLookupIndex(state);
    return cornerLookupTable[index];
}

uint64_t LookupTable::GetCornerLookupIndex(RubiksCubeState& state) {
    array<unsigned int, 8> cornerPermutation = {
        state.corners[0].index,
        state.corners[1].index,
        state.corners[2].index,
        state.corners[3].index,
        state.corners[4].index,
        state.corners[5].index,
        state.corners[6].index,
        state.corners[7].index
    };
    
    int rotationIndex = 0;
   
    for (int i = 0; i < 7; i++) {
        rotationIndex += powersOfThree[i] * state.corners[i].rotation;
    }

    return LookupTable::cornerIndexer.rank(cornerPermutation) * CORNER_ROTATIONS_COUNT + rotationIndex;
}