#ifndef GRAY_H_
#define GRAY_H_

#include <stdint.h>
#include <string>
#include <optional>

#define USE_GRAY_TABLE 0

class GrayConverter {
public:
    GrayConverter();
    ~GrayConverter();

    uint8_t positionToGray(int pos);
    static std::optional<int> charToPosition(uint8_t c);
    static std::string utf8ToLatin(std::string s);
private:
#if USE_GRAY_TABLE
    uint8_t* tbl;
#endif
};

#endif