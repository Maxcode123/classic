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
#include "symbol_table.h"

class Script {
 public:
  Script() {}

  llvm::LLVMContext* context;
  llvm::Module* module;
  llvm::IRBuilder<>* ir_builder;

  SymbolTableProxy proxy;

  llvm::Function* func;

  void init() {
    this->context = new llvm::LLVMContext();
    this->module = new llvm::Module("IR builder", *this->context);
    this->ir_builder = new llvm::IRBuilder<>(*this->context);

    this->proxy = SymbolTableProxy(new SymbolTable_());

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

  llvm::Value* load(llvm::Type* t, llvm::Value* mem_ptr, std::string name) {
    return this->ir_builder->CreateLoad(t, mem_ptr, name);
  }
};

int main() {
  Script s = Script();
  s.init();

  s.create_basic_block();

  llvm::AllocaInst* mem_ptr1 =
      s.allocate(llvm::Type::getInt64Ty(*s.context), "var_1");

  s.store(llvm::ConstantInt::get(*s.context, llvm::APInt(64, 10, false)),
          mem_ptr1, "");

  llvm::AllocaInst* mem_ptr2 =
      s.allocate(llvm::Type::getInt64Ty(*s.context), "var_2");

  s.store(llvm::ConstantInt::get(*s.context, llvm::APInt(64, 15, false)),
          mem_ptr2, "");

  llvm::Value* res = s.ir_builder->CreateAdd((llvm::Value*)mem_ptr1,
                                             (llvm::Value*)mem_ptr2, "addtmp");

  s.module->print(llvm::errs(), nullptr);

  return 0;
}