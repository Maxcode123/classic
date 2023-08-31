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

  llvm::StoreInst* store(llvm::Value* v, llvm::AllocaInst* mem_ptr) {
    return this->ir_builder->CreateStore(v, mem_ptr);
  }

  void allocate_and_store(llvm::Value* value, llvm::Type* t, std::string name) {
    llvm::AllocaInst* mem_ptr = this->allocate(t, name);
    this->store(value, mem_ptr);
  }

  llvm::Value* load(llvm::Type* t, llvm::Value* mem_ptr, std::string name) {
    return this->ir_builder->CreateLoad(t, mem_ptr, name);
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

  ClassicBuiltinType create_builtin_int() {
    return new ClassicBuiltinType_(classic_builtin_types::INT);
  }

  ClassicBuiltinType create_builtin_dupl() {
    return new ClassicBuiltinType_(classic_builtin_types::DUPL);
  }

  template <class T>
  BinaryOperationExpression build_binary_operation_exp(T left,
                                                       BinaryOperator op,
                                                       T right) {
    return new BinaryOperationExpression_(
        op, (new LiteralExpression_(left))->upcast(),
        (new LiteralExpression_(right))->upcast());
  }

  BinaryOperationExpression build_binary_operation_exp(const char* left,
                                                       BinaryOperator op,
                                                       const char* right) {
    return new BinaryOperationExpression_(
        op, (new VariableExpression_(left))->upcast(),
        (new VariableExpression_(right))->upcast());
  }

  template <class T>
  BinaryOperationExpression build_binary_operation_exp(T left,
                                                       BinaryOperator op,
                                                       const char* right) {
    return new BinaryOperationExpression_(
        op, (new LiteralExpression_(left))->upcast(),
        (new VariableExpression_(right))->upcast());
  }

  template <class T>
  BinaryOperationExpression build_binary_operation_exp(const char* left,
                                                       BinaryOperator op,
                                                       T right) {
    return new BinaryOperationExpression_(
        op, (new VariableExpression_(left))->upcast(),
        (new LiteralExpression_(right))->upcast());
  }
};

TEST_F(CodeGeneratorTest, TestGenerateFunctionEmptyBody) {
  // oper: dupl myfunc(dupl a, dupl b) { exodus a + b; }

  BinaryOperationExpression binop =
      this->build_binary_operation_exp("a", BINARY_PLUS, "b");
  binop->set_builtin_type(classic_builtin_types::DUPL);
  ExodusStatement exodus = new ExodusStatement_(binop->upcast());

  FunctionBody body =
      new FunctionBody_((new EmptyStatement_())->upcast(), exodus);

  Param a = new Param_(this->create_builtin_dupl()->upcast(), "a");
  Param b = new Param_(this->create_builtin_dupl()->upcast(), "b");
  ParamList param_list =
      (new PairParamList_((new LastParamList_(b))->upcast(), a))->upcast();

  Function myfunc = new Function_(
      "myfunc", this->create_builtin_dupl()->upcast(), param_list, body);

  this->code_generator.generate(myfunc);

  llvm::BasicBlock& myfunc_entry = this->module->getFunction("myfunc")->front();

  auto itr = myfunc_entry.begin();
  EXPECT_EQ((itr++)->getOpcode(), llvm::Instruction::Alloca);
  EXPECT_EQ((itr++)->getOpcode(), llvm::Instruction::Alloca);
  EXPECT_EQ((itr++)->getOpcode(), llvm::Instruction::FAdd);
  EXPECT_EQ((itr++)->getOpcode(), llvm::Instruction::Ret);
  EXPECT_EQ(itr, myfunc_entry.end());
}

