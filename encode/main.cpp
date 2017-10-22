#include "utf8_char.h"
#include "dict.h"
#include "sequence.h"

int main (int argc, char **argv) {
    if (argc == 1) {
        printf("Usage : \n./encode createdict input1 input2 input 3 > dict\n");
        printf("./encode encode < input > output\n\n");
        return 1;
    }
    if (strcmp(argv[1], "createdict") == 0) {
        Dict *dict = new Dict();
        dict->create_dict(argc - 1, argv+1 , stdout);
        return 1;
    }
    if (strcmp(argv[1], "encode") == 0) {
        Dict *dict = new Dict();
        FILE *fdict = fopen("dict", "r");
        dict->load_dict(fdict);
        dict->encode(stdin, stdout);
    }
    return 0;
}
