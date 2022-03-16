#include <string>
#include "FileManagement.h"
#include <fstream>
#include <iostream>


inline bool exists_test0 (const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}
    
//template<typename T>
int FileManagement::WriteBufferToFile(std::string path, const char* buffer, int bufferSize) {
    std::ofstream ofs;
    ofs.open(path);

    if (!ofs) {
        return 1;
    }

    ofs.write(buffer, bufferSize);    

    if (!ofs) {
        return 2;
    }

    ofs.close();
    /*
    char array[] = "YOUR TEXT HERE";

    // Open a file for writing. 
    // (This will replace any existing file. Use "w+" for appending)
    FILE *file = fopen(path, "w");

    int results = fputs(buffer, file);
    
    fclose(file);

    if (results == EOF) {
        return 1;    
    }
    */

    return 0;
}

char* FileManagement::LoadBufferFromFile(std::string path, int* bufferSize) {
    std::streampos size;
    char* memblock;

    if (!exists_test0(path)) {
        std::cout << "File couldn't be found." << std::endl;
        return nullptr;
    }

    std::ifstream file (path, std::ios::in|std::ios::binary|std::ios::ate);
    
    if (file.is_open()) {
        size = file.tellg();
        std::cout <<"found size " << size << std::endl;
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
  /*
    std::ofstream file;
    file.open("test.txt");
    
    if(!file) {  
        return 1;
    }

    auto input = file.rdbuf();
    input.
    file.close();
    return 0;
  */
}
