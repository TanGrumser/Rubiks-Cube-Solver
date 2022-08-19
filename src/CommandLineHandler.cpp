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
#include "Utils/Logging/Logger.h"
#include "Utils/Logging/ConsoleLogger.h"
#include "Utils/Logging/FileLogger.h"
#include "Utils/robin-map/robin_map.h"

void SolveCube(RubiksCubeState& state, Logger* logger);
void parseFile(std::string path);
void solveShufflesFromFile(string path, Logger* logger);
void GenerateLookupTable(int table);
void PrintHelp();
void generateShufflesFile(string path, int numShuffles, int shuffleLenght);
void GenerateDuplciateStateLookupTable(int lookupTableIndex);

void CommandLineHandler::start(int argc, char *argv[]) {
    RubiksCubeState state = RubiksCubeState::InitialState().Copy();
    ConsoleLogger consoleLogger("");
    Logger* logger = &consoleLogger;

    //Parse all command line arguments.
    for (int i = 1; i < argc; i++) {
        if (((string) argv[i]).compare("-dsmd") == 0) {
            DuplicateState::maxDepth = std::atoi(argv[i + 1]);
            std::cout << "Set max depth bound for duplicate state detection to" << DuplicateState::maxDepth << std::endl;
        }

        if (((string) argv[i]).compare("--generateLookupTable") == 0 || ((string) argv[i]).compare("-glt") == 0) {
            GenerateLookupTable(std::atoi(argv[i + 1]));
        }

        if (((string) argv[i]).compare("--generateDuplicateStateLookupTable") == 0 || ((string) argv[i]).compare("-gdslt") == 0) {
            GenerateDuplciateStateLookupTable(std::atoi(argv[i + 1]));
        }

        if (((string) argv[i]).compare("--ds") == 0) {
            int index = std::atoi(argv[i + 1]);
            string answer;
            
            switch (index) {
                case 0:
                    DuplicateState::mode = DuplicateState::Mode::OFF;
                    answer = "Deactivated duplcite State detetction.";
                break;

                case 1:
                    DuplicateState::mode = DuplicateState::Mode::STATE_INDEX;
                    DuplicateState::LoadDuplicateStateIndex();
                    answer = "Using state indexing for dulciate state detection.";
                break;

                case 2:
                    DuplicateState::mode = DuplicateState::Mode::TURN_INDEX;
                    DuplicateState::LoadDuplicateStateTurnIndex();
                    answer = "Using turn indexing for dulciate state detection.";
                break;
                
                case 3:
                    DuplicateState::mode = DuplicateState::Mode::HASH_TABLE;
                    answer = "Storing every node visited in hashtable.";
                break;

            default:
                break;
            }

            logger->logNewLine(answer);
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
        }

        if (((string) argv[i]).compare("-loadFile") == 0) {
            string path = argv[i + 1];
            solveShufflesFromFile(path, logger);
        }

        if (((string) argv[i]).compare("-logTo") == 0) {
            string path = argv[i + 1];
            logger = new FileLogger(path, "");
        }

        if (((string) argv[i]).compare("-generateShufflesFile") == 0) {
            string path = argv[i + 1];
            int numShuffles = std::atoi(argv[i + 2]);
            int shuffleLenght = std::atoi(argv[i + 3]);
            
            generateShufflesFile(path, numShuffles, shuffleLenght);
        }

        if (((string) argv[i]).compare("-t") == 0) {
            string turnString = (string) argv[i + 1];
            vector<string> turns = StringUtils::Split(turnString, " ");
            
            for (int i = 0; i < turns.size(); i++) {
                Turn turn = Turn::Parse(turns[i]);
                state.ApplyTurn(turn);
            }
        }

        if (((string) argv[i]).compare("-solve") == 0) {
            SolveCube(state, logger);
        }
    }
}

void SolveCube(RubiksCubeState& state, Logger* logger) {
    vector<Turn> solution;
    StopWatch timer;
    LookupTable::LoadAllLookupTables();

    timer.StartTimer();

    logger->logNewLine("Starting to solve cube with IDA*.");
    
    #if SORT_SUCCESSORS
    logger->logNewLine("Sorting successors.");
    #endif

    solution = Solver::PR_IterativeDeepeningAStar(state, logger); 

    logger->logNewLine("Time required: " + timer.GetFormattedTimeInSeconds());

    string solutionString;

    for (int i = 0; i < solution.size(); i++) {
        solutionString += solution[i].ToString();

        if (i < solution.size() - 1) {
            solutionString += " ";
        }
    }

    logger->logNewLine("Solution:\n" + solutionString + "\n");
}

