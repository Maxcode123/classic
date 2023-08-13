#include "parse.h"

void yyerror(std::string s) {cerr << yylineno << ": " << pos << "  " << s << "\n";}
