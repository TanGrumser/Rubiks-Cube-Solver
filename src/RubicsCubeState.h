#pragma once

#include <string>
#include "RubicsCubePiece.h"
#include "Turn.h"
#include "StateIndex.h"

using namespace std;

class RubicsCubeState {
    private:
    public:
        RubicsCubePiece* edgePieces;
        RubicsCubePiece* cornerPieces;
        static RubicsCubeState* initialState;

        int** edgeNeighbourIndicieRotations;
        static RubicsCubeState* InitialState();
        RubicsCubeState(RubicsCubePiece* edgePieces, RubicsCubePiece* cornerPieces);
        ~RubicsCubeState();
        string GetStateString();
        static RubicsCubeState* ParseStateString(string stateString);
        RubicsCubeState* Copy();
        void CopyInto(RubicsCubeState* state);
        RubicsCubePiece* GetEdgePieces();
        RubicsCubePiece* GetCornerPieces();
        void ApplyTurn(Turn turn);
        bool Equals(RubicsCubeState* other);
        Turn GetTurnTo(RubicsCubeState* other);
        bool ContainsNegativeNumber();
        StateIndex GetLookupIndex();
};
