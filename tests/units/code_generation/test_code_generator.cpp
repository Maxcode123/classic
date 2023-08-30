#include <gtest/gtest.h>

#include "../../../src/code_generation/code_generator.h"
#include "../../../src/code_generation/symbol_table.h"
#include "../../../src/syntax/ast/nodes.h"

#define INT64_TYPE llvm::Type::getInt64Ty(*this->context)
#define FLOAT_TYPE llvm::Type::getFloatTy(*this->context)
#define INT1_TYPE llvm::Type::getInt1Ty(*this->context)

class CodeGeneratorTest : public testing::Test {
 protected:
  llvm::LLVMContext* context;
  llvm::Module* module;
  llvm::IRBuilder<>* ir_builder;

  SymbolTable symbol_table;
  SymbolTableProxy proxy;

  CodeGenerator code_generator;

  llvm::Function* test_func;

  void SetUp() override {
    this->context = new llvm::LLVMContext();
    this->module = new llvm::Module("test_code_generator.cpp", *this->context);
    this->ir_builder = new llvm::IRBuilder<>(*this->context);

    this->symbol_table = new SymbolTable_();
    this->proxy = SymbolTableProxy(this->symbol_table);

    this->code_generator = CodeGenerator(this->proxy, this->context,
                                         this->module, this->ir_builder);

    this->test_func = this->get_test_func();
  }

  llvm::Function* get_test_func() {
    llvm::FunctionType* ft = llvm::FunctionType::get(INT64_TYPE, false);
    llvm::Function* f = llvm::Function::Create(
        ft, llvm::Function::ExternalLinkage, "test_func", this->module);
    return f;
  }

  void create_basic_block() {
    llvm::BasicBlock* bb =
        llvm::BasicBlock::Create(*this->context, "test_entry", this->test_func);
    this->ir_builder->SetInsertPoint(bb);
  }

  llvm::AllocaInst* allocate(llvm::Type* t, std::string name) {
    llvm::AllocaInst* mem_ptr = this->ir_builder->CreateAlloca(t);
    this->proxy.update(name, mem_ptr);
    return mem_ptr;
  }

  llvm::StoreInst* store(llvm::Value* v, llvm::AllocaInst* mem_ptr,
                         std::string name) {
    llvm::StoreInst* store = this->ir_builder->CreateStore(v, mem_ptr);
    this->proxy.update(name, store);
    return store;
  }

  void allocate_and_store(llvm::Value* value, llvm::Type* t, std::string name) {
    llvm::AllocaInst* mem_ptr = this->allocate(t, name);
    this->store(value, mem_ptr, name);
  }

  llvm::ConstantInt* create_int64(int val) {
    return llvm::ConstantInt::get(*this->context,
                                  llvm::APInt(INTEGER_BITSIZE, val, true));
  }

  llvm::ConstantFP* create_float(float val) {
    return llvm::ConstantFP::get(*this->context, llvm::APFloat(val));
  }

  llvm::ConstantInt* create_int1(int val) {
    return llvm::ConstantInt::get(*this->context, llvm::APInt(1, val, false));
  }
};

TEST_F(CodeGeneratorTest, TestGenerateLiteralExpressionInt) {
  LiteralExpression exp = new LiteralExpression_(10);
  llvm::ConstantInt* value =
      (llvm::ConstantInt*)this->code_generator.generate(exp);

  EXPECT_EQ(value->getSExtValue(), 10);
  EXPECT_EQ(value->getBitWidth(), 64);
}

TEST_F(CodeGeneratorTest, TestGenerateLiteralExpressionDupl) {
  LiteralExpression exp = new LiteralExpression_(.00267);
  llvm::ConstantFP* value =
      (llvm::ConstantFP*)this->code_generator.generate(exp);

  EXPECT_FLOAT_EQ(value->getValueAPF().convertToFloat(), .00267);
}

TEST_F(CodeGeneratorTest, TestGenerateLiteralExpressionAnef) {
  LiteralExpression exp = new LiteralExpression_(classic_builtin_types::ANEF);
  llvm::ConstantInt* value =
      (llvm::ConstantInt*)this->code_generator.generate(exp);

  EXPECT_EQ(value->getSExtValue(), 0);
  EXPECT_EQ(value->getBitWidth(), 1);
}

TEST_F(CodeGeneratorTest, TestGenerateVariableExpressionInt64) {
  this->create_basic_block();
  this->allocate_and_store(this->create_int64(5), INT64_TYPE, "int_var");

  llvm::StoreInst* value = llvm::dyn_cast<llvm::StoreInst>(
      this->code_generator.generate(new VariableExpression_("int_var")));

  EXPECT_NE(value, nullptr);

  llvm::ConstantInt* const_int =
      llvm::dyn_cast<llvm::ConstantInt>(value->getValueOperand());

  EXPECT_NE(const_int, nullptr);
  EXPECT_EQ(const_int->getSExtValue(), 5);
}

