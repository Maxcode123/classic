#pragma once

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <stack>
#include <string>

#include "../../utils/errors.h"
#include "errors.h"

#define DECL_TYPEDEF(cls) \
  class cls##_;           \
  typedef cls##_ *cls;

// forward declarations
DECL_TYPEDEF(ASTNode)
DECL_TYPEDEF(TerminalASTNode)
DECL_TYPEDEF(BinaryOperatorASTNode)
DECL_TYPEDEF(ClassicType)
DECL_TYPEDEF(ClassicBuiltinType)
DECL_TYPEDEF(ClassicCustomType)
DECL_TYPEDEF(Program)
DECL_TYPEDEF(ClassdefList)
DECL_TYPEDEF(PairClassdefList)
DECL_TYPEDEF(LastClassdefList)
DECL_TYPEDEF(EmptyClassdefList)
DECL_TYPEDEF(Classdef)
DECL_TYPEDEF(FunctionList)
DECL_TYPEDEF(PairFunctionList)
DECL_TYPEDEF(LastFunctionList)
DECL_TYPEDEF(Function)
DECL_TYPEDEF(ParamList)
DECL_TYPEDEF(PairParamList)
DECL_TYPEDEF(LastParamList)
DECL_TYPEDEF(EmptyParamList)
DECL_TYPEDEF(Param)
DECL_TYPEDEF(FunctionBody)
DECL_TYPEDEF(Statement)
DECL_TYPEDEF(CompoundStatement)
DECL_TYPEDEF(AssignStatement)
DECL_TYPEDEF(ExpressionStatement)
DECL_TYPEDEF(EmptyStatement)
DECL_TYPEDEF(ExodusStatement)
DECL_TYPEDEF(Expression)
DECL_TYPEDEF(ParenthesesExpression)
DECL_TYPEDEF(BinaryOperationExpression)
DECL_TYPEDEF(FunctionCallExpression)
DECL_TYPEDEF(LiteralExpression)
DECL_TYPEDEF(VariableExpression)
DECL_TYPEDEF(ArgumentList)
DECL_TYPEDEF(PairArgumentList)
DECL_TYPEDEF(LastArgumentList)
DECL_TYPEDEF(EmptyArgumentList)
DECL_TYPEDEF(Argument)

/* Interface for Nodes of the Abstract Syntax Tree (AST). */
class ASTNode_ {
 public:
  static std::string cls();
  virtual ~ASTNode_() = 0;
};

/// @brief BaseTypeASTNode_ is a generic ASTNode class that is subclassed. e.g.
/// class Statement_ : virtual public BaseTypeASTNode_<Statement_*,
/// StatementType> {};
/// @tparam T_enum Enumeration of subclassses
/// @tparam T_ptr Pointer to class that is defined
template <class T_ptr, class T_enum>
class BaseTypeASTNode_ : virtual public ASTNode_ {
 public:
  T_enum type;

  template <class T>
  T downcast() {
    return dynamic_cast<T>(this->data);
  }

 protected:
  T_ptr data;
  void set(T_enum t, T_ptr d) {
    type = t;
    data = d;
  }
};

/* TerminalASTNode is an AST node containing a terminal symbol with a semantic
 * value. */
class TerminalASTNode_ : virtual public ASTNode_ {
 public:
  std::string semantic_value;
  TerminalASTNode_(std::string val) { semantic_value = val; }

  static std::string cls() { return "TerminalASTNode"; }
};

namespace classic_builtin_types {
enum Type {
  INT,
  DUPL,
  STR,
  ANEF,
};
};

enum BinaryOperator { BINARY_PLUS, BINARY_MINUS, BINARY_DIV, BINARY_TIMES };

class BinaryOperatorASTNode_ : virtual public ASTNode_ {
 public:
  BinaryOperator binop;
  BinaryOperatorASTNode_(BinaryOperator b) { binop = b; }

  static std::string cls() { return "BinaryOperatorASTNode"; }
};

enum ClassicTypeGroup { BUILTIN_TYPE, CUSTOM_TYPE };

class ClassicType_
    : virtual public BaseTypeASTNode_<ClassicType, ClassicTypeGroup> {
 public:
  ClassicType_(ClassicBuiltinType n) { set(BUILTIN_TYPE, (ClassicType)n); };
  ClassicType_(ClassicCustomType n) { set(CUSTOM_TYPE, (ClassicType)n); }
  ClassicType_() {}

  bool eq(ClassicType t) {
    if (this->type != t->type)
      return false;
    else if (t->type == BUILTIN_TYPE) {
      return this->downcast<ClassicBuiltinType>()->eq(
          t->downcast<ClassicBuiltinType>());
    } else
      throw CustomTypesNotSupported(
          "cannot compare ClassicType(s) of type CUSTOM_TYPE. Custom types are "
          "not supported.");
  }

  static std::string cls() { return "ClassicType"; }
};

