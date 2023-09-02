#include "analyzer.h"

std::vector<SemanticErrorMessage> SemanticAnalyzer::analyze(
    FunctionList func_list) {
  if (func_list->type == PAIR_FUNCTION_LIST)
    return this->analyze(func_list->downcast<PairFunctionList>());
  else
    return this->analyze(func_list->downcast<LastFunctionList>());
}

std::vector<SemanticErrorMessage> SemanticAnalyzer::analyze(
    PairFunctionList func_list) {
  std::vector<SemanticErrorMessage> errs = this->analyze(func_list->function);
  return this->merge(errs, this->analyze(func_list->next));
}

std::vector<SemanticErrorMessage> SemanticAnalyzer::analyze(
    LastFunctionList func_list) {
  return this->analyze(func_list->function);
}

std::vector<SemanticErrorMessage> SemanticAnalyzer::analyze(Function func) {
  std::vector<SemanticErrorMessage> errs1 = this->analyze(func->param_list);
  std::vector<SemanticErrorMessage> errs2 =
      this->merge(errs1, this->analyze(func->body->statement));
  return this->merge(errs2, this->analyze(func->body->exodus));
}

std::vector<SemanticErrorMessage> SemanticAnalyzer::analyze(
    ParamList param_list) {
  if (param_list->type == PAIR_PARAM_LIST)
    return this->analyze(param_list->downcast<PairParamList>());
  else if (param_list->type == LAST_PARAM_LIST)
    return this->analyze(param_list->downcast<LastParamList>());
  return std::vector<SemanticErrorMessage>{};
}

std::vector<SemanticErrorMessage> SemanticAnalyzer::analyze(
    PairParamList param_list) {
  std::vector<SemanticErrorMessage> errs1 = this->analyze(param_list->param);
  std::vector<SemanticErrorMessage> errs2 = this->analyze(param_list->next);
  return this->merge(errs1, errs2);
}

std::vector<SemanticErrorMessage> SemanticAnalyzer::analyze(
    LastParamList param_list) {
  return this->analyze(param_list->param);
}

std::vector<SemanticErrorMessage> SemanticAnalyzer::analyze(Param param) {
  return this->validator.validate(param);
}

std::vector<SemanticErrorMessage> SemanticAnalyzer::analyze(Statement stm) {
  switch (stm->type) {
    case COMPOUND_STATEMENT:
      return this->analyze(stm->downcast<CompoundStatement>());
    case ASSIGN_STATEMENT:
      return this->analyze(stm->downcast<AssignStatement>());
    case EXPRESSION_STATEMENT:
      return this->analyze(stm->downcast<ExpressionStatement>());
    default:
      return std::vector<SemanticErrorMessage>{};
  }
}

std::vector<SemanticErrorMessage> SemanticAnalyzer::analyze(
    CompoundStatement stm) {
  std::vector<SemanticErrorMessage> errs = this->analyze(stm->first_statement);
  return this->merge(errs, this->analyze(stm->second_statement));
}

std::vector<SemanticErrorMessage> SemanticAnalyzer::analyze(
    AssignStatement stm) {
  stm->rhs_expression->set_classic_type(
      this->type_deductor.deduce(stm->rhs_expression));
  return this->validator.validate(stm);
}

std::vector<SemanticErrorMessage> SemanticAnalyzer::analyze(
    ExpressionStatement stm) {
  return this->analyze(stm->expression);
}

std::vector<SemanticErrorMessage> SemanticAnalyzer::analyze(
    ExodusStatement stm) {
  return this->analyze(stm->expression);
}

std::vector<SemanticErrorMessage> SemanticAnalyzer::analyze(Expression exp) {
  switch (exp->type) {
    case LITERAL_EXPRESSION:
      return std::vector<SemanticErrorMessage>{};
    case VARIABLE_EXPRESSION:
      return this->analyze(exp->downcast<VariableExpression>());
    case FUNCTION_CALL_EXPRESSION:
      return this->analyze(exp->downcast<FunctionCallExpression>());
    case BINARY_OPERATION_EXPRESSION:
      return this->analyze(exp->downcast<BinaryOperationExpression>());
    case PARENTHESES_EXPRESSION:
      return this->analyze(exp->downcast<ParenthesesExpression>());
    default:
      return std::vector<SemanticErrorMessage>{};
  }
}

std::vector<SemanticErrorMessage> SemanticAnalyzer::analyze(
    VariableExpression exp) {
  return this->validator.validate(exp);
}

std::vector<SemanticErrorMessage> SemanticAnalyzer::analyze(
    FunctionCallExpression exp) {
  for (auto itr = ArgumentListIterator(exp->argument_list),
            end = ArgumentListIterator(nullptr);
       itr != end; ++itr) {
    itr->expression->set_classic_type(
        this->type_deductor.deduce(itr->expression));
  }

  return this->validator.validate(exp);
}

std::vector<SemanticErrorMessage> SemanticAnalyzer::analyze(
    BinaryOperationExpression exp) {
  std::vector<SemanticErrorMessage> left;
  if (exp->left_expression->type == BINARY_OPERATION_EXPRESSION) {
    left = this->analyze(exp->left_expression);
  } else {
    exp->left_expression->set_classic_type(
        this->type_deductor.deduce(exp->left_expression));
    left = this->analyze(exp->left_expression);
  }

  std::vector<SemanticErrorMessage> right;
  if (exp->right_expression->type == BINARY_OPERATION_EXPRESSION) {
    right = this->analyze(exp->right_expression);
  } else {
    exp->right_expression->set_classic_type(
        this->type_deductor.deduce(exp->right_expression));
    right = this->analyze(exp->right_expression);
  }

  if (left.size() == 0 && right.size() == 0) {
    std::vector<SemanticErrorMessage> errs = this->validator.validate(exp);
    if (errs.size() == 0) {
      exp->set_classic_type(exp->right_expression->classic_type);
      return errs;
    }
    this->merge(right, errs);
  }
  exp->set_classic_type((new ClassicIndefinableType_())->upcast());
  return this->merge(left, right);
}

std::vector<SemanticErrorMessage> SemanticAnalyzer::analyze(
    ParenthesesExpression exp) {
  return this->analyze(exp->expression);
}

std::vector<SemanticErrorMessage> SemanticAnalyzer::merge(
    std::vector<SemanticErrorMessage> errs1,
    std::vector<SemanticErrorMessage> errs2) {
  errs1.insert(errs1.begin(), errs2.begin(), errs2.end());
  return errs1;
}