#pragma once

#include <iostream>
#include <string>

#include "../lexical/tokens.h"
#include "../lexical/scan.h"

using namespace std;

extern int yydebug;

int yyparse (void); // prototype for parsing function

void yyerror(std::string);  // prototype for error print function