class ClassicBuiltinType_ : virtual public ClassicType_ {
 public:
  classic_builtin_types::Type type;
  ClassicBuiltinType_(classic_builtin_types::Type t) { type = t; }
  ClassicType upcast() { return new ClassicType_(this); }

  bool eq(ClassicBuiltinType t) { return this->type == t->type; }

 private:
  using ClassicType_::downcast;
};

class ClassicCustomType_ : virtual public ClassicType_ {
 public:
  std::string type_name;
  ClassicCustomType_(std::string n) { type_name = n; }
  ClassicType upcast() { return new ClassicType_(this); }

 private:
  using ClassicType_::downcast;
};

class Program_ : virtual public ASTNode_ {
 public:
  ClassdefList classdef_list;
  FunctionList function_list;
  Program_(ClassdefList c, FunctionList f) {
    classdef_list = c;
    function_list = f;
  }
  Program_(){};
};

enum ClassdefListType {
  PAIR_CLASSDEF_LIST,
  LAST_CLASSDEF_LIST,
  EMPTY_CLASSDEFLIST
};

class ClassdefList_ : virtual public ASTNode_ {
 public:
  static const ClassdefListType type;
};

class PairClassdefList_ : virtual public ClassdefList_ {
 public:
  static const ClassdefListType type = PAIR_CLASSDEF_LIST;
  PairClassdefList_(){};
};

class LastClassdefList_ : virtual public ClassdefList_ {
 public:
  static const ClassdefListType type = LAST_CLASSDEF_LIST;
  LastClassdefList_(){};
};

class EmptyClassdefList_ : virtual public ClassdefList_ {
 public:
  static const ClassdefListType type = EMPTY_CLASSDEFLIST;
  EmptyClassdefList_(){};
};

class Classdef_ {};

enum FunctionListType { PAIR_FUNCTION_LIST, LAST_FUNCTION_LIST };

class FunctionList_
    : virtual public BaseTypeASTNode_<FunctionList, FunctionListType> {
 public:
  FunctionList_(PairFunctionList func_list) {
    set(PAIR_FUNCTION_LIST, (FunctionList)func_list);
  }
  FunctionList_(LastFunctionList func_list) {
    set(LAST_FUNCTION_LIST, (FunctionList)func_list);
  }
  FunctionList_() {}

  static std::string cls() { return "FunctionList"; }
};

class PairFunctionList_ : virtual public FunctionList_ {
 public:
  Function function;
  FunctionList next;
  PairFunctionList_(FunctionList n, Function f) {
    function = f;
    next = n;
  };
  FunctionList upcast() { return new FunctionList_(this); }
  static std::string cls() { return "PairFunctionList"; }

 private:
  using FunctionList_::downcast;
};

class LastFunctionList_ : virtual public FunctionList_ {
 public:
  Function function;
  LastFunctionList_(Function f) { function = f; };
  FunctionList upcast() { return new FunctionList_(this); }
  static std::string cls() { return "LastFunctionList"; }

 private:
  using FunctionList_::downcast;
};

class Function_ : virtual public ASTNode_ {
 public:
  std::string name;
  ClassicType return_type;
  ParamList param_list;
  FunctionBody body;
  Function_(std::string n, ClassicType t, ParamList p, FunctionBody b) {
    name = n;
    return_type = t;
    param_list = p;
    body = b;
  }
  Function_(char *n, ClassicType t, ParamList p, FunctionBody b);
  Function_() {}

  static std::string cls() { return "Function"; }
};

enum ParamListType { PAIR_PARAM_LIST, LAST_PARAM_LIST, EMPTY_PARAM_LIST };

class ParamList_ : virtual public BaseTypeASTNode_<ParamList, ParamListType> {
 public:
  Param param;

  ParamList_(PairParamList param_list) {
    set(PAIR_PARAM_LIST, (ParamList)param_list);
  }
  ParamList_(LastParamList param_list) {
    set(LAST_PARAM_LIST, (ParamList)param_list);
  }
  ParamList_(EmptyParamList param_list) {
    set(EMPTY_PARAM_LIST, (ParamList)param_list);
  }
  ParamList_() {}

  static std::string cls() { return "ParamList"; }
};

