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

int main(int argc, char **argv) {
  ast = AST();
  proxy = ASTStackProxy(new ASTStack_());
  builder = ASTBuilder(proxy);
  handler = ErrorHandler<ASTBuildError>();
  pos = 0;

  if (argc != 2) {
    fprintf(stderr, "usage: ./a.out filename\n");
    exit(1);
  }
  // yydebug = 1;
  yyin = fopen(argv[1], "r");

  if (yyparse() == 0) {
    printf("\nParsing successfull\n");
  }
  return 0;
}
