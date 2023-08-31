#pragma once

#include <iostream>

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

// Thrown wherever a custom type is expected to be used (until custom types are
// supported).
class CustomTypesNotSupported : public std::exception {
 public:
  const char *what() { return message.c_str(); }
  CustomTypesNotSupported(char *msg) { message = msg; }
  CustomTypesNotSupported(const std::string msg) { message = msg; }

 private:
  std::string message;
};