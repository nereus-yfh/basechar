#include "utf8_char.h"
#include <new>
#include <cstring>
#include <string>
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
    Sequence(int type,const char *s) ;
    
public:
    inline bool operator == (const Sequence& seq);
    inline bool operator < (const Sequence& seq) {return _uv > seq.uv();}
    inline size_t uv() const {return _uv;}
    inline size_t size() const {return _words.size();}
    inline int type() const {return _type;}
    inline const char* cstr() const {return _words.c_str();}
    inline const std::string& words() const {return _words;}
    inline void add_uv() {_uv++;}

    //inline BaseChar *words(size_t id) const {return _words[id];}
    //int set_word(int type, size_t id, BaseChar *word);
private:
    std::string _words;
    int _type;
    size_t _uv;
};

Sequence :: Sequence() {
    _uv = 0;
    _type = SEQUENCE_TYPE_NONE;
}

Sequence :: ~Sequence() {
}

Sequence :: Sequence(int type, const char *s) {
    _words = s;
    /*
    if (type == SEQUENCE_TYPE_PAIR && second != NULL) {
        _words += second->cstr();
    }
    */
    _type = type;
    _uv = 1;
}
inline bool Sequence:: operator == (const Sequence &seq) {
    if (_type != seq.type()) {
        return false;
    }
    return _words == seq.words();
}
/*
template <class char_type>
BaseChar *char_dynamic_create() {
    return new(std::nothrow) char_type();
}
*/
#endif
