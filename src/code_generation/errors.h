#pragma once

#include <iostream>

// Unknown builtin type that is encountered during code generation.
class UnknownBuiltinType : public std::exception {
 public:
  const char *what() { return message.c_str(); }
  UnknownBuiltinType(char *msg) { message = msg; }
  UnknownBuiltinType(const std::string msg) { message = msg; }

 private:
  std::string message;
};
