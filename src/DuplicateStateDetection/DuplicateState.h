#pragma once

#include <unordered_map>
#include <array>
#include "TurnIndexer.h"
#include "../Model/RubiksCubeState.h"
#include "../Model/StateIndex.h"
#include "../Utils/Constants.h"
#include "../Model/RubiksCubeStateShift.h"

namespace DuplicateState {
    
    enum Mode {
        OFF,
        STATE_INDEX,
        TURN_INDEX,
        HASH_TABLE
    };

    const string DUPLICATE_STATE_PATH = "LookupTables/duplicateStates";
    const string TURN_INDEX_DUPLICATE_STATE_PATH = "LookupTables/duplicateStatesTurnIndex";

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

    extern Mode mode;
    extern uint8_t maxDepth;
    extern int foundDuplicates;

    void GenerateLookupTable();
    void GenerateTurnBasedLookupTable();
    void SetBaseState(RubiksCubeState* state);
    void LoadDuplicateStateIndex();
    void LoadDuplicateStateTurnIndex();
    bool WasStateReached(StateIndex index, int depth);
    bool PruneByTurnIndex(array<Turn, 50> moves, int size);
    bool PruneWithHashTable(StateIndex index, int depth);
    bool IsStateContained(StateIndex index);
    void ResetAllStates();

    inline bool PruneState(RubiksCubeState& state, RubiksCubeStateShift shift, uint8_t depth, array<Turn, 50> moves) {
        if (depth > maxDepth) {
            return false;
        }
        
        switch (mode) {
            case Mode::OFF:         return false;
            case Mode::STATE_INDEX: return WasStateReached(shift.GetShiftedState(state), depth);
            case Mode::TURN_INDEX:  return PruneByTurnIndex(moves, depth);
            case Mode::HASH_TABLE:  return PruneWithHashTable(state.GetLookupIndex(), depth);

            default: return false;
        }
    }
}