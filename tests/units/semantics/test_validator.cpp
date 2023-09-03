#include <gtest/gtest.h>

#include "../../../src/semantics/errors.h"
#include "../../../src/semantics/validator.h"

class ValidatorTest : public testing::Test {
 protected:
  EnvironmentsProxy proxy;

  Validator validator;

  void SetUp() override {
    VariableEnvironmentProxy vars =
        VariableEnvironmentProxy(new VariableEnvironment_());
    FunctionEnvironmentProxy funcs =
        FunctionEnvironmentProxy(new FunctionEnvironment_());
    this->proxy = EnvironmentsProxy(vars, funcs);
    this->validator = Validator(this->proxy);
  }

  FunctionSignature build_signature() {
    Param a = new Param_(
        (new ClassicBuiltinType_(classic_builtin_types::STR))->upcast(), "a");
    Param b = new Param_(
        (new ClassicBuiltinType_(classic_builtin_types::INT))->upcast(), "b");
    ParamList param_list =
        (new PairParamList_((new LastParamList_(b))->upcast(), a))->upcast();

    FunctionSignature fs = new FunctionSignature_(
        "test_func",
        (new ClassicBuiltinType_(classic_builtin_types::ANEF))->upcast(),
        param_list);
    return fs;
  }

  ArgumentList build_argument_list(std::vector<Argument> args) {
    if (args.size() == 0)
      return (new EmptyArgumentList_())->upcast();
    else if (args.size() == 1)
      return (new LastArgumentList_(args.front()))->upcast();
    ArgumentList rest = this->build_argument_list(
        std::vector<Argument>{++args.begin(), args.end()});
    PairArgumentList pair_list = new PairArgumentList_(args.front(), rest);
    return pair_list->upcast();
  }

  template <class T>
  LiteralExpression build_literal(T val) {
    return new LiteralExpression_(val);
  }

  Expression build_typed_literal(std::string val) {
    LiteralExpression lit = this->build_literal<std::string>(val);
    lit->set_classic_type(
        (new ClassicBuiltinType_(classic_builtin_types::STR))->upcast());
    Expression exp = lit->upcast();
    exp->set_classic_type(lit->classic_type);
    return exp;
  }

  Expression build_typed_literal(int val) {
    LiteralExpression lit = this->build_literal<int>(val);
    lit->set_classic_type(
        (new ClassicBuiltinType_(classic_builtin_types::INT))->upcast());
    Expression exp = lit->upcast();
    exp->set_classic_type(lit->classic_type);
    return exp;
  }

  Expression build_typed_literal(double val) {
    LiteralExpression lit = this->build_literal<double>(val);
    lit->set_classic_type(
        (new ClassicBuiltinType_(classic_builtin_types::DUPL))->upcast());
    Expression exp = lit->upcast();
    exp->set_classic_type(lit->classic_type);
    return exp;
  }
};

TEST_F(ValidatorTest, TestValidateParamBuiltinType) {
  ClassicType t =
      (new ClassicBuiltinType_(classic_builtin_types::ANEF))->upcast();
  Param p = new Param_(t, "a");
  std::vector<SemanticErrorMessage> errs = this->validator.validate(p);
  EXPECT_EQ(errs.size(), 0);
}

TEST_F(ValidatorTest, TestValidateParamIndefinableType) {
  ClassicType t = (new ClassicIndefinableType_())->upcast();
  Param p = new Param_(t, "a");
  std::vector<SemanticErrorMessage> errs = this->validator.validate(p);
  EXPECT_EQ(errs.size(), 1);

  EXPECT_EQ(errs.begin()->type, UNDEFINED_TYPE);
}

TEST_F(ValidatorTest,
       TestValidateAssignStatementUndeclaredVariableNoTypeError) {
  Expression exp = (new LiteralExpression_(2))->upcast();
  AssignStatement stm = new AssignStatement_("c", exp);
  // exp does not have a classic type
  EXPECT_THROW(this->validator.validate(stm), NoTypeError);
}

TEST_F(ValidatorTest, TestValidateAssignStatementUndeclaredVariable) {
  Expression exp = this->build_typed_literal(2);
  AssignStatement stm = new AssignStatement_("c", exp);
  std::vector<SemanticErrorMessage> errs = this->validator.validate(stm);
  EXPECT_EQ(errs.size(), 0);
  EXPECT_NE(this->proxy.get_variable("c"), nullptr);
}

