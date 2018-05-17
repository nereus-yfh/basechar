//字符类型基类

#include <cstdio>
#ifndef ENCODE_BASE_CHAR_H
#define ENCODE_BASE_CHAR_H

enum __CHAR_STAT {
    CHAR_STAT_SUCC = 0,
    CHAR_STAT_EOF,
    CHAR_STAT_ERR,
};

class BaseChar {
public:
    BaseChar();
    BaseChar(const BaseChar &c);
    virtual ~BaseChar();
public:
    virtual int read(FILE *) = 0;
    virtual char * cstr() const;
    virtual size_t size() const;
    virtual void clear();
protected:
    char *_word;
    size_t _len;
};

BaseChar::BaseChar() {
    _word = NULL;
    _len = 0;
}

BaseChar::BaseChar(const BaseChar &c) {
    _word = new char[c.size() + 1];
    snprintf(_word, c.size() + 1, "%s", c.cstr());
    _len = c.size();
}

BaseChar::~BaseChar() {
    clear();
}

char *BaseChar::cstr() const{
    return _word;
}

size_t BaseChar::size() const{
    return _len;
}

void BaseChar::clear() {
    delete[] _word;
    _word = NULL;
    _len = 0;
}
#endif
