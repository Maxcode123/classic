#pragma once

#include <vector>

#include "../syntax/ast/nodes.h"
#include "environment.h"
#include "errors.h"

class Validator {
 public:
  Validator() {
    functions_env = FunctionEnvironment();
    function_variables_env = VariableEnvironment();
  }
  Validator(FunctionEnvironment functions, VariableEnvironment variables) {
    functions_env = functions;
    function_variables_env = variables;
  }

  std::vector<SemanticError> validate(Function func);

  std::vector<SemanticError> validate(Param param);

  std::vector<SemanticError> validate(AssignStatement stm);
  std::vector<SemanticError> validate(ExpressionStatement stm);
  std::vector<SemanticError> validate(ExodusStatement stm);

  std::vector<SemanticError> validate(VariableExpression exp);
  std::vector<SemanticError> validate(FunctionCallExpression exp);
  std::vector<SemanticError> validate(BinaryOperationExpression exp);
  std::vector<SemanticError> validate(ParenthesesExpression exp);

  std::vector<SemanticError> compare(ParamList param_list,
                                     ArgumentList arg_list);

 private:
  FunctionEnvironment functions_env;
  VariableEnvironment function_variables_env;
};
