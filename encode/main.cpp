#include "utf8_char.h"
#include "dict.h"
#include "sequence.h"

int main (int argc, char **argv) {
    //Utf8Char tmp(*utf8);
    Dict *dict = new Dict();
    dict->create_dict(argc, argv, stdout);
    return 0;
}
