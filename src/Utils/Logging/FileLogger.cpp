#include "FileLogger.h"

#include <fstream>
#include <iostream>
#include <vector>

const std::string FileLogger::STANDARD_PATH = "log.txt";

FileLogger::FileLogger(std::string path, std::string jobDescriton) {
    this->path = path;

    std::ofstream log (path);
    
    log << jobDescriton << std::endl;
    log.close();
}

void FileLogger::logNewLine(std::string message) {
    updateLastLine(message + "\n");
}

void FileLogger::updateLastLine(std::string message) {
    std::ifstream fin(this->path);
    std::vector<std::string> lines;
    
    while (!fin.eof()) {
        std::string buffer;
        getline(fin, buffer);
        lines.push_back(buffer + '\n');
    }

    fin.close();
    lines[lines.size() - 1] = message;
    
    std::ofstream fout(this->path);

    for (std::string line: lines) { // c++11 syntax
        fout << line;
    }

    fout.close();
}