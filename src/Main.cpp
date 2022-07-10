#include "CommandLineHandler.h"

//XXX
#include "Utils/Logging/FileLogger.h"
#include "Utils/Logging/Logger.h"
#include "Utils/Logging/ConsoleLogger.h"

void LogTest(Logger* logger) {
    logger->logNewLine("Line 1");
    logger->logNewLine("Multiline 1\nMultiline 2");
    logger->logNewLine("Line to be overwritten.");
    logger->updateLastLine("updated line");
}

int main(int argc, char *argv[]) {
    
    
    FileLogger fLogger("log.txt", "File Test");
    ConsoleLogger cLogger("Console Test");
 
    LogTest(&fLogger);
    LogTest(&cLogger);
    
    return 0;
}