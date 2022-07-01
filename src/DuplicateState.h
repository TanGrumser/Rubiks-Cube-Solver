#pragma once

#include <unordered_map>
#include "RubicsCubeState.h"
#include "StateIndex.h"

namespace DuplicateState {
    
    const string DUPLICATE_STATE_PATH = "LookupTables\\duplicateStates";
    const string TURN_INDEX_DUPLICATE_STATE_PATH = "LookupTables\\duplicateStates";

    const uint64 NUMBER_OF_PATHS_AT_DEPTH[10] = {
        1,
        1 + 18,
        1 + 18 + 18 * 18,
        1 + 18 + 18 * 18 + 18 * 18 * 18,
        1 + 18 + 18 * 18 + 18 * 18 * 18 + 18 * 18 * 18 * 18,
        1 + 18 + 18 * 18 + 18 * 18 * 18 + 18 * 18 * 18 * 18 + 18 * 18 * 18 * 18 * 18,
        1 + 18 + 18 * 18 + 18 * 18 * 18 + 18 * 18 * 18 * 18 + 18 * 18 * 18 * 18 * 18 + 18 * 18 * 18 * 18 * 18 * 18,
        1 + 18 + 18 * 18 + 18 * 18 * 18 + 18 * 18 * 18 * 18 + 18 * 18 * 18 * 18 * 18 + 18 * 18 * 18 * 18 * 18 * 18 + 18 * 18 * 18 * 18 * 18 * 18 * 18,
        1 + 18 + 18 * 18 + 18 * 18 * 18 + 18 * 18 * 18 * 18 + 18 * 18 * 18 * 18 * 18 + 18 * 18 * 18 * 18 * 18 * 18 + 18 * 18 * 18 * 18 * 18 * 18 * 18 + 18 * 18 * 18 * 18 * 18 * 18 * 18 * 18,
        1 + 18 + 18 * 18 + 18 * 18 * 18 + 18 * 18 * 18 * 18 + 18 * 18 * 18 * 18 * 18 + 18 * 18 * 18 * 18 * 18 * 18 + 18 * 18 * 18 * 18 * 18 * 18 * 18 + 18 * 18 * 18 * 18 * 18 * 18 * 18 * 18 + 18 * 18 * 18 * 18 * 18 * 18 * 18 * 18 * 18
    };

    extern bool active;

    void GenerateLookupTable();
    void GenerateLookupTurnBasedTable();
    void SetBaseState(RubicsCubeState* state);
    void LoadDuplicateStateIndex();
    bool WasStateReached(StateIndex index, int depth);
    bool IsStateContained(StateIndex index);
    void ResetAllStates();
}