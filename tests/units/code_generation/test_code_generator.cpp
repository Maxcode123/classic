#include <gtest/gtest.h>

#include "../../../src/code_generation/code_generator.h"
#include "../../../src/code_generation/symbol_table.h"
#include "../../../src/syntax/ast/nodes.h"

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
    llvm::FunctionType* ft =
        llvm::FunctionType::get(llvm::Type::getInt64Ty(*this->context), false);
    llvm::Function* f = llvm::Function::Create(
        ft, llvm::Function::ExternalLinkage, "test_func", this->module);
    return f;
  }

  void create_basic_block() {
    llvm::BasicBlock* bb =
        llvm::BasicBlock::Create(*this->context, "test_entry", this->test_func);
    this->ir_builder->SetInsertPoint(bb);
  }

  void allocate(llvm::Type* t, std::string name) {
    this->proxy.update(name, this->ir_builder->CreateAlloca(t));
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
  this->allocate(llvm::Type::getInt64Ty(*this->context), "int_var");

  llvm::AllocaInst* value = (llvm::AllocaInst*)this->code_generator.generate(
      new VariableExpression_("int_var"));

  EXPECT_EQ(value->getAllocatedType(), llvm::Type::getInt64Ty(*this->context));
}

TEST_F(CodeGeneratorTest, TestGenerateVariableExpressionDupl) {
  this->create_basic_block();
  this->allocate(llvm::Type::getFloatTy(*this->context), "dupl_var");

  llvm::AllocaInst* value = (llvm::AllocaInst*)this->code_generator.generate(
      new VariableExpression_("dupl_var"));

  EXPECT_EQ(value->getAllocatedType(), llvm::Type::getFloatTy(*this->context));
}

TEST_F(CodeGeneratorTest, TestGenerateVariableExpressionAnef) {
  this->create_basic_block();
  this->allocate(llvm::Type::getInt1Ty(*this->context), "anef_var");

  llvm::AllocaInst* value = (llvm::AllocaInst*)this->code_generator.generate(
      new VariableExpression_("anef_var"));

  EXPECT_EQ(value->getAllocatedType(), llvm::Type::getInt1Ty(*this->context));
}

TEST_F(CodeGeneratorTest, TestGenerateFunctionCallTestFunc) {
  FunctionCallExpression exp = new FunctionCallExpression_(
      "test_func", (new EmptyArgumentList_())->upcast());

  this->create_basic_block();

  llvm::CallInst* call = (llvm::CallInst*)this->code_generator.generate(exp);
  EXPECT_EQ(call->getCalledFunction(), this->test_func);
  EXPECT_TRUE(call->getAttributes().isEmpty());
}

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