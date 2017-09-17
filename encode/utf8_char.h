#include "base_char.h"

class Utf8Char : public BaseChar {
public:
    Utf8Char();
    virtual ~Utf8Char();
public:
    virtual int read(FILE *fp);
};

Utf8Char::Utf8Char() {
}

Utf8Char::~Utf8Char() {
}

int Utf8Char::read(FILE *fp) {
    if (feof(fp)) {
        return CHAR_STAT_EOF;
    }
    unsigned char c = fgetc(fp);
    if ((c & 0x80) == 0) {
        _len = 1;
        delete[] _word;
        _word = new char[_len + 1];
        _word[0] = c;
        return CHAR_STAT_SUCC;
    }
    return CHAR_STAT_ERR;
}
