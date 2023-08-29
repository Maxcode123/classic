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
#include "../code_generation/validator.h"
#include "../syntax/ast/nodes.h"

extern std::unique_ptr<llvm::LLVMContext> the_context;
extern std::unique_ptr<llvm::Module> the_module;
extern std::unique_ptr<llvm::IRBuilder<>> the_builder;

class CodeGenerator {
 public:
  CodeGenerator(SymbolTableProxy p) { proxy = p; }

  llvm::Function* generate(Function func);

  llvm::Type* map_type(ClassicType t);

  std::vector<llvm::Type*> generate(ParamList param_list);
  std::vector<llvm::Type*> generate(PairParamList param_list);
  std::vector<llvm::Type*> generate(LastParamList param_list);
  llvm::Type* generate(Param param);

  void generate_and_insert(Statement stm);
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
  llvm::Value* generate(Argument arg);

 private:
  SymbolTableProxy proxy;
};