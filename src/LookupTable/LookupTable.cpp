#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <iostream>
#include <iomanip>
#include <mutex>
#include <condition_variable>
#include <bitset>
#include <atomic>

#include "LookupTable.h"
#include "../Utils/FileManagement.h"
#include "../Model/RubiksCubeState.h"
#include "../Utils/Stopwatch.h"

using std::string;

const char UNINITIALIIZED = -1;
uint64_t reachedDuplicates = 0;
std::atomic_uint64_t a_reachedStates;
std::mutex consoleMutex;

// TODO put this somewhere else
uint64_t currentDepthStates = 0;
char* cornerLookupTable ;
char* upperEdgeLookupTable;
char* lowerEdgeLookupTable;
char* bigUpperEdgeLookupTable;
char* bigLowerEdgeLookupTable;
char* edgePermutationLookupTable;
char* fullEdgeLookupTable;

// This is just a helper Object to bundle some data, that need to get distributed between various threads.
struct ConcurrencyData {
    int threadCount;
    int* reachedStates;
    int* finishedThreads;

    ConcurrencyData(const int threadCount, int* reachedStates, int* finishedThreads) {
        this->threadCount = threadCount;
        this->reachedStates = reachedStates;
        this->finishedThreads = finishedThreads;
    }
};

namespace LookupTable {
    int threadCount = 1;
}

// Thread, that logs metadata every second to the console.
void Logger(
    uint64_t* reachedStates, 
    bool* hasGenerationFinished, 
    char* currentDepth, 
    uint64_t maxReachableStates
);

// The entry point for the lookup table generation.
void EvaluatePositionWithIterativeDeepening(
    LookupTable::IndexCalculation IndexCalculator, 
    uint64_t maxReachableStates, 
    char* currentDepth, 
    std::vector<char>* shortestMovesPossible, 
    uint64_t* reachedStates, 
    std::mutex* mutex
);

// The work horse. Evealuates a position and traverses the graph deeper up to @maxDepth and populates the lookup table.
void EvaluatePositionRecursive(
    LookupTable::IndexCalculation IndexCalculator, 
    RubiksCubeState state, 
    char depth, 
    Turn lastTurn, 
    std::vector<Turn> exploredTurns, 
    char maxDepth, 
    std::vector<char>* shortestMovesPossible, 
    uint64_t* reachedStates, 
    std::mutex* mutex
);

// Alternative searching method when a certain depth is reached.
void EvaluatePositionWithInverseStateIndex(
    LookupTable::IndexCalculation IndexCalculator, 
    LookupTable::StateCalculator StateCalculator, 
    char depth, std::vector<char>* shortestMovesPossible, 
    uint64_t* localReachedStates, 
    uint64_t startIndex, 
    uint64_t endIndex
);

void LookupTable::GenerateCornerLookupTable() { GenerateLookupTable(LookupTable::CORNER_LOOKUP_TABLE_PATH, GetCornerLookupIndex, CORNER_STATES_COUNT); }
void LookupTable::GenerateUpperEdgeLookupTable() { GenerateLookupTable(LookupTable::E2_LOOKUP_TABLE_PATH, GetE2LookupIndex, EDGE_STATES_COUNT); }
void LookupTable::GenerateLowerEdgeLookupTable() { GenerateLookupTable(LookupTable::E1_LOOKUP_TABLE_PATH, GetE1LookupIndex, EDGE_STATES_COUNT); }
void LookupTable::GenerateBigUpperEdgeLookupTable() { GenerateLookupTable(LookupTable::E2_LOOKUP_TABLE_PATH, GetE2LookupIndex, BIG_EDGE_STATES_COUNT); }
void LookupTable::GenerateBigLowerEdgeLookupTable() { GenerateLookupTable(LookupTable::E1_LOOKUP_TABLE_PATH, GetE1LookupIndex, BIG_EDGE_STATES_COUNT); }
void LookupTable::GenerateEdgePermutationLookupTable() { GenerateLookupTable(LookupTable::EDGE_PERMUTATION_LOOKUP_TABLE_PATH, GetEdgePermutationLookupIndex, FULL_EDGE_PERMUTATIONS_COUNT); }
void LookupTable::GenerateFullEdgeLookupTable(string path) { GenerateLookupTable(path.compare("") == 0 ?  LookupTable::FULL_EDGE_LOOKUP_TABLE_PATH : path, GetEdgeLookupIndex, FULL_EDGE_STATES_COUNT); }