class PairParamList_ : virtual public ParamList_ {
 public:
  ParamList next;
  PairParamList_(ParamList n, Param p) {
    param = p;
    next = n;
  }
  ParamList upcast() { return new ParamList_(this); }
  static std::string cls() { return "PairParamList"; }

 private:
  using ParamList_::downcast;
};

class LastParamList_ : virtual public ParamList_ {
 public:
  LastParamList_(Param p) { param = p; }
  ParamList upcast() { return new ParamList_(this); }
  static std::string cls() { return "LastParamList"; }

 private:
  using ParamList_::downcast;
};

class EmptyParamList_ : virtual public ParamList_ {
 public:
  EmptyParamList_(){};
  ParamList upcast() { return new ParamList_(this); }
  static std::string cls() { return "EmptyParamList"; }

 private:
  using ParamList_::downcast;
};

class ParamListIterator {
 public:
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = Param_;
  using pointer = value_type *;
  using reference = value_type &;

  ParamListIterator(ParamList p) {
    if (p == nullptr || p->type == EMPTY_PARAM_LIST)
      param_list = nullptr;
    else
      param_list = p;
  }

  reference operator*() const {
    if (this->param_list == nullptr) {
      throw IteratorError("Cannot dereference empty param list iterator.");
    }
    if (this->param_list->type == PAIR_PARAM_LIST) {
      return *(this->param_list->downcast<PairParamList>()->param);
    } else {
      return *(this->param_list->downcast<LastParamList>()->param);
    }
  }
  pointer operator->() {
    if (this->param_list == nullptr) {
      throw IteratorError("Cannot reference empty param list iterator.");
    }
    if (this->param_list->type == PAIR_PARAM_LIST) {
      return this->param_list->downcast<PairParamList>()->param;
    } else {
      return this->param_list->downcast<LastParamList>()->param;
    }
  }

  ParamListIterator &operator++() {
    if (this->param_list->type == PAIR_PARAM_LIST) {
      this->param_list = this->param_list->downcast<PairParamList>()->next;
    } else {
      this->param_list = nullptr;
    }
    return *this;
  }

  ParamListIterator operator++(int) {
    ParamListIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  friend bool operator==(const ParamListIterator &a,
                         const ParamListIterator &b) {
    return a.param_list == b.param_list;
  }
  friend bool operator!=(const ParamListIterator &a, ParamListIterator &b) {
    return a.param_list != b.param_list;
  }

 private:
  ParamList param_list;
};

class Param_ : virtual public ASTNode_ {
 public:
  ClassicType classic_type;
  std::string name;
  Param_(ClassicType t, std::string n) {
    classic_type = t;
    name = n;
  }
  Param_(ClassicType t, char *n);
  Param_() {}

  static std::string cls() { return "Param"; }
};

class FunctionBody_ : virtual public ASTNode_ {
 public:
  Statement statement;
  ExodusStatement exodus;
  FunctionBody_(Statement stm, ExodusStatement ex) {
    statement = stm;
    exodus = ex;
  }
  FunctionBody_() {}

  static std::string cls() { return "FunctionBody"; }
};

enum StatementType {
  COMPOUND_STATEMENT,
  ASSIGN_STATEMENT,
  EXPRESSION_STATEMENT,
  EMPTY_STATEMENT
};

class Statement_ : virtual public BaseTypeASTNode_<Statement, StatementType> {
 public:
  Statement_(CompoundStatement stm) { set(COMPOUND_STATEMENT, (Statement)stm); }
  Statement_(AssignStatement stm) { set(ASSIGN_STATEMENT, (Statement)stm); }
  Statement_(ExpressionStatement stm) {
    set(EXPRESSION_STATEMENT, (Statement)stm);
  }
  Statement_(EmptyStatement stm) { set(EMPTY_STATEMENT, (Statement)stm); }
  Statement_() {}

  static std::string cls() { return "Statement"; }
};

class CompoundStatement_ : virtual public Statement_ {
 public:
  Statement first_statement;
  Statement second_statement;
  CompoundStatement_(Statement first, Statement second) {
    first_statement = first;
    second_statement = second;
  }
  Statement upcast() { return new Statement_(this); }
  static std::string cls() { return "CompoundStatement"; }

 private:
  using Statement_::downcast;
};

class AssignStatement_ : virtual public Statement_ {
 public:
  std::string lhs_id;
  Expression rhs_expression;
  AssignStatement_(std::string id, Expression exp) {
    lhs_id = id;
    rhs_expression = exp;
  }
  AssignStatement_(char *id, Expression exp);
  Statement upcast() { return new Statement_(this); }
  static std::string cls() { return "AssignStatement"; }

