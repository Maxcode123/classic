#include "code_generator.h"

std::vector<llvm::Function*> CodeGenerator::generate(FunctionList func_list) {
  if (func_list->type == PAIR_FUNCTION_LIST)
    return this->generate(func_list->downcast<PairFunctionList>());
  else
    return this->generate(func_list->downcast<LastFunctionList>());
}

std::vector<llvm::Function*> CodeGenerator::generate(
    PairFunctionList func_list) {
  std::vector<llvm::Function*> funcs{this->generate(func_list->function)};
  std::vector<llvm::Function*> next = this->generate(func_list->next);
  funcs.insert(funcs.begin(), next.begin(), next.end());
  return funcs;
}

std::vector<llvm::Function*> CodeGenerator::generate(
    LastFunctionList func_list) {
  return std::vector<llvm::Function*>{this->generate(func_list->function)};
}

llvm::Function* CodeGenerator::generate(Function func) {
  std::vector<llvm::Type*> params = this->generate(func->param_list);
  llvm::Type* ret = this->map_type(func->return_type);

  llvm::FunctionType* ft;
  if (params.size() > 0)
    ft = llvm::FunctionType::get(ret, params, false);
  else
    ft = llvm::FunctionType::get(ret, false);

  llvm::Function* f = llvm::Function::Create(
      ft, llvm::Function::ExternalLinkage, func->name, this->module);

  int i = 0;
  for (auto it = ParamListIterator(func->param_list),
            end = ParamListIterator(nullptr);
       it != end; ++it, ++i) {
    f->getArg(i)->setName(it->name);
  }

  llvm::BasicBlock* bb = llvm::BasicBlock::Create(*this->context, "entry", f);
  this->ir_builder->SetInsertPoint(bb);

  this->proxy.clear();
  for (auto& arg : f->args()) {
    this->proxy.update(
        arg.getName().str(),
        this->ir_builder->CreateAlloca(arg.getType(), nullptr,
                                       func->name + ":" + arg.getName()));
  }

  this->generate(func->body->statement);
  this->ir_builder->CreateRet(this->generate(func->body->exodus));
  return f;
}

llvm::Type* CodeGenerator::map_type(ClassicType t) {
  if (t == nullptr) {
    throw NoClassicTypeError(
        "cannot map ClassicType to llvm::Type, given type is null. ");
  }
  if (t->type == BUILTIN_TYPE) {
    ClassicBuiltinType bt = t->downcast<ClassicBuiltinType>();
    switch (bt->type) {
      case classic_builtin_types::INT:
        return llvm::Type::getInt64Ty(*this->context);
      case classic_builtin_types::DUPL:
        return llvm::Type::getDoubleTy(*this->context);
      case classic_builtin_types::ANEF:
        return llvm::Type::getInt1Ty(*this->context);
      default:
        throw UnknownBuiltinType(
            "cannot map ClassicType to llvm::Type. Builtin type is not one of "
            "INT, DUPL, ANEF. ");
    }
  }
  throw CustomTypesNotSupported(
      "cannot map ClassicType to llvm::Type. Custom types are not supported. ");
}

std::vector<llvm::Type*> CodeGenerator::generate(ParamList param_list) {
  if (param_list->type == LAST_PARAM_LIST)
    return this->generate(param_list->downcast<LastParamList>());
  else if (param_list->type == PAIR_PARAM_LIST)
    return this->generate(param_list->downcast<PairParamList>());
  return this->generate(param_list->downcast<EmptyParamList>());
}

std::vector<llvm::Type*> CodeGenerator::generate(PairParamList param_list) {
  std::vector<llvm::Type*> params{this->generate(param_list->param)};
  std::vector<llvm::Type*> next = this->generate(param_list->next);
  params.insert(params.begin(), next.begin(), next.end());
  return params;
}

std::vector<llvm::Type*> CodeGenerator::generate(LastParamList param_list) {
  return std::vector<llvm::Type*>{this->generate(param_list->param)};
}

std::vector<llvm::Type*> CodeGenerator::generate(EmptyParamList param_list) {
  return std::vector<llvm::Type*>{};
}

llvm::Type* CodeGenerator::generate(Param param) {
  return this->map_type(param->classic_type);
}

