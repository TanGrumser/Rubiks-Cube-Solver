#include "DuplicateState.h"
#include "RubicsCubeState.h"
#include "StateIndexHasher.h"
// TODO remove this. This is only used for index calculation.
#include "LookupTable.h"
#include "StateIndex.h"
#include "robin-map/robin_map.h"
#include <iostream>
#include <string>
#include <vector>
#include "Turn.h"
#include <exception>
#include <tuple>
#include "utils.h"
#include "FileManagement.h"
#include <mutex>


tsl::robin_map<StateIndex, char, StateIndexHasher> reachedStates;
tsl::robin_map<StateIndex, bool, StateIndexHasher> duplicateReachableStates;
tsl::robin_map<StateIndex, int, StateIndexHasher> duplicateReachableStates2;
std::mutex duplicateStateLookup;

void EvaluateState(RubicsCubeState& state, char depth, Turn lastTurn, std::vector<Turn> exploredTurns, char maxDepth, vector<Turn> lastTurns); 
uint64 CalculateStateIndex(RubicsCubeState* state);

namespace DuplicateState {
    bool active = false;
    int foundDuplicates = 0;
}

bool DuplicateState::IsStateContained(StateIndex index) {
    return duplicateReachableStates2.find(index) != duplicateReachableStates2.end();
}

bool DuplicateState::WasStateReached(StateIndex index, int depth) {
    if (duplicateReachableStates2.find(index) != duplicateReachableStates2.end()) {
        duplicateStateLookup.lock();

        // we check if it was already reached in the search with the current max depth and return if so, 
        // since all following states will be processed already.
        if (duplicateReachableStates2[index] <= depth) {
            duplicateStateLookup.unlock();
            return true;
        } else {
            // if it hasn't been reached in the current search we set it's flag, so no other path will process follwing states again and continiue.
            duplicateReachableStates2[index] = depth;
        }

        duplicateStateLookup.unlock();
    }

    return false;
}

void DuplicateState::ResetAllStates() {
    for(auto it = duplicateReachableStates2.begin(); it != duplicateReachableStates2.end(); ++it) {
        it.value() = 100;
    }
}

bool CheckAndSetDuplicateReachableState(RubicsCubeState* state) {
    StateIndex shiftedStateIndex;

    if (duplicateReachableStates.find(shiftedStateIndex) != duplicateReachableStates.end()) {
        if (duplicateReachableStates[shiftedStateIndex]) {
            return true;
        } else { 
            duplicateReachableStates[shiftedStateIndex] = true;
            return false;
        }
    }

    return false;
}

void DuplicateState::LoadDuplicateStateIndex() {
    uint64_t* bufferSize = new uint64_t();

    unsigned char* buffer = (unsigned char*)FileManagement::LoadBufferFromFile(DUPLICATE_STATE_PATH, bufferSize);

    for (int entry = 0; entry < *bufferSize; entry += 16) {
        StateIndex stateIndex;
        stateIndex.cornerIndex = 0;
        stateIndex.edgeIndex   = 0;

        for (int i = 0; i < 8; i++) {
            stateIndex.cornerIndex |= ((uint64)buffer[entry + i]) << (8 * i);
            stateIndex.edgeIndex   |= ((uint64)buffer[entry + i + 8]) << (8 * i);
        }

        duplicateReachableStates2[stateIndex] = 100;
    }

    std::cout << "Loaded duplicate states:" << endl;
}

void DuplicateState::GenerateLookupTable() {
    RubicsCubeState& initialState = RubicsCubeState::InitialState().Copy();
    const int MAX_DEPTH = 7;
    vector<Turn> lastTurns;

    for (int depth = 0; depth <= MAX_DEPTH; depth++) {
        std::cout << "Searching at depth " << depth;
        
        EvaluateState(initialState, 0, Turn::Empty(), Turn::AllTurns, depth, lastTurns);

        std::cout << " and found number of duplicates at this depth is " << foundDuplicates << endl;
        foundDuplicates = 0;
    }

    std::cout << "Completed duplicate state generation." << endl;

    vector<StateIndex> duplicteStatesBuffer;

    for(auto it = duplicateReachableStates.begin(); it != duplicateReachableStates.end(); ++it) {
        duplicteStatesBuffer.push_back(it.key());
    }

    FileManagement::WriteBufferToFile(DUPLICATE_STATE_PATH, (char*)duplicteStatesBuffer.data(), duplicteStatesBuffer.size() * sizeof(StateIndex));    
}

void EvaluateState(RubicsCubeState& state, char depth, Turn lastTurn, std::vector<Turn> exploredTurns, char maxDepth, vector<Turn> lastTurns) {
    StateIndex stateIndex = state.GetLookupIndex();
    
    // we found a state, that is reachable from more than one path.
    if (duplicateReachableStates.find(stateIndex) != duplicateReachableStates.end()) {
        // we check if it was already reached in the search with the current max depth and return if so, 
        // since all following states will be processed already.
        if (duplicateReachableStates[stateIndex]) {
            return;
        } else {
            // if it hasn't been reached in the current search we set it's flag, so no other path will process follwing states again and continiue.
            duplicateReachableStates[stateIndex] = true;
        }
    }

    if (depth == maxDepth) {
        if (reachedStates.find(stateIndex) != reachedStates.end()) {
            char entry = reachedStates[stateIndex];

            // we found a state that can be reached via multiple paths and save it into our duplicate reachable states table.
            if (Utils::IsMostSignificantBitSet(entry)) {
                duplicateReachableStates[stateIndex] = false;

                DuplicateState::foundDuplicates++;

            } else {
                reachedStates[stateIndex] = depth | 0b10000000;
                //std::cout << (int)std::get<1>(reachedStates[stateIndex]) << " was the bit succesfully set? " << std::endl;
                //throw std::exception();
            }
        } else {
            // The state was reached before and can 
            reachedStates[stateIndex] = depth | 0b10000000;
        }
    } else if (depth < maxDepth) {        
        for (Turn turn : exploredTurns) {
            if (turn.IsTurnBacktracking(lastTurn)) {
                continue;
            }
            
            lastTurns.push_back(turn);
            state.ApplyTurn(turn);
            EvaluateState(state, depth + 1, turn, Turn::AllTurns, maxDepth, lastTurns);
            state.ApplyTurn(turn.Inverse());
            lastTurns.pop_back();
        }
    }
}

uint64 CalculateStateIndex(RubicsCubeState& state) {
    int cornerIndex = LookupTable::GetCornerLookupIndex(state);
    int edgeIndex = LookupTable::GetFullEdgeLookupIndex(state);

    return cornerIndex * LookupTable::FULL_EDGE_STATES_COUNT + edgeIndex;
}