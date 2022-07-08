#include <iostream>
#include <vector>
#include <algorithm>

#include "Solver.h"
#include "../Model/RubiksCubeState.h"
#include "../Model/Turn.h"

int Search(vector<RubiksCubeState> *path, int depth, int bound, Turn lastTurn);
vector<Turn> GenerateTurnSequenceFromStateSequence(vector<RubiksCubeState*> stateSequence);
const int MAX_BOUND = 100000;
const int SOLUTION_FOUND = -1;

vector<Turn> Solver::SR_IterativeDeepeningAStar(RubiksCubeState& startState) {
        int bound = Solver::GetDistanceHeuristic(startState, 100);
        vector<RubiksCubeState> path = {};
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

int Search(vector<RubiksCubeState> *path, int depth, int bound, Turn lastTurn) {
    RubiksCubeState& node = path->back();
    int totalEstimatedCost = depth + Solver::GetDistanceHeuristic(node, depth);
    
    if (totalEstimatedCost > bound) {
        return totalEstimatedCost;
    } 

    if (node.Equals(RubiksCubeState::InitialState())) {
        return SOLUTION_FOUND;
    } 

    int min = MAX_BOUND;

    // TODO allocating over and over again isn't necessary. Rather a stack with pre allocated states should be used.

    RubiksCubeState& succesor = node.Copy();
    path->push_back(succesor);

    for (int i = 0; i < Turn::CountAllTurns; i++) {
        if (!Turn::AllTurns[i].IsTurnBacktracking(lastTurn)) {
            succesor.ApplyTurn(Turn::AllTurns[i]);
            
            int newBound = Search(path, depth + 1, bound, Turn::AllTurns[i]);

            if (newBound == SOLUTION_FOUND) {
                return SOLUTION_FOUND;
            }

            if (newBound < min) {
                min = newBound;
            } 

            succesor.ApplyTurn(Turn::AllTurns[i].Inverse());
        }
    }

    path->back();
    path->pop_back();

    return min;
}