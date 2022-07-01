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

void idaSearch(RubicsCubeState& startState, int bound, array<Turn, 50>* solution, int* nextBound, std::vector<Turn> turnsToExplore, atomic_bool* solved);

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
    atomic_bool* solved = new atomic_bool(startState.Equals(RubicsCubeState::InitialState()));
    uint8_t bound = Solver::GetDistanceHeuristic(startState, 100);
    vector<array<Turn, 50>*> moves(Solver::threadCount, new array<Turn, 50>);
    array<Turn, 50> solutionMoves;
    StopWatch timer;

    shift = new RubicsCubeStateShift(startState);

    timer.StartTimer();

    while (!*solved) {
        std::vector<std::thread> threads(Solver::threadCount);
        traversedStatesAtDepth = 0;
        int* newBounds = new int[Solver::threadCount];
        int newBound = 0xFF;

        for (int i = 0; i < Solver::threadCount; i++) {
          moves[i] = new array<Turn, 50>();
          newBounds[i] = 0xff;
          RubicsCubeState state(startState);
          std::vector<Turn> exploredTurns = Turn::GetSubsetTurns(i, LookupTable::threadCount);
          array<Turn, 50>* movesStack = moves[i];

          std::thread thread = std::thread([&state, bound, movesStack, newBounds, i, exploredTurns, solved]() {
            idaSearch(state, bound, movesStack, newBounds + i, exploredTurns, solved);
          });

          threads[i] = std::move(thread);
        }

        for (int i = 0; i < LookupTable::threadCount; i++) {
            threads[i].join();

            if (newBounds[i] == 0xFF) {
              solutionMoves = *moves[i];
              *solved = true;
            } else if (newBounds[i] < newBound) {
              newBound = newBounds[i];
            }
        }

        bound = newBound;

        cout << "IDA*: Finished bound " << (unsigned)bound
              << ".  Elapsed time: " << timer.GetFormattedTimeInSeconds() << ". " <<
              "traversed staes at this bound: " << traversedStatesAtDepth
              << endl;
    }

    vector<Turn> moveVec;

    for (unsigned i = 0; i < solutionMoves.size() && !solutionMoves.at(i).Equals(Turn::Empty()); ++i)
      moveVec.push_back(solutionMoves.at(i));

    return moveVec;
}

void idaSearch(RubicsCubeState& startState, int bound, array<Turn, 50>* moves, int* nextBound, std::vector<Turn> turnsToExplore, atomic_bool* solved) {
    typedef priority_queue<PrioritizedMove, vector<PrioritizedMove>,
      greater<PrioritizedMove> > moveQueue_t;

    stack<Node>           nodeStack;
    Node                  curNode;
    
    (*moves)[0] = Turn::Empty();
    //nodeStack.push({startState, Turn::Empty(), 1});

    // Fill the node stack with only the states, that are reachable through the turns that this thread is supposed to explore
    for (int i = 0; i < turnsToExplore.size(); i++) {
      RubicsCubeState cubeCopy(startState);
      cubeCopy.ApplyTurn(turnsToExplore[i]);
      nodeStack.push({cubeCopy, turnsToExplore[i], 1});
    }

    while (!(*solved) && !nodeStack.empty()) {
      traversedStatesAtDepth++;
      curNode = nodeStack.top();
      nodeStack.pop();

      // Keep the list of moves.  The moves end at 0xFF.
      moves->at(curNode.depth) = Turn::Empty();

      if (curNode.depth != 0)
        (*moves)[curNode.depth - 1] = curNode.move;

      if (curNode.depth == bound && curNode.cube.Equals(RubicsCubeState::InitialState())) {
          *nextBound = 0xFF;
          *solved = true;
          break;
      } else {
        // This is used to sort the successors by estimated moves.
        moveQueue_t successors;

        for (uint8_t i = 0; i < 18; ++i) {
          Turn move = Turn::AllTurns[i];

          if (!move.IsTurnBacktracking(curNode.move)) {
            RubicsCubeState cubeCopy(curNode.cube);

            cubeCopy.ApplyTurn(move);

            // if this state was reached via another path, we don't need to traverse it any further
            if (DuplicateState::active && curNode.depth <= 6 && DuplicateState::WasStateReached(shift->GetShiftedState(cubeCopy), curNode.depth + 1)) {
              continue;
            }

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