TEST_F(CodeGeneratorTest, TestGenerateFunctionThrowsUnknownBuiltinTypeError) {
  // oper: dupl myfunc(dupl a, dupl b) { exodus a + b; }
  BinaryOperationExpression binop =
      this->build_binary_operation_exp("a", BINARY_PLUS, "b");
  // builtin type of binop is not set
  ExodusStatement exodus = new ExodusStatement_(binop->upcast());

  FunctionBody body =
      new FunctionBody_((new EmptyStatement_())->upcast(), exodus);

  Param a = new Param_(this->create_builtin_dupl()->upcast(), "a");
  Param b = new Param_(this->create_builtin_dupl()->upcast(), "b");
  ParamList param_list =
      (new PairParamList_((new LastParamList_(b))->upcast(), a))->upcast();

  Function myfunc = new Function_(
      "myfunc", this->create_builtin_dupl()->upcast(), param_list, body);

  EXPECT_THROW(this->code_generator.generate(myfunc), UnknownBuiltinType);
}

TEST_F(CodeGeneratorTest, TestGenerateFunctionCompoundStatementBody) {
  // oper: int myfunc(int c) { c = c + 1; c = c + 2; exodus c; }
}

TEST_F(CodeGeneratorTest, TestGeneratePairParamListSize) {
  Param p = new Param_(this->create_builtin_int()->upcast(), "myint");
  LastParamList param_list = new LastParamList_(p);
  PairParamList pair_list = new PairParamList_(param_list->upcast(), p);

  std::vector<llvm ::Type*> types = this->code_generator.generate(pair_list);
  EXPECT_EQ(types.size(), 2);
}

TEST_F(CodeGeneratorTest, TestGenerateLastParamListSize) {
  Param p = new Param_(this->create_builtin_int()->upcast(), "myint");
  LastParamList param_list = new LastParamList_(p);

  std::vector<llvm::Type*> types = this->code_generator.generate(param_list);
  EXPECT_EQ(types.size(), 1);
}

TEST_F(CodeGeneratorTest, TestGenerateEmptyParamListSize) {
  std::vector<llvm::Type*> types =
      this->code_generator.generate(new EmptyParamList_());
  EXPECT_EQ(types.size(), 0);
}

TEST_F(CodeGeneratorTest, TestGenerateParam) {
  Param p = new Param_(this->create_builtin_int()->upcast(), "myint");

  llvm::Type* t = this->code_generator.generate(p);
  EXPECT_TRUE(t->isIntegerTy());
  EXPECT_EQ(t->getIntegerBitWidth(), INTEGER_BITSIZE);
}

TEST_F(CodeGeneratorTest, TestGenerateStatementCompoundStatementInstructions) {
  // a = 1;
  Expression exp1 = (new LiteralExpression_(1))->upcast();
  exp1->set_classic_type(this->create_builtin_int()->upcast());
  Statement stm1 = (new AssignStatement_("a", exp1))->upcast();

  // b = a + 2;
  BinaryOperationExpression binop =
      this->build_binary_operation_exp("a", BINARY_PLUS, 2);
  binop->set_builtin_type(classic_builtin_types::INT);
  Expression exp2 = binop->upcast();
  exp2->set_classic_type(this->create_builtin_int()->upcast());
  Statement stm2 = (new AssignStatement_("b", exp2))->upcast();

  CompoundStatement cmp_stm = new CompoundStatement_(stm1, stm2);

  this->create_basic_block();
  llvm::BasicBlock& entry = this->test_func->front();

  this->code_generator.generate(cmp_stm->upcast());

  /*
  %a = alloca i64, align 8
  store i64 1, ptr %a, align 4
  %b = alloca i64, align 8
  %addtmp = add ptr %a, i64 2
  store ptr %addtmp, ptr %b, align 8
  */
  auto itr = entry.begin();
  EXPECT_EQ((itr++)->getOpcode(), llvm::Instruction::Alloca);
  EXPECT_EQ((itr++)->getOpcode(), llvm::Instruction::Store);
  EXPECT_EQ((itr++)->getOpcode(), llvm::Instruction::Alloca);
  EXPECT_EQ((itr++)->getOpcode(), llvm::Instruction::Add);
  EXPECT_EQ((itr++)->getOpcode(), llvm::Instruction::Store);
  EXPECT_EQ(itr, entry.end());
}

