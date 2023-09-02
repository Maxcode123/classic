#pragma once

#include <map>
#include <string>

enum SemanticError {
  UNDEFINED_VARIABLE = 15,
  UNDEFINED_FUNCTION,
  UNDEFINED_TYPE,

  UNDECLARED_ARGUMENT,

  INCOMPATIBLE_BINARY_OPERATION_TYPES,

  WRONG_EXODUS_TYPE,
  WRONG_ARGUMENT_TYPE,
  WRONG_ARGUMENT_NUMBER,
  WRONG_ASSIGNMENT_TYPE
};

static std::map<SemanticError, std::string> error_type_descriptions = {
    {UNDEFINED_VARIABLE,
     "Reference to variable that has not been previously defined."},
    {UNDEFINED_FUNCTION,
     "Reference to function that has not been previously defined."},
    {UNDEFINED_TYPE, "Reference to type that has not been previously defined."},
    {UNDECLARED_ARGUMENT,
     "Reference to argument (in function call) that has not been declared in "
     "function signature."},
    {INCOMPATIBLE_BINARY_OPERATION_TYPES,
     "Operands of binary operation have different types."},
    {WRONG_EXODUS_TYPE, "Exodus type does not match function signature."},
    {WRONG_ARGUMENT_TYPE,
     "Argument type (in function call) does not match function signature (but "
     "argument has correct name)."},
    {WRONG_ARGUMENT_NUMBER,
     "Number of arguments (in function call) is less than in function "
     "signature."},
    {WRONG_ASSIGNMENT_TYPE, "Assignment to variable of different type."}

};

class SemanticErrorMessage {
 public:
  SemanticErrorMessage(SemanticError error_type, std::string msg) {
    error_type_description = error_type_descriptions.at(error_type);
    message = msg;
  }

  std::string message;
  std::string error_type_description;
};

class TypeDeductionError : public std::exception {
 public:
  const char *what() { return message.c_str(); }
  TypeDeductionError(char *msg) { message = msg; }
  TypeDeductionError(const std::string msg) { message = msg; }

 private:
  std::string message;
};
