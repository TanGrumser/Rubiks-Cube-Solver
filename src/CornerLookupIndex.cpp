#include <string>
#include <iostream>
#include <iomanip> 
#include "LookupTable.h" 
#include "RubicsCubeState.h"
#include "FileManagement.h"
#include "PermutationIndexer.h"

using std::string;
using LookupTable::CORNER_ROTATIONS_COUNT;
using LookupTable::CORNER_STATES_COUNT;
using LookupTable::CORNER_LOOKUP_TABLE_PATH;

int GetPermutationIndex(RubicsCubeState* state);

const int powersOfThree[] = {
    1, //3^0
    3, 
    3 * 3,
    3 * 3 * 3,
    3 * 3 * 3 * 3,
    3 * 3 * 3 * 3 * 3,
    3 * 3 * 3 * 3 * 3 * 3,
    3 * 3 * 3 * 3 * 3 * 3 * 3, // 3^7
};

/*
void LookupTable::TestCornerLookupCaluclation() {
    RubicsCubeState* state = RubicsCubeState::InitialState()->Copy();

    for (int i = 0; i < CORNER_STATES_COUNT; i++) {
        SetStateToCornerStateByIndex(i, state);
        
        if (GetCornerLookupIndex(state) != i) {
            std::cout << "error for index " << i << endl;
            return;
        } else  if (i % 1000000 == 0) {
            double progress = (double(i)) / (double(CORNER_STATES_COUNT)) * 100.0;
            std::cout << "Prorgess " << setprecision(4) << fixed << progress << "% \r";
        }
    }

    std::cout << "Finsihed testing. No errors were found." << endl;
}
*/

PermutationIndexer<8> cornerIndexer;

uint64 LookupTable::GetCornerLookupIndex(RubicsCubeState& state) {
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

    return cornerIndexer.rank(cornerPermutation) * CORNER_ROTATIONS_COUNT + GetRotationIndex(state);
}

int inline LookupTable::GetRotationIndex(RubicsCubeState& state) {
    int rotationIndex = 0;
   
    for (int i = 0; i < 7; i++) {
        rotationIndex += powersOfThree[i] * state.corners[i].rotation;
    }

    return rotationIndex;
}


int GetPermutationIndex(RubicsCubeState* state) {
    int result = 0;

    const int coefficients[] = {
        1 * 2 * 3 * 4 * 5 * 6 * 7, 
        1 * 2 * 3 * 4 * 5 * 6,
        1 * 2 * 3 * 4 * 5,
        1 * 2 * 3 * 4,
        1 * 2 * 3,
        1 * 2,
        1
    };

    for (int i = 0; i < 8 - 1; i++) {
        int position = 0;

        // i is the index of the piece we're looking for, j is the position of the piece.
        for (int j = 0; j < 8; j++) {
            if (state->corners[j].index == i) {
                result += position * coefficients[i];
                break;
            }
            
            if (i < state->corners[j].index) {
                position++;
            }
        }
    }

    return result;
}