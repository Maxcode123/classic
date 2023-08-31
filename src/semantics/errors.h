#pragma once

#include <string>

#define MEMBER(n, d) inline static const SemanticError n = SemanticError(#n, d)

class SemanticErrors {
 private:
  class SemanticError {
   public:
    SemanticError(std::string n) { name = n; }
    SemanticError(std::string n, std::string d) {
      name = n;
      description = d;
    }

    std::string name;
    std::string description;
  };

 public:
  MEMBER(UNDEFINED_VARIABLE,
         "Reference to variable that has not been previously defined.");
  MEMBER(UNDEFINED_FUNCTION,
         "Reference to function that has not been previously defined.");
  MEMBER(UNDEFINED_TYPE,
         "Reference to type that has not been previously defined.");

  MEMBER(UNDECLARED_ARGUMENT,
         "Reference to argument (in function call) that has not been declared "
         "in function signature.");

  MEMBER(INCOMPATIBLE_BINARY_OPERATION_TYPES,
         "Operands of binary operation have different types.");

  MEMBER(WRONG_EXODUS_TYPE, "Exodus type does not match function signature.");
  MEMBER(WRONG_ARGUMENT_TYPE,
         "Argument type (in function call) does not match function signature "
         "(but argument has correct name).");
  MEMBER(WRONG_ARGUMENT_NUMBER,
         "Number of arguments (in function call) is less than in function "
         "signature.");
  MEMBER(WRONG_ASSIGNMENT_TYPE, "Assignment to variable of different type.");
};