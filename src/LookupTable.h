#pragma once

#include <string>
#include "RubicsCubeState.h"

using std::string;

#define uint64 unsigned long long

namespace LookupTable {
    extern int threadCount;
    typedef uint64(*IndexCalculation)(RubicsCubeState&);
    
    const string CORNER_LOOKUP_TABLE_PATH         = "LookupTables\\cornerLookupTable";
    const string UPPER_EDGE_LOOKUP_TABLE_PATH     = "LookupTables\\upperEdgeLookupTable";
    const string LOWER_EDGE_LOOKUP_TABLE_PATH     = "LookupTables\\lowerEdgeLookupTable";
    const string BIG_UPPER_EDGE_LOOKUP_TABLE_PATH = "LookupTables\\bigUpperEdgeLookupTable";
    const string BIG_LOWER_EDGE_LOOKUP_TABLE_PATH = "LookupTables\\bigLowerEdgeLookupTable";
    const string EDGE_PERMUTATION_LOOKUP_TABLE_PATH = "LookupTables\\edgePermutationLookupTable";
    const string FULL_EDGE_LOOKUP_TABLE_PATH      = "fullEdgeLookupTable";

    const int CORNER_ROTATIONS_COUNT = 3 * 3 * 3 * 3 * 3 * 3 * 3; // 3^7
    const int CORNER_PERMUTATIONS_COUNT = 8 * 7 * 6 * 5 * 4 * 3 * 2 * 1; // 8!
    const int CORNER_STATES_COUNT = CORNER_ROTATIONS_COUNT * CORNER_PERMUTATIONS_COUNT; // 8! * 3^7

    const int EDGE_ROTATION_COUNT = 2 * 2 * 2 * 2 * 2 * 2; // 2^6
    const int EDGE_PERMUTATIONS_COUNT = 12 * 11 * 10 * 9 * 8 * 7; // 12! / 6!
    const int EDGE_STATES_COUNT = EDGE_ROTATION_COUNT * EDGE_PERMUTATIONS_COUNT; // (12! / 6!) * 2^6
    
    const int BIG_EDGE_ROTATION_COUNT = 2 * 2 * 2 * 2 * 2 * 2 * 2; // 2^7
    const int BIG_EDGE_PERMUTATIONS_COUNT = 12 * 11 * 10 * 9 * 8 * 7 * 6; // 12! / 5!
    const int BIG_EDGE_STATES_COUNT = BIG_EDGE_ROTATION_COUNT * BIG_EDGE_PERMUTATIONS_COUNT; // (12! / 5!) * 2^7
    
    const uint64 FULL_EDGE_ROTATION_COUNT = 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2; // 2^11
    const uint64 FULL_EDGE_PERMUTATIONS_COUNT = 12 * 11 * 10 * 9 * 8 * 7 * 6 * 5 * 4 * 3 * 2 * 1; // 12!
    const uint64 FULL_EDGE_STATES_COUNT = FULL_EDGE_ROTATION_COUNT * FULL_EDGE_PERMUTATIONS_COUNT; // (12!) * 2^11

    const unsigned long long ALL_STATES_COUNT = FULL_EDGE_STATES_COUNT * CORNER_STATES_COUNT;

    void GenerateLookupTable(string path, IndexCalculation indexCalculator, uint64 maxReachableStates);
    void LoadLookupTables();
    
    int GetCornerStateDistance(RubicsCubeState& state);
    int GetUpperEdgeStateDistance(RubicsCubeState& state);
    int GetLowerEdgeStateDistance(RubicsCubeState& state);
    int GetBigUpperEdgeStateDistance(RubicsCubeState& state);
    int GetBigLowerEdgeStateDistance(RubicsCubeState& state);
    int GetEdgePermutationStateDistance(RubicsCubeState& state);
    int GetFullEdgeStateDistance(RubicsCubeState& state);
    
    // TODO comment what these do
    uint64 GetCornerLookupIndex(RubicsCubeState& state);
    uint64 GetUpperEdgeLookupIndex(RubicsCubeState& state);
    uint64 GetLowerEdgeLookupIndex(RubicsCubeState& state);
    uint64 GetBigUpperEdgeLookupIndex(RubicsCubeState& state);
    uint64 GetBigLowerEdgeLookupIndex(RubicsCubeState& state);
    uint64 GetEdgePermutationLookupIndex(RubicsCubeState& state);
    uint64 GetFullEdgeLookupIndex(RubicsCubeState& state);
    
    //void TestCornerLookupCaluclation();
    //void SetStateToCornerStateByIndex(int index, RubicsCubeState& state);
    
    void GenerateCornerLookupTable();
    void GenerateUpperEdgeLookupTable();
    void GenerateLowerEdgeLookupTable();
    void GenerateBigUpperEdgeLookupTable();
    void GenerateBigLowerEdgeLookupTable();
    void GenerateEdgePermutationLookupTable();
    void GenerateFullEdgeLookupTable(string path = "");

    // TODO delete
    int GetRotationIndex(RubicsCubeState& state);
}

