#include <gtest/gtest.h>

#include "../../../src/lexical/tokens.h"

TEST(TokenTest, TestInitNoArgs) {
  Token t = Token();
  EXPECT_TRUE(t.lexeme_str == "");
  EXPECT_TRUE(t.semantval() == "");
}

TEST(TokenTest, TestInitLexeme) {
  Token t = Token("ID");
  EXPECT_TRUE(t.lexeme_str == "ID");
  EXPECT_TRUE(t.semantval() == "");
}

TEST(TokenTest, TestInitAllArgs) {
  Token t = Token("ID", "myvar");
  EXPECT_TRUE(t.lexeme_str == "ID");
  EXPECT_TRUE(t.semantval() == "myvar") << Lexeme::semantic[0];
}

TEST(TokenTest, TestSemantval) {
  Token t = Token("ANEF", "value");
  EXPECT_TRUE(t.semantval() == "");
}

TEST(LexemeItemTest, TestInit) {
  LexemeItem i = LexemeItem("smth", 10);
  EXPECT_TRUE(i.name == "smth");
  EXPECT_TRUE(i.value == 10);
}
