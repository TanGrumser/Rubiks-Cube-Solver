#include <string>
#include <iostream>
#include <iomanip> 
#include "LookupTable.h" 
#include "RubicsCubeState.h"
#include "FileManagement.h"

using std::string;
using LookupTable::CORNER_ROTATIONS_COUNT;
using LookupTable::CORNER_STATES_COUNT;
using LookupTable::CORNER_LOOKUP_TABLE_PATH;

// Helper Methods
int GetRotationIndex(RubicsCubeState* state);
int GetPermutationIndex(RubicsCubeState* state);
int* GetCornerPositionsPerIndex(int index);

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

uint64 LookupTable::GetCornerLookupIndex(RubicsCubeState* state) {
    return GetPermutationIndex(state) * CORNER_ROTATIONS_COUNT + GetRotationIndex(state);
}

int GetRotationIndex(RubicsCubeState* state) {
    int rotationIndex = 0; 
   
    for (int i = 0; i < 7; i++) {
        rotationIndex += powersOfThree[i] * state->cornerPieces[i].rotation;
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
            if (state->cornerPieces[j].index == i) {
                result += position * coefficients[i];
                break;
            }
            
            if (i < state->cornerPieces[j].index) {
                position++;
            }
        }
    }

    return result;
}

void LookupTable::SetStateToCornerStateByIndex(int index, RubicsCubeState* state) {
    int* cornerIndicies = GetCornerPositionsPerIndex(index);
    int rotationSum = 0;

    for (int i = 0; i < 8; i++) {
        int rotationIndex = index % CORNER_ROTATIONS_COUNT;

        state->cornerPieces[i].rotation = (rotationIndex / powersOfThree[i]) % 3;
        state->cornerPieces[i].index = cornerIndicies[i];

        if (i == 8 - 1) {
            state->cornerPieces[8 - 1].rotation = 3 - (rotationSum % 3);
        } else {
            rotationSum += state->cornerPieces[i].rotation;
        }
    }

}

int* GetCornerPositionsPerIndex(int index) {
    int permutationIndex = index / CORNER_ROTATIONS_COUNT;
    int* positions = new int[8] {7, 7, 7, 7, 7, 7, 7, 7};

    const int coefficients[] = {
        1 * 2 * 3 * 4 * 5 * 6 * 7 * 8,
        1 * 2 * 3 * 4 * 5 * 6 * 7,
        1 * 2 * 3 * 4 * 5 * 6,
        1 * 2 * 3 * 4 * 5,
        1 * 2 * 3 * 4,
        1 * 2 * 3,
        1 * 2,
        1, 
        1 // TODO delte last entry?
    };
    
    // TODO this needs to cleaned up.
    for (int i = 0; i < 7; i++) {
        int localPosition = (permutationIndex % coefficients[i]) / coefficients[i + 1];
        int globalPosition = 0;

        for (int j = 0; j <= localPosition + globalPosition; j++) {
            if (positions[j] <= i) {
                globalPosition++;
            }
        }

        positions[localPosition + globalPosition] = i;
    }

    return positions;
}

