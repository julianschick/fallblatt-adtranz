#include "gray.h"

GrayConverter::GrayConverter() {
    tbl = new uint8_t[84] {
        0x7F, 0x3F, 0x1F, 0x5F, 0x4F, 0x0F, 0x2F, 0x6F, 0x67, 0x27, 
        0x07, 0x47, 0x57, 0x17, 0x37, 0x77, 0x73, 0x33, 0x13, 0x53,
        0x43, 0x03, 0x23, 0x63, 0x6B, 0x2B, 0x0B, 0x4B, 0x5B, 0x1B,
        0x3B, 0x7B, 0x79, 0x39, 0x19, 0x59, 0x49, 0x09, 0x29, 0x69,
        0x61, 0x21, 0x01, 0x41, 0x51, 0x11, 0x31, 0x71, 0x75, 0x35,
        0x15, 0x55, 0x45, 0x05, 0x25, 0x65, 0x6D, 0x2D, 0x0D, 0x4D,
        0x5D, 0x1D, 0x3D, 0x7D, 0x7C, 0x3C, 0x1C, 0x5C, 0x4C, 0x0C,
        0x2C, 0x6C, 0x64, 0x24, 0x04, 0x44, 0x54, 0x14, 0x34, 0x74,
        0x70, 0x30, 0x10, 0x50
    };
}

GrayConverter::~GrayConverter() {
    delete[] tbl;
}

uint8_t GrayConverter::positionToGray2(int pos) {
    if (pos < 0 || pos > 83) {
        return tbl[0];
    } else {
        return tbl[pos];
    }
}

uint8_t GrayConverter::positionToGray(int pos) {
    if (pos < 0 || pos > 83) {
        return 0x7F;
    } else {
        uint8_t pos8 = pos;
        pos8 = ~(pos8 ^ (pos8 >> 1));

        // reverse bit order
        pos8 = (((pos8 >> 7) & 1) | ((pos8 >> 5) & 2) | ((pos8 >> 3) & 4) | ((pos8 >> 1) & 8) | ((pos8 << 1) & 0x10) | ((pos8 << 3) & 0x20) | ((pos8 << 5) & 0x40) | ((pos8 << 7) & 0x80)) >> 1;
        return pos8;
    }
}

int GrayConverter::charToPosition(uint8_t c) {

    // A-Z
    if (c >= 65 && c <= 90) {
        return c - 54;
    }
    // a-z
    if (c >= 97 && c <= 122) {
        return c - 57;
    }
    // 0-9
    if (c >= 48 && c <= 57) {
        return c - 47;
    }

    switch (c) {
        // space
        case 32: return 0;
        // ,
        case 44: return 72;
        // -
        case 45: return 70;
        // .
        case 46: return 71;
        // (
        case 40: return 73;
        // )
        case 41: return 74;
        // ?
        case 63: return 75;
        // !
        case 33: return 76;
        // *
        case 42: return 77;
        // :
        case 58: return 78;
        // /
        case 47: return 79;
        // "
        case 34: return 81;
        // =
        case 61: return 82;
        // +
        case 43: return 83; 
        // |
        case 124: return 80;
        // Ä
        case 196: return 37;
        // Ö
        case 214: return 38;
        // Ü
        case 220: return 39;
        // ä
        case 228: return 66;
        // ö
        case 246: return 67;
        // ü
        case 252: return 68;
    }


    return 0;
}

std::string GrayConverter::utf8ToLatin(std::string s) {
    std::string result;

    for (int i = 0; i < s.size(); i++) {
        if (i < s.size() - 1 && s[i] == 0xC3) {
            char c2 = s[i+1];

            if (c2 == 0x84)      { result.push_back(196); i++; }
            else if (c2 == 0x96) { result.push_back(214); i++; }
            else if (c2 == 0x9C) { result.push_back(220); i++; }
            else if (c2 == 0xA4) { result.push_back(228); i++; }
            else if (c2 == 0xB6) { result.push_back(246); i++; }
            else if (c2 == 0xBC) { result.push_back(252); i++; }
            else {
                result.push_back(s[i]);
            }
        } else {
            result.push_back(s[i]);
        }
    }

    return result;
}