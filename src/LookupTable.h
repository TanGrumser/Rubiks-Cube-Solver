#pragma once

#include <string>
#include "RubicsCubeState.h"

using std::string;

namespace LookupTable {
    const string CORNER_LOOKUP_TABLE_PATH = "C:\\Users\\tangr\\Desktop\\Uni\\Bachelorarbeit\\Rubiks-Cube-Solver\\cornerLookupTable";

    const int CORNER_ROTATIONS_COUNT = 3 * 3 * 3 * 3 * 3 * 3 * 3; // 3^7
    const int CORNER_PERMUTATIONS_COUNT = 8 * 7 * 6 * 5 * 4 * 3 * 2 * 1; // 8!
    const int CORNER_STATES_COUNT = CORNER_ROTATIONS_COUNT * CORNER_PERMUTATIONS_COUNT; // 8! * 3^7

    const int EDGE_ROTATION_STATES_COUNT = 2 * 2 * 2 * 2 * 2 * 2 * 2; // 2^7
    const int EDGE_PERMUTATIONS_COUNT = 12 * 11 * 10 * 9 * 8 * 7 * 6; // 12! / 5!


    void LoadLookupTables();
    void GenerateCornerLookupTable(string path, int maxDepth);
    int GetCornerStateDistance(RubicsCubeState* state);
    int GetEdgeStateDistance(RubicsCubeState* state);
    int GetCornerLookupIndex(RubicsCubeState* state);
    int GetEdgeLookupIndex(RubicsCubeState* state);
    void TestCornerLookupCaluclation();
    void SetStateToCornerStateByIndex(int index, RubicsCubeState* state);
}