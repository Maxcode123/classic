#include <gtest/gtest.h>

#include "../../../src/lexical/tokens.h"
#include "../../../src/syntax/ast/builder.h"
#include "../../../src/syntax/ast/errors.h"
#include "../../../src/syntax/ast/nodes.h"
#include "../../../src/syntax/parse.h"
#include "../../../src/utils/error_handler.h"
#include "mock_lex..yy.h"

AST ast;
ASTBuilder builder;
ErrorHandler<ASTBuildError> handler;
ASTStackProxy proxy;
int counter;
int pos;
int yylineno;
Token current_token;
std::list<SemanticLexeme>* lexemes;

void update_ast() { ast.update(proxy.top()); }

class ParserTest : public testing::Test {
  void SetUp() override {
    ast = AST();
    proxy = ASTStackProxy(new ASTStack_());
    builder = ASTBuilder(proxy);
    handler = ErrorHandler<ASTBuildError>();
    counter = 0;
    pos = 0;
    // yydebug = 1;
  }
};

/* oper: int initus() { exodus 0; } */
TEST_F(ParserTest, TestMinimalInitus) {
  lexemes = new std::list<SemanticLexeme>{
      {Lexeme::OPER, ""},      {Lexeme::COLON, ""},  {Lexeme::INT_TYPE, ""},
      {Lexeme::INITUS, ""},    {Lexeme::LPAREN, ""}, {Lexeme::RPAREN, ""},
      {Lexeme::LBRACK, ""},    {Lexeme::EXODUS, ""}, {Lexeme::INT, "0"},
      {Lexeme::SEMICOLON, ""}, {Lexeme::RBRACK, ""}, {Lexeme::END, ""},
  };
  EXPECT_EQ(yyparse(), 0);

  LastFunctionList f = ast.get<FunctionList>()->downcast<LastFunctionList>();

  EXPECT_EQ(f->function->name, "initus");
  EXPECT_EQ(f->function->return_type->type, BUILTIN_TYPE);
  EXPECT_NE(f->function->body->statement->downcast<EmptyStatement>(), nullptr);
  EXPECT_NE(f->function->param_list->downcast<EmptyParamList>(), nullptr);
}

/*
oper: int add(int a, int b) { exodus (f + g); }
oper: int subt(int a, int b) { exodus (c - e); }
oper: int initus() { exodus func(arg1=f1(q=10, p=2), arg2=f2(i=6, j=2)); }
 */
