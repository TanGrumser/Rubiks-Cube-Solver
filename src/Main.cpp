#include "CommandLineHandler.h"

//XXX
#include "LookupTable/StateIndexReverser.h"
#include "Utils/PermutationIndexer.h"
#include <array>
#include <vector>
#include <algorithm>
#include "LookupTable/LookupTable.h"
#include "Utils/FileManagement.h"

int main(int argc, char *argv[]) {
    
    CommandLineHandler::start(argc, argv);
    
    return 0;
}