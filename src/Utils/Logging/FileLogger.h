#pragma once

#include <string>

#include "Logger.h"

class FileLogger : public Logger {
    

    private:
        std::string path;

    public:
        static const std::string STANDARD_PATH;

        FileLogger(std::string path, std::string jobDescriton);
        
        /**
         * @brief append a message in a new line at the end of the file.
         * 
         * @param message the that will be appended
         */
        void logNewLine(std::string message);

        /**
         * @brief overwrite the last line, that is included in the file.
         * 
         * @param message the message that should overwrite the last line.
         */
        void updateLastLine(std::string message);
};