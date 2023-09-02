#include "analyzer.h"

std::vector<SemanticError> SemanticAnalyzer::analyze(FunctionList func_list) {
  if (func_list->type == PAIR_FUNCTION_LIST)
    return this->analyze(func_list->downcast<PairFunctionList>());
  else
    return this->analyze(func_list->downcast<LastFunctionList>());
}

std::vector<SemanticError> SemanticAnalyzer::analyze(
    PairFunctionList func_list) {
  std::vector<SemanticError> errs = this->analyze(func_list->function);
  return this->merge(errs, this->analyze(func_list->next));
}

std::vector<SemanticError> SemanticAnalyzer::analyze(
    LastFunctionList func_list) {
  return this->analyze(func_list->function);
}

std::vector<SemanticError> SemanticAnalyzer::analyze(Function func) {
  std::vector<SemanticError> errs1 = this->analyze(func->param_list);
  std::vector<SemanticError> errs2 =
      this->merge(errs1, this->analyze(func->body->statement));
  return this->merge(errs2, this->analyze(func->body->exodus));
}

std::vector<SemanticError> SemanticAnalyzer::analyze(ParamList param_list) {
  if (param_list->type == PAIR_PARAM_LIST)
    return this->analyze(param_list->downcast<PairParamList>());
  else if (param_list->type == LAST_PARAM_LIST)
    return this->analyze(param_list->downcast<LastParamList>());
  return std::vector<SemanticError>{};
}

std::vector<SemanticError> SemanticAnalyzer::analyze(PairParamList param_list) {
  std::vector<SemanticError> errs1 = this->analyze(param_list->param);
  std::vector<SemanticError> errs2 = this->analyze(param_list->next);
  return this->merge(errs1, errs2);
}

std::vector<SemanticError> SemanticAnalyzer::analyze(LastParamList param_list) {
  return this->analyze(param_list->param);
}

std::vector<SemanticError> SemanticAnalyzer::analyze(Param param) {
  return this->validator.validate(param);
}

std::vector<SemanticError> SemanticAnalyzer::analyze(Statement stm) {
  switch (stm->type) {
    case COMPOUND_STATEMENT:
      return this->analyze(stm->downcast<CompoundStatement>());
    case ASSIGN_STATEMENT:
      return this->analyze(stm->downcast<AssignStatement>());
    case EXPRESSION_STATEMENT:
      return this->analyze(stm->downcast<ExpressionStatement>());
    default:
      return std::vector<SemanticError>{};
  }
}

std::vector<SemanticError> SemanticAnalyzer::analyze(CompoundStatement stm) {
  std::vector<SemanticError> errs = this->analyze(stm->first_statement);
  return this->merge(errs, this->analyze(stm->second_statement));
}

std::vector<SemanticError> SemanticAnalyzer::analyze(AssignStatement stm) {
  stm->rhs_expression->set_classic_type(
      this->type_deductor.deduce(stm->rhs_expression));
  return this->validator.validate(stm);
}

std::vector<SemanticError> SemanticAnalyzer::analyze(ExpressionStatement stm) {
  return this->analyze(stm->expression);
}

std::vector<SemanticError> SemanticAnalyzer::analyze(ExodusStatement stm) {
  return this->analyze(stm->expression);
}

std::vector<SemanticError> SemanticAnalyzer::analyze(Expression exp) {
  switch (exp->type) {
    case LITERAL_EXPRESSION:
      return std::vector<SemanticError>{};
    case VARIABLE_EXPRESSION:
      return this->analyze(exp->downcast<VariableExpression>());
    case FUNCTION_CALL_EXPRESSION:
      return this->analyze(exp->downcast<FunctionCallExpression>());
    case BINARY_OPERATION_EXPRESSION:
      return this->analyze(exp->downcast<BinaryOperationExpression>());
    case PARENTHESES_EXPRESSION:
      return this->analyze(exp->downcast<ParenthesesExpression>());
    default:
      return std::vector<SemanticError>{};
  }
}

std::vector<SemanticError> SemanticAnalyzer::analyze(VariableExpression exp) {
  return this->validator.validate(exp);
}

std::vector<SemanticError> SemanticAnalyzer::analyze(
    FunctionCallExpression exp) {
  for (auto itr = ArgumentListIterator(exp->argument_list),
            end = ArgumentListIterator(nullptr);
       itr != end; ++itr) {
    itr->expression->set_classic_type(
        this->type_deductor.deduce(itr->expression));
  }

  return this->validator.validate(exp);
}

std::vector<SemanticError> SemanticAnalyzer::analyze(
    BinaryOperationExpression exp) {
  exp->left_expression->set_classic_type(
      this->type_deductor.deduce(exp->left_expression));
  exp->right_expression->set_classic_type(
      this->type_deductor.deduce(exp->right_expression));
  return this->validator.validate(exp);
}

std::vector<SemanticError> SemanticAnalyzer::analyze(
    ParenthesesExpression exp) {
  return this->analyze(exp->expression);
}

std::vector<SemanticError> SemanticAnalyzer::merge(
    std::vector<SemanticError> errs1, std::vector<SemanticError> errs2) {
  errs1.insert(errs1.begin(), errs2.begin(), errs2.end());
  return errs1;
}