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
    SEQUENCE_TYPE_NOUSE,
};
//template <class CharType>
class Sequence {
public:
    Sequence();
    ~Sequence();
    Sequence(int type,const char *s, int mid) ;
    
public:
    inline bool operator == (const Sequence& seq);
    inline bool operator < (const Sequence& seq) {return _uv*_words.size() > seq.uv() * seq.size();}
    inline size_t uv() const {return _uv;}
    inline size_t size() const {return _words.size();}
    inline int type() const {return _type;}
    inline int mid() const {return _mid;}
    inline const char* cstr() const {return _words.c_str();}
    inline const std::string& words() const {return _words;}
    inline void add_uv() {_uv++;}
    inline void set_type(int type) {_type = type;}

    //inline BaseChar *words(size_t id) const {return _words[id];}
    //int set_word(int type, size_t id, BaseChar *word);
private:
    std::string _words;
    int _type;
    int _mid;
    size_t _uv;
};

Sequence :: Sequence() {
    _uv = 0;
    _type = SEQUENCE_TYPE_NONE;
}

Sequence :: ~Sequence() {
}

Sequence :: Sequence(int type, const char *s, int mid = 0) {
    _words = s;
    /*
    if (type == SEQUENCE_TYPE_PAIR && second != NULL) {
        _words += second->cstr();
    }
    */
    _mid = mid;
    _type = type;
    _uv = 0;
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