 private:
  using Statement_::downcast;
};

class ExpressionStatement_ : virtual public Statement_ {
 public:
  Expression expression;
  ExpressionStatement_(Expression exp) { expression = exp; }
  Statement upcast() { return new Statement_(this); }
  static std::string cls() { return "ExpressionStatement"; }

 private:
  using Statement_::downcast;
};

class EmptyStatement_ : virtual public Statement_ {
 public:
  EmptyStatement_() {}
  Statement upcast() { return new Statement_(this); }
  static std::string cls() { return "EmptyStatement"; }

 private:
  using Statement_::downcast;
};

class ExodusStatement_ : virtual public ASTNode_ {
 public:
  Expression expression;
  ExodusStatement_(Expression exp) { expression = exp; }
  ExodusStatement_() {}

  static std::string cls() { return "ExodusStatement"; }
};

enum ExpressionType {
  PARENTHESES_EXPRESSION,
  BINARY_OPERATION_EXPRESSION,
  FUNCTION_CALL_EXPRESSION,
  LITERAL_EXPRESSION,
  VARIABLE_EXPRESSION
};

class Expression_
    : virtual public BaseTypeASTNode_<Expression, ExpressionType> {
 public:
  ClassicType classic_type;

  Expression_(ParenthesesExpression exp) {
    set(PARENTHESES_EXPRESSION, (Expression)exp);
  }
  Expression_(BinaryOperationExpression exp) {
    set(BINARY_OPERATION_EXPRESSION, (Expression)exp);
  }
  Expression_(FunctionCallExpression exp) {
    set(FUNCTION_CALL_EXPRESSION, (Expression)exp);
  }
  Expression_(LiteralExpression exp) {
    set(LITERAL_EXPRESSION, (Expression)exp);
  }
  Expression_(VariableExpression exp) {
    set(VARIABLE_EXPRESSION, (Expression)exp);
  }
  Expression_() {}

  void set_classic_type(ClassicType t) { classic_type = t; }

  static std::string cls() { return "Expression"; }
};

class ParenthesesExpression_ : virtual public Expression_ {
 public:
  Expression expression;
  ParenthesesExpression_(Expression exp) { expression = exp; }
  ParenthesesExpression_() {}
  Expression upcast() { return new Expression_(this); }
  static std::string cls() { return "ParenthesesExpression"; }

 private:
  using Expression_::downcast;
};

class BinaryOperationExpression_ : virtual public Expression_ {
 public:
  BinaryOperator binary_operator;
  Expression left_expression;
  Expression right_expression;
  classic_builtin_types::Type classic_builtin_type =
      classic_builtin_types::ANEF;

  BinaryOperationExpression_(BinaryOperator binop, Expression left,
                             Expression right) {
    binary_operator = binop;
    left_expression = left;
    right_expression = right;
  }
  BinaryOperationExpression_() {}

  Expression upcast() { return new Expression_(this); }
  void set_builtin_type(classic_builtin_types::Type t) {
    classic_builtin_type = t;
  }

  static std::string cls() { return "BinaryOperationExpression"; }

 private:
  using Expression_::downcast;
};

class FunctionCallExpression_ : virtual public Expression_ {
 public:
  std::string function_name;
  ArgumentList argument_list;
  FunctionCallExpression_(std::string name, ArgumentList arg_list) {
    function_name = name;
    argument_list = arg_list;
  }
  FunctionCallExpression_(char *name, ArgumentList arg_list);
  FunctionCallExpression_() {}
  Expression upcast() { return new Expression_(this); }
  static std::string cls() { return "FunctionCallExpression"; }

 private:
  using Expression_::downcast;
};

class LiteralExpression_ : virtual public Expression_ {
 public:
  std::string literal_str;
  classic_builtin_types::Type classic_builtin_type;
  LiteralExpression_(int literal) {
    set(std::to_string(literal), classic_builtin_types::INT);
  }
  LiteralExpression_(double literal) {
    set(std::to_string(literal), classic_builtin_types::DUPL);
  }
  LiteralExpression_(std::string literal) {
    set(literal, classic_builtin_types::STR);
  }
  LiteralExpression_(char *literal) {
    set(literal, classic_builtin_types::STR);
  }
  LiteralExpression_(classic_builtin_types::Type type) {
    if (type == classic_builtin_types::ANEF)
      set("anef", type);
    else
      set("", type);
  }
  LiteralExpression_() {}
  Expression upcast() { return new Expression_(this); }
  static std::string cls() { return "LiteralExpression"; }

