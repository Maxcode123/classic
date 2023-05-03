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
# define LPAREN 262  // ()
# define RPAREN 263  // )
# define LBRACK 264  // {
# define RBRACK 265  // }
# define DOT 266  // .
# define DQUOTE 267  // "
# define EQUAL 268  // =
# define SEMICOLON 269  // ;
# define COLON 270  // :
# define COMMA 271 // ,

# define ID 290  
# define NUM 291

# define NOTA 310
# define ADESM 311
# define DESM 312
# define INT 313
# define SERIES 314
# define EXEMP 315
# define OPER 316
# define ANEF 317
