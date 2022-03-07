#include "Solver.h"
#include "RubicsCubeState.h"


const int EDGE_CORNER_NEIGHBOR_INIDICIES[12][2] = {
    {0, 1},
    {1, 2},
    {2, 3},
    {3, 0},
    {4, 0},
    {5, 1},
    {6, 2},
    {7, 3},
    {5, 4},
    {6, 5},
    {7, 6},
    {4, 7},
};

const int EDGE_CORNER_ROTATION_OFFSET[12][4] = {
    {0, 0, 1, 2},
    {0, 0, 1, 2},
    {0, 0, 1, 2},
    {0, 0, 1, 2},
   
    {2, 1, 2, 1},
    {2, 1, 2, 1},
    {2, 1, 2, 1},
    {2, 1, 2, 1},
   
    {0, 0, 1, 2},
    {0, 0, 1, 2},
    {0, 0, 1, 2},
    {0, 0, 1, 2}
};

int GetNeighbourHeuristic(RubicsCubeState* from, RubicsCubeState* to);
int** GetEdgeNeighbourIndiciesRotations();

int Solver::GetDistanceHeuristic(RubicsCubeState* from, RubicsCubeState* to) {
    return GetNeighbourHeuristic(from, to);
}

int GetNeighbourHeuristic(RubicsCubeState* from, RubicsCubeState* to) {
    return 0;
}


int** GetEdgeNeighbourIndiciesRotations(RubicsCubeState* state) {
    if (state->edgeNeighbourIndicieRotations == nullptr) {
        int** edgeNeighbourIndicieRotations = (int**) malloc(sizeof(int) * 12 * 4);
        state->edgeNeighbourIndicieRotations = edgeNeighbourIndicieRotations;

        for (int i = 0; i < 12; i++) {
            int leftCornerNeighbourIndex = EDGE_CORNER_NEIGHBOR_INIDICIES[i][state->edgePieces[i].rotation == 0 ? 0 : 1];
            int rightCornerNeighbourIndex = EDGE_CORNER_NEIGHBOR_INIDICIES[i][state->edgePieces[i].rotation == 0 ? 1 : 0];
            int leftCornerRotationOffset = EDGE_CORNER_ROTATION_OFFSET[i][0 + (state->edgePieces[i].rotation == 0 ? 0 : 2)];
            int rightCornerRotationOffset = EDGE_CORNER_ROTATION_OFFSET[i][1 + (state->edgePieces[i].rotation == 0 ? 0 : 2)];

            edgeNeighbourIndicieRotations[state->edgePieces[i].index] = new int[4] {
                state->cornerPieces[leftCornerNeighbourIndex].index,
                state->cornerPieces[rightCornerNeighbourIndex].index,
                (state->cornerPieces[leftCornerNeighbourIndex].rotation + leftCornerRotationOffset) % 3,
                (state->cornerPieces[rightCornerNeighbourIndex].rotation + rightCornerRotationOffset) % 3
            };
        }
    }
}


int GetDistanceHeuristic(RubicsCubeState* other) {
    int progress = 0;
    int** otherEdgeNeighbourIndiciesRotation = other->edgeNeighbourIndicieRotations;
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
