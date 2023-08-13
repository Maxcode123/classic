#pragma once

#include <iostream>

template <class T_err>
class ErrorHandler;
template <class T_err>
class EmbeddedErrorHandler;

enum ErrorHandlingStrategy {
  ERROR_HANDLING_EXIT,
  ERROR_HANDLING_IGNORE,
  ERROR_HANDLING_CALLBACK,
};

template <class T_err>
class ErrorHandler {
 public:
  ErrorHandlingStrategy strategy;

  // T_return must be a pointer
  template <class T_return, class T_caller>
  T_return handle(T_caller *caller, T_return (T_caller::*func)()) {
    T_return result = nullptr;
    try {
      result = (caller->*func)();
    } catch (T_err &e) {
      this->execute_strategy(e);
    }
    return result;
  }

  // T_return must be a pointer
  template <class T_return>
  T_return handle(T_return (*func)()) {
    T_return result = nullptr;
    try {
      result = func();
    } catch (T_err &e) {
      this->execute_strategy(e);
    }
    return result;
  }

  ErrorHandler() { strategy = ERROR_HANDLING_EXIT; }
  ErrorHandler(ErrorHandlingStrategy s) { strategy = s; }
  ErrorHandler(void (*cb)(T_err &)) {
    strategy = ERROR_HANDLING_CALLBACK;
    callback = cb;
  }

 protected:
  void (*callback)(T_err &) = nullptr;

  void execute_strategy(T_err &e) {
    switch (this->strategy) {
      case ERROR_HANDLING_EXIT:
        error_handling_exit(e);
        break;
      case ERROR_HANDLING_IGNORE:
        error_handling_ignore();
        break;
      case ERROR_HANDLING_CALLBACK:
        if (this->callback == nullptr) {
          std::cerr << "No callback provided, exiting program:\n";
          error_handling_exit(e);
          break;
        }
        this->callback(e);
    }
  }

  void error_handling_exit(T_err &e) {
    std::cerr << e.what() << "\n";
    exit(EXIT_FAILURE);
  };

  void error_handling_ignore() {}
};
