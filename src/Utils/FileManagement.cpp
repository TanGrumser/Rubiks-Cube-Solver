#include <string>
#include "FileManagement.h"
#include <fstream>
#include <iostream>
#include <stdexcept>

inline bool exists_test0 (const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}
    
int FileManagement::WriteBufferToFile(std::string path, const char* buffer, uint64_t bufferSize) {
    std::ofstream ofs;
    ofs.open(path, std::ios::binary);

    if (!ofs) {
        throw std::runtime_error("Couldn't open file handle to " + path);
    }

    ofs.write(buffer, bufferSize); 

    if (!ofs) {
        throw std::runtime_error("Writing to file " + path + " failed.");
    }

    ofs.close();

    return 0;
}

char* FileManagement::LoadBufferFromFile(std::string path, uint64_t* bufferSize) {
    std::streampos size;
    char* memblock;

    if (!exists_test0(path)) {
        std::cout << "File couldn't be found. (" << path << ")" << std::endl;
        return nullptr;
    }

    std::ifstream file (path, std::ios::in|std::ios::binary|std::ios::ate);
    
    if (file.is_open()) {
        size = file.tellg();
        memblock = new char [size];
        file.seekg (0, std::ios::beg);
        file.read (memblock, size);
        file.close();
        *bufferSize = size;

        return memblock;
    } else {
        std::cout << "File couldn't be opened." << std::endl;
        throw std::exception();
    }

    return 0;
}

void FileManagement::CompareFiles(std::string firstPath, std::string secondPath) {
    uint64_t* localSize = new uint64_t(0);
    uint64_t* downloadSize = new uint64_t(0);
    char* local = FileManagement::LoadBufferFromFile(firstPath, localSize);
    char* download = FileManagement::LoadBufferFromFile(secondPath, downloadSize);

    if (*localSize != *downloadSize) {
        std::cout << "sizes are not equal. " << *localSize << ", " << *downloadSize << std::endl;
        return;
    }
 
    for (int i = 0; i < *localSize; i++) {
        if (local[i] != download[i]) {
            std::cout << "Files differ at " << i << " with " << std::to_string(local[i]) << " and " << std::to_string(download[i]) << std::endl;
        }
    }

    std::cout << "Comparison finished. Everything is fine." << std::endl;
}


