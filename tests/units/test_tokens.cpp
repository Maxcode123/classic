#include <gtest/gtest.h>

#include "../../src/lexical/tokens.h"

TEST(TokenTest, testinit_no_args) {
    Token t = Token();
    EXPECT_TRUE(t.lexeme_str == "");    
    EXPECT_TRUE(t.semantval() == "");
}

TEST(TokenTest, testinit_lexeme) {
    Token t = Token("ID");
    EXPECT_TRUE(t.lexeme_str == "ID");
    EXPECT_TRUE(t.semantval() == "");
}

TEST(TokenTest, testinit_all_args) {
    Token t = Token("ID", "myvar");
    EXPECT_TRUE(t.lexeme_str == "ID");
    EXPECT_TRUE(t.semantval() == "myvar") << Lexeme::semantic[0];
}

TEST(TokenTest, testsemantval) {
    Token t = Token("ANEF", "value");
    EXPECT_TRUE(t.semantval() == "");
}

TEST(LexemeItemTest, testinit) {
    LexemeItem i = LexemeItem("smth", 10);
    EXPECT_TRUE(i.name == "smth");
}
