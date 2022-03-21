#include <iostream>
#include <vector>
#include "RubicsCubeState.h"
#include "Turn.h"
#include "CommandLineHandler.h"
#include "StringUtils.h"
#include "Solver.h"
#include "Stopwatch.h"
#include "LookupTable.h"
#include "FileManagement.h"

void SolveCube(RubicsCubeState* state);
void ParseFile(std::string path);

void CommandLineHandler::Start(int argc, char *argv[]) {
    RubicsCubeState* state = RubicsCubeState::InitialState()->Copy();
    LookupTable::LoadLookupTables();

    //Parse all command line arguments.
    for (int i = 1; i < argc; i++) {
        if (((string) argv[i]).compare("-s") == 0) {
            string stateString = (string) argv[i + 1];
            state = RubicsCubeState::ParseStateString(stateString);
        }

        if (((string) argv[i]).compare("-h") == 0) {
            Solver::heuristicMode = std::atoi(argv[i + 1]);
        }

        if (((string) argv[i]).compare("-t") == 0) {
            string turnString = (string) argv[i + 1];
            vector<string> turns = StringUtils::Split(turnString, " ");
            
            for (int i = 0; i < turns.size(); i++) {
                Turn turn = Turn::Parse(turns[i]);
                state->ApplyTurn(turn);
            }
        }
    }

    //ParseFile(LookupTable::LOWER_EDGE_LOOKUP_TABLE_PATH);
    SolveCube(state);
    //std::cout << LookupTable::GetUpperEdgeStateDistance(state) << endl;
}

void SolveCube(RubicsCubeState* state) {
    std::cout << "starting solve." << endl;
    //std::cout << Solver::GetDistanceHeuristic(state, RubicsCubeState::InitialState()) << endl;
    Stopwatch::StartTimer();
    vector<Turn> solution = Solver::IterativeDeepeningAStar(state);
    Stopwatch::StopTimer();

    std::cout << "Time required: " << Stopwatch::GetFormattedTimeInSeconds() << endl;

    for (int i = 0; i < solution.size(); i++) {
        std::cout << solution[i].ToString();

        if (i < solution.size() - 1) {
            std::cout << " ";
        }
    }
}

void ParseFile(std::string path) {
    std::cout << "Parsing file" << std::endl;
    int* size = new int();
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