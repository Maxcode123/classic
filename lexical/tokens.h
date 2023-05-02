#pragma once

#include "../utils/util.h"

typedef union {
    int pos;
    int ival;
    str sval;
} YYSTYPE;
extern YYSTYPE yylval;


# define PLUS 257  // +
# define MINUS 258  // -
# define TIMES 259  // *
# define DIV 260  // /
# define BACKSLASH 261  /* \ */
# define LPAREN 262  2// ()
# define RPAREN 263  // )
# define LBRACK 264  // {
# define RBRACK 265  // }
# define DOT 266  // .
# define DQUOTE 267  // "
# define EQUAL 268  // =
# define SEMICOLON 269  // ;
# define COLON 270  // :
# define ID 271  
# define NUM 272
# define NOTA 273
# define ADESM 274
# define DESM 275
# define INT 276
# define SERIES 277
# define EXEMP 278
# define OPER 279
# define ANEF 280
