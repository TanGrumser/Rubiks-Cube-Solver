#pragma once

#include <string>
#include "RubicsCubePiece.h"
#include "Turn.h"
#include "StateIndex.h"
#include <array>
#include "../Utils/Constants.h"

using namespace std;

class RubicsCubeState {
    private:

    public:
        array<RubicsCubePiece, 8> corners;
        array<RubicsCubePiece, 12> edges;

        static RubicsCubeState initialState;
        static RubicsCubeState& InitialState();
        
        string GetStateString() const;
        static RubicsCubeState& ParseStateString(string stateString);
        RubicsCubeState& Copy() const;
        void ApplyTurn(Turn turn);
        bool Equals(RubicsCubeState& other);
        Turn GetTurnTo(RubicsCubeState& other);
        StateIndex GetLookupIndex();
        void scramble(int turns);
};