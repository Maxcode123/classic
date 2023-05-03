#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "tokens.h"
#include "../utils/hashtable.h"

YYSTYPE yylval;

extern FILE *yyin;

int pos = 0;

int yylex(void); /* prototype for the lexing function */

static HashTable tokenmap;

// Creates and assigns tokenmap.
HashTable tokenmapinit();

// map int value to token str
str getname(int i);

// set fname as lex input
void reset(str fname);
