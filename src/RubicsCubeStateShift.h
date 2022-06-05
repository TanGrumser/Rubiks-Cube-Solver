#pragma once

#include "RubicsCubeState.h"
#include "StateIndex.h"
#include "PermutationIndexer.h"
#include "LookupTable.h"

struct RubicsCubeStateShift {    
    array<unsigned int, 8> cornerIndiciesMapping;
    array<unsigned int, 8> cornerRotationOffset;
    array<unsigned int, 12> edgeIndiciesMapping;
    array<unsigned int, 12> edgeRotationOffset;

    PermutationIndexer<8> cornerIndexer;
    PermutationIndexer<12> edgeIndexer;

    const unsigned int powersOfThree[8] = {
        1,
        3,
        3 * 3,
        3 * 3 * 3,
        3 * 3 * 3 * 3,
        3 * 3 * 3 * 3 * 3,
        3 * 3 * 3 * 3 * 3 * 3,
        3 * 3 * 3 * 3 * 3 * 3 * 3,
    };

    const unsigned int powersOfTwo[11] = {
        1,
        2, 
        2 * 2,
        2 * 2 * 2,
        2 * 2 * 2 * 2,
        2 * 2 * 2 * 2 * 2,
        2 * 2 * 2 * 2 * 2 * 2,
        2 * 2 * 2 * 2 * 2 * 2 * 2,
        2 * 2 * 2 * 2 * 2 * 2 * 2 * 2,
        2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2,
        2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2,
    };

    RubicsCubeStateShift (RubicsCubeState* state) {
        for (int i = 0; i < 8; i++) {
            this->cornerIndiciesMapping[state->cornerPermutaion[i]] = i;
            this->cornerRotationOffset[state->cornerPermutaion[i]]  = (3u - state->cornerRotation[i]) % 3u;
        }

        for (int i = 0; i < 12; i++) {
            this->edgeIndiciesMapping[state->edgePermutaion[i]] = i;
            this->edgeRotationOffset[state->edgePermutaion[i]]  = state->edgeRotation[i];
        }
    }

    StateIndex GetShiftedState(RubicsCubeState* state) {
        StateIndex stateIndex;

        array<unsigned, 8> cornerPermutaion;
        array<unsigned, 8> cornerRotations;
        array<unsigned, 12> edgePermutaion;

        int cornerRotationIndex = 0;
        int edgeRotationIndex = 0;

        for (int i = 0; i < 8; i++) {
            cornerPermutaion[i] = this->cornerIndiciesMapping[state->cornerPermutaion[i]];
            
            if (i < 7) {
                cornerRotationIndex += ((state->cornerRotation[i] + this->cornerRotationOffset[state->cornerPermutaion[i]]) % 3u) * powersOfThree[i];
            }
        }
            
        
        for (int i = 0; i < 12; i++) {
            edgePermutaion[i] = this->edgeIndiciesMapping[state->edgePermutaion[i]];
            
            if (i < 11) {
                edgeRotationIndex += ((state->edgeRotation[i] + this->edgeRotationOffset[state->edgePermutaion[i]]) % 2u) * powersOfTwo[i];
            }
        }

        stateIndex.cornerIndex = cornerIndexer.rank(cornerPermutaion) * LookupTable::CORNER_ROTATIONS_COUNT + cornerRotationIndex;
        stateIndex.edgeIndex = edgeIndexer.rank(edgePermutaion) * LookupTable::FULL_EDGE_ROTATION_COUNT + edgeRotationIndex;
        
        return stateIndex;
    }
};