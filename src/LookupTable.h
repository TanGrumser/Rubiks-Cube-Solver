#pragma once

#include <string>
#include "RubicsCubeState.h"

using std::string;

namespace LookupTable {
    typedef int(*IndexCalculation)(RubicsCubeState*);
    const string CORNER_LOOKUP_TABLE_PATH = "C:\\Users\\tangr\\Desktop\\Uni\\Bachelorarbeit\\Rubiks-Cube-Solver\\cornerLookupTable";
    const string UPPER_EDGE_LOOKUP_TABLE_PATH = "C:\\Users\\tangr\\Desktop\\Uni\\Bachelorarbeit\\Rubiks-Cube-Solver\\upperEdgeLookupTable";
    const string LOWER_EDGE_LOOKUP_TABLE_PATH = "C:\\Users\\tangr\\Desktop\\Uni\\Bachelorarbeit\\Rubiks-Cube-Solver\\lowerEdgeLookupTable";

    const int CORNER_ROTATIONS_COUNT = 3 * 3 * 3 * 3 * 3 * 3 * 3; // 3^7
    const int CORNER_PERMUTATIONS_COUNT = 8 * 7 * 6 * 5 * 4 * 3 * 2 * 1; // 8!
    const int CORNER_STATES_COUNT = CORNER_ROTATIONS_COUNT * CORNER_PERMUTATIONS_COUNT; // 8! * 3^7

    const int EDGE_ROTATION_COUNT = 2 * 2 * 2 * 2 * 2 * 2; // 2^6
    const int EDGE_PERMUTATIONS_COUNT = 12 * 11 * 10 * 9 * 8 * 7; // 12! / 6!
    const int EDGE_STATES_COUNT = EDGE_ROTATION_COUNT * EDGE_PERMUTATIONS_COUNT; // (12! / 6!) * 2^6

    void GenerateLookupTable(string path, IndexCalculation IndexCalculator, int maxReachableStates);
    void LoadLookupTables();
    
    int GetCornerStateDistance(RubicsCubeState* state);
    int GetUpperEdgeStateDistance(RubicsCubeState* state);
    int GetLowerEdgeStateDistance(RubicsCubeState* state);
    
    int GetCornerLookupIndex(RubicsCubeState* state);
    int GetUpperEdgeLookupIndex(RubicsCubeState* state);
    int GetLowerEdgeLookupIndex(RubicsCubeState* state);
    
    void TestCornerLookupCaluclation();
    void SetStateToCornerStateByIndex(int index, RubicsCubeState* state);
    
    void GenerateCornerLookupTable(string path);
    void GenerateUpperEdgeLookupTable(string path);
    void GenerateLowerEdgeLookupTable(string path);
}

