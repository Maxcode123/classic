#include "tokens.h"

int LexemeItem::value() const {return _value + lexeme_base;}

std::string Lexeme::semantic[] = {
    Lexeme::ID.name,
    Lexeme::INT.name,
    Lexeme::DUPL.name,
    Lexeme::STR.name
    };

std::string Token::semantval() {
    if (std::find(std::begin(Lexeme::semantic), std::end(Lexeme::semantic), lexeme_str) != std::end(Lexeme::semantic)) {
        return semant_str;
    }
    return "";
}

void set_lexeme_base(int b) {lexeme_base = b;}