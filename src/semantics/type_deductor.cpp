#include "type_deductor.h"

#include "errors.h"

ClassicType TypeDeductor::deduce(VariableExpression exp) {
  return this->proxy.get_variable(exp->var_name);
}

ClassicType TypeDeductor::deduce(FunctionCallExpression exp) {
  return this->proxy.get_function(exp->function_name)->return_type;
}