#include "code_generator.h"

llvm::Value* CodeGenerator::generate(ExpressionStatement stm) {
  return this->generate(stm->expression);
}

llvm::Value* CodeGenerator::generate(ExodusStatement stm) {
  return this->generate(stm->expression);
}

llvm::Value* CodeGenerator::generate(Expression exp) {
  switch (exp->type) {
    case LITERAL_EXPRESSION:
      return this->generate(exp->downcast<LiteralExpression>());
    case VARIABLE_EXPRESSION:
      return this->generate(exp->downcast<VariableExpression>());
    case FUNCTION_CALL_EXPRESSION:
      return this->generate(exp->downcast<FunctionCallExpression>());
    case BINARY_OPERATION_EXPRESSION:
      return this->generate(exp->downcast<BinaryOperationExpression>());
    case PARENTHESES_EXPRESSION:
      return this->generate(exp->downcast<ParenthesesExpression>());
    default:
      return nullptr;
  }
}

llvm::Value* CodeGenerator::generate(LiteralExpression exp) {
  switch (exp->type) {
    case BUILTIN_INT_TYPE:
      return llvm::ConstantInt::get(*the_context,
                                    llvm::APSInt(std::stoi(exp->literal_str)));
    case BUILTIN_DUPL_TYPE:
      return llvm::ConstantFP::get(*the_context,
                                   llvm::APFloat(std::stof(exp->literal_str)));
    case BUILTIN_ANEF_TYPE:
      return llvm::ConstantInt::get(*the_context, llvm::APInt(1, 0, false));
  };
  return nullptr;
}

llvm::Value* CodeGenerator::generate(VariableExpression exp) {
  return this->proxy.get(exp->var_name);
}

llvm::Value* CodeGenerator::generate(FunctionCallExpression exp) {
  llvm::Function* func = the_module->getFunction(exp->function_name);
  std::vector<llvm::Value*> args = this->generate(exp->argument_list);
  return the_builder->CreateCall(func, args, "calltmp");
}

llvm::Value* CodeGenerator::generate(BinaryOperationExpression exp) {
  llvm::Value* left = this->generate(exp->left_expression);
  llvm::Value* right = this->generate(exp->right_expression);
  switch (exp->binary_operator) {
    case BINARY_MINUS:
      return the_builder->CreateFSub(left, right, "subtmp");
    case BINARY_PLUS:
      return the_builder->CreateFAdd(left, right, "addtmp");
    case BINARY_DIV:
      return the_builder->CreateFDiv(left, right, "divtmp");
    case BINARY_TIMES:
      return the_builder->CreateFMul(left, right, "multmp");
    default:
      return nullptr;
  }
}

llvm::Value* CodeGenerator::generate(ParenthesesExpression exp) {
  return this->generate(exp->expression);
}

std::vector<llvm::Value*> CodeGenerator::generate(ArgumentList arg_list) {
  if (arg_list->type == LAST_ARG_LIST)
    return this->generate(arg_list->downcast<LastArgumentList>());
  else
    return this->generate(arg_list->downcast<PairArgumentList>());
}

std::vector<llvm::Value*> CodeGenerator::generate(PairArgumentList arg_list) {
  std::vector<llvm::Value*> args{this->generate(arg_list->argument)};
  std::vector<llvm::Value*> next = this->generate(arg_list->next);
  args.insert(args.begin(), next.begin(), next.end());
  return args;
}

std::vector<llvm::Value*> CodeGenerator::generate(LastArgumentList arg_list) {
  std::vector<llvm::Value*> args{this->generate(arg_list->argument)};
  return args;
}

llvm::Value* CodeGenerator::generate(Argument arg) {
  return this->generate(arg->expression);
}
