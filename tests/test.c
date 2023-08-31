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