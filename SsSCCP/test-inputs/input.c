int foo (void) {
  int a = 1;
  int b = 2;
  int c = 3;
  int e = 100;
  int d = 4;
  if (a == 1) {
    b = b + d;
    b *= 2;
  } else {
    e = 6;
    if (e > 3) {
      a = 5;
      if (e > c) {
        a = 6 + d;
      } else {
        e = 999 + d;
        if (c > a) {
          a = b + d;
        }
      }
    }
  }
  a = a + c + d;

  return 0;
}


int main(void) {
  foo();
  return 0;
}
