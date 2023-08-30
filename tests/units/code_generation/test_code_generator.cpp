#include <gtest/gtest.h>

#include "../../../src/code_generation/code_generator.h"
#include "../../../src/code_generation/symbol_table.h"
#include "../../../src/syntax/ast/nodes.h"

class CodeGeneratorTest : public testing::Test {
 protected:
  llvm::LLVMContext* context;
  llvm::Module* module;
  llvm::IRBuilder<>* ir_builder;
  //
  SymbolTable symbol_table;

  CodeGenerator code_generator;

  void SetUp() override {
    this->context = new llvm::LLVMContext();
    this->module = new llvm::Module("test_code_generator.cpp", *this->context);
    this->ir_builder = new llvm::IRBuilder<>(*this->context);

    this->symbol_table = new SymbolTable_();

    this->code_generator =
        CodeGenerator(SymbolTableProxy(this->symbol_table), this->context,
                      this->module, this->ir_builder);
  }
};

TEST_F(CodeGeneratorTest, TestGenerateLiteralExpressionInt) {
  LiteralExpression exp = new LiteralExpression_(10);
  llvm::ConstantInt* value =
      (llvm::ConstantInt*)this->code_generator.generate(exp);

  EXPECT_EQ(value->getSExtValue(), 10);
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
  std::vector<llvm::Value*> values =
      this->code_generator.generate(arg_list->upcast());

  EXPECT_EQ(values.size(), 0);
}