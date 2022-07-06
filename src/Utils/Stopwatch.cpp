#include "Stopwatch.h"

void StopWatch::StartTimer() {
    this->t1 = std::chrono::system_clock::now();
}

void StopWatch::StopTimer() {
    this->t2 = std::chrono::system_clock::now();
}

double StopWatch::GetTimeInMilliseconds() {
    this->t2 = std::chrono::system_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;
    return ms_double.count();
}

std::string StopWatch::GetFormattedTimeInSeconds() {
    this->t2 = std::chrono::system_clock::now();
    return std::to_string(duration<double>(t2 - t1).count()) + "s";
}

std::string StopWatch::GetFormattedTimeInMilliseconds() {
    this->t2 = std::chrono::system_clock::now();
    return std::to_string(GetTimeInMilliseconds() / 1000.0);
}