TEST_F(CodeGeneratorTest, TestGenerateStatementAssignStatementInstructions) {
  Expression exp = (new LiteralExpression_(8))->upcast();
  exp->set_classic_type(this->create_builtin_int()->upcast());
  AssignStatement stm = new AssignStatement_("a", exp);

  this->create_basic_block();
  llvm::BasicBlock& entry = this->test_func->front();

  this->code_generator.generate(stm->upcast());

  EXPECT_EQ(entry.front().getName(), "a");
  EXPECT_EQ(entry.front().getOpcode(), llvm::Instruction::Alloca);
  EXPECT_EQ(entry.back().getOpcode(), llvm::InsertElementInst::Store);
}

TEST_F(CodeGeneratorTest, TestGenerateStatementThrowsNoClassicTypeError) {
  AssignStatement stm =
      new AssignStatement_("b", (new LiteralExpression_(2))->upcast());
  // expression in assign statement has no classic_type

  this->create_basic_block();

  EXPECT_THROW(this->code_generator.generate(stm->upcast()),
               NoClassicTypeError);
}

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

TEST_F(CodeGeneratorTest, TestGenerateLiteralExpressionStr) {
  // this test should break once string are supported. kept here to remind of
  // changes that should be made to generate(LiteralExpresion) when strings
  // are introduced.
  LiteralExpression exp = new LiteralExpression_(classic_builtin_types::STR);
  EXPECT_THROW(this->code_generator.generate(exp), UnknownBuiltinType);
}

TEST_F(CodeGeneratorTest, TestGenerateVariableExpressionInt64) {
  this->create_basic_block();
  this->allocate_and_store(this->create_int64(5), INT64_TYPE, "int_var");

  llvm::AllocaInst* value = llvm::dyn_cast<llvm::AllocaInst>(
      this->code_generator.generate(new VariableExpression_("int_var")));

  EXPECT_NE(value, nullptr);
  EXPECT_TRUE(value->getAllocatedType()->isIntegerTy());
  EXPECT_EQ(value->getAllocatedType()->getIntegerBitWidth(), INTEGER_BITSIZE);
}

TEST_F(CodeGeneratorTest, TestGenerateVariableExpressionDupl) {
  this->create_basic_block();
  this->allocate_and_store(this->create_float(2.32), FLOAT_TYPE, "dupl_var");

  llvm::AllocaInst* value = llvm::dyn_cast<llvm::AllocaInst>(
      this->code_generator.generate(new VariableExpression_("dupl_var")));

  EXPECT_NE(value, nullptr);
  EXPECT_TRUE(value->getAllocatedType()->isFloatTy());
}

TEST_F(CodeGeneratorTest, TestGenerateVariableExpressionAnef) {
  this->create_basic_block();
  this->allocate_and_store(this->create_int1(0), INT1_TYPE, "anef_var");

  llvm::AllocaInst* value = llvm::dyn_cast<llvm::AllocaInst>(
      this->code_generator.generate(new VariableExpression_("anef_var")));

  EXPECT_NE(value, nullptr);
  EXPECT_TRUE(value->getAllocatedType()->isIntegerTy());
  EXPECT_EQ(value->getAllocatedType()->getIntegerBitWidth(), 1);
}

TEST_F(CodeGeneratorTest, TestGenerateFunctionCallTestFunc) {
  FunctionCallExpression exp = new FunctionCallExpression_(
      "test_func", (new EmptyArgumentList_())->upcast());

  this->create_basic_block();

  llvm::CallInst* call =
      llvm::dyn_cast<llvm::CallInst>(this->code_generator.generate(exp));
  EXPECT_NE(call, nullptr);
  EXPECT_EQ(call->getCalledFunction(), this->test_func);
  EXPECT_TRUE(call->getAttributes().isEmpty());
}

