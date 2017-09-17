#include "utf8_char.h"
#include "sequence.h"

int main () {
    Utf8Char *utf8 = new Utf8Char();
    utf8->read(stdin);
    Utf8Char tmp(*utf8);
    Sequence *seq = new Sequence();
    seq->set_word(1, 0, utf8);
    printf("%s\n", utf8->cstr());
    char s[100];
    int ret = seq->cstr(s, 100);
    printf("%d    %s\n", ret, s);
    return 0;
}
