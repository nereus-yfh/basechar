#include "base_char.h"
#ifndef ENCODE_UTF8_CHAR_H
#define ENCODE_UTF8_CHAR_H
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
    delete[] _word;
    if ((c & 0x80) == 0) {
        _len = 1;
        _word = new char[_len + 1];
        _word[0] = c;
        _word[_len] = 0;
        return CHAR_STAT_SUCC;
    }
    int cnt = 0;
    unsigned char mask = 0x80;
    while (c & mask) {
        cnt ++;
        mask >>= 1;
    }
    if (cnt > 6) {
        return CHAR_STAT_ERR;
    }
    _len = cnt;
    _word = new char[_len+1];
    _word[0] = c;
    _word[_len] = 0;
    int status = CHAR_STAT_SUCC;
    for(int i = 1; i < cnt; i++) {
        if(feof(fp)) {
            status = CHAR_STAT_EOF;
            break; 
        }
        _word[i] = fgetc(fp);
        if ((_word[i] & 0xc0) != 0x80) {
            status = CHAR_STAT_ERR;
            break;
        }
    }
    if (status != CHAR_STAT_SUCC) {
        clear();
        return CHAR_STAT_ERR;
    }
    return CHAR_STAT_SUCC;
}
#endif
