#include <gtest/gtest.h>

#include "../../../../src/syntax/ast/nodes.h"

TEST(NodesConstructorTest, TestParamConstructor) {
  Param p = new Param_(
      (new ClassicBuiltinType_(classic_builtin_types::INT))->upcast(), "myint");
  EXPECT_TRUE(p->classic_type->type == BUILTIN_TYPE);
  EXPECT_TRUE(p->name == "myint");
}

TEST(NodesConstructorTest, TestParamListConstructor) {
  LastParamList l = new LastParamList_(new Param_(
      (new ClassicBuiltinType_(classic_builtin_types::INT))->upcast(),
      "mystr"));
  EXPECT_TRUE(l->param->classic_type->type == BUILTIN_TYPE);
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

TEST(NodesTest, TestEqClassicTypeBuiltin) {
  ClassicType t1 =
      (new ClassicBuiltinType_(classic_builtin_types::INT))->upcast();
  ClassicType t2 =
      (new ClassicBuiltinType_(classic_builtin_types::INT))->upcast();
  EXPECT_TRUE(eq(t1, t2));
}

TEST(NodesTest, TestNEqClassicTypeBuiltin) {
  ClassicType t1 =
      (new ClassicBuiltinType_(classic_builtin_types::INT))->upcast();
  ClassicType t2 =
      (new ClassicBuiltinType_(classic_builtin_types::DUPL))->upcast();
  EXPECT_FALSE(eq(t1, t2));
}

TEST(NodesTest, TestNEqClassicType) {
  ClassicType t1 = (new ClassicCustomType_("myclass"))->upcast();
  ClassicType t2 =
      (new ClassicBuiltinType_(classic_builtin_types::DUPL))->upcast();
  EXPECT_FALSE(eq(t1, t2));
}

TEST(NodesTest, TestEqClassicTypeIndefinable) {
  ClassicType t1 = (new ClassicIndefinableType_())->upcast();
  ClassicType t2 = (new ClassicIndefinableType_())->upcast();
  EXPECT_TRUE(eq(t1, t2));
}

TEST(NodesTest, TestStrClassicTypeBuiltin) {
  ClassicType t =
      (new ClassicBuiltinType_(classic_builtin_types::STR))->upcast();
  EXPECT_EQ(str(t), "str");
}

TEST(NodesTest, TestStrClassicTypeCustom) {
  ClassicType t = (new ClassicCustomType_("Person"))->upcast();
  EXPECT_EQ(str(t), "Person");
}

TEST(NodesTest, TestStrClassicTypeIndefinable) {
  ClassicType t = (new ClassicIndefinableType_())->upcast();
  EXPECT_EQ(str(t), "indefinable");
}

class ParamListIteratorTest : public testing::Test {
 protected:
  Param build_param(classic_builtin_types::Type t = classic_builtin_types::INT,
                    std::string v = "myvar") {
    return new Param_((new ClassicBuiltinType_(t))->upcast(), v);
  }

  LastParamList build_last_param_list(
      classic_builtin_types::Type t = classic_builtin_types::INT,
      std::string v = "myvar") {
    return new LastParamList_(this->build_param(t, v));
  }
};

TEST_F(ParamListIteratorTest, TestIterateLastParamListIterations) {
  ParamList p = (new LastParamList_(this->build_param()))->upcast();
  int c = 0;
  for (auto it = ParamListIterator(p), end = ParamListIterator(nullptr);
       it != end; ++it) {
    c++;
  }
  EXPECT_EQ(c, 1);
}

TEST_F(ParamListIteratorTest, TestIteratePairParamListIterations) {
  ParamList p = (new PairParamList_(this->build_last_param_list()->upcast(),
                                    this->build_param()))
                    ->upcast();
  int c = 0;
  for (auto it = ParamListIterator(p), end = ParamListIterator(nullptr);
       it != end; ++it) {
    c++;
  }
  EXPECT_EQ(c, 2);
}

TEST_F(ParamListIteratorTest, TestIterateEmptyParamList) {
  ParamList p = (new EmptyParamList_())->upcast();
  EXPECT_EQ(ParamListIterator(p), ParamListIterator(nullptr));
}

TEST_F(ParamListIteratorTest, TestLastParamListParam) {
  LastParamList last =
      this->build_last_param_list(classic_builtin_types::DUPL, "dupl");
  ParamList p = last->upcast();

  ParamListIterator it = ParamListIterator(p);
  EXPECT_EQ(it->classic_type->downcast<ClassicBuiltinType>()->type,
            classic_builtin_types::DUPL);
  EXPECT_EQ(it->name, "dupl");
}

TEST_F(ParamListIteratorTest, TestPairParamListParams) {
  PairParamList pair = new PairParamList_(
      this->build_last_param_list(classic_builtin_types::STR, "mystr")
          ->upcast(),
      this->build_param(classic_builtin_types::INT, "myint"));
  PairParamList pair2 = new PairParamList_(
      pair->upcast(), this->build_param(classic_builtin_types::DUPL, "mydupl"));

  ParamList p = pair2->upcast();
  ParamListIterator it = ParamListIterator(p);

  EXPECT_EQ(it->name, "mydupl");
  it++;
  EXPECT_EQ(it->name, "myint");
  it++;
  EXPECT_EQ(it->name, "mystr");
}

TEST_F(ParamListIteratorTest, TestDerefenceEndIterator) {
  ParamListIterator it = ParamListIterator(nullptr);
  EXPECT_THROW(*it, IteratorError);
}

TEST_F(ParamListIteratorTest, TestReferenceEndIterator) {
  ParamListIterator it = ParamListIterator(nullptr);
  EXPECT_THROW(it->name, IteratorError);
}