#include "StateIndexReverser.h"

void StateIndexReverser::Test() {
    const int TEST_COUNT = 10000;
    const int APPLIED_MOVES = 100;
    StopWatch timer;

    std::cout << "Starting reversed lexicographic rank function tests. (10 000 Iterations)" << std::endl;
    
    timer.StartTimer();

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

    std::cout << "Test finished succesfully. Time required: " << timer.GetFormattedTimeInSeconds() << std::endl;
}

RubicsCubeState StateIndexReverser::GetStateFromIndex(StateIndex index) {
    RubicsCubeState result;

    // Extract 
    uint64 edgePermutationIndex   = index.edgeIndex / FULL_EDGE_ROTATION_COUNT;
    uint64 edgeRotationIndex      = index.edgeIndex % FULL_EDGE_ROTATION_COUNT;
    uint64 cornerPermutationIndex = index.cornerIndex / CORNER_ROTATIONS_COUNT;
    uint64 cornerRotationIndex    = index.cornerIndex % CORNER_ROTATIONS_COUNT;

    array<unsigned, 8> cornerIndicies = cornerIndexer.getPermutation(cornerPermutationIndex);
    array<unsigned, 12> edgeIndicies = egdeIndexer.getPermutation(edgePermutationIndex);
    
    unsigned rotationSum = 0;

    for (int i = 0; i < 8; i++) {
        result.corners[i].index = cornerIndicies[i];
        result.corners[i].rotation = (cornerRotationIndex / powersOfThree[i]) % 3u;

        if (i == 8 - 1) {
            result.corners[8 - 1].rotation = (21u - rotationSum) % 3u;
        } else {
            rotationSum += result.corners[i].rotation;
        }
    }

    rotationSum = 0;

    for (int i = 0; i < 12; i++) {
        result.edges[i].index = edgeIndicies[i];
        result.edges[i].rotation = (edgeRotationIndex / powersOfTwo[i]) % 2u;

        if (i == 12 - 1) {
            result.edges[12 - 1].rotation = (rotationSum) % 2u;
        } else {
            rotationSum += result.edges[i].rotation;
        }
    }

    return result;
}