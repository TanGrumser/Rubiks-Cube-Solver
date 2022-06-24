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
#include "DuplicateState.h"
#include "RubicsCubeStateShift.h"
#include "Stopwatch.h"
#include <stack>
#include <queue>
#include <stdexcept>
#include <atomic>

using std::vector;



int Search(vector<RubicsCubeState*>* path, int depth, int bound, Turn lastTurn, int* minBound, vector<Turn> nextTurns, bool* isSolutionFound);
vector<Turn> GenerateTurnSequenceFromStateSequence(vector<RubicsCubeState*> stateSequence);
const int MAX_BOUND = 100000;
const int SOLUTION_FOUND = -1;
unsigned long long traversedStates = 0;
unsigned long long duplicateStates = 0;
std::atomic_int32_t traversedStatesAtDepth;

RubicsCubeStateShift* shift;

void idaSearch(RubicsCubeState& startState, int bound, array<Turn, 50>* solution, int* nextBound);

struct Node {
    RubicsCubeState cube;
    Turn move;
    uint8_t depth;
};

struct PrioritizedMove {
    RubicsCubeState cube;
    Turn move;
    uint8_t estMoves; // Priority.  Least number of moves to most.
    bool operator>(const PrioritizedMove& rhs) const {
        return this->estMoves > rhs.estMoves;
    }
};

vector<Turn> Solver::PR_IterativeDeepeningAStar(RubicsCubeState& startState) {
    bool solved = startState.Equals(RubicsCubeState::InitialState());
    StopWatch timer;
    uint8_t bound = Solver::GetDistanceHeuristic(startState, 100);
    array<Turn, 50> moves = {Turn::Empty()};

  
    while (!solved) {
        std::vector<std::thread> threads(Solver::threadCount);
        traversedStatesAtDepth = 0;
        vector<int>* newBounds = new vector<int>();

        for (int i = 0; i < Solver::threadCount; i++) {
          newBounds.push_back(0xff);
          RubicsCubeState state(startState);
          std::vector<Turn> exploredTurns = Turn::GetSubsetTurns(i, LookupTable::threadCount);

          std::thread thread = std::thread(idaSearch, state, bound, &moves, &newBounds[i]);
          threads[i] = std::move(thread);
        }

        for (int i = 0; i < LookupTable::threadCount; i++) {
            threads[i].join();
        }

        if (bound == 0xFF) {
          solved = true;
        }

        cout << "IDA*: Finished bound " << (unsigned)bound
              << ".  Elapsed time: " << timer.GetFormattedTimeInSeconds() << ". " <<
              "traversed staes at this bound: " << traversedStatesAtDepth
              << endl;
    }

    cout << "IDA*: Goal reached in " << timer.GetFormattedTimeInSeconds() << "s. "
         << "traversed staes at this bound: " << traversedStatesAtDepth
         << endl;
    
    vector<Turn> moveVec;

    for (unsigned i = 0; i < moves.size() && !moves.at(i).Equals(Turn::Empty()); ++i)
      moveVec.push_back(moves.at(i));

    return moveVec;
}

void idaSearch(RubicsCubeState& startState, int bound, array<Turn, 50>* moves, int* nextBound) {
    typedef priority_queue<PrioritizedMove, vector<PrioritizedMove>,
      greater<PrioritizedMove> > moveQueue_t;

    stack<Node>           nodeStack;
    Node                  curNode;
    bool solved = false;
    
    (*moves)[0] = Turn::Empty();
    nodeStack.push({startState, Turn::Empty(), 0});

    while (!nodeStack.empty()) {
      traversedStatesAtDepth++;
      curNode = nodeStack.top();
      nodeStack.pop();

      // Keep the list of moves.  The moves end at 0xFF.
      moves->at(curNode.depth) = Turn::Empty();

      if (curNode.depth != 0)
        (*moves)[curNode.depth - 1] = curNode.move;

      if (curNode.depth == bound && curNode.cube.Equals(RubicsCubeState::InitialState())) {
          *nextBound = 0xFF;
          break;
      } else {
        // This is used to sort the successors by estimated moves.
        moveQueue_t successors;

        for (uint8_t i = 0; i < 18; ++i) {
          Turn move = Turn::AllTurns[i];

          if (!move.IsTurnBacktracking(curNode.move)) {
            RubicsCubeState cubeCopy(curNode.cube);

            cubeCopy.ApplyTurn(move);

            uint8_t estSuccMoves = curNode.depth + 1 + Solver::GetDistanceHeuristic(cubeCopy, bound - curNode.depth - 1);

            if (estSuccMoves <= bound) {
              // If the twisted cube is estimated to take fewer move than the
              // current bound, push it, otherwise it's pruned.
              successors.push({cubeCopy, move, estSuccMoves});
            } else if (estSuccMoves < *nextBound) {
              // The next bound is the minimum of all successor node moves that's
              // greater than the current bound.
              *nextBound = estSuccMoves;
            }
          }
        }

        while (!successors.empty()) {
          // Push the nodes in sorted order.
          nodeStack.push({
            successors.top().cube,
            successors.top().move,
            (uint8_t)(curNode.depth + 1)
          });

          successors.pop();
        }
      }
    }
}
