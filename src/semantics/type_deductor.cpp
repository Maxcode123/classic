#include "type_deductor.h"

#include "errors.h"

ClassicType TypeDeductor::deduce(Expression exp) {
  switch (exp->type) {
    case LITERAL_EXPRESSION:
      return exp->downcast<LiteralExpression>()->classic_type;
    case VARIABLE_EXPRESSION:
      return this->proxy.get_variable(
          exp->downcast<VariableExpression>()->var_name);
    case FUNCTION_CALL_EXPRESSION:
      return this->proxy
          .get_function(exp->downcast<FunctionCallExpression>()->function_name)
          ->return_type;
    case BINARY_OPERATION_EXPRESSION:
      throw TypeDeductionError(
          "Deduction of binary operation expression type is not supported.");
    case PARENTHESES_EXPRESSION:
      return this->deduce(exp->downcast<ParenthesesExpression>()->expression);
    default:
      return nullptr;
  }
}