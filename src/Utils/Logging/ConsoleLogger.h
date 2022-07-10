#pragma once

#include <string>

#include "Logger.h"

class ConsoleLogger : public Logger {
    public:
        /**
         * @brief Construct a new Console Logger object and logs the job Description.
         * 
         */
        ConsoleLogger(std::string jobDescription);
    
        /**
         * @brief append a message in a new line at the end of the file.
         * 
         * @param message the that will be appended
         */
        void logNewLine(std::string message);

        /**
         * @brief print the message at the beggining of the last line. Note, that the end of the last line isn't deleted.
         * 
         * @param message the message that should overwrite the last line.
         */
        void updateLastLine(std::string message);
};