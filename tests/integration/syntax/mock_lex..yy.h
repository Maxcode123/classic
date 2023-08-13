#pragma once

#include <list>

#include "../../../src/lexical/tokens.h"
#include "../../../src/syntax/ast/builder.h"
#include "../../../src/syntax/ast/nodes.h"

typedef struct SemanticLexeme {
  LexemeItem lexeme;
  std::string semantic_value;
} SemanticLexeme;

extern AST ast;
extern ASTBuilder builder;
extern ASTStackProxy proxy;
extern int counter;
extern int pos;
extern int yylineno;
extern Token current_token;
extern std::list<SemanticLexeme> *lexemes;

int yylex(void);