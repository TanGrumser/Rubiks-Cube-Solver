#pragma once

#include <string>
#include <vector>

namespace FileManagement {
    
    //template<typename T>
    int WriteBufferToFile(std::string path, const char* buffer, uint64_t bufferSize);
    char* LoadBufferFromFile(std::string path, uint64_t* bufferSize);
    void CompareFiles(std::string firstPath, std::string secondPath);
    
    void writeLinesToFile(std::string path, std::vector<std::string> lines);
    std::vector<std::string> parseALlLines(std::string path);
}