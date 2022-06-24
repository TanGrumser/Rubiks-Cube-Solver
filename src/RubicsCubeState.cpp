#include "RubicsCubeState.h"
#include "StringUtils.h"
#include "Turn.h"
#include <iostream>
#include "LookupTable.h"


bool initialized = false;

RubicsCubeState RubicsCubeState::initialState = RubicsCubeState();

RubicsCubeState& RubicsCubeState::InitialState() {
    if (!initialized) {
        for (int i = 0; i < 8; i++) {
            RubicsCubeState::initialState.corners[i].index = i;
            RubicsCubeState::initialState.corners[i].rotation = 0;
        }
        
        for (int i = 0; i < 12; i++) {
            RubicsCubeState::initialState.edges[i].index = i;
            RubicsCubeState::initialState.edges[i].rotation = 0;
        }

        initialized = true;
    }

    return RubicsCubeState::initialState;
}

RubicsCubeState& RubicsCubeState::Copy() {
    RubicsCubeState* copy = new RubicsCubeState();

    for (int i = 0; i < 8; i++) {
        copy->corners[i].index = this->corners[i].index;
        copy->corners[i].rotation = this->corners[i].rotation;
    }

    for (int i = 0; i < 12; i++) {
        copy->edges[i].index = this->edges[i].index;
        copy->edges[i].rotation = this->edges[i].rotation;
    }

    return *copy;
}

string RubicsCubeState::GetStateString() {
    string result = "";

    for (int i = 0; i < 12; i++) {
        result += std::to_string(edges[i].index) + "," + std::to_string(edges[i].rotation);

        if (i < 12 - 1) {
            result +=  ";";
        }
    }

    result += "|";

    for (int i = 0; i < 8; i++) {
        result += std::to_string(corners[i].index) + "," + std::to_string(corners[i].rotation);

        if (i < 8 - 1) {
            result += ";";
        }
    }

    return result;
}

RubicsCubeState& RubicsCubeState::ParseStateString(string stateString) {
    RubicsCubeState* state = new RubicsCubeState();

    vector<string> lines = StringUtils::Split(stateString, "|");
    vector<string> edgeValues = StringUtils::Split(lines[0], ";");
    vector<string> cornerValues = StringUtils::Split(lines[1], ";");

    for (int i = 0; i < edgeValues.size(); i++) {
        vector<string> values = StringUtils::Split(edgeValues[i], ",");
        
        state->edges[i].index = stoi(values[0]);
        state->edges[i].rotation = stoi(values[1]);
    }

    for (int i = 0; i < cornerValues.size(); i++) {
        vector<string> values = StringUtils::Split(cornerValues[i], ",");
        
        state->corners[i].index = stoi(values[0]);
        state->corners[i].rotation = stoi(values[1]);
    }
    
    return *state;
}

Turn RubicsCubeState::GetTurnTo(RubicsCubeState& other) {

    for (int i = 0; i < Turn::CountAllTurns; i++) {
        RubicsCubeState& state = Copy();
        state.ApplyTurn(Turn::AllTurns[i]);

        if (state.Equals(other)) {
            return Turn::AllTurns[i];
        }
    }

    throw std::runtime_error("Couldn't find turn");
}

bool RubicsCubeState::Equals(RubicsCubeState& other) {
    for (int i = 0; i < 12; i++) {
        if (edges[i].index != other.edges[i].index
         || edges[i].rotation != other.edges[i].rotation) {
             return false;
         }
    }

    for (int i = 0; i < 8; i++) {
        if (corners[i].index != other.corners[i].index
         || corners[i].rotation != other.corners[i].rotation) {
             return false;
         }
    }

    return true;
}

StateIndex RubicsCubeState::GetLookupIndex() {
    StateIndex index;

    index.cornerIndex = LookupTable::GetCornerLookupIndex(*this);
    index.edgeIndex = LookupTable::GetFullEdgeLookupIndex(*this);

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