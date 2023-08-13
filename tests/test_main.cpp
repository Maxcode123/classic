#include "../src/syntax/ast/builder.h"
#include "../src/syntax/ast/nodes.h"

int main(int argc, char **argv) {
  AST ast = AST();
  ASTStack stack = new ASTStack_();
  ASTBuilder builder = ASTBuilder(ASTStackProxy(stack));

  stack->push(new ExpressionStatement_(new VariableExpression_("myvar")));
  stack->push(new ExpressionStatement_(new LiteralExpression_(10)));

  CompoundStatement stm = builder.build_compound_statement();

  std::cout << stm->first_statement->cls() << "\n";

  return 0;
}