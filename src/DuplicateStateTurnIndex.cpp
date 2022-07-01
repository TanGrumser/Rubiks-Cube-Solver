#include <iostream>

#include "DuplicateState.h"
#include "FileManagement.h"
#include "robin-map/robin_map.h"
#include "StateIndexHasher.h"
#include "TurnIndexer.h"

int foundDuplicates = 0;
char* duplicateStates;
tsl::robin_map<StateIndex, bool, StateIndexHasher> reachedStates;


void EvaluateState(RubicsCubeState& state, char depth, Turn lastTurn, std::vector<Turn> exploredTurns, char maxDepth, vector<Turn> lastTurns); 

void DuplicateState::GenerateLookupTurnBasedTable() {
    RubicsCubeState& initialState = RubicsCubeState::InitialState().Copy();
    const int MAX_DEPTH = 7;
    vector<Turn> lastTurns;
    duplicateStates = (char*)malloc(sizeof(char) * DuplicateState::NUMBER_OF_PATHS_AT_DEPTH[MAX_DEPTH]);

    for (int depth = 0; depth <= MAX_DEPTH; depth++) {
        std::cout << "Searching at depth " << depth;
        
        EvaluateState(initialState, 0, Turn::Empty(), Turn::AllTurns, depth, lastTurns);

        std::cout << " and found number of duplicates at this depth is " << foundDuplicates << endl;
        foundDuplicates = 0;
    }

    std::cout << "Completed duplicate state generation." << endl;

    FileManagement::WriteBufferToFile(TURN_INDEX_DUPLICATE_STATE_PATH, duplicateStates, DuplicateState::NUMBER_OF_PATHS_AT_DEPTH[MAX_DEPTH]);    
}

void EvaluateState(RubicsCubeState& state, char depth, Turn lastTurn, std::vector<Turn> exploredTurns, char maxDepth, vector<Turn> lastTurns) {
    StateIndex stateIndex = state.GetLookupIndex();
    
    // we found a state, that is reachable from more than one path.
    if (reachedStates.find(stateIndex) != reachedStates.end()) {
        // we check if it was already reached in the search with the current max depth and return if so, 
        // since all following states will be processed already.
        if (reachedStates[stateIndex]) {
            return;
        } else {
            // if it hasn't been reached in the current search we set it's flag, so no other path will process follwing states again and continiue.
            reachedStates[stateIndex] = true;
        }
    }

    if (depth == maxDepth) {
        if (reachedStates.find(stateIndex) != reachedStates.end()) {
            uint64 turnIndex =  TurnIndexer::getIndexFromVector(lastTurns);
            duplicateStates[turnIndex] = 1;

            // we found a state that can be reached via multiple paths and save it into our duplicate reachable states table.
            if (Utils::IsMostSignificantBitSet(entry)) {
                reachedStates[stateIndex] = false;

                foundDuplicates++;

            } else {
                reachedStates[stateIndex] = depth | 0b10000000;
                //std::cout << (int)std::get<1>(reachedStates[stateIndex]) << " was the bit succesfully set? " << std::endl;
                //throw std::exception();
            }
        } else {
            // The state was reached before and can 
            reachedStates[stateIndex] = true;
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
