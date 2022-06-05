#include "RubicsCubeState.h"
#include "StringUtils.h"
#include "Turn.h"
#include <iostream>
#include "LookupTable.h"


/*
RubicsCubeState::RubicsCubeState(array<unsigned int, 8> cornerPermutation, array<unsigned int, 8> cornerRotation, array<unsigned int, 12> edgePermutation, array<unsigned int, 12> edgeRotation) {
    this->cornerPermutaion = cornerPermutaion;
    this->cornerRotation = cornerRotation;
    this->edgePermutaion = edgePermutaion;
    this->edgeRotation = edgeRotation;


    this->edgeNeighbourIndicieRotations = nullptr;
}
*/


RubicsCubeState::~RubicsCubeState() {
    
    // deleting a null pointer seems to be safe (https://stackoverflow.com/questions/6731331/is-it-still-safe-to-delete-nullptr-in-c0x)
    delete[] edgeNeighbourIndicieRotations;
}


RubicsCubeState* RubicsCubeState::initialState = nullptr;

RubicsCubeState* RubicsCubeState::InitialState() {
    if (RubicsCubeState::initialState == nullptr) {
        RubicsCubeState::initialState = new RubicsCubeState();

        for (int i = 0; i < 8; i++) {
            RubicsCubeState::initialState->cornerPermutaion[i] = i;
            RubicsCubeState::initialState->cornerRotation[i] = 0;
        }
        
        for (int i = 0; i < 12; i++) {
            RubicsCubeState::initialState->edgePermutaion[i] = i;
            RubicsCubeState::initialState->edgeRotation[i] = 0;
        }
    }

    return RubicsCubeState::initialState;
}

RubicsCubeState* RubicsCubeState::Copy() {
    RubicsCubeState* copy = new RubicsCubeState();

    for (int i = 0; i < 8; i++) {
        copy->cornerPermutaion[i] = this->cornerPermutaion[i];
        copy->cornerRotation[i] = this->cornerRotation[i];
    }

    for (int i = 0; i < 12; i++) {
        copy->edgePermutaion[i] = this->edgePermutaion[i];
        copy->edgeRotation[i] = this->edgeRotation[i];
    }

    return copy;
}

string RubicsCubeState::GetStateString() {
    string result = "";

    for (int i = 0; i < 12; i++) {
        result += std::to_string(edgePermutaion[i]) + "," + std::to_string(edgeRotation[i]);

        if (i < 12 - 1) {
            result +=  ";";
        }
    }

    result += "|";

    for (int i = 0; i < 8; i++) {
        result += std::to_string(cornerPermutaion[i]) + "," + std::to_string(cornerRotation[i]);

        if (i < 8 - 1) {
            result += ";";
        }
    }

    return result;
}

RubicsCubeState* RubicsCubeState::ParseStateString(string stateString) {
    RubicsCubeState* state = new RubicsCubeState();

    vector<string> lines = StringUtils::Split(stateString, "|");
    vector<string> edgeValues = StringUtils::Split(lines[0], ";");
    vector<string> cornerValues = StringUtils::Split(lines[1], ";");

    for (int i = 0; i < edgeValues.size(); i++) {
        vector<string> values = StringUtils::Split(edgeValues[i], ",");
        
        state->edgePermutaion[i] = stoi(values[0]);
        state->edgeRotation[i] = stoi(values[1]);
    }

    for (int i = 0; i < cornerValues.size(); i++) {
        vector<string> values = StringUtils::Split(cornerValues[i], ",");
        
        state->cornerPermutaion[i] = stoi(values[0]);
        state->cornerRotation[i] = stoi(values[1]);
    }
    
    return state;
}

Turn RubicsCubeState::GetTurnTo(RubicsCubeState* other) {
    RubicsCubeState* state;

    for (int i = 0; i < Turn::CountAllTurns; i++) {
        state = Copy();
        state->ApplyTurn(Turn::AllTurns[i]);

        if (state->Equals(other)) {
            return Turn::AllTurns[i];
        }
    }

    throw std::runtime_error("Couldn't find turn");
}

bool RubicsCubeState::Equals(RubicsCubeState* other) {
    for (int i = 0; i < 12; i++) {
        if (edgePermutaion[i] != other->edgePermutaion[i]
         || edgeRotation[i] != other->edgeRotation[i]) {
             return false;
         }
    }

    for (int i = 0; i < 8; i++) {
        if (cornerPermutaion[i] != other->cornerPermutaion[i]
         || cornerRotation[i] != other->cornerRotation[i]) {
             return false;
         }
    }

    return true;
}

StateIndex RubicsCubeState::GetLookupIndex() {
    StateIndex index;

    index.cornerIndex = LookupTable::GetCornerLookupIndex(this);
    index.edgeIndex = LookupTable::GetFullEdgeLookupIndex(this);

    return index;
}

void RubicsCubeState::scramble(int turns) {
    Turn turn, lastTurn = Turn::Empty();
        
    for (int j = 0; j < turns; j++) {
        turn = Turn::Random();

        while (turn.IsTurnBacktracking(lastTurn)) {
            turn = Turn::Random();
        }

        lastTurn = turn;
        
        this->ApplyTurn(turn);
    }
}