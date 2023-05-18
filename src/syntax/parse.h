#pragma once

#include <iostream>
#include <string>

#include "../lexical/scan.h"
#include "classical.tab.h"

using namespace std;

extern FILE *yyin;

int yyparse (void); // prototype for parsing function

void yyerror(std::string);