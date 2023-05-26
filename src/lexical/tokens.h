#pragma once

#include <string>
#include <algorithm>

using namespace std;


class LexemeItem {
    public:
        std::string name;
        int value;
        LexemeItem(std::string str, int val) {name = str; value = val;}
        LexemeItem(std::string str) : LexemeItem(str, 1) {}
        void set_value(int val);
};


// Pseudo enumeration of lexemes.
class Lexeme {
    public:
        inline static const LexemeItem OPER = LexemeItem("OPER");
        inline static const LexemeItem EXEMP = LexemeItem("EXEMP");
        inline static const LexemeItem INT_TYPE = LexemeItem("INT_TYPE");
        inline static const LexemeItem DUPL_TYPE = LexemeItem("DUPL_TYPE");
        inline static const LexemeItem STR_TYPE = LexemeItem("STR_TYPE");
        inline static const LexemeItem ANEF = LexemeItem("ANEF");
        inline static const LexemeItem EGO = LexemeItem("EGO");
        inline static const LexemeItem INITUS = LexemeItem("INITUS");
        inline static const LexemeItem EXODUS = LexemeItem("EXODUS");
        inline static const LexemeItem PLUS = LexemeItem("PLUS");
        inline static const LexemeItem MINUS = LexemeItem("MINUS");
        inline static const LexemeItem DIV = LexemeItem("DIV");
        inline static const LexemeItem TIMES = LexemeItem("TIMES");
        inline static const LexemeItem LPAREN = LexemeItem("LPAREN");
        inline static const LexemeItem RPAREN = LexemeItem("RPAREN");
        inline static const LexemeItem LBRACK = LexemeItem("LBRACK");
        inline static const LexemeItem RBRACK = LexemeItem("RBRACK");
        inline static const LexemeItem COLON = LexemeItem("COLON");
        inline static const LexemeItem SEMICOLON = LexemeItem("SEMICOLON");
        inline static const LexemeItem DOT = LexemeItem("DOT");
        inline static const LexemeItem COMMA = LexemeItem("COMMA");
        inline static const LexemeItem EQUAL = LexemeItem("EQUAL");
        inline static const LexemeItem ID = LexemeItem("ID");
        inline static const LexemeItem INT = LexemeItem("INT");
        inline static const LexemeItem DUPL = LexemeItem("DUPL");
        inline static const LexemeItem STR = LexemeItem("STR");

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