TEST_F(ParserTest, TestFunctionsProgram) {
  lexemes = new std::list<SemanticLexeme>{
      {Lexeme::OPER, ""},      {Lexeme::COLON, ""},  {Lexeme::INT_TYPE, ""},
      {Lexeme::ID, "add"},     {Lexeme::LPAREN, ""}, {Lexeme::INT_TYPE, ""},
      {Lexeme::ID, "a"},       {Lexeme::COMMA, ""},  {Lexeme::INT_TYPE, ""},
      {Lexeme::ID, "b"},       {Lexeme::RPAREN, ""}, {Lexeme::LBRACK, ""},
      {Lexeme::EXODUS, ""},    {Lexeme::LPAREN, ""}, {Lexeme::ID, "f"},
      {Lexeme::PLUS, ""},      {Lexeme::ID, "g"},    {Lexeme::RPAREN, ""},
      {Lexeme::SEMICOLON, ""}, {Lexeme::RBRACK, ""},

      {Lexeme::OPER, ""},      {Lexeme::COLON, ""},  {Lexeme::INT_TYPE, ""},
      {Lexeme::ID, "subt"},    {Lexeme::LPAREN, ""}, {Lexeme::INT_TYPE, ""},
      {Lexeme::ID, "a"},       {Lexeme::COMMA, ""},  {Lexeme::INT_TYPE, ""},
      {Lexeme::ID, "b"},       {Lexeme::RPAREN, ""}, {Lexeme::LBRACK, ""},
      {Lexeme::EXODUS, ""},    {Lexeme::LPAREN, ""}, {Lexeme::ID, "c"},
      {Lexeme::MINUS, ""},     {Lexeme::ID, "e"},    {Lexeme::RPAREN, ""},
      {Lexeme::SEMICOLON, ""}, {Lexeme::RBRACK, ""},

      {Lexeme::OPER, ""},      {Lexeme::COLON, ""},  {Lexeme::INT_TYPE, ""},
      {Lexeme::INITUS, ""},    {Lexeme::LPAREN, ""}, {Lexeme::RPAREN, ""},
      {Lexeme::LBRACK, ""},    {Lexeme::EXODUS, ""}, {Lexeme::ID, "func"},
      {Lexeme::LPAREN, ""},    {Lexeme::ID, "arg1"}, {Lexeme::EQUAL},
      {Lexeme::ID, "f1"},      {Lexeme::LPAREN, ""}, {Lexeme::ID, "q"},
      {Lexeme::EQUAL, ""},     {Lexeme::INT, "10"},  {Lexeme::COMMA, ""},
      {Lexeme::ID, "p"},       {Lexeme::EQUAL, ""},  {Lexeme::INT, "2"},
      {Lexeme::RPAREN, ""},    {Lexeme::COMMA, ""},  {Lexeme::ID, "arg2"},
      {Lexeme::EQUAL, ""},     {Lexeme::ID, "f2"},   {Lexeme::LPAREN, ""},
      {Lexeme::ID, "i"},       {Lexeme::EQUAL, ""},  {Lexeme::INT, "6"},
      {Lexeme::COMMA, ""},     {Lexeme::ID, "j"},    {Lexeme::EQUAL, ""},
      {Lexeme::INT, "2"},      {Lexeme::RPAREN, ""}, {Lexeme::RPAREN, ""},
      {Lexeme::SEMICOLON, ""}, {Lexeme::RBRACK, ""}, {Lexeme::END, ""},
  };
  EXPECT_EQ(yyparse(), 0);

  PairFunctionList f = ast.get<FunctionList>()->downcast<PairFunctionList>();
  EXPECT_NE(f, nullptr);
  EXPECT_EQ(f->function->name, "initus");
  EXPECT_EQ(f->next->type, PAIR_FUNCTION_LIST);

  PairFunctionList f2 = f->next->downcast<PairFunctionList>();
  EXPECT_EQ(f2->function->name, "subt");
  EXPECT_EQ(f2->next->type, LAST_FUNCTION_LIST);

  LastFunctionList f3 = f2->next->downcast<LastFunctionList>();
  EXPECT_EQ(f3->function->name, "add");
}

/*
oper: anef void(dupl void, dupl void) {
  a = 1;
  b = 2;
  c = anef;
  exodus (void + void);
}
oper: int initus() { exodus void(); }
*/
TEST_F(ParserTest, TestCompoundStatements) {
  lexemes = new std::list<SemanticLexeme>{
      {Lexeme::OPER, ""},      {Lexeme::COLON, ""},     {Lexeme::ANEF, ""},
      {Lexeme::ID, "void"},    {Lexeme::LPAREN, ""},    {Lexeme::DUPL_TYPE, ""},
      {Lexeme::ID, "void"},    {Lexeme::COMMA, ""},     {Lexeme::DUPL_TYPE, ""},
      {Lexeme::ID, "void"},    {Lexeme::RPAREN, ""},    {Lexeme::LBRACK, ""},
      {Lexeme::ID, "a"},       {Lexeme::EQUAL, ""},     {Lexeme::INT, "1"},
      {Lexeme::SEMICOLON, ""}, {Lexeme::ID, "b"},       {Lexeme::EQUAL, ""},
      {Lexeme::INT, "2"},      {Lexeme::SEMICOLON, ""}, {Lexeme::ID, "c"},
      {Lexeme::EQUAL, ""},     {Lexeme::ANEF, ""},      {Lexeme::SEMICOLON, ""},
      {Lexeme::EXODUS, ""},    {Lexeme::LPAREN, ""},    {Lexeme::ID, "void"},
      {Lexeme::PLUS, ""},      {Lexeme::ID, "void"},    {Lexeme::RPAREN, ""},
      {Lexeme::SEMICOLON, ""}, {Lexeme::RBRACK},

      {Lexeme::OPER, ""},      {Lexeme::COLON, ""},     {Lexeme::INT_TYPE, ""},
      {Lexeme::INITUS, ""},    {Lexeme::LPAREN, ""},    {Lexeme::RPAREN, ""},
      {Lexeme::LBRACK, ""},    {Lexeme::EXODUS, ""},    {Lexeme::ID, "void"},
      {Lexeme::LPAREN, ""},    {Lexeme::RPAREN, ""},    {Lexeme::SEMICOLON, ""},
      {Lexeme::RBRACK, ""},    {Lexeme::END, ""},
  };
  EXPECT_EQ(yyparse(), 0);

  PairFunctionList f = ast.get<FunctionList>()->downcast<PairFunctionList>();
  EXPECT_NE(f, nullptr);
  EXPECT_EQ(f->next->type, LAST_FUNCTION_LIST);

  Function voidf = f->next->downcast<LastFunctionList>()->function;
  EXPECT_EQ(voidf->body->statement->type, COMPOUND_STATEMENT);

  CompoundStatement body_stm =
      voidf->body->statement->downcast<CompoundStatement>();
  EXPECT_EQ(body_stm->first_statement->type, COMPOUND_STATEMENT);
  EXPECT_EQ(body_stm->second_statement->type, ASSIGN_STATEMENT);

  CompoundStatement first_stm =
      body_stm->first_statement->downcast<CompoundStatement>();
  EXPECT_EQ(first_stm->first_statement->type, ASSIGN_STATEMENT);
  EXPECT_EQ(first_stm->second_statement->type, ASSIGN_STATEMENT);
}

