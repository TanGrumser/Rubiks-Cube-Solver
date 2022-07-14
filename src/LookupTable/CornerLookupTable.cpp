#include <string>
#include <iostream>
#include <iomanip> 
#include "LookupTable.h" 
#include "StateIndexReverser.h" 
#include "LookupTableGenerator.h" 
#include "../Model/RubiksCubeState.h"
#include "../Utils/FileManagement.h"
#include "../Utils/PermutationIndexer.h"
#include "../Utils/Logging/ConsoleLogger.h"
#include "../Utils/Logging/FileLogger.h"

char* cornerLookupTable;

void LookupTable::GenerateCornerLookupTable() { 
    ConsoleLogger logger("Corner Lookup Table Generation");

    LookupTableGenerator generator(
        CORNER_STATES_COUNT, 
        GetCornerLookupIndex,
        LookupTable::GetCornerStateFromIndex,
        &logger
    );

    generator.StartLogger(100);
    generator.PopulateWithIterativeDeepeningDepthFirstSearch(8, 6);
    generator.PopulateWithInverseStateIndexSearch(8);
    generator.WriteLookupTableToFile(CORNER_LOOKUP_TABLE_PATH);
}

void LookupTable::LoadCornerLookupTable() {
    uint64_t* size = new uint64_t(0);
    cornerLookupTable = FileManagement::LoadBufferFromFile(CORNER_LOOKUP_TABLE_PATH, size);
    delete size;

    if (cornerLookupTable == nullptr) {
        GenerateCornerLookupTable();
        LoadCornerLookupTable();
    }
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

RubiksCubeState LookupTable::GetCornerStateFromIndex(uint64_t index) {
    RubiksCubeState result;
    
    uint64_t cornerPermutationIndex = index / CORNER_ROTATIONS_COUNT;
    uint64_t cornerRotationIndex    = index % CORNER_ROTATIONS_COUNT;
    
    array<unsigned, 8> cornerIndicies = StateIndexReverser::cornerIndexer.getPermutation(cornerPermutationIndex);
    
    unsigned rotationSum = 0;

    for (int i = 0; i < 8; i++) {
        result.corners[i].index = cornerIndicies[i];
        result.corners[i].rotation = (cornerRotationIndex / powersOfThree[i]) % 3u;

        rotationSum += result.corners[i].rotation;
    }

    result.corners[8 - 1].rotation = (21u - rotationSum) % 3u;

    return result;
}