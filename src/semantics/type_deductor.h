#pragma once

#include "../syntax/ast/nodes.h"
#include "environment.h"

class TypeDeductor {
 public:
  TypeDeductor(EnvironmentsProxy p) { proxy = p; }
  TypeDeductor() {}

  ClassicType deduce(VariableExpression exp);
  ClassicType deduce(FunctionCallExpression exp);

 private:
  EnvironmentsProxy proxy;
};
