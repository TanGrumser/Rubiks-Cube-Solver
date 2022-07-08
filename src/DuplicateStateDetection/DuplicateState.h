#pragma once

#include <unordered_map>
#include <array>
#include "TurnIndexer.h"
#include "../Model/RubiksCubeState.h"
#include "../Model/StateIndex.h"
#include "../Utils/Constants.h"

namespace DuplicateState {
    
    const string DUPLICATE_STATE_PATH = "LookupTables\\duplicateStates";
    const string TURN_INDEX_DUPLICATE_STATE_PATH = "LookupTables\\duplicateStatesTurnIndex";

    const uint64_t NUMBER_OF_PATHS_AT_DEPTH[10] = {
        1ull,
        1ull + 18ull,
        1ull + 18ull + 18ull * 18ull,
        1ull + 18ull + 18ull * 18ull + 18ull * 18ull * 18ull,
        1ull + 18ull + 18ull * 18ull + 18ull * 18ull * 18ull + 18ull * 18ull * 18ull * 18ull,
        1ull + 18ull + 18ull * 18ull + 18ull * 18ull * 18ull + 18ull * 18ull * 18ull * 18ull + 18ull * 18ull * 18ull * 18ull * 18ull,
        1ull + 18ull + 18ull * 18ull + 18ull * 18ull * 18ull + 18ull * 18ull * 18ull * 18ull + 18ull * 18ull * 18ull * 18ull * 18ull + 18ull * 18ull * 18ull * 18ull * 18ull * 18ull,
        1ull + 18ull + 18ull * 18ull + 18ull * 18ull * 18ull + 18ull * 18ull * 18ull * 18ull + 18ull * 18ull * 18ull * 18ull * 18ull + 18ull * 18ull * 18ull * 18ull * 18ull * 18ull + 18ull * 18ull * 18ull * 18ull * 18ull * 18ull * 18ull,
        1ull + 18ull + 18ull * 18ull + 18ull * 18ull * 18ull + 18ull * 18ull * 18ull * 18ull + 18ull * 18ull * 18ull * 18ull * 18ull + 18ull * 18ull * 18ull * 18ull * 18ull * 18ull + 18ull * 18ull * 18ull * 18ull * 18ull * 18ull * 18ull + 18ull * 18ull * 18ull * 18ull * 18ull * 18ull * 18ull * 18ull,
        1ull + 18ull + 18ull * 18ull + 18ull * 18ull * 18ull + 18ull * 18ull * 18ull * 18ull + 18ull * 18ull * 18ull * 18ull * 18ull + 18ull * 18ull * 18ull * 18ull * 18ull * 18ull + 18ull * 18ull * 18ull * 18ull * 18ull * 18ull * 18ull + 18ull * 18ull * 18ull * 18ull * 18ull * 18ull * 18ull * 18ull + 18ull * 18ull * 18ull * 18ull * 18ull * 18ull * 18ull * 18ull * 18ull
    };

    extern bool active;
    extern int foundDuplicates;

    void GenerateLookupTable();
    void GenerateTurnBasedLookupTable();
    void SetBaseState(RubiksCubeState* state);
    void LoadDuplicateStateIndex();
    void LoadDuplicateStateTurnIndex();
    bool WasStateReached(StateIndex index, int depth);
    bool PruneByTurnIndex(array<Turn, 50> moves, int size);
    bool IsStateContained(StateIndex index);
    void ResetAllStates();
}