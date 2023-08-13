#include "scan.h"


int main(int argc, char **argv) {
    pos = 0;

    if (argc != 2) {
        fprintf(stderr, "usage: ./a.out filename\n");
        exit(1);
    }
    
    yyin = fopen(argv[1], "r");
    int i;


    for (;;) {
        i = yylex();
        if (i == 0) break;
        std::cout << yylineno << ": " << pos << "  " << current_token.lexeme_str << "  " << current_token.semantval() << "\n";
    }
    return 0;
}
