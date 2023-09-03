#include "nodes.h"

ASTNode_::~ASTNode_() {}

Function_::Function_(char* n, ClassicType r, ParamList p, FunctionBody b) {
  std::string _name;
  if (n == NULL)
    std::string _name("");
  else
    _name = n;

  this->name = _name;
  this->return_type = r;
  this->param_list = p;
  this->body = b;
}

Param_::Param_(ClassicType t, char* n) {
  std::string _name;
  if (n == NULL)
    _name = "";
  else
    _name = n;
  this->classic_type = t;
  this->name = _name;
}

AssignStatement_::AssignStatement_(char* id, Expression exp) {
  std::string _id;
  if (id == NULL)
    _id = "";
  else
    _id = id;
  this->lhs_id = id;
  this->rhs_expression = exp;
}

FunctionCallExpression_::FunctionCallExpression_(char* name,
                                                 ArgumentList arg_list) {
  std::string _name;
  if (name == NULL)
    _name = "";
  else
    _name = name;
  this->function_name = _name;
  this->argument_list = arg_list;
}

VariableExpression_::VariableExpression_(char* n) {
  std::string _name;
  if (n == NULL)
    _name = "";
  else
    _name = n;
  this->var_name = _name;
}

Argument_::Argument_(char* n, Expression exp) {
  std::string _name;
  if (n == NULL)
    _name = "";
  else
    _name = n;
  this->name = _name;
  this->expression = exp;
}

bool eq(ClassicType t1, ClassicType t2) {
  if (t1->type != t2->type)
    return false;
  else if (t1->type == BUILTIN_TYPE) {
    return eq(t1->downcast<ClassicBuiltinType>(),
              t2->downcast<ClassicBuiltinType>());
  } else if (t1->type == INDEFINABLE_TYPE) {
    return true;
  } else
    throw CustomTypesNotSupported(
        "cannot compare ClassicType(s) of type CUSTOM_TYPE. Custom types are "
        "not supported.");
}

bool eq(ClassicBuiltinType t1, ClassicBuiltinType t2) {
  return t1->type == t2->type;
}

std::string str(ClassicType t) {
  if (t->type == BUILTIN_TYPE)
    return str(t->downcast<ClassicBuiltinType>());
  else if (t->type == CUSTOM_TYPE)
    return t->downcast<ClassicCustomType>()->type_name;
  return "indefinable";
}

std::string str(ClassicBuiltinType t) {
  switch (t->type) {
    case classic_builtin_types::ANEF:
      return "anef";
    case classic_builtin_types::DUPL:
      return "dupl";
    case classic_builtin_types::INT:
      return "int";
    case classic_builtin_types::STR:
      return "str";
    default:
      return "unknown";
  }
}
