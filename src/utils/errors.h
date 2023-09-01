#pragma once

#include <iostream>

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