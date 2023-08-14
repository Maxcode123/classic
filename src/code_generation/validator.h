#pragma once

#include "../syntax/ast/nodes.h"

class Validator {
 public:
  static bool is_valid();
  static bool type_check();
  static bool declaration_check();
  static bool type_and_declaration_check();
  static void infer_type(Expression exp);
};