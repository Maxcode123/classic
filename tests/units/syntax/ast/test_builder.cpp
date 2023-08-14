#include <gtest/gtest.h>

#include "../../../../src/syntax/ast/builder.h"
#include "../../../../src/syntax/ast/errors.h"

class TestASTNode_ : virtual public ASTNode_ {
 public:
  std::string cls() { return "test"; }
  TestASTNode_(){};
};

class ASTStackTest : public testing::Test {};

TEST(ASTStackTest, TestPush) {
  ASTStack_ stack = ASTStack_();
  stack.push(new TestASTNode_());
  EXPECT_EQ(stack.stack.size(), 1);
}

TEST(ASTStackTest, TestMultiplePush) {
  ASTStack_ stack = ASTStack_();
  for (int i = 0; i < 5; i++) stack.push(new TestASTNode_());
  EXPECT_EQ(stack.stack.size(), 5);
}

TEST(ASTStackTest, TestPop) {
  ASTStack_ stack = ASTStack_();
  stack.stack.push(new TestASTNode_());
  stack.pop();
  EXPECT_EQ(stack.stack.size(), 0);
}

class ASTStackProxyTest : public testing::Test {};

class ASTBuilderTest : public testing::Test {
 protected:
  ASTBuilder builder;
  ASTStack stack;
  void SetUp() override {
    this->stack = new ASTStack_();
    this->builder = ASTBuilder(ASTStackProxy(this->stack));
  }
  void push(std::string str) { this->stack->push(new TerminalASTNode_(str)); }
  void push(classic_builtin_types::Type t) {
    this->stack->push((new ClassicBuiltinType_(t))->upcast());
  }
  void push(ASTNode n) { this->stack->push(n); }
  template <class T_base, class T>
  T pop() {
    return (dynamic_cast<T_base>(this->stack->pop()))->template downcast<T>();
  }
  template <class T>
  T pop() {
    return dynamic_cast<T>(this->stack->pop());
  }
};

#define EXPECT_AST_BUILD_ERROR(expression) \
  EXPECT_THROW(expression, ASTBuildError)

#define BUILD_ARGUMENT(arg_name, type, val) \
  this->push(arg_name);                     \
  type *exp = new type(val);                \
  this->push(exp);                          \
  Argument arg = this->builder.build_argument();

#define TEST_BUILD_ARGUMENT_BUILDS(arg_name, type, val) \
  BUILD_ARGUMENT(arg_name, type, val);                  \
  EXPECT_EQ(arg->name, arg_name);                       \
  EXPECT_EQ(exp, dynamic_cast<type *>(arg->expression));

#define TEST_BUILD_ARGUMENT_PUSHES(arg_name, type, val) \
  BUILD_ARGUMENT(arg_name, type, val);                  \
  EXPECT_EQ(arg, this->pop<Argument>());

#define CREATE_BUILD_ARGUMENT_TESTS(test_name, arg_name, type, val)     \
  TEST_F(ASTBuilderTest, test_name){TEST_BUILD_ARGUMENT_BUILDS(         \
      arg_name, type, val)} TEST_F(ASTBuilderTest, test_name##Pushes) { \
    TEST_BUILD_ARGUMENT_PUSHES(arg_name, type, val)                     \
  }

CREATE_BUILD_ARGUMENT_TESTS(TestBuildArgumentVariableExpression, "myarg",
                            VariableExpression_, "myvar");

CREATE_BUILD_ARGUMENT_TESTS(TestBuildArgumentLiteralExpressionInt, "int_arg",
                            LiteralExpression_, 192);

CREATE_BUILD_ARGUMENT_TESTS(TestBuildArgumentLiteralExpressionDupl, "dupl_arg",
                            LiteralExpression_, 10.76);

CREATE_BUILD_ARGUMENT_TESTS(TestBuildArgumentLiteralExpressionStr, "str_arg",
                            LiteralExpression_, "literal_string");

TEST_F(ASTBuilderTest, TestBuildArgumentRaisesEmptyStack) {
  EXPECT_THROW({ this->builder.build_argument(); }, ASTBuildError);
}

