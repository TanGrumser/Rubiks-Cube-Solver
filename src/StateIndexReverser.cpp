#include "StateIndexReverser.h"

void StateIndexReverser::Test() {
    const int TEST_COUNT = 10000;
    const int APPLIED_MOVES = 100;

    std::cout << "Starting reversed lexicographic rank function tests." << std::endl;

    for (int i = 0; i < TEST_COUNT; i++) {
        RubicsCubeState state = RubicsCubeState::InitialState().Copy();

        for (int j = 0; j < APPLIED_MOVES; j++) {
            state.ApplyTurn(Turn::Random());
        }

        StateIndex index = state.GetLookupIndex();
        RubicsCubeState reconstruction = GetStateFromIndex(index);

        if (!state.Equals(reconstruction)) {
            std::cout << "Test failed. The two states are\n" << state.GetStateString() << "\n" << reconstruction.GetStateString() << std::endl;
            return;
        }
    }

    std::cout << "Test finished succesfully." << std::endl;
}

RubicsCubeState StateIndexReverser::GetStateFromIndex(StateIndex index) {
    RubicsCubeState result;

    // Extract
    uint64 edgePermutation = index.edgeIndex % // Extract constants form Lookup Table to own header file to use it here.

    return result;
}