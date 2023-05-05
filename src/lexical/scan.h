#pragma once

#include <unordered_map>
#include <string>
#include <iostream>

#include "tokens.h"

using namespace std;

YYSTYPE yylval;

extern FILE *yyin;

int pos = 0;

int yylex(void); /* prototype for the lexing function */

static std::unordered_map<int, std::string> tokenmap;

// Creates and assigns tokenmap.
void tokenmapinit();
