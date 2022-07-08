#pragma once

#include <string>
#include "RubiksCubePiece.h"
#include "Turn.h"
#include "StateIndex.h"
#include <array>
#include "../Utils/Constants.h"

using namespace std;

class RubiksCubeState {
    private:

    public:
        array<RubiksCubePiece, 8> corners;
        array<RubiksCubePiece, 12> edges;

        static RubiksCubeState initialState;
        static RubiksCubeState& InitialState();
        
        string GetStateString() const;
        static RubiksCubeState& ParseStateString(string stateString);
        RubiksCubeState& Copy() const;
        void ApplyTurn(Turn turn);
        bool Equals(RubiksCubeState& other);
        Turn GetTurnTo(RubiksCubeState& other);
        StateIndex GetLookupIndex();
        void scramble(int turns);
};
