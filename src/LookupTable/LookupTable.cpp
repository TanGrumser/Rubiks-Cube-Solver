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