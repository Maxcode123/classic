#pragma once

#include <iostream>

#include "../utils/errors.h"

// No type where one is expected (has not been set during semantic analysis).
class NoClassicTypeError : public std::exception {
 public:
  const char *what() { return message.c_str(); }
  NoClassicTypeError(char *msg) { message = msg; }
  NoClassicTypeError(const std::string msg) { message = msg; }

 private:
  std::string message;
};

// Unknown builtin type that is encountered during code generation.
class UnknownBuiltinType : public std::exception {
 public:
  const char *what() { return message.c_str(); }
  UnknownBuiltinType(char *msg) { message = msg; }
  UnknownBuiltinType(const std::string msg) { message = msg; }

 private:
  std::string message;
};