#pragma once

#include <string>
#include "RubicsCubePiece.h"
#include "Turn.h"
#include "StateIndex.h"
#include <array>

using namespace std;

class RubicsCubeState {
    private:

    public:
        array<RubicsCubePiece, 8> corners;
        array<RubicsCubePiece, 12> edges;

        static RubicsCubeState* initialState;

        static RubicsCubeState* InitialState();
        
        string GetStateString();
        static RubicsCubeState* ParseStateString(string stateString);
        RubicsCubeState* Copy();
        void ApplyTurn(Turn turn);
        bool Equals(RubicsCubeState* other);
        Turn GetTurnTo(RubicsCubeState* other);
        StateIndex GetLookupIndex();
        void scramble(int turns);
};