void LookupTable::LoadLookupTables() {
    uint64_t* size = new uint64_t(0);
    cornerLookupTable = FileManagement::LoadBufferFromFile(CORNER_LOOKUP_TABLE_PATH, size);
    bigUpperEdgeLookupTable = FileManagement::LoadBufferFromFile(E2_LOOKUP_TABLE_PATH, size);
    bigLowerEdgeLookupTable = FileManagement::LoadBufferFromFile(E1_LOOKUP_TABLE_PATH, size);
    //edgePermutationLookupTable = FileManagement::LoadBufferFromFile(EDGE_PERMUTATION_LOOKUP_TABLE_PATH, size);
}

int LookupTable::GetCornerStateDistance(RubiksCubeState& state) {
    int index = GetCornerLookupIndex(state);
    return cornerLookupTable[index];
}

int LookupTable::GetE1StateDistance(RubiksCubeState& state) {
    int index = GetE1LookupIndex(state);
    return lowerEdgeLookupTable[index];
}

int LookupTable::GetE2StateDistance(RubiksCubeState& state) {
    int index = GetE2LookupIndex(state);
    return upperEdgeLookupTable[index];
}

int LookupTable::GetEdgePermutationStateDistance(RubiksCubeState& state) {
    int index = GetEdgePermutationLookupIndex(state);
    return edgePermutationLookupTable[index];
}

int LookupTable::GetFullEdgeStateDistance(RubiksCubeState& state) {
    int index = GetEdgeLookupIndex(state);
    return fullEdgeLookupTable[index];
}

void inline ClenaupReachedFlags(std::vector<char>* buffer) {
    for (uint64_t i = 0; i < buffer->capacity(); i++) {
        if ((*buffer)[i] != UNINITIALIIZED) {
            (*buffer)[i] &= 0b01111111;
        }
    }
}

void inline SetReachedFlag(uint64_t index, std::vector<char>* buffer) {
    (*buffer)[index] = (*buffer)[index] | 0b10000000;
}

bool inline GetReachedFlag(uint64_t index, std::vector<char>* buffer) {
    return (*buffer)[index] != UNINITIALIIZED && ((*buffer)[index] & 0b10000000) != 0;
}

void LookupTable::GenerateLookupTable(string path, IndexCalculation IndexCalculator, uint64_t maxReachableStates) {
    uint64_t* reachedStates = new uint64_t(0);
    int* finishedThreads = new int(0);
    char* currentDepth = new char(0);
    bool* hasGenerationFinished = new bool(false);
    std::vector<char>* shortestPossibleMoves = new std::vector<char>(maxReachableStates, UNINITIALIIZED);

    std::mutex* bufferMutex = new std::mutex();

    std::thread(Logger, reachedStates, hasGenerationFinished, currentDepth, maxReachableStates).detach();
    EvaluatePositionWithIterativeDeepening(IndexCalculator, maxReachableStates, currentDepth, shortestPossibleMoves, reachedStates, bufferMutex);

    *hasGenerationFinished = true;

    // Write the content of the lookup table buffer into a file.
    std::cout << endl << "Writing to file." << endl;

    FileManagement::WriteBufferToFile(path, shortestPossibleMoves->data(), maxReachableStates);
    
    std::cout << endl << "Finished writing to file. Table has succesfuly been generated." << endl;
    delete[] shortestPossibleMoves;
}

void EvaluatePositionWithIterativeDeepening(LookupTable::IndexCalculation IndexCalculator, uint64_t maxReachableStates, char* currentDepth, std::vector<char>* shortestMovesPossible, uint64_t* reachedStates, std::mutex* mutex) {
    std::vector<std::thread> threads(LookupTable::threadCount);
    a_reachedStates = 0;
    const char INVERSE_SEARCH_DEPTH_THRESHOLD = 9;

    while (a_reachedStates < maxReachableStates) {
        for (int i = 0; i < LookupTable::threadCount; i++) {
            std::vector<Turn> exploredTurns = Turn::GetSubsetTurns(i, LookupTable::threadCount);
            std::thread thread = std::thread(EvaluatePositionRecursive, IndexCalculator, RubiksCubeState::InitialState().Copy(), 0, Turn::Empty(), exploredTurns, *currentDepth, shortestMovesPossible, reachedStates, mutex);
            threads[i] = std::move(thread);
        }

        for (int i = 0; i < LookupTable::threadCount; i++) {
            threads[i].join();
        }
        
        consoleMutex.lock();
        std::cout << "\33[2K" << "Reached states at depth " << ((int)*currentDepth) << " are " << currentDepthStates << endl;
        consoleMutex.unlock();

        // Reset all reached state flags.
        ClenaupReachedFlags(shortestMovesPossible);

        currentDepthStates = 0;
        (*currentDepth)++;
    }
}

