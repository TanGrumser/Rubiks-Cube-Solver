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

}