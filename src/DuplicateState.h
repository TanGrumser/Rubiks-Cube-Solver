#pragma once

#include <unordered_map>
#include "RubicsCubeState.h"

namespace DuplicateState {
    const string DUPLICATE_STATE_PATH = "duplicateStates";

    void GenerateLookupTable();
    void SetBaseState(RubicsCubeState* state);
    void LoadDuplicateStateIndex();
}