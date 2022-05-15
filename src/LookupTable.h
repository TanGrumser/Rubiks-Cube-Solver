#pragma once

#include <string>
#include "RubicsCubeState.h"

using std::string;

#define uint64 unsigned long long

namespace LookupTable {
    extern int threadCount;
    typedef int(*IndexCalculation)(RubicsCubeState*);

    
    #if DEBUG
        const string CORNER_LOOKUP_TABLE_PATH         = "C:\\Users\\tangr\\Desktop\\Uni\\Bachelorarbeit\\Rubiks-Cube-Solver\\cornerLookupTable";
        const string UPPER_EDGE_LOOKUP_TABLE_PATH     = "C:\\Users\\tangr\\Desktop\\Uni\\Bachelorarbeit\\Rubiks-Cube-Solver\\upperEdgeLookupTable";
        const string LOWER_EDGE_LOOKUP_TABLE_PATH     = "C:\\Users\\tangr\\Desktop\\Uni\\Bachelorarbeit\\Rubiks-Cube-Solver\\lowerEdgeLookupTable";
        const string BIG_UPPER_EDGE_LOOKUP_TABLE_PATH = "C:\\Users\\tangr\\Desktop\\Uni\\Bachelorarbeit\\Rubiks-Cube-Solver\\bigUpperEdgeLookupTable";
        const string BIG_LOWER_EDGE_LOOKUP_TABLE_PATH = "C:\\Users\\tangr\\Desktop\\Uni\\Bachelorarbeit\\Rubiks-Cube-Solver\\bigLowerEdgeLookupTable";
    #else
        const string CORNER_LOOKUP_TABLE_PATH         = "cornerLookupTable";
        const string UPPER_EDGE_LOOKUP_TABLE_PATH     = "upperEdgeLookupTable";
        const string LOWER_EDGE_LOOKUP_TABLE_PATH     = "lowerEdgeLookupTable";
        const string BIG_UPPER_EDGE_LOOKUP_TABLE_PATH = "bigUpperEdgeLookupTable";
        const string BIG_LOWER_EDGE_LOOKUP_TABLE_PATH = "bigLowerEdgeLookupTable";
    #endif

    const int CORNER_ROTATIONS_COUNT = 3 * 3 * 3 * 3 * 3 * 3 * 3; // 3^7
    const int CORNER_PERMUTATIONS_COUNT = 8 * 7 * 6 * 5 * 4 * 3 * 2 * 1; // 8!
    const int CORNER_STATES_COUNT = CORNER_ROTATIONS_COUNT * CORNER_PERMUTATIONS_COUNT; // 8! * 3^7

    const int EDGE_ROTATION_COUNT = 2 * 2 * 2 * 2 * 2 * 2; // 2^6
    const int EDGE_PERMUTATIONS_COUNT = 12 * 11 * 10 * 9 * 8 * 7; // 12! / 6!
    const int EDGE_STATES_COUNT = EDGE_ROTATION_COUNT * EDGE_PERMUTATIONS_COUNT; // (12! / 6!) * 2^6
    
    const int BIG_EDGE_ROTATION_COUNT = 2 * 2 * 2 * 2 * 2 * 2 * 2; // 2^7
    const int BIG_EDGE_PERMUTATIONS_COUNT = 12 * 11 * 10 * 9 * 8 * 7 * 6; // 12! / 5!
    const int BIG_EDGE_STATES_COUNT = BIG_EDGE_ROTATION_COUNT * BIG_EDGE_PERMUTATIONS_COUNT; // (12! / 7!) * 2^7
    
    const uint64 FULL_EDGE_ROTATION_COUNT = 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2; // 2^11
    const uint64 FULL_EDGE_PERMUTATIONS_COUNT = 12 * 11 * 10 * 9 * 8 * 7 * 6 * 5 * 4 * 3; // 12! / 2!
    const uint64 FULL_EDGE_STATES_COUNT = FULL_EDGE_ROTATION_COUNT * FULL_EDGE_PERMUTATIONS_COUNT; // (12! / 2!) * 2^11

    const unsigned long long ALL_STATES_COUNT = FULL_EDGE_STATES_COUNT * CORNER_STATES_COUNT;

    void GenerateLookupTable(string path, IndexCalculation IndexCalculator, int maxReachableStates);
    void LoadLookupTables();
    
    int GetCornerStateDistance(RubicsCubeState* state);
    int GetUpperEdgeStateDistance(RubicsCubeState* state);
    int GetLowerEdgeStateDistance(RubicsCubeState* state);
    int GetBigUpperEdgeStateDistance(RubicsCubeState* state);
    int GetBigLowerEdgeStateDistance(RubicsCubeState* state);
    
    // TODO comment what these do
    int GetCornerLookupIndex(RubicsCubeState* state);
    int GetUpperEdgeLookupIndex(RubicsCubeState* state);
    int GetLowerEdgeLookupIndex(RubicsCubeState* state);
    int GetBigUpperEdgeLookupIndex(RubicsCubeState* state);
    int GetBigLowerEdgeLookupIndex(RubicsCubeState* state);
    uint64 GetFullEdgeLookupIndex(RubicsCubeState* state);
    
    void TestCornerLookupCaluclation();
    void SetStateToCornerStateByIndex(int index, RubicsCubeState* state);
    
    void GenerateCornerLookupTable();
    void GenerateUpperEdgeLookupTable();
    void GenerateLowerEdgeLookupTable();
    void GenerateBigUpperEdgeLookupTable();
    void GenerateBigLowerEdgeLookupTable();
}

