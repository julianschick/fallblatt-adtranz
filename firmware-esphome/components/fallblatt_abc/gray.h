#ifndef GRAY_H_
#define GRAY_H_

#include <stdint.h>
#include <string>

class GrayConverter {
public:
    GrayConverter();
    ~GrayConverter();

    uint8_t positionToGray(int pos);
    uint8_t positionToGray2(int pos);
    static int charToPosition(uint8_t c);
    static std::string utf8ToLatin(std::string s);
private:
    uint8_t* tbl;
};

#endif