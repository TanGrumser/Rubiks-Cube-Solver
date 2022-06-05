#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <iostream>
#include <iomanip>
#include <mutex>
#include <condition_variable>
#include "FileManagement.h"
#include "RubicsCubeState.h"
#include "LookupTable.h"
#include <bitset>
#include <atomic>

using std::string;
using LookupTable::CORNER_STATES_COUNT;
using LookupTable::EDGE_STATES_COUNT;

const char UNINITIALIIZED = -1;
int reachedDuplicates = 0;
std::atomic_int a_reachedStates;
std::mutex consoleMutex;

const long long int statesAtDepth[12] = {
    1ll, //18^0
    18ll, //18^1
    18ll * 18ll, //18^2
    18ll * 18ll * 18ll, //18^3
    18ll * 18ll * 18ll * 18ll, //18^4
    18ll * 18ll * 18ll * 18ll * 18ll, //18^5
    18ll * 18ll * 18ll * 18ll * 18ll * 18ll, //18^6
    18ll * 18ll * 18ll * 18ll * 18ll * 18ll * 18ll, //18^7
    18ll * 18ll * 18ll * 18ll * 18ll * 18ll * 18ll * 18ll, //18^8
    18ll * 18ll * 18ll * 18ll * 18ll * 18ll * 18ll * 18ll * 18ll, //18^9
    18ll * 18ll * 18ll * 18ll * 18ll * 18ll * 18ll * 18ll * 18ll * 18ll, //18^10
    18ll * 18ll * 18ll * 18ll * 18ll * 18ll * 18ll * 18ll * 18ll * 18ll * 18ll , //18^11
};

// TODO put this somewhere else
int currentDepthStates = 0;
char* cornerLookupTable;
char* upperEdgeLookupTable;
char* lowerEdgeLookupTable;
char* bigUpperEdgeLookupTable;
char* bigLowerEdgeLookupTable;
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

void Logger(int* reachedStates, bool* hasGenerationFinished, char* currentDepth, int maxReachableStates, std::vector<long long int*>* threadProgresses);
void EvaluatePositionWithIterativeDeepening(LookupTable::IndexCalculation IndexCalculator, int maxReachableStates, char* currentDepth, std::vector<char>* shortestMovesPossible, int* reachedStates, std::mutex* mutex, std::vector<long long int*>* threadProgresses);
void EvaluatePosition(LookupTable::IndexCalculation IndexCalculator, RubicsCubeState* state, char depth, Turn lastTurn, std::vector<Turn> exploredTurns, char maxDepth, std::vector<char>* shortestMovesPossible, int* reachedStates, std::mutex* mutex, long long int* threadProgress);

void LookupTable::GenerateCornerLookupTable() { GenerateLookupTable(LookupTable::CORNER_LOOKUP_TABLE_PATH, GetCornerLookupIndex, CORNER_STATES_COUNT); }
void LookupTable::GenerateUpperEdgeLookupTable() { GenerateLookupTable(LookupTable::UPPER_EDGE_LOOKUP_TABLE_PATH, GetUpperEdgeLookupIndex, EDGE_STATES_COUNT); }
void LookupTable::GenerateLowerEdgeLookupTable() { GenerateLookupTable(LookupTable::LOWER_EDGE_LOOKUP_TABLE_PATH, GetLowerEdgeLookupIndex, EDGE_STATES_COUNT); }
void LookupTable::GenerateBigUpperEdgeLookupTable() { GenerateLookupTable(LookupTable::BIG_UPPER_EDGE_LOOKUP_TABLE_PATH, GetBigUpperEdgeLookupIndex, BIG_EDGE_STATES_COUNT); }
void LookupTable::GenerateBigLowerEdgeLookupTable() { GenerateLookupTable(LookupTable::BIG_LOWER_EDGE_LOOKUP_TABLE_PATH, GetBigLowerEdgeLookupIndex, BIG_EDGE_STATES_COUNT); }
void LookupTable::GenerateFullEdgeLookupTable() { GenerateLookupTable(LookupTable::FULL_EDGE_LOOKUP_TABLE_PATH, GetFullEdgeLookupIndex, FULL_EDGE_STATES_COUNT); }


void LookupTable::LoadLookupTables() {
    int* size = new int(0);
    cornerLookupTable = FileManagement::LoadBufferFromFile(CORNER_LOOKUP_TABLE_PATH, size);
    bigUpperEdgeLookupTable = FileManagement::LoadBufferFromFile(BIG_UPPER_EDGE_LOOKUP_TABLE_PATH, size);
    bigLowerEdgeLookupTable = FileManagement::LoadBufferFromFile(BIG_LOWER_EDGE_LOOKUP_TABLE_PATH, size);
}

int LookupTable::GetCornerStateDistance(RubicsCubeState* state) {
    int index = GetCornerLookupIndex(state);
    return cornerLookupTable[index];
}

int LookupTable::GetUpperEdgeStateDistance(RubicsCubeState* state) {
    int index = GetUpperEdgeLookupIndex(state);
    return upperEdgeLookupTable[index];
}

int LookupTable::GetLowerEdgeStateDistance(RubicsCubeState* state) {
    int index = GetLowerEdgeLookupIndex(state);
    return lowerEdgeLookupTable[index];
}

int LookupTable::GetBigUpperEdgeStateDistance(RubicsCubeState* state) {
    int index = GetBigUpperEdgeLookupIndex(state);
    return bigUpperEdgeLookupTable[index];
}

