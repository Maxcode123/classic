#pragma once

#include <map>
#include <vector>

#include "../syntax/ast/nodes.h"
#include "environment.h"
#include "errors.h"

/*
This class is responsible for validating specific ast nodes and producing
semantic error messages. All the semantics logic is encapsulated in its'
methods.
Methods expect the nodes to have a types.
*/
class Validator {
 public:
  Validator() {}
  Validator(EnvironmentsProxy p) { proxy = p; }

  std::vector<SemanticErrorMessage> validate(Param param);

  std::vector<SemanticErrorMessage> validate(AssignStatement stm);

  std::vector<SemanticErrorMessage> validate(VariableExpression exp);
  std::vector<SemanticErrorMessage> validate(FunctionCallExpression exp);
  std::vector<SemanticErrorMessage> validate(BinaryOperationExpression exp);

  std::vector<SemanticErrorMessage> compare(ParamList param_list,
                                            ArgumentList arg_list);

 private:
  EnvironmentsProxy proxy;
};
