#include "DuplicateState.h"
#include "RubicsCubeState.h"
#include "StateIndexHasher.h"
// TODO remove this. This is only used for index calculation.
#include "LookupTable.h"
#include "StateIndex.h"

#include <unordered_map>

#define uint64 unsigned long long

unordered_map<StateIndex, char, StateIndexHasher> reachedStates;


void EvaluateState(RubicsCubeState* state, char depth, Turn lastTurn, std::vector<Turn> exploredTurns, char maxDepth); 
uint64 CalculateStateIndex(RubicsCubeState* state);

void DuplicateState::GenerateLookupTable() {
    RubicsCubeState* initialState = RubicsCubeState::InitialState()->Copy();
    const int MAX_DEPTH = 8;

    for (int depth = 0; depth < MAX_DEPTH; depth++) {
        EvaluateState(initialState, 0, Turn::Empty(), Turn::AllTurns, depth);
    }
}

void EvaluateState(RubicsCubeState* state, char depth, Turn lastTurn, std::vector<Turn> exploredTurns, char maxDepth) {
    StateIndex stateIndex = state->GetLookupIndex();

    if (reachedStates.find(stateIndex) != reachedStates.end()) {
        // we have reached a state, that was never visited before.
        reachedStates[stateIndex] = depth;
    } else {
        // The state was reached before and can 
    }

    if (depth < maxDepth) {
        for (Turn turn : exploredTurns) {
            if (turn.IsTurnBacktracking(lastTurn)) {
                continue;
            }

            state->ApplyTurn(turn);
            EvaluateState(state, depth + 1, turn, Turn::AllTurns, maxDepth);
            state->ApplyTurn(turn.Inverse());
        }
    }
}

uint64 CalculateStateIndex(RubicsCubeState* state) {
    int cornerIndex = LookupTable::GetCornerLookupIndex(state);
    int edgeIndex = LookupTable::GetFullEdgeLookupIndex(state);

    return cornerIndex * LookupTable::FULL_EDGE_STATES_COUNT + edgeIndex;
}