TEST_F(ASTBuilderTest, TestBuildArgumentRaisesNoTerminal) {
  this->push(new LiteralExpression_(2312));
  EXPECT_AST_BUILD_ERROR({ this->builder.build_argument(); });
}

TEST_F(ASTBuilderTest, TestBuildArgumentRaisesInvalidTerminal) {
  this->push(new LiteralExpression_(12321));
  this->push(new LiteralExpression_(1930));
  EXPECT_AST_BUILD_ERROR({ this->builder.build_argument(); });
}

TEST_F(ASTBuilderTest, TestBuildVariableExpression) {
  VariableExpression exp = this->builder.build_variable_expression("myvar");
  EXPECT_EQ(exp->var_name, "myvar");
}

TEST_F(ASTBuilderTest, TestBuildVariableExpressionPushes) {
  VariableExpression exp = this->builder.build_variable_expression("myvar");
  EXPECT_EQ(exp, (this->pop<Expression, VariableExpression>()));
}

TEST_F(ASTBuilderTest, TestBuildLastArgumentList) {
  Argument arg = new Argument_("my_arg", new LiteralExpression_(10));
  this->push(arg);
  LastArgumentList arg_list = this->builder.build_last_argument_list();
  EXPECT_EQ(arg_list->argument, arg);
}

TEST_F(ASTBuilderTest, TestBuildLastArgumentListPushes) {
  Argument arg = new Argument_("my_arg", new LiteralExpression_(10));
  this->push(arg);
  LastArgumentList arg_list = this->builder.build_last_argument_list();
  EXPECT_EQ(arg_list, (this->pop<ArgumentList, LastArgumentList>()));
}

TEST_F(ASTBuilderTest, TestBuildLastArgumentListInvalidArg) {
  this->push(new LiteralExpression_(11));
  EXPECT_AST_BUILD_ERROR({ this->builder.build_last_argument_list(); });
}

TEST_F(ASTBuilderTest, TestBuildLastArgumentListEmptyStack) {
  EXPECT_AST_BUILD_ERROR({ this->builder.build_last_argument_list(); });
}

TEST_F(ASTBuilderTest, TestBuildPairArgumentList) {
  LastArgumentList last_list = new LastArgumentList_(new Argument_());
  this->push(last_list);
  Argument arg = new Argument_();
  this->push(arg);

  PairArgumentList arg_list = this->builder.build_pair_argument_list();

  EXPECT_EQ(arg_list->next, last_list);
  EXPECT_EQ(arg_list->argument, arg);
}

TEST_F(ASTBuilderTest, TestBuildPairArgumentListPushes) {
  LastArgumentList last_list = new LastArgumentList_(new Argument_());
  this->push(last_list);
  Argument arg = new Argument_();
  this->push(arg);

  PairArgumentList arg_list = this->builder.build_pair_argument_list();

  EXPECT_EQ(arg_list, (this->pop<ArgumentList, PairArgumentList>()));
}

TEST_F(ASTBuilderTest, TestBuildPairArgumentListInvalidArgument) {
  this->push(new LastArgumentList_(new Argument_()));
  EXPECT_AST_BUILD_ERROR(this->builder.build_pair_argument_list());
}

TEST_F(ASTBuilderTest, TestBuildPairArgumentListInvalidArgumentList) {
  this->push(new Argument_());
  this->push(new Argument_());
  EXPECT_AST_BUILD_ERROR(this->builder.build_pair_argument_list());
}

TEST_F(ASTBuilderTest, TestBuildPairArgumentListEmptyStack) {
  EXPECT_AST_BUILD_ERROR(this->builder.build_pair_argument_list());
}

TEST_F(ASTBuilderTest, TestBuildPairArgumentListArgumentListEmptyStack) {
  this->push(new Argument_());
  EXPECT_AST_BUILD_ERROR(this->builder.build_pair_argument_list());
}

TEST_F(ASTBuilderTest, TestBuildFunctionCallExpression) {
  this->push("my_func");
  LastArgumentList arg_list = new LastArgumentList_(new Argument_());
  this->push(arg_list);

  FunctionCallExpression exp = this->builder.build_function_call_expression();

  EXPECT_EQ(exp->function_name, "my_func");
  EXPECT_EQ(exp->argument_list, arg_list);
}