void CodeGenerator::generate(Statement stm) {
  switch (stm->type) {
    case COMPOUND_STATEMENT: {
      CompoundStatement cmp_stm = stm->downcast<CompoundStatement>();
      this->generate(cmp_stm->first_statement);
      this->generate(cmp_stm->second_statement);
      break;
    }
    case ASSIGN_STATEMENT: {
      AssignStatement asn_stm = stm->downcast<AssignStatement>();
      llvm::Value* mem_ptr = this->proxy.get(asn_stm->lhs_id);
      if (mem_ptr == nullptr) {
        mem_ptr = this->allocate(asn_stm->rhs_expression->classic_type,
                                 asn_stm->lhs_id);
      }
      this->store(this->generate(asn_stm->rhs_expression), mem_ptr);
      break;
    }
    case EXPRESSION_STATEMENT: {
      this->generate(stm->downcast<ExpressionStatement>()->expression);
      break;
    }
    case EMPTY_STATEMENT:
      return;
    default:
      break;
  }
}

llvm::Value* CodeGenerator::generate(AssignStatement stm) {
  return this->generate(stm->rhs_expression);
}

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
  switch (exp->classic_builtin_type) {
    case classic_builtin_types::INT:
      return llvm::ConstantInt::get(
          *this->context,
          llvm::APInt(INTEGER_BITSIZE, std::stoi(exp->literal_str), false));
    case classic_builtin_types::DUPL:
      return llvm::ConstantFP::get(*this->context,
                                   llvm::APFloat(std::stof(exp->literal_str)));
    case classic_builtin_types::ANEF:
      return llvm::ConstantInt::get(*this->context, llvm::APInt(1, 0, false));
  };
  throw UnknownBuiltinType(
      "cannot generate code for LiteralExpression. Builtin type is not one of "
      "INT, DUPL. ANEF. ");
}

llvm::Value* CodeGenerator::generate(VariableExpression exp) {
  return this->proxy.get(exp->var_name);
}

llvm::Value* CodeGenerator::generate(FunctionCallExpression exp) {
  llvm::Function* func = this->module->getFunction(exp->function_name);
  std::vector<llvm::Value*> args = this->generate(exp->argument_list);
  return this->ir_builder->CreateCall(func, args, "calltmp");
}

llvm::Value* CodeGenerator::generate(BinaryOperationExpression exp) {
  llvm::Value* left = this->generate(exp->left_expression);
  llvm::Value* right = this->generate(exp->right_expression);
  if (exp->classic_builtin_type == classic_builtin_types::INT) {
    switch (exp->binary_operator) {
      case BINARY_MINUS:
        return this->ir_builder->CreateSub(left, right, "subtmp");
      case BINARY_PLUS:
        return this->ir_builder->CreateAdd(left, right, "addtmp");
      case BINARY_DIV:
        return this->ir_builder->CreateSDiv(left, right, "divtmp");
      case BINARY_TIMES:
        return this->ir_builder->CreateMul(left, right, "multmp");
      default:
        break;
    }
  } else if (exp->classic_builtin_type == classic_builtin_types::DUPL) {
    switch (exp->binary_operator) {
      case BINARY_MINUS:
        return this->ir_builder->CreateFSub(left, right, "subtmp");
      case BINARY_PLUS:
        return this->ir_builder->CreateFAdd(left, right, "addtmp");
      case BINARY_DIV:
        return this->ir_builder->CreateFDiv(left, right, "divtmp");
      case BINARY_TIMES:
        return this->ir_builder->CreateFMul(left, right, "multmp");
      default:
        break;
    }
  }
  throw UnknownBuiltinType(
      "cannot generate code for BinaryOperationExpression. Builtin type is "
      "neither INT nor DUPL. ");
}

llvm::Value* CodeGenerator::generate(ParenthesesExpression exp) {
  return this->generate(exp->expression);
}

std::vector<llvm::Value*> CodeGenerator::generate(ArgumentList arg_list) {
  if (arg_list->type == LAST_ARG_LIST)
    return this->generate(arg_list->downcast<LastArgumentList>());
  else if (arg_list->type == PAIR_ARG_LIST)
    return this->generate(arg_list->downcast<PairArgumentList>());
  return this->generate(arg_list->downcast<EmptyArgumentList>());
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

std::vector<llvm::Value*> CodeGenerator::generate(EmptyArgumentList arg_list) {
  return std::vector<llvm::Value*>{};
}

llvm::Value* CodeGenerator::generate(Argument arg) {
  return this->generate(arg->expression);
}

llvm::AllocaInst* CodeGenerator::allocate(ClassicType t, std::string name) {
  llvm::AllocaInst* mem_ptr =
      this->ir_builder->CreateAlloca(this->map_type(t), nullptr, name);
  this->proxy.update(name, mem_ptr);
  return mem_ptr;
}

llvm::StoreInst* CodeGenerator::store(llvm::Value* value,
                                      llvm::Value* mem_ptr) {
  return this->ir_builder->CreateStore(value, mem_ptr);
}