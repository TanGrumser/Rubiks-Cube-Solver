#include "Solver.h"
#include "RubicsCubeState.h"

int GetNeighbourHeuristic(RubicsCubeState* from, RubicsCubeState* to);
int** GetEdgeNeighbourIndiciesRotations();

int Solver::GetDistanceHeuristic(RubicsCubeState* from, RubicsCubeState* to) {
    GetNeighbourHeuristic(from, to);
}

int GetNeighbourHeuristic(RubicsCubeState* from, RubicsCubeState* to) {
    return 0;
}

int** GetEdgeNeighbourIndiciesRotations(RubicsCubeState* state) {
    if (state->edgeNeighbourIndicieRotations == nullptr) {
        int* edgeNeighbourIndicieRotations[4] = (int*) malloc(sizeof(int) * 12 * 4);
        state->edgeNeighbourIndicieRotations = edgeNeighbourIndicieRotations;

        for (int i = 0; i < 12; i++) {
            byte leftCornerNeighbourIndex = EDGE_CORNER_NEIGHBOR_INIDICIES[i][edgePieces[i].rotation == 0 ? 0 : 1];
            byte rightCornerNeighbourIndex = EDGE_CORNER_NEIGHBOR_INIDICIES[i][edgePieces[i].rotation == 0 ? 1 : 0];
            byte leftCornerRotationOffset = EDGE_CORNER_ROTATION_OFFSET[i][0 + (edgePieces[i].rotation == 0 ? 0 : 2)];
            byte rightCornerRotationOffset = EDGE_CORNER_ROTATION_OFFSET[i][1 + (edgePieces[i].rotation == 0 ? 0 : 2)];

            edgeNeighbourIndicieRotations[edgePieces[i].index] = new byte[] {
                cornerPieces[leftCornerNeighbourIndex].index,
                cornerPieces[rightCornerNeighbourIndex].index,
                (byte)((cornerPieces[leftCornerNeighbourIndex].rotation + leftCornerRotationOffset) % 3),
                (byte)((cornerPieces[rightCornerNeighbourIndex].rotation + rightCornerRotationOffset) % 3)
            };
        }
    }

    return edgeNeighbourIndicieRotations;
}


public int GetDistanceHeuristic(RubicsCubeState2 other) {
    int progress = 0;
    byte[][] otherEdgeNeighbourIndiciesRotation = other.GetEdgeNeighbourIndiciesRotations();
    int leftNeightbourIndex = -1;
    int rightNeightbourIndex = -1;
    int leftNeightbourRotation= -1;
    int rightNeightbourRotation = -1;

    for (int i = 0 ; i < 12; i++) {
        byte[] otherNeighboursPositionRotations = otherEdgeNeighbourIndiciesRotation[edgePieces[i].index];
        leftNeightbourIndex = cornerPieces[EDGE_CORNER_NEIGHBOR_INIDICIES[i][edgePieces[i].rotation == 0 ? 0 : 1]].index;
        rightNeightbourIndex = cornerPieces[EDGE_CORNER_NEIGHBOR_INIDICIES[i][edgePieces[i].rotation == 0 ? 1 : 0]].index;

        leftNeightbourRotation = (byte)((cornerPieces[EDGE_CORNER_NEIGHBOR_INIDICIES[i][edgePieces[i].rotation == 0 ? 0 : 1]].rotation
            + EDGE_CORNER_ROTATION_OFFSET[i][0 + (edgePieces[i].rotation == 0 ? 0 : 2)]) % 3);
        rightNeightbourRotation = (byte)((cornerPieces[EDGE_CORNER_NEIGHBOR_INIDICIES[i][edgePieces[i].rotation == 0 ? 1 : 0]].rotation
            + EDGE_CORNER_ROTATION_OFFSET[i][1 + (edgePieces[i].rotation == 0 ? 0 : 2)]) % 3);
        
        
        if (leftNeightbourIndex == otherNeighboursPositionRotations[0] // are edges correct
            && leftNeightbourRotation == otherNeighboursPositionRotations[2] // are they correclty rotated
        ) {

            progress++;
        }

        
        if (rightNeightbourIndex == otherNeighboursPositionRotations[1]
            && rightNeightbourRotation == otherNeighboursPositionRotations[3]
        ) {
            progress++;
        }
    }

    return progress;
}