void generateShufflesFile(string path, int numShuffles, int shuffleLenght) {
    std::vector<string> shufflesStrings;

    for (int i = 0; i < numShuffles; i++) {
        string shuffleString;
        Turn lastTurn = Turn::Empty();

        for (int j = 0; j < shuffleLenght; j++) {
            Turn randomTurn;

            do {
                randomTurn = Turn::Random();
            } while (randomTurn.IsTurnBacktracking(lastTurn));

            if (randomTurn.index >= 18 || randomTurn.index < 0) {
                std::cout << randomTurn.index << endl;
            }

            shuffleString += randomTurn.ToString() + (j != shuffleLenght - 1 ? " " : "");
            lastTurn = randomTurn;
        }

        shufflesStrings.push_back(shuffleString);
    }

    FileManagement::writeLinesToFile(path, shufflesStrings);
}

void solveShufflesFromFile(string path, Logger* logger) {
    vector<string> lines = FileManagement::parseALlLines(path);

    LookupTable::LoadAllLookupTables();

    for (int i = 0; i < 4; i++) {
        switch (i) {
        case 0: 
            logger->logNewLine("Shifted State Index Duplicate State Detexction (Max Depth: 7)");
            DuplicateState::LoadDuplicateStateIndex();
            DuplicateState::mode = DuplicateState::Mode::STATE_INDEX;
            DuplicateState::maxDepth = 7;
        break;

        case 1: 
            logger->logNewLine("Shifted State Index Duplicate State Detexction (Max Depth: 8)");
            DuplicateState::mode = DuplicateState::Mode::STATE_INDEX;
            DuplicateState::maxDepth = 8;
        break;

        case 2: 
            logger->logNewLine("Turn Index Duplicate State Detexction (Max Depth: 7)");
            DuplicateState::LoadDuplicateStateTurnIndex();
            DuplicateState::mode = DuplicateState::Mode::TURN_INDEX;
            DuplicateState::maxDepth = 7;
        break;

        case 3: 
            logger->logNewLine("Turn Index Duplicate State Detexction (Max Depth: 8)");
            DuplicateState::mode = DuplicateState::Mode::TURN_INDEX;
            DuplicateState::maxDepth = 8;
        break;
        
        default:
            break;
        }

        for (int j = 0; j < 1; j++) {
            vector<Turn> shuffle = Turn::parseShuffle(lines[j]);
            RubiksCubeState state = RubiksCubeState::InitialState().Copy();

            for (Turn turn : shuffle) {
                state.ApplyTurn(turn);
            }

            logger->logNewLine("Starting solve of shuffle\n" + lines[j]);
            SolveCube(state, logger);
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

void GenerateDuplciateStateLookupTable(int lookupTableIndex) {
    switch (lookupTableIndex) {
        case 0: DuplicateState::GenerateLookupTable(); break;
        case 1: DuplicateState::GenerateTurnBasedLookupTable(); break;

        default: throw std::runtime_error("Provided lookup table index is invalid: " + lookupTableIndex);
    }
}

void parseFile(std::string path) {
    std::cout << "Parsing file" << std::endl;
    uint64_t* size = new uint64_t();
    char* lookupTable = FileManagement::LoadBufferFromFile(path, size);
    tsl::robin_map<char, uint64_t> occurencesMap;
    
    std::vector<int> occurences(15, 0);
    std::cout << "size: " << *size << endl;
    
    for (uint64_t i = 0; i < *size; i++) {
        if (occurencesMap.find(lookupTable[i]) == occurencesMap.end()) {
            occurencesMap[lookupTable[i]] = 0;
        }

        occurencesMap[lookupTable[i]]++;
    }

    uint64_t sum = 0;

    for (const std::pair<char, uint64_t> occurence : occurencesMap) {
        std::cout << (int)occurence.first << " occured " << occurence.second << endl;

        if (14 >= occurence.first)
        sum += occurence.second;
    }

    std::cout << "sum of values below 14: " << sum << endl;
}

void PrintHelp() {
    std::string help = 
        "Use --turn or -t to pass a sequence of shuffle tunrs (e.g.: -t U L' D2 r l')\n"
        "";
    std::cout << help << endl;
}