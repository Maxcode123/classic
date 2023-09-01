#pragma once

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

#include <vector>

#include "../code_generation/symbol_table.h"
#include "../syntax/ast/nodes.h"
#include "errors.h"

#define INTEGER_BITSIZE 64

class CodeGenerator {
 public:
  CodeGenerator(SymbolTableProxy p, llvm::LLVMContext* ctx, llvm::Module* mod,
                llvm::IRBuilder<>* bld) {
    proxy = p;
    context = ctx;
    module = mod;
    ir_builder = bld;
  }
  CodeGenerator() {}

  std::vector<llvm::Function*> generate(FunctionList func_list);
  std::vector<llvm::Function*> generate(PairFunctionList func_list);
  std::vector<llvm::Function*> generate(LastFunctionList func_list);
  llvm::Function* generate(Function func);

  llvm::Type* map_type(ClassicType t);

  std::vector<llvm::Type*> generate(ParamList param_list);
  std::vector<llvm::Type*> generate(PairParamList param_list);
  std::vector<llvm::Type*> generate(LastParamList param_list);
  std::vector<llvm::Type*> generate(EmptyParamList param_list);
  llvm::Type* generate(Param param);

  void generate(Statement stm);
  llvm::Value* generate(AssignStatement stm);
  llvm::Value* generate(ExpressionStatement stm);
  llvm::Value* generate(ExodusStatement stm);

  llvm::Value* generate(Expression exp);
  llvm::Value* generate(LiteralExpression exp);
  llvm::Value* generate(VariableExpression exp);
  llvm::Value* generate(FunctionCallExpression exp);
  llvm::Value* generate(BinaryOperationExpression exp);
  llvm::Value* generate(ParenthesesExpression exp);

  std::vector<llvm::Value*> generate(ArgumentList arg_list);
  std::vector<llvm::Value*> generate(PairArgumentList arg_list);
  std::vector<llvm::Value*> generate(LastArgumentList arg_list);
  std::vector<llvm::Value*> generate(EmptyArgumentList arg_list);
  llvm::Value* generate(Argument arg);

 private:
  SymbolTableProxy proxy;
  llvm::LLVMContext* context;
  llvm::Module* module;
  llvm::IRBuilder<>* ir_builder;

  llvm::AllocaInst* allocate(ClassicType t, std::string name);
  llvm::StoreInst* store(llvm::Value* value, llvm::Value* mem_ptr);
};