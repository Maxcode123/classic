#include "nodes.h"

ASTNode_::~ASTNode_() {}

Function_::Function_(char* n, classic_types::Type r, ParamList p,
                     FunctionBody b) {
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

Param_::Param_(classic_types::Type t, char* n) {
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
