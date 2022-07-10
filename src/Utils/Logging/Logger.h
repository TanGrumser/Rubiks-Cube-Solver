#pragma once

#include <string>

class Logger {

    public:
        /**
         * @brief 
         * 
         * @brief append a message in a new line at the end of the file.
         * 
         * @param message the that will be appended
         */
        virtual void logNewLine(std::string message) = 0;
        

        /**
         * @brief overwrite the last line, that is included in the file.
         * 
         * @param message the message that should overwrite the last line.
         */
        virtual void updateLastLine(std::string message) = 0;
};