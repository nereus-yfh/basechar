#include "sequence.h"
#include "utf8_char.h"
#include "vector"
#include <ext/hash_map>

enum _dict_enum {
    DICT_SUCC = 0,
    DICT_ERR,
};
namespace __gnu_cxx
{
    template<> struct hash<const std::string>
    {
        size_t operator()(const std::string& s) const
        { return hash<const char*>()( s.c_str() ); } 
    };
    template<> struct hash<std::string>
    {
        size_t operator()(const std::string& s) const
        { return hash<const char*>()( s.c_str() ); }
    };
}

class Dict {
public :
    Dict();
    ~Dict();
public : 
    int create(int filecount, char **filname, FILE *foutput);
};

Dict::Dict() {
}

Dict::~Dict() {
}

int Dict::create(int filecount, char **filename, FILE *foutput) {
    size_t file_size = 0;
    std::vector<Sequence *> single_word;
    std::vector<Sequence *> pair_word;
    __gnu_cxx::hash_map<std::string, int> single_map;
    __gnu_cxx::hash_map<std::string, int> pair_map;
    for (int filei = 0; filei < filecount; filei++) {
        FILE * fp = fopen(filename[filei], "r");
        if (fp == NULL) {
            printf("there is no file[%s]", filename[filei]);
            return DICT_ERR;
        }
        printf("file %d %s", filei, filename[filei]);
        fseek(fp, 0, SEEK_END);
        file_size += ftell(fp);
        fseek(fp, 0, SEEK_SET);
        Utf8Char* ch = new Utf8Char();
        Utf8Char* last_ch = new Utf8Char();
        std::string b, lb;
        std::string a;
        if (ch->read(fp) == CHAR_STAT_SUCC) {
            a = ch->cstr();
        } else {
            return DICT_ERR;
        }
        if (single_map.find(a) == single_map.end()) {
            Sequence *seqa = new Sequence(SEQUENCE_TYPE_SINGLE, a.c_str());
            single_map[a] = single_word.size(); 
            single_word.push_back(seqa);
        }
        single_word[single_map[a]]->add_uv();
        lb = ch->cstr();
        std::swap(ch, last_ch);
        while (!feof(fp)) {
            if (ch->read(fp) == CHAR_STAT_SUCC) {
                a = ch->cstr();
            } else {
                return DICT_ERR;
            }
            if (single_map.find(a) == single_map.end()) {
                Sequence *seqa = new Sequence(SEQUENCE_TYPE_SINGLE, a.c_str());
                single_map[a] = single_word.size(); 
                single_word.push_back(seqa);
            }
            single_word[single_map[a]]->add_uv();
            b = a + last_ch->cstr();
            if (lb != b) {
                if(pair_map.find(b) == pair_map.end()) {
                    Sequence *seqb = new Sequence(SEQUENCE_TYPE_PAIR, b.c_str());
                    pair_map[b] = pair_word.size();
                    pair_word.push_back(seqb);
                }
                pair_word[pair_map[b]]->add_uv();
                lb = b;
            } else {
                lb = "";
            }
        }
        fclose(fp);
    }


}