int LookupTable::GetBigLowerEdgeStateDistance(RubicsCubeState* state) {
    int index = GetBigLowerEdgeLookupIndex(state);
    return bigLowerEdgeLookupTable[index];
}

int LookupTable::GetFullEdgeStateDistance(RubicsCubeState* state) {
    int index = GetFullEdgeLookupIndex(state);
    return fullEdgeLookupTable[index];
}

void inline ClenaupReachedFlags(std::vector<char>* buffer) {
    for (int i = 0; i < buffer->capacity(); i++) {
        if ((*buffer)[i] != UNINITIALIIZED) {
            (*buffer)[i] &= 0b01111111;
        }
    }
}

void inline SetReachedFlag(int index, std::vector<char>* buffer) {
    (*buffer)[index] = (*buffer)[index] | 0b10000000;
}

bool inline GetReachedFlag(int index, std::vector<char>* buffer) {
    return (*buffer)[index] != UNINITIALIIZED && ((*buffer)[index] & 0b10000000) != 0;
}

void LookupTable::GenerateLookupTable(string path, IndexCalculation IndexCalculator, uint64 maxReachableStates) {
    int* reachedStates = new int(0);
    int* finishedThreads = new int(0);
    char* currentDepth = new char(0);
    bool* hasGenerationFinished = new bool(false);
    std::vector<long long int*>* threadProgresses = new std::vector<long long int*>(LookupTable::threadCount, new long long int(0));
    std::vector<char>* shortestPossibleMoves = new std::vector<char>(maxReachableStates, UNINITIALIIZED);

    std::vector<std::thread> threads = {};
    std::mutex* bufferMutex = new std::mutex();

    ConcurrencyData concurrencyData(LookupTable::threadCount, reachedStates, finishedThreads);

    std::thread(Logger, reachedStates, hasGenerationFinished, currentDepth, maxReachableStates, threadProgresses).detach();
    EvaluatePositionWithIterativeDeepening(IndexCalculator, maxReachableStates, currentDepth, shortestPossibleMoves, reachedStates, bufferMutex, threadProgresses);

    *hasGenerationFinished = true;

    // Write the content of the lookup table buffer into a file.
    std::cout << endl << "Writing to file." << endl;

    FileManagement::WriteBufferToFile(path, shortestPossibleMoves->data(), maxReachableStates);
    
    std::cout << endl << "Finished writing to file. Table has succesfuly been generated." << endl;
    delete[] shortestPossibleMoves;
}

void EvaluatePositionWithIterativeDeepening(LookupTable::IndexCalculation IndexCalculator, int maxReachableStates, char* currentDepth, std::vector<char>* shortestMovesPossible, int* reachedStates, std::mutex* mutex, std::vector<long long int*>* threadProgresses) {
    std::vector<std::thread> threads(LookupTable::threadCount);
    a_reachedStates = 0;

    while (a_reachedStates < maxReachableStates) {
        for (int i = 0; i < LookupTable::threadCount; i++) {
            *(*threadProgresses)[i] = 0ll;
            std::vector<Turn> exploredTurns = Turn::GetSubsetTurns(i, LookupTable::threadCount);
            std::thread thread = std::thread(EvaluatePosition, IndexCalculator, RubicsCubeState::InitialState()->Copy(), 0, Turn::Empty(), exploredTurns, *currentDepth, shortestMovesPossible, reachedStates, mutex, (*threadProgresses)[0]);
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

void EvaluatePosition(LookupTable::IndexCalculation IndexCalculator, RubicsCubeState* state, char depth, Turn lastTurn, std::vector<Turn> exploredTurns, char maxDepth, std::vector<char>* shortestMovesPossible, int* reachedStates, std::mutex* mutex, long long int* threadProgress) {
    if (a_reachedStates == (*shortestMovesPossible).size()) {
        return;
    }

    uint64 lookupIndex = IndexCalculator(state);
    
    
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
        //*threadProgress += statesAtDepth[maxDepth - depth];
        //mutex->unlock();
		return;
	}
    
    
    SetReachedFlag(lookupIndex, shortestMovesPossible);

    

    if (depth < maxDepth) {
        for (Turn turn : exploredTurns) {
            if (turn.IsTurnBacktracking(lastTurn)) {
                //*threadProgress += statesAtDepth[maxDepth - depth - 1];
                continue;
            }

            state->ApplyTurn(turn);
            EvaluatePosition(IndexCalculator, state, depth + 1, turn, Turn::AllTurns, maxDepth, shortestMovesPossible, reachedStates, mutex, threadProgress);
            //*threadProgress += statesAtDepth[maxDepth - depth - 1];
            state->ApplyTurn(turn.Inverse());
        }
    } else {
        *threadProgress += statesAtDepth[depth];
    }
}

// TODO remove thread progresses or finally implement fill functionality.
void Logger(int* reachedStates, bool* hasGenerationFinished, char* currentDepth, int maxReachableStates, std::vector<long long int*>* threadProgresses) {
    auto startTime = std::chrono::system_clock::now();
    
    while (!(*hasGenerationFinished)) {
        auto timeSinceStart = (std::chrono::system_clock::now() - startTime);
        double progress = ((double)(a_reachedStates) / (double)maxReachableStates * 100);
        
        consoleMutex.lock();
        
        std::cout  << "Elapsed Time: " << timeSinceStart.count() / 1000000000 << "s"
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
