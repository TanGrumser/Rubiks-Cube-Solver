#pragma once

#include <string>

#include "../Model/RubiksCubeState.h"
#include "../Utils/PermutationIndexer.h"

using std::string;

namespace LookupTable {
    typedef uint64_t(*IndexCalculator)(RubiksCubeState&);
    typedef RubiksCubeState(*StateCalculator)(uint64_t);

    extern PermutationIndexer<8>     cornerIndexer;
    extern PermutationIndexer<12, 7> eGroupIndexer;
    extern PermutationIndexer<12>    edgeIndexer;

    extern int threadCount;
    
    const string CORNER_LOOKUP_TABLE_PATH           = "LookupTables\\cornerLookupTable";
    const string E1_LOOKUP_TABLE_PATH               = "LookupTables\\bigLowerEdgeLookupTable";
    const string E2_LOOKUP_TABLE_PATH               = "LookupTables\\bigUpperEdgeLookupTable";
    const string EDGE_PERMUTATION_LOOKUP_TABLE_PATH = "LookupTables\\edgePermutationLookupTable";
    const string EDGE_LOOKUP_TABLE_PATH             = "LookupTables\\fullEdgeLookupTable";

    void PopulateLookupTableWithIDDFS(string path, IndexCalculator indexCalculator, uint64_t maxReachableStates);
 
    void LoadCornerLookupTable();
    void LoadE1LookupTable();
    void LoadE2LookupTable();
    void LoadEdgeLookupTable();
    void LoadEdgePermutationLookupTable();
    
    char GetCornerStateDistance(RubiksCubeState& state);
    char GetE1StateDistance(RubiksCubeState& state);
    char GetE2StateDistance(RubiksCubeState& state);
    char GetEdgePermutationStateDistance(RubiksCubeState& state);
    char GetEdgeStateDistance(RubiksCubeState& state);
    
    // TODO comment what these do
    uint64_t GetCornerLookupIndex(RubiksCubeState& state);
    uint64_t GetE1LookupIndex(RubiksCubeState& state);
    uint64_t GetE2LookupIndex(RubiksCubeState& state);
    uint64_t GetEdgePermutationLookupIndex(RubiksCubeState& state);
    uint64_t GetEdgeLookupIndex(RubiksCubeState& state);
    

    RubiksCubeState GetCornerStateFromIndex(uint64_t index);
    RubiksCubeState GetEdgeStateFromIndex(uint64_t index);

    //void TestCornerLookupCaluclation();
    //void SetStateToCornerStateByIndex(int index, RubiksCubeState& state);
    
    void GenerateCornerLookupTable();
    void GenerateE1LookupTable();
    void GenerateE2LookupTable();
    void GenerateEdgePermutationLookupTable();
    void GenerateEdgeLookupTable(string path = "");
}