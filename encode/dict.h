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
//hash函数
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
//字符对排序比较函数
bool cmpseq(Sequence *a, Sequence *b) {
    return a->uv()*a->size() > b->uv() * b->size();
}

//dict类，主要用于字典生成和加密
class Dict {
public :
    Dict();
    ~Dict();
public : 
    int create_dict(int filecount, char **filname, FILE *foutput);
    int load_dict(FILE *dict_file);
    int encode(FILE *dict, FILE *fin, FILE *fout);
    int encode(FILE *fin, FILE *fout);
private:
    __gnu_cxx::hash_map<std::string, std::string> _single_2_pair;
    __gnu_cxx::hash_map<std::string, std::string> _pair_2_single;
};

Dict::Dict() {
}

Dict::~Dict() {
}

//生成字典
int Dict::create_dict(int filecount, char **filename, FILE *foutput) {
    size_t file_size = 0;
    std::vector<Sequence *> single_word;
    std::vector<Sequence *> pair_word;
    __gnu_cxx::hash_map<std::string, int> single_map;
    __gnu_cxx::hash_map<std::string, int> pair_map;

    //遍历文件读取单个字符和字符对
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
    }
    //根据字符对长度和频率排序
    std::sort(single_word.begin(), single_word.end(), cmpseq);
    std::sort(pair_word.begin(), pair_word.end(), cmpseq);
    
    __gnu_cxx::hash_set<std::string> single_used;
    
    //替换并打印字典
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
    for (int i = 0; i < pair_word.size(); i++) {
        bool found = false;
        Sequence *seq = pair_word[i];
        if (seq->type() == SEQUENCE_TYPE_NOUSE) {
            continue;
        }
        std::string temps;
        for (int j = single_word.size() - 1;j >= 0; j--) {
            Sequence *single_seq = single_word[j];
            if (single_seq->type() == SEQUENCE_TYPE_NOUSE) {
                continue;
            }
            if (single_used.find(single_seq->words()) != single_used.end()) {
                continue;
            }
            if (single_seq->size() * single_seq->uv() < seq->size() * seq->uv()) {
                temps += single_seq->words();;
                found = true;
                single_used.insert(single_seq->words());
                break;
            }
        }
        if (found) {
            fwrite(temps.c_str(), temps.size(), 1, foutput);
            fwrite(seq->words().c_str(), seq->size(), 1, foutput);
        }
    }
        
    return 0;
}

//字典加载
int Dict::load_dict(FILE *fp) {
    _single_2_pair.clear();
    _pair_2_single.clear();
    Utf8Char* ch = new Utf8Char();
    Utf8Char* chleft = new Utf8Char();
    std::string single, pair;
    while(!feof(fp)) {
        if (ch->read(fp) == CHAR_STAT_SUCC) {
            single = ch->cstr();
        } else {
            return DICT_ERR;
        }
        if (ch->read(fp) != CHAR_STAT_SUCC) {
            return DICT_ERR;
        }
        if (chleft->read(fp) != CHAR_STAT_SUCC) {
            return DICT_ERR;
        }
        pair = chleft->cstr();
        pair += ch -> cstr();
        //printf("%d ", single[0]);
        //printf("%d ", pair[0]);
        //printf("%d \n", pair[1]);
        //std::cout << single << "   " << pair << std::endl;
        _single_2_pair[single] = pair;
        _pair_2_single[pair] = single;
    }
}

int Dict::encode(FILE *dict, FILE *fin, FILE *fout) {
    load_dict(dict);
    encode(fin, fout);
}

//压缩
int Dict::encode(FILE *fin, FILE *fout) {
    int state = 0;
    std::string a, b;
    Utf8Char* ch = new Utf8Char();
    while (!feof(fin)) {
        int errxxno = 0;
        if ((errxxno = ch->read(fin)) != CHAR_STAT_ERR) {
            a = ch->cstr();
        } else {
            return DICT_ERR;
        }
        if (errxxno == CHAR_STAT_EOF) {
            if (state == 1) {
                fwrite(b.c_str(), b.size(), 1, fout);
            }
            break;
        }
        if (state == 0) {
            if (_single_2_pair.find(a) != _single_2_pair.end()) {
                std::string outbuf = _single_2_pair[a];
                fwrite(outbuf.c_str(), outbuf.size(), 1, fout);
                b.clear();
                state = 0;
            } else {
                b = a;
                state = 1;
            }
        } else {
            std::string pair = b + a;
            if (_pair_2_single.find(pair) != _pair_2_single.end()) {
                std::string outbuf = _pair_2_single[pair];
                fwrite(outbuf.c_str(), outbuf.size(), 1, fout);
                b.clear();
                state = 0;
            } else {
                fwrite(b.c_str(), b.size(), 1, fout);
                if (_single_2_pair.find(a) != _single_2_pair.end()) {
                    std::string outbuf = _single_2_pair[a];
                    int buflen = outbuf.size();
                    if (outbuf.substr(0, buflen/2) == outbuf.substr(buflen/2, buflen - buflen/2) 
                        && outbuf.substr(0, buflen/2) == b) {
                        fwrite(a.c_str(), a.size(), 1, fout);
                    } else {
                        fwrite(outbuf.c_str(), buflen, 1, fout);
                    }
                    b.clear();
                    state = 0;
                } else {
                    state = 1;
                    b = a;
                }
            }
        }
    }
    return DICT_SUCC; 
}
