#include "CommandLineHandler.h"

//XXX
#include "Utils/Logging/FileLogger.h"
#include "Utils/Logging/Logger.h"
#include "Utils/Logging/ConsoleLogger.h"

int main(int argc, char *argv[]) {

    CommandLineHandler::start(argc, argv);

    return 0;
}