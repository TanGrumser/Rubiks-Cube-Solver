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
#include "TurnIndexer.h"

int main(int argc, char *argv[]) {
    int last = 0;

    for (int i = 0; i < 18; i++) {
        int newV =  TurnIndexer::getIndex(new int[3] {i, 0, 0}, 1);

        if (last + 1 != newV) {
            std::cout << "false: " << last << " " << newV << endl;
        }

        last = newV;
    }

    for (int i = 0; i < 18; i++) {
        for (int j = 0; j < 18; j++) {
            int newV =  TurnIndexer::getIndex(new int[3] {i, j, 0}, 2);

            if (last + 1 != newV) {
                std::cout << "false: " << last << " " << newV << endl;
            }

            last = newV;
        }
    }

    for (int i = 0; i < 18; i++) {
        for (int j = 0; j < 18; j++) {
            for (int k = 0; k < 18; k++) {
                int newV =  TurnIndexer::getIndex(new int[3] {i, j, k}, 3);

                if (last + 1 != newV) {
                    std::cout << "false: " << last << " " << newV << endl;
                }

                last = newV;
            }
        }
    }

    //CommandLineHandler::Start(argc, argv);

    return 0;
}