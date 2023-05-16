#pragma once

#include <string>
#include <algorithm>

using namespace std;


class LexemeItem {
    public:
        std::string name;
        LexemeItem(std::string str, int val) {name = str; _value = val;}
        LexemeItem(std::string str) : LexemeItem(str, 1) {}
        int value() const;
    private:
        int _value;
};


// Pseudo enumeration of lexemes.
class Lexeme {
    public:
        inline static const LexemeItem OPER = LexemeItem("OPER", 1);
        inline static const LexemeItem EXEMP = LexemeItem("EXEMP", 2);
        inline static const LexemeItem INT_TYPE = LexemeItem("INT_TYPE", 3);
        inline static const LexemeItem DUPL_TYPE = LexemeItem("DUPL_TYPE", 4);
        inline static const LexemeItem STR_TYPE = LexemeItem("STR_TYPE", 5);
        inline static const LexemeItem ANEF = LexemeItem("ANEF", 6);
        inline static const LexemeItem EGO = LexemeItem("EGO", 7);
        inline static const LexemeItem INITUS = LexemeItem("INITUS", 8);
        inline static const LexemeItem EXODUS = LexemeItem("EXODUS", 9);
        inline static const LexemeItem PLUS = LexemeItem("PLUS", 10);
        inline static const LexemeItem MINUS = LexemeItem("MINUS", 11);
        inline static const LexemeItem DIV = LexemeItem("DIV", 12);
        inline static const LexemeItem TIMES = LexemeItem("TIMES", 13);
        inline static const LexemeItem LPAREN = LexemeItem("LPAREN", 14);
        inline static const LexemeItem RPAREN = LexemeItem("RPAREN", 15);
        inline static const LexemeItem LBRACK = LexemeItem("LBRACK", 16);
        inline static const LexemeItem RBRACK = LexemeItem("RBRACK", 17);
        inline static const LexemeItem COLON = LexemeItem("COLON", 18);
        inline static const LexemeItem SEMICOLON = LexemeItem("SEMICOLON", 19);
        inline static const LexemeItem DOT = LexemeItem("DOT", 20);
        inline static const LexemeItem COMMA = LexemeItem("COMMA", 21);
        inline static const LexemeItem EQUAL = LexemeItem("EQUAL", 22);
        inline static const LexemeItem ID = LexemeItem("ID", 23);
        inline static const LexemeItem INT = LexemeItem("INT", 24);
        inline static const LexemeItem DUPL = LexemeItem("DUPL", 25);
        inline static const LexemeItem STR = LexemeItem("STR", 26);

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


extern Token current_token;

static int lexeme_base = 0;

void set_lexeme_base(int b);