TEST_F(CodeGeneratorTest, TestGenerateBinaryOperationExpressionSub) {
  // {literal} - {literal} produces an llvm::Constant
  BinaryOperationExpression exp =
      this->build_binary_operation_exp(1, BINARY_MINUS, 10);
  exp->set_builtin_type(classic_builtin_types::INT);

  llvm::ConstantInt* value =
      llvm::dyn_cast<llvm::ConstantInt>(this->code_generator.generate(exp));
  EXPECT_NE(value, nullptr);
  EXPECT_EQ(value->getSExtValue(), 1 - 10);
}

TEST_F(CodeGeneratorTest, TestGenerateBinaryOperationExpressionFSub) {
  // {literal} - {literal} produces an llvm::Constant
  BinaryOperationExpression exp =
      this->build_binary_operation_exp(10.25, BINARY_MINUS, 1.20);
  exp->set_builtin_type(classic_builtin_types::DUPL);

  llvm::ConstantFP* value =
      llvm::dyn_cast<llvm::ConstantFP>(this->code_generator.generate(exp));
  EXPECT_NE(value, nullptr);
  EXPECT_FLOAT_EQ(value->getValueAPF().convertToFloat(), 10.25 - 1.20);
}

TEST_F(CodeGeneratorTest, TestGenerateBinaryOperationAdd) {
  // {literal} + {literal} produces an llvm::Constant
  BinaryOperationExpression exp =
      this->build_binary_operation_exp(56, BINARY_PLUS, 44);
  exp->set_builtin_type(classic_builtin_types::INT);

  llvm::ConstantInt* value =
      llvm::dyn_cast<llvm::ConstantInt>(this->code_generator.generate(exp));
  EXPECT_NE(value, nullptr);
  EXPECT_EQ(value->getSExtValue(), 56 + 44);
}

TEST_F(CodeGeneratorTest, TestGenerateBinaryOperationFAdd) {
  // {literal} + {literal} produces an llvm::Constant
  BinaryOperationExpression exp =
      this->build_binary_operation_exp(.25, BINARY_PLUS, .21);
  exp->set_builtin_type(classic_builtin_types::DUPL);

  llvm::ConstantFP* value =
      llvm::dyn_cast<llvm::ConstantFP>(this->code_generator.generate(exp));
  EXPECT_NE(value, nullptr);
  EXPECT_FLOAT_EQ(value->getValueAPF().convertToFloat(), .25 + .21);
}

TEST_F(CodeGeneratorTest, TestGenerateBinaryOperationSDiv) {
  // {literal} / {literal} produces an llvm::Constant
  BinaryOperationExpression exp =
      this->build_binary_operation_exp(100, BINARY_DIV, 3);
  exp->set_builtin_type(classic_builtin_types::INT);

  llvm::ConstantInt* value =
      llvm::dyn_cast<llvm::ConstantInt>(this->code_generator.generate(exp));
  EXPECT_NE(value, nullptr);
  EXPECT_EQ(value->getSExtValue(), 33);
}

TEST_F(CodeGeneratorTest, TestGenerateBinaryOperationFDiv) {
  // {literal} / {literal} produces an llvm::Constant
  BinaryOperationExpression exp =
      this->build_binary_operation_exp(100.3, BINARY_DIV, 3.0);
  exp->set_builtin_type(classic_builtin_types::DUPL);

  llvm::ConstantFP* value =
      llvm::dyn_cast<llvm::ConstantFP>(this->code_generator.generate(exp));
  EXPECT_NE(value, nullptr);
  EXPECT_FLOAT_EQ(value->getValueAPF().convertToFloat(), 100.3 / 3.0);
}

