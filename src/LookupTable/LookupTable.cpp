#include "LookupTable.h"
#include "../Utils/Stopwatch.h"

bool LookupTable::areLookupTablesLoaded = false;
PermutationIndexer<8>     LookupTable::cornerIndexer;
PermutationIndexer<12, 7> LookupTable::eGroupIndexer;
PermutationIndexer<12>    LookupTable::edgeIndexer;

void LookupTable::LoadAllLookupTables() {
    StopWatch timer;
    timer.StartTimer();

    if (!areLookupTablesLoaded) {
        std::cout << "Loading lookup tables and duplicate state table." << endl;
        
        LoadCornerLookupTable();
        
        #ifdef USE_FULL_EDGE_LT
        LoadEdgeLookupTable();
        #else
        LoadE1LookupTable();
        LoadE2LookupTable();
        LoadEdgePermutationLookupTable();
        #endif
        
        std::cout << "Finished loading in " << timer.GetFormattedTimeInSeconds() << endl << endl;
        areLookupTablesLoaded = true;
    }
}

void LookupTable::TestStateReconstruction() {
    for (int i = 0; i < 100; i++) {
        RubiksCubeState state = RubiksCubeState::InitialState().Copy();

        for (int j = 0; j < 100; j++) {
            state.ApplyTurn(Turn::Random());
        }

        uint64_t e1Index = LookupTable::GetE2LookupIndex(state);
        RubiksCubeState reconstruction = LookupTable::GetE2StateFromIndex(e1Index);
        uint64_t reconstructionIndex = LookupTable::GetE2LookupIndex(reconstruction);
        
        if (e1Index != reconstructionIndex) {
            std::cout << "fail" << endl;
            return;
        }
    }

    std::cout << "sucess" << endl;
}