/*
oper: MyCustomType whatever() {
  amount = get_amount();
  cost = get_cost();
  tax = call_one(one=1) - call_two(two=2.3) - call_three(three=3);
  side_effect();
  (another_side_effect());
  exodus 2.54;
}
oper: int initus() { whatever(); whatever(); exodus 0.01; }
*/
TEST_F(ParserTest, TestSideEffectExpressions) {
  lexemes = new std::list<SemanticLexeme>{
      {Lexeme::OPER, ""},           {Lexeme::COLON, ""},
      {Lexeme::ID, "MyCustomType"}, {Lexeme::ID, "whatever"},
      {Lexeme::LPAREN, ""},         {Lexeme::RPAREN, ""},
      {Lexeme::LBRACK, ""},

      {Lexeme::ID, "amount"},       {Lexeme::EQUAL, ""},
      {Lexeme::ID, "get_amount"},   {Lexeme::LPAREN, ""},
      {Lexeme::RPAREN, ""},         {Lexeme::SEMICOLON, ""},

      {Lexeme::ID, "cost"},         {Lexeme::EQUAL, ""},
      {Lexeme::ID, "get_cost"},     {Lexeme::LPAREN, ""},
      {Lexeme::RPAREN, ""},         {Lexeme::SEMICOLON, ""},

      {Lexeme::ID, "tax"},          {Lexeme::EQUAL, ""},
      {Lexeme::ID, "call_one"},     {Lexeme::LPAREN, ""},
      {Lexeme::ID, "one"},          {Lexeme::EQUAL, ""},
      {Lexeme::INT, "1"},           {Lexeme::RPAREN, ""},
      {Lexeme::MINUS, ""},          {Lexeme::ID, "call_two"},
      {Lexeme::LPAREN, ""},         {Lexeme::ID, "two"},
      {Lexeme::EQUAL, ""},          {Lexeme::DUPL, "2.3"},
      {Lexeme::RPAREN, ""},         {Lexeme::MINUS, ""},
      {Lexeme::ID, "call_three"},   {Lexeme::LPAREN, ""},
      {Lexeme::ID, "three"},        {Lexeme::EQUAL, ""},
      {Lexeme::INT, "3"},           {Lexeme::RPAREN, ""},
      {Lexeme::SEMICOLON, ""},

      {Lexeme::ID, "side_effect"},  {Lexeme::LPAREN, ""},
      {Lexeme::RPAREN, ""},         {Lexeme::SEMICOLON, ""},

      {Lexeme::LPAREN, ""},         {Lexeme::ID, "another_side_effect"},
      {Lexeme::LPAREN, ""},         {Lexeme::RPAREN, ""},
      {Lexeme::RPAREN, ""},         {Lexeme::SEMICOLON, ""},

      {Lexeme::EXODUS, ""},         {Lexeme::DUPL, "2.54"},
      {Lexeme::SEMICOLON, ""},      {Lexeme::RBRACK, ""},

      {Lexeme::OPER, ""},           {Lexeme::COLON, ""},
      {Lexeme::INT_TYPE, ""},       {Lexeme::INITUS, ""},
      {Lexeme::LPAREN, ""},         {Lexeme::RPAREN, ""},
      {Lexeme::LBRACK, ""},         {Lexeme::ID, "whatever"},
      {Lexeme::LPAREN, ""},         {Lexeme::RPAREN, ""},
      {Lexeme::SEMICOLON, ""},      {Lexeme::ID, "whatever"},
      {Lexeme::SEMICOLON, ""},      {Lexeme::EXODUS, ""},
      {Lexeme::DUPL, "0.01"},       {Lexeme::SEMICOLON, ""},
      {Lexeme::RBRACK, ""},         {Lexeme::END, ""},
  };
  EXPECT_EQ(yyparse(), 0);

  // PairFunctionList f = ast.get<FunctionList>()->downcast<PairFunctionList>();
  // EXPECT_EQ(f->next->type, LAST_FUNCTION_LIST);

  // Function whatever = f->next->downcast<LastFunctionList>()->function;
  // EXPECT_EQ(whatever->body->statement->type, COMPOUND_STATEMENT);

  // CompoundStatement last_stm =
  //     whatever->body->statement->downcast<CompoundStatement>();
  // EXPECT_EQ(last_stm->second_statement->type, EXPRESSION_STATEMENT);
  // EXPECT_EQ(last_stm->first_statement->type, COMPOUND_STATEMENT);

  // Expression last_exp =
  //     last_stm->second_statement->downcast<ExpressionStatement>()->expression;
  // EXPECT_EQ(last_exp->type, PARENTHESES_EXPRESSION);

  // CompoundStatement penult_stm =
  //     last_stm->first_statement->downcast<CompoundStatement>();
  // EXPECT_EQ(penult_stm->second_statement->type, EXPRESSION_STATEMENT);

  // Expression penult_exp =
  //     penult_stm->second_statement->downcast<ExpressionStatement>()->expression;
  // EXPECT_EQ(penult_exp->type, FUNCTION_CALL_EXPRESSION);
}

