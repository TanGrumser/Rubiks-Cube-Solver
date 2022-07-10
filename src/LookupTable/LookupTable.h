#pragma once

#include <string>
#include "../Model/RubiksCubeState.h"
#include <iostream>
#include "../Utils/PermutationIndexer.h"

using std::string;

namespace LookupTable {
    extern PermutationIndexer<8> cornerIndexer;
    extern PermutationIndexer<12, 7> eGroupIndexer;
    extern PermutationIndexer<12> edgeIndexer;

    extern int threadCount;
    typedef uint64_t(*IndexCalculation)(RubiksCubeState&);
    typedef RubiksCubeState&(*StateCalculator)(uint64_t);
    
    const string CORNER_LOOKUP_TABLE_PATH           = "LookupTables\\cornerLookupTable";
    const string E1_LOOKUP_TABLE_PATH               = "LookupTables\\bigLowerEdgeLookupTable";
    const string E2_LOOKUP_TABLE_PATH               = "LookupTables\\bigUpperEdgeLookupTable";
    const string EDGE_PERMUTATION_LOOKUP_TABLE_PATH = "LookupTables\\edgePermutationLookupTable";
    const string FULL_EDGE_LOOKUP_TABLE_PATH        = "fullEdgeLookupTable";

    void GenerateLookupTable(string path, IndexCalculation indexCalculator, uint64_t maxReachableStates);
    void LoadLookupTables();
    
    int GetCornerStateDistance(RubiksCubeState& state);
    int GetE2StateDistance(RubiksCubeState& state);
    int GetE1StateDistance(RubiksCubeState& state);
    int GetEdgePermutationStateDistance(RubiksCubeState& state);
    int GetFullEdgeStateDistance(RubiksCubeState& state);
    
    // TODO comment what these do
    uint64_t GetCornerLookupIndex(RubiksCubeState& state);
    uint64_t GetE1LookupIndex(RubiksCubeState& state);
    uint64_t GetE2LookupIndex(RubiksCubeState& state);
    uint64_t GetEdgePermutationLookupIndex(RubiksCubeState& state);
    uint64_t GetEdgeLookupIndex(RubiksCubeState& state);
    
    //void TestCornerLookupCaluclation();
    //void SetStateToCornerStateByIndex(int index, RubiksCubeState& state);
    
    void GenerateCornerLookupTable();
    void GenerateUpperEdgeLookupTable();
    void GenerateLowerEdgeLookupTable();
    void GenerateBigUpperEdgeLookupTable();
    void GenerateBigLowerEdgeLookupTable();
    void GenerateEdgePermutationLookupTable();
    void GenerateFullEdgeLookupTable(string path = "");
}

