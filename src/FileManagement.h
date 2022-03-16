#pragma once

#include <string>

namespace FileManagement {
    
    //template<typename T>
    int WriteBufferToFile(std::string path, const char* buffer, int bufferSize);
    char* LoadBufferFromFile(std::string path, int* bufferSize);
}