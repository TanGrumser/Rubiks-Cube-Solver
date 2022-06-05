#include <iostream>
#include <string>
#include "RubicsCubeState.h"
#include "Turn.h"
#include "StringUtils.h"
#include <vector>
#include "Solver.h"
#include "Stopwatch.h"
#include "LookupTable.h"
#include <thread>
#include <mutex>
#include "FileManagement.h"
#include "LookupTable.h"
#include "CommandLineHandler.h"
#include "DuplicateState.h"
#include "utils.h"
#include "RubicsCubeStateShift.h"
#include "PermutationIndexer.h"
#include <vector>
#include <random>
#include "PerformanceTest.h"

int main(int argc, char *argv[]) {
        
    CommandLineHandler::Start(argc, argv);
    //DuplicateState::GenerateLookupTable();
    /*
    std::cout << "Loading lookup tables and duplicate state table." << endl;
        LookupTable::LoadLookupTables();
        DuplicateState::LoadDuplicateStateIndex();
    std::cout << "Finished Loading." << endl;

    std::cout << "Finished Loading." << endl;
    vector<Turn> s;

    
    for (int i = 0; i < 1000; i++) {
        RubicsCubeState* state = RubicsCubeState::InitialState()->Copy();
        state->scramble(1);

        Stopwatch::StartTimer();
        vector<Turn> solution = Solver::PR_IterativeDeepeningAStar(state);
        Stopwatch::StopTimer();


        s.insert(s.end(), solution.begin(), solution.end());

        delete state;
    }


    std::cout << Stopwatch::GetFormattedTimeInSeconds() << " " << s.size() << endl;
    */

    return 0;
}