TEST_F(ASTBuilderTest, TestBuildFunctionCallExpressionPushes) {
  this->push("my_func");
  this->push(new LastArgumentList_(new Argument_()));

  FunctionCallExpression exp = this->builder.build_function_call_expression();

  EXPECT_EQ(exp, (this->pop<Expression, FunctionCallExpression>()));
}

TEST_F(ASTBuilderTest, TestBuildFunctionCallExpressionInvalidArgumentList) {
  this->push("not an arg list");

  EXPECT_AST_BUILD_ERROR(this->builder.build_function_call_expression());
}

TEST_F(ASTBuilderTest, TestBuildFunctionCallExpressionInvalidFunctionName) {
  this->push(new Argument_());  // not a function name terminal node
  this->push(new LastArgumentList_(new Argument_()));

  EXPECT_AST_BUILD_ERROR(this->builder.build_function_call_expression());
}

TEST_F(ASTBuilderTest, TestBuildFunctionCallExpressionEmptyStack) {
  EXPECT_AST_BUILD_ERROR(this->builder.build_function_call_expression());
}

TEST_F(ASTBuilderTest, TestBuildFunctionCallExpressionFunctionNameEmptyStack) {
  this->push(new LastArgumentList_(new Argument_()));

  EXPECT_AST_BUILD_ERROR(this->builder.build_function_call_expression());
}

TEST_F(ASTBuilderTest, TestBuildBinaryOperationExpression) {
  LiteralExpression left = new LiteralExpression_(5);
  LiteralExpression right = new LiteralExpression_(10);
  this->push(left);
  this->push(new BinaryOperatorASTNode_(BINARY_MINUS));
  this->push(right);

  BinaryOperationExpression exp =
      this->builder.build_binary_operation_expression();

  EXPECT_EQ(exp->left_expression, left);
  EXPECT_EQ(exp->right_expression, right);
}

TEST_F(ASTBuilderTest, TestBuildBinaryOperationExpressionPushes) {
  this->push(new LiteralExpression_(100));
  this->push(new BinaryOperatorASTNode_(BINARY_PLUS));
  this->push(new LiteralExpression_(-100));

  BinaryOperationExpression exp =
      this->builder.build_binary_operation_expression();

  EXPECT_EQ(exp, (this->pop<Expression, BinaryOperationExpression>()));
}

TEST_F(ASTBuilderTest,
       TestBuildBinaryOperationExpressionInvalidRightExpression) {
  this->push("not an expression");

  EXPECT_AST_BUILD_ERROR(this->builder.build_binary_operation_expression());
}

TEST_F(ASTBuilderTest, TestBuildBinaryOperationExpressionInvalidBinop) {
  this->push("not a binop");
  this->push(new VariableExpression_("var_exp"));

  EXPECT_AST_BUILD_ERROR(this->builder.build_binary_operation_expression());
}

TEST_F(ASTBuilderTest,
       TestBuildBinaryOperationExpressionInvalidLeftExpression) {
  this->push("not an epxression");
  this->push(new BinaryOperatorASTNode_(BINARY_DIV));
  this->push(new LiteralExpression_(1));

  EXPECT_AST_BUILD_ERROR(this->builder.build_binary_operation_expression());
}

TEST_F(ASTBuilderTest, TestBuildBinaryOperationExpressionEmptyStack) {
  EXPECT_AST_BUILD_ERROR(this->builder.build_binary_operation_expression());
}

TEST_F(ASTBuilderTest,
       TestBuildBinaryOperationExpressionRightExpressionEmptyStack) {
  this->push(new LiteralExpression_(10.1));  // valid right expression

  EXPECT_AST_BUILD_ERROR(this->builder.build_binary_operation_expression());
}

TEST_F(ASTBuilderTest, TesBuildParenthesesExpression) {
  VariableExpression exp = new VariableExpression_("myvar");
  this->push(exp);

  ParenthesesExpression paren_exp =
      this->builder.build_parentheses_expression();

  EXPECT_EQ(paren_exp->expression, exp);
}

