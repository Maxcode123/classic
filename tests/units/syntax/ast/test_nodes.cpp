#include <gtest/gtest.h>

#include "../../../../src/syntax/ast/nodes.h"

TEST(NodesConstructorTest, TestParamConstructor) {
  Param p = new Param_("INT", "myint");
  EXPECT_TRUE(p->type == "INT");
  EXPECT_TRUE(p->name == "myint");
}

TEST(NodesConstructorTest, TestParamListConstructor) {
  LastParamList l = new LastParamList_(new Param_("str", "mystr"));
  EXPECT_TRUE(l->param->type == "str");
  EXPECT_TRUE(l->param->name == "mystr");
}

TEST(NodesTest, TestFunctionListDowncastPairFunctionList) {
  PairFunctionList pair_list =
      new PairFunctionList_(new FunctionList_(), new Function_());
  FunctionList func_list = new FunctionList_(pair_list);
  EXPECT_EQ(func_list->downcast<PairFunctionList>(), pair_list);
}

TEST(NodesTest, TestFunctionListDowncastLastFunctionList) {
  LastFunctionList last_list = new LastFunctionList_(new Function_());
  FunctionList func_list = new FunctionList_(last_list);
  EXPECT_EQ(func_list->downcast<LastFunctionList>(), last_list);
}

TEST(NodesTest, TestStatementDowncastCompoundStatement) {
  CompoundStatement cmp =
      new CompoundStatement_(new EmptyStatement_(), new EmptyStatement_());
  Statement stm = new Statement_(cmp);
  EXPECT_EQ(stm->downcast<CompoundStatement>(), cmp);
}

TEST(NodesTest, TestStatementDowncastAssignStatement) {
  AssignStatement asgn = new AssignStatement_("a", new LiteralExpression_(2.5));
  Statement stm = new Statement_(asgn);
  EXPECT_EQ(stm->downcast<AssignStatement>(), asgn);
}

TEST(NodesTest, TestStatementDowncastExpressionStatement) {
  ExpressionStatement ex_stm =
      new ExpressionStatement_(new LiteralExpression_("str"));
  Statement stm = new Statement_(ex_stm);
  EXPECT_EQ(stm->downcast<ExpressionStatement>(), ex_stm);
}

TEST(NodesTest, TestStatementDowncastEmptyStatement) {
  EmptyStatement empty = new EmptyStatement_();
  Statement stm = new Statement_(empty);
  EXPECT_EQ(stm->downcast<EmptyStatement>(), empty);
}

TEST(NodesTest, TestCompoundStatementUpcast) {
  CompoundStatement cmp =
      new CompoundStatement_(new EmptyStatement_(), new EmptyStatement_());
  Statement stm = cmp->upcast();
  EXPECT_EQ(stm->type, COMPOUND_STATEMENT);
}

TEST(NodesTest, TestAssignStatementUpcast) {
  AssignStatement asgn = new AssignStatement_("a", new LiteralExpression_(0));
  Statement stm = asgn->upcast();
  EXPECT_EQ(stm->type, ASSIGN_STATEMENT);
}

TEST(NodesTest, TestExpressionStatementUpcast) {
  ExpressionStatement exp_stm =
      new ExpressionStatement_(new VariableExpression_("b"));
  Statement stm = exp_stm->upcast();
  EXPECT_EQ(stm->type, EXPRESSION_STATEMENT);
}

TEST(NodesTest, TestEmptyStatementUpcast) {
  EmptyStatement empty = new EmptyStatement_();
  Statement stm = empty->upcast();
  EXPECT_EQ(stm->type, EMPTY_STATEMENT);
}
