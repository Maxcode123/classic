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
  std::vector<SemanticErrorMessage> errs{};
  if (stm->rhs_expression->type == BINARY_OPERATION_EXPRESSION) {
    BinaryOperationExpression binop =
        stm->rhs_expression->downcast<BinaryOperationExpression>();
    this->merge(errs, this->analyze(binop));
    stm->rhs_expression->set_classic_type(binop->classic_type);
  } else {
    stm->rhs_expression->set_classic_type(
        this->deduce_type(stm->rhs_expression));
  }
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
    case LITERAL_EXPRESSION: {
      LiteralExpression lit = exp->downcast<LiteralExpression>();
      exp->set_classic_type(
          (new ClassicBuiltinType_(lit->classic_builtin_type))->upcast());
      return std::vector<SemanticErrorMessage>{};
    }
    case VARIABLE_EXPRESSION: {
      VariableExpression var = exp->downcast<VariableExpression>();
      std::vector<SemanticErrorMessage> errs = this->analyze(var);
      exp->set_classic_type(var->classic_type);
      return errs;
    }
    case FUNCTION_CALL_EXPRESSION: {
      FunctionCallExpression call = exp->downcast<FunctionCallExpression>();
      std::vector<SemanticErrorMessage> errs = this->analyze(call);
      exp->set_classic_type(call->classic_type);
      return errs;
    }
    case BINARY_OPERATION_EXPRESSION: {
      BinaryOperationExpression binop =
          exp->downcast<BinaryOperationExpression>();
      std::vector<SemanticErrorMessage> errs = this->analyze(binop);
      exp->set_classic_type(binop->classic_type);
      return errs;
    }
    case PARENTHESES_EXPRESSION: {
      ParenthesesExpression paren = exp->downcast<ParenthesesExpression>();
      std::vector<SemanticErrorMessage> errs = this->analyze(paren);
      exp->set_classic_type(paren->classic_type);
      return errs;
    }
    default:
      return std::vector<SemanticErrorMessage>{};
  }
}

std::vector<SemanticErrorMessage> SemanticAnalyzer::analyze(
    VariableExpression exp) {
  exp->set_classic_type(this->type_deductor.deduce(exp));
  return this->validator.validate(exp);
}

std::vector<SemanticErrorMessage> SemanticAnalyzer::analyze(
    FunctionCallExpression exp) {
  std::vector<SemanticErrorMessage> errs{};
  for (auto itr = ArgumentListIterator(exp->argument_list),
            end = ArgumentListIterator(nullptr);
       itr != end; ++itr) {
    if (itr->expression->type == BINARY_OPERATION_EXPRESSION) {
      BinaryOperationExpression binop =
          itr->expression->downcast<BinaryOperationExpression>();
      this->merge(errs, this->analyze(binop));
      itr->expression->set_classic_type(binop->classic_type);
    } else {
      itr->expression->set_classic_type(this->deduce_type(itr->expression));
    }
  }

  exp->set_classic_type(this->type_deductor.deduce(exp));
  return this->merge(errs, this->validator.validate(exp));
}

std::vector<SemanticErrorMessage> SemanticAnalyzer::analyze(
    BinaryOperationExpression exp) {
  std::vector<SemanticErrorMessage> left;
  if (exp->left_expression->type == BINARY_OPERATION_EXPRESSION) {
    left = this->analyze(exp->left_expression);
  } else {
    exp->left_expression->set_classic_type(
        this->deduce_type(exp->left_expression));
    left = this->analyze(exp->left_expression);
  }

  std::vector<SemanticErrorMessage> right;
  if (exp->right_expression->type == BINARY_OPERATION_EXPRESSION) {
    right = this->analyze(exp->right_expression);
  } else {
    exp->right_expression->set_classic_type(
        this->deduce_type(exp->right_expression));
    right = this->analyze(exp->right_expression);
  }

  if (left.size() == 0 && right.size() == 0) {
    std::vector<SemanticErrorMessage> errs = this->validator.validate(exp);
    if (errs.size() == 0) {
      exp->set_classic_type(exp->right_expression->classic_type);
      exp->set_builtin_type(
          exp->right_expression->classic_type->downcast<ClassicBuiltinType>()
              ->type);
      return std::vector<SemanticErrorMessage>{};
    }
    this->merge(right, errs);
  }
  exp->set_classic_type((new ClassicIndefinableType_())->upcast());
  return this->merge(left, right);
}

std::vector<SemanticErrorMessage> SemanticAnalyzer::analyze(
    ParenthesesExpression exp) {
  std::vector<SemanticErrorMessage> errs = this->analyze(exp->expression);
  exp->set_classic_type(exp->expression->classic_type);
  return errs;
}

std::vector<SemanticErrorMessage> SemanticAnalyzer::merge(
    std::vector<SemanticErrorMessage> errs1,
    std::vector<SemanticErrorMessage> errs2) {
  errs1.insert(errs1.begin(), errs2.begin(), errs2.end());
  return errs1;
}

ClassicType SemanticAnalyzer::deduce_type(Expression exp) {
  // binary operation exp returns null
  switch (exp->type) {
    case LITERAL_EXPRESSION:
      return exp->downcast<LiteralExpression>()->classic_type;
    case VARIABLE_EXPRESSION:
      return this->type_deductor.deduce(exp->downcast<VariableExpression>());
    case FUNCTION_CALL_EXPRESSION:
      return this->type_deductor.deduce(
          exp->downcast<FunctionCallExpression>());
    case PARENTHESES_EXPRESSION:
      return this->deduce_type(
          exp->downcast<ParenthesesExpression>()->expression);
    default:
      return nullptr;
  }
}