TEST_F(ASTBuilderTest, TesBuildParenthesesExpressionPushes) {
  this->push(new VariableExpression_("myvar"));

  ParenthesesExpression paren_exp =
      this->builder.build_parentheses_expression();

  EXPECT_EQ(paren_exp, (this->pop<Expression, ParenthesesExpression>()));
}

TEST_F(ASTBuilderTest, TesBuildParenthesesExpressionInvalidExpression) {
  this->push(new Argument_());  // not an expression
  EXPECT_AST_BUILD_ERROR(this->builder.build_parentheses_expression());
}

TEST_F(ASTBuilderTest, TesBuildParenthesesExpressionEmptyStack) {
  EXPECT_AST_BUILD_ERROR(this->builder.build_parentheses_expression());
}

TEST_F(ASTBuilderTest, TestBuildExodusStatement) {
  VariableExpression exp = new VariableExpression_("myvar");
  this->push(exp);

  ExodusStatement exodus = this->builder.build_exodus_statement();

  EXPECT_EQ(exodus->expression, exp);
}

TEST_F(ASTBuilderTest, TestBuildExodusStatementPushes) {
  this->push(new VariableExpression_("myvar"));

  ExodusStatement exodus = this->builder.build_exodus_statement();

  EXPECT_EQ(exodus, this->pop<ExodusStatement>());
}

TEST_F(ASTBuilderTest, TestBuildExodusStatementInvalidExpression) {
  this->push(new Argument_());  // not an expression
  EXPECT_AST_BUILD_ERROR(this->builder.build_exodus_statement());
}

TEST_F(ASTBuilderTest, TestBuildExodusStatementEmptyStack) {
  EXPECT_AST_BUILD_ERROR(this->builder.build_exodus_statement());
}

TEST_F(ASTBuilderTest, TestBuildEmptyStatement) {
  EmptyStatement stm = this->builder.build_empty_statement();
  EXPECT_EQ(stm, (this->pop<Statement, EmptyStatement>()));
}

TEST_F(ASTBuilderTest, TestBuildExpressionStatement) {
  LiteralExpression exp = new LiteralExpression_(10.2);
  this->push(exp);

  ExpressionStatement stm = this->builder.build_expression_statement();

  EXPECT_EQ(stm->expression, exp);
}

TEST_F(ASTBuilderTest, TestBuildExpressionStatementPushes) {
  this->push(new LiteralExpression_(2));

  ExpressionStatement stm = this->builder.build_expression_statement();

  EXPECT_EQ(stm, (this->pop<Statement, ExpressionStatement>()));
}

TEST_F(ASTBuilderTest, TestBuildExpressionStatementInvalidExpression) {
  this->push(new Argument_());  // not an expression
  EXPECT_AST_BUILD_ERROR(this->builder.build_expression_statement());
}

TEST_F(ASTBuilderTest, TestBuildExpressionStatementEmptyStack) {
  EXPECT_AST_BUILD_ERROR(this->builder.build_expression_statement());
}

TEST_F(ASTBuilderTest, TestBuildAssignStatement) {
  this->push("a");
  LiteralExpression rhs = new LiteralExpression_(9);
  this->push(rhs);

  AssignStatement stm = this->builder.build_assign_statement();

  EXPECT_EQ(stm->lhs_id, "a");
  EXPECT_EQ(stm->rhs_expression, rhs);
}

TEST_F(ASTBuilderTest, TestBuildAssignStatementPushes) {
  this->push("b");
  this->push(new LiteralExpression_(8));

  AssignStatement stm = this->builder.build_assign_statement();

  EXPECT_EQ(stm, (this->pop<Statement, AssignStatement>()));
}

TEST_F(ASTBuilderTest, TestBuildAssignStatementInvalidExpression) {
  this->push("c");              // valid terminal node lhs id
  this->push(new Argument_());  // not an expression
  EXPECT_AST_BUILD_ERROR(this->builder.build_assign_statement());
}

