#include "RubicsCubeState.h"
#include "StringUtils.h"
#include <iostream>

RubicsCubeState::RubicsCubeState(RubicsCubePiece* edgePieces, RubicsCubePiece* cornerPieces) {
    this->edgePieces = edgePieces;
    this->cornerPieces = cornerPieces;
}

RubicsCubeState* RubicsCubeState::InitialState() {
    //RubicsCubePiece* edgePieces = (RubicsCubePiece*) malloc(sizeof(RubicsCubePiece) * 12);
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

    return new RubicsCubeState(edgePieces, cornerPieces);
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
