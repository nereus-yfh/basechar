#include "utf8_char.h"
#include "dict.h"
#include "sequence.h"

int main () {
    Utf8Char *utf8 = new Utf8Char();
    utf8->read(stdin);
    //Utf8Char tmp(*utf8);
    return 0;
}
