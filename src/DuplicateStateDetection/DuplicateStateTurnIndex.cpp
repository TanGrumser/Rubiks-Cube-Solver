#include <iostream>

#include "DuplicateState.h"
#include "TurnIndexer.h"
#include "../Utils/FileManagement.h"
#include "../Utils/robin-map/robin_map.h"
#include "../Model/StateIndexHasher.h"

char* duplicateTurnIndexStates;
tsl::robin_map<StateIndex, bool, StateIndexHasher> reachedStatesTurnIndex;

void EvaluateStateTurnIndex(RubiksCubeState& state, char depth, Turn lastTurn, std::vector<Turn> exploredTurns, char maxDepth, vector<Turn> lastTurns); 


bool DuplicateState::PruneByTurnIndex(array<Turn, 50> moves, int size) {
    if (size > 7) {
        return false;
    }

    return duplicateTurnIndexStates[TurnIndexer::getIndexFromArray(moves, size)] == 1;
}

void DuplicateState::LoadDuplicateStateTurnIndex() {
    uint64_t* bufferSize = new uint64_t();

    duplicateTurnIndexStates = (char*)FileManagement::LoadBufferFromFile(TURN_INDEX_DUPLICATE_STATE_PATH, bufferSize);

    std::cout << "Loaded duplicate states Turn Index" << endl;
}

void DuplicateState::GenerateTurnBasedLookupTable() {
    RubiksCubeState& initialState = RubiksCubeState::InitialState().Copy();
    const int MAX_DEPTH = DuplicateState::maxDepth;
    vector<Turn> lastTurns;
    duplicateTurnIndexStates = (char*)malloc(sizeof(char) * DuplicateState::NUMBER_OF_PATHS_AT_DEPTH[MAX_DEPTH]);

    for (int depth = 0; depth <= MAX_DEPTH; depth++) {
        std::cout << "Searching at depth " << depth;
        
        EvaluateStateTurnIndex(initialState, 0, Turn::Empty(), Turn::AllTurns, depth, lastTurns);

        std::cout << " and found number of duplicates at this depth is " << foundDuplicates << endl;
        foundDuplicates = 0;
    }

    std::cout << "Completed duplicate state generation." << endl;

    FileManagement::WriteBufferToFile(TURN_INDEX_DUPLICATE_STATE_PATH, duplicateTurnIndexStates, DuplicateState::NUMBER_OF_PATHS_AT_DEPTH[MAX_DEPTH]);    
}

void EvaluateStateTurnIndex(RubiksCubeState& state, char depth, Turn lastTurn, std::vector<Turn> exploredTurns, char maxDepth, vector<Turn> lastTurns) {
    StateIndex stateIndex = state.GetLookupIndex();
    
    if (depth == maxDepth) {
        if (reachedStatesTurnIndex.find(stateIndex) != reachedStatesTurnIndex.end()) {
            uint64_t turnIndex =  TurnIndexer::getIndexFromVector(lastTurns);
            duplicateTurnIndexStates[turnIndex] = 1;
            DuplicateState::foundDuplicates++;
        } else {
            // The state was reached before and can 
            reachedStatesTurnIndex[stateIndex] = true;
        }
    } else if (depth < maxDepth) {        
        for (Turn turn : exploredTurns) {
            if (turn.IsTurnBacktracking(lastTurn)) {
                continue;
            }
            
            lastTurns.push_back(turn);
            state.ApplyTurn(turn);
            EvaluateStateTurnIndex(state, depth + 1, turn, Turn::AllTurns, maxDepth, lastTurns);
            state.ApplyTurn(turn.Inverse());
            lastTurns.pop_back();
        }
    }
}
