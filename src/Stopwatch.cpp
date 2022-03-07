#include <chrono>
#include <string>
#include <iostream>
#include <sstream>

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

namespace Stopwatch {
    std::chrono::system_clock::time_point t1;
    std::chrono::system_clock::time_point t2;

    void StartTimer() {
        t1 = std::chrono::system_clock::now();
    }

    void StopTimer() {
        t2 = std::chrono::system_clock::now();
    }

    double GetTimeInMilliseconds() {
        duration<double, std::milli> ms_double = t2 - t1;
        return ms_double.count();
    }

    std::string GetFormattedTimeInSeconds() {
        return std::to_string(duration<double>(t2 - t1).count()) + "s";
    }

    std::string GetFormattedTimeInMilliseconds() {
        return std::to_string(GetTimeInMilliseconds() / 1000.0);
    }
}