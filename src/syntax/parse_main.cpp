#include "parse.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: ./a.out filename\n");
        exit(1);
    }
    
    yyin = fopen(argv[1], "r");
    
    set_lexeme_values();
    yyparse();
    
    return 0;
}
