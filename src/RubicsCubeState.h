#pragma once

#include <string>
#include "RubicsCubePiece.h"
#include "Turn.h"

using namespace std;

class RubicsCubeState {
    private:
        RubicsCubePiece* edgePieces;
        RubicsCubePiece* cornerPieces;

    public:
        static RubicsCubeState* InitialState();
        RubicsCubeState (RubicsCubePiece* edgePieces, RubicsCubePiece* cornerPieces);
        string GetStateString();
        static RubicsCubeState* ParseStateString(string stateString);
        RubicsCubeState* Copy();
        RubicsCubePiece* GetEdgePieces();
        RubicsCubePiece* GetCornerPieces();
        void ApplyTurn(Turn turn);
};
