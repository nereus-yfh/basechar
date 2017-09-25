#include "sequence.h"
#include "utf8_char.h"
#include "vector"
#include <ext/hash_map>
#include <ext/hash_set>
#include <iostream>

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

bool cmpseq(Sequence *a, Sequence *b) {
    return a->uv() > b->uv();
}
class Dict {
public :
    Dict();
    ~Dict();
public : 
    int create_dict(int filecount, char **filname, FILE *foutput);
};

Dict::Dict() {
}

Dict::~Dict() {
}

int Dict::create_dict(int filecount, char **filename, FILE *foutput) {
    size_t file_size = 0;
    std::vector<Sequence *> single_word;
    std::vector<Sequence *> pair_word;
    __gnu_cxx::hash_map<std::string, int> single_map;
    __gnu_cxx::hash_map<std::string, int> pair_map;
    for (int filei = 1; filei < filecount; filei++) {
        FILE * fp = fopen(filename[filei], "r");
        if (fp == NULL) {
            printf("there is no file[%s]", filename[filei]);
            return DICT_ERR;
        }
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
            printf("file %d %s read err\n", filei, filename[filei]);
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
        while (!feof(fp) && single_word.size() < 100000) {
            int errxxno = 0;
            if ((errxxno = ch->read(fp)) != CHAR_STAT_ERR) {
                a = ch->cstr();
            } else {
                printf("file %d %s read err[%d]\n", filei, filename[filei], errxxno);
                return DICT_ERR;
            }
            if (errxxno == CHAR_STAT_EOF) {
                break;
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
                    Sequence *seqb = new Sequence(SEQUENCE_TYPE_PAIR, b.c_str(), a.size());
                    pair_map[b] = pair_word.size();
                    pair_word.push_back(seqb);
                }
                pair_word[pair_map[b]]->add_uv();
                lb = b;
            } else {
                lb = "";
            }
            std::swap(ch, last_ch);
        }
        fclose(fp);

        std::sort(single_word.begin(), single_word.end(), cmpseq);
        std::sort(pair_word.begin(), pair_word.end(), cmpseq);
        
        __gnu_cxx::hash_set<std::string> single_used;
        
        for (int i = 0; i < pair_word.size(); i ++) {
            Sequence *seq = pair_word[i];
            const std::string &seq_word = seq->words();
            std::string first = std::string(seq_word, 0, seq->mid());
            std::string second = std::string(seq_word, seq->mid(), seq_word.size());
            if (single_used.find(first) != single_used.end() || single_used.find(second) != single_used.end()) {
                seq->set_type(SEQUENCE_TYPE_NOUSE);
            } else {
                single_used.insert(first);
                single_used.insert(second);
            }
        }

        for (int i = 0 ; i < pair_word.size(); i ++) {
            if(pair_word[i]->type() != SEQUENCE_TYPE_NOUSE) {
                std::cout << pair_word[i]->words() << "\n";
            }
        }

    }


}
