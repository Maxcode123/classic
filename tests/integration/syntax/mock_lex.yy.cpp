#include "mock_lex..yy.h"

int yylex(void) {
  SemanticLexeme semantic = lexemes->front();
  current_token = Token(semantic.lexeme.name, semantic.semantic_value);
  lexemes->pop_front();
  return semantic.lexeme.value;
}
