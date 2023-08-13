#pragma once

#include <algorithm>
#include <string>

#include "../syntax/classical.tab.h"

using namespace std;

class LexemeItem {
 public:
  std::string name;
  int value;
  LexemeItem(std::string str, int val) {
    name = str;
    value = val;
  }
  LexemeItem(std::string str) : LexemeItem(str, 1) {}
  void set_value(int val);
};

// Pseudo enumeration of lexemes.
class Lexeme {
 public:
  inline static const LexemeItem OPER = LexemeItem("OPER", _OPER);
  inline static const LexemeItem EXEMP = LexemeItem("EXEMP", _EXEMP);
  inline static const LexemeItem INT_TYPE = LexemeItem("INT_TYPE", _INT_TYPE);
  inline static const LexemeItem DUPL_TYPE =
      LexemeItem("DUPL_TYPE", _DUPL_TYPE);
  inline static const LexemeItem STR_TYPE = LexemeItem("STR_TYPE", _STR_TYPE);
  inline static const LexemeItem ANEF = LexemeItem("ANEF", _ANEF);
  inline static const LexemeItem EGO = LexemeItem("EGO", _EGO);
  inline static const LexemeItem INITUS = LexemeItem("INITUS", _INITUS);
  inline static const LexemeItem EXODUS = LexemeItem("EXODUS", _EXODUS);
  inline static const LexemeItem PLUS = LexemeItem("PLUS", _PLUS);
  inline static const LexemeItem MINUS = LexemeItem("MINUS", _MINUS);
  inline static const LexemeItem DIV = LexemeItem("DIV", _DIV);
  inline static const LexemeItem TIMES = LexemeItem("TIMES", _TIMES);
  inline static const LexemeItem LPAREN = LexemeItem("LPAREN", _LPAREN);
  inline static const LexemeItem RPAREN = LexemeItem("RPAREN", _RPAREN);
  inline static const LexemeItem LBRACK = LexemeItem("LBRACK", _LBRACK);
  inline static const LexemeItem RBRACK = LexemeItem("RBRACK", _RBRACK);
  inline static const LexemeItem COLON = LexemeItem("COLON", _COLON);
  inline static const LexemeItem SEMICOLON =
      LexemeItem("SEMICOLON", _SEMICOLON);
  inline static const LexemeItem DOT = LexemeItem("DOT", _DOT);
  inline static const LexemeItem COMMA = LexemeItem("COMMA", _COMMA);
  inline static const LexemeItem EQUAL = LexemeItem("EQUAL", _EQUAL);
  inline static const LexemeItem ID = LexemeItem("ID", _ID);
  inline static const LexemeItem INT = LexemeItem("INT", _INT);
  inline static const LexemeItem DUPL = LexemeItem("DUPL", _DUPL);
  inline static const LexemeItem STR = LexemeItem("STR", _STR);
  inline static const LexemeItem END = LexemeItem("EOF", YYEOF);

  static std::string semantic[];
};

class Token {
 public:
  std::string lexeme_str;
  std::string semantval();
  Token(std::string lexeme, std::string semant) {
    lexeme_str = lexeme;
    semant_str = semant;
  }
  Token(std::string lexeme) : Token(lexeme, "") {}
  Token() : Token("", "") {}

 private:
  std::string semant_str;
};
