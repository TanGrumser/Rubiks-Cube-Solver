#include "CommandLineHandler.h"

//XXX
#include "Utils/PermutationIndexer.h"
#include <iostream>
#include <array>

int main(int argc, char *argv[]) {
    PermutationIndexer<6> indexer;

    array<unsigned, 6> perm = {0, 1, 2, 3, 4, 5};
    array<unsigned ,6> recosntruction = indexer.getPermutation(indexer.rank(perm));

    for (int i = 0; i < 6; i++) {
        if (perm[i] != recosntruction[i]) {
            std::cout << "Fail" << std::endl;
        }
    }

    std::cout << "Sucess" << std::endl;
    //CommandLineHandler::start(argc, argv);
    
    return 0;
}