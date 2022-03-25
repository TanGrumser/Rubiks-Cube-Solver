#include <iostream>
#include <vector>
#include "RubicsCubeState.h"
#include "Solver.h"
#include "Turn.h"
#include <algorithm>

int Search(vector<RubicsCubeState*> *path, int depth, int bound, Turn lastTurn);
vector<Turn> GenerateTurnSequenceFromStateSequence(vector<RubicsCubeState*> stateSequence);
const int MAX_BOUND = 100000;
const int SOLUTION_FOUND = -1;

vector<Turn> Solver::SR_IterativeDeepeningAStar(RubicsCubeState* startState) {
        int bound = Solver::GetDistanceHeuristic(startState, RubicsCubeState::InitialState());
        vector<RubicsCubeState*> path = {};
        path.push_back(startState);

        while (true) {
            std::cout << "Searching with an heuristic bound of " << bound << endl;
            int newBound = Search(&path, 0, bound, Turn::Empty());

            if (newBound == SOLUTION_FOUND) {
                break;
            } 
            
            if (newBound == MAX_BOUND) {
                return {};
            } 

            bound = newBound;
        }

        return Solver::GenerateTurnSequenceFromStateSequence(path);
    }

int Search(vector<RubicsCubeState*> *path, int depth, int bound, Turn lastTurn) {
    RubicsCubeState* node = path->back();
    int totalEstimatedCost = depth + Solver::GetDistanceHeuristic(node, RubicsCubeState::InitialState());
    
    if (totalEstimatedCost > bound) {
        return totalEstimatedCost;
    } 

    if (node->Equals(RubicsCubeState::InitialState())) {
        return SOLUTION_FOUND;
    } 

    int min = MAX_BOUND;

    // TODO allocating over and over again isn't necessary. Rather a stack with pre allocated states should be used.

    RubicsCubeState* succesor = node->Copy();
    path->push_back(succesor);

    for (int i = 0; i < Turn::CountAllTurns; i++) {
        if (!Turn::AllTurns[i].IsTurnBacktracking(lastTurn)) {
            succesor->ApplyTurn(Turn::AllTurns[i]);
            
            int newBound = Search(path, depth + 1, bound, Turn::AllTurns[i]);

            if (newBound == SOLUTION_FOUND) {
                return SOLUTION_FOUND;
            }

            if (newBound < min) {
                min = newBound;
            } 

            succesor->ApplyTurn(Turn::AllTurns[i].Inverse());
        }
    }

    delete path->back();
    path->pop_back();

    return min;
}