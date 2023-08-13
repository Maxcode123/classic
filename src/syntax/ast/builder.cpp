#include "builder.h"

#include "errors.h"

void ASTStack_::push(ASTNode n) { this->stack.push(n); }

ASTNode ASTStack_::pop() {
  if (this->stack.size() == 0) {
    throw EmptyASTStackError("Cannot pop empty AST stack.");
  }
  ASTNode n = this->stack.top();
  this->stack.pop();
  return n;
}

/* macro that defines a try catch for poping from AST stack, catching if the
stack is empty and a try catch for asserting if the popped value is of the
correct type. */
#define TRY_CATCH_POP(type, p_type, name)                           \
  p_type name;                                                      \
  try {                                                             \
    name = this->pop<p_type>();                                     \
  } catch (EmptyASTStackError) {                                    \
    THROW_AST_BUILD_ERROR_EMPTY_STACK(type, p_type##_::cls());      \
  }                                                                 \
  if (name == nullptr) {                                            \
    THROW_AST_BUILD_ERROR_INVALID_AST_NODE(type, p_type##_::cls()); \
  }

PairFunctionList ASTBuilder::build_pair_function_list() {
  TRY_CATCH_POP("PairFunctionList", Function, func)
  TRY_CATCH_POP("PairFunctionList", FunctionList, func_list)
  PairFunctionList pair_list = new PairFunctionList_(func_list, func);
  this->push(pair_list->upcast());
#ifdef DEBUG_MODE
  std::cout << "built PairFunctionList\n";
#endif
  return pair_list;
}

LastFunctionList ASTBuilder::build_last_function_list() {
  TRY_CATCH_POP("LastFunctionList", Function, func)
  LastFunctionList func_list = new LastFunctionList_(func);
  this->push(func_list->upcast());
#ifdef DEBUG_MODE
  std::cout << "built LastFunctionList\n";
#endif
  return func_list;
}

Function ASTBuilder::build_function() {
  TRY_CATCH_POP("Function", FunctionBody, body)
  TRY_CATCH_POP("Function", ParamList, param_list)
  TRY_CATCH_POP("Function", TerminalASTNode, func_name)
  TRY_CATCH_POP("Function", TerminalASTNode, exodus_type)
  Function func = new Function_(func_name->semantic_value,
                                exodus_type->semantic_value, param_list, body);
  this->push(func);
#ifdef DEBUG_MODE
  std::cout << "built Function " << func_name->semantic_value << "\n";
#endif
  return func;
}

Function ASTBuilder::build_initus_function() {
  TRY_CATCH_POP("InitusFunction", FunctionBody, body)
  Function initus = new Function_("initus", "int", new EmptyParamList_(), body);
  this->push(initus);
#ifdef DEBUG_MODE
  std::cout << "built initus function\n";
#endif
  return initus;
}

FunctionBody ASTBuilder::build_function_body() {
  TRY_CATCH_POP("FunctionBody", ExodusStatement, ex_stm)
  TRY_CATCH_POP("FunctionBody", Statement, stm)
  FunctionBody body = new FunctionBody_(stm, ex_stm);
  this->push(body);
#ifdef DEBUG_MODE
  std::cout << "built FunctionBody\n";
#endif
  return body;
}

PairParamList ASTBuilder::build_pair_param_list() {
  TRY_CATCH_POP("PairParamList", Param, p)
  TRY_CATCH_POP("PairParamList", ParamList, param_list)
  PairParamList pair_list = new PairParamList_(param_list, p);
  this->push(pair_list->upcast());
#ifdef DEBUG_MODE
  std::cout << "built PairParamList\n";
#endif
  return pair_list;
}

LastParamList ASTBuilder::build_last_param_list() {
  TRY_CATCH_POP("LastParamList", Param, p)
  LastParamList param_list = new LastParamList_(p);
  this->push(param_list->upcast());
#ifdef DEBUG_MODE
  std::cout << "built LastParamList\n";
#endif
  return param_list;
}

EmptyParamList ASTBuilder::build_empty_param_list() {
  EmptyParamList param_list = new EmptyParamList_();
  this->push(param_list->upcast());
#ifdef DEBUG_MODE
  std::cout << "built EmptyParamList\n";
#endif
  return param_list;
}

Param ASTBuilder::build_param() {
  TRY_CATCH_POP("Param", TerminalASTNode, name)
  TRY_CATCH_POP("Param", TerminalASTNode, type)
  Param p = new Param_(type->semantic_value, name->semantic_value);
  this->push(p);
#ifdef DEBUG_MODE
  std::cout << "built Param " << type->semantic_value << " "
            << name->semantic_value << "\n";
#endif
  return p;
}

CompoundStatement ASTBuilder::build_compound_statement() {
  TRY_CATCH_POP("CompoundStatement", Statement, second_stm)
  TRY_CATCH_POP("CompoundStatement", Statement, first_stm)
  CompoundStatement stm = new CompoundStatement_(first_stm, second_stm);
  this->push(stm->upcast());
#ifdef DEBUG_MODE
  std::cout << "built CompoundStatement\n";
#endif
  return stm;
}

AssignStatement ASTBuilder::build_assign_statement() {
  TRY_CATCH_POP("AssignStatement", Expression, rhs_expression)
  TRY_CATCH_POP("AssignStatement", TerminalASTNode, lhs_id)
  AssignStatement stm =
      new AssignStatement_(lhs_id->semantic_value, rhs_expression);
  this->push(stm->upcast());
#ifdef DEBUG_MODE
  std::cout << "built AssignStatement\n";
#endif
  return stm;
}

ExpressionStatement ASTBuilder::build_expression_statement() {
  TRY_CATCH_POP("ExpressionStatement", Expression, exp)
  ExpressionStatement exp_stm = new ExpressionStatement_(exp);
  this->push(exp_stm->upcast());
#ifdef DEBUG_MODE
  std::cout << "built ExpressionStatement\n";
#endif
  return exp_stm;
}

EmptyStatement ASTBuilder::build_empty_statement() {
  EmptyStatement stm = new EmptyStatement_();
  this->push(stm->upcast());
#ifdef DEBUG_MODE
  std::cout << "built EmptyStatement\n";
#endif
  return stm;
}

ExodusStatement ASTBuilder::build_exodus_statement() {
  TRY_CATCH_POP("ExodusStatement", Expression, exp)
  ExodusStatement ex_stm = new ExodusStatement_(exp);
  this->push(ex_stm);
#ifdef DEBUG_MODE
  std::cout << "built ExodusStatement\n";
#endif
  return ex_stm;
}

ParenthesesExpression ASTBuilder::build_parentheses_expression() {
  TRY_CATCH_POP("ParenthesesExpression", Expression, exp)
  ParenthesesExpression paren_exp = new ParenthesesExpression_(exp);
  this->push(paren_exp->upcast());
#ifdef DEBUG_MODE
  std::cout << "built ParenthesesExpression\n";
#endif
  return paren_exp;
}

BinaryOperationExpression ASTBuilder::build_binary_operation_expression() {
  TRY_CATCH_POP("BinaryOperationExpression", Expression, right)
  TRY_CATCH_POP("BinaryOperationExpression", BinaryOperatorASTNode, binop_node)
  TRY_CATCH_POP("BinaryOperationExpression", Expression, left)
  BinaryOperationExpression exp =
      new BinaryOperationExpression_(binop_node->binop, left, right);
  this->push(exp->upcast());
#ifdef DEBUG_MODE
  std::cout << "built BinaryOperationExpression\n";
#endif
  return exp;
}

FunctionCallExpression ASTBuilder::build_function_call_expression() {
  TRY_CATCH_POP("FunctionCallExpression", ArgumentList, arg_list)
  TRY_CATCH_POP("FunctionCallExpression", TerminalASTNode, function_name)
  FunctionCallExpression exp =
      new FunctionCallExpression_(function_name->semantic_value, arg_list);
  this->push(exp->upcast());
#ifdef DEBUG_MODE
  std::cout << "built FunctionCallExpression\n";
#endif
  return exp;
}

LiteralExpression ASTBuilder::build_literal_expression(int literal) {
  LiteralExpression exp = new LiteralExpression_(literal);
  this->push(exp->upcast());
#ifdef DEBUG_MODE
  std::cout << "built int LiteralExpression\n";
#endif
  return exp;
}

LiteralExpression ASTBuilder::build_literal_expression(double literal) {
  LiteralExpression exp = new LiteralExpression_(literal);
  this->push(exp->upcast());
#ifdef DEBUG_MODE
  std::cout << "built double LiteralExpression\n";
#endif
  return exp;
}

LiteralExpression ASTBuilder::build_literal_expression(std::string literal) {
  LiteralExpression exp = new LiteralExpression_(literal);
  this->push(exp->upcast());
#ifdef DEBUG_MODE
  std::cout << "built string LiteralExpression\n";
#endif
  return exp;
}

VariableExpression ASTBuilder::build_variable_expression(std::string var_name) {
  VariableExpression exp = new VariableExpression_(var_name);
  this->push(exp->upcast());
#ifdef DEBUG_MODE
  std::cout << "built VariableExpression\n";
#endif
  return exp;
}

PairArgumentList ASTBuilder::build_pair_argument_list() {
  TRY_CATCH_POP("PairArgumentList", Argument, arg)
  TRY_CATCH_POP("PairArgumentList", ArgumentList, arg_list)
  PairArgumentList pair_list = new PairArgumentList_(arg, arg_list);
  this->push(pair_list->upcast());
#ifdef DEBUG_MODE
  std::cout << "built PairArgumentList\n";
#endif
  return pair_list;
}

LastArgumentList ASTBuilder::build_last_argument_list() {
  TRY_CATCH_POP("LastArgumentList", Argument, arg)
  LastArgumentList arg_list = new LastArgumentList_(arg);
  this->push(arg_list->upcast());
#ifdef DEBUG_MODE
  std::cout << "built LastArgumentList\n";
#endif
  return arg_list;
}

EmptyArgumentList ASTBuilder::build_empty_argument_list() {
  EmptyArgumentList arg_list = new EmptyArgumentList_();
  this->push(arg_list->upcast());
#ifdef DEBUG_MODE
  std::cout << "built EmptyArgumentList\n";
#endif
  return arg_list;
}

Argument ASTBuilder::build_argument() {
  TRY_CATCH_POP("Argument", Expression, exp)
  TRY_CATCH_POP("Argument", TerminalASTNode, term)
  Argument arg = new Argument_(term->semantic_value, exp);
  this->push(arg);
#ifdef DEBUG_MODE
  std::cout << "built Argument\n";
#endif
  return arg;
}

std::string ASTBuilder::store(std::string val) {
  this->push(new TerminalASTNode_(val));
  return val;
}

BinaryOperator ASTBuilder::store(BinaryOperator b) {
  this->push(new BinaryOperatorASTNode_(b));
  return b;
}

void ASTBuilder::push(ASTNode n) { this->stack_proxy.push(n); }

template <class T>
T ASTBuilder::pop() {
  return dynamic_cast<T>(this->stack_proxy.pop());
}