TEST_F(CodeGeneratorTest, TestGenerateVariableExpressionDupl) {
  this->create_basic_block();
  this->allocate_and_store(this->create_float(2.32), FLOAT_TYPE, "dupl_var");

  llvm::StoreInst* value = llvm::dyn_cast<llvm::StoreInst>(
      this->code_generator.generate(new VariableExpression_("dupl_var")));

  EXPECT_NE(value, nullptr);

  llvm::ConstantFP* const_fp =
      llvm::dyn_cast<llvm::ConstantFP>(value->getValueOperand());

  EXPECT_NE(const_fp, nullptr);
  EXPECT_FLOAT_EQ(const_fp->getValueAPF().convertToFloat(), 2.32);
}

TEST_F(CodeGeneratorTest, TestGenerateVariableExpressionAnef) {
  this->create_basic_block();
  this->allocate_and_store(this->create_int1(0), INT1_TYPE, "anef_var");

  llvm::StoreInst* value = llvm::dyn_cast<llvm::StoreInst>(
      this->code_generator.generate(new VariableExpression_("anef_var")));

  EXPECT_NE(value, nullptr);

  llvm::ConstantInt* const_int =
      llvm::dyn_cast<llvm::ConstantInt>(value->getValueOperand());

  EXPECT_NE(const_int, nullptr);
  EXPECT_EQ(const_int->getSExtValue(), 0);
}

TEST_F(CodeGeneratorTest, TestGenerateFunctionCallTestFunc) {
  FunctionCallExpression exp = new FunctionCallExpression_(
      "test_func", (new EmptyArgumentList_())->upcast());

  this->create_basic_block();

  llvm::CallInst* call = (llvm::CallInst*)this->code_generator.generate(exp);
  EXPECT_EQ(call->getCalledFunction(), this->test_func);
  EXPECT_TRUE(call->getAttributes().isEmpty());
}

TEST_F(CodeGeneratorTest, TestGenerateBinaryOperationExpressionSub) {
  LiteralExpression left = new LiteralExpression_(1);
  LiteralExpression right = new LiteralExpression_(10);
  BinaryOperationExpression exp = new BinaryOperationExpression_(
      BINARY_MINUS, left->upcast(), right->upcast());
  exp->set_type(classic_builtin_types::INT);

  llvm::ConstantInt* value =
      llvm::dyn_cast<llvm::ConstantInt>(this->code_generator.generate(exp));
  EXPECT_EQ(value->getSExtValue(), 1 - 10);
}

TEST_F(CodeGeneratorTest, TestGenerateBinaryOperationExpressionFSub) {
  LiteralExpression left = new LiteralExpression_(10.25);
  LiteralExpression right = new LiteralExpression_(1.20);
  BinaryOperationExpression exp = new BinaryOperationExpression_(
      BINARY_MINUS, left->upcast(), right->upcast());
  exp->set_type(classic_builtin_types::DUPL);

  llvm::ConstantFP* value =
      llvm::dyn_cast<llvm::ConstantFP>(this->code_generator.generate(exp));
  EXPECT_FLOAT_EQ(value->getValueAPF().convertToFloat(), 10.25 - 1.20);
}

TEST_F(CodeGeneratorTest, TestGenerateBinaryOperationAdd) {
  LiteralExpression left = new LiteralExpression_(56);
  LiteralExpression right = new LiteralExpression_(44);
  BinaryOperationExpression exp = new BinaryOperationExpression_(
      BINARY_PLUS, left->upcast(), right->upcast());
  exp->set_type(classic_builtin_types::INT);

  llvm::ConstantInt* value =
      llvm::dyn_cast<llvm::ConstantInt>(this->code_generator.generate(exp));
  EXPECT_EQ(value->getSExtValue(), 56 + 44);
}

TEST_F(CodeGeneratorTest, TestGenerateBinaryOperationFAdd) {
  LiteralExpression left = new LiteralExpression_(.25);
  LiteralExpression right = new LiteralExpression_(.21);
  BinaryOperationExpression exp = new BinaryOperationExpression_(
      BINARY_PLUS, left->upcast(), right->upcast());
  exp->set_type(classic_builtin_types::DUPL);

  llvm::ConstantFP* value =
      llvm::dyn_cast<llvm::ConstantFP>(this->code_generator.generate(exp));
  EXPECT_FLOAT_EQ(value->getValueAPF().convertToFloat(), .25 + .21);
}

