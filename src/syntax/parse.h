#pragma once

#include <iostream>
#include <string>

#include "classical.tab.h"
#include "../lexical/tokens.h"

using namespace std;

extern FILE *yyin;

extern int yylineno;

extern int pos;

extern Token current_token;

int yyparse (void); // prototype for parsing function

void yyerror(std::string);  // prototype for error print function

void set_lexeme_values();
