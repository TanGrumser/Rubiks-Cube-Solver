#pragma once

#include <chrono>
#include <string>
#include <iostream>
#include <sstream>
#include <string>

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

class StopWatch {
    private:
        std::chrono::system_clock::time_point t1;
        std::chrono::system_clock::time_point t2;

    public:
        void StartTimer();
        void StopTimer();
        double GetTimeInMilliseconds();
        std::string GetFormattedTimeInSeconds();
        std::string GetFormattedTimeInMilliseconds();
};