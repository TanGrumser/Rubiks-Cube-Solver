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

using std::string;
using LookupTable::CORNER_STATES_COUNT;

const char UNINITIALIIZED = -1;

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

const int THREAD_COUNT = 6;
const int MAX_DEPTH = 11;

void Logger(int* reachedStates, bool* hasGenerationFinished, char* currentDepth, std::vector<long long int*>* threadProgresses);
void EvaluatePositionWithIterativeDeepening(char maxDepth, char* currentDepth, std::vector<char>* shortestMovesPossible, int* reachedStates, std::mutex* mutex, std::vector<long long int*>* threadProgresses);
void EvaluatePosition(RubicsCubeState* state, char depth, Turn lastTurn, std::vector<Turn> exploredTurns, char maxDepth, std::vector<char>* shortestMovesPossible, int* reachedStates, std::mutex* mutex, long long int* threadProgress);

void ClenaupFlags(std::vector<char>* buffer) {
    for (int i = 0; i < CORNER_STATES_COUNT; i++) {
        if ((*buffer)[i] != UNINITIALIIZED) {
            (*buffer)[i] &= 0b00111111;
        }
    }
}

void SetIrrelevantFlag(int index, std::vector<char>* buffer) {
    (*buffer)[index] =  ((*buffer)[index] & 0b00111111) | 0b10000000;
}

bool IsIrrelevantFlagSet(int index, std::vector<char>* buffer) {
    return (*buffer)[index] != UNINITIALIIZED && ((*buffer)[index] & 0b10000000) != 0;
}

void LookupTable::GenerateCornerLookupTable(string path, int maxDepth = 11) {
    

    int* reachedStates = new int(0);
    int* finishedThreads = new int(0);
    char* currentDepth = new char(0);
    bool* hasGenerationFinished = new bool(false);
    std::vector<long long int*>* threadProgresses = new std::vector<long long int*>(THREAD_COUNT, new long long int(0));
    std::vector<char>* shortestPossibleMoves = new std::vector<char>(CORNER_STATES_COUNT, UNINITIALIIZED);

    std::vector<std::thread> threads = {};
    std::mutex* bufferMutex = new std::mutex();

    ConcurrencyData concurrencyData(THREAD_COUNT, reachedStates, finishedThreads);

    std::thread(Logger, reachedStates, hasGenerationFinished, currentDepth, threadProgresses).detach();
    EvaluatePositionWithIterativeDeepening(maxDepth, currentDepth, shortestPossibleMoves, reachedStates, bufferMutex, threadProgresses);


    *hasGenerationFinished = true;

    // Write the content of the lookup table buffer into a file.
    std::cout << endl << "Writing to file." << endl;
    FileManagement::WriteBufferToFile(path, shortestPossibleMoves->data(), CORNER_STATES_COUNT);
}

void Logger(int* reachedStates, bool* hasGenerationFinished, char* currentDepth, std::vector<long long int*>* threadProgresses) {
    auto startTime = std::chrono::system_clock::now();
    
    while (!(*hasGenerationFinished)) {
        auto timeSinceStart = (std::chrono::system_clock::now() - startTime);
        double progress = ((double)(*reachedStates) / (double)CORNER_STATES_COUNT * 100);
        
        std::cout  << "Elapsed Time: " << timeSinceStart.count() / 1000000000 << "s"
            << ", reachedStates: " << (*reachedStates) 
            << ", progress: " << setprecision(4) << fixed << progress << "%"
            << ", depth: (" << to_string(*currentDepth) << "/" << MAX_DEPTH << ")"
            << ", states at depth: " << currentDepthStates
            << ", ";
        
        for (int i = 0; i < THREAD_COUNT; i++) {
            double tProgress = double(*(*threadProgresses)[i]) / double(statesAtDepth[*currentDepth]) * 100.0;
            std::cout << "t" << i << ": " << setprecision(2) << fixed << *(*threadProgresses)[i] << "% ";
        }

        std::cout << "\r";
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "Finished Logger" << endl;
}

void EvaluatePositionWithIterativeDeepening(char maxDepth, char* currentDepth, std::vector<char>* shortestMovesPossible, int* reachedStates, std::mutex* mutex, std::vector<long long int*>* threadProgresses) {
    std::vector<std::thread> threads(THREAD_COUNT);
    
    while (*currentDepth <= maxDepth) {
        
        for (int i = 0; i < THREAD_COUNT; i++) {
            *(*threadProgresses)[i] = 0ll;
            std::vector exploredTurns = std::vector<Turn> {Turn(i * 3), Turn(i * 3 + 1), Turn(i * 3 + 2)};
            std::thread thread = std::thread(EvaluatePosition, RubicsCubeState::InitialState()->Copy(), 0, Turn::Empty(), exploredTurns, *currentDepth, shortestMovesPossible, reachedStates, mutex, (*threadProgresses)[0]);
            threads[i] = std::move(thread);
        }

        for (int i = 0; i < THREAD_COUNT; i++) {
            threads[i].join();
        }

        //std::cin.get();
        currentDepthStates = 0;
        (*currentDepth)++;
    }
}

void EvaluatePosition(RubicsCubeState* state, char depth, Turn lastTurn, std::vector<Turn> exploredTurns, char maxDepth, std::vector<char>* shortestMovesPossible, int* reachedStates, std::mutex* mutex, long long int* threadProgress) {
    if (*reachedStates == (*shortestMovesPossible).size()) {
        return;
    }

    int lookupIndex = LookupTable::GetCornerLookupIndex(state);

    mutex->lock();
	
    if ((*shortestMovesPossible)[lookupIndex] == UNINITIALIIZED) {
		// This state has never been reached before.
		(*shortestMovesPossible)[lookupIndex] = depth;
		(*reachedStates)++;
        currentDepthStates++;
	} else if (depth > (*shortestMovesPossible)[lookupIndex]) {
        // we reached an explored state, with a longer path and don't want to continue, since there are no follwoing states will habve shorter paths.
        mutex->unlock();
        //*threadProgress += statesAtDepth[maxDepth - depth];
		return;
	}

    mutex->unlock();

    if (depth < maxDepth) {
        for (Turn turn : exploredTurns) {
            if (turn.IsTurnBacktracking(lastTurn)) {
                //*threadProgress += statesAtDepth[maxDepth - depth - 1];
                continue;
            }

            state->ApplyTurn(turn);
            EvaluatePosition(state, depth + 1, turn, Turn::AllTurns, maxDepth, shortestMovesPossible, reachedStates, mutex, threadProgress);
            //*threadProgress += statesAtDepth[maxDepth - depth - 1];
            state->ApplyTurn(turn.Inverse());
        }
    } else {
        *threadProgress += statesAtDepth[depth];
    }
}