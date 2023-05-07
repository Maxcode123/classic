#pragma once

#include <string>
#include <algorithm>

#include "../utils/util.h"

using namespace std;


// Pseudo enumeration of lexemes.
class Lexeme {
    public:
        inline static const std::string PLUS = "PLUS";
        inline static const std::string MINUS = "MINUS";
        inline static const std::string TIMES = "TIMES";
        inline static const std::string DIV = "DIV";
        inline static const std::string BACKSLASH = "BACKSLASH";
        inline static const std::string LPAREN = "LPAREN";
        inline static const std::string RPAREN = "RPAREN";
        inline static const std::string LBRACK = "LBRACK";
        inline static const std::string RBRACK = "RBRACK";
        inline static const std::string DOT = "DOT";
        inline static const std::string DQUOTE = "DQUOTE";
        inline static const std::string EQUAL = "EQUAL";
        inline static const std::string SEMICOLON = "SEMICOLON";
        inline static const std::string COLON = "COLON";
        inline static const std::string COMMA = "COMMA";
        inline static const std::string ID = "ID";
        inline static const std::string NUM_I = "NUM_I";
        inline static const std::string NUM_D = "NUM_D";
        inline static const std::string STR = "STR";
        inline static const std::string NOTA = "NOTA";
        inline static const std::string ADESM = "ADESM";
        inline static const std::string DESM = "DESM";
        inline static const std::string INT = "INT";
        inline static const std::string DUPL = "DUPL";
        inline static const std::string SERIES = "SERIES";
        inline static const std::string EXEMP = "EXEMP";
        inline static const std::string OPER = "OPER";
        inline static const std::string ANEF = "ANEF";
        inline static const std::string RETURN = "RETURN";

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


extern Token yylval;
