#include <stdio.h>
#include <stdlib.h>

#include "tokens.h"

YYSTYPE yylval;

extern FILE *yyin;

int pos = 0;

int yylex(void); /* prototype for the lexing function */

static str toknames[] = {
    "PLUS",
    "MINUS",
    "TIMES",
    "DIV",
    "ID",
    "NUM",
    "ASSIGN",
    "LPAREN",
    "RPAREN",
    "COMMA",
    "SEMICOLON",
    "PRINT"
};

str getname(int i) {
    return i<257 || i>269 ? "BAD TOKEN" : toknames[i-257];
}

void reset(str fname) {
    yyin = fopen(fname, "r");
    if (!yyin) {
        fprintf(stderr, "cannot open file\n");
        exit(1);
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: ./a.out filename\n");
        exit(1);
    }
    
    str fname;
    int i;

    fname = argv[1];
    reset(fname);

    for (;;) {
        i = yylex();
        if (i == 0) break;
        switch (i) {
            case ID:
            printf("%10s %4d %s\n", getname(i), pos, yylval.sval);
            break;
            case NUM:
            printf("%10s %4d %d\n", getname(i), pos, yylval.ival);
            break;
            default:
            printf("%10s %4d\n", getname(i), pos);
        }
    }
    return 0;
}