 private:
  using Expression_::classic_type;
  using Expression_::downcast;

  void set(std::string str, classic_builtin_types::Type t) {
    literal_str = str, classic_builtin_type = t;
  }
};

class VariableExpression_ : virtual public Expression_ {
 public:
  std::string var_name;
  VariableExpression_(std::string n) { var_name = n; }
  VariableExpression_(char *n);
  VariableExpression_() {}
  Expression upcast() { return new Expression_(this); }
  static std::string cls() { return "VariableExpression"; }

 private:
  using Expression_::downcast;
};

enum ArgumentListType { PAIR_ARG_LIST, LAST_ARG_LIST, EMPTY_ARG_LIST };

class ArgumentList_
    : virtual public BaseTypeASTNode_<ArgumentList, ArgumentListType> {
 public:
  ArgumentList_(PairArgumentList arg_list) {
    set(PAIR_ARG_LIST, (ArgumentList)arg_list);
  }
  ArgumentList_(LastArgumentList arg_list) {
    set(LAST_ARG_LIST, (ArgumentList)arg_list);
  }
  ArgumentList_(EmptyArgumentList arg_list) {
    set(EMPTY_ARG_LIST, (ArgumentList)arg_list);
  }
  ArgumentList_() {}
  static std::string cls() { return "ArgumentList"; }
};

class PairArgumentList_ : virtual public ArgumentList_ {
 public:
  Argument argument;
  ArgumentList next;
  PairArgumentList_(Argument arg, ArgumentList n) {
    argument = arg;
    next = n;
  }
  PairArgumentList_() {}
  ArgumentList upcast() { return new ArgumentList_(this); }
  static std::string cls() { return "PairArgumentList"; }

 private:
  using ArgumentList_::downcast;
};

class LastArgumentList_ : virtual public ArgumentList_ {
 public:
  Argument argument;
  LastArgumentList_(Argument arg) { argument = arg; }
  LastArgumentList_() {}
  ArgumentList upcast() { return new ArgumentList_(this); }
  static std::string cls() { return "LastArgumentList"; }

 private:
  using ArgumentList_::downcast;
};

class EmptyArgumentList_ : virtual public ArgumentList_ {
 public:
  EmptyArgumentList_() {}
  ArgumentList upcast() { return new ArgumentList_(this); }
  static std::string cls() { return "EmptyArgumentList"; }

 private:
  using ArgumentList_::downcast;
};

class ArgumentListIterator {
 public:
  using iterator_category = std::forward_iterator_tag;
  using difference_type = std::ptrdiff_t;
  using value_type = Argument_;
  using pointer = value_type *;
  using reference = value_type &;

  ArgumentListIterator(ArgumentList a) {
    if (a == nullptr || a->type == EMPTY_ARG_LIST)
      this->arg_list = nullptr;
    else
      this->arg_list = a;
  }

  reference operator*() const {
    if (this->arg_list == nullptr) {
      throw IteratorError("Cannot dereference empty argument list iterator.");
    }
    if (this->arg_list->type == PAIR_ARG_LIST)
      return *(this->arg_list->downcast<PairArgumentList>()->argument);
    else
      return *(this->arg_list->downcast<LastArgumentList>()->argument);
  }

  pointer operator->() {
    if (this->arg_list == nullptr)
      throw IteratorError("Cannot reference empty argument list iterator.");
    if (this->arg_list->type == PAIR_ARG_LIST)
      return this->arg_list->downcast<PairArgumentList>()->argument;
    else
      return this->arg_list->downcast<LastArgumentList>()->argument;
  }

  ArgumentListIterator &operator++() {
    if (this->arg_list->type == PAIR_ARG_LIST)
      this->arg_list = this->arg_list->downcast<PairArgumentList>()->next;
    else
      this->arg_list = nullptr;
    return *this;
  }

  ArgumentListIterator operator++(int) {
    ArgumentListIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  friend bool operator==(const ArgumentListIterator &a,
                         const ArgumentListIterator &b) {
    return a.arg_list == b.arg_list;
  }

  friend bool operator!=(const ArgumentListIterator &a,
                         ArgumentListIterator &b) {
    return a.arg_list != b.arg_list;
  }

 private:
  ArgumentList arg_list;
};

class Argument_ : virtual public ASTNode_ {
 public:
  std::string name;
  Expression expression;
  Argument_(std::string n, Expression exp) {
    name = n;
    expression = exp;
  }
  Argument_(char *n, Expression exp);
  Argument_() {}

  static std::string cls() { return "Argument"; }
};
