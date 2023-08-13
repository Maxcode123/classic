#include <gtest/gtest.h>

#include "../../../src/syntax/ast/errors.h"
#include "../../../src/utils/error_handler.h"

class MockError : public std::exception {
 public:
  const char *what() { return message.c_str(); }
  MockError(char *msg) { message = msg; }
  MockError(const std::string msg) { message = msg; }

 private:
  std::string message;
};

typedef class ReturnType_ {
} *ReturnType;

void *_throw_error() { throw MockError("mock error"); }
ReturnType _dont_throw() { return new ReturnType_(); }

class Caller {
 public:
  void *throw_error() { return _throw_error(); }
  ReturnType dont_throw() { return _dont_throw(); }
};

int flag;

void empty_callback(MockError &e) {}

void incr_flag_callback(MockError &e) { flag++; }

#define HANDLE_METHOD_ERROR \
  (handler.handle<void *, Caller>(&this->caller, &Caller::throw_error))

#define HANDLE_METHOD_NO_ERROR \
  (handler.handle<ReturnType, Caller>(&this->caller, &Caller::dont_throw))

#define HANDLE_FUNCTION_ERROR (handler.handle<void *>(&_throw_error))

#define HANDLE_FUNCTION_NO_ERROR (handler.handle<ReturnType>(&_dont_throw))

class ErrorHandlerTest : public testing::Test {
 protected:
  Caller caller;
  void SetUp() override {
    this->caller = Caller();
    flag = 0;
  }
};

class ErrorHandlerDeathTest : public ErrorHandlerTest {};

TEST_F(ErrorHandlerDeathTest, TestMethodExitStrategy) {
  ErrorHandler<MockError> handler =
      ErrorHandler<MockError>(ERROR_HANDLING_EXIT);
  EXPECT_EXIT(HANDLE_METHOD_ERROR, testing::ExitedWithCode(EXIT_FAILURE),
              "mock error");
}

TEST_F(ErrorHandlerTest, TestMethodExitStrategyDontThrow) {
  ErrorHandler<MockError> handler =
      ErrorHandler<MockError>(ERROR_HANDLING_EXIT);
  EXPECT_NO_FATAL_FAILURE(HANDLE_METHOD_NO_ERROR);
}

TEST_F(ErrorHandlerDeathTest, TestFunctionExitStrategy) {
  ErrorHandler<MockError> handler =
      ErrorHandler<MockError>(ERROR_HANDLING_EXIT);
  EXPECT_EXIT(HANDLE_FUNCTION_ERROR, testing::ExitedWithCode(EXIT_FAILURE),
              "mock error");
}

TEST_F(ErrorHandlerTest, TestFunctionExitStrategyDontThrow) {
  ErrorHandler<MockError> handler =
      ErrorHandler<MockError>(ERROR_HANDLING_EXIT);
  EXPECT_NO_FATAL_FAILURE(HANDLE_FUNCTION_NO_ERROR);
}

TEST_F(ErrorHandlerDeathTest, TestMethodCallbackStrategyNoCallback) {
  ErrorHandler<MockError> handler =
      ErrorHandler<MockError>(ERROR_HANDLING_CALLBACK);
  EXPECT_EXIT(HANDLE_METHOD_ERROR, testing::ExitedWithCode(EXIT_FAILURE),
              "No callback provided, exiting program");
}

TEST_F(ErrorHandlerDeathTest, TestFunctionCallbackStrategyNoCallback) {
  ErrorHandler<MockError> handler =
      ErrorHandler<MockError>(ERROR_HANDLING_CALLBACK);
  EXPECT_EXIT(HANDLE_FUNCTION_ERROR, testing::ExitedWithCode(EXIT_FAILURE),
              "No callback provided, exiting program");
}

TEST_F(ErrorHandlerTest, TestMethodIgnoreStrategy) {
  ErrorHandler<MockError> handler =
      ErrorHandler<MockError>(ERROR_HANDLING_IGNORE);
  EXPECT_NO_THROW(HANDLE_METHOD_ERROR);
}

TEST_F(ErrorHandlerTest, TestFunctionIgnoreStrategy) {
  ErrorHandler<MockError> handler =
      ErrorHandler<MockError>(ERROR_HANDLING_IGNORE);
  EXPECT_NO_THROW(HANDLE_FUNCTION_ERROR);
}

TEST_F(ErrorHandlerTest, TestMethodCallbackStrategyEmptyCallback) {
  ErrorHandler<MockError> handler = ErrorHandler<MockError>(&empty_callback);
  EXPECT_NO_THROW(HANDLE_METHOD_ERROR);
}

TEST_F(ErrorHandlerTest, TestFunctionCallbackStrategyEmptyCallback) {
  ErrorHandler<MockError> handler = ErrorHandler<MockError>(&empty_callback);
  EXPECT_NO_THROW(HANDLE_FUNCTION_ERROR);
}

TEST_F(ErrorHandlerTest, TestMethodCallbackStrategyIncrFlagCallback) {
  ErrorHandler<MockError> handler =
      ErrorHandler<MockError>(&incr_flag_callback);
  HANDLE_METHOD_ERROR;
  EXPECT_EQ(flag, 1);
  HANDLE_METHOD_ERROR;
  EXPECT_EQ(flag, 2);
}

TEST_F(ErrorHandlerTest, TestFunctionCallbackStrategyIncrFlagCallback) {
  ErrorHandler<MockError> handler =
      ErrorHandler<MockError>(&incr_flag_callback);
  HANDLE_FUNCTION_ERROR;
  EXPECT_EQ(flag, 1);
  HANDLE_FUNCTION_ERROR;
  EXPECT_EQ(flag, 2);
}
