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
using std::vector;



int Search(vector<RubicsCubeState*>* path, int depth, int bound, Turn lastTurn, int* minBound, vector<Turn> nextTurns, bool* isSolutionFound);
vector<Turn> GenerateTurnSequenceFromStateSequence(vector<RubicsCubeState*> stateSequence);
const int MAX_BOUND = 100000;
const int SOLUTION_FOUND = -1;
unsigned long long traversedStatesAtDepth = 0;
unsigned long long traversedStates = 0;
unsigned long long duplicateStates = 0;

RubicsCubeStateShift* shift;


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
    typedef priority_queue<PrioritizedMove, vector<PrioritizedMove>,
      greater<PrioritizedMove> > moveQueue_t;

    StopWatch             timer;
    stack<Node>           nodeStack;
    Node                  curNode;
    array<Turn, 50>       moves     = {Turn::Empty()};
    uint8_t               bound     = 0;
    uint8_t               nextBound = Solver::GetDistanceHeuristic(startState, 100);

      timer.StartTimer();

    while (!solved)
    {
      if (nodeStack.empty())
      {
        if (bound != 0)
        {

          cout << "IDA*: Finished bound " << (unsigned)bound
               << ".  Elapsed time: " << timer.GetFormattedTimeInSeconds() << ". " <<
               "traversed staes at this bound: " << traversedStatesAtDepth
               << endl;
        }

        traversedStatesAtDepth = 0;

        // Start with the scrambled (root) node.  Depth 0, no move required.
        nodeStack.push({startState, Turn::Empty(), 0});

        // If nextBound is initialized to 0 above but the cube is not solved,
        // the DB is bad.
        if (nextBound == 0)
          throw std::runtime_error("DB is bad");

        // If the next bound is not updated then all branches were pruned.
        // This also indicates a bad DB.
        if (nextBound == 0xFF)
          throw std::runtime_error("Move is 0xFF");

        bound     = nextBound;
        nextBound = 0xFF;
      }

      curNode = nodeStack.top();
      nodeStack.pop();
      ++traversedStatesAtDepth;

      // Keep the list of moves.  The moves end at 0xFF.
      moves.at(curNode.depth) = Turn::Empty();

      if (curNode.depth != 0)
        moves[curNode.depth - 1] = curNode.move;

      if (curNode.depth == bound)
      {
        if (curNode.cube.Equals(RubicsCubeState::InitialState()))
          solved = true;
      }
      else
      {
        // This is used to sort the successors by estimated moves.
        moveQueue_t successors;

        for (uint8_t i = 0; i < 18; ++i)
        {
          Turn move = Turn::AllTurns[i];

          if (!move.IsTurnBacktracking(curNode.move))
          {
            RubicsCubeState cubeCopy(curNode.cube);

            cubeCopy.ApplyTurn(move);

            uint8_t estSuccMoves = curNode.depth + 1 + Solver::GetDistanceHeuristic(cubeCopy, bound - curNode.depth - 1);

            if (estSuccMoves <= bound)
            {
              // If the twisted cube is estimated to take fewer move than the
              // current bound, push it, otherwise it's pruned.
              successors.push({cubeCopy, move, estSuccMoves});
            }
            else if (estSuccMoves < nextBound)
            {
              // The next bound is the minimum of all successor node moves that's
              // greater than the current bound.
              nextBound = estSuccMoves;
            }
          }
        }

        while (!successors.empty())
        {
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

    cout << "IDA*: Goal reached in " << timer.GetFormattedTimeInSeconds() << "s. "
         << "traversed staes at this bound: " << traversedStatesAtDepth
         << endl;

    // Convert the move to a vector.
    vector<Turn> moveVec;

    for (unsigned i = 0; i < moves.size() && !moves.at(i).Equals(Turn::Empty()); ++i)
      moveVec.push_back(moves.at(i));

    return moveVec;
}
