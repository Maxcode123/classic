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
};

int main() {
  Script s = Script();
  s.init();

  s.create_basic_block();

  Expression exp1 = (new LiteralExpression_(1))->upcast();
  exp1->set_classic_type(
      new ClassicType_(new ClassicBuiltinType_(classic_builtin_types::INT)));
  Statement stm1 = (new AssignStatement_("a", exp1))->upcast();

  // b = a + 2;
  BinaryOperationExpression binop = (new BinaryOperationExpression_(
      BINARY_PLUS, (new VariableExpression_("a"))->upcast(),
      (new LiteralExpression_(2))->upcast()));
  binop->set_builtin_type(classic_builtin_types::INT);
  Expression exp2 = binop->upcast();
  exp2->set_classic_type(
      new ClassicType_(new ClassicBuiltinType_(classic_builtin_types::INT)));
  Statement stm2 = (new AssignStatement_("b", exp2))->upcast();

  CompoundStatement cmp_stm = new CompoundStatement_(stm1, stm2);

  s.code_generator.generate(cmp_stm->upcast());

  s.module->print(llvm::errs(), nullptr);

  return 0;
}