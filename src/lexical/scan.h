#pragma once

#include <unordered_map>
#include <string>
#include <iostream>

#include "tokens.h"

using namespace std;

extern FILE *yyin;

Token yylval;

int pos = 0;

int yylex(void); /* prototype for the lexing function */