TEST_F(ASTBuilderTest, TestBuildAssignStatementInvalidTerminalNode) {
  this->push(new Argument_());             // invalid terminal node lhs id
  this->push(new LiteralExpression_(10));  // valid expression
  EXPECT_AST_BUILD_ERROR(this->builder.build_assign_statement());
}

TEST_F(ASTBuilderTest, TestBuildAssignStatementEmptyStack) {
  EXPECT_AST_BUILD_ERROR(this->builder.build_assign_statement());
}

TEST_F(ASTBuilderTest, TestBuildAssignStatementExpressionEmptyStack) {
  this->push("d");
  EXPECT_AST_BUILD_ERROR(this->builder.build_assign_statement());
}

TEST_F(ASTBuilderTest, TestBuildCompoundStatement) {
  ExpressionStatement first =
      new ExpressionStatement_(new VariableExpression_("a"));
  this->push(first);
  ExpressionStatement second =
      new ExpressionStatement_(new VariableExpression_("b"));
  this->push(second);

  CompoundStatement stm = this->builder.build_compound_statement();

  EXPECT_EQ(stm->first_statement, first);
  EXPECT_EQ(stm->second_statement, second);
}

TEST_F(ASTBuilderTest, TestBuildCompoundStatementPushes) {
  this->push(new ExpressionStatement_(new LiteralExpression_(2)));
  this->push(new ExpressionStatement_(new LiteralExpression_(20)));

  CompoundStatement stm = this->builder.build_compound_statement();

  EXPECT_EQ(stm, (this->pop<Statement, CompoundStatement>()));
}

TEST_F(ASTBuilderTest, TestBuildCompoundStatementInvalidFirstStatement) {
  this->push(new Argument_());  // not a valid statement
  this->push(new ExpressionStatement_(new LiteralExpression_(1)));

  EXPECT_AST_BUILD_ERROR(this->builder.build_compound_statement());
}

TEST_F(ASTBuilderTest, TestBuildCompoundStatementInvalidSecondStatement) {
  this->push(new Argument_());  // not a valid expression

  EXPECT_AST_BUILD_ERROR(this->builder.build_compound_statement());
}

TEST_F(ASTBuilderTest, TestBuildCompoundStatementEmptyStack) {
  EXPECT_AST_BUILD_ERROR(this->builder.build_compound_statement());
}

TEST_F(ASTBuilderTest, TestBuildCompoundStatementSecondExpressionEmptyStack) {
  this->push(new ExpressionStatement_(new LiteralExpression_(1)));

  EXPECT_AST_BUILD_ERROR(this->builder.build_compound_statement());
}

TEST_F(ASTBuilderTest, TestBuildParam) {
  this->push(classic_builtin_types::INT);
  this->push("counter");

  Param p = this->builder.build_param();

  EXPECT_EQ(p->classic_type->type, BUILTIN_TYPE);
  EXPECT_EQ(p->name, "counter");
}

TEST_F(ASTBuilderTest, TestBuildParamPushes) {
  this->push(classic_builtin_types::INT);
  this->push("counter");

  Param p = this->builder.build_param();

  EXPECT_EQ(p, this->pop<Param>());
}

TEST_F(ASTBuilderTest, TestBuildParamInvalidFirstTerminalNode) {
  this->push(classic_builtin_types::DUPL);
  this->push(new Argument_());  // invalid terminal node 'name'

  EXPECT_AST_BUILD_ERROR(this->builder.build_param());
}

TEST_F(ASTBuilderTest, TestBuildParamInvalidSecondTerminalNode) {
  this->push(new Argument_());  // invalid terminal node 'type'
  this->push("cost");

  EXPECT_AST_BUILD_ERROR(this->builder.build_param());
}

TEST_F(ASTBuilderTest, TestBuildParamEmptyStack) {
  EXPECT_AST_BUILD_ERROR(this->builder.build_param());
}

TEST_F(ASTBuilderTest, TestBuildParamFirstTerminalNodeEmptyStack) {
  this->push("i");
  EXPECT_AST_BUILD_ERROR(this->builder.build_param());
}

TEST_F(ASTBuilderTest, TestBuildEmptyParamList) {
  EmptyParamList param_list = this->builder.build_empty_param_list();

  EXPECT_EQ(param_list, (this->pop<ParamList, EmptyParamList>()));
}

