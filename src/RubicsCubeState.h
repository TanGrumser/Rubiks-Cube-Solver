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
        array<unsigned int, 8> cornerPermutaion;
        array<unsigned int, 8> cornerRotation;
        array<unsigned int, 12> edgePermutaion;
        array<unsigned int, 12> edgeRotation;

        static RubicsCubeState* initialState;

        unsigned int** edgeNeighbourIndicieRotations;
        static RubicsCubeState* InitialState();
        //RubicsCubeState(array<unsigned int, 8> cornerPermutation, array<unsigned int, 8> cornerRotation, array<unsigned int, 12> edgePermutation, array<unsigned int, 12> edgeRotation);
        ~RubicsCubeState();
        string GetStateString();
        static RubicsCubeState* ParseStateString(string stateString);
        RubicsCubeState* Copy();
        void ApplyTurn(Turn turn);
        bool Equals(RubicsCubeState* other);
        Turn GetTurnTo(RubicsCubeState* other);
        StateIndex GetLookupIndex();
        void scramble(int turns);
};
