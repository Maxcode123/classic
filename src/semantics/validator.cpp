#include "validator.h"

std::vector<SemanticErrorMessage> Validator::validate(Param param) {
  if (param->classic_type->type == CUSTOM_TYPE) {
    ClassicCustomType custom_type =
        param->classic_type->downcast<ClassicCustomType>();
    if (!this->proxy.contains_type(custom_type->type_name))
      return std::vector<SemanticErrorMessage>{SemanticErrorMessage(
          UNDEFINED_TYPE, "type '" + custom_type->type_name +
                              "' of parameter " + param->name +
                              " has not been defined. ")};
  } else if (param->classic_type->type == INDEFINABLE_TYPE) {
    return std::vector<SemanticErrorMessage>{SemanticErrorMessage(
        UNDEFINED_TYPE,
        "parameter " + param->name + " has indefinable type. ")};
  }
  return std::vector<SemanticErrorMessage>{};
}

std::vector<SemanticErrorMessage> Validator::validate(AssignStatement stm) {
  if (stm->rhs_expression->classic_type == nullptr) {
    throw NoTypeError(
        "cannot validate AssignStatement, rhs does not have a type. ");
  } else if (stm->rhs_expression->classic_type->type == INDEFINABLE_TYPE) {
    return std::vector<SemanticErrorMessage>{SemanticErrorMessage(
        WRONG_ASSIGNMENT_TYPE,
        "cannot assign an expression of indefinable type. ")};
  }
  ClassicType lhs_type = this->proxy.get_variable(stm->lhs_id);
  if (lhs_type == nullptr || eq(lhs_type, stm->rhs_expression->classic_type)) {
    this->proxy.update_variable(stm->lhs_id, stm->rhs_expression->classic_type);
    return std::vector<SemanticErrorMessage>{};
  }
  return std::vector<SemanticErrorMessage>{SemanticErrorMessage(
      WRONG_ASSIGNMENT_TYPE, "cannot assign an expression of type " +
                                 str(stm->rhs_expression->classic_type) +
                                 " to a variable of type " + str(lhs_type) +
                                 " (" + stm->lhs_id + "). ")};
}

std::vector<SemanticErrorMessage> Validator::validate(VariableExpression exp) {
  if (this->proxy.get_variable(exp->var_name) == nullptr) {
    return std::vector<SemanticErrorMessage>{SemanticErrorMessage(
        UNDEFINED_VARIABLE, "variable " + exp->var_name + " is not defined. ")};
  }
  return std::vector<SemanticErrorMessage>{};
}

std::vector<SemanticErrorMessage> Validator::validate(
    FunctionCallExpression exp) {
  FunctionSignature signature = this->proxy.get_function(exp->function_name);
  if (signature == nullptr) {
    return std::vector<SemanticErrorMessage>{SemanticErrorMessage(
        UNDEFINED_FUNCTION,
        "function " + exp->function_name + " has not been defined. ")};
  }
  return this->compare(signature->param_list, exp->argument_list);
}

std::vector<SemanticErrorMessage> Validator::validate(
    BinaryOperationExpression exp) {
  if (exp->left_expression->classic_type == nullptr) {
    throw NoTypeError(
        "cannot validate binary operation, left operand does not have a "
        "type. ");
  } else if (exp->left_expression->classic_type->type == CUSTOM_TYPE) {
    return std::vector<SemanticErrorMessage>{SemanticErrorMessage(
        INVALID_BINARY_OPERATION_TYPE,
        "left operand of binary operation has custom type. ")};
  } else if (exp->left_expression->classic_type->type == INDEFINABLE_TYPE) {
    return std::vector<SemanticErrorMessage>{SemanticErrorMessage(
        INVALID_BINARY_OPERATION_TYPE,
        "left operand of binary operation has indefinable type. ")};
  }

  if (exp->right_expression->classic_type == nullptr) {
    throw NoTypeError(
        "cannot validate binary operation, right operand does not have a "
        "type. ");
  } else if (exp->right_expression->classic_type->type == CUSTOM_TYPE) {
    return std::vector<SemanticErrorMessage>{SemanticErrorMessage(
        INVALID_BINARY_OPERATION_TYPE,
        "right operand of binary operation has custom type. ")};
  } else if (exp->right_expression->classic_type->type == INDEFINABLE_TYPE) {
    return std::vector<SemanticErrorMessage>{SemanticErrorMessage(
        INVALID_BINARY_OPERATION_TYPE,
        "right operand of binary operation has indefinable type. ")};
  }

  if (!eq(exp->left_expression->classic_type,
          exp->right_expression->classic_type)) {
    return std::vector<SemanticErrorMessage>{SemanticErrorMessage(
        INCOMPATIBLE_BINARY_OPERATION_TYPES,
        "operands of binary operation have different types: (left) " +
            str(exp->left_expression->classic_type) + ", (right) " +
            str(exp->right_expression->classic_type))};
  }
  return std::vector<SemanticErrorMessage>{};
}

std::vector<SemanticErrorMessage> Validator::compare(ParamList param_list,
                                                     ArgumentList arg_list) {
  std::map<std::string, ClassicType> args{};
  for (auto itr = ArgumentListIterator(arg_list),
            end = ArgumentListIterator(nullptr);
       itr != end; ++itr) {
    if (itr->expression->classic_type == nullptr) {
      throw NoTypeError(
          "cannot validate argument list, expression of argument " + itr->name +
          " does not have a classic type. ");
    }
    args.insert(std::make_pair(itr->name, itr->expression->classic_type));
  }

  std::map<std::string, ClassicType> params{};
  for (auto itr = ParamListIterator(param_list),
            end = ParamListIterator(nullptr);
       itr != end; ++itr) {
    params.insert(std::make_pair(itr->name, itr->classic_type));
  }

  std::vector<SemanticErrorMessage> errs{};
  for (auto itr = args.begin(); itr != args.end(); ++itr) {
    auto p = params.find(itr->first);
    if (p == params.end()) {
      errs.push_back(SemanticErrorMessage(
          UNDECLARED_ARGUMENT,
          "argument " + itr->first +
              " is not a parameter of the called function. "));
    } else {
      if (!eq(p->second, itr->second)) {
        errs.push_back(SemanticErrorMessage(
            WRONG_ARGUMENT_TYPE,
            "argument " + itr->first + " has wrong type. "));
      }
    }
  }
  if (args.size() < params.size()) {
    errs.push_back(SemanticErrorMessage(WRONG_ARGUMENT_NUMBER));
  }
  return errs;
}
