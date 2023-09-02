#pragma once

#include "../syntax/ast/nodes.h"
#include "type_deductor.h"
#include "validator.h"

class SemanticAnalyzer {
 public:
  SemanticAnalyzer(Validator v, TypeDeductor t) {
    validator = v;
    type_deductor = t;
  }
  SemanticAnalyzer() {}

  std::vector<SemanticError> analyze(FunctionList func_list);
  std::vector<SemanticError> analyze(PairFunctionList func_list);
  std::vector<SemanticError> analyze(LastFunctionList func_list);
  std::vector<SemanticError> analyze(Function func);

  std::vector<SemanticError> analyze(ParamList param_list);
  std::vector<SemanticError> analyze(PairParamList param_list);
  std::vector<SemanticError> analyze(LastParamList param_list);
  std::vector<SemanticError> analyze(Param param);

  std::vector<SemanticError> analyze(Statement stm);
  std::vector<SemanticError> analyze(CompoundStatement stm);
  std::vector<SemanticError> analyze(AssignStatement stm);
  std::vector<SemanticError> analyze(ExpressionStatement stm);
  std::vector<SemanticError> analyze(ExodusStatement stm);

  std::vector<SemanticError> analyze(Expression exp);
  std::vector<SemanticError> analyze(VariableExpression exp);
  std::vector<SemanticError> analyze(FunctionCallExpression exp);
  std::vector<SemanticError> analyze(BinaryOperationExpression exp);
  std::vector<SemanticError> analyze(ParenthesesExpression exp);

 private:
  Validator validator;
  TypeDeductor type_deductor;
  std::vector<SemanticError> merge(std::vector<SemanticError> errs1,
                                   std::vector<SemanticError> errs2);
};