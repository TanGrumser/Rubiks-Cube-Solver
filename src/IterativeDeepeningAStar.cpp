#include <iostream>
#include <vector>
#include "RubicsCubeState.h"
#include "Solver.h"
#include "Turn.h"
#include <algorithm>

using std::vector;

struct StackEntry {
    RubicsCubeState* state;
    Turn lastTurn;
    int distance;

    StackEntry(RubicsCubeState* state, Turn lastTurn, int distance) {
        this->state = state;
        this->lastTurn = lastTurn;
        this->distance = distance;
    }

    ~StackEntry() {
        delete state;
    }

    StackEntry(const StackEntry& entry) {
        this->state = entry.state;
        this->lastTurn = entry.lastTurn;
        this->distance = entry.distance;
    }
    
    StackEntry& operator=(StackEntry other)
    {
        std::cout << "copy assignment of A\n";
        std::swap(state, other.state);
        std::swap(lastTurn, other.lastTurn);
        std::swap(distance, other.distance);    
        return *this;
    }
};

int Search(vector<RubicsCubeState*> *path, int depth, int bound, Turn lastTurn);
int Worker(vector<StackEntry>* stateStack, int bound);
vector<Turn> GenerateTurnSequenceFromStateSequence(vector<RubicsCubeState*> stateSequence);
const int MAX_BOUND = 100000;
const int SOLUTION_FOUND = -1;
int searchedStates = 0;

vector<Turn> Solver::IterativeDeepeningAStar(RubicsCubeState* startState) {
    int bound = Solver::GetDistanceHeuristic(startState, RubicsCubeState::InitialState());
    vector<RubicsCubeState*> path = {};
    vector<StackEntry>* stateStack = new vector<StackEntry>();

    path.push_back(startState);
    stateStack->push_back(StackEntry(startState, Turn::Empty(), 0));

    while (true) {
        int newBound = Worker(stateStack, bound);

        if (newBound == SOLUTION_FOUND) {
            break;
        } 
        
        if (newBound == MAX_BOUND) {
            return {};
        } 

        bound = newBound;
    }

    std::cout << "Finished Search" << endl;

    return GenerateTurnSequenceFromStateSequence(path);
}

int Worker(vector<StackEntry>* stateStack, int bound) {
    int min = MAX_BOUND;

    while (stateStack->size() != 0) {
        StackEntry entry = stateStack->back();
        stateStack->pop_back();

        if (entry.state->Equals(RubicsCubeState::InitialState())) {
            return SOLUTION_FOUND;
        }
        
        int totalEstimatedCost = entry.distance + Solver::GetDistanceHeuristic(entry.state, RubicsCubeState::InitialState());

        if (totalEstimatedCost > bound) {
            min = std::min(min, totalEstimatedCost);
        } else {
            for (int i = 0; i < Turn::CountAllTurns; i++) {
                if (!Turn::AllTurns[i].IsTurnBacktracking(entry.lastTurn)) {
                    RubicsCubeState* newState = entry.state->Copy();
                    newState->ApplyTurn(Turn::AllTurns[i]);
                    
                    stateStack->push_back(StackEntry(newState, Turn::AllTurns[i], entry.distance + 1));
                }
            }
        }
    }

    return min;
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