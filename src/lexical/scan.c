#include "scan.h"


int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: ./a.out filename\n");
        exit(1);
    }
    
    tokenmapinit();
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

HashTable tokenmapinit() {
    tokenmap = ht_init(30);
    ht_set(tokenmap, "257", "PLUS");
    ht_set(tokenmap, "258", "MINUS");
    ht_set(tokenmap, "259", "TIMES");
    ht_set(tokenmap, "260", "DIV");
    ht_set(tokenmap, "261", "BACKSLASH");
    ht_set(tokenmap, "262", "LPAREN");
    ht_set(tokenmap, "263", "RPAREN");
    ht_set(tokenmap, "264", "LBRACK");
    ht_set(tokenmap, "265", "RBRACK");
    ht_set(tokenmap, "266", "DOT");
    ht_set(tokenmap, "267", "DQUOTE");
    ht_set(tokenmap, "268", "EQUAL");
    ht_set(tokenmap, "269", "SEMICOLON");
    ht_set(tokenmap, "270", "COLON");
    ht_set(tokenmap, "271", "COMMA");

    ht_set(tokenmap, "290", "ID");
    ht_set(tokenmap, "291", "NUM");

    ht_set(tokenmap, "310", "NOTA");
    ht_set(tokenmap, "311", "ADESM");
    ht_set(tokenmap, "312", "DESM");
    ht_set(tokenmap, "313", "INT");
    ht_set(tokenmap, "314", "SERIES");
    ht_set(tokenmap, "315", "EXEMP");
    ht_set(tokenmap, "316", "OPER");
    ht_set(tokenmap, "317", "ANEF");
}

str getname(int i) {
    if (i<257 || i > 317) return "BAD TOKEN";
    char token[20];
    sprintf(token, "%d", i);
    return ht_get(tokenmap, token);
}

void reset(str fname) {
    yyin = fopen(fname, "r");
    if (!yyin) {
        fprintf(stderr, "cannot open file\n");
        exit(1);
    }
}