void EvaluatePositionRecursive(LookupTable::IndexCalculation IndexCalculator, RubiksCubeState state, char depth, Turn lastTurn, std::vector<Turn> exploredTurns, char maxDepth, std::vector<char>* shortestMovesPossible, uint64_t* reachedStates, std::mutex* mutex) {
    if (a_reachedStates == (*shortestMovesPossible).size()) { // TODO change statement to variable refference and not size function
        return;
    }

    uint64_t lookupIndex = IndexCalculator(state);
    
    if (lookupIndex != 0 && GetReachedFlag(lookupIndex, shortestMovesPossible)) {
        reachedDuplicates++;
        //mutex->unlock();
        return;
    }

    char& entry = (*shortestMovesPossible)[lookupIndex];
    
    mutex->lock();
    
    if (entry == UNINITIALIIZED) {
		// This state has never been reached before.
		entry = depth;
		a_reachedStates++;
        currentDepthStates++;        
	}

    mutex->unlock();
      
    char entryDepth = entry & 0b01111111;

    if (depth > entryDepth) {
        // we reached an explored state, with a longer path and don't want to continue, since there are no follwoing states will habve shorter paths.
		return;
	}
    
    SetReachedFlag(lookupIndex, shortestMovesPossible);

    if (depth < maxDepth) {
        for (Turn turn : exploredTurns) {
            if (turn.IsTurnBacktracking(lastTurn)) {
                continue;
            }

            state.ApplyTurn(turn);
            EvaluatePositionRecursive(IndexCalculator, state, depth + 1, turn, Turn::AllTurns, maxDepth, shortestMovesPossible, reachedStates, mutex);
            state.ApplyTurn(turn.Inverse());
        }
    }
}

void EvaluatePositionWithInverseStateIndex(
    LookupTable::IndexCalculation IndexCalculator, 
    LookupTable::StateCalculator StateCalculator, 
    char depth, 
    std::vector<char>* shortestMovesPossible, 
    uint64_t* localReachedStates, 
    uint64_t startIndex, 
    uint64_t endIndex
) {
    // We go through all entries in the lookup table
    for (uint64_t index = startIndex; index < endIndex; index++) {
        
        // When a value has not been initialized yet.
        if ((*shortestMovesPossible)[index] == UNINITIALIIZED) {
            // get the corrosponding state
            RubiksCubeState state = StateCalculator(index);

            // and look at all neighbour states
            for (Turn turn : Turn::AllTurns) {
                state.ApplyTurn(turn);
                
                uint64_t neighbourIndex = IndexCalculator(state);
                
                // when a neighbour state has depth - 1 distance, the unitilized state can be reached in depth moves. 
                if ((*shortestMovesPossible)[neighbourIndex] == depth - 1) {
                    (*shortestMovesPossible)[index] = depth;
                    *localReachedStates++;
                    break;
                }

                state.ApplyTurn(turn.Inverse());
            }
        }
    }
}

// TODO remove thread progresses or finally implement fill functionality.
void Logger(uint64_t* reachedStates, bool* hasGenerationFinished, char* currentDepth, uint64_t maxReachableStates) {
    StopWatch timer;
    
    timer.StartTimer();

    
    while (!(*hasGenerationFinished)) {
        double progress = ((double)(a_reachedStates) / (double)maxReachableStates * 100);
        
        consoleMutex.lock();
        
        std::cout  << "Elapsed Time: " << timer.GetFormattedTimeInSeconds()
            << ", reachedStates: " << a_reachedStates 
            << ", progress: " << setprecision(4) << fixed << progress << "%"
            << ", depth: " << to_string(*currentDepth)
            << ", states at depth: " << currentDepthStates
            << ", duplicates: " << reachedDuplicates
            << "\r";
        
        consoleMutex.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Finished Logger" << endl;
}
