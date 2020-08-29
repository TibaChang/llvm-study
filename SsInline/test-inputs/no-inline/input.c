
int foo(int a, int b) {
  return a * 2 + b;
}

int bar(int c, int d) {
  return c - d;
}

int main(int argc, char *argv[]) {
  int a = 123;
  int b = 456;
  int c = 1000;
  int d = 999;
  int main_ret = 0;

  main_ret += foo(a, b);
  main_ret += bar(c, d);

  return main_ret;
}