TEST_F(ValidatorTest,
       TestValidateAssignStatementUndeclaredVariableIndefinableExpression) {
  Expression exp = (new LiteralExpression_(2))->upcast();
  exp->set_classic_type((new ClassicIndefinableType_())->upcast());
  AssignStatement stm = new AssignStatement_("c", exp);
  std::vector<SemanticErrorMessage> errs = this->validator.validate(stm);
  EXPECT_EQ(this->proxy.get_variable("c"), nullptr);
  EXPECT_EQ(errs.size(), 1);

  EXPECT_EQ(errs.begin()->type, WRONG_ASSIGNMENT_TYPE);
}

TEST_F(ValidatorTest, TestValidateAssignStatementDeclaredVariable) {
  this->proxy.update_variable(
      "u", (new ClassicBuiltinType_(classic_builtin_types::DUPL))->upcast());
  Expression exp = this->build_typed_literal(.2);
  AssignStatement stm = new AssignStatement_("u", exp);
  std::vector<SemanticErrorMessage> errs = this->validator.validate(stm);
  EXPECT_EQ(errs.size(), 0);
}

TEST_F(ValidatorTest, TestValidateAssignStatementDeclaredVariableWrongType) {
  this->proxy.update_variable(
      "u", (new ClassicBuiltinType_(classic_builtin_types::DUPL))->upcast());
  Expression exp = this->build_typed_literal(6);
  AssignStatement stm = new AssignStatement_("u", exp);
  std::vector<SemanticErrorMessage> errs = this->validator.validate(stm);
  EXPECT_EQ(errs.size(), 1);
  EXPECT_EQ(errs.begin()->type, WRONG_ASSIGNMENT_TYPE);
}

TEST_F(ValidatorTest,
       TestValidateAssignStatementDeclaredVariableIndefinableExpression) {
  this->proxy.update_variable(
      "u", (new ClassicBuiltinType_(classic_builtin_types::DUPL))->upcast());
  Expression exp = (new LiteralExpression_(.2))->upcast();
  exp->set_classic_type((new ClassicIndefinableType_())->upcast());
  AssignStatement stm = new AssignStatement_("u", exp);
  std::vector<SemanticErrorMessage> errs = this->validator.validate(stm);
  EXPECT_EQ(errs.size(), 1);
  EXPECT_EQ(errs.begin()->type, WRONG_ASSIGNMENT_TYPE);
}

TEST_F(ValidatorTest, TestValidateVariableExpressionNoError) {
  this->proxy.update_variable("f",
                              (new ClassicCustomType_("MyClass"))->upcast());
  VariableExpression exp = new VariableExpression_("f");
  std::vector<SemanticErrorMessage> errs = this->validator.validate(exp);
  EXPECT_EQ(errs.size(), 0);
}

TEST_F(ValidatorTest, TestValidateVariableExpressionUndefinedVar) {
  VariableExpression exp = new VariableExpression_("f");
  std::vector<SemanticErrorMessage> errs = this->validator.validate(exp);
  EXPECT_EQ(errs.size(), 1);
  EXPECT_EQ(errs.begin()->type, UNDEFINED_VARIABLE);
}

TEST_F(ValidatorTest, TestValidateFunctionCallExpression) {
  FunctionSignature fs = new FunctionSignature_(
      "myfunc",
      (new ClassicBuiltinType_(classic_builtin_types::ANEF))->upcast(),
      (new EmptyParamList_())->upcast());
  this->proxy.update_function(fs->name, fs);

  FunctionCallExpression exp = new FunctionCallExpression_(
      "myfunc", (new EmptyArgumentList_())->upcast());
  std::vector<SemanticErrorMessage> errs = this->validator.validate(exp);
  EXPECT_EQ(errs.size(), 0);
}

TEST_F(ValidatorTest, TestValidateFunctionCallExpressionUndefinedFunction) {
  FunctionCallExpression exp = new FunctionCallExpression_(
      "myfunc", (new EmptyArgumentList_())->upcast());
  std::vector<SemanticErrorMessage> errs = this->validator.validate(exp);
  EXPECT_EQ(errs.size(), 1);
  EXPECT_EQ(errs.begin()->type, UNDEFINED_FUNCTION);
}

