#pragma once

#include <unordered_map>
#include <string>
#include <iostream>

#include "tokens.h"

using namespace std;

extern FILE *yyin;

extern int yylineno;

extern Token current_token;

extern int pos;

int yylex(void); /* prototype for the lexing function */
