#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

namespace Utils {
    void inline SetMostSinificantBit(char* value) {
        *value |= 0b10000000;
    }

    void inline UnsetMostSinificantBit(char* value) {
        *value &= 0b01111111;
    }

    bool inline IsMostSignificantBitSet(char value) {
        return (value & 0b10000000) != 0;
    }

    std::string inline remove_zeros(std::string numberstring) {
        auto it = numberstring.end() - 1;
        
        while(*it == '0') {
            numberstring.erase(it);
            it = numberstring.end() - 1;
        }

        if(*it == '.') {
            numberstring.erase(it);
        } 
        
        return numberstring;
    }

    std::string inline convert(double number, int decimalPlaces) {
        std::stringstream ss{};
        ss << std::setprecision(decimalPlaces) << std::fixed << std::showpoint << number;
        std::string numberstring{ss.str()};
        return remove_zeros(numberstring);
    }
}