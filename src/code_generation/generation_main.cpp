#include "../syntax/ast/builder.h"
#include "../syntax/ast/errors.h"
#include "../syntax/parse.h"
#include "../utils/error_handler.h"
#include "code_generator.h"

AST ast;
ASTBuilder builder;
ASTStackProxy proxy;
ErrorHandler<ASTBuildError> handler;

llvm::Module *llvm_module;

void update_ast() { ast.update(proxy.top()); }

CodeGenerator create_generator() {
  llvm::LLVMContext *context = new llvm::LLVMContext();
  llvm_module = new llvm::Module("IR code generation main", *context);
  llvm::IRBuilder<> *ir_builder = new llvm::IRBuilder<>(*context);
  SymbolTableProxy proxy = SymbolTableProxy(new SymbolTable_());

  return CodeGenerator(proxy, context, llvm_module, ir_builder);
}

int main(int argc, char **argv) {
  ast = AST();
  proxy = ASTStackProxy(new ASTStack_());
  builder = ASTBuilder(proxy);
  handler = ErrorHandler<ASTBuildError>();
  pos = 0;

  CodeGenerator generator = create_generator();

  if (argc != 2) {
    fprintf(stderr, "usage: ./a.out filename\n");
    exit(1);
  }
  // yydebug = 1;
  yyin = fopen(argv[1], "r");

  if (yyparse() == 0) {
    printf("\nParsing successfull\n");
  }

  FunctionList f = ast.get<FunctionList>();
  generator.generate(f);

  llvm_module->print(llvm::errs(), nullptr);
  return 0;
}
