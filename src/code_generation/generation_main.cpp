#include "../syntax/ast/builder.h"
#include "../syntax/ast/errors.h"
#include "../syntax/parse.h"
#include "../utils/error_handler.h"
#include "code_generator.h"

AST ast;
ASTBuilder builder;
ASTStackProxy proxy;
ErrorHandler<ASTBuildError> handler;

std::unique_ptr<llvm::LLVMContext> the_context;
std::unique_ptr<llvm::Module> the_module;
std::unique_ptr<llvm::IRBuilder<>> the_builder;

void update_ast() { ast.update(proxy.top()); }

void init_module() {
  the_context = std::make_unique<llvm::LLVMContext>();
  the_module =
      std::make_unique<llvm::Module>("generation_main.cpp", *the_context);
  the_builder = std::make_unique<llvm::IRBuilder<>>(*the_context);
}

int main(int argc, char **argv) {
  ast = AST();
  proxy = ASTStackProxy(new ASTStack_());
  builder = ASTBuilder(proxy);
  handler = ErrorHandler<ASTBuildError>();
  pos = 0;
  init_module();

  if (argc != 2) {
    fprintf(stderr, "usage: ./a.out filename\n");
    exit(1);
  }
  // yydebug = 1;
  yyin = fopen(argv[1], "r");

  if (yyparse() == 0) {
    printf("\nParsing successfull\n");
  }

  LastFunctionList f = ast.get<LastFunctionList>();

  the_module->print(llvm::errs(), nullptr);
  return 0;
}