TEST_F(CodeGeneratorTest, TestGenerateBinaryOperationSDiv) {
  LiteralExpression left = new LiteralExpression_(100);
  LiteralExpression right = new LiteralExpression_(3);
  BinaryOperationExpression exp = new BinaryOperationExpression_(
      BINARY_DIV, left->upcast(), right->upcast());
  exp->set_type(classic_builtin_types::INT);

  llvm::ConstantInt* value =
      llvm::dyn_cast<llvm::ConstantInt>(this->code_generator.generate(exp));
  EXPECT_EQ(value->getSExtValue(), 33);
}

TEST_F(CodeGeneratorTest, TestGenerateBinaryOperationFDiv) {
  LiteralExpression left = new LiteralExpression_(100.3);
  LiteralExpression right = new LiteralExpression_(3.0);
  BinaryOperationExpression exp = new BinaryOperationExpression_(
      BINARY_DIV, left->upcast(), right->upcast());
  exp->set_type(classic_builtin_types::DUPL);

  llvm::ConstantFP* value =
      llvm::dyn_cast<llvm::ConstantFP>(this->code_generator.generate(exp));
  EXPECT_FLOAT_EQ(value->getValueAPF().convertToFloat(), 100.3 / 3.0);
}

TEST_F(CodeGeneratorTest, TestGenerateBinaryOperationMul) {
  LiteralExpression left = new LiteralExpression_(55);
  LiteralExpression right = new LiteralExpression_(6);
  BinaryOperationExpression exp = new BinaryOperationExpression_(
      BINARY_TIMES, left->upcast(), right->upcast());
  exp->set_type(classic_builtin_types::INT);

  llvm::ConstantInt* value =
      llvm::dyn_cast<llvm::ConstantInt>(this->code_generator.generate(exp));
  EXPECT_EQ(value->getSExtValue(), 55 * 6);
}

TEST_F(CodeGeneratorTest, TestGenerateBinaryOperationFMul) {
  LiteralExpression left = new LiteralExpression_(12.36);
  LiteralExpression right = new LiteralExpression_(9.0194);
  BinaryOperationExpression exp = new BinaryOperationExpression_(
      BINARY_TIMES, left->upcast(), right->upcast());
  exp->set_type(classic_builtin_types::DUPL);

  llvm::ConstantFP* value =
      llvm::dyn_cast<llvm::ConstantFP>(this->code_generator.generate(exp));
  EXPECT_FLOAT_EQ(value->getValueAPF().convertToFloat(), 12.36 * 9.0194);
}

// TEST_F(CodeGeneratorTest, TestGenerateBinaryOperationAddVariableExpressions)
// {
//   llvm::AllocaInst* var_1 =
//       this->allocate(llvm::Type::getInt64Ty(*this->context), "var_1");

//   llvm::AllocaInst* var_2 =
//       this->allocate(llvm::Type::getInt64Ty(*this->context), "var_2");
// }

TEST_F(CodeGeneratorTest, TestGenerateArgumentType) {
  Argument arg = new Argument_("myint", (new LiteralExpression_(10))->upcast());
  llvm::Value* value = this->code_generator.generate(arg);

  EXPECT_TRUE(value->getType()->isIntegerTy());
}

TEST_F(CodeGeneratorTest, TestGenerateArgumentUses) {
  Argument arg = new Argument_("arg", (new LiteralExpression_(2.02))->upcast());
  llvm::Value* value = this->code_generator.generate(arg);

  EXPECT_TRUE(value->use_empty());
}

TEST_F(CodeGeneratorTest, TestGenerateLastArgumentListSize) {
  Argument arg = new Argument_("myint", (new LiteralExpression_(10))->upcast());
  LastArgumentList arg_list = new LastArgumentList_(arg);
  std::vector<llvm::Value*> values = this->code_generator.generate(arg_list);

  EXPECT_EQ(values.size(), 1);
}

TEST_F(CodeGeneratorTest, TestGeneratePairArgumentListSize) {
  Argument arg = new Argument_("myint", (new LiteralExpression_(10))->upcast());
  LastArgumentList arg_list = new LastArgumentList_(arg);
  Argument arg2 =
      new Argument_("arg", (new LiteralExpression_(2.02))->upcast());
  PairArgumentList pair_list = new PairArgumentList_(arg2, arg_list->upcast());

  std::vector<llvm::Value*> values = this->code_generator.generate(pair_list);

  EXPECT_EQ(values.size(), 2);
}

TEST_F(CodeGeneratorTest, TestGenerateEmptyArgumentList) {
  EmptyArgumentList arg_list = new EmptyArgumentList_();
  std::vector<llvm::Value*> values = this->code_generator.generate(arg_list);

  EXPECT_EQ(values.size(), 0);
}