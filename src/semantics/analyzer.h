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

  std::vector<SemanticErrorMessage> analyze(FunctionList func_list);
  std::vector<SemanticErrorMessage> analyze(PairFunctionList func_list);
  std::vector<SemanticErrorMessage> analyze(LastFunctionList func_list);
  std::vector<SemanticErrorMessage> analyze(Function func);

  std::vector<SemanticErrorMessage> analyze(ParamList param_list);
  std::vector<SemanticErrorMessage> analyze(PairParamList param_list);
  std::vector<SemanticErrorMessage> analyze(LastParamList param_list);
  std::vector<SemanticErrorMessage> analyze(Param param);

  std::vector<SemanticErrorMessage> analyze(Statement stm);
  std::vector<SemanticErrorMessage> analyze(CompoundStatement stm);
  std::vector<SemanticErrorMessage> analyze(AssignStatement stm);
  std::vector<SemanticErrorMessage> analyze(ExpressionStatement stm);
  std::vector<SemanticErrorMessage> analyze(ExodusStatement stm);

  std::vector<SemanticErrorMessage> analyze(Expression exp);
  std::vector<SemanticErrorMessage> analyze(VariableExpression exp);
  std::vector<SemanticErrorMessage> analyze(FunctionCallExpression exp);
  std::vector<SemanticErrorMessage> analyze(BinaryOperationExpression exp);
  std::vector<SemanticErrorMessage> analyze(ParenthesesExpression exp);

 private:
  Validator validator;
  TypeDeductor type_deductor;
  std::vector<SemanticErrorMessage> merge(
      std::vector<SemanticErrorMessage> errs1,
      std::vector<SemanticErrorMessage> errs2);
  ClassicType deduce_type(Expression exp);
};