TEST_F(ValidatorTest, TestValidateFunctionCallExpressionNoTypeError) {
  FunctionSignature fs = this->build_signature();
  this->proxy.update_function(fs->name, fs);

  // exp does not have a type
  Argument a = new Argument_("a", (new LiteralExpression_("mystr"))->upcast());
  Argument b = new Argument_("b", (new LiteralExpression_(9))->upcast());
  ArgumentList arg_list =
      (new PairArgumentList_(a, (new LastArgumentList_(b))->upcast()))
          ->upcast();
  FunctionCallExpression exp =
      new FunctionCallExpression_("test_func", arg_list);
  EXPECT_THROW(this->validator.validate(exp), NoTypeError);
}

TEST_F(ValidatorTest, TestValidateFunctionCallExpressionUndeclaredArgument) {
  FunctionSignature fs = this->build_signature();
  this->proxy.update_function(fs->name, fs);

  Argument a = new Argument_("a", this->build_typed_literal("mystr"));
  Argument b = new Argument_("b", this->build_typed_literal(9));
  Argument c = new Argument_("c", this->build_typed_literal(2.3));

  ArgumentList arg_list =
      this->build_argument_list(std::vector<Argument>{a, b, c});

  FunctionCallExpression exp =
      new FunctionCallExpression_("test_func", arg_list);

  std::vector<SemanticErrorMessage> errs = this->validator.validate(exp);
  EXPECT_EQ(errs.size(), 1);
  EXPECT_EQ(errs.begin()->type, UNDECLARED_ARGUMENT);
}

TEST_F(ValidatorTest, TestValidateFunctionCallExpressionWrongArgumentType) {
  FunctionSignature fs = this->build_signature();
  this->proxy.update_function(fs->name, fs);

  Argument a = new Argument_("a", this->build_typed_literal("mystr"));
  Argument b = new Argument_("b", this->build_typed_literal(9.0));

  ArgumentList arg_list =
      this->build_argument_list(std::vector<Argument>{a, b});

  FunctionCallExpression exp =
      new FunctionCallExpression_("test_func", arg_list);

  std::vector<SemanticErrorMessage> errs = this->validator.validate(exp);
  EXPECT_EQ(errs.size(), 1);
  EXPECT_EQ(errs.begin()->type, WRONG_ARGUMENT_TYPE);
}

TEST_F(ValidatorTest, TestValidateFunctionCallExpressionWrongArgumentNumber) {
  FunctionSignature fs = this->build_signature();
  this->proxy.update_function(fs->name, fs);

  Argument a = new Argument_("a", this->build_typed_literal("mystr"));
  ArgumentList arg_list = this->build_argument_list(std::vector<Argument>{a});

  FunctionCallExpression exp =
      new FunctionCallExpression_("test_func", arg_list);

  std::vector<SemanticErrorMessage> errs = this->validator.validate(exp);
  EXPECT_EQ(errs.size(), 1);
  EXPECT_EQ(errs.begin()->type, WRONG_ARGUMENT_NUMBER);
}

TEST_F(ValidatorTest, TestValidateFunctionCallExpressionMultipleErrors) {
  FunctionSignature fs = this->build_signature();
  this->proxy.update_function(fs->name, fs);

  Argument a = new Argument_("a", this->build_typed_literal(1));
  Argument c = new Argument_("c", this->build_typed_literal(9.0));

  ArgumentList arg_list =
      this->build_argument_list(std::vector<Argument>{a, c});

  FunctionCallExpression exp =
      new FunctionCallExpression_("test_func", arg_list);

  std::vector<SemanticErrorMessage> errs = this->validator.validate(exp);
  EXPECT_EQ(errs.size(), 2);
  auto itr = errs.begin();
  EXPECT_EQ((itr++)->type, WRONG_ARGUMENT_TYPE);
  EXPECT_EQ(itr->type, UNDECLARED_ARGUMENT);
}

