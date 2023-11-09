/*
This file is used for debugging.
Run `make ir` to compile it to LLVM assembly. You can then inspect the produced
test.ll file to see how C-statements get matched to LLVM assembly statements.
*/

int my_func() {
  int a = 1;
  double b = 2;
  int c = a + b;
  return c;
}

void arg_func(int a, void* smt) {
  a++;
  smt++;
}

int main() {
  int a = my_func();
  int b = my_func();
  return b;
}