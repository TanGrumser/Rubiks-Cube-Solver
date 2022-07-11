#include <iostream>
#include <vector>

#include "CommandLineHandler.h"
#include "Model/RubiksCubeState.h"
#include "Model/Turn.h"
#include "Utils/StringUtils.h"
#include "Solver/Solver.h"
#include "Utils/Stopwatch.h"
#include "LookupTable/LookupTable.h"
#include "Utils/FileManagement.h"
#include "DuplicateStateDetection/DuplicateState.h"

void SolveCube(RubiksCubeState& state);
void ParseFile(std::string path);
void GenerateLookupTable(int table);
void PrintHelp();

void CommandLineHandler::start(int argc, char *argv[]) {
    RubiksCubeState state = RubiksCubeState::InitialState().Copy();

    //Parse all command line arguments.
    for (int i = 1; i < argc; i++) {
        if (((string) argv[i]).compare("--generateLookupTable") == 0 || ((string) argv[i]).compare("-glt") == 0) {
            GenerateLookupTable(std::atoi(argv[i + 1]));
        }

        if (((string) argv[i]).compare("--ds") == 0) {
            DuplicateState::active = true;
        }

        if (((string) argv[i]).compare("--help") == 0) {
            PrintHelp();
        }

        if (((string) argv[i]).compare("-s") == 0) {
            string stateString = (string) argv[i + 1];
            state = RubiksCubeState::ParseStateString(stateString);
        }

        if (((string) argv[i]).compare("-h") == 0) {
            Solver::heuristicMode = std::atoi(argv[i + 1]);
        }
        
        if (((string) argv[i]).compare("-a") == 0) {
            Solver::solverIndex = std::atoi(argv[i + 1]);
        }

        if (((string) argv[i]).compare("-threads") == 0) {
            std::cout << "Set threads" << endl;
            Solver::threadCount = std::atoi(argv[i + 1]);
            //LookupTable::threadCount = std::atoi(argv[i + 1]);
        }

        if (((string) argv[i]).compare("-t") == 0) {
            string turnString = (string) argv[i + 1];
            vector<string> turns = StringUtils::Split(turnString, " ");
            
            for (int i = 0; i < turns.size(); i++) {
                Turn turn = Turn::Parse(turns[i]);
                state.ApplyTurn(turn);
            }
        }
    }
    
    LookupTable::GenerateCornerLookupTable();
}

void SolveCube(RubiksCubeState& state) {
    vector<Turn> solution;
    StopWatch* timer = new StopWatch();
    timer->StartTimer();

    std::cout << "Loading lookup tables and duplicate state table." << endl;
        LookupTable::LoadCornerLookupTable();
        LookupTable::LoadE1LookupTable();
        LookupTable::LoadE2LookupTable();
        //DuplicateState::LoadDuplicateStateIndex();
        //DuplicateState::LoadDuplicateStateTurnIndex();
    std::cout << "Finished loading in " << timer->GetFormattedTimeInSeconds() << endl << endl;

    
    std::cout << "Starting solve." << endl;

    timer->StartTimer();
    
    switch (Solver::solverIndex) {
        case 0: solution = Solver::PR_IterativeDeepeningAStar(state); break;
        case 1: solution = Solver::SR_IterativeDeepeningAStar(state); break; // XXX

        default: throw std::runtime_error("Specified solver index isn't refferencing a solver: " + Solver::solverIndex);
    }

    timer->StopTimer();

    std::cout << "Time required: " << timer->GetFormattedTimeInSeconds() << endl;

    for (int i = 0; i < solution.size(); i++) {
        std::cout << solution[i].ToString();

        if (i < solution.size() - 1) {
            std::cout << " ";
        }
    }
}
void GenerateLookupTable(int lookupTableIndex) {
    switch (lookupTableIndex)
    {
        case 0: LookupTable::GenerateCornerLookupTable(); break;
        case 1: LookupTable::GenerateE1LookupTable(); break;
        case 2: LookupTable::GenerateE2LookupTable(); break;
        case 4: LookupTable::GenerateEdgePermutationLookupTable(); break;
        case 5: LookupTable::GenerateEdgeLookupTable(); break;
            
        default: throw std::runtime_error("Provided lookup table index is invalid: " + lookupTableIndex);
    }
}

void ParseFile(std::string path) {
    std::cout << "Parsing file" << std::endl;
    uint64_t* size = new uint64_t();
    char* lookupTable = FileManagement::LoadBufferFromFile(path, size);

    std::vector<int> occurences(12, 0);
    std::cout << "size: " << *size << endl;
    
    for (int i = 0; i < *size; i++) {        
        if (lookupTable[i] != -1) {
            occurences[lookupTable[i]]++;
        }
        
    }

    int sum = 0;

    for (int i = 0; i < 12; i++) {
        std::cout << occurences[i] << endl;
        sum+= occurences[i];
    }

    std::cout << "sum: " << sum << endl;
}

void PrintHelp() {
    std::string help = 
        "Use --turn or -t to pass a sequence of shuffle tunrs (e.g.: -t U L' D2 r l')\n"
        "";
    std::cout << help << endl;
}