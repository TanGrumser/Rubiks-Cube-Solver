#include <iostream>
#include <vector>
#include "RubicsCubeState.h"
#include "Solver.h"
#include "Turn.h"
#include <algorithm>

int Search(RubicsCubeState* state, vector<Turn>* path, int depth, int bound, Turn lastTurn);
vector<Turn> GenerateTurnSequenceFromStateSequence(vector<RubicsCubeState*> stateSequence);
const int MAX_BOUND = 100000;
int searchedStates = 0;

vector<Turn> Solver::IterativeDeepeningAStar(RubicsCubeState* startState) {
        int bound = Solver::GetDistanceHeuristic(startState, RubicsCubeState::InitialState());
        vector<Turn>* path = new std::vector<Turn>(20, Turn::Empty());

        for (int i = 0; i < path->size(); i++) {
            (*path)[i] = Turn::Empty();
        }

        while (true) {
            int newBound = Search(startState, path, 0, bound, Turn::Empty());

            if (newBound == -1) {
                break;
            } 
            
            if (newBound == MAX_BOUND) {
                return {};
            } 

            bound = newBound;
        }

        while (path->back().Equals(Turn::Empty())) {
            path->pop_back();
        }

        return *path;
    }

int Search(RubicsCubeState* state, vector<Turn>* path, int depth, int bound, Turn lastTurn) {
    int totalEstimatedCost = depth + Solver::GetDistanceHeuristic(state, RubicsCubeState::InitialState());
    
    if (totalEstimatedCost > bound) {
        return totalEstimatedCost;
    } 

    if (state->Equals(RubicsCubeState::InitialState())) {
        return -1;
    } 

    int min = MAX_BOUND;

    // TODO allocating over and over again isn't necessary. Rather a stack with pre allocated states should be used.

    for (int i = 0; i < Turn::CountAllTurns; i++) {
        if (!Turn::AllTurns[i].IsTurnBacktracking(lastTurn)) {
            (*path)[depth] = Turn::AllTurns[i];
            state->ApplyTurn(Turn::AllTurns[i]);
            
            int newBound = Search(state, path, depth + 1, bound, Turn::AllTurns[i]);

            if (newBound == -1) {
                return -1;
            }            

            if (newBound < min) {
                min = newBound;
            } 

            state->ApplyTurn(Turn::AllTurns[i].Inverse());
        }
    }

    return min;
}

vector<Turn> GenerateTurnSequenceFromStateSequence(vector<RubicsCubeState*> stateSequence) {
    vector<Turn> turnSequence = {};
    RubicsCubeState* lastState = stateSequence.back();
    stateSequence.pop_back();

    while (stateSequence.size() != 0) {
        RubicsCubeState* currentState = stateSequence.back();
        turnSequence.push_back(currentState->GetTurnTo(lastState));
        stateSequence.pop_back();
        delete lastState;
        lastState = currentState;
    }    

    std::reverse(turnSequence.begin(), turnSequence.end());
    return turnSequence;
}