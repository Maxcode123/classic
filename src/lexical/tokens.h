#pragma once

#include <string>
#include <algorithm>

using namespace std;


// Pseudo enumeration of lexemes.
class Lexeme {
    public:
        inline static const std::string OPER = "OPER";
        inline static const std::string EXEMP = "EXEMP";
        inline static const std::string INT_TYPE = "INT_TYPE";
        inline static const std::string DUPL_TYPE = "DUPL_TYPE";
        inline static const std::string STR_TYPE = "STR_TYPE";
        inline static const std::string ANEF = "ANEF";
        inline static const std::string EGO = "EGO";
        inline static const std::string INITUS = "INITUS";
        inline static const std::string EXODUS = "EXODUS";
        inline static const std::string PLUS = "PLUS";
        inline static const std::string MINUS = "MINUS";
        inline static const std::string DIV = "DIV";
        inline static const std::string TIMES = "TIMES";
        inline static const std::string LPAREN = "LPAREN";
        inline static const std::string RPAREN = "RPAREN";
        inline static const std::string LBRACK = "LBRACK";
        inline static const std::string RBRACK = "RBRACK";
        inline static const std::string COLON = "COLON";
        inline static const std::string SEMICOLON = "SEMICOLON";
        inline static const std::string DOT = "DOT";
        inline static const std::string COMMA = "COMMA";
        inline static const std::string EQUAL = "EQUAL";
        inline static const std::string ID = "ID";
        inline static const std::string INT = "INT";
        inline static const std::string DUPL = "DUPL";
        inline static const std::string STR = "STR";

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
