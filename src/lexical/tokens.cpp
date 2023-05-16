#include "tokens.h"

std::string Lexeme::semantic[] = {Lexeme::ID, Lexeme::INT, Lexeme::DUPL, Lexeme::STR};

std::string Token::semantval() {
    if (std::find(std::begin(Lexeme::semantic), std::end(Lexeme::semantic), lexeme_str) != std::end(Lexeme::semantic)) {
        return semant_str;
    }
    return "";
}