TEST_F(CodeGeneratorTest, TestGenerateBinaryOperationMul) {
  // {literal} * {literal} produces an llvm::Constant
  BinaryOperationExpression exp =
      this->build_binary_operation_exp(55, BINARY_TIMES, 6);
  exp->set_builtin_type(classic_builtin_types::INT);

  llvm::ConstantInt* value =
      llvm::dyn_cast<llvm::ConstantInt>(this->code_generator.generate(exp));
  EXPECT_NE(value, nullptr);
  EXPECT_EQ(value->getSExtValue(), 55 * 6);
}

TEST_F(CodeGeneratorTest, TestGenerateBinaryOperationFMul) {
  // {literal} * {literal} produces an llvm::Constant
  BinaryOperationExpression exp =
      this->build_binary_operation_exp(12.36, BINARY_TIMES, 9.0194);
  exp->set_builtin_type(classic_builtin_types::DUPL);

  llvm::ConstantFP* value =
      llvm::dyn_cast<llvm::ConstantFP>(this->code_generator.generate(exp));
  EXPECT_NE(value, nullptr);
  EXPECT_FLOAT_EQ(value->getValueAPF().convertToFloat(), 12.36 * 9.0194);
}

TEST_F(CodeGeneratorTest, TestGenerateBinaryOperationAddVariableExpressions) {
  // {var} + {var} produces an llvm::Instruction
  this->create_basic_block();
  this->allocate_and_store(this->create_int64(10), INT64_TYPE, "var_1");
  this->allocate_and_store(this->create_int64(20), INT64_TYPE, "var_2");

  BinaryOperationExpression exp =
      this->build_binary_operation_exp("var_1", BINARY_PLUS, "var_2");
  exp->set_builtin_type(classic_builtin_types::INT);

  int instructions = this->test_func->getInstructionCount();
  llvm::BinaryOperator* value =
      llvm::dyn_cast<llvm::BinaryOperator>(this->code_generator.generate(exp));
  EXPECT_NE(value, nullptr);
  EXPECT_EQ(this->test_func->getInstructionCount(), instructions + 1);
}

TEST_F(CodeGeneratorTest, TestGenerateBinaryOperationAddLiteralToVariable) {
  // {var} + {literal} produces an llvm::Instruction
  this->create_basic_block();
  this->allocate_and_store(this->create_int64(23), INT64_TYPE, "var_1");

  BinaryOperationExpression exp =
      this->build_binary_operation_exp("var_1", BINARY_PLUS, 5);
  exp->set_builtin_type(classic_builtin_types::INT);

  int instructions = this->test_func->getInstructionCount();
  llvm::BinaryOperator* value =
      llvm::dyn_cast<llvm::BinaryOperator>(this->code_generator.generate(exp));
  EXPECT_NE(value, nullptr);
  EXPECT_EQ(this->test_func->getInstructionCount(), instructions + 1);
}

TEST_F(CodeGeneratorTest, TestGenerateBinaryOperationAddFunctionCalls) {
  // {func_call} + {func_call} produces two call instructions and one add
  // instruction
  FunctionCallExpression left = new FunctionCallExpression_(
      "test_func", (new EmptyArgumentList_())->upcast());
  FunctionCallExpression right = new FunctionCallExpression_(
      "test_func", (new EmptyArgumentList_())->upcast());
  BinaryOperationExpression exp = new BinaryOperationExpression_(
      BINARY_PLUS, left->upcast(), right->upcast());
  exp->set_builtin_type(classic_builtin_types::INT);

  this->create_basic_block();

  int instructions = this->test_func->getInstructionCount();
  llvm::BinaryOperator* value =
      llvm::dyn_cast<llvm::BinaryOperator>(this->code_generator.generate(exp));
  EXPECT_NE(value, nullptr);
  EXPECT_EQ(this->test_func->getInstructionCount(), instructions + 3);
}

TEST_F(CodeGeneratorTest, TestGenerateBinaryOperationThrows) {
  BinaryOperationExpression exp =
      this->build_binary_operation_exp(100.3, BINARY_DIV, 3.0);
  // exp builtin type is not set

  EXPECT_THROW(this->code_generator.generate(exp), UnknownBuiltinType);
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
