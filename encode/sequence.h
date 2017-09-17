#include "utf8_char.h"
#include <cstring>
#ifndef ENCODE_SEQUENCE_CHAR_H
#define ENCODE_SEQUENCE_CHAR_H
enum __sequence_enum {
    SEQUENCE_CSTR_ERR = -1,
    SEQUENCE_TYPE_NONE= 0,
    SEQUENCE_TYPE_SINGLE = 1,
    SEQUENCE_TYPE_PAIR,
};
//template <class CharType>
class Sequence {
public:
    Sequence();
    ~Sequence();
public:
    inline bool operator == (const Sequence& seq);
    inline bool operator < (const Sequence& seq);
    inline size_t uv() const {return _uv;}
    inline size_t size() const {return _len;}
    inline int type() const {return _type;}
    int cstr(char *s, size_t len) const;
    inline BaseChar *words(size_t id) const {return _words[id];}
    int set_word(size_t id, BaseChar *word);
private:
    BaseChar *_words[2];
    int _type;
    size_t _uv;
    size_t _len;
};

Sequence :: Sequence() {
    _uv = 0;
    _type = SEQUENCE_TYPE_NONE;
    _len = 0;
}

Sequence :: ~Sequence() {
}

inline bool Sequence:: operator == (const Sequence &seq) {
    if (_type != seq.type()) {
        return false;
    }
    if (_type == SEQUENCE_TYPE_SINGLE) {
        if (_words[0] == NULL || seq.words(0) == NULL) {
            return false;
        }
        return strcmp(_words[0]->cstr(), (*(seq.words(0))).cstr()) == 0;
    } else {
        if (_words[0] == NULL || _words[1] == NULL || 
            seq.words(0) == NULL || seq.words(1) == NULL) {
            return false;
        }
        return strcmp(_words[0]->cstr(), (*(seq.words(0))).cstr()) == 0
            && strcmp(_words[1]->cstr(), (*(seq.words(1))).cstr()) == 0;
    }
}

int Sequence::cstr(char *s, size_t len) const {
    if (len > _len) {
        return SEQUENCE_CSTR_ERR;
    }
    int offset = snprintf(s, len, "%s", _words[0]->cstr());
    if (_type == SEQUENCE_TYPE_PAIR) {
        offset = snprintf(s+offset, len, "%s", _words[1]->cstr());
    }
    return offset;
}
#endif
