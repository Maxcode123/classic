#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>

#include "../syntax/ast/nodes.h"
#include "code_generator.h"
#include "symbol_table.h"

class Script {
 public:
  Script() {}

  llvm::LLVMContext* context;
  llvm::Module* module;
  llvm::IRBuilder<>* ir_builder;

  SymbolTableProxy proxy;

  CodeGenerator code_generator;

  llvm::Function* func;

  void init() {
    this->context = new llvm::LLVMContext();
    this->module = new llvm::Module("IR builder", *this->context);
    this->ir_builder = new llvm::IRBuilder<>(*this->context);

    this->proxy = SymbolTableProxy(new SymbolTable_());

    this->code_generator = CodeGenerator(this->proxy, this->context,
                                         this->module, this->ir_builder);

    this->func = this->get_func();
  }

  llvm::Function* get_func() {
    llvm::FunctionType* ft =
        llvm::FunctionType::get(llvm::Type::getInt64Ty(*this->context), false);
    llvm::Function* f = llvm::Function::Create(
        ft, llvm::Function::ExternalLinkage, "func", this->module);
    return f;
  }

  void create_basic_block() {
    llvm::BasicBlock* bb =
        llvm::BasicBlock::Create(*this->context, "entry", this->func);
    this->ir_builder->SetInsertPoint(bb);
  }

  llvm::AllocaInst* allocate(llvm::Type* t, std::string name) {
    llvm::AllocaInst* mem_ptr =
        this->ir_builder->CreateAlloca(t, nullptr, name);
    this->proxy.update(name, mem_ptr);
    return mem_ptr;
  }

  llvm::StoreInst* store(llvm::Value* v, llvm::AllocaInst* mem_ptr) {
    llvm::StoreInst* store = this->ir_builder->CreateStore(v, mem_ptr);
    return store;
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

int main() {
  Script s = Script();
  s.init();

  // oper: int add3(int c) { c = c + 1; c = c + 2; exodus c; }
  ExodusStatement exodus =
      new ExodusStatement_((new VariableExpression_("c"))->upcast());

  BinaryOperationExpression binop1 =
      s.build_binary_operation_exp("c", BINARY_PLUS, 1);
  binop1->set_builtin_type(classic_builtin_types::INT);
  Statement stm1 = (new AssignStatement_("c", binop1->upcast()))->upcast();

  BinaryOperationExpression binop2 =
      s.build_binary_operation_exp("c", BINARY_PLUS, 2);
  binop2->set_builtin_type(classic_builtin_types::INT);
  Statement stm2 = (new AssignStatement_("c", binop2->upcast()))->upcast();

  CompoundStatement cmp_stm = new CompoundStatement_(stm1, stm2);

  FunctionBody body = new FunctionBody_(cmp_stm->upcast(), exodus);

  Param c = new Param_(s.create_builtin_int()->upcast(), "c");
  ParamList param_list = (new LastParamList_(c))->upcast();

  Function add3 =
      new Function_("add3", s.create_builtin_int()->upcast(), param_list, body);

  s.code_generator.generate(add3);
  s.module->print(llvm::errs(), nullptr);

  return 0;
}