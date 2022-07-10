#include "RubiksCubeState.h"
#include "Turn.h"
#include "../Utils/StringUtils.h"
#include "../LookupTable/LookupTable.h"
#include <iostream>


bool initialized = false;

RubiksCubeState RubiksCubeState::initialState = RubiksCubeState();

RubiksCubeState& RubiksCubeState::InitialState() {
    if (!initialized) {
        for (int i = 0; i < 8; i++) {
            RubiksCubeState::initialState.corners[i].index = i;
            RubiksCubeState::initialState.corners[i].rotation = 0;
        }
        
        for (int i = 0; i < 12; i++) {
            RubiksCubeState::initialState.edges[i].index = i;
            RubiksCubeState::initialState.edges[i].rotation = 0;
        }

        initialized = true;
    }

    return RubiksCubeState::initialState;
}

RubiksCubeState& RubiksCubeState::Copy() const {
    RubiksCubeState* copy = new RubiksCubeState();

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

string RubiksCubeState::GetStateString() const {
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

RubiksCubeState& RubiksCubeState::ParseStateString(string stateString) {
    RubiksCubeState* state = new RubiksCubeState();

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

Turn RubiksCubeState::GetTurnTo(RubiksCubeState& other) {

    for (int i = 0; i < Turn::CountAllTurns; i++) {
        RubiksCubeState& state = Copy();
        state.ApplyTurn(Turn::AllTurns[i]);

        if (state.Equals(other)) {
            return Turn::AllTurns[i];
        }
    }

    throw std::runtime_error("Couldn't find turn");
}

bool RubiksCubeState::Equals(RubiksCubeState& other) {
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

StateIndex RubiksCubeState::GetLookupIndex() {
    StateIndex index;

    index.cornerIndex = LookupTable::GetCornerLookupIndex(*this);
    index.edgeIndex = LookupTable::GetEdgeLookupIndex(*this);

    return index;
}

void RubiksCubeState::scramble(int turns) {
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