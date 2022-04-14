// This is a parrallel recursive Implementation of the iterative deepening A* algorithm for solving the rubiks cube.
// It is parralelized by spliting the different paths going off from the initial state into different searches that are assigned to individual processes.
// This method has the restrictio, that it currently requires a number of threads that is a valid divisor of 18 (Number of adjacent nodes for the start state)

#include <iostream>
#include <vector>
#include "RubicsCubeState.h"
#include "Solver.h"
#include "Turn.h"
#include <algorithm>
#include <thread>

using std::vector;



int Search(vector<RubicsCubeState*>* path, int depth, int bound, Turn lastTurn, int* minBound, vector<Turn> nextTurns, bool* isSolutionFound);
vector<Turn> GenerateTurnSequenceFromStateSequence(vector<RubicsCubeState*> stateSequence);
const int MAX_BOUND = 100000;
const int SOLUTION_FOUND = -1;

vector<Turn> Solver::PR_IterativeDeepeningAStar(RubicsCubeState* startState) {
    int bound = Solver::GetDistanceHeuristic(startState, RubicsCubeState::InitialState());
    vector<RubicsCubeState*> path = {};
    bool* isSolutionFound = new bool(false);

    path.push_back(startState);

    vector<vector<RubicsCubeState*>*> paths(Solver::threadCount);
    vector<RubicsCubeState*>* solutionPath;
    vector<int*> newBounds(Solver::threadCount);

    for (int i = 0; i < Solver::threadCount; i++) {
        paths[i] = new vector<RubicsCubeState*>(1, startState->Copy());
        newBounds[i] = new int();
    }

    while (true) {
        vector<std::thread> threads(Solver::threadCount);

        for (int i = 0; i < Solver::threadCount; i++) {
            *(newBounds[i]) = MAX_BOUND;
            std::vector<Turn> exploredTurns = Turn::GetSubsetTurns(i ,Solver::threadCount);
            std::thread thread(Search, paths[i], 0, bound, Turn::Empty(), newBounds[i], exploredTurns, isSolutionFound);
            threads[i] = std::move(thread);
        }

        std::cout << "Searching with an heuristic bound of " << bound << endl;
        bound = MAX_BOUND;
        // Wait for all threads to finish.
        for (int i = 0; i < Solver::threadCount; i++) { 
            threads[i].join(); 

            if (*(newBounds[i]) == SOLUTION_FOUND) {
                bound = SOLUTION_FOUND;
                solutionPath = paths[i];
            }

            // This is not a problem to be called after a solution was found, since no boudn will be smaller then SOLUTION_FOUND (= -1)
            if (*(newBounds[i]) < bound) {
                bound = *newBounds[i];
            }
        } 

        if (bound == SOLUTION_FOUND) {
            break;
        }
    }

    return Solver::GenerateTurnSequenceFromStateSequence(*solutionPath);
}

int Search(vector<RubicsCubeState*>* path, int depth, int bound, Turn lastTurn, int* minBound, vector<Turn> nextTurns, bool* isSolutionFound) {
    RubicsCubeState* node = path->back();
    int totalEstimatedCost = depth + Solver::GetDistanceHeuristic(node, RubicsCubeState::InitialState());
    
    if (totalEstimatedCost > bound || *isSolutionFound) {
        *minBound = totalEstimatedCost;
        return totalEstimatedCost;
    } 

    if (node->Equals(RubicsCubeState::InitialState())) {
        *isSolutionFound = true;
        *minBound = SOLUTION_FOUND;
        return SOLUTION_FOUND;
    } 

    int min = MAX_BOUND;

    // TODO allocating over and over again isn't necessary. Rather a stack with pre allocated states should be used.

    RubicsCubeState* succesor = node->Copy();
    path->push_back(succesor);

    for (int i = 0; i < nextTurns.size(); i++) {
        if (!nextTurns[i].IsTurnBacktracking(lastTurn)) {
            succesor->ApplyTurn(nextTurns[i]);
            
            int newBound = Search(path, depth + 1, bound, nextTurns[i], minBound, Turn::AllTurns, isSolutionFound);

            if (newBound == SOLUTION_FOUND) {
                *minBound = SOLUTION_FOUND;
                return SOLUTION_FOUND;
            }

            if (newBound < min) {
                min = newBound;
            } 

            succesor->ApplyTurn(nextTurns[i].Inverse());
        }
    }

    delete path->back();
    path->pop_back();

    *minBound = min;
    return min;
}
