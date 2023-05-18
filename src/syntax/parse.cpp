#include "parse.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: ./a.out filename\n");
        exit(1);
    }
    
    yyin = fopen(argv[1], "r");
    
    yyparse();
    
    return 0;
}

void yyerror(std::string s) {cerr << yylineno << ": " << pos << "  " << s;}