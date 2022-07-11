#include "ConsoleLogger.h"

#include <iostream>

ConsoleLogger::ConsoleLogger(std::string jobDescription) {
    std::cout << jobDescription;
}

void ConsoleLogger::logNewLine(std::string message) {
    std::cout << '\r' << message << std::endl;
}

void ConsoleLogger::updateLastLine(std::string message) {
    std::cout << '\r' << message;
}