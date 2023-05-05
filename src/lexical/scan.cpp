#include "scan.h"


int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: ./a.out filename\n");
        exit(1);
    }
    
    tokenmapinit();
    yyin = fopen(argv[1], "r");
    int i;


    for (;;) {
        i = yylex();
        if (i == 0) break;
        std::cout << tokenmap.find(i)->second << " " << pos << "\n";
        // switch (i) {
        //     case ID:
             
        //     break;
        //     case NUM_I:
        //     std:cout << tokenmap.find(i)->second << " " << pos << "\n";
        //     case NUM_D:
        //     printf("%10s %4d %f\n", tokenmap.find(i), pos, yylval.dval);
        //     break;
        //     default:
        //     printf("%10s %4d\n", tokenmap.find(i), pos);
        // }
    }
    return 0;
}

void tokenmapinit() {
    tokenmap = {
        {257, "PLUS"},
        {258, "MINUS"},
        {259, "TIMES"},
        {260, "DIV"},
        {261, "BACKSLASH"},
        {262, "LPAREN"},
        {263, "RPAREN"},
        {264, "LBRACK"},
        {265, "RBRACK"},
        {266, "DOT"},
        {267, "DQUOTE"},
        {268, "EQUAL"},
        {269, "SEMICOLON"},
        {270, "COLON"},
        {271, "COMMA"},
        {290, "ID"},
        {291, "NUM_I"},
        {292, "NUM_D"},
        {310, "NOTA"},
        {311, "ADESM"},
        {312, "DESM"},
        {313, "INT"},
        {314, "SERIES"},
        {315, "EXEMP"},
        {316, "OPER"},
        {317, "ANEF"},
        {318, "RETURN"}
    };
}
