// This is a parrallel recursive Implementation of the iterative deepening A* algorithm for solving the rubiks cube.
// It is parralelized by spliting the different paths going off from the initial state into different searches that are assigned to individual processes.
// This method has the restrictio, that it currently requires a number of threads that is a valid divisor of 18 (Number of adjacent nodes for the start state)

#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <stdexcept>
#include <atomic>
#include <algorithm>
#include <thread>

#include "Solver.h"
#include "../Model/RubiksCubeState.h"
#include "../Model/Turn.h"
#include "../DuplicateStateDetection/DuplicateState.h"
#include "../Model/RubiksCubeStateShift.h"
#include "../Utils/Stopwatch.h"

using std::vector;



int Search(vector<RubiksCubeState*>* path, int depth, int bound, Turn lastTurn, int* minBound, vector<Turn> nextTurns, bool* isSolutionFound);
vector<Turn> GenerateTurnSequenceFromStateSequence(vector<RubiksCubeState*> stateSequence);
unsigned long long traversedStates = 0;
std::atomic_int32_t traversedStatesAtDepth;

RubiksCubeStateShift* shift;

void idaSearch(RubiksCubeState& startState, int bound, array<Turn, 50>* solution, int* nextBound, std::vector<Turn> turnsToExplore, atomic_bool* solved);

struct Node {
    RubiksCubeState cube;
    Turn move;
    uint8_t depth;
};

struct PrioritizedMove {
    RubiksCubeState cube;
    Turn move;
    uint8_t estMoves; // Priority.  Least number of moves to most.
    bool operator>(const PrioritizedMove& rhs) const {
        return this->estMoves > rhs.estMoves;
    }
};

vector<Turn> Solver::PR_IterativeDeepeningAStar(RubiksCubeState& startState, Logger* logger) {
    atomic_bool* solved = new atomic_bool(startState.Equals(RubiksCubeState::InitialState()));
    uint8_t bound = Solver::GetDistanceHeuristic(startState, 100);
    vector<array<Turn, 50>*> moves(Solver::threadCount, new array<Turn, 50>);
    array<Turn, 50> solutionMoves;
    StopWatch timer;

    shift = new RubiksCubeStateShift(startState);

    timer.StartTimer();

    while (!*solved) {
        std::vector<std::thread> threads(Solver::threadCount);
        traversedStatesAtDepth = 0;
        int* newBounds = new int[Solver::threadCount];
        int newBound = 0xFF;

        for (int i = 0; i < Solver::threadCount; i++) {
          moves[i] = new array<Turn, 50>();
          newBounds[i] = 0xff;
          RubiksCubeState state(startState);
          std::vector<Turn> exploredTurns = Turn::GetSubsetTurns(i, Solver::threadCount);
          array<Turn, 50>* movesStack = moves[i];

          std::thread thread = std::thread([&state, bound, movesStack, newBounds, i, exploredTurns, solved]() {
            idaSearch(state, bound, movesStack, newBounds + i, exploredTurns, solved);
          });

          threads[i] = std::move(thread);
        }

        for (int i = 0; i < Solver::threadCount; i++) {
            threads[i].join();

            if (newBounds[i] == 0xFF) {
              solutionMoves = *moves[i];
              *solved = true;
            } else if (newBounds[i] < newBound) {
              newBound = newBounds[i];
            }
        }

        logger->logNewLine( 
            "IDA* Finished bound " +  to_string(bound) + ". " + 
            "Elapsed time: " + timer.GetFormattedTimeInSeconds() + ". " + 
            "Traversed states at this bound: " + to_string(traversedStatesAtDepth)
        );

        bound = newBound;
    }

    vector<Turn> moveVec;

    for (unsigned i = 0; i < solutionMoves.size() && !solutionMoves.at(i).Equals(Turn::Empty()); ++i)
      moveVec.push_back(solutionMoves.at(i));

    return moveVec;
}

void idaSearch(RubiksCubeState& startState, int bound, array<Turn, 50>* moves, int* nextBound, std::vector<Turn> turnsToExplore, atomic_bool* solved) {
    typedef priority_queue<PrioritizedMove, vector<PrioritizedMove>,
      greater<PrioritizedMove> > moveQueue_t;

    stack<Node>           nodeStack;
    Node                  curNode;
    
    (*moves)[0] = Turn::Empty();
    //nodeStack.push({startState, Turn::Empty(), 1});

    // Fill the node stack with only the states, that are reachable through the turns that this thread is supposed to explore
    for (int i = 0; i < turnsToExplore.size(); i++) {
      RubiksCubeState cubeCopy(startState);
      cubeCopy.ApplyTurn(turnsToExplore[i]);
      nodeStack.push({cubeCopy, turnsToExplore[i], 1});
    }

    while (!(*solved) && !nodeStack.empty()) {
      traversedStatesAtDepth++;
      curNode = nodeStack.top();
      nodeStack.pop();

      // Keep the list of moves.
      moves->at(curNode.depth) = Turn::Empty();

      if (curNode.depth != 0)
        (*moves)[curNode.depth - 1] = curNode.move;

      if (curNode.depth == bound && curNode.cube.Equals(RubiksCubeState::InitialState())) {
          *nextBound = 0xFF;
          *solved = true;
          break;
      } else {
        // This is used to sort the successors by estimated moves.
        moveQueue_t successors;

        for (uint8_t i = 0; i < 18; ++i) {
          Turn move = Turn::AllTurns[i];

          if (!move.IsTurnBacktracking(curNode.move)) {
            RubiksCubeState cubeCopy(curNode.cube);

            cubeCopy.ApplyTurn(move);

            // if this state was reached via another path, we don't need to traverse it any further
            if (DuplicateState::active && DuplicateState::PruneByTurnIndex(*moves, curNode.depth + 1)) {
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
