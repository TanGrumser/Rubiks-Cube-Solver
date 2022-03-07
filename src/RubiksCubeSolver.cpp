#include <iostream>
#include <string>
#include "RubicsCubeState.h"
#include "Turn.h"
#include "StringUtils.h"
#include <vector>
#include "Solver.h"
#include "Stopwatch.h"

using namespace std;

int main(int argc, char *argv[]) {
    RubicsCubeState* state = RubicsCubeState::InitialState()->Copy();

    for (int i = 1; i < argc; i++) {
        if (((string) argv[i]).compare("-s") == 0) {
            string stateString = (string) argv[i + 1];
            state = RubicsCubeState::ParseStateString(stateString);
        }

        if (((string) argv[i]).compare("-t") == 0) {
            string turnString = (string) argv[i + 1];
            vector<string> turns = StringUtils::Split(turnString, " ");
            
            for (int i = 0; i < turns.size(); i++) {
                Turn turn = Turn::Parse(turns[i]);
                state->ApplyTurn(turn);
            }
        }
    }

    //std::cout << Solver::GetDistanceHeuristic(state, RubicsCubeState::InitialState()) << endl;
    Stopwatch::StartTimer();
    vector<Turn> solution = Solver::IterativeDeepeningAStar(state);
    Stopwatch::StopTimer();

    std::cout << "Time required: " << Stopwatch::GetFormattedTimeInSeconds() << endl;

    for (int i = 0; i < solution.size(); i++) {
        std::cout << solution[i].ToString();

        if (i < solution.size() - 1) {
            std::cout << " ";
        }
    }
    /*
    */  
    
    return 0;
} 