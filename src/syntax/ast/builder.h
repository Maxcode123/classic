#pragma once

#include <gtest/gtest.h>

#include "../debug.h"
#include "nodes.h"

/* Facade of the Abstract Syntax Tree. Use this class to retrieve the AST built
 * during parsing.*/
class AST {
 public:
  template <class T>
  T get() {
    return dynamic_cast<T>(root);
  }
  void update(ASTNode n) { root = n; }
  AST() {}

 private:
  ASTNode root;
};

// macro needed for accessing private attributes of tested class
#define TEST_AST_STACK                         \
  friend class ASTStackTest;                   \
  FRIEND_TEST(ASTStackTest, TestPush);         \
  FRIEND_TEST(ASTStackTest, TestMultiplePush); \
  FRIEND_TEST(ASTStackTest, TestPop);

// macro needed for accessing private attributes of tested class
#define TEST_AST_STACK_PROXY                          \
  friend class ASTStackProxyTest;                     \
  FRIEND_TEST(ASTStackProxyTest, TestGetInitializes); \
  FRIEND_TEST(ASTStackProxyTest, TestGetReturnsSameObject);

/* Stack where AST nodes are stored during parsing of the program. Do not
 * instantiate this class directly, use ASTStackProxy rather. */
typedef class ASTStack_ {
  TEST_AST_STACK
 public:
  void push(ASTNode n);
  ASTNode pop();
  int size() { return this->stack.size(); }
  ASTNode top() { return this->stack.top(); }
  ASTStack_(){};

 private:
  std::stack<ASTNode> stack;
} *ASTStack;

/* Proxy of ASTStack. ASTStack should only be accessed/created throught this
 * class. */
class ASTStackProxy {
  TEST_AST_STACK_PROXY
 public:
  void push(ASTNode n) { this->stack->push(n); }
  ASTNode pop() { return this->stack->pop(); }
  int size() { return this->stack->size(); }
  ASTNode top() { return this->stack->top(); }
  ASTStackProxy(ASTStack stk) { stack = stk; }
  ASTStackProxy() { stack = new ASTStack_(); }

 private:
  ASTStack stack;
};

#define THROW_AST_BUILD_ERROR_EMPTY_STACK(type, expected)                  \
  do {                                                                     \
    std::string msg =                                                      \
        "Cannot build " type "; AST stack is empty and does not contain "; \
    msg += expected;                                                       \
    msg += ".";                                                            \
    throw ASTBuildError(msg);                                              \
  } while (0)

#define THROW_AST_BUILD_ERROR_INVALID_AST_NODE(type, expected)              \
  do {                                                                      \
    std::string msg = "Cannot build " type "; invalid AST node; expected "; \
    msg += expected;                                                        \
    msg += ".";                                                             \
    throw ASTBuildError(msg);                                               \
  } while (0)

/* Responsible for building composite AST nodes. Contains
 * one method to build each AST node. 'store' method is used
 * to store terminal symbols for later use when a 'build'
 * call is invoked. */
class ASTBuilder {
 public:
  ASTBuilder(ASTStackProxy proxy) { stack_proxy = proxy; }
  ASTBuilder() {}

  // types
  ClassicBuiltinType build_builtin_type(classic_builtin_types::Type t);
  ClassicCustomType build_custom_type(std::string type_name);

  // classdefs
  PairClassdefList build_pair_classdef_list();
  LastClassdefList build_last_classdef_list();
  EmptyClassdefList build_empty_classdef_list();
  Classdef build_classdef();

  // functions
  PairFunctionList build_pair_function_list();
  LastFunctionList build_last_function_list();
  Function build_function();
  Function build_initus_function();
  FunctionBody build_function_body();

  // params
  PairParamList build_pair_param_list();
  LastParamList build_last_param_list();
  EmptyParamList build_empty_param_list();
  Param build_param();

  // statements
  CompoundStatement build_compound_statement();
  AssignStatement build_assign_statement();
  ExpressionStatement build_expression_statement();
  EmptyStatement build_empty_statement();
  ExodusStatement build_exodus_statement();

  // expressions
  ParenthesesExpression build_parentheses_expression();
  BinaryOperationExpression build_binary_operation_expression();
  FunctionCallExpression build_function_call_expression();
  LiteralExpression build_literal_expression(int literal);
  LiteralExpression build_literal_expression(double literal);
  LiteralExpression build_literal_expression(std::string literal);
  LiteralExpression build_literal_expression(classic_builtin_types::Type t);
  VariableExpression build_variable_expression(std::string var_name);

  // arguments
  PairArgumentList build_pair_argument_list();
  LastArgumentList build_last_argument_list();
  EmptyArgumentList build_empty_argument_list();
  Argument build_argument();

  /* Store a terminal symbol's semantic value for later use
   * in a non-terminal symbol's production rule. */
  std::string store(std::string val);
  /* Store a binary operator for later use in a binary operation production
  rule */
  BinaryOperator store(BinaryOperator b);

 private:
  ASTStackProxy stack_proxy;
  void push(ASTNode n);
  template <class T>
  T pop();
};
