#include "RubicsCubeState.h"
#include "StringUtils.h"
#include "Turn.h"
#include <iostream>

RubicsCubeState::RubicsCubeState(RubicsCubePiece* edgePieces, RubicsCubePiece* cornerPieces) {
    this->edgePieces = edgePieces;
    this->cornerPieces = cornerPieces;
}

RubicsCubeState* RubicsCubeState::initialState = nullptr;

RubicsCubeState* RubicsCubeState::InitialState() {
    if (RubicsCubeState::initialState == nullptr) {
        RubicsCubePiece* edgePieces = new RubicsCubePiece[12] {
            RubicsCubePiece(0, 0),
            RubicsCubePiece(1, 0),
            RubicsCubePiece(2, 0),
            RubicsCubePiece(3, 0),
            RubicsCubePiece(4, 0),
            RubicsCubePiece(5, 0),
            RubicsCubePiece(6, 0),
            RubicsCubePiece(7, 0),
            RubicsCubePiece(8, 0),
            RubicsCubePiece(9, 0),
            RubicsCubePiece(10, 0),
            RubicsCubePiece(11, 0)
        };

        RubicsCubePiece* cornerPieces = new RubicsCubePiece [8] {
            RubicsCubePiece(0, 0),
            RubicsCubePiece(1, 0),
            RubicsCubePiece(2, 0),
            RubicsCubePiece(3, 0),
            RubicsCubePiece(4, 0),
            RubicsCubePiece(5, 0),
            RubicsCubePiece(6, 0),
            RubicsCubePiece(7, 0)
        };

        RubicsCubeState::initialState = new RubicsCubeState(edgePieces, cornerPieces);
    }

    return RubicsCubeState::initialState;
}

RubicsCubeState* RubicsCubeState::Copy() {
    RubicsCubePiece* edgePieces   = (RubicsCubePiece*) malloc(sizeof(RubicsCubePiece) * 12);
    RubicsCubePiece* cornerPieces = (RubicsCubePiece*) malloc(sizeof(RubicsCubePiece) * 8);

    for (int i = 0; i < 12; i++) {
        edgePieces[i] = this->edgePieces[i].Copy();
    }

    for (int i = 0; i < 8; i++) {
        cornerPieces[i] = this->cornerPieces[i].Copy();
    }

    return new RubicsCubeState(edgePieces, cornerPieces);
}

string RubicsCubeState::GetStateString() {
    string result = "";

    for (int i = 0; i < 12; i++) {
        result += edgePieces[i].ToString();

        if (i < 12 - 1) {
            result +=  ";";
        }
    }

    result += "|";

    for (int i = 0; i < 8; i++) {
        result += cornerPieces[i].ToString();

        if (i < 8 - 1) {
            result += ";";
        }
    }

    return result;
}

RubicsCubeState* RubicsCubeState::ParseStateString(string stateString) {
    RubicsCubePiece* edgePieces   = (RubicsCubePiece*) malloc(sizeof(RubicsCubePiece) * 12);
    RubicsCubePiece* cornerPieces = (RubicsCubePiece*) malloc(sizeof(RubicsCubePiece) * 8);

    vector<string> lines = StringUtils::Split(stateString, "|");
    vector<string> edgeValues = StringUtils::Split(lines[0], ";");
    vector<string> cornerValues = StringUtils::Split(lines[1], ";");

    for (int i = 0; i < edgeValues.size(); i++) {
        vector<string> values = StringUtils::Split(edgeValues[i], ",");
        
        edgePieces[i].index = stoi(values[0]);
        edgePieces[i].rotation = stoi(values[1]);
    }

    for (int i = 0; i < cornerValues.size(); i++) {
        vector<string> values = StringUtils::Split(cornerValues[i], ",");
        
        cornerPieces[i].index = stoi(values[0]);
        cornerPieces[i].rotation = stoi(values[1]);
    }
    
    return new RubicsCubeState(edgePieces, cornerPieces);
}

RubicsCubePiece* RubicsCubeState::GetEdgePieces() {
    return this->edgePieces;
}

RubicsCubePiece* RubicsCubeState::GetCornerPieces() {
    return this->cornerPieces;
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

    return Turn::Empty();
}

bool RubicsCubeState::Equals(RubicsCubeState* other) {
    for (int i = 0; i < 12; i++) {
        if (edgePieces[i].index != other->edgePieces[i].index
         || edgePieces[i].rotation != other->edgePieces[i].rotation) {
             return false;
         }
    }

    for (int i = 0; i < 8; i++) {
        if (cornerPieces[i].index != other->cornerPieces[i].index
         || cornerPieces[i].rotation != other->cornerPieces[i].rotation) {
             return false;
         }
    }

    return true;
}

bool RubicsCubeState::ContainsNegativeNumber() {
    for (int i = 0; i < 12; i++) {
        if (edgePieces[i].index < 0
         || edgePieces[i].rotation < 0) {
             return true;
         }
    }

    for (int i = 0; i < 8; i++) {
        if (cornerPieces[i].index < 0
         || cornerPieces[i].rotation < 0) {
             return true;
         }
    }

    return false;

}