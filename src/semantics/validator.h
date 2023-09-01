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

  FunctionEnvironment& get_functions_env() { return this->functions_env; }
  VariableEnvironment& get_function_variables_env() {
    return this->function_variables_env;
  }

  std::vector<SemanticError> validate(FunctionList func_list);
  std::vector<SemanticError> validate(PairFunctionList func_list);
  std::vector<SemanticError> validate(LastFunctionList func_list);
  std::vector<SemanticError> validate(Function func);

  std::vector<SemanticError> validate(ParamList param_list);
  std::vector<SemanticError> validate(PairParamList param_list);
  std::vector<SemanticError> validate(LastParamList param_list);
  std::vector<SemanticError> validate(EmptyParamList param_list);
  std::vector<SemanticError> validate(Param param);

  std::vector<SemanticError> validate(Statement stm);
  std::vector<SemanticError> validate(AssignStatement stm);
  std::vector<SemanticError> validate(ExpressionStatement stm);
  std::vector<SemanticError> validate(ExodusStatement stm);

  std::vector<SemanticError> validate(Expression exp);
  std::vector<SemanticError> validate(VariableExpression exp);
  std::vector<SemanticError> validate(FunctionCallExpression exp);
  std::vector<SemanticError> validate(BinaryOperationExpression exp);
  std::vector<SemanticError> validate(ParenthesesExpression exp);

 private:
  FunctionEnvironment functions_env;
  VariableEnvironment function_variables_env;
};
