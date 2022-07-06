#include "StateIndexReverser.h"

void StateIndexReverser::Test() {
    const int TEST_COUNT = 1000000;
    const int APPLIED_MOVES = 100;
    StopWatch timer;

    std::cout << "Starting reversed lexicographic rank function tests. (1 000 000 Iterations)" << std::endl;
    
    timer.StartTimer();

    for (int i = 0; i < TEST_COUNT; i++) {
        RubicsCubeState state = RubicsCubeState::InitialState().Copy();

        for (int j = 0; j < APPLIED_MOVES; j++) {
            state.ApplyTurn(Turn((i * 1000 + j * 100) % 18));
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
    uint64_t edgePermutationIndex   = index.edgeIndex / FULL_EDGE_ROTATION_COUNT;
    uint64_t edgeRotationIndex      = index.edgeIndex % FULL_EDGE_ROTATION_COUNT;
    uint64_t cornerPermutationIndex = index.cornerIndex / CORNER_ROTATIONS_COUNT;
    uint64_t cornerRotationIndex    = index.cornerIndex % CORNER_ROTATIONS_COUNT;

    array<unsigned, 8> cornerIndicies = cornerIndexer.getPermutation(cornerPermutationIndex);
    array<unsigned, 12> edgeIndicies = egdeIndexer.getPermutation(edgePermutationIndex);
    
    unsigned rotationSum = 0;

    for (int i = 0; i < 8; i++) {
        result.corners[i].index = cornerIndicies[i];
        result.corners[i].rotation = (cornerRotationIndex / powersOfThree[i]) % 3u;

        rotationSum += result.corners[i].rotation;
    }

    result.corners[8 - 1].rotation = (21u - rotationSum) % 3u;

    rotationSum = 0;

    for (int i = 0; i < 12; i++) {
        result.edges[i].index = edgeIndicies[i];
        result.edges[i].rotation = edgeRotationIndex >> i & 1u; // simulates dividing by 2^i but is faster

        rotationSum += result.edges[i].rotation;
    }

    result.edges[12 - 1].rotation = rotationSum & 1u; // this simulates mod2 but is faster

    return result;
}