// TEST_F(ASTBuilderTest, TestBuildEmptyParamListPushes) {}

// TEST_F(ASTBuilderTest, TestBuildLastParamList) {}

// TEST_F(ASTBuilderTest, TestBuildLastParamListPushes) {}

// TEST_F(ASTBuilderTest, TestBuildLastParamListInvalidParam) {}

// TEST_F(ASTBuilderTest, TestBuildLastParamListEmptyStack) {}

// TEST_F(ASTBuilderTest, TestBuildPairParamList) {}

// TEST_F(ASTBuilderTest, TestBuildPairParamListPushes) {}

// TEST_F(ASTBuilderTest, TestBuildPairParamListInvalidParamList) {}

// TEST_F(ASTBuilderTest, TestBuildPairParamListInvalidParam) {}

// TEST_F(ASTBuilderTest, TestBuildPairParamListEmptyStack) {}

// TEST_F(ASTBuilderTest, TestBuildPairParamListParamListEmptyStack) {}

// TEST_F(ASTBuilderTest, TestBuildFunctionBody) {}

// TEST_F(ASTBuilderTest, TestBuildFunctionBodyPushes) {}

// TEST_F(ASTBuilderTest, TestBuildFunctionBodyInvalidStatement) {}

// TEST_F(ASTBuilderTest, TestBuildFunctionBodyInvalidExodusStatement) {}

// TEST_F(ASTBuilderTest, TestBuildFunctionBodyEmptyStack) {}

// TEST_F(ASTBuilderTest, TestBuildFunctionBodyStatementEmptyStack) {}

// TEST_F(ASTBuilderTest, TestBuildInitusFunction) {}

// TEST_F(ASTBuilderTest, TestBuildInitusFunctionPushes) {}

// TEST_F(ASTBuilderTest, TestBuildInitusFunctionInvalidFunctionBody) {}

// TEST_F(ASTBuilderTest, TestBuildInitusFunctionEmptyStack) {}

// TEST_F(ASTBuilderTest, TestBuildFunction) {}

// TEST_F(ASTBuilderTest, TestBuildFunctionPushes) {}

// TEST_F(ASTBuilderTest, TestBuildFunctionInvalidExodusTypeTerminalNode) {}

// TEST_F(ASTBuilderTest, TestBuildFunctionInvalidFunctionNameTerminalNode) {}

// TEST_F(ASTBuilderTest, TestBuildFunctionInvalidParamList) {}

// TEST_F(ASTBuilderTest, TestBuildFunctionInvalidFunctionBody) {}

// TEST_F(ASTBuilderTest, TestBuildFunctionEmptyStack) {}

// TEST_F(ASTBuilderTest, TestBuildFunctionExodusTypeTerminalNodeEmptyStack) {}

// TEST_F(ASTBuilderTest, TestBuildFunctionFunctionNameTerminalNodeEmptyStack)
// {}

// TEST_F(ASTBuilderTest, TestBuildFunctionParamListEmptyStack) {}

// TEST_F(ASTBuilderTest, TestBuildFunctionFunctionBodyEmptyStack) {}

// TEST_F(ASTBuilderTest, TestBuildLastFunctionList) {}

// TEST_F(ASTBuilderTest, TestBuildLastFunctionListPushes) {}

// TEST_F(ASTBuilderTest, TestBuildLastFunctionListInvalidFunction) {}

// TEST_F(ASTBuilderTest, TestBuildLastFunctionListEmptyStack) {}

// TEST_F(ASTBuilderTest, TestBuildPairFunctionList) {}

// TEST_F(ASTBuilderTest, TestBuildPairFunctionListPushes) {}

// TEST_F(ASTBuilderTest, TestBuildPairFunctionListInvalidFunctionList) {}

// TEST_F(ASTBuilderTest, TestBuildPairFunctionListInvalidFunction) {}

// TEST_F(ASTBuilderTest, TestBuildPairFunctionListEmptyStack) {}

// TEST_F(ASTBuilderTest, TestBuildPairFunctionListFunctionListEmptyStack) {}