/*
oper: int initus() { a; b; c; exodus -1; }
 */
TEST_F(ParserTest, TestVariableExpressions) {
  lexemes = new std::list<SemanticLexeme>{
      {Lexeme::OPER, ""},      {Lexeme::COLON, ""},     {Lexeme::INT_TYPE, ""},
      {Lexeme::INITUS, ""},    {Lexeme::LPAREN, ""},    {Lexeme::RPAREN, ""},
      {Lexeme::LBRACK, ""},    {Lexeme::ID, "a"},       {Lexeme::SEMICOLON, ""},
      {Lexeme::ID, "b"},       {Lexeme::SEMICOLON, ""}, {Lexeme::ID, "c"},
      {Lexeme::SEMICOLON, ""}, {Lexeme::EXODUS, ""},    {Lexeme::INT, "-1"},
      {Lexeme::SEMICOLON, ""}, {Lexeme::RBRACK, ""},    {Lexeme::END, ""},
  };
  EXPECT_EQ(yyparse(), 0);

  LastFunctionList f = ast.get<FunctionList>()->downcast<LastFunctionList>();
  EXPECT_EQ(f->function->body->statement->type, COMPOUND_STATEMENT);

  CompoundStatement cmp_stm =
      f->function->body->statement->downcast<CompoundStatement>();
  EXPECT_EQ(cmp_stm->second_statement->type, EXPRESSION_STATEMENT);
  EXPECT_EQ(cmp_stm->first_statement->type, COMPOUND_STATEMENT);

  ExpressionStatement third_stm =
      cmp_stm->second_statement->downcast<ExpressionStatement>();
  EXPECT_EQ(third_stm->expression->type, VARIABLE_EXPRESSION);

  CompoundStatement f_cmp_stm =
      cmp_stm->first_statement->downcast<CompoundStatement>();
  EXPECT_EQ(f_cmp_stm->second_statement->type, EXPRESSION_STATEMENT);
  EXPECT_EQ(f_cmp_stm->first_statement->type, EXPRESSION_STATEMENT);

  ExpressionStatement second_stm =
      f_cmp_stm->second_statement->downcast<ExpressionStatement>();
  EXPECT_EQ(second_stm->expression->type, VARIABLE_EXPRESSION);

  ExpressionStatement first_stm =
      f_cmp_stm->first_statement->downcast<ExpressionStatement>();
  EXPECT_EQ(first_stm->expression->type, VARIABLE_EXPRESSION);
}