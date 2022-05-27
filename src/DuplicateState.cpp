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

#define uint64 unsigned long long

tsl::robin_map<StateIndex, tuple<string, char>, StateIndexHasher> reachedStates;
tsl::robin_map<StateIndex, bool, StateIndexHasher> duplicateReachableStates;
int foundDuplicates = 0;

void EvaluateState(RubicsCubeState* state, char depth, Turn lastTurn, std::vector<Turn> exploredTurns, char maxDepth, vector<Turn> lastTurns); 
uint64 CalculateStateIndex(RubicsCubeState* state);

void DuplicateState::GenerateLookupTable() {
    RubicsCubeState* initialState = RubicsCubeState::InitialState()->Copy();
    const int MAX_DEPTH = 7;
    vector<Turn> lastTurns;

    for (int depth = 0; depth <= MAX_DEPTH; depth++) {
        std::cout << "Searching at depth " << depth;
        
        EvaluateState(initialState, 0, Turn::Empty(), Turn::AllTurns, depth, lastTurns);

        std::cout << " and found number of duplicates at this depth is " << foundDuplicates << endl;
        foundDuplicates = 0;
        /*
        for(auto it = reachedStates.begin(); it != reachedStates.end(); ++it) {
            std::cout << (int)std::get<1>(it.value()) << endl;
        }
        

        for(auto it = reachedStates.begin(); it != reachedStates.end(); ++it) {
            it.value() = {std::get<0>(it.value()), std::get<1>(it.value()) & 0b01111111};
        }
        */
    }

}

void EvaluateState(RubicsCubeState* state, char depth, Turn lastTurn, std::vector<Turn> exploredTurns, char maxDepth, vector<Turn> lastTurns) {
    StateIndex stateIndex = state->GetLookupIndex();
    string turnString = "";
    
    for (int i = 0; i < lastTurns.size(); i++)  {
        turnString += lastTurns[i].ToString() + " ";
    }
    /*
    
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
    */

    if (depth == maxDepth) {
        if (reachedStates.find(stateIndex) != reachedStates.end()) {
            char entry = std::get<1>(reachedStates[stateIndex]);

            // we found a state that can be reached via multiple paths and save it into our duplicate reachable states table.
            if (Utils::IsMostSignificantBitSet(entry)) {
                duplicateReachableStates[stateIndex] = false;

                //std::cout << std::get<0>(reachedStates[stateIndex]) << endl;
                //std::cout << turnString << "\n" << endl;
                
                foundDuplicates++;

            } else {
                reachedStates[stateIndex] = {std::get<0>(reachedStates[stateIndex]), depth | 0b10000000};
                //std::cout << (int)std::get<1>(reachedStates[stateIndex]) << " was the bit succesfully set? " << std::endl;
                //throw std::exception();
            }
        } else {
            char entry = std::get<1>(reachedStates[stateIndex]);
            // The state was reached before and can 
            reachedStates[stateIndex] = {turnString, depth | 0b10000000};
        }
    } else if (depth < maxDepth) {        
        for (Turn turn : exploredTurns) {
            if (turn.IsTurnBacktracking(lastTurn)) {
                continue;
            }
            lastTurns.push_back(turn);
            state->ApplyTurn(turn);
            EvaluateState(state, depth + 1, turn, Turn::AllTurns, maxDepth, lastTurns);
            state->ApplyTurn(turn.Inverse());
            lastTurns.pop_back();
        }
    }
}

uint64 CalculateStateIndex(RubicsCubeState* state) {
    int cornerIndex = LookupTable::GetCornerLookupIndex(state);
    int edgeIndex = LookupTable::GetFullEdgeLookupIndex(state);

    return cornerIndex * LookupTable::FULL_EDGE_STATES_COUNT + edgeIndex;
}