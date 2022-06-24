#pragma once

#include <vector>
#include "RubicsCubeState.h"

namespace Solver {
    extern int heuristicMode;
    extern int solverIndex;
    extern int threadCount;
    extern int searchedStates;
    /**
     * @brief 
     * A parallel recusrive implementation of the IDA* algorithm, which  is parallelized by splitting all possible moves from the first state up onto threads.
     * 
     * 
     * @param startState a state of a rubics cube, for which the shortest sequence of moves will be searched for, that solves the cube.
     * @return A vector<Turn> of the moves, that will solve the cube, when applied to the start state.
     */
    vector<Turn> PR_IterativeDeepeningAStar(RubicsCubeState& startState);
    
    /**
     * @brief 
     * An single threaded recusrive implementation of the IDA* algorithm.
     * 
     * 
     * @param startState a state of a rubics cube, for which the shortest sequence of moves will be searched for, that solves the cube.
     * @return A vector<Turn> of the moves, that will solve the cube, when applied to the start state.
     */
    vector<Turn> SR_IterativeDeepeningAStar(RubicsCubeState& startState);


    int GetDistanceHeuristic(RubicsCubeState& state, int bound);
    vector<Turn> GenerateTurnSequenceFromStateSequence(vector<RubicsCubeState> stateSequence);

}