TEST_F(ValidatorTest, TestValidateBinaryOperationExpressionLeftNoTypeError) {
  Expression left = this->build_literal<int>(2)->upcast();
  Expression right = this->build_typed_literal(1);
  BinaryOperationExpression exp =
      new BinaryOperationExpression_(BINARY_DIV, left, right);
  EXPECT_THROW(this->validator.validate(exp), NoTypeError);
}

TEST_F(ValidatorTest, TestValidateBinaryOperationExpressionLeftCustomType) {
  Expression left = (new VariableExpression_("b"))->upcast();
  left->set_classic_type((new ClassicCustomType_("B"))->upcast());
  Expression right = this->build_typed_literal(9);
  BinaryOperationExpression exp =
      new BinaryOperationExpression_(BINARY_TIMES, left, right);
  std::vector<SemanticErrorMessage> errs = this->validator.validate(exp);
  EXPECT_EQ(errs.size(), 1);
  EXPECT_EQ(errs.begin()->type, INVALID_BINARY_OPERATION_TYPE);
}

TEST_F(ValidatorTest,
       TestValidateBinaryOperationExpressionLeftIndefinableType) {
  Expression left = (new VariableExpression_("b"))->upcast();
  left->set_classic_type((new ClassicIndefinableType_())->upcast());
  Expression right = this->build_typed_literal(9);
  BinaryOperationExpression exp =
      new BinaryOperationExpression_(BINARY_TIMES, left, right);
  std::vector<SemanticErrorMessage> errs = this->validator.validate(exp);
  EXPECT_EQ(errs.size(), 1);
  EXPECT_EQ(errs.begin()->type, INVALID_BINARY_OPERATION_TYPE);
}

TEST_F(ValidatorTest, TestValidateBinaryOperationExpressionRightNoTypeError) {
  Expression left = this->build_typed_literal(5);
  Expression right = this->build_literal(3)->upcast();
  BinaryOperationExpression exp =
      new BinaryOperationExpression_(BINARY_MINUS, left, right);
  EXPECT_THROW(this->validator.validate(exp), NoTypeError);
}

TEST_F(ValidatorTest, TestValidateBinaryOperationExpressionRightCustomType) {
  Expression left = this->build_typed_literal(9);
  Expression right = (new VariableExpression_("b"))->upcast();
  right->set_classic_type((new ClassicCustomType_("B"))->upcast());
  BinaryOperationExpression exp =
      new BinaryOperationExpression_(BINARY_TIMES, left, right);
  std::vector<SemanticErrorMessage> errs = this->validator.validate(exp);
  EXPECT_EQ(errs.size(), 1);
  EXPECT_EQ(errs.begin()->type, INVALID_BINARY_OPERATION_TYPE);
}

TEST_F(ValidatorTest,
       TestValidateBinaryOperationExpressionRightIndefinableType) {
  Expression left = this->build_typed_literal(9);
  Expression right = (new VariableExpression_("b"))->upcast();
  right->set_classic_type((new ClassicIndefinableType_())->upcast());
  BinaryOperationExpression exp =
      new BinaryOperationExpression_(BINARY_TIMES, left, right);
  std::vector<SemanticErrorMessage> errs = this->validator.validate(exp);
  EXPECT_EQ(errs.size(), 1);
  EXPECT_EQ(errs.begin()->type, INVALID_BINARY_OPERATION_TYPE);
}

TEST_F(ValidatorTest, TestValidateBinaryOperationExpressionDifferentTypes) {
  Expression left = this->build_typed_literal(2);
  Expression right = this->build_typed_literal(2.1);
  BinaryOperationExpression exp =
      new BinaryOperationExpression_(BINARY_PLUS, left, right);
  std::vector<SemanticErrorMessage> errs = this->validator.validate(exp);
  EXPECT_EQ(errs.size(), 1);
  EXPECT_EQ(errs.begin()->type, INCOMPATIBLE_BINARY_OPERATION_TYPES);
}

TEST_F(ValidatorTest, TestValidateBinaryOperationExpression) {
  Expression left = this->build_typed_literal(4);
  Expression right = this->build_typed_literal(5);
  BinaryOperationExpression exp =
      new BinaryOperationExpression_(BINARY_MINUS, left, right);
  std::vector<SemanticErrorMessage> errs = this->validator.validate(exp);
  EXPECT_EQ(errs.size(), 0);
}
