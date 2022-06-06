#pragma once

#include <unordered_map>
#include "RubicsCubeState.h"
#include "StateIndex.h"

namespace DuplicateState {
    const string DUPLICATE_STATE_PATH = "duplicateStates";
    extern bool active;

    void GenerateLookupTable();
    void SetBaseState(RubicsCubeState* state);
    void LoadDuplicateStateIndex();
    bool WasStateReached(StateIndex index);
    void ResetAllStates();
}