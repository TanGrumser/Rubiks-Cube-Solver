#pragma once

#include "RubicsCubeState.h"
#include "StateIndex.h"
#include "PermutationIndexer.h"
#include "LookupTable.h"
#include "Constants.h"

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

    RubicsCubeStateShift (RubicsCubeState& state) {
        for (int i = 0; i < 8; i++) {
            this->cornerIndiciesMapping[state.corners[i].index] = i;
            this->cornerRotationOffset[state.corners[i].index]  = (3u - state.corners[i].rotation) % 3u;
        }

        for (int i = 0; i < 12; i++) {
            this->edgeIndiciesMapping[state.edges[i].index] = i;
            this->edgeRotationOffset[state.edges[i].index]  = state.edges[i].rotation;
        }
    }

    StateIndex GetShiftedState(RubicsCubeState& state) {
        StateIndex stateIndex;

        array<unsigned, 8> cornerPermutaion;
        array<unsigned, 12> edgePermutaion;

        int cornerRotationIndex = 0;
        int edgeRotationIndex = 0;

        for (int i = 0; i < 8; i++) {
            cornerPermutaion[i] = this->cornerIndiciesMapping[state.corners[i].index];
            
            if (i < 7) {
                cornerRotationIndex += ((state.corners[i].rotation + this->cornerRotationOffset[state.corners[i].index]) % 3u) * powersOfThree[i];
            }
        }
            
        
        for (int i = 0; i < 12; i++) {
            edgePermutaion[i] = this->edgeIndiciesMapping[state.edges[i].index];
            
            if (i < 11) {
                edgeRotationIndex += ((state.edges[i].rotation + this->edgeRotationOffset[state.edges[i].index]) % 2u) * powersOfTwo[i];
            }
        }

        stateIndex.cornerIndex = cornerIndexer.rank(cornerPermutaion) * CORNER_ROTATIONS_COUNT + cornerRotationIndex;
        stateIndex.edgeIndex = edgeIndexer.rank(edgePermutaion) * FULL_EDGE_ROTATION_COUNT + edgeRotationIndex;
        
        return stateIndex;
    }
};