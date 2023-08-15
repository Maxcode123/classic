#pragma once

#include <iostream>

// Denotes occurence of error during building of the AST.
class ASTBuildError : public std::exception {
 public:
  const char *what() { return message.c_str(); }
  ASTBuildError(char *msg) { message = msg; }
  ASTBuildError(const std::string msg) { message = msg; }

 private:
  std::string message;
};

/* Denotes that the AST stack is empty; Should only be thrown when attempting
 to pop the stack. */
class EmptyASTStackError : public std::exception {
 public:
  const char *what() { return message.c_str(); }
  EmptyASTStackError(char *msg) { message = msg; }
  EmptyASTStackError(const std::string msg) { message = msg; }

 private:
  std::string message;
};

class IteratorError : public std::exception {
 public:
  const char *what() { return message.c_str(); }
  IteratorError(char *msg) { message = msg; }
  IteratorError(const std::string msg) { message = msg; }

